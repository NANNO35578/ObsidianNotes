https://stackoverflow.com/questions/3058272/explain-leader-follower-pattern

https://www.dre.vanderbilt.edu/~schmidt/POSA/POSA2/conc-patterns.html

# Concurrency Patterns

This chapter presents five patterns that address various types of concurrency architecture and design issues for components, subsystems, and applications: Active Object, Monitor Object, Half-Sync/Half-Async, Leader/Followers, and Thread-Specific Storage.

The choice of concurrency architecture has a significant impact on the design and performance of multi-threaded networking middleware and applications. No single concurrency architecture is suitable for all workload conditions and hardware and software platforms. The patterns in this chapter therefore collectively provide solutions to a variety of concurrency problems.

The first two patterns in this chapter specify designs for sharing resources among multiple threads or processes:

- The _Active Object_ design pattern decouples method execution from method invocation. Its purpose is to enhance concurrency and simplify synchronized access to objects that reside in their own threads of control

- The _Monitor Object_ design pattern synchronizes concurrent method execution to ensure that only one method at a time runs within an object. It also allows an object's methods to schedule their execution sequences cooperatively.

Both patterns can synchronize and schedule methods invoked concurrently on objects. The main difference is that an active object executes its methods in a different thread than its clients, whereas a monitor object executes its methods by borrowing the thread of its clients. As a result active objects can perform more sophisticated--albeit expensive--scheduling to determine the order in which their methods execute.

The next two patterns in this chapter define higher-level concurrency architectures:

- The _Half-Sync/Half-Async_ architectural pattern decouples asynchronous and synchronous processing in concurrent systems, to simplify programming without reducing performance undudly. This pattern introduces two intercommunicating layers, one for asynchronous and one for synchronous service processing. A queuing layer mediates communication between services in the asynchronous and synchronous layers.

- The _Leader/Followers_ architectural pattern provides an efficient concurrency model where multiple threads take turns to share a set of event sources to detect, demultiplex, dispatch, and process service requests that occur on the event sources. The Leader/Followers pattern can be used in lieu of the Half-Sync/Half-Async and Active Object patterns to improve performance when there are no synchronization or ordering constraints on the processing of requests by pooled threads.

Implementors of the Half-Sync/Half-Async and Leader/Followers patterns can use the Active Object and Monitor Object patterns to coordinate access to shared objects efficiently.

The final pattern in this chapter offers a different strategy for addressing certain inherent complexities of concurrency:

- The _Thread-Specific Storage_ design pattern allows multiple threads to use one `logically global` access point to retrieve an object that is local to a thread, without incurring locking overhead on each access to the object. To some extent this pattern can be viewed as the `antithesis` of the other patterns in this section, because it addresses several inherent complexities of concurrency by preventing the sharing of resources among threads.

Implementations of all patterns in this chapter can use the Synchronization patterns presented in Chapter 4 to protect critical regions from concurrent access.

Other patterns in the literature that address concurrency-related issues include Master-Slave [POSA1], Producer-Consumer [Grand98], Scheduler [Lea99a], and Two-phase Termination [Grand98].


---
![[../../../'attachments/LFThreadPool.png]]

下面给出一个基于 C++11 且采用“领导者–跟随者”模式的线程池实现。任务用一个简单的结构体表示：

```cpp
// 任务结构体，func 接受 (void* data, int len)
struct Task {
    void (*func)(void*, int);
    void* data;
    int len;
};
```

核心思路：

1. 每个工作线程启动后，要么首先成为唯一的「领导者」（leader），要么成为「跟随者」（follower）。
2. 领导者线程阻塞等待新任务；跟随者线程阻塞等待被提升为新的领导者。
3. 当有新任务到达时，只唤醒当前领导者；领导者取任务后，立即将自己地位交给一个跟随者（`notify_one`），自己降级为「工人」（processor）来执行任务。
4. 执行完任务的工人线程又回到「跟随者」状态，等待下次被提升。

---

```cpp
#include <iostream>
#include <vector>
#include <deque>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>

// 任务结构体
struct Task {
    void (*func)(void*, int);
    void* data;
    int len;
};

class LeaderFollowerThreadPool {
public:
    LeaderFollowerThreadPool(size_t thread_count)
      : stopping(false), leader_exists(false)
    {
        for (size_t i = 0; i < thread_count; ++i) {
            threads.emplace_back(&LeaderFollowerThreadPool::threadLoop, this);
        }
    }

    ~LeaderFollowerThreadPool() {
        {
            std::lock_guard<std::mutex> lk(mtx);
            stopping = true;
            // 唤醒所有，退出阻塞
            task_cv.notify_all();
            follower_cv.notify_all();
        }
        for (auto& t : threads) {
            if (t.joinable()) t.join();
        }
    }

    // 提交任务
    void submit(const Task& task) {
        {
            std::lock_guard<std::mutex> lk(mtx);
            if (stopping) throw std::runtime_error("ThreadPool is stopping");
            task_queue.push_back(task);
        }
        // 唤醒领导者（如果当前确实有领导者在等候）
        std::lock_guard<std::mutex> lk(mtx);
        if (leader_exists) {
            task_cv.notify_one();
        } else {
            // 万一没有领导者，则唤醒一个跟随者来担任新领导者
            follower_cv.notify_one();
        }
    }

private:
    // 线程主循环：先选举身份，然后进入相应循环
    void threadLoop() {
        std::unique_lock<std::mutex> lk(mtx);
        if (!leader_exists) {
            leader_exists = true;
            leaderLoop(lk);
        } else {
            followerLoop(lk);
        }
    }

    // 领导者循环：等待新任务，拿到任务后选举继任者，自身降级为“工人”执行任务
    void leaderLoop(std::unique_lock<std::mutex>& lk) {
        while (!stopping) {
            // 等待新任务
            task_cv.wait(lk, [&]{ return stopping || !task_queue.empty(); });
            if (stopping) break;

            // 拿到一个任务
            Task task = task_queue.front();
            task_queue.pop_front();

            // 选举新的领导者
            leader_exists = false;
            follower_cv.notify_one();

            // 自身变为工人，释放锁去执行任务
            lk.unlock();
            task.func(task.data, task.len);
            lk.lock();

            // 执行完毕，自己重新成为跟随者
            followerLoop(lk);
            return ;
        }
    }

    // 跟随者循环：等待被提升为领导者
    void followerLoop(std::unique_lock<std::mutex>& lk) {
        while (!stopping) {
            // 等待领导者职位空缺
            follower_cv.wait(lk, [&]{ return stopping || !leader_exists; });
            if (stopping) break;

            // 自己被提升为新领导者
            leader_exists = true;
            leaderLoop(lk);
            return;
        }
    }

    // 管理成员
    std::vector<std::thread>    threads;
    std::deque<Task>            task_queue;
    std::mutex                  mtx;
    std::condition_variable     task_cv;     // 领导者等待新任务
    std::condition_variable     follower_cv; // 跟随者等待领导者职位
    bool                        leader_exists;
    bool                        stopping;
};

// 示例：一个简单的工作函数
void printData(void* data, int len) {
    std::string* s = static_cast<std::string*>(data);
    std::cout << "Thread " << std::this_thread::get_id()
              << " got data = " << *s
              << ", len = " << len << std::endl;
}

int main() {
    LeaderFollowerThreadPool pool(4);

    // 提交 10 个简单任务
    for (int i = 0; i < 10; ++i) {
        auto s = new std::string("hello_" + std::to_string(i));
        Task t;
        t.func = &printData;
        t.data = s;
        t.len  = static_cast<int>(s->size());
        pool.submit(t);
        // 注意：真实场景下要管理好 data 的生命周期，这里只是示例
    }

    // 稍等一下，让所有任务跑完
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}
```

解释要点：

- `leader_exists` 标志当前是否有一位领导者在等待新任务。
- 所有非领导者线程都在 `follower_cv` 上等待，一旦领导者空缺就会被唤醒并成为新的领导者。
- 领导者线程一旦收到任务，就先从队列里弹出任务，立即把领导权交给一个跟随者（`follower_cv.notify_one()`），再自己降级为工人去执行任务，执行完毕后重返跟随者身份。
- 通过这种方式，线程间无需反复交换任务队列锁（只有一次锁/解锁），并且领导者-工作者角色转换在同一个线程内部完成，提高了 CPU cache 一致性。

这个简单示例可根据需要扩展：超时关闭、批量任务、任务优先级等。


----

# 为什么没有栈溢出

> 每个线程好像是一直在leaderLoop中调用followerLoop, 然后在followerLoop中调用leaderLoop. 

下面其实并没有“无穷递归”——每一次从 `leaderLoop` 跳到 `followerLoop`（或反过来）都是一个**尾调用**（tail call），并且紧跟着一个 `return`，所以调用链并不会无限增长。

具体地看一下调用流程（以某个线程为例）：

1. 线程进来，执行  
   ```
   threadLoop()  // 栈帧 1
     └── leaderLoop()  // 栈帧 2
   ```
2. `leaderLoop` 等到一个任务，干完以后：  
   ```cpp
   followerLoop(lk);  // 这是尾调用
   return;            // 紧跟着就 return
   ```  
   - 因为它是尾调用，编译器往往会把它当作一次跳转（jmp）而不是再压一个新的栈帧。  
   - 即便不作 TCO（tail‐call optimization），执行完 `followerLoop` 立即就 `return`，`leaderLoop` 的栈帧也被弹掉。

3. `followerLoop` 被唤醒、被提升为 leader：  
   ```cpp
   leaderLoop(lk);  // 这也是尾调用
   return;
   ```  
   同样地，它也是尾调用＋紧跟 `return`，调用完马上退回去。

4. 最终整条调用链会这样「进—出—退」：
   ```
   threadLoop()        ←──┐
     leaderLoop()       ←─┘  （尾调用 followerLoop 后马上 return）
       followerLoop()  ←──┐
         leaderLoop()   ←─┘  （尾调用后 return）
   // 回到 threadLoop，threadLoop 也没有后续代码，直接 return，线程结束
   ```

所以：

- **递归深度** 最多就是那两三层栈帧，并不会随着任务数量累积下去。  
- **尾调用优化** 又会把那些 “互相调用然后马上 return” 的模式，编译器进一步变成跳转，也完全不会增长新的栈帧。  

只要两点都成立，就绝不可能出现「层层递归、栈一直往下压」的情况，自然也就不会有栈溢出。

在这里之所以不会“栈溢出”，关键有两点：

1. 你看到的并不是无限制地互相调用、层层压栈再也不弹出的那种“深度递归”；  
2. 这两处调用都是**尾调用**（tail call），在逻辑上等同于“跳”而不是“压→返回→再压→再返回” 的那种循环。

---

## 1. 尾调用（tail call）≠ 仅仅自我调用

- **尾递归（tail‐recursion）**：函数在自己的尾部直接调用自己，编译器可以做“尾调用优化”（TCO），把那次调用优化成一次跳转，**不再保留当前函数的栈帧**。  
- **尾调用** 的概念更广：只要一个函数在尾部（return 前）调用另一个函数（或自己），编译器理论上都可以把这次调用当成一次跳转而不是压栈，调用完就直接复用/销毁栈帧。  
- 你代码里是“互相尾调用”——`leaderLoop→followerLoop→leaderLoop`，每次调用后紧跟 `return`，这都符合尾调用的形态。

---

## 2. 调用栈深度是有上限的，而且每次都会返回

假设编译器**不做**任何尾调用优化，最坏的调用链也只是：

```
threadLoop()            // 栈帧1
  └─ leaderLoop()       // 栈帧2
       └─ followerLoop()// 栈帧3
            └─ leaderLoop() // 栈帧4
               // 执行完后返回，
            ←─ 返回 followerLoop（弹栈帧4）
         ←─ 返回 leaderLoop（弹栈帧3）
  ←─ 返回 threadLoop（弹栈帧2/1）
```

—— 最多 4 层，很快就全部弹栈，线程的主循环就结束了（如果你的代码里需要线程一直循环接任务，可以把 `return;` 换成 `continue;` 再进一轮 leader/follower 循环）。

如果编译器做了**尾调用优化**，那上面那个“帧 3 调帧 4 → 然后马上 return”根本就不会真正*新增*一个栈帧，而是编译器把它当作一次跳转，直接复用帧 3。这样压栈深度就恒定了。

---

## 3. 为什么不长成无限深度

- 每次 `leaderLoop` 干完活调用 `followerLoop` 时，**立刻**执行 `return`，它的栈帧就会被弹掉。  
- `followerLoop` 同理，调用完 `leaderLoop` 也马上 `return`，再把自己的帧弹掉。  
- 所以循环调用之间并不会把“老的”帧留在栈上，最多常驻 2～4 个帧，很快就都弹光了。

---

总结：  
- “尾调用”不限于“尾递归”（自己调自己），只要调用位置在 `return` 之前即可做 TCO；  
- 就算没有 TCO，调用深度也是常数级，调用完马上返回、弹栈，不会累积到无穷大。  
因此，这个 leader–follower 模式下，你看似互相调用、循环切换，实际上**并不会**导致栈帧无限增长，也就不会发生栈溢出。


-----
----
----


数据库管理单例, 可进行查插入, 查表;



![[../../../'attachments/map.png]]


[![](https://mermaid.ink/img/pako:eNp1U1tP1EAY_SuTeTCQLGUL7e7SBw275bq7gEBitOVhbL-9hO5MM21R3G6iMRANYIjyYIIhMV6fFBNjCPwbQ3fxXzi9bNwX-zSXc853vu90uthiNmANNxz2yGoR7qNN3aRIfF7wsMmJ20JLq5U1VGPMTc8RmjVmLQtcf3IdrB0UfTvsvzi-vjy4vvq1hSYmbodrzPMrrOM64LcZDVF5rA52m_iMS4zGanM7QP3xVA6obdJ0WY7ZqNJNtQY_rqKzgzu99K6SKCcFdeKTEOlGdP7s5sPe9cUl2iTe9tYobkOoZsZCNGf0L_YGx_uT0cejwclXtLoD3CFuRtCTovOGiTdbHIi9xphTm5dE9522PxYrj5s4w84n4jWBAv57__U9xreBh2hBkPsvv9y8P0SNgFpjtjCYc4D-Iy4kRRa7_bffo-PPf949RXp52NliInofvBAtGXq5TihpApeIt0utuwHw3UxjKdFYFhAUnZ7dnD8XDkbmqpfXwXWG4OUEXI1nFB3uRW-OosuT0TFV065HHaywENWMwenP_qtP_6PVRmlVdCs7qBtxqhvAxWQlV6Qfzz-j1BPEijESiTCeZYDWIfBgC-dwk7dtrPk8gBzuAO-QeIu7sYaJ_RZ0wMSaWNrQIIHjm9ikPUFzCX3AWGfI5CxotoabwBVBgN4m4jcWiAZxvBgifACvsID6WJtWSokG1rr4MdamZFUq5GemZvKqoqpKQS3m8C7WFEUqlYoluVhQp-V8vqD2cvhJUlWW5LysFGbkYqmoFqZLylQOi0hEIvX0WSWvq_cX5pIz6Q?type=png)](https://mermaid.live/edit#pako:eNp1U1tP1EAY_SuTeTCQLGUL7e7SBw275bq7gEBitOVhbL-9hO5MM21R3G6iMRANYIjyYIIhMV6fFBNjCPwbQ3fxXzi9bNwX-zSXc853vu90uthiNmANNxz2yGoR7qNN3aRIfF7wsMmJ20JLq5U1VGPMTc8RmjVmLQtcf3IdrB0UfTvsvzi-vjy4vvq1hSYmbodrzPMrrOM64LcZDVF5rA52m_iMS4zGanM7QP3xVA6obdJ0WY7ZqNJNtQY_rqKzgzu99K6SKCcFdeKTEOlGdP7s5sPe9cUl2iTe9tYobkOoZsZCNGf0L_YGx_uT0cejwclXtLoD3CFuRtCTovOGiTdbHIi9xphTm5dE9522PxYrj5s4w84n4jWBAv57__U9xreBh2hBkPsvv9y8P0SNgFpjtjCYc4D-Iy4kRRa7_bffo-PPf949RXp52NliInofvBAtGXq5TihpApeIt0utuwHw3UxjKdFYFhAUnZ7dnD8XDkbmqpfXwXWG4OUEXI1nFB3uRW-OosuT0TFV065HHaywENWMwenP_qtP_6PVRmlVdCs7qBtxqhvAxWQlV6Qfzz-j1BPEijESiTCeZYDWIfBgC-dwk7dtrPk8gBzuAO-QeIu7sYaJ_RZ0wMSaWNrQIIHjm9ikPUFzCX3AWGfI5CxotoabwBVBgN4m4jcWiAZxvBgifACvsID6WJtWSokG1rr4MdamZFUq5GemZvKqoqpKQS3m8C7WFEUqlYoluVhQp-V8vqD2cvhJUlWW5LysFGbkYqmoFqZLylQOi0hEIvX0WSWvq_cX5pIz6Q)















