# 开发周志 —— 第13周：Elasticsearch 全文检索与 OCR 模块

## 本周 TODO
- [ ] 封装 EsClient 工具类，实现 ES 连接与索引操作
- [ ] 实现笔记创建/更新时的异步 ES 索引同步
- [ ] 实现 `/api/note/search` 多字段加权搜索与高亮返回
- [ ] 集成第三方 OCR API，实现 `/api/ocr/recognize` 与 `/api/ocr/status`
- [ ] OCR 完成后自动生成 Markdown 格式新笔记

## 工作内容概括

本周完成系统的两个智能化能力。全文检索方面：
- 封装 `EsClient`，管理 ES 连接池与 `notes` 索引的增删改查；
- 笔记创建/更新后，通过 Drogon 异步回调在后台线程中同步 ES 索引，ES 失败不阻塞主流程；
- 搜索接口支持多字段加权：`title^3`、`tags^2`、`summary^2`、`content^1`，返回高亮片段（`<mark>` 标注）与相关度分数 `_score`。

OCR 模块方面：
- `/api/ocr/recognize` 接收文件提交，创建 OCR 任务记录，状态为 `processing`；
- 后台异步调用第三方 OCR API，识别结果按版面结构转为 Markdown 文本；
- 识别完成后自动创建新笔记，标题为文件名，内容为 Markdown；
- 客户端通过 `/api/ocr/status` 轮询任务进度。

## 关键产出
- EsClient 工具类与搜索 API
- OCR 识别与自动笔记生成链路
- 异步 ES 同步机制

---

## 工作记录（精简版）

### 一、EsClient 封装

封装 `EsClient` 单例类，基于 `libcurl` 发送 HTTP 请求至 ES REST API。

| 方法 | 功能 |
|------|------|
| `indexDocument(id, json)` | 创建/更新笔记索引 |
| `deleteDocument(id)` | 删除索引（笔记软删时同步调用）|
| `search(query, from, size)` | 执行 DSL 查询，返回高亮结果 |

**连接配置**：ES 地址 `localhost:9200`，索引名 `notes`，批量操作使用 `_bulk` API。

### 二、异步 ES 同步机制

笔记创建/更新后，主线程返回响应，后台线程异步执行 ES 索引：

```cpp
// NoteService 中
auto future = std::async(std::launch::async, [note]() {
    EsClient::instance().indexDocument(note.id, note.toJson());
});
```

失败时记录至 `es_sync_log` 表（note_id, fail_time, retry_count），供后续补偿任务扫描重试。

### 三、搜索 API 实现

`/api/note/search?keyword=&page=&page_size=`

ES DSL 核心：

```json
{
  "query": {
    "bool": {
      "must": [
        { "multi_match": { "query": "keyword", "fields": ["title^3", "tags^2", "summary^2", "content"] }}
      ],
      "filter": [
        { "term": { "user_id": 123 }}
      ]
    }
  },
  "highlight": { "fields": { "title": {}, "content": { "fragment_size": 150 }}}
}
```

返回字段：`note_id`, `title`（高亮）, `content`（高亮片段）, `_score`。

### 四、OCR 模块

**流程**：提交 `file_id` → 校验文件状态为 `done` → 创建 OCR 任务（写 `ocr_task` 表，status=processing）→ 后台调用第三方 OCR API → 识别结果转 Markdown → 创建新笔记 → 更新 OCR 任务为 done。

**第三方 OCR API 选型**：采用某云服务商通用文字识别接口，支持 PDF/图片，返回结构化 JSON（段落坐标 + 文本），服务端按段落拼接为 Markdown。

### 五、本周问题

- **ES 索引延迟**：异步索引导致笔记创建后立即可搜索出现延迟（通常 < 500ms）。方案：对新建笔记的搜索请求同时查 ES 与 DB（DB 作为 fallback），推荐列表等容忍延迟的场景仅查 ES。
- **OCR Markdown 格式**：纯文本拼接丢失标题层级。优化：根据字体大小与段落间距推断标题（大字号→H1/H2），普通文本→段落，提升可读性。

### 六、会议备忘

自查：搜索接口已通过 Postman 测试，中文分词「机器学习」可正确召回；OCR 端到端链路（上传 PDF → 识别 → 生成笔记）已跑通，识别准确率约 85%（印刷体）。
