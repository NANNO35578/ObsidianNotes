- C语言源码, 可在GitHub找到很多
- [加密原理在线分析](https://sha256algorithm.com/) 
- [GitHub 已有的SHA256加密程序](https://github.com/System-Glitch/SHA256)

----
### SHA256模块


模块文件结构:
```bash
.
├── README.md
├── SHA256.cpp
├── SHA256.hpp
└── sha256.pri

1 directory, 4 files
```


##### 1. `sha256.pri` 文件内容

```bash
HEADERS += \
    $$PWD/SHA256.hpp

SOURCES +=     $$PWD/SHA256.cpp

INCLUDEPATH += ./
```

- Qt项目`.pro`文件加入以下内容即可导入成功. 
```bash
INCLUDEPATH +=  ./sha256/

include(./sha256/sha256.pri)
```

----

##### 2. 头文件. 源文件

头文件 `SHA256.hpp` 和源文件 `SHA256.cpp`

- 使用了 C++17 的 `std::array`、`std::string` 等标准库类型 
- 提供类 `SHA256`，支持流式 `update()`，最后通过 `digest()` 拿到二进制摘要，也可通过 `toHexString()` 转为十六进制字符串
- 同时提供了一个便捷的 free-function `sha256()`

---

```cpp title:"SHA256.hpp 头文件"
// SHA256.hpp
#pragma once
#include <array>
#include <cstdint>
#include <cstddef>
#include <string>

class SHA256 {
public:
    static constexpr std::size_t HashBytes = 32;

    SHA256();
    void reset();

    // 更新消息
    void update(const uint8_t* data, std::size_t len);
    void update(const std::string& s);

    // 获取二进制摘要，调用后内部状态会被填充为最终状态（如果要复用，请 reset()）
    std::array<uint8_t, HashBytes> digest();

    // 将二进制摘要转为十六进制字符串
    static std::string toHexString(const std::array<uint8_t, HashBytes>&);

private:
    void transform(const uint8_t block[64]);

private:
    uint8_t  data_[64];
    uint32_t datalen_;
    uint64_t bitlen_;
    uint32_t state_[8];

    static const uint32_t k_[64];
};
  
// 便捷函数：直接对 std::string 进行 SHA256，返回 64 字符十六进制
std::string sha256(const std::string& s);
```

```cpp title:"SHA256.cpp 源文件"
// SHA256.cpp
#include "SHA256.hpp"
#include <cstring>
#include <sstream>
#include <iomanip>

//
// 常量和宏
//
#define ROTLEFT(x,n)  (((x) << (n)) | ((x) >> (32-(n))))
#define ROTRIGHT(x,n) (((x) >> (n)) | ((x) << (32-(n))))
#define CH(x,y,z)     (((x) & (y)) ^ (~(x) & (z)))
#define MAJ(x,y,z)    (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
#define EP0(x)        (ROTRIGHT(x,2) ^ ROTRIGHT(x,13) ^ ROTRIGHT(x,22))
#define EP1(x)        (ROTRIGHT(x,6) ^ ROTRIGHT(x,11) ^ ROTRIGHT(x,25))
#define SIG0(x)       (ROTRIGHT(x,7) ^ ROTRIGHT(x,18) ^ ((x) >> 3))
#define SIG1(x)       (ROTRIGHT(x,17)^ ROTRIGHT(x,19) ^ ((x) >> 10))

// SHA256 常量表
const uint32_t SHA256::k_[64] = {
    0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
    0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
    0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
    0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
    0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
    0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
    0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
    0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
};

SHA256::SHA256() {
    reset();
}

void SHA256::reset() {
    datalen_ = 0;
    bitlen_ = 0;
    state_[0] = 0x6a09e667;
    state_[1] = 0xbb67ae85;
    state_[2] = 0x3c6ef372;
    state_[3] = 0xa54ff53a;
    state_[4] = 0x510e527f;
    state_[5] = 0x9b05688c;
    state_[6] = 0x1f83d9ab;
    state_[7] = 0x5be0cd19;
}

void SHA256::update(const uint8_t* data, std::size_t len) {
    for (std::size_t i = 0; i < len; ++i) {
        data_[datalen_++] = data[i];
        if (datalen_ == 64) {
            transform(data_);
            bitlen_ += 512;
            datalen_ = 0;
        }
    }
}

void SHA256::update(const std::string& s) {
    update(reinterpret_cast<const uint8_t*>(s.data()), s.size());
}

std::array<uint8_t, SHA256::HashBytes> SHA256::digest() {
    std::array<uint8_t, HashBytes> hash{};
    uint32_t i = datalen_;

    // 填充 0x80，然后 0x00
    if (datalen_ < 56) {
        data_[i++] = 0x80;
        while (i < 56) data_[i++] = 0x00;
    } else {
        data_[i++] = 0x80;
        while (i < 64) data_[i++] = 0x00;
        transform(data_);
        std::memset(data_, 0, 56);
    }

    // 加入消息长度（big-endian）
    bitlen_ += uint64_t(datalen_) * 8;
    data_[63] = uint8_t(bitlen_);
    data_[62] = uint8_t(bitlen_ >> 8);
    data_[61] = uint8_t(bitlen_ >> 16);
    data_[60] = uint8_t(bitlen_ >> 24);
    data_[59] = uint8_t(bitlen_ >> 32);
    data_[58] = uint8_t(bitlen_ >> 40);
    data_[57] = uint8_t(bitlen_ >> 48);
    data_[56] = uint8_t(bitlen_ >> 56);
    transform(data_);

    // 生成最终 hash（big-endian 输出）
    for (i = 0; i < 4; ++i) {
        hash[i      ] = uint8_t((state_[0] >> (24 - i * 8)) & 0xFF);
        hash[i + 4  ] = uint8_t((state_[1] >> (24 - i * 8)) & 0xFF);
        hash[i + 8  ] = uint8_t((state_[2] >> (24 - i * 8)) & 0xFF);
        hash[i + 12 ] = uint8_t((state_[3] >> (24 - i * 8)) & 0xFF);
        hash[i + 16 ] = uint8_t((state_[4] >> (24 - i * 8)) & 0xFF);
        hash[i + 20 ] = uint8_t((state_[5] >> (24 - i * 8)) & 0xFF);
        hash[i + 24 ] = uint8_t((state_[6] >> (24 - i * 8)) & 0xFF);
        hash[i + 28 ] = uint8_t((state_[7] >> (24 - i * 8)) & 0xFF);
    }

    return hash;
}

void SHA256::transform(const uint8_t block[64]) {
    uint32_t m[64];
    uint32_t a,b,c,d,e,f,g,h, t1, t2;

    // 前 16 个字按 big-endian 解码
    for (uint32_t i = 0, j = 0; i < 16; ++i, j += 4) {
        m[i] = (uint32_t(block[j]) << 24)
             | (uint32_t(block[j+1]) << 16)
             | (uint32_t(block[j+2]) <<  8)
             | (uint32_t(block[j+3]));
    }
    // 后 48 个字扩展
    for (uint32_t i = 16; i < 64; ++i) {
        m[i] = SIG1(m[i-2]) + m[i-7] + SIG0(m[i-15]) + m[i-16];
    }

    a = state_[0];
    b = state_[1];
    c = state_[2];
    d = state_[3];
    e = state_[4];
    f = state_[5];
    g = state_[6];
    h = state_[7];

    for (uint32_t i = 0; i < 64; ++i) {
        t1 = h + EP1(e) + CH(e,f,g) + k_[i] + m[i];
        t2 = EP0(a) + MAJ(a,b,c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    state_[0] += a;
    state_[1] += b;
    state_[2] += c;
    state_[3] += d;
    state_[4] += e;
    state_[5] += f;
    state_[6] += g;
    state_[7] += h;
}

std::string SHA256::toHexString(const std::array<uint8_t, HashBytes>& d) {
    std::ostringstream oss;
    oss << std::hex << std::setfill('0');
    for (auto byte : d) {
        oss << std::setw(2) << static_cast<int>(byte);
    }
    return oss.str();
}

std::string sha256(const std::string& s) {
    SHA256 ctx;
    ctx.update(s);
    auto d = ctx.digest();
    return SHA256::toHexString(d);
}
```

----
##### 3. Usage示例：

```cpp
#include "SHA256.hpp"
#include <iostream>

int main() {
    std::string msg = "hello world";
    // 二进制摘要
    SHA256 ctx;
    ctx.update(msg);
    auto bin = ctx.digest();
    // 十六进制摘要
    std::cout << "SHA256: " << SHA256::toHexString(bin) << "\n";
    // 或者直接：
    std::cout << "one-liner: " << sha256(msg) << "\n";
    return 0;
}
```

以上就是完整的 C++ 封装，线程安全可在每个线程各自用独立的 `SHA256` 对象


----

# Qt线程获取文件SHA256

下面给出两种在 Qt 中把“文件 SHA256 计算”放到后台线程的方法
- 第一种是基于 QObject + moveToThread 的“Worker”模式
- 第二种是直接继承 QThread 并重写 run()  

----

### 1. Worker+moveToThread 模式（推荐）

```cpp title:"SHA256Worker.h"
#pragma once
#include <QObject>
#include <QString>
#include "SHA256.hpp"    // 你之前的 C++ SHA256 封装

class SHA256Worker : public QObject {
    Q_OBJECT
public:
    explicit SHA256Worker(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    // 主线程通过 QueuedConnection 调用此槽
    void doHash(const QString &filePath);

signals:
    void progress(int percent);        // [0..100]
    void finished(const QString &hash); // 最终 64 字符 hex
    void error(const QString &errMsg);
};
```

```cpp title:"SHA256Worker.cpp"
#include "SHA256Worker.h"
#include <QFile>
#include <QFileInfo>

void SHA256Worker::doHash(const QString &filePath) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error(QStringLiteral("无法打开文件：“%1”").arg(filePath));
        return;
    }

    qint64 fileSize = file.size();
    const qint64 chunkSize = 1024 * 1024; // 1MB 块
    QByteArray buffer;
    buffer.resize(chunkSize);

    SHA256 ctx;
    qint64 processed = 0;

    while (!file.atEnd()) {
        qint64 readBytes = file.read(buffer.data(), chunkSize);
        if (readBytes <= 0) {
            emit error(QStringLiteral("读取文件失败"));
            return;
        }
        // 更新 SHA256
        ctx.update(reinterpret_cast<const uint8_t*>(buffer.constData()), size_t(readBytes));

        processed += readBytes;
        int pct = int(processed * 100 / fileSize);
        emit progress(pct);
    }

    // 完成
    auto digest = ctx.digest();
    QString hex = QString::fromStdString(SHA256::toHexString(digest));
    emit finished(hex);
}
```

##### 在主线程中使用
```cpp
#include "SHA256Worker.h"
#include <QThread>

// …… 比如在你的 MainWindow 构造里：
QThread *thread = new QThread(this);
SHA256Worker *worker = new SHA256Worker;
worker->moveToThread(thread);

// 连接信号
connect(this, &MainWindow::destroyed, thread, &QThread::quit);
connect(thread, &QThread::finished, worker, &QObject::deleteLater);
connect(thread, &QThread::finished, thread, &QObject::deleteLater);

connect(this, &MainWindow::startHash,       // 自定义信号，带 QString
        worker, &SHA256Worker::doHash);
connect(worker, &SHA256Worker::progress,
        this,    &MainWindow::onHashProgress);
connect(worker, &SHA256Worker::finished,
        this,    &MainWindow::onHashFinished);
connect(worker, &SHA256Worker::error,
        this,    &MainWindow::onHashError);

// 启动线程
thread->start();

// 某处触发文件计算
emit startHash(QStringLiteral("/path/to/large.file"));
```

----

### 2. 继承 QThread 重写 run()

如果你习惯直接 subclass QThread，也可以这么做：

```cpp title:"SHA256Worker.h"
#pragma once
#include <QThread>
#include <QString>
#include "SHA256.hpp"

class FileHashThread : public QThread {
    Q_OBJECT
public:
    explicit FileHashThread(const QString &filePath, QObject *parent = nullptr)
        : QThread(parent), m_filePath(filePath) {}

signals:
    void progress(int percent);
    void finished(const QString &hash);
    void error(const QString &errMsg);

protected:
    void run() override;

private:
    QString m_filePath;
};
```

```cpp title:"FileHashThread.cpp"
#include "FileHashThread.h"
#include <QFile>

void FileHashThread::run() {
    QFile file(m_filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        emit error(QStringLiteral("无法打开文件"));
        return;
    }

    qint64 fileSize = file.size();
    const qint64 chunkSize = 1024 * 1024;
    QByteArray buffer; buffer.resize(chunkSize);

    SHA256 ctx;
    qint64 processed = 0;
    while (!file.atEnd()) {
        qint64 n = file.read(buffer.data(), chunkSize);
        if (n <= 0) {
            emit error(QStringLiteral("读取失败"));
            return;
        }
        ctx.update(reinterpret_cast<const uint8_t*>(buffer.constData()), size_t(n));
        processed += n;
        emit progress(int(processed * 100 / fileSize));
    }

    auto d = ctx.digest();
    QString hex = QString::fromStdString(SHA256::toHexString(d));
    emit finished(hex);
}
```

##### 主线程使用
```cpp
auto *thr = new FileHashThread("/path/to/big.file", this);
connect(thr, &FileHashThread::progress, this, &MainWindow::onHashProgress);
connect(thr, &FileHashThread::finished, this, &MainWindow::onHashFinished);
connect(thr, &FileHashThread::error,    this, &MainWindow::onHashError);
// 计算完毕后自动删除线程对象
connect(thr, &FileHashThread::finished, thr, &QObject::deleteLater);
connect(thr, &FileHashThread::error,    thr, &QObject::deleteLater);

thr->start();
```

----

两种方案都能将大文件的 SHA256 计算放到后台线程，避免阻塞 UI 主循环。Worker+moveToThread 模式更符合 Qt 官方推荐的“工作者对象”模型，生命周期管理更灵活，也更容易复用