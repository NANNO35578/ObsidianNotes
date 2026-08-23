# 开发周志 —— 第9周：各模块 API 接口详细设计

## 本周 TODO
- [ ] 完成用户模块 API 设计（注册 / 登录 / 退出 / 用户信息）
- [ ] 完成笔记模块 API 设计（增删改查 / 搜索 / 历史版本）
- [ ] 完成文件夹与标签模块 API 设计
- [ ] 完成文件模块 API 设计（上传 / 列表 / 删除 / 状态轮询）
- [ ] 完成 OCR、推荐、点赞收藏等高级功能 API 设计

## 工作内容概括

本周将 API 框架细化为可落地的接口文档，涵盖 8 大模块共 30+ 个接口：

| 模块 | 核心接口 |
|------|---------|
| 用户 | `/api/auth/register`, `/api/auth/login`, `/api/auth/logout`, `/api/user/profile` |
| 笔记 | `/api/note/create`, `/api/note/update`, `/api/note/delete`, `/api/note/list`, `/api/note/detail` |
| 搜索 | `/api/note/search`（ES 多字段加权检索） |
| 标签 | `/api/tags/hot`, `/api/notes/{id}/tags`, `/api/notes/{id}/tags/ai` |
| 文件夹 | `/api/folder/create`, `/api/folder/list`, `/api/folder/update`, `/api/folder/delete` |
| 文件 | `/api/file/upload`, `/api/file/list`, `/api/file/delete`, `/api/file/status`, `/api/file/info` |
| OCR | `/api/ocr/recognize`, `/api/ocr/status` |
| 互动 | `/api/note/view`, `/api/note/like`, `/api/note/collect`, `/api/recommend/notes` |

每个接口文档包含请求方式、URL、参数表（必填/类型/说明）、请求/响应示例、错误码与处理流程。特别注意文件上传与 OCR 采用异步模式：上传后返回 `file_id` 与 `processing` 状态，客户端通过轮询 `/api/file/status` 获取最终结果。

## 关键产出
- api.md（完整版 REST API 文档）
- 各模块接口参数校验规则
- 前端与 Android 对接所需的 API 清单

---

## 工作记录（精简版）

### 一、接口清单（8 模块 34 个）

| 模块 | 接口数 | 代表接口 | 特殊说明 |
|------|--------|----------|----------|
| 用户 | 4 | `/api/auth/login` | 返回 Token + user_id |
| 笔记 | 5 | `/api/note/search` | GET，keyword + folder_id 参数 |
| 文件夹 | 4 | `/api/folder/list` | GET，parent_id 为空时返回根目录 |
| 标签 | 4 | `/api/notes/{id}/tags/ai` | POST，异步触发 AI 生成 |
| 文件 | 5 | `/api/file/upload` | POST，multipart，返回 file_id + processing |
| OCR | 2 | `/api/ocr/recognize` | POST，提交 file_id，异步识别 |
| 互动 | 5 | `/api/recommend/notes` | GET，page + page_size，返回推荐列表 |
| 搜索历史 | 1 | `/api/search/history` | GET，返回用户最近 10 条搜索词 |

### 二、关键接口设计决策

**1. 笔记搜索 `/api/note/search`**
- 参数：`keyword`（必填）、`folder_id`（可选）、`page`、`page_size`
- ES 查询：multi_match 加权 `title^3 + tags^2 + summary^2 + content^1`
- 返回：高亮片段（`<mark>` 标注）、`_score` 相关度分数

**2. 文件上传 `/api/file/upload`**
- 请求：`multipart/form-data`，字段 `file` + 可选 `note_id`
- 响应：`{ file_id, status: "processing", url: null }`
- 客户端轮询：`GET /api/file/status?file_id=`，间隔 1s，最多 60 次

**3. OCR 识别 `/api/ocr/recognize`**
- 输入：已上传完成的 `file_id`
- 处理：后台调用第三方 OCR API，识别结果转 Markdown
- 输出：完成后自动生成新笔记，`note_id` 写入 OCR 任务记录

**4. 推荐 `/api/recommend/notes`**
- 新用户：冷启动融合标签（笔记 Top3 + 最新 Top2 + 搜索 Top2 + 热门 Top3）
- 老用户：兴趣分数公式 `score = (view + 3×like + 7×collect) × e^(-0.1×Δt)`，取 Top 5 标签
- 兜底：标签为空或无结果时返回最新公开笔记

### 三、参数校验规则

- 数值型：`id > 0`，`page ≥ 1`，`page_size ∈ [1, 100]`
- 字符串：`title` 长度 0-255，`content` 不限，非空字段trim后校验
- 枚举：`is_public ∈ {0, 1}`，`status ∈ {processing, done, failed}`
- 权限：所有写操作校验资源归属（`note.user_id == current_user_id`）

### 四、三方会签确认

后端输出 api.md v1.0 → Web 前端确认字段命名与嵌套层级 → Android 确认响应体积与分页格式 → 三方无异议后冻结接口。

**冻结后变更流程**：任何字段增删需提 Issue，三方重新评估影响范围，毕业设计周期内原则上不破坏性变更。

### 五、本周问题

- **URL 路径参数 vs 查询参数**：`/api/notes/{id}/tags` 用路径参数标识资源，`/api/note/list?folder_id=` 用查询参数过滤列表。前者符合 REST 资源定位语义，后者利于缓存与日志分析。
- **AI 生成标签接口的幂等性**：同一笔记多次调用可能生成不同标签。方案：每次调用替换旧标签，不保证幂等，前端提示「重新生成将覆盖现有标签」。

### 六、会议备忘

后端、Web、Android 三方对接会：确认接口文档以 Markdown 表格形式维护，存放于 `calcite_server/docs/api.md`，Web 与 Android 侧引用同一份文档，避免信息不同步。Postman Collection 作为辅助验证工具，由后端维护并导出分享给前端/Android。
