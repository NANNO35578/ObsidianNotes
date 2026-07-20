# 我们知道在Linux下epoll是一个高效的IO多路复用同步非阻塞的机制, 在Windows下有没有类似的机制? 
- 最明显的区别在于: epoll是就绪模型, IOPC是完成模型
- 就绪模型意思是, 当某个文件fd有数据可读写时, 系统内核通知程序可以进行读写操作, 然后再去做下面的操作. 而完成模型意思是, 某个文件数据读写已经完成了, 可以直接执行下面的操作了.

IOCP 是“我告诉系统我要做什么，然后系统做完了告诉我”，这是**完成模型**；
epoll 是“我问系统现在能不能做，然后我自己去做”，这是**就绪模型**。

`[!!example:用投简历比喻]` 内推时, IOPC就是我们跟瑞哥说想投XX公司内推, 当有内推码后, 瑞哥就帮我们投了, 然后我们就开始面试了. <br>而epoll就相当于我们跟瑞哥说想投XX公司内推, 有内推码后, 瑞哥把内推码告诉我们, 我们自己去投递, 然后再去面试.

相比下, epoll的就绪线程安全问题是我们自己保证的: 当事件就绪时, 我们分为接受连接任务和事件业务, 在`if`分支中, 分别构建不同的任务投入队列队列, 然后由线程池加锁保证的并发情况下加锁不会出现多个线程处理同一任务. 
流程为: `事件就绪->epoll_wait返回->构造任务->投入线程池->线程池线程上锁读取任务->执行任务`

同时, IOPC的事件就绪后只会有一个线程操作, 一般不会出现数据竞争等线程安全问题. 

首先分析一下IOPC流程:`创建线程-> 线程阻塞等待IOPC事件-> 初始化IOPC事件-> 投递异步任务(读取客户端数据)-> 任务完成-> 线程处理任务-> 任务完成(回显数据)-> 继续投递异步事件-> .....`

原因是, 每次投递的异步操作使用独立的上下文, 什么意思呢? 就是每次投递的是单个socket与其绑定的完成异步操作的数据. 
而系统通过绑定的数据
IOCP 本质是事件队列, 因为事件是**单线程消费的**
`主线程投递 WSARecv() --> 内核后台处理 --> 操作完成 --> IOCP 入队 --> 某线程取出 --> 处理完成（再投递下一次）`

----

# 有没有了解其他操作系统上类似机制, 比如BSD上的kqueue?

- `kqueue` 与epoll类似, 也是就绪模型, 但是比epoll强一些.
- 强在哪儿? 支持监听的种类多
	1. fd的读写
	2. 信号捕获
	3. 进程退出通知
	4. 文件修改
	5. 定时器事件

| 特性     | kqueue (BSD)                 | epoll (Linux)                       | IOCP (Windows)                                                            |
| ------ | ---------------------------- | ----------------------------------- | ------------------------------------------------------------------------- |
| 模型类型   | readiness（就绪）                | readiness（就绪），支持 LT/ET              | completion（完成）                                                            |
| 调用流程   | kevent 注册/变更事件 → kevent 等待事件 | epoll_ctl 注册/变更事件 → epoll_wait 等待事件 | CreateIoCompletionPort 绑定句柄 → 发起异步 I/O → GetQueuedCompletionStatus 获取完成事件 |
| 线程模型   | 通常单线程＋非阻塞 I/O                | 通常单线程或多线程＋非阻塞 I/O                   | 完全异步＋线程池（工作者线程由系统唤醒）                                                      |
| 支持事件类型 | 文件、socket、定时器、信号等            | 主要是文件描述符上的读写                        | 文件、socket，真正的异步读写（Overlapped I/O）                                         |
| 触发模式   | level-triggered              | 支持 LT（默认）与 ET                       | 仅完成触发                                                                     |
| 可扩展性   | O(1)                         | O(1)                                | 高度可伸缩（线程池自动伸缩，IO 完成后唤醒线程）                                                 |
| 典型用途   | FreeBSD/macOS 高并发网络服务器       | Linux 高并发网络服务器                      | Windows 高并发网络或磁盘 I/O 服务                                                   |
|        |                              |                                     |                                                                           |


----

# [`kqueue`](https://zh.wikipedia.org/wiki/Kqueue) [IOPC](https://zh.wikipedia.org/wiki/IOCP)
**kqueue** 是一种可扩展的事件通知接口。2000 年 7 月发布的 [FreeBSD](https://zh.wikipedia.org/wiki/FreeBSD "FreeBSD") 4.1 中首次引入了 `kqueue`[1](https://zh.wikipedia.org/wiki/Kqueue#cite_note-1)，随后也被 [NetBSD](https://zh.wikipedia.org/wiki/NetBSD "NetBSD")、[OpenBSD](https://zh.wikipedia.org/wiki/OpenBSD "OpenBSD")、[macOS](https://zh.wikipedia.org/wiki/MacOS "MacOS") 等操作系统支持

`kqueue` 在[内核](https://zh.wikipedia.org/wiki/%E5%86%85%E6%A0%B8 "内核")与[用户空间](https://zh.wikipedia.org/wiki/%E7%94%A8%E6%88%B7%E7%A9%BA%E9%97%B4 "用户空间")之间充当输入输出事件的管线。因此在[事件循环](https://zh.wikipedia.org/wiki/%E4%BA%8B%E4%BB%B6%E5%BE%AA%E7%8E%AF "事件循环")的迭代中，进行一次 `kevent(2)` [系统调用](https://zh.wikipedia.org/wiki/%E7%B3%BB%E7%BB%9F%E8%B0%83%E7%94%A8 "系统调用")不仅可以接收未决事件，还可以修改事件过滤器

支持 `kqueue` 且与操作系统无关的库：

- [`libevent`](https://zh.wikipedia.org/wiki/Libevent)
- [`libuv`](https://zh.wikipedia.org/wiki/Libuv "Libuv")

其它平台上与 kqueue 等价的库：

- Solaris、 Windows、 AIX：[IOCP](https://zh.wikipedia.org/wiki/IOCP "IOCP")
- Linux：
    - [`epoll`](https://zh.wikipedia.org/wiki/Epoll "Epoll") 系统调用语义类似，但并不完全相同。`epoll` 在文件描述符可进行 I/O 操作时进行通知，而 kqueue 和 IOCP 都在请求的操作完成时进行通知
    - [`inotify`](https://zh.wikipedia.org/wiki/Inotify "Inotify") 是 Linux 上的内核子系统，可以在文件系统发生变化时通知应用程序

`libkqueue` 是在[用户空间](https://zh.wikipedia.org/wiki/%E7%94%A8%E6%88%B7%E7%A9%BA%E9%97%B4 "用户空间")实现的 `kqueue(2)`，将调用翻译为操作系统原生的事件机制。[2](https://zh.wikipedia.org/wiki/Kqueue#cite_note-2)







----

**面试题：**  
请比较 kqueue（BSD）、epoll（Linux）和 IOCP（Windows）三种*高效 I/O 多路复用／异步 I/O 机制*的原理、使用场景和优缺点，并分别给出一个最小可运行的示例代码框架

---

## 一、三种机制原理对比

|特性|kqueue (BSD)|epoll (Linux)|IOCP (Windows)|
|---|---|---|---|
|模型类型|readiness（就绪）|readiness（就绪），支持 LT/ET|completion（完成）|
|调用流程|kevent 注册/变更事件 → kevent 等待事件|epoll_ctl 注册/变更事件 → epoll_wait 等待事件|CreateIoCompletionPort 绑定句柄 → 发起异步 I/O → GetQueuedCompletionStatus 获取完成事件|
|线程模型|通常单线程＋非阻塞 I/O|通常单线程或多线程＋非阻塞 I/O|完全异步＋线程池（工作者线程由系统唤醒）|
|支持事件类型|文件、socket、定时器、信号等|主要是文件描述符上的读写|文件、socket，真正的异步读写（Overlapped I/O）|
|触发模式|level-triggered|支持 LT（默认）与 ET|仅完成触发|
|可扩展性|O(1)|O(1)|高度可伸缩（线程池自动伸缩，IO 完成后唤醒线程）|
|典型用途|FreeBSD/macOS 高并发网络服务器|Linux 高并发网络服务器|Windows 高并发网络或磁盘 I/O 服务|

---

## 二、各机制使用要点

### 1. kqueue

- 创建：`int kq = kqueue();`
- 注册事件：填充 `struct kevent`，调用 `kevent(kq, &changelist, nchanges, NULL, 0, NULL)`
- 等待事件：`n = kevent(kq, NULL, 0, evlist, maxevents, timeout);`
- 支持多种 filter：`EVFILT_READ`、`EVFILT_WRITE`、`EVFILT_TIMER`、`EVFILT_SIGNAL`……

### 2. epoll

- 创建：`int epfd = epoll_create1(0);`
- 注册／修改／删除事件：`epoll_ctl(epfd, EPOLL_CTL_ADD/MOD/DEL, fd, &ev);`
- 等待事件：`n = epoll_wait(epfd, evlist, maxevents, timeout);`
- 支持边沿触发（ET）：在注册时设 `ev.events |= EPOLLET;`

### 3. IOCP

- 创建完成端口：`HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);`
- 关联句柄：`CreateIoCompletionPort(fileHandle, iocp, (ULONG_PTR)key, 0);`
- 发起异步 I/O：例如 `ReadFile`／`WSARecv`，传入一个 `OVERLAPPED` 结构
- 等待并获取完成：`GetQueuedCompletionStatus(iocp, &transferred, &key, &overlapped, INFINITE);`

---

## 三、优缺点比较

| 方面         | kqueue                      | epoll                     | IOCP                     |
| ---------- | --------------------------- | ------------------------- | ------------------------ |
| 编程复杂度      | 中等，需要管理 kevent 结构           | 较低，API 简单                 | 较高，需要处理 OVERLAPPED、线程同步  |
| 资源占用       | 内核空间数据结构稍重，支持多种 filter      | 轻量，专注于文件描述符               | 内核＋用户空间线程池配合，资源占用中等      |
| 延迟与性能      | 非常低，O(1)，且支持 timer、signal 等 | 非常低，O(1)，ET 模式更省 syscalls | 低延迟，高并发下自动伸缩线程池，真正异步完成队列 |
| 平台依赖       | 仅 BSD 系列（FreeBSD/macOS 等）   | 仅 Linux                   | 仅 Windows                |
| 支持的 I/O 类型 | 广泛（包括定时器/信号/文件/网络）          | 主要网络／文件                   | 纯异步文件和网络                 |

---

## 四、示例代码框架

以下示例均为最简框架，省略错误检查与部分细节。

### 1. kqueue 示例（C）

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/event.h>
#include <sys/time.h>
#include <fcntl.h>
#include <netinet/in.h>

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // bind & listen 略
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);

    int kq = kqueue();
    struct kevent change;
    EV_SET(&change, listen_fd, EVFILT_READ, EV_ADD, 0, 0, NULL);
    kevent(kq, &change, 1, NULL, 0, NULL);

    struct kevent events[16];
    while (1) {
        int n = kevent(kq, NULL, 0, events, 16, NULL);
        for (int i = 0; i < n; i++) {
            if (events[i].ident == (unsigned)listen_fd) {
                int conn = accept(listen_fd, NULL, NULL);
                fcntl(conn, F_SETFL, O_NONBLOCK);
                EV_SET(&change, conn, EVFILT_READ, EV_ADD, 0, 0, NULL);
                kevent(kq, &change, 1, NULL, 0, NULL);
            } else {
                int fd = events[i].ident;
                char buf[1024];
                int r = read(fd, buf, sizeof(buf));
                if (r <= 0) {
                    close(fd);
                } else {
                    // echo 回显
                    write(fd, buf, r);
                }
            }
        }
    }
    return 0;
}
```

### 2. epoll 示例（C）

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <fcntl.h>
#include <netinet/in.h>

int main() {
    int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    // bind & listen 略
    fcntl(listen_fd, F_SETFL, O_NONBLOCK);

    int epfd = epoll_create1(0);
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.fd = listen_fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, listen_fd, &ev);

    struct epoll_event events[16];
    while (1) {
        int n = epoll_wait(epfd, events, 16, -1);
        for (int i = 0; i < n; i++) {
            int fd = events[i].data.fd;
            if (fd == listen_fd) {
                int conn = accept(listen_fd, NULL, NULL);
                fcntl(conn, F_SETFL, O_NONBLOCK);
                struct epoll_event ev2 = { .events = EPOLLIN, .data.fd = conn };
                epoll_ctl(epfd, EPOLL_CTL_ADD, conn, &ev2);
            } else {
                char buf[1024];
                int r = read(fd, buf, sizeof(buf));
                if (r <= 0) {
                    epoll_ctl(epfd, EPOLL_CTL_DEL, fd, NULL);
                    close(fd);
                } else {
                    write(fd, buf, r);
                }
            }
        }
    }
    return 0;
}
```

### 3. IOCP 示例（C，Windows）

```c
#include <winsock2.h>
#include <windows.h>
#include <stdio.h>

#define WORKER_THREADS 4

typedef struct {
    OVERLAPPED overlapped;
    SOCKET sock;
    WSABUF wbuf;
    char buf[1024];
} PER_IO;

DWORD WINAPI Worker(LPVOID arg) {
    HANDLE iocp = (HANDLE)arg;
    DWORD bytes;
    ULONG_PTR key;
    OVERLAPPED *olp;
    while (GetQueuedCompletionStatus(iocp, &bytes, &key, &olp, INFINITE)) {
        PER_IO *pio = CONTAINING_RECORD(olp, PER_IO, overlapped);
        if (bytes == 0) { // 客户端关闭
            closesocket(pio->sock);
            free(pio);
            continue;
        }
        // echo
        pio->wbuf.len = bytes;
        pio->wbuf.buf = pio->buf;
        DWORD sent;
        WSASend(pio->sock, &pio->wbuf, 1, &sent, 0, &pio->overlapped, NULL);
    }
    return 0;
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);
    SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    // bind & listen 略

    HANDLE iocp = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0);
    CreateIoCompletionPort((HANDLE)listen_sock, iocp, 0, 0);

    // 启动工作线程
    for (int i = 0; i < WORKER_THREADS; i++)
        CreateThread(NULL, 0, Worker, iocp, 0, NULL);

    while (1) {
        SOCKET client = accept(listen_sock, NULL, NULL);
        CreateIoCompletionPort((HANDLE)client, iocp, 0, 0);

        PER_IO *pio = calloc(1, sizeof(*pio));
        pio->sock = client;
        pio->wbuf.buf = pio->buf;
        pio->wbuf.len = sizeof(pio->buf);
        DWORD flags = 0;
        WSARecv(client, &pio->wbuf, 1, NULL, &flags, &pio->overlapped, NULL);
    }
    return 0;
}
```

---

## 五、总结

- **kqueue／epoll** 适合跨平台（BSD vs. Linux）场景下的高并发网络服务，编程相对简单，使用**就绪**模型，需要手动读写非阻塞 socket。
- **IOCP** 是 Windows 下最强的高并发 I/O 方案，基于**完成**模型，真正异步，系统会自动调度线程池，编程复杂度较高但性能可在线性扩展到数万并发。
- 面试中重点考察：
    1. 就绪（readiness）vs. 完成（completion）模型区别
    2. 边沿触发（ET）vs. 水平触发（LT）
    3. 各自的线程／事件调度模型
    4. 实际场景下如何选型及优化（如 avoid thundering herd、保持非阻塞、合并小包读写等）










































