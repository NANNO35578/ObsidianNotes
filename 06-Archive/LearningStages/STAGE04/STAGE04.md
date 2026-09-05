# STAGE04 —— Linux 与服务器开发笔记索引

> 本目录整理了 Stage_04 下的全部笔记：图片内容已识别并转换为可编辑的 mermaid 图，原图片与原引用保留在 `'attachments/` 中，并按主题补充完整、重命名归档至 `markdowns/` 目录。

## 第一章 进程线程与系统编程基础

- [1.1.正则表达式.md](markdowns/1.1.正则表达式.md)
- [1.2.Linux进程与C_C++开发样例.md](markdowns/1.2.Linux进程与C_C++开发样例.md)
- [1.3.并发应用(多进程拷贝).md](markdowns/1.3.并发应用(多进程拷贝).md)
- [1.4.ICP进程间通信.md](markdowns/1.4.ICP进程间通信.md)
- [1.5.守护进程Daemon.md](markdowns/1.5.守护进程Daemon.md)
- [1.6.pthread线程基础.md](markdowns/1.6.pthread线程基础.md)
- [1.7.线程安全与线程互斥访问.md](markdowns/1.7.线程安全与线程互斥访问.md)
- [1.8.信号Signal.md](markdowns/1.8.信号Signal.md)

## 第二章 服务器开发

- [2.1.SOCKET套接字编程.md](markdowns/2.1.SOCKET套接字编程.md)
- [2.2.单机服务器开发.md](markdowns/2.2.单机服务器开发.md)
- [2.3.多进程服务器开发.md](markdowns/2.3.多进程服务器开发.md)
- [2.4.多线程服务器开发.md](markdowns/2.4.多线程服务器开发.md)
- [2.5.IO多路复用与三种模型对比.md](markdowns/2.5.IO多路复用与三种模型对比.md)
- [2.6.Epoll线程池与负载均衡.md](markdowns/2.6.Epoll线程池与负载均衡.md)

## 整理说明

- **图片处理**：`'attachments/` 中 9 张 jpg 笔记截图与 11 张 png 流程图全部识别，关键图示均已在对应笔记中以 **mermaid** 重绘（可编辑），原图片文件保留、原笔记中的图片引用保留。
- **原始素材归档**：15 个原始 md 文件保留在 `markdowns/_source/` 中，未改动。
- **主题补充**：在原笔记 + 图片识别 + 需求知识点之外，补充了正则表达式语法体系、进程状态机、fork 版本演进、IPC 方式对比、信号生命周期、SIGCHLD 回收、线程池设计、Reactor 模型、零拷贝对比等未涉及内容；每个主题均附 C/C++ 可编译样例代码。
- **命名规则**：`编号.主题`（示例：`1.1.正则表达式`），第一章为系统编程基础，第二章为服务器开发。
