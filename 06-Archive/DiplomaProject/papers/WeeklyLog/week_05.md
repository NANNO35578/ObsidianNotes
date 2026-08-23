# 开发周志 —— 第5周：模块接口规范与开发环境搭建

## 本周 TODO
- [ ] 细化各模块的接口契约（输入参数、返回值、错误码）
- [ ] 搭建后端 Drogon 开发环境，验证编译与运行
- [ ] 搭建 Web 前端 Vue 3 + Vite 工程骨架
- [ ] 搭建 Android Kotlin 工程，配置 AGP 与依赖版本目录
- [ ] 配置 Git 子模块结构（根仓库聚合三个独立仓库）

## 工作内容概括

本周进入工程化准备阶段。后端完成 Drogon 框架的安装与最小可运行验证，确认 `drogon_ctl` 工具可用；创建 `calcite_server` 子模块仓库，初始化 CMake 构建脚本与 `main.cc` 入口。Web 端初始化 `calcite_web` 子模块，配置 Vite 代理（`/api` → `localhost:8888`）、Element Plus 按需引入、md-editor-v3 集成测试。Android 端创建 `calcite_android` 子模块，配置 Gradle Version Catalog（`libs.versions.toml`）、Kotlin 2.0.21、Room KSP、Navigation Safe Args 等关键插件。

三个子模块以 Git submodule 形式挂载到根仓库 `calcite/`，确保各端可独立迭代，根仓库仅负责聚合与文档管理。同步完成了 `.gitignore`、`AGENTS.md`、`Readme.md` 等基础文档模板。

## 关键产出
- 后端最小可运行服务（Hello World）
- Web 与 Android 工程骨架
- Git 子模块配置完成

---

## 详细工作记录

### 一、接口契约细化过程

本周前半周专注于将上周的 API 规范草案转化为可落地的接口契约。采用「前后端/Android 三方会签」机制：后端出初稿 → Web 前端审阅调用便利性 → Android 审阅响应体积与嵌套层级 → 后端定稿。共完成 8 个模块、34 个接口的契约定义。

#### 1.1 接口契约模板示例

以「创建笔记」接口为例，展示契约的完整字段：

```markdown
### POST /api/note/create

**功能**：创建新笔记

**请求参数**：
| 字段 | 类型 | 必填 | 说明 | 校验规则 |
|------|------|------|------|----------|
| title | string | 否 | 笔记标题 | 长度 0-255，允许为空（空时显示「无标题」）|
| content | string | 否 | Markdown 内容 | 长度不限，允许为空 |
| summary | string | 否 | 摘要 | 长度 0-1000，可为 AI 自动生成或用户编辑 |
| folder_id | int64 | 否 | 所属文件夹 ID | 必须存在且属于当前用户，为空时归入根目录 |
| is_public | int | 否 | 是否公开 | 0=私有（默认），1=公开 |

**响应数据（data）**：
| 字段 | 类型 | 说明 |
|------|------|------|
| note_id | int64 | 新创建笔记的 ID |
| created_at | string | 创建时间，ISO 8601 格式 |

**错误码**：
- 1：参数错误（folder_id 不存在或不属于当前用户）
- 1001：Token 无效
```

#### 1.2 接口设计争议与决策

**争议 1：列表接口的分页与排序**
- 方案 A：所有列表接口统一支持 `page` + `page_size` + `sort_by` + `order`；
- 方案 B：简单列表（如标签列表）不分页，复杂列表（如笔记列表）分页；
- **决策**：采用方案 B，避免过度设计。笔记列表支持分页（默认 page_size=20），文件夹列表与标签列表不分页（假设用户文件夹数 < 1000，可一次返回）。

**争议 2：嵌套返回 vs 扁平返回**
- 方案 A：`/api/note/detail` 返回笔记详情时嵌套 `tags` 数组与 `files` 数组，前端一次调用即可渲染完整页面；
- 方案 B：笔记详情仅返回基础字段，`tags` 与 `files` 通过独立接口 `/api/notes/{id}/tags` 和 `/api/file/list?note_id={id}` 获取，降低单接口复杂度；
- **决策**：采用方案 B。理由：① 笔记详情查询频率最高，应避免 JOIN 过多导致性能下降；② 右侧边栏的标签与文件面板可独立刷新，解耦更利于状态管理；③ Drogon ORM 对复杂嵌套查询的支持较弱，扁平化接口更易实现。

**争议 3：文件上传的 Content-Type**
- 方案 A：Base64 编码放在 JSON body 中，统一 Content-Type 为 `application/json`；
- 方案 B：使用 `multipart/form-data`，文件以二进制流上传；
- **决策**：采用方案 B。Base64 会增加约 33% 体积，且大文件 JSON 解析耗内存；Drogon 原生支持 `MultipartFormData` 解析，与 MinIO SDK 的流式上传对接更自然。

### 二、后端开发环境搭建

#### 2.1 依赖安装与踩坑记录

后端开发环境基于 Ubuntu 22.04 LTS，以下为关键依赖的安装记录：

```bash
# Drogon 框架（从源码编译，最新稳定版 v1.9.6）
git clone https://github.com/drogonframework/drogon.git
cd drogon && git checkout v1.9.6
mkdir build && cd build
cmake .. -DBUILD_ORM=1 -DBUILD_POSTGRESQL=0  # 仅需 MySQL/MariaDB ORM
make -j$(nproc) && sudo make install

# 验证安装
drogon_ctl version  # 输出：1.9.6
drogon_ctl create project calcite  # 生成标准项目结构
```

**踩坑 1：MariaDB 连接库缺失**
- 现象：CMake 配置时提示 `MySQL not found`；
- 原因：Ubuntu 默认安装 `libmysqlclient-dev`，但 Drogon 需要 `libmariadb-dev`；
- 解决：`sudo apt install libmariadb-dev libmariadb-dev-compat`。

**踩坑 2：jsoncpp 版本冲突**
- 现象：编译时 `Json::Value` 类部分方法未定义；
- 原因：系统同时安装了 jsoncpp 1.7.4（apt 默认）与 1.9.5（手动编译），CMake 链接了旧版本；
- 解决：卸载 apt 版本，强制指定 `JSONCPP_INCLUDE_DIR` 与 `JSONCPP_LIBRARY` 为手动编译路径。

#### 2.2 最小可运行服务验证

完成 `main.cc` 与第一个测试控制器 `TestController`：

```cpp
// controllers/TestController.h
#pragma once
#include <drogon/HttpController.h>
using namespace drogon;

class TestController : public drogon::HttpController<TestController> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(TestController::hello, "/api/test", Get);
    METHOD_LIST_END

    void hello(const HttpRequestPtr &req,
               std::function<void(const HttpResponsePtr &)> &&callback) {
        auto resp = HttpResponse::newHttpJsonResponse({{"hello", "world"}});
        callback(resp);
    }
};
```

编译并运行：

```bash
mkdir build && cd build
cmake ..
make -j$(nproc)
./calcite
# 输出：202X-XX-XX HH:MM:SS.mmm [INFO] Server started on 0.0.0.0:8888
```

使用 `curl` 验证：

```bash
curl http://localhost:8888/api/test
# 输出：{"hello":"world"}
```

**性能基线**：在 Hello World 场景下，使用 `wrk` 测得单核 QPS 约 82,000，为后续优化建立基准。

#### 2.3 项目目录初始化

基于上周的架构设计，建立完整目录结构：

```bash
cd calcite_server/calcite
mkdir -p controllers services models utils filters test
mkdir -p plugins  # 预留插件目录（导师建议）
touch main.cc CMakeLists.txt config.json
# models/ 目录暂时为空，待数据库设计完成后由 drogon_ctl 生成
```

### 三、Web 前端工程搭建

#### 3.1 Vite 工程初始化与配置

```bash
cd calcite_web
npm create vite@latest calcite-web -- --template vue
cd calcite-web
npm install
```

**关键配置项**：

```javascript
// vite.config.js
import { defineConfig } from 'vite'
import vue from '@vitejs/plugin-vue'
import { resolve } from 'path'

export default defineConfig({
  plugins: [vue()],
  resolve: {
    alias: {
      '@': resolve(__dirname, 'src'),
    },
  },
  server: {
    port: 5173,
    proxy: {
      '/api': {
        target: 'http://localhost:8888',
        changeOrigin: true,
        // rewrite: (path) => path.replace(/^\/api/, ''), // 后端已带 /api 前缀，无需 rewrite
      },
    },
  },
  build: {
    chunkSizeWarningLimit: 1000,
    rollupOptions: {
      output: {
        manualChunks: {
          'element-plus': ['element-plus'],
          'md-editor': ['md-editor-v3'],
        },
      },
    },
  },
})
```

**配置说明**：
- `proxy`：开发阶段将 `/api` 转发至后端 `localhost:8888`，解决 CORS 问题；
- `manualChunks`：将 Element Plus 与 md-editor-v3 拆分为独立 chunk，避免首屏加载过大；
- `chunkSizeWarningLimit`：调整为 1000KB，因为 md-editor-v3 含编辑器完整功能，体积较大属预期内。

#### 3.2 依赖安装与主题系统预研

```bash
npm install element-plus md-editor-v3 axios vue-router@4 pinia
npm install -D sass
```

Element Plus 采用「按需引入」策略（非完整引入），通过 `unplugin-vue-components` 与 `unplugin-auto-import` 实现：

```javascript
// vite.config.js（补充）
import AutoImport from 'unplugin-auto-import/vite'
import Components from 'unplugin-vue-components/vite'
import { ElementPlusResolver } from 'unplugin-vue-components/resolvers'

export default defineConfig({
  plugins: [
    vue(),
    AutoImport({
      resolvers: [ElementPlusResolver()],
    }),
    Components({
      resolvers: [ElementPlusResolver()],
    }),
  ],
  // ...
})
```

主题系统预研：在 `src/styles/theme.js` 中定义 Everforest 深色主题色板，验证 CSS 变量注入方案可行。

#### 3.3 第一个页面验证

完成 `App.vue` 骨架与 `main.js` 入口，运行开发服务器：

```bash
npm run dev
# VITE v7.2.4  ready in 345 ms
# ➜  Local:   http://localhost:5173/
# ➜  Network: http://192.168.x.x:5173/
```

浏览器访问 `http://localhost:5173`，确认 Element Plus 按钮组件渲染正常，主题色已生效。

### 四、Android 工程搭建

#### 4.1 Gradle Version Catalog 配置

Android 工程采用 Gradle Kotlin DSL + Version Catalog 管理依赖，以下为 `libs.versions.toml` 核心内容：

```toml
[versions]
kotlin = "2.0.21"
agp = "8.13.0"
room = "2.7.1"
navigation = "2.8.0"
retrofit = "2.11.0"
okhttp = "4.12.0"
workmanager = "2.10.0"
coil = "2.5.0"
markwon = "4.6.2"
datastore = "1.1.4"

[libraries]
# AndroidX Core
androidx-core-ktx = { module = "androidx.core:core-ktx", version = "1.15.0" }
androidx-appcompat = { module = "androidx.appcompat:appcompat", version = "1.7.0" }
androidx-constraintlayout = { module = "androidx.constraintlayout:constraintlayout", version = "2.2.1" }

# Room
room-runtime = { module = "androidx.room:room-runtime", version.ref = "room" }
room-ktx = { module = "androidx.room:room-ktx", version.ref = "room" }
room-compiler = { module = "androidx.room:room-compiler", version.ref = "room" }

# Navigation
navigation-fragment = { module = "androidx.navigation:navigation-fragment-ktx", version.ref = "navigation" }
navigation-ui = { module = "androidx.navigation:navigation-ui-ktx", version.ref = "navigation" }

# Network
retrofit = { module = "com.squareup.retrofit2:retrofit", version.ref = "retrofit" }
retrofit-gson = { module = "com.squareup.retrofit2:converter-gson", version.ref = "retrofit" }
okhttp-logging = { module = "com.squareup.okhttp3:logging-interceptor", version.ref = "okhttp" }

# WorkManager
workmanager = { module = "androidx.work:work-runtime-ktx", version.ref = "workmanager" }

# DataStore
datastore-preferences = { module = "androidx.datastore:datastore-preferences", version.ref = "datastore" }

# Image & Markdown
coil = { module = "io.coil-kt:coil", version.ref = "coil" }
markwon-core = { module = "io.noties.markwon:core", version.ref = "markwon" }

[bundles]
room = ["room-runtime", "room-ktx"]
navigation = ["navigation-fragment", "navigation-ui"]
network = ["retrofit", "retrofit-gson", "okhttp-logging"]

[plugins]
android-application = { id = "com.android.application", version.ref = "agp" }
kotlin-android = { id = "org.jetbrains.kotlin.android", version.ref = "kotlin" }
ksp = { id = "com.google.devtools.ksp", version = "2.0.21-1.0.27" }
navigation-safeargs = { id = "androidx.navigation.safeargs.kotlin", version.ref = "navigation" }
```

#### 4.2 模块 build.gradle.kts 关键配置

```kotlin
// app/build.gradle.kts
plugins {
    alias(libs.plugins.android.application)
    alias(libs.plugins.kotlin.android)
    alias(libs.plugins.ksp)
    alias(libs.plugins.navigation.safeargs)
}

android {
    namespace = "com.calcite.notes"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.calcite.notes"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "1.0"
    }

    buildFeatures {
        viewBinding = true
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_11
        targetCompatibility = JavaVersion.VERSION_11
    }

    kotlinOptions {
        jvmTarget = "11"
    }
}

dependencies {
    implementation(libs.androidx.core.ktx)
    implementation(libs.androidx.appcompat)
    implementation(libs.androidx.constraintlayout)
    implementation(libs.bundles.room)
    ksp(libs.room.compiler)
    implementation(libs.bundles.navigation)
    implementation(libs.bundles.network)
    implementation(libs.workmanager)
    implementation(libs.datastore.preferences)
    implementation(libs.coil)
    implementation(libs.markwon.core)
}
```

#### 4.3 包结构初始化

```
com.calcite.notes/
├── MainActivity.kt
├── data/
│   ├── local/
│   │   ├── AppDatabase.kt
│   │   ├── DataStoreModule.kt
│   │   └── entity/
│   ├── remote/
│   │   ├── ApiService.kt
│   │   └── AuthInterceptor.kt
│   ├── repository/
│   └── sync/
├── model/
├── ui/
│   ├── login/
│   ├── register/
│   ├── home/
│   └── main/
└── utils/
```

#### 4.4 首次编译验证

```bash
./gradlew build
# BUILD SUCCESSFUL in 2m 15s
```

**踩坑**：KSP 版本与 Kotlin 版本不匹配（最初使用 KSP 1.0.25 配 Kotlin 2.0.21），导致 Room 注解处理失败；升级至 `2.0.21-1.0.27` 后解决。

### 五、Git 子模块结构配置

#### 5.1 子模块初始化策略

三个子项目（后端、Web、Android）均已有独立仓库，需在根仓库中聚合。采用 Git 子模块（submodule）而非 Monorepo，理由是：
- 各端技术栈差异大（C++ / JS / Kotlin），构建工具与 CI/CD 策略完全不同；
- 子模块允许各端独立版本控制，避免不必要的全量提交；
- 毕业设计后期可能需要将某一端开源，子模块结构更利于拆分。

#### 5.2 子模块配置命令记录

```bash
cd /path/to/calcite  # 根仓库
git submodule add <server-repo-url> calcite_server
git submodule add <web-repo-url> calcite_web
git submodule add <android-repo-url> calcite_android

git submodule update --init --recursive

# 写入 .gitmodules
[submodule "calcite_server"]
    path = calcite_server
    url = <server-repo-url>
[submodule "calcite_web"]
    path = calcite_web
    url = <web-repo-url>
[submodule "calcite_android"]
    path = calcite_android
    url = <android-repo-url>
```

#### 5.3 根仓库文档初始化

根仓库仅负责聚合与文档管理，初始化以下文件：

- `Readme.md`：项目总览、三端简介、快速启动指引；
- `AGENTS.md`：面向 AI Coding Agent 的架构说明与技术栈速查；
- `commands.sh`：常用 Git 子模块操作命令速查；
- `.gitignore`：忽略 IDE 配置文件与临时文件。

### 六、开发工具链统一

为保证三端开发体验一致，本周统一以下工具链：

| 工具 | 后端 | Web | Android | 说明 |
|------|------|-----|---------|------|
| IDE | VS Code + CMake Tools | VS Code + Volar | Android Studio | 统一代码风格插件 |
| 代码风格 | clang-format | ESLint + Prettier | ktlint | 提交前自动格式化 |
| HTTP 测试 | curl + Postman | Postman | Postman | 共享 Collection |
| 版本控制 | Git | Git | Git | 统一 .gitignore 模板 |
| 文档 | Markdown | Markdown | Markdown | 统一存放于各模块 docs/ |

### 七、本周问题与解决

**问题 1：Vite 代理与 Drogon 的 CORS 预检请求**
- 现象：Web 前端调用 `/api/test` 时，浏览器发送 OPTIONS 预检请求，Drogon 默认返回 404；
- 解决：在 Drogon `config.json` 中配置 CORS 过滤器，允许 `OPTIONS` 方法与前端开发服务器 origin `http://localhost:5173`。

**问题 2：Android DataStore 与 Room 的协程作用域冲突**
- 现象：初稿中计划在 `Application.onCreate()` 中初始化 DataStore 与 Room，但两者均需 `CoroutineScope`，而 `Application` 无生命周期感知的 Scope；
- 解决：为 DataStore 创建全局 `GlobalScope`（仅用于轻量读写），Room 数据库构建使用 `Room.databaseBuilder` 的同步 API（实际 IO 发生在首次查询时，由 Room 内部管理）。

**问题 3：CMake 与 Drogon 的 FindPackage 路径**
- 现象：在其他机器上克隆后端仓库后，CMake 找不到 Drogon 安装路径；
- 解决：在 `CMakeLists.txt` 中增加 `find_package(Drogon CONFIG REQUIRED)`，并要求团队成员统一安装路径（`/usr/local`），后续可通过 Docker 镜像固化环境。

### 八、里程碑验证

本周结束时，三端均达到「Hello World」级别可运行状态：

| 端 | 验证命令 | 验证结果 |
|----|----------|----------|
| 后端 | `curl http://localhost:8888/api/test` | 返回 `{"hello":"world"}`，wrk QPS 82k |
| Web | `npm run dev` → 浏览器访问 | 页面渲染正常，主题色生效，代理可用 |
| Android | `./gradlew build` → 模拟器运行 | APK 编译成功，MainActivity 白屏启动（无业务 UI） |

### 九、会议记录

**202X-XX-XX 环境搭建同步会**
- 参与人：本人（三端负责人）
- 议题：确认三端开发环境均可独立运行
- 自查结论：
  1. 后端 Drogon 编译通过，但需在其他机器上复现安装流程，验证文档完整性；
  2. Web 端 Vite 代理配置正确，但生产构建尚未验证；
  3. Android 端 KSP 版本兼容性问题已解决，需记录到 `docs/` 供后续参考；
  4. Git 子模块拉取命令需写入根仓库 `Readme.md`，避免新成员踩坑。
- 下一步：进入第 6 周数据库设计，同步开始后端的 `drogon_ctl` 模型生成工具预研。

---

## 工作记录（精简版）

### 一、接口契约

完成 8 模块 34 个接口的契约定义，采用三方会签：后端出稿 → Web 审调用便利性 → Android 审响应体积 → 后端定稿。

**关键决策**：
- 列表分页：简单列表（标签/文件夹）不分页，复杂列表（笔记）默认 page_size=20
- 详情扁平化：笔记详情不含嵌套 tags/files，独立接口获取，降低复杂度
- 文件上传：`multipart/form-data`，避免 Base64 膨胀

### 二、后端环境搭建

- Drogon v1.9.6 源码编译安装
- 踩坑：`libmariadb-dev` 替代 `libmysqlclient-dev`；jsoncpp 1.7.4/1.9.5 版本冲突，卸载 apt 版指定手动编译路径
- 最小服务验证：`curl /api/test` 返回 `{"hello":"world"}`，wrk 单核 82k QPS

### 三、Web 环境搭建

- Vue 3 + Vite 初始化，`/api` 代理至 `localhost:8888`
- Element Plus 按需引入，`md-editor-v3` 集成
- 代码分割：Element Plus 与 md-editor-v3 拆为独立 chunk

### 四、Android 环境搭建

- Kotlin 2.0.21 + AGP 8.13.0 + Gradle Version Catalog
- Room KSP `2.0.21-1.0.27`（踩坑：KSP 与 Kotlin 版本必须匹配）
- 首编通过，`minSdk=24`，`compileSdk=36`

### 五、Git 子模块

根仓库聚合三端独立仓库，各端独立版本控制。初始化 `Readme.md`、`AGENTS.md`、`.gitignore`。

### 六、工具链统一

| 维度 | 选型 |
|------|------|
| 代码风格 | clang-format / ESLint+Prettier / ktlint |
| HTTP 测试 | Postman 共享 Collection |
| 文档 | 各模块 `docs/` Markdown |

### 七、里程碑验证

| 端 | 状态 |
|----|------|
| 后端 | `curl /api/test` 通，82k QPS |
| Web | `npm run dev` 通，主题/代理正常 |
| Android | `./gradlew build` 通，APK 可启动 |

### 八、会议备忘

自查结论：后端安装流程需在其他机器复现验证；Web 生产构建待验证；KSP 踩坑需记入文档；子模块拉取命令写入根仓库 Readme。
