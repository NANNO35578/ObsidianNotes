# 开发周志 —— 第4周：系统总体架构设计

## 本周 TODO
- [ ] 绘制系统总体架构图（前后端分离 + 基础设施层）
- [ ] 设计三端交互方式与数据流图
- [ ] 确定异步任务处理方案（ES 同步、MinIO 上传、OCR）
- [ ] 制定模块划分与层级依赖原则（Controller → Service → Model）

## 工作内容概括

本周将需求转化为技术架构。系统采用经典前后端分离模式：服务端为 Drogon 高性能 RESTful API；Web 端为 Vue 3 SPA；Android 端为原生 Kotlin App。三端通过 HTTP/REST 通信，采用无状态 JWT 认证。

在数据流设计中，以「用户创建笔记并生成 AI 标签」为主线，完整梳理了从 UI 输入到 Drogon Controller、Service 层、ORM 落库、异步 ES 索引、AI 服务编排、最终返回 note_id 的全链路。明确异步任务采用 Drogon 异步数据库回调 + 后台线程实现，不引入独立消息队列，以降低部署复杂度。后端内部分层严格执行 Controller（参数/响应）→ Service（业务逻辑）→ Model/Utils（数据/基础设施）的依赖方向。

## 关键产出
- 系统总体架构图
- 三端数据流时序图
- 后端模块划分与目录结构定义

---

## 详细工作记录

### 一、架构设计迭代过程

本周经历两轮架构设计迭代。初稿采用「分层架构」（表现层 / 业务层 / 数据层），但在细化文件上传链路时发现分层过于粗粒度，无法表达「异步任务如何穿越层级」的复杂性。经查阅 Drogon 官方文档与《软件架构设计：大型网站技术架构与业务架构融合之道》后，改为「分层 + 管道」混合架构：同步请求走传统分层，异步任务走独立管道。

### 二、系统总体架构图（详细版）

#### 2.1 架构分层定义

```
┌──────────────────────────────────────────────────────────────┐
│ 接入层（Presentation Layer）                                  │
│  Web SPA (Vue 3) │ Android App (Kotlin) │ 第三方 OCR API    │
└──────────────────────────┬───────────────────────────────────┘
                           │ HTTPS/REST JSON
┌──────────────────────────▼───────────────────────────────────┐
│ 网关/控制层（Controller Layer）                               │
│  AuthController │ NoteController │ FolderController │ ...    │
│  职责：参数校验、鉴权、路由分发、响应组装                      │
└──────────────────────────┬───────────────────────────────────┘
                           │
┌──────────────────────────▼───────────────────────────────────┐
│ 业务逻辑层（Service Layer）                                   │
│  AuthService │ NoteFolderService │ OcrService │ KimiService │
│  职责：业务规则编排、事务边界、跨域协同                        │
└──────────────┬─────────────────────────────┬─────────────────┘
               │                             │
┌──────────────▼──────────────┐  ┌───────────▼────────────────┐
│ 数据访问层（Data Layer）     │  │ 基础设施层（Infra Layer）   │
│  Drogon ORM → MariaDB       │  │  EsClient → Elasticsearch  │
│  职责：CRUD、事务、索引优化   │  │  MinioClient → MinIO       │
│                             │  │  JwtUtil / PasswordUtil    │
└─────────────────────────────┘  └────────────────────────────┘
```

#### 2.2 关键架构决策记录（ADR）

**ADR-001：不使用独立消息队列**
- 背景：文件上传、ES 同步、OCR 识别均为异步任务，业界通常引入 Kafka/RabbitMQ；
- 考虑：引入消息队列会增加部署复杂度（需维护 Broker、Consumer Group），与毕业设计「轻量自托管」的目标冲突；
- 决策：使用 Drogon 的异步数据库回调（`execSqlAsync`）与 C++ `std::thread` 后台线程替代；
- 后果：系统吞吐量上限受限于线程池大小，但毕业设计场景下完全足够；丢失了消息持久化能力，若进程崩溃可能导致任务丢失，通过「数据库状态字段 + 定时扫描补偿」缓解。

**ADR-002：Web 端不使用 SSR**
- 背景：Vue 3 支持 Nuxt.js SSR，可改善首屏加载与 SEO；
- 考虑：笔记系统为强交互型应用，SEO 需求极低；SSR 增加部署复杂度（需 Node.js 服务器）；
- 决策：采用纯 CSR SPA，首屏加载通过 Vite 代码分割 + 懒加载优化。

**ADR-003：Android 端本地数据库优先展示**
- 背景：Android 端在无网络时需要可浏览笔记；
- 考虑：「远程优先」（每次都请求 API）简单但离线不可用；「本地优先」需解决数据一致性；
- 决策：采用「本地优先 + 后台同步」策略（Read-Through / Write-Behind），所有列表优先从 Room 读取，网络请求成功后更新本地缓存。

### 三、数据流详细设计

以「用户创建笔记并生成 AI 标签」为主线，绘制完整数据流：

#### 3.1 正常流程（时序图文字版）

```
用户(Web/Android)            前端/移动端              后端                 数据库/ES/AI
    │                           │                     │                       │
    │ 输入标题、内容、选择文件夹   │                     │                       │
    │──────────────────────────>│                     │                       │
    │                           │  POST /api/note/create                      │
    │                           │────────────────────>│                       │
    │                           │                     │  参数校验、JWT 鉴权    │
    │                           │                     │─────┐                 │
    │                           │                     │     │                 │
    │                           │                     │<────┘                 │
    │                           │                     │  ORM 插入 note 表     │
    │                           │                     │─────────────────────>│
    │                           │                     │  返回 note_id         │
    │                           │                     │<─────────────────────│
    │                           │                     │  触发后台线程         │
    │                           │                     │─────┐                 │
    │                           │                     │     │ 异步索引 ES      │
    │                           │                     │     │────────────────>│
    │                           │                     │     │ 调用 DeepSeek   │
    │                           │                     │     │────────────────>│
    │                           │                     │<────┘                 │
    │                           │<────────────────────│                       │
    │                           │  {"code":0,"data":{"note_id":1}}             │
    │  展示新笔记                │                     │                       │
    │<──────────────────────────│                     │                       │
    │                           │                     │                       │
    │                           │  [稍后] 轮询标签状态  │                       │
    │                           │────────────────────>│                       │
    │                           │                     │  查询 note_tag 关联   │
    │                           │                     │─────────────────────>│
    │                           │                     │  返回已绑定标签       │
    │                           │                     │<─────────────────────│
    │                           │<────────────────────│                       │
    │  展示 AI 生成标签          │                     │                       │
    │<──────────────────────────│                     │                       │
```

#### 3.2 异常流程设计

- **ES 索引失败**：后台线程捕获异常后记录日志，不阻塞主响应；通过定时任务（或管理员手动触发）重新同步失败笔记；
- **DeepSeek API 超时**：设置 10 秒超时，超时后返回笔记创建成功但标签为空，前端提示「标签生成失败，请稍后重试」；
- **MariaDB 主键冲突**：ORM 层已处理自增主键，理论上不会出现；若出现（如手动插入），返回 code=1，message="数据冲突"。

### 四、异步任务处理方案详解

#### 4.1 异步任务分类

| 任务类型 | 触发时机 | 执行方式 | 状态追踪 | 失败补偿 |
|----------|----------|----------|----------|----------|
| ES 索引同步 | 笔记创建/更新/删除 | Drogon 事件循环 + 后台线程 | 无（隐性） | 定时扫描 `updated_at` 与 ES 不一致的笔记 |
| MinIO 文件上传 | 文件元数据写入后 | 独立后台线程 | `file_resource.status` | 用户手动重新上传 |
| OCR 识别 | 文件上传完成后 | 独立后台线程 + HTTP 客户端 | `file_resource.status` + 新 note | 用户手动重新提交 |
| AI 标签生成 | 笔记创建/更新后 | HTTP 客户端异步调用 | `note_tag` 关联表 | 用户手动触发重新生成 |

#### 4.2 Drogon 异步机制选型

Drogon 提供三种异步能力，本周逐一验证并确定使用场景：

1. **`execSqlAsync`**：异步数据库操作，适用于所有 DB 查询/写入，避免阻塞 IO 线程；
2. **`HttpClient`**：异步 HTTP 请求，适用于调用 DeepSeek API 和第三方 OCR API；
3. **`app().getLoop()->runInLoop()` + `std::thread`**：将耗时操作（文件流读取、MinIO 上传） offload 到后台线程，完成后通过 Event Loop 回调通知主线程。

**验证代码片段**（MinIO 异步上传）：

```cpp
// 主线程：接收上传请求，写入 DB，启动后台线程
auto fileId = orm::FileResource(model).insert().get();
std::thread([fileId, fileData]() {
    // 后台线程：上传至 MinIO
    bool success = MinioClient::upload(fileData);
    // 通过 Event Loop 回调更新 DB 状态
    app().getLoop()->runInLoop([fileId, success]() {
        auto status = success ? "done" : "failed";
        orm::FileResource::updateStatus(fileId, status);
    });
}).detach();
```

### 五、模块划分与目录结构定义

#### 5.1 后端目录结构（calcite_server/calcite/）

```
calcite/
├── CMakeLists.txt          # 主构建脚本
├── config.json             # Drogon 运行配置（端口、DB、线程数、日志）
├── main.cc                 # 程序入口，注册控制器与过滤器
├── controllers/            # HTTP 控制器（无业务逻辑）
│   ├── AuthController.h/cc
│   ├── NoteController.h/cc
│   ├── FolderController.h/cc
│   ├── TagController.h/cc
│   ├── FileController.h/cc
│   ├── OcrController.h/cc
│   ├── RecommendController.h/cc
│   └── UserController.h/cc
├── services/               # 业务逻辑层
│   ├── AuthService.h/cc
│   ├── NoteFolderService.h/cc
│   ├── OcrService.h/cc
│   ├── KimiService.h/cc
│   └── DsService.h/cc
├── models/                 # Drogon ORM 自动生成（禁止手写）
│   └── ...
├── utils/                  # 基础设施工具
│   ├── EsClient.h/cc
│   ├── JwtUtil.h/cc
│   ├── MinioClient.h/cc
│   └── PasswordUtil.h/cc
├── filters/                # Drogon 过滤器
│   └── JwtFilter.cc
└── test/                   # 测试子项目
    ├── CMakeLists.txt
    └── test_main.cc
```

#### 5.2 层级依赖原则

严格遵循依赖方向：

```
controllers/  →  services/  →  models/
     ↓              ↓            ↓
  filters/      utils/       utils/
```

- Controller 可调用 Service 和 Utils；
- Service 可调用 Models 和 Utils，**不可**调用 Controller；
- Utils 为纯工具类，**不可**调用 Controller 或 Service；
- Models 为 ORM 实体，**不可**调用任何上层代码。

** enforce 手段**：Code Review + 循环依赖检测脚本（`grep -r "#include.*controllers/" services/` 应无结果）。

### 六、非功能需求补充

根据上周导师意见，本周补充以下非功能需求指标：

| 类别 | 指标 | 目标值 | 验证方式 |
|------|------|--------|----------|
| 性能 | 接口平均响应时间 | < 200ms（P99 < 1s） | wrk / Postman 压测 |
| 性能 | 并发用户支持 | 1000 同时在线 | 实验室服务器压测 |
| 性能 | 首屏加载时间（Web） | < 3s（4G 网络） | Lighthouse |
| 可用性 | 系统可用性 | > 99%（毕业设计演示期间） | 人工监控 |
| 安全 | 密码存储 | bcrypt，cost factor ≥ 10 | 代码审计 |
| 安全 | Token 传输 | HTTPS + Bearer Header | 抓包验证 |
| 兼容性 | Web 浏览器 | Chrome ≥ 90, Firefox ≥ 88, Edge ≥ 90 | 浏览器矩阵测试 |
| 兼容性 | Android 系统 | Android 8.0 (API 24) 及以上 | 模拟器测试 |

### 七、本周问题与解决

**问题 1：Controller 与 Service 的职责边界模糊**
- 现象：初稿中 Controller 直接调用 ORM 查询，Service 沦为「透传层」；
- 讨论：参考 Spring Boot 与 Drogon 社区最佳实践，Controller 应仅做「HTTP 语义转换」（参数解析、响应码设置、JSON 组装），所有业务判断（如「文件夹是否循环引用」）必须下沉到 Service；
- 解决：重新定义 Service 为「事务边界与业务规则持有者」，Controller 不得直接操作 Models。

**问题 2：ES 与 MariaDB 的数据一致性策略**
- 现象：若采用「先写 ES 再写 DB」，ES 成功但 DB 失败会导致数据丢失；若「先写 DB 再写 ES」，DB 成功但 ES 失败会导致搜索不到；
- 讨论： CAP 定理下，选择**最终一致性**，以 DB 为唯一数据源（Source of Truth），ES 为异步镜像；
- 解决：采用「先写 DB，后异步同步 ES」策略，ES 失败时记录日志，通过定时补偿任务修复。

### 八、会议记录

**202X-XX-XX 架构评审会**
- 参与人：导师、本人、实验室后端方向同学 1 名
- 议题：系统总体架构设计 v0.5 评审
- 导师意见：
  1. 异步任务不引入消息队列的决策合理，但需在毕业论文中明确说明「为何不用消息队列」以及「替代方案的局限性」；
  2. 目录结构中缺少 `plugins/` 目录，若后续需要 Drogon 插件（如定时任务），建议预留；
  3. `config.json` 中硬编码数据库密码存在安全隐患，建议改为环境变量注入（毕业设计可简化，但需在文档中标注「生产环境需改进」）。
- 行动计划：
  1. 在架构文档中补充 ADR 格式的决策记录；
  2. 预留 `plugins/` 目录；
  3. `config.json` 支持从环境变量读取敏感配置。

---

## 工作记录（精简版）

### 一、架构选型

采用前后端分离 + 分层架构，异步任务不走消息队列，以 Drogon 异步回调 + 后台线程实现，降低部署复杂度。

```
Web/Android → Controller → Service → Models/Utils → MariaDB/ES/MinIO/AI
```

**关键 ADR**：
- 不用独立消息队列，以 DB 状态字段 + 定时扫描补偿
- Web 纯 CSR，不用 SSR
- Android 本地优先 + 后台同步

### 二、数据流设计

以「创建笔记并生成 AI 标签」为主链路：

1. Controller 校验参数与 Token
2. Service 编排：ORM 写 DB → 返回 note_id → 触发后台线程
3. 后台线程：异步索引 ES + 调用 DeepSeek API 生成标签
4. 客户端轮询获取已绑定标签

**异常策略**：ES 失败不阻塞主流程，记录日志后定时补偿；DeepSeek 超时返回空标签，前端提示重试。

### 三、模块划分

| 目录 | 职责 | 约束 |
|------|------|------|
| controllers/ | 参数校验、鉴权、响应组装 | 不直接操作 Models |
| services/ | 业务规则、事务边界 | 不调用 Controller |
| models/ | ORM 实体（自动生成） | 不调用上层 |
| utils/ | 纯工具类 | 无状态 |
| filters/ | JwtFilter 全局鉴权 | — |

### 四、非功能需求指标

| 类别 | 指标 | 目标 |
|------|------|------|
| 性能 | 接口平均响应 | P99 < 1s |
| 性能 | 并发用户 | 1000 在线 |
| 安全 | 密码存储 | bcrypt cost ≥ 10 |
| 兼容 | Web 浏览器 | Chrome/Firefox/Edge 最新两版 |
| 兼容 | Android | API 24+ |

### 五、本周问题

- **Controller/Service 边界模糊**：强制 Controller 只做 HTTP 语义转换，业务判断下沉 Service
- **ES 与 DB 一致性**：选最终一致性，DB 为唯一 Source of Truth，ES 为异步镜像

### 六、会议备忘

导师意见：毕业论文需说明不用消息队列的理由与局限；预留 `plugins/` 目录；`config.json` 敏感配置改为环境变量注入。
