你是一名资深的后端 C++ 开发工程师，熟练使用 Drogon 框架进行 RESTful API 开发。
当前项目在调用`/api/notes/tags/ai`这个API时有时会崩溃.
你需要查看`/api/notes/tags/ai`的实现, 也就是`NoteController::generateNoteTagsByAi`函数的实现.

其中会调用`DsService::performLlmRequest`这个函数, 这个函数的实现似乎不是异步回调的, 我觉得可能会引发崩溃.

你需要查看`./calcite/Services/DsService.*`的实现, 找出可能崩溃原因并修复.
尽量不修改除了`./calcite/Services/DsService.*`以外的代码.


将你的操作在./docs中形成以260418_为前缀的文档.



````markdown
已完成对 `DsService` 的崩溃分析与修复。

---

### 🔍 崩溃根因

`DsService::performLlmRequest` 使用了 `std::thread` + `detach()` 的伪异步模式来包装同步的 `curl_easy_perform`，存在三个致命问题：

1. **线程生命周期失控**：每次请求都创建一个 detach 线程，服务器无法跟踪和回收。高并发下线程数可能飙升；服务器关闭时这些线程仍在访问 `drogon::app().getLoop()` 等全局对象，极易触发 use-after-free / 空指针解引用而崩溃。
2. **HTTP 连接断开后 callback 仍被调用**：LLM 请求耗时数秒至数十秒，期间客户端可能超时断开。但 detach 的线程对此一无所知，完成后仍通过 `queueInLoop` 调用 Drogon HTTP callback，向已销毁的连接写数据，导致未定义行为（偶发崩溃）。
3. **SSL 验证被强制关闭**：`CURLOPT_SSL_VERIFYPEER = 0` 降低了安全性。

---

### ✅ 修复方案

**用 Drogon 原生 `HttpClient` 完全替代裸 `curl` + `std::thread`**。

- `HttpClient` 完全基于事件循环，**不需要任何额外线程**；
- 请求回调都在事件循环线程中执行，天然线程安全；
- 当底层连接断开时，框架会自动安全处理，不会访问已销毁对象；
- 项目中 `EsClient` 已大量采用此模式，有成熟范例。

---

### 📁 修改内容

| 文件 | 变更 |
|------|------|
| `calcite/services/DsService.h` | 移除 `API_URL`，新增 `API_HOST` / `API_PATH`；新增 `drogon::HttpClientPtr client_` 成员 |
| `calcite/services/DsService.cc` | 删除 `curl` 相关代码；构造函数初始化 `client_`；`performLlmRequest` 改用 `client_->sendRequest` 纯异步实现 |
| `docs/260418_DsService_CrashFix.md` | 详细的崩溃分析、修复方案、编译验证及后续建议 |

---

### 📋 编译验证

主目标 `calcite` 编译通过（`calcite_test` 链接失败为已有问题，与本次修改无关）。

---

### ⚠️ 附带发现（未修改，仅记录）

在 `NoteController::generateNoteTagsByAi` 中存在一个独立的 `doInsert` 悬挂引用 bug：

```cpp
[doInsert, index](...) { doInsert(index + 1); }  // 按引用捕获局部 std::function
```

异步回调执行时 `doInsert` 栈对象已销毁，构成 use-after-free。文档中已给出修复建议（使用 `std::shared_ptr<std::function<void(size_t)>>` 按值捕获），如需修复可后续处理。
````