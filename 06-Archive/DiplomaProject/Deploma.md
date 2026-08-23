# 基于Web与Android双端的智能笔记管理系统

[[开题答辩]] 
[[每周小结]] 
[[note_origin]] 
[[note_251202]] 
[[note_251217]] 
[[note_260112]] 
[[note_260125]] 
[[note_260201]] 



[[周志]] 
[[周志_puretext]] 


---


[[0513_论文_参考文献.md]]
[[0513_论文_新增页面展示.md]]
[[0514_王莉莉.md]]
[[0514_王莉莉_简洁.md]]
[[0515_deepseek_Chapter3.md]]
[[0515_kimi_增加页面.md]]
[[0516_降AI.md]]
[[流程图.md]]
[[论文_GPT框架.md]]
[[论文_markdown.md]]
[[论文_纯markdown.md]]
[[260417_数据库操作.md]]
[[API设计&实现.md]]
[[DraftNote.md]]
[[DsService开发.md]]
[[DS获取标签bug.md]]
[[ES客户端重构.md]]
[[note_260410.md]]
[[note_260411.md]]
[[note_260412.md]]
[[note_260413_api.md]]
[[note_260413_file.md]]
[[note_260413_ocr.md]]
[[note_260414_gpt4o.md]]
[[note_260414_kimi.md]]
[[note_260415.md]]
[[note_260415_bugfix.md]]
[[note_260415_edit.md]]
[[note_260416_design.md]]
[[note_260417.md]]
[[note_260417_edit.md]]
[[note_260417_work.md]]
[[note_260420_API.md]]
[[note_260421_Android.md]]
[[note_260421_theme.md]]
[[note_260421_前端.md]]
[[note_260421_前端Pinia.md]]
[[note_260422.md]]
[[Open修改文件树可上下滑动.md]]
[[修改已有API.md]]
[[公开笔记预览页面.md]]
[[前端agent.md]]
[[多了个符号而已.md]]
[[推荐页面设计.md]]
[[搜索API重构.md]]
[[整理API.md]]
[[测试编译修复.md]]
[[论文 1.md]]
[[论文_降AIGC.md]]
[[重构搜索框.md]]
[[note_0_schema.md]]
[[note_251202.md]]
[[note_251217.md]]
[[note_260112.md]]
[[note_260125.md]]
[[note_260201.md]]
[[note_260203.md]]
[[note_260308.md]]
[[note_260315.md]]
[[note_260326.md]]
[[note_260329.md]]
[[note_260401_ChatGPT.md]]
[[note_260405.md]]
[[note_260407_0_选型&安装.md]]
[[note_260407_1_后端开发.md]]
[[note_260408.md]]
[[note_origin.md]]
[[开题答辩.md]]
[[每周小结.md]]
[[前端用户基本模块实现.md]]
[[后端基本用户模块API.md]]
[[周志.md]]
[[周志_2.md]]
[[周志_puretext.md]]
[[week_01.md]]
[[week_02.md]]
[[week_03.md]]
[[week_04.md]]
[[week_05.md]]
[[week_06.md]]
[[week_07.md]]
[[week_08.md]]
[[week_09.md]]
[[week_10.md]]
[[week_11.md]]
[[week_12.md]]
[[week_13.md]]
[[week_14.md]]
[[week_15.md]]
[[week_16.md]]
[[week_17.md]]
[[week_18.md]]
[[week_19.md]]
[[week_20.md]]
[[week_21.md]]
[[week_22.md]]


----


以下是一份可直接用于简历的 **Calcite** 项目描述，包含一句话概述、技术栈、核心亮点与量化数据：

---

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