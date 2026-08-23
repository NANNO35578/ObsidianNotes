[gyan.dev有编译好的](https://www.gyan.dev/ffmpeg/builds/)
# 编译非常慢
![[painfullySlow.png]]

---
# MinGW64环境安装

前往[官网](https://www.msys2.org/)下载安装

# 必要工具安装
1. 打开msys2安装目录下`mingw64.exe` 输入以下系列命令
```bash
# 首先是gcc 
pacman -S mingw-w64-x86_64-toolchain

# make
pacman -S make

# 汇编器
pacman -S mingw-w64-x86_64-yasm
pacman -S mingw-w64-x86_64-nasm

# 其他工具
pacman -S mingw-w64-x86_64-libx264 
pacman -S mingw-w64-x86_64-libvpx
```


- `./configure: line 1638: cmp: command not found`：
- 若出现这条错误消息表明在配置过程中，`cmp` 命令未找到。`cmp` 是一个用于比较两个文件的命令，通常在 Unix/Linux 系统中可用。
- 使用`pacman -S coreutils`安装

> [!bug] 仍然显示`./configure: line 1638: cmp: command not found`
> 若Windows安装了`git`, 可将git的安装目录下如
> - `E:\Git\usr\bin\cmp.exe`
> - `C:\Program Files\Microsoft Visual Studio\2022\Professional\Common7\IDE\CommonExtensions\Microsoft\TeamFoundation\Team Explorer\Git\usr\bin\cmp.exe`
> - `E:\AI_Painting\sd-webui-aki-v4.4\git\usr\bin\cmp.exe`
> 等复制到`C:\msys64\mingw64\bin\` 使用

# 配置编译开发库文件 ~~`.lib`~~
- github下载zip文件，解压，mingw64.exe更改工作目录到解压文件夹，输入命令
```bash
./configure --prefix=./ffmpeg-4.4.6 --enable-shared --enable-static
```
- 成功配置后输入`make` 编译
- `make install` 将输出文件安装到 `./ffmpeg-4.4.6`
- 编译完成即可使用

> ~~仍然需要`find . -name "*.lib"`找到相关lib文件~~
> ~~`find . -name "*.dll"` 找到链接库~~

> [!warning] 这种编译的是MinGW版，生成`.def`, `.dll.a`
> MSVC版才是`.lib`


> [!error] 实际上`Qt`用的就是MinGW版本

---
# `MSVC` 编译

- 将上述配置 [[#必要工具安装]] 完成后
- win + q 搜索x64 Native Tools Command Prompt for VS 2022
- 打开后输入以下程序，检查是否已安装，若未安装，安装后添加到path
```bash
cl 
perl -v
nasm -v
```

- 打开msys的bash，我的在`C:\msys64\usr\bin\bash.exe`
- 运行后将是一个空bash，可在~/.bashrc中添加额外path来支持部分工具使用
```bash
export PATH=$PATH:/c/msys64/mingw64/bin  
export PATH=$PATH:/c/Users/YINHA/AppData/Local/bin/NASM  
export PATH=$PATH:/c/msys64/usr/bin  
export PATH=$PATH:/c/msys64/mingw64/bin
```

- 然后输入`source ~/.bashrc` 立即应用
- 此时cd到FFmpeg目录输入如下命令后即可编译
```bash
./configure --toolchain=msvc --arch=x86_64 --enable-shared --prefix=./ffmpeg_7.0.2
make
make install
```

```bash
./configure --enable-gpl \
            --toolchain=msvc \
            --enable-postproc \
            --enable-static \
            --enable-w32threads \
            --enable-libx264 \
            --disable-shared \   # disable 生成静态库 enable生成lib或dll.a 同时需要拷贝dll
            --disable-debug \
            --prefix=./ffmpeg_7.0.2
```

##### 输入`./configure ...`完成后应该看到
![[ffmpegconfigure.png]]


##### 输入`make` 应该看到
![[ffmpegmake.png]]

##### 执行`make install` 应当有如下目录在`ffmpeg_7.0.2`
```bash
usr24@Thatchy:/mnt/d/Desktop/ffmpeg-7.0.2/ffmpeg_7.0.2$ tree -L 2
.
├── bin
│   ├── avcodec-61.dll
│   ├── avcodec.lib
│   ├── avdevice-61.dll
│   ├── avdevice.lib
│   ├── avfilter-10.dll
│   ├── avfilter.lib
│   ├── avformat-61.dll
│   ├── avformat.lib
│   ├── avutil-59.dll
│   ├── avutil.lib
│   ├── ffmpeg.exe
│   ├── ffprobe.exe
│   ├── swresample-5.dll
│   ├── swresample.lib
│   ├── swscale-8.dll
│   └── swscale.lib
├── include
│   ├── libavcodec
│   ├── libavdevice
│   ├── libavfilter
│   ├── libavformat
│   ├── libavutil
│   ├── libswresample
│   └── libswscale
├── lib
│   ├── avcodec-61.def
│   ├── avdevice-61.def
│   ├── avfilter-10.def
│   ├── avformat-61.def
│   ├── avutil-59.def
│   ├── pkgconfig
│   ├── swresample-5.def
│   └── swscale-8.def
└── share
    ├── ffmpeg
    └── man

15 directories, 23 files
```


---
# 启用`H264`编解码
安装x264
1. 从[官网](https://code.videolan.org/videolan/x264)搜索下载`x264`源代码
2. `./configure --enable-shared --prefix=./x264` 此时需要`nasm`
![[x264conf.png]]
3. `make`
4. `make install`
5. 将生成的x264文件夹拷贝到固定位置,将其中bin文件夹的路径添加到path
6. 终端输入`x264 --version`正确输出 即编译完成 可继续ffmpeg编译

---

[`.def.a`文件是什么](https://stackoverflow.com/questions/185326/whats-a-dll-a-file)
## `.def` 文件（Module Definition File）
- 作用：`.def` 文件是“模块定义文件”，主要用于 Windows 平台的 DLL（动态链接库）开发
- 内容：它列出了 DLL 中导出的函数、变量等符号
- 用途：
	- 让链接器知道哪些符号可以被外部程序调用
	- 生成导入库（`.lib` 或 `.dll.a`）
	- 有时用于手动控制导出符号，或解决符号冲突
- `FFmpeg` 例子：比如 `avcodec-58.def`，里面会有 `avcodec_open2`、`avcodec_send_packet` 等导出函数名。
## `.dll.a` 文件（Import Library）
- 作用：`.dll.a` 是“导入库”，是 Windows 下 MinGW/GCC 工具链的格式（MSVC 下是 `.lib`）
- 用途：
	- 不是实际的 DLL，而是一个“中介”
	- 编译你的程序时，链接器用 `.dll.a` 文件来解析外部符号（比如 `avcodec_open2`），并在运行时让你的程序去加载真正的 DLL（如 avcodec-58.dll）
	- 你只需要在链接参数里加 `-lavcodec`，GCC 会自动找到 `libavcodec.dll.a`
- 注意：运行时还需要对应的 DLL 文件（如 avcodec-58.dll）在 PATH 下


| 文件类型 | 主要作用 | 何时用到 | 备注 |
|---------------|------------------------------|-------------------------|---------------------------|
| .def | 控制 DLL 导出符号 | 编译 DLL 时 | 通常自动生成 |
| .dll.a | 连接时解析 DLL 符号 | 编译/链接你的程序时 | MinGW/GCC 下的导入库 |
| .lib | 连接时解析 DLL 符号 | MSVC 下编译/链接时 | MSVC 下的导入库 |
| .dll | 实际的动态链接库 | 程序运行时 | 需放在 PATH 或同目录下 |
#####  实际开发流程
编译 `FFmpeg` → 得到 `.dll`（运行时用）、`.dll.a`（开发时链接用）、`.def`（辅助生成导入库）
开发你的程序 → 链接时用 `-lavcodec`，GCC 会用 `.dll.a`，运行时需要 `.dll`
不用直接操作 `.def` 文件，除非你要自定义导出符号

[微软官方 .def 文件说明](https://learn.microsoft.com/en-us/cpp/build/reference/module-definition-dot-def-files)
[MinGW 导入库说明](http://www.mingw.org/wiki/sampleDLL)