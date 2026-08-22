
## Calcite —— 跨平台智能笔记管理系统

**项目概述**：独立设计并全栈开发的一款跨平台智能笔记系统，覆盖 Web 端、Android 端及 C++ 后端，支持 Markdown 编辑、Elasticsearch 全文检索、AI 标签生成与 OCR 识别，总代码量 **3.5 万+ 行**。

**技术栈**
- **后端**：C++17/20、Drogon、MariaDB、Elasticsearch 8.12.2、MinIO、JWT
- **Web 前端**：Vue 3 (Composition API)、Vite、Pinia、Element Plus、Axios、md-editor-v3
- **Android**：Kotlin、Jetpack (Room / Navigation / WorkManager)、Retrofit、OkHttp、Coil、Markwon

**核心职责与亮点**
- **全栈架构设计**：独立负责前后端整体架构，后端采用分层架构（Controller → Service → ORM），Web 端基于 Pinia 做状态管理，Android 端采用 MVVM + Repository + 离线优先模式。
- **高性能全文检索**：集成 Elasticsearch + IK 中文分词器，实现毫秒级笔记全文搜索，并基于 `user_id` 做数据权限隔离，确保多用户场景下的检索安全。
- **AI 能力集成**：调用 DeepSeek / Kimi 在线 API，实现笔记智能标签推荐与 OCR 文本提取，减少用户手动整理成本。
- **对象存储与异步上传**：基于 MinIO 搭建 S3 兼容的文件存储服务，设计异步上传 + 轮询状态机制，避免前端长时间阻塞，提升大文件传输稳定性。
- **离线同步与移动端体验**：Android 端基于 Room 实现本地缓存与离线浏览，结合 WorkManager 完成后台增量同步；Web 端基于 CSS 变量实现 Everforest 深色/浅色双主题，完整覆盖 Element Plus 与 Markdown 编辑器组件。
- **安全与认证**：统一采用 JWT Bearer Token 认证，配合前端 401 拦截自动清理失效凭证；后端使用密码哈希存储，防止明文泄露。

**项目规模**
- 后端 C++ 服务：**~24,000 行** | Web 前端：**~6,400 行** | Android 客户端：**~5,000 行**
- 数据库设计：**13 张核心表**，涵盖笔记、文件夹、标签、用户、文件、OCR 记录等完整业务域

---

如果你希望针对 **后端/前端/Android/算法** 某个方向进行侧重删减，或者需要翻译成英文版本，告诉我，我可以进一步调整。