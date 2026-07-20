https://www.processon.com/view/57202fe0e4b005d0701bdc27

https://www.cnblogs.com/zhaoyixiang/p/14692753.html

https://zhuanlan.zhihu.com/p/437541721



|        模型        |                                        机制                                        |                                特性                                 |
| :--------------: | :------------------------------------------------------------------------------: | :---------------------------------------------------------------: |
|     select模型     |                         通过select函数来管理I/O，可以确定一个或多个套接字的状态                         |            该模型的优势是程序能够在单个线程内同时处理多个套接字连接，避免了阻塞模式下的线程膨胀             |
| WSAAsyncSelect模型 | WSAAsyncSelect函数把socket设为非阻塞模式，并为socket绑定一个窗口句柄，依靠Windows的消息驱动机制，通过窗口进行消息接收、事件处理 |         该模型最突出的特点是与Windows的消息驱动机制融合在一起，使得开发带GUI界面的网络程序更简单         |
| WSAEventSelect模型 |     该模型与WSAAsyncSelect模型类似，允许应用程序在一个或多个socket上接收基于事件的网络通知，不过该模型是经由事件对象句柄通知的      | 该模型简单易用，也不需要窗口环境，缺点是最多等待64个事件对象的限制，当socket连接数量增加时，必须创建多个线程来处理I/O  |
|     重叠I/O模型      |                    该模型引入了重叠数据结构，允许应用程序使用重叠结构一次投递一个或多个异步I/O请求                     |       该模型使用Winsock 2.0库的API，如：WSASend、WSARecv等，真正做到了“异步处理”        |
|      IOCP模型      |              IOCP模型通过socket绑定完成端口，在socket上投递事件，工作线程在完成端口上轮询接收、处理事件               | IOCP充分利用内核对象的调度，只使用少量的几个线程来处理所有网络通信，消除了无谓的线程上下文切换，最大限度地提高了网络通信的性能 |

https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports

看吧 官方的


https://www.cnblogs.com/coder2012/p/3185715.html

----

下面给出一个封装好的基于 IOCP 的 TCP 服务端类 `TCPServer`，它在接收到完整报文（先 4 字节长度头，再报文体）时，会把数据拷贝到一个 `TaskData` 结构，并交给用户传入的无锁线程池 `LFThreadPool::submit()`。同时，`TCPServer` 提供了一个线程池中调用的发送接口 `send(clientId, data, len)`，用户可以在任务处理完成后调用这个接口异步发送数据。

```cpp title:"TCPServer.h"
// TCPServer.h
#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <windows.h>
#include <unordered_map>
#include <mutex>
#include <cstdio>
#include <cstdlib>

// 用户的任务结构
struct TaskData {
    const char* data;
    int          len;
    unsigned long from;
};

// 前向声明
class LFThreadPool;

class TCPServer {
public:
    // port: 监听端口
    // workerThreads: IOCP 工作线程数
    // pool: 用户传入的线程池引用
    TCPServer(unsigned short port, int workerThreads, LFThreadPool& pool);
    ~TCPServer();

    // 启动并进入 accept 循环（阻塞）
    bool run();

    // 在用户线程池里处理完业务后，如需发数据调用此接口
    // clientId 即 TaskData.from
    void send(unsigned long clientId, const char* data, int len);

private:
    // 每次一个 IO 操作的上下文
    struct PER_IO {
        OVERLAPPED   overlapped;
        OP_TYPE      op_type;
        SOCKET       sock;
        WSABUF       wbuf[2];
        UINT32       body_len;
        char         header_buf[sizeof(UINT32)];
        char*        body_buf;
        bool         sendOnly;   // true 表示仅用于发送，完成后释放
    };

    enum OP_TYPE { OP_RECV_HEADER, OP_RECV_BODY, OP_SEND };

    void postRecvHeader(PER_IO* pio);
    static DWORD WINAPI workerThread(LPVOID arg);
    void closeClient(PER_IO* pio);

    unsigned short      port_;
    int                 workerThreads_;
    LFThreadPool&       pool_;
    SOCKET              listenSock_;
    HANDLE              iocp_;
    std::unordered_map<unsigned long, SOCKET> clients_;
    std::mutex          clientsMutex_;
};

```

```cpp title:"TCPServer.cpp"
// TCPServer.cpp
#include "TCPServer.h"
#include "LFThreadPool.h"

#pragma comment(lib, "Ws2_32.lib")

TCPServer::TCPServer(unsigned short port, int workerThreads, LFThreadPool& pool)
    : port_(port), workerThreads_(workerThreads), pool_(pool),
      listenSock_(INVALID_SOCKET), iocp_(NULL) {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
}

TCPServer::~TCPServer() {
    if (listenSock_ != INVALID_SOCKET) closesocket(listenSock_);
    if (iocp_) CloseHandle(iocp_);
    WSACleanup();
}

bool TCPServer::run() {
    // 1) 创建 IOCP
    iocp_ = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    if (!iocp_) return false;

    // 2) 创建工作线程
    for (int i = 0; i < workerThreads_; i++) {
        HANDLE h = CreateThread(NULL, 0, workerThread, this, 0, NULL);
        CloseHandle(h);
    }

    // 3) 创建监听 socket
    listenSock_ = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr = {};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port_);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(listenSock_, (sockaddr*)&addr, sizeof(addr));
    listen(listenSock_, SOMAXCONN);

    printf("TCPServer listening on port %u\n", port_);

    // 4) accept 循环
    while (true) {
        SOCKET client = accept(listenSock_, NULL, NULL);
        if (client == INVALID_SOCKET) continue;

        // 关联到 IOCP
        CreateIoCompletionPort((HANDLE)client, iocp_, 0, 0);

        // 保存 client
        {
            std::lock_guard<std::mutex> lk(clientsMutex_);
            clients_[(unsigned long)client] = client;
        }

        // 为接收头部创建 PER_IO，开始接收
        PER_IO* pio = new PER_IO();
        ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
        pio->sock      = client;
        pio->op_type   = OP_RECV_HEADER;
        pio->body_buf  = nullptr;
        pio->sendOnly  = false;
        pio->wbuf[0].buf = pio->header_buf;
        pio->wbuf[0].len = sizeof(UINT32);

        DWORD flags = 0;
        WSARecv(client, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
        printf("Client connected: %u\n", (unsigned long)client);
    }

    return true;
}

void TCPServer::postRecvHeader(PER_IO* pio) {
    ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
    pio->op_type      = OP_RECV_HEADER;
    pio->wbuf[0].buf  = pio->header_buf;
    pio->wbuf[0].len  = sizeof(UINT32);
    DWORD flags = 0;
    WSARecv(pio->sock, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
}

void TCPServer::workerThread(LPVOID arg) {
    TCPServer* self = (TCPServer*)arg;
    DWORD bytes;
    ULONG_PTR key;
    OVERLAPPED* ov;

    while (GetQueuedCompletionStatus(self->iocp_, &bytes, &key, &ov, INFINITE)) {
        PER_IO* pio = CONTAINING_RECORD(ov, PER_IO, overlapped);
        if (bytes == 0) {
            // 客户端断开
            self->closeClient(pio);
            continue;
        }
        if (pio->op_type == OP_RECV_HEADER) {
            // 收完 4 字节头
            if (bytes != sizeof(UINT32)) { self->closeClient(pio); continue; }
            UINT32 netlen;
            memcpy(&netlen, pio->header_buf, sizeof(UINT32));
            pio->body_len = ntohl(netlen);
            if (pio->body_len == 0 || pio->body_len > 10*1024*1024) {
                self->closeClient(pio); continue;
            }
            // 准备接收 body
            pio->body_buf = (char*)malloc(pio->body_len);
            ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
            pio->op_type = OP_RECV_BODY;
            pio->wbuf[0].buf = pio->body_buf;
            pio->wbuf[0].len = pio->body_len;
            DWORD flags=0;
            int r = WSARecv(pio->sock, &pio->wbuf[0], 1, NULL, &flags, &pio->overlapped, NULL);
            if (r==SOCKET_ERROR && WSAGetLastError()!=WSA_IO_PENDING) {
                self->closeClient(pio);
            }
        }
        else if (pio->op_type == OP_RECV_BODY) {
            // 收完 body，打包任务提交线程池
            if ((DWORD)bytes != pio->body_len) {
                self->closeClient(pio); continue;
            }
            // 深拷贝一份给任务
            char* userBuf = (char*)malloc(pio->body_len);
            memcpy(userBuf, pio->body_buf, pio->body_len);
            TaskData task{ userBuf, (int)pio->body_len, (unsigned long)pio->sock };
            self->pool_.submit(task);
            // 释放 IO 缓冲，并继续接头
            free(pio->body_buf);
            pio->body_buf = nullptr;
            self->postRecvHeader(pio);
        }
        else if (pio->op_type == OP_SEND && pio->sendOnly) {
            // 发送完成，释放内存
            free(pio->body_buf);
            delete pio;
        }
    }
}

void TCPServer::closeClient(PER_IO* pio) {
    printf("Client disconnected: %u\n", (unsigned long)pio->sock);
    closesocket(pio->sock);
    {
        std::lock_guard<std::mutex> lk(clientsMutex_);
        clients_.erase((unsigned long)pio->sock);
    }
    if (pio->body_buf) free(pio->body_buf);
    delete pio;
}

void TCPServer::send(unsigned long clientId, const char* data, int len) {
    std::lock_guard<std::mutex> lk(clientsMutex_);
    auto it = clients_.find(clientId);
    if (it == clients_.end()) return;  // 客户端已断开
    SOCKET s = it->second;

    // 新建一个 send-only 的 PER_IO
    PER_IO* pio = new PER_IO();
    ZeroMemory(&pio->overlapped, sizeof(OVERLAPPED));
    pio->sock      = s;
    pio->op_type   = OP_SEND;
    pio->sendOnly  = true;
    pio->body_len  = len;
    // header
    UINT32 netlen = htonl(len);
    memcpy(pio->header_buf, &netlen, sizeof(UINT32));
    // body
    pio->body_buf = (char*)malloc(len);
    memcpy(pio->body_buf, data, len);
    // WSABUF
    pio->wbuf[0].buf = pio->header_buf;
    pio->wbuf[0].len = sizeof(UINT32);
    pio->wbuf[1].buf = pio->body_buf;
    pio->wbuf[1].len = len;

    DWORD sent = 0;
    int r = WSASend(s, pio->wbuf, 2, &sent, 0, &pio->overlapped, NULL);
    if (r==SOCKET_ERROR && WSAGetLastError()!=WSA_IO_PENDING) {
        // 发送失败，马上清理
        free(pio->body_buf);
        delete pio;
    }
}
```

使用示例（`main.cpp`）：

```cpp title:"main.cpp"
#include "TCPServer.h"
#include "LFThreadPool.h"

int main() {
    // 用户自行实现的无锁线程池
    LFThreadPool pool;

    // TCPServer 监听 12345 端口，创建 4 个 IOCP 线程
    TCPServer server(12345, 4, pool);

    // 在线程池中处理业务：这里只做简单的 echo
    pool.setWorkerFunc([&server](const TaskData& task) {
        // 处理 task.data, task.len
        // 这里直接回显
        server.send(task.from, task.data, task.len);
        // 任务数据由我们在 server 中 malloc，处理完后要 free
        free((void*)task.data);
    });

    // 启动 IOCP 服务（阻塞）
    server.run();

    return 0;
}
```

主要点：

- 收到完整报文后深拷贝到 `TaskData`，并调用 `LFThreadPool::submit()`。
- 提供 `TCPServer::send(clientId, data, len)`，在 IOCP 上异步发送（先发 4 字节长度头，再发内容）。
- `clientId` 简化地使用客户端的 `SOCKET` 值转换得到，用户在业务线程中可通过它来指定目标客户端。
- 线程池完成任务后需要负责 `free(task.data)`。