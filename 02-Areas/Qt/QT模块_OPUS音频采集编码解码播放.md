**Opus 官方源代码（尤其是 tarball 版本）** 默认是基于 **autotools** 构建系统的，通常是通过 `./configure` → `make` → `make install` 这套流程来编译（而 CMake 是社区后期维护的跨平台方案）

在 **Windows (MinGW)** 环境下，如果要用 `./configure` 方式，需要使用 **MSYS2 或 MSYS** 这样的 POSIX 兼容环境


----

##### 1. 从OPUS 网站下载[源码](https://opus-codec.org/downloads/) 
##### 2. 解压, 终端打开. 进入MinGw的MSYS环境
##### 3. 设置编译生成库文件

- 生成路径需要绝对路径
- 这样会引入Stack-Smashing Protector(可解决)
```bash
./configure --prefix=/d/Desktop/opus-1.5.2/opus --enable-shared --disable-static
```

> [!warning] 使用`windeployqt`打包后 需要手动将QT安装目录下`Tools\mingw<version>\bin`中`libssp-0.dll`拷贝到可执行文件所在目录

若最终如图, 则设置成功
![[../../'attachments/makeOPUS.png]]

##### 3. 开始编译

```bash
make -j16
```
- 开启16线程并行编译
- `-jN` 告诉 `make` 同时启动 N 个编译任务，可以利用多核 CPU 的性能. 
- - **N 的选择**：
    - 一般等于或略大于 CPU 逻辑核心数
    - 比如 8 核 16 线程 CPU，可用 `-j16`
- **不加数字（`-j`）**：
    - GNU make 会自动开足够多的任务（有时甚至会开太多）
- **注意内存**：
    - 每个编译任务会消耗内存，如果任务数过高可能导致系统卡顿或 OOM

##### 4. 输出到目录
```bash
make install
```


---

# 创建音频采集编码压缩, 解码播放模块

##### 0. 创建Qt的`.pri`模块
- 可将生成文件夹的`include/opus`文件夹中头文件直接放入`include`中, 在输出目录创建opus.pri文件.

```bash title:"opus.pri"

QT += multimedia

# ┌───────────────────────────────────────────┐
# │                opus.pri                   │
# └───────────────────────────────────────────┘

# 根目录就是当前 pri 所在的目录
OPUS_ROOT = $$PWD

# 1) include 搜索路径
INCLUDEPATH += $$OPUS_ROOT/include

# 2) 链接库目录
LIBS += -L$$OPUS_ROOT/lib -lopus

# （如果你更喜欢直接用 .a 文件，也可写成：）
#   LIBS += $$OPUS_ROOT/lib/libopus.a

# Windows 下还可能需要的系统库
# win32:LIBS += -lws2_32
win32 { # 加入这个原因是编译时引入了, 不然找不到 
    # 把 stack-protector 运行时也拉了进来. (可不引入❌)
    LIBS += -lssp_nonshared -lssp
    LIBS += -lgcc_s
}

# 3) 自动添加 OpusEncoder/OpusDecoder 的源和头
HEADERS += \
    $$OPUS_ROOT/opusencoder.h \
    $$OPUS_ROOT/opusdecoder.h

SOURCES += \
    $$OPUS_ROOT/opusencoder.cpp \
    $$OPUS_ROOT/opusdecoder.cpp

# 4) （可选）如果你想把 include/lib 都装到 pkgconfig 里
# CONFIG += pkgconfig
# PKGCONFIG += $$OPUS_ROOT/lib/pkgconfig/opus
```

Qt + MinGW 下，将 Opus 库引入工程，并利用 QAudioInput 捕获 PCM、通过 Opus 编码，再通过信号/槽传递压缩包，最后在另一端解码并用 QAudioOutput 播放

1. QAudioInput 捕获 PCM → OpusEncoderWrapper 编码 → 发射 `packetReady`
2. OpusDecoderWrapper 收到 `packetReady` → 解码 PCM → QAudioOutput 播放

---

##### 1. `OPUS` 模块目录

模块目录结构：

```
opus/
├── include
│   ├── opus_defines.h
│   ├── opus.h
│   ├── opus_multistream.h
│   ├── opus_projection.h
│   └── opus_types.h
├── lib
│   ├── libopus.a
│   ├── libopus.la
│   └── pkgconfig
│       └── opus.pc
├── opusdecoder.cpp
├── opusdecoder.h
├── opusencoder.cpp
├── opusencoder.h
├── opus.pri
├── README_OPUS1.5.2
└── share
    └── aclocal
        └── opus.m4

6 directories, 15 files
```

在`opus.pri` 文件内容在上文. 在项目 `.pro`文件中加入：

```cmake
# Opus include 路径
INCLUDEPATH += $$PWD/opus/

include(./opus/opus.pri)
```

这样 qmake 就能找到头文件和库文件了

---

##### 2. 音频捕获 → Opus 编码 → 发射信号

`OpusEncoder` QObject，把 `QAudioInput` 捕获到的 PCM frame 编码成 Opus packet，并发出信号

```cpp title:"opusencoder.h 编码发送类头文件"
// opusencoder.h
#pragma once
#include <QObject>
#include <QAudioInput>
#include <QIODevice>
#include "opus.h"

class OpusEncoderWrapper : public QObject {
    Q_OBJECT
public:
    OpusEncoderWrapper(QObject* parent = nullptr);
    ~OpusEncoderWrapper();

    // 启动音频捕获与编码
    void start();
    void stop();

signals:
    // 发射压缩好的 opus 包
    void packetReady(const QByteArray &packet);

private slots:
    void handleReadyRead();

private:
    QAudioInput*     m_audioIn = nullptr;
    QIODevice*       m_io = nullptr;
    OpusEncoder*     m_encoder = nullptr;

    const int        m_sampleRate = 48000;
    const int        m_channels   = 1;
    const int        m_frameSize  = 960;   // 20ms @ 48kHz
    const int        m_maxDataBytes = 4000; // 容量上限
    QByteArray       m_buffer;  // 用来读 PCM
};
```

```cpp title:"opusencoder 类源文件"
// opusencoder.cpp
#include "opusencoder.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>

OpusEncoderWrapper::OpusEncoderWrapper(QObject* parent)
    : QObject(parent),
      m_buffer(m_frameSize * m_channels * sizeof(short), Qt::Uninitialized)
{
    int err;
    m_encoder = opus_encoder_create(m_sampleRate, m_channels, OPUS_APPLICATION_AUDIO, &err);
    if (err != OPUS_OK) {
        qFatal("Opus encoder create failed: %s", opus_strerror(err));
    }
}

OpusEncoderWrapper::~OpusEncoderWrapper() {
    stop();
    if (m_encoder) opus_encoder_destroy(m_encoder);
}

void OpusEncoderWrapper::start() {
    QAudioFormat fmt;
    fmt.setSampleRate(m_sampleRate);
    fmt.setChannelCount(m_channels);
    fmt.setSampleSize(16);
    fmt.setCodec("audio/pcm");
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultInputDevice();
    if (!info.isFormatSupported(fmt)) {
        fmt = info.nearestFormat(fmt);
    }

    m_audioIn = new QAudioInput(info, fmt, this);
    m_io = m_audioIn->start();
    connect(m_io, &QIODevice::readyRead, this, &OpusEncoderWrapper::handleReadyRead);
}

void OpusEncoderWrapper::stop() {
    if (m_audioIn) {
        m_audioIn->stop();
        delete m_audioIn;
        m_audioIn = nullptr;
    }
    m_io = nullptr;
}

void OpusEncoderWrapper::handleReadyRead() {
    while (m_audioIn->bytesReady() >= m_buffer.size()) {
        qint64 len = m_io->read(m_buffer.data(), m_buffer.size());
        if (len != m_buffer.size()) break;

        // PCM 数据指针
        const opus_int16* pcm = reinterpret_cast<const opus_int16*>(m_buffer.constData());
        unsigned char outbuf[4000];
        int nbBytes = opus_encode(m_encoder, pcm, m_frameSize, outbuf, m_maxDataBytes);
        if (nbBytes < 0) {
            qWarning("Opus encoding failed: %s", opus_strerror(nbBytes));
            continue;
        }
        // 发射压缩包
        QByteArray packet(reinterpret_cast<char*>(outbuf), nbBytes);
        emit packetReady(packet);
    }
}
```

---

##### 3. Opus 解码 → 播放

做一个 `OpusDecoder` QObject，接收上面发出的压缩包信号，解码后写入 `QAudioOutput` 播放

```cpp title:"opusdecoder.h 接收解码类头文件"
// opusdecoder.h
#pragma once
#include <QObject>
#include <QAudioOutput>
#include <QIODevice>
#include "opus.h"

class OpusDecoderWrapper : public QObject {
    Q_OBJECT
public:
    OpusDecoderWrapper(QObject* parent = nullptr);
    ~OpusDecoderWrapper();

public slots:
    // 接收压缩包
    void playPacket(const QByteArray &packet);

private:
    QAudioOutput*    m_audioOut = nullptr;
    QIODevice*       m_outIo = nullptr;
    OpusDecoder*     m_decoder = nullptr;

    const int        m_sampleRate = 48000;
    const int        m_channels   = 1;
    const int        m_frameSize  = 960;
    QByteArray       m_pcmBuffer;    // 解码后 PCM
};
```

```cpp title:"opusdecoder 类源文件"
// opusdecoder.cpp
#include "opusdecoder.h"
#include <QAudioFormat>
#include <QAudioDeviceInfo>

OpusDecoderWrapper::OpusDecoderWrapper(QObject* parent)
    : QObject(parent),
      m_pcmBuffer(m_frameSize * m_channels * sizeof(short), Qt::Uninitialized)
{
    int err;
    m_decoder = opus_decoder_create(m_sampleRate, m_channels, &err);
    if (err != OPUS_OK) {
        qFatal("Opus decoder create failed: %s", opus_strerror(err));
    }

    QAudioFormat fmt;
    fmt.setSampleRate(m_sampleRate);
    fmt.setChannelCount(m_channels);
    fmt.setSampleSize(16);
    fmt.setCodec("audio/pcm");
    fmt.setByteOrder(QAudioFormat::LittleEndian);
    fmt.setSampleType(QAudioFormat::SignedInt);

    QAudioDeviceInfo info = QAudioDeviceInfo::defaultOutputDevice();
    if (!info.isFormatSupported(fmt)) {
        fmt = info.nearestFormat(fmt);
    }

    m_audioOut = new QAudioOutput(info, fmt, this);
    m_outIo = m_audioOut->start();
}

OpusDecoderWrapper::~OpusDecoderWrapper() {
    if (m_audioOut) {
        m_audioOut->stop();
        delete m_audioOut;
    }
    if (m_decoder) opus_decoder_destroy(m_decoder);
}

void OpusDecoderWrapper::playPacket(const QByteArray &packet) {
    const unsigned char* data = reinterpret_cast<const unsigned char*>(packet.constData());
    int outSamples = opus_decode(m_decoder, data, packet.size(),
                                 reinterpret_cast<opus_int16*>(m_pcmBuffer.data()),
                                 m_frameSize, 0);
    if (outSamples < 0) {
        qWarning("Opus decode failed: %s", opus_strerror(outSamples));
        return;
    }
    // 写入 QAudioOutput 播放
    m_outIo->write(m_pcmBuffer.constData(), outSamples * m_channels * sizeof(short));
}
```

---

##### 4. 在 `main.cpp` 里连接信号/槽

- 根据项目需要在合适位置定义对象, 绑定信号和槽函数, 需要采集时调用encoder的start, 关闭调用stop.
- decoder会自动播放音频数据

```cpp title:"根据项目需要调用"
#include <QCoreApplication>
#include "opusencoder.h"
#include "opusdecoder.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    OpusEncoderWrapper encoder;
    OpusDecoderWrapper decoder;

    QObject::connect(&encoder, &OpusEncoderWrapper::packetReady,
                     &decoder, &OpusDecoderWrapper::playPacket,
                     Qt::QueuedConnection);

    encoder.start();

    return a.exec();
}
```



----

在使用 `windeployqt` 工具打包 Qt 应用时，如果遇到类似错误：

```
The code execution cannot proceed because libssp-0.dll was not found. Reinstalling the program may fix this problem.
```

这个问题通常与缺失的 `libssp-0.dll` 文件有关。`libssp-0.dll` 是一个与 Stack Smashing Protection (SSP) 相关的运行时库，它通常与一些 C++ 编译器设置相关，尤其是 MinGW 编译器。

### 解决方法：

1. **确保 `libssp-0.dll` 已包含在应用的可执行文件目录中**：  
    使用 `windeployqt` 工具时，它会自动将大多数 Qt 相关的 DLL 文件复制到你的应用程序目录，但是它可能没有包含 `libssp-0.dll`。你可以手动从 MinGW 或 Qt 安装目录中找到该 DLL 文件，然后将其复制到你的应用程序目录。
    
    - 如果你使用的是 **MinGW** 编译器，`libssp-0.dll` 通常位于 MinGW 的 `bin` 文件夹中。例如`{bash}C:\Qt\Tools\mingw<version>\bin`
    - 如果你使用的是 **MSVC** 编译器，这个问题不应该出现，因为 MSVC 不依赖 `libssp-0.dll`
    
    找到 `libssp-0.dll` 后，将它复制到你的应用程序的可执行文件目录，或者将它放入与应用程序相同目录的 `bin` 文件夹下
    
2. **检查 `windeployqt` 是否完全执行**：  
    有时，`windeployqt` 工具可能未能正确包括所有需要的 DLL 文件。你可以尝试手动查找并复制缺失的 DLL 文件。检查以下常见的 Qt 动态库：
    
    - `libeay32.dll` / `ssleay32.dll`（如果使用 OpenSSL）
    - `Qt5Core.dll`, `Qt5Gui.dll`, `Qt5Widgets.dll` 等
	
3. **使用 Visual Studio 重新编译**：  
    如果你是使用 **MSVC 编译器**进行编译，并且仍然遇到此问题，那么可能是由于你没有使用合适的运行时库。可以尝试使用 Visual Studio 的运行时库（比如 `MSVCR` 和 `MSVCP` 系列库）来代替 MinGW。这样可以避免 `libssp-0.dll` 的问题
    
4. **重新安装 MinGW 或 Qt**：  
    如果 `libssp-0.dll` 丢失或损坏，尝试重新安装 Qt 或 MinGW 工具链，确保所有相关的 DLL 文件都被正确安装

### 额外的调试步骤：

- **确认使用的编译器版本**：在 `windeployqt` 打包过程中，确保你使用的 Qt 编译器版本与目标环境兼容。例如，使用 MSVC 编译的 Qt 应用程序需要与 MSVC 运行时库相匹配，使用 MinGW 编译的应用程序则需要 MinGW 运行时
- **检查 Qt 的 QtWebEngine**：如果你的应用程序使用了 QtWebEngine，确保相关的 `libssp-0.dll` 和其他依赖库也被正确部署



----

# 编译不引入Stack-Smashing Protector `[!!bug:我测试无用]`

为了避免MinGW/GCC 编译出来的 `libopus.a` 默认打开了 GCC 的 Stack-Smashing Protector（`-fstack-protector`）和／或 `_FORTIFY_SOURCE`，它会在内部引用诸如
```sqf
__stack_chk_fail  
__stack_chk_guard  
__memcpy_chk  
…  
```

这些符号。要么要在链接时把提供它们的运行时库也加进来（在 MinGW 上是 `libssp` 或者 `libgcc_s`），要么直接把 Opus 重新编译得不要引用这些检查函数

```bash

make clean
CFLAGS="-O3 -fno-stack-protector -U_FORTIFY_SOURCE" \
CXXFLAGS="-O3 -fno-stack-protector -U_FORTIFY_SOURCE" \
./configure --prefix=/d/Desktop/opus-1.5.2/opus \
            --disable-shared \
            --enable-static
make -j16
make install
```
