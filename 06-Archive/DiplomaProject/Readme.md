<!--------- 作为本科毕业设计只能拿到良. 也就这样了 ------------>
## [Calcite](https://github.com/NANNO35578/calcite) —— 跨平台智能笔记管理系统

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



---------



[260823-kimi-目录整理](260823-kimi-目录整理.md)


## Devlope Log (Modified)

[251202-周进度小结](DevelopLog/251202-周进度小结.md)
[251202-实施流程规划](DevelopLog/251202-实施流程规划.md)
[251202-数据库设计](DevelopLog/251202-数据库设计.md)
[251202-测试用笔记](DevelopLog/251202-测试用笔记.md)
[251217-项目初始化Git](DevelopLog/251217-项目初始化Git.md)
[251217-项目立项](DevelopLog/251217-项目立项.md)
[260112-用户模块JWT](DevelopLog/260112-用户模块JWT.md)
[260201-标签分类API](DevelopLog/260201-标签分类API.md)
[260203-标签绑定修复](DevelopLog/260203-标签绑定修复.md)
[260308-TAGFolderAPI](DevelopLog/260308-TAGFolderAPI.md)
[260308-测试CMake](DevelopLog/260308-测试CMake.md)
[260326-Android模块开发](DevelopLog/260326-Android模块开发.md)
[260329-Vue文件树](DevelopLog/260329-Vue文件树.md)
[260329-文件树滑动修复](DevelopLog/260329-文件树滑动修复.md)
[260401-ES与OCR选型](DevelopLog/260401-ES与OCR选型.md)
[260405-ES后端集成](DevelopLog/260405-ES后端集成.md)
[260405-ES客户端重构](DevelopLog/260405-ES客户端重构.md)
[260405-JSON解析异常](DevelopLog/260405-JSON解析异常.md)
[260407-C++S3接入](DevelopLog/260407-C++S3接入.md)
[260407-MinIO与OCR部署](DevelopLog/260407-MinIO与OCR部署.md)
[260408-OCR线上方案](DevelopLog/260408-OCR线上方案.md)
[260410-文件与OCRAPI](DevelopLog/260410-文件与OCRAPI.md)
[260411-Web功能计划](DevelopLog/260411-Web功能计划.md)
[260412-Markdown与上传](DevelopLog/260412-Markdown与上传.md)
[260413-API设计详述](DevelopLog/260413-API设计详述.md)
[260413-前端OCR功能](DevelopLog/260413-前端OCR功能.md)
[260413-前端文件管理](DevelopLog/260413-前端文件管理.md)
[260414-Android开发指南](DevelopLog/260414-Android开发指南.md)
[260414-Android架构](DevelopLog/260414-Android架构.md)
[260415-Android修复清单](DevelopLog/260415-Android修复清单.md)
[260415-Note表结构修改](DevelopLog/260415-Note表结构修改.md)
[260415-前端API重构](DevelopLog/260415-前端API重构.md)
[260415-文件树Bug修复](DevelopLog/260415-文件树Bug修复.md)
[260415-笔记API调整](DevelopLog/260415-笔记API调整.md)
[260416-DsService实现](DevelopLog/260416-DsService实现.md)
[260416-推荐API实现](DevelopLog/260416-推荐API实现.md)
[260416-推荐算法设计](DevelopLog/260416-推荐算法设计.md)
[260417-推荐开发计划](DevelopLog/260417-推荐开发计划.md)
[260417-推荐标签方案](DevelopLog/260417-推荐标签方案.md)
[260417-推荐算法公式](DevelopLog/260417-推荐算法公式.md)
[260417-数据库原始设计](DevelopLog/260417-数据库原始设计.md)
[260418-DsService崩溃修复](DevelopLog/260418-DsService崩溃修复.md)
[260418-搜索API优化](DevelopLog/260418-搜索API优化.md)
[260420-API设计文档](DevelopLog/260420-API设计文档.md)
[260420-公开笔记预览](DevelopLog/260420-公开笔记预览.md)
[260420-悬浮搜索框](DevelopLog/260420-悬浮搜索框.md)
[260421-Android项目概览](DevelopLog/260421-Android项目概览.md)
[260421-Pinia状态管理](DevelopLog/260421-Pinia状态管理.md)
[260421-主题切换修复](DevelopLog/260421-主题切换修复.md)
[260421-前端性能优化](DevelopLog/260421-前端性能优化.md)
[260421-前端项目分析](DevelopLog/260421-前端项目分析.md)
[260421-推荐页面开发](DevelopLog/260421-推荐页面开发.md)
[260422-后端AGENTS](DevelopLog/260422-后端AGENTS.md)






## Papers
[251129-开题答辩](papers/251129-开题答辩.md)
[260428-论文](papers/260428-论文.md)
[260428-论文_GPT框架](papers/260428-论文_GPT框架.md)
[260428-论文_markdown](papers/260428-论文_markdown.md)
[260428-论文_puremarkdown](papers/260428-论文_puremarkdown.md)
[260513_论文_参考文献](papers/260513_论文_参考文献.md)
[260513_论文_新增页面展示](papers/260513_论文_新增页面展示.md)
[260513-流程图](papers/260513-流程图.md)
[260514-王莉莉-修改意见](papers/260514-王莉莉-修改意见.md)
[260515_deepseek_Chapter3](papers/260515_deepseek_Chapter3.md)
[260515_kimi_增加页面](papers/260515_kimi_增加页面.md)
[260516-降AIGC](papers/260516-降AIGC.md)
[260816-降AIGC-1](papers/260816-降AIGC-1.md)



### Weekly Log

[output-周志_puretext](papers/WeeklyLog/output-周志_puretext.md)
[prompt-周志](papers/WeeklyLog/prompt-周志.md)
[prompt-周志_2](papers/WeeklyLog/prompt-周志_2.md)
[week_01](papers/WeeklyLog/week_01.md)
[week_02](papers/WeeklyLog/week_02.md)
[week_03](papers/WeeklyLog/week_03.md)
[week_04](papers/WeeklyLog/week_04.md)
[week_05](papers/WeeklyLog/week_05.md)
[week_06](papers/WeeklyLog/week_06.md)
[week_07](papers/WeeklyLog/week_07.md)
[week_08](papers/WeeklyLog/week_08.md)
[week_09](papers/WeeklyLog/week_09.md)
[week_10](papers/WeeklyLog/week_10.md)
[week_11](papers/WeeklyLog/week_11.md)
[week_12](papers/WeeklyLog/week_12.md)
[week_13](papers/WeeklyLog/week_13.md)
[week_14](papers/WeeklyLog/week_14.md)
[week_15](papers/WeeklyLog/week_15.md)
[week_16](papers/WeeklyLog/week_16.md)
[week_17](papers/WeeklyLog/week_17.md)
[week_18](papers/WeeklyLog/week_18.md)
[week_19](papers/WeeklyLog/week_19.md)
[week_20](papers/WeeklyLog/week_20.md)
[week_21](papers/WeeklyLog/week_21.md)
[week_22](papers/WeeklyLog/week_22.md)








