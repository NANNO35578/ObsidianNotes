# 开发周志 —— 第20周：Android 工程搭建与认证模块

## 本周 TODO
- [ ] 初始化 Android 工程（Kotlin 2.0.21、AGP 8.13.0、JVM 11）
- [ ] 配置 Gradle Version Catalog（Room、Navigation、Retrofit、WorkManager、Coil、Markwon）
- [ ] 设计 MVVM + Repository 架构，创建基础包结构（data / ui / model / utils）
- [ ] 实现 Room 数据库（AppDatabase、Entity、Dao）与 DataStore Token 存储
- [ ] 实现 Retrofit + OkHttp + AuthInterceptor，定义 ApiService 接口
- [ ] 实现 LoginFragment / RegisterFragment + ViewModel + AuthRepository

## 工作内容概括

本周启动 Android 客户端开发。工程配置与依赖管理采用 Gradle Version Catalog（`libs.versions.toml`），统一版本号。架构严格遵循 MVVM + Repository：
- `data/remote/`：Retrofit 定义 `ApiService`，OkHttp `AuthInterceptor` 从 DataStore 读取 Token 注入请求头；
- `data/local/`：Room 定义 `NoteEntity`、`FolderEntity`、`TagEntity` 等，支持离线浏览；DataStore 存储 Token 与用户偏好；
- `data/repository/`：`AuthRepository` 聚合远程登录与本地 Token 持久化；
- `ui/login/`：`LoginFragment` + `LoginViewModel`，输入校验后调用 Repository，登录成功写入 DataStore 并跳转主界面。

同步完成注册页面与 Navigation 导航图配置，确保登录/注册/主界面之间的跳转符合 Jetpack Navigation 规范。

## 关键产出
- Android 工程骨架与依赖配置
- Room 本地数据库与 DataStore
- Retrofit + OkHttp 网络层
- 登录 / 注册模块（含 UI）

---

## 工作记录（精简版）

### 一、Room 数据库设计

| Entity | 核心字段 | 说明 |
|--------|----------|------|
| `NoteEntity` | `id`, `title`, `content`, `folderId`, `isDeleted`, `updatedAt` | 与后端 note 表对齐，支持离线浏览 |
| `FolderEntity` | `id`, `name`, `parentId` | 树形结构本地缓存 |
| `TagEntity` | `id`, `name` | 标签本地缓存 |
| `NoteTagCrossRef` | `noteId`, `tagId` | 多对多关联 |

**Dao 设计**：`NoteDao` 提供 `getByFolderId`、`searchByKeyword`（本地备用检索）、`insertAll`、`deleteById`；`FolderDao` 提供 `getAll`、`getChildrenByParentId`。

### 二、DataStore Token 存储

```kotlin
// DataStoreModule.kt
val Context.dataStore: DataStore<Preferences> by preferencesDataStore(name = "calcite_prefs")
val TOKEN_KEY = stringPreferencesKey("auth_token")
val USER_ID_KEY = longPreferencesKey("user_id")
```

登录成功后写入 DataStore，后续 `AuthInterceptor` 读取注入请求头。DataStore 替代 SharedPreferences，支持类型安全与协程异步读写。

### 三、Retrofit + OkHttp 配置

```kotlin
// ApiService.kt
interface ApiService {
    @POST("/api/auth/login")
    suspend fun login(@Body req: LoginRequest): ApiResponse<LoginData>
    
    @GET("/api/note/list")
    suspend fun getNoteList(@Query("folder_id") folderId: Long?): ApiResponse<List<NoteItem>>
    // ... 共 34 个接口
}
```

`AuthInterceptor`：从 DataStore 读取 `TOKEN_KEY`，添加 `Authorization: Bearer $token`。

### 四、MVVM 架构验证

以登录模块为例：

```
LoginFragment → LoginViewModel → AuthRepository
                     ↓
              ┌──────┴──────┐
         ApiService      DataStore
              ↓
         Drogon Backend
```

- Fragment 负责 UI 渲染与事件绑定；
- ViewModel 持有 `LiveData<LoginState>`，处理输入校验与协程作用域；
- Repository 聚合远程登录与本地 Token 持久化，上层无感知数据来源。

### 五、本周问题

- **Room 与后端模型字段命名差异**：后端用 snake_case（`folder_id`），Kotlin 用 camelCase（`folderId`）。解决：Gson `@SerializedName("folder_id")` 注解映射。
- **DataStore 读取首次为空**：应用冷启动时 DataStore 读取有延迟，导致首个请求未带 Token。解决：`AuthInterceptor` 中若 Token 为空，同步阻塞读取一次（DataStore 支持 `runBlocking` 首次预热）。

### 六、会议备忘

自查：Android 工程编译通过，Login/Register UI 可输入，登录联调成功（Token 正确写入 DataStore 并注入后续请求）。下一步构建主界面框架。
