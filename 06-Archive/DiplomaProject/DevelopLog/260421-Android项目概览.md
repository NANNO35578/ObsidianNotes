项目探索与 `AGENTS.md` 编写已完成。

我全面分析了该项目的结构、配置和源码，主要发现如下：

**项目定位**：Calcite Notes Android 是一款 Kotlin 编写的笔记应用，采用 **MVVM + Repository + 离线优先** 架构。

**技术栈**：
- Android Gradle Plugin 8.13.0 / Kotlin 2.0.21 / Compile SDK 36
- Navigation Component、ViewBinding、Room 2.7.1、WorkManager 2.10.0
- Retrofit + OkHttp + Gson 处理网络请求
- Markwon + Coil 实现 Markdown 渲染
- DataStore Preferences 存储 Token 与状态

**架构特点**：
- 所有数据变更遵循“先写服务端 API，成功后再回写本地 Room”的原则
- 无网络时禁止本地伪造写入，直接返回错误，保障数据一致性
- `SyncWorker` 每 10 分钟全量拉取服务端数据覆盖本地
- 左右双抽屉布局：左抽屉为树形笔记列表，右抽屉为标签与文件管理

**测试现状**：目前仅有 Android Studio 模板生成的占位测试（JUnit 4 + Espresso），业务逻辑尚未覆盖。

**安全注意点**：
- 后端地址硬编码为 `http://127.0.0.1:8888`，且开启了明文 HTTP 传输
- JWT Token 以明文存储在 DataStore 中，未加密

我已经将上述所有发现以及更详细的模块划分、数据流、构建命令、业务逻辑速查等内容，按照项目中文注释的语言习惯，完整地写入了项目根目录的 **`AGENTS.md`** 文件中。


目前后端API已更新, 现有API以./docs/api.md为主.
请检查项目, 找出需要修改的地方, 列出修改步骤.
至少有以下部分需要修改:
1. 标签系统: 不在支持自定义标签, 标签由后端自动生成, 用户仅能刷新
2. 后端搜索支持*搜索自己/公开笔记*.
3. 对于公开笔记, 应该有个*预览界面*, 同时支持点赞/收藏, 应当向后端发送view/like/collect等请求.
4. 原标签管理页面可考虑修改为*笔记信息界面*
	- 对于自己笔记, 能够看到笔记标题, 摘要(summary, 可修改, 最长30字), 创建时间, 修改时间, 标签(附带一个刷新按钮), 删除按钮
	- 对于公开笔记, 笔记标题, 摘要(summary), 创建时间, 修改时间, 标签, 作者id
5. 获取*推荐按钮*, 点击后显示推荐的笔记列表


````markdown title:"PLAN"
# Calcite Notes Android 后端 API 升级实施计划

## 背景

后端 API 已升级，新版 API 以 `docs/api.md` 为准。核心变化：
1. **标签系统重构**：废弃旧版全局标签 CRUD 接口，标签改由后端 AI 自动生成（`POST /api/notes/tags/ai`），用户仅可查看和触发重新生成。
2. **笔记新增公开属性与交互**：笔记增加 `author_id`、`is_public`、`summary`、`like_count`、`collect_count`、`has_liked`、`has_collected` 等字段；新增 view/like/collect/unlike/uncollect 接口。
3. **搜索增强**：`GET /api/note/search` 新增 `is_public` 参数，支持搜索公开笔记。
4. **新增推荐接口**：`GET /api/recommend/notes` 返回个性化推荐公开笔记列表。

## 目标

完成以下 5 大模块改造：
1. 标签系统：移除自定义标签，改为 AI 自动生成、只读展示、支持手动触发刷新。
2. 搜索增强：支持筛选"自己的笔记"或"公开笔记"。
3. 公开笔记预览：新建只读预览界面，支持点赞/收藏，自动发送 view 请求。
4. 右抽屉改造：原"标签+文件"双 Tab 改为"笔记信息+文件"双 Tab，笔记信息页展示标题、摘要（可编辑，限 30 字）、创建/修改时间、标签（带 AI 刷新按钮）、删除按钮（仅自己笔记）或作者 ID（仅公开笔记）。
5. 推荐功能：底部导航新增"推荐"入口，展示推荐笔记列表，点击跳转到预览界面。

## 技术约束

- 保持现有 **MVVM + Repository + Room + Retrofit** 架构不变。
- Room 数据库当前 **version = 1**，新增字段需要 **Migration 1→2**。
- 当前用户 ID 尚未持久化，需要扩展 DataStore。
- 写操作遵循"先调 API，成功后更新 Room"的离线优先原则。

---

## 详细实施步骤

### Phase 1 — 数据模型与数据库层

#### 1.1 更新笔记数据模型 (`model/NoteModels.kt`)
- `NoteDetail` 新增字段：`author_id: Long`、`is_public: Int`（或 Boolean）、`like_count: Int`、`collect_count: Int`、`view_count: Int`、`has_liked: Boolean`、`has_collected: Boolean`。
- `UpdateNoteRequest` 新增 `summary: String?` 和 `is_public: Boolean?`（若编辑器需要支持设置公开状态）。

#### 1.2 更新搜索数据模型 (`model/SearchModels.kt`)
- `SearchResultItem` 新增 `author_id: Long`、`is_public: Int` 等字段，与后端搜索返回对齐。

#### 1.3 重构标签数据模型 (`model/TagModels.kt`)
- **删除** 废弃模型：`CreateTagRequest`、`UpdateTagRequest`、`DeleteTagRequest`、`BindTagRequest`、`TagCreateData`。
- **新增** `AiGenerateTagsRequest`（若需 Body 传参）或直接使用 Query 参数。
- `Tag` 类本身保留（`id`, `name`, `created_at`）。

#### 1.4 Room 实体升级 (`data/local/entity/NoteEntity.kt`)
- 新增字段：`authorId: Long = 0L`、`isPublic: Boolean = false`。
- 可选：新增 `summary` 字段（若本地需要缓存摘要编辑）。

#### 1.5 数据库迁移 (`data/local/database/AppDatabase.kt`)
- 版本号提升到 **2**。
- 编写 `Migration(1, 2)`：使用 `ALTER TABLE notes ADD COLUMN ...` 添加 `authorId`（INTEGER, 默认 0）和 `isPublic`（INTEGER, 默认 0）。

#### 1.6 扩展 DataStore 存储用户 ID (`data/local/AppDataStore.kt`)
- 新增 `USER_ID_KEY`。
- 提供 `userId: Flow<Long?>`、`saveUserId(userId: Long)`、`clearUserId()`。
- 在 `clearAll()` 中一并清除 userId。

#### 1.7 登录/注册时保存用户 ID
- 在 `LoginViewModel` 和 `RegisterViewModel` 登录/注册成功后，将 `LoginData.user_id` 或 `RegisterData.user_id` 写入 DataStore。

---

### Phase 2 — 网络 API 层 (`data/remote/ApiService.kt`)

#### 2.1 废弃旧标签接口
- 删除：`createTag`、`updateTag`、`deleteTag`、`getTagList`、`bindTags` 及其导入的 Request 类。

#### 2.2 新增/修改笔记相关接口
- **修改** `searchNotes`：新增 `@Query("is_public") isPublic: Int? = null`。
- **新增** `getNoteTags(@Query("id") noteId: Long): ApiResponse<List<Tag>>`。
- **新增** `aiGenerateTags(@Query("id") noteId: Long): ApiResponse<List<Tag>>`（后端文档写"路径参数"但 URL 无 `{id}`，按 Query 处理，若实际为 Body 则再调整）。
- **新增** `viewNote(@Body request: ViewNoteRequest)`。
- **新增** `likeNote(@Body request: LikeNoteRequest)`。
- **新增** `collectNote(@Body request: CollectNoteRequest)`。
- **新增** `unlikeNote(@Query("note_id") noteId: Long)`（DELETE）。
- **新增** `uncollectNote(@Query("note_id") noteId: Long)`（DELETE）。
- **新增** `getRecommendNotes(@Query("page") page: Int = 1, @Query("page_size") pageSize: Int = 10): ApiResponse<List<RecommendNoteItem>>`。
- **新增** `getHotTags(): ApiResponse<List<HotTagItem>>`。

#### 2.3 新建/修改请求/响应模型
- 在 `NoteModels.kt` 中新增：`ViewNoteRequest`、`LikeNoteRequest`、`CollectNoteRequest`。
- 新增 `RecommendModels.kt`：`RecommendNoteItem`（字段：`id`, `title`, `summary`, `created_at`, `updated_at`，可与 `Note` 复用或独立）。
- 新增 `HotTagItem`（`tag: String`, `count: Int`），可放在 `TagModels.kt` 或独立文件。

---

### Phase 3 — Repository 层

#### 3.1 重构 `TagRepository.kt`
- **删除**：`createTag`、`updateTag`、`deleteTag`、`bindTags`、`syncAllTags`、`getAllTagsFromRemote`、`getAllTags`。
- **保留/改造**：`getTagsByNoteFromRemote` → 改为调用新接口 `apiService.getNoteTags(noteId)`。
- **新增**：`aiGenerateTags(context, noteId): Result<List<Tag>>`，成功后回写 `noteTagDao`。
- 本地标签表不再维护"全局列表"，仅维护"笔记-标签"绑定关系。

#### 3.2 增强 `NoteRepository.kt`
- 所有涉及 `NoteEntity` 的转换，补充 `authorId` 和 `isPublic` 字段映射。
- **新增**：
  - `viewNote(context, noteId)`
  - `likeNote(context, noteId)`
  - `collectNote(context, noteId)`
  - `unlikeNote(context, noteId)`
  - `uncollectNote(context, noteId)`
  - `getRecommendNotes(context, page, pageSize)`
- `updateNote` 支持传入 `summary` 和 `is_public`。

#### 3.3 增强 `SearchRepository.kt`
- `search(keyword, isPublic, from, size)`：将 `isPublic` 透传给 API。

---

### Phase 4 — 右抽屉改造：笔记信息界面

#### 4.1 重写布局 `res/layout/fragment_tool_panel.xml`
- **Tab 1（标签 → 笔记信息）**：
  - 标题（TextView，只读）。
  - 摘要（EditText，可编辑，`maxLength=30`，仅自己笔记时 enable）。
  - 创建时间、修改时间（TextView，只读）。
  - 标签区域（ChipGroup，只读展示）。
  - AI 刷新标签按钮（Button/TextButton，图标+文字"AI 生成标签"）。
  - 删除按钮（Button，红色，仅自己笔记时显示）。
  - 作者 ID（TextView，仅公开笔记且非自己时显示）。
- **Tab 2（文件）**：保持现有文件上传、筛选、列表展示逻辑不变。

#### 4.2 重写 `ToolPanelViewModel.kt`
- **删除**：`allTags`、`createTag`、`renameTag`、`deleteTag`、`bindTag`、`unbindTag`。
- **新增状态**：
  - `_noteInfo = MutableLiveData<NoteDetail?>()`
  - `_isOwnNote = MutableLiveData<Boolean>()`（根据 `author_id == currentUserId` 判断）
  - `_currentNoteTags = MutableLiveData<List<Tag>>()`（当前笔记标签，不再来自交叉表 Flow，而是主动拉取）
- **新增方法**：
  - `loadNoteInfo(noteId)`：调 `noteRepository.getNoteDetail` 获取完整信息。
  - `updateSummary(noteId, summary)`：调 `noteRepository.updateNote` 仅更新 summary。
  - `deleteNote(noteId)`：调 `noteRepository.deleteNote`。
  - `refreshTags(noteId)`：调 `tagRepository.aiGenerateTags`，成功后重新拉取标签。
  - `loadNoteTags(noteId)`：调 `tagRepository.getTagsByNoteFromRemote` 或本地缓存。
- **保留**：文件上传、删除、状态筛选逻辑不变。

#### 4.3 重写 `ToolPanelFragment.kt`
- `onResume` / `setNoteId` 时：除了 `setNoteId`，还要调用 `viewModel.loadNoteInfo(noteId)` 和 `viewModel.loadNoteTags(noteId)`。
- `renderNoteInfo()`：根据 `_noteInfo` 和 `_isOwnNote` 渲染 UI。
  - 自己笔记：摘要 EditText 可编辑，显示"保存摘要"按钮（或失焦自动保存）、显示"删除笔记"按钮。
  - 他人公开笔记：摘要只读，隐藏删除按钮，显示作者 ID。
- `renderTags()`：仅展示当前笔记的标签 Chip（只读，无关闭图标、无点击绑定逻辑）。
- 点击"AI 生成标签" → `viewModel.refreshTags(currentNoteId)`。
- 点击"删除笔记" → 确认对话框 → `viewModel.deleteNote()` → 成功后通知主界面关闭 Editor。

---

### Phase 5 — 搜索增强

#### 5.1 修改布局 `res/layout/fragment_search.xml`
- 在搜索框下方增加一行筛选控件：
  - SwitchMaterial 或 Chip："仅搜索公开笔记"。
  - 默认关闭（搜索自己的笔记）。

#### 5.2 修改 `SearchViewModel.kt`
- 新增 `_isPublicSearch = MutableLiveData(false)`。
- `search(keyword)` 中透传 `isPublic = if (_isPublicSearch.value == true) 1 else 0`（或 null）。
- 提供 `togglePublicSearch(isPublic: Boolean)`。

#### 5.3 修改 `SearchFragment.kt`
- 绑定 Switch 的切换事件到 `viewModel.togglePublicSearch()`。
- **点击搜索结果跳转逻辑改造**：
  - 从 `SearchResultItem` 中获取 `author_id`。
  - 需要获取当前用户 ID（可从 DataStore 或 Shared ViewModel 读取）。
  - 若 `author_id == currentUserId` → 跳转 `NoteEditorFragment`（现有逻辑）。
  - 若 `author_id != currentUserId` → 跳转新建的 `NotePreviewFragment`。

---

### Phase 6 — 公开笔记预览界面

#### 6.1 新建布局 `res/layout/fragment_note_preview.xml`
- 顶部：标题（TextView，大字号）。
- 次级信息：作者 ID、创建时间、修改时间、标签 ChipGroup（只读）。
- 中部：Markdown 渲染内容（使用 Markwon + Coil，与 Editor 预览一致）。
- 底部操作栏：
  - 点赞按钮（ToggleButton 或 Chip）+ 点赞数。
  - 收藏按钮（ToggleButton 或 Chip）+ 收藏数。
- （可选）摘要展示区域。

#### 6.2 新建 `NotePreviewViewModel.kt`
- 依赖：`NoteRepository`、`TagRepository`、`AppDataStore`。
- 状态：`_noteDetail`、`_tags`、`_likeCount`、`_collectCount`、`_hasLiked`、`_hasCollected`。
- 方法：
  - `loadNote(noteId)`：调 `noteRepository.getNoteDetail`，加载成功后自动调 `viewNote()` 发送浏览请求。
  - `loadTags(noteId)`：调 `tagRepository.getTagsByNoteFromRemote`。
  - `toggleLike()` / `toggleCollect()`：根据当前状态调 like/unlike 或 collect/uncollect，成功后刷新本地状态。

#### 6.3 新建 `NotePreviewFragment.kt`
- 接收参数 `noteId`。
- 使用 Markwon 渲染 Markdown 内容（只读）。
- 绑定点赞/收藏按钮到 ViewModel。
- 进入时自动发送 view 请求（由 ViewModel 处理）。

#### 6.4 更新导航图 `nav_graph.xml`
- 新增 `<fragment android:id="@+id/notePreviewFragment" android:name="...NotePreviewFragment">`。
- `SearchFragment` 和 `RecommendFragment` 点击条目时可跳转至此。

---

### Phase 7 — 推荐功能

#### 7.1 修改底部导航菜单 `res/menu/bottom_nav_menu.xml`
- 新增 item：`android:id="@+id/menu_recommend"`，title="推荐"，使用新图标（或复用现有图标）。

#### 7.2 新建布局 `res/layout/fragment_recommend.xml`
- 顶部：标题"推荐笔记"。
- 中部：RecyclerView，展示推荐列表。
- 每项布局（可复用或新建 `item_recommend_note.xml`）：标题、摘要、创建时间。
- 支持下拉刷新（SwipeRefreshLayout）。

#### 7.3 新建 `RecommendViewModel.kt`
- 依赖：`NoteRepository`。
- 状态：`_recommendList = MutableLiveData<List<RecommendNoteItem>>()`、`_isLoading`。
- 方法：`loadRecommendations(page = 1)` 调 `noteRepository.getRecommendNotes`。

#### 7.4 新建 `RecommendFragment.kt`
- RecyclerView + Adapter，点击条目跳转 `NotePreviewFragment`（带 `noteId`）。

#### 7.5 更新导航图 `nav_graph.xml`
- 新增 `<fragment android:id="@+id/recommendFragment" android:name="...RecommendFragment">`。

#### 7.6 更新 `MainActivity.kt`
- `bottomNav.setOnItemSelectedListener` 中处理 `R.id.menu_recommend`：
  - `navController.navigate(R.id.recommendFragment)`。
- 返回键处理：将 `recommendFragment` 和 `notePreviewFragment` 加入判断，不直接 `finish()`，而是返回 `noteEditorFragment` 或 `homeFragment`。

---

### Phase 8 — 后台同步与全局适配

#### 8.1 更新 `SyncWorker.kt`
- `syncTags` 方法改造：
  - 删除对 `getAllTagsFromRemote` 的调用（已废弃）。
  - 改为：遍历本地所有笔记，逐个调用 `tagRepository.getTagsByNoteFromRemote(note.id)`，更新 `noteTagDao` 和 `tagDao`。
- `syncNotes`：确保拉取详情时新字段（`author_id`、`is_public`）写入 Room。

#### 8.2 `NoteEditorViewModel.kt` 微调
- `updateNote` 和 `saveNote` 支持 `summary` 字段同步保存（若编辑器界面不编辑 summary，则保持原值）。
- 若需要在编辑器内设置笔记公开状态，可在 `NoteEditorFragment` 添加 Toggle（可选，本计划暂不在编辑器中加，由右抽屉负责）。

#### 8.3 `NoteEditorFragment.kt` 微调
- 返回键/生命周期逻辑保持不变。
- 标题修改对话框保持不变。

---

## 文件变更清单

### 修改文件（≈ 18 个）
| 文件 | 变更类型 |
|------|---------|
| `model/NoteModels.kt` | 新增字段、Request 类 |
| `model/TagModels.kt` | 删除废弃类，保留 Tag |
| `model/SearchModels.kt` | 新增字段 |
| `model/AuthModels.kt` / `model/UserModels.kt` | 无改动（已有 user_id） |
| `data/local/AppDataStore.kt` | 新增 user_id 存储 |
| `data/local/entity/NoteEntity.kt` | 新增 authorId、isPublic |
| `data/local/database/AppDatabase.kt` | version=2 + Migration |
| `data/remote/ApiService.kt` | 大量接口增删改 |
| `data/repository/TagRepository.kt` | 重构：移除 CRUD，新增 AI 生成 |
| `data/repository/NoteRepository.kt` | 新增交互方法、推荐方法 |
| `data/repository/SearchRepository.kt` | 新增 isPublic 参数 |
| `data/sync/SyncWorker.kt` | 改造标签同步逻辑 |
| `ui/main/ToolPanelViewModel.kt` | 重构为笔记信息 ViewModel |
| `ui/main/ToolPanelFragment.kt` | 重构为笔记信息 UI |
| `ui/main/SearchViewModel.kt` | 新增公开搜索 |
| `ui/main/SearchFragment.kt` | 新增筛选 UI、跳转判断 |
| `ui/login/LoginViewModel.kt` | 登录成功保存 user_id |
| `ui/register/RegisterViewModel.kt` | 注册成功保存 user_id |
| `MainActivity.kt` | 底部导航推荐入口、返回键 |
| `res/layout/fragment_tool_panel.xml` | 重写为笔记信息布局 |
| `res/layout/fragment_search.xml` | 新增公开筛选 Switch |
| `res/menu/bottom_nav_menu.xml` | 新增"推荐"菜单 |
| `res/navigation/nav_graph.xml` | 新增 Preview、Recommend |

### 新建文件（≈ 8 个）
| 文件 | 说明 |
|------|------|
| `model/RecommendModels.kt` | 推荐笔记 Item、热门标签 Item |
| `model/NoteActionModels.kt`（或合并到 NoteModels.kt） | View/Like/Collect Request |
| `ui/main/NotePreviewFragment.kt` | 公开笔记预览 Fragment |
| `ui/main/NotePreviewViewModel.kt` | 预览 ViewModel |
| `ui/main/RecommendFragment.kt` | 推荐列表 Fragment |
| `ui/main/RecommendViewModel.kt` | 推荐 ViewModel |
| `res/layout/fragment_note_preview.xml` | 预览布局 |
| `res/layout/fragment_recommend.xml` | 推荐列表布局 |
| `res/layout/item_recommend_note.xml` | 推荐列表项布局 |

---

## 风险与注意事项

1. **数据库迁移**：Room version 1→2 必须提供 Migration，否则已安装 App 会崩溃。若开发调试阶段可接受卸载重装，可先做 destructive migration，但生产计划必须写正式 Migration。
2. **当前用户 ID 获取**：`SearchFragment` 跳转判断和 `ToolPanelFragment` 的"自己/他人"判断都依赖 DataStore 中的 user_id。必须在登录/注册流程中正确写入。
3. **标签本地缓存策略变更**：旧逻辑维护全局标签列表和绑定关系；新逻辑下标签依附于笔记，SyncWorker 需要逐个笔记拉取标签，注意循环调用时的性能（笔记数量多时）。
4. **公开笔记的 Room 缓存**：推荐列表和公开搜索结果理论上也可以写入 Room，但为简化实现，推荐列表和公开笔记预览优先走网络，不强制要求本地缓存。
5. **返回栈管理**：新增 `RecommendFragment` 和 `NotePreviewFragment` 后，`MainActivity` 的返回键统一处理需要更新，防止多层 Fragment 堆栈导致返回行为混乱。

````


````markdown title:""
计划文件已撰写完成。由于系统状态变化，我直接在这里为您呈现核心结论：

---

## 需要修改的地方总结

基于新版 `docs/api.md` 与现有代码对比，共涉及 **5 大模块、约 26 个文件** 的变更：

### 1. 标签系统重构
- **废弃接口**：`POST/GET /api/tag/*` 全套 CRUD 及绑定接口在代码中仍被 `ApiService` 和 `TagRepository` 调用，需要全部移除。
- **新增接口**：`GET /api/notes/tags`、`POST /api/notes/tags/ai`。
- **改造点**：`ToolPanelFragment` 当前支持"新建/重命名/删除标签"和"点击绑定"，需改为**只读展示 + AI 刷新按钮**。

### 2. 搜索增强
- `ApiService.searchNotes` 缺少 `is_public` 参数。
- `SearchFragment` 缺少公开/自己笔记的筛选 UI。
- 点击搜索结果后需要根据 `author_id` 判断跳转到**编辑器**（自己笔记）还是**预览界面**（他人公开笔记）。

### 3. 公开笔记预览界面（新建）
- 后端新增 `POST /api/note/view`、`/like`、`/collect` 及对应的 DELETE 取消接口。
- 需要新建 `NotePreviewFragment` + `ViewModel`，支持：
  - Markwon 只读渲染
  - 自动发送 `view` 请求
  - 点赞/收藏按钮及数量展示

### 4. 右抽屉改为笔记信息界面
- 当前 `ToolPanelFragment` 是"标签 Tab + 文件 Tab"。
- 需将**标签 Tab 替换为笔记信息 Tab**：
  - 自己笔记：标题、摘要（可编辑，限 30 字）、创建/修改时间、标签（AI 刷新）、**删除按钮**
  - 公开笔记：标题、摘要（只读）、时间、标签、**作者 ID**
- 文件 Tab 保留。

### 5. 推荐功能（新建）
- 后端新增 `GET /api/recommend/notes`。
- 底部导航需新增"推荐"入口。
- 新建 `RecommendFragment` 展示列表，点击跳转到 `NotePreviewFragment`。

### 附：基础层必须同步修改
- **Room 数据库**：`NoteEntity` 需新增 `authorId`、`isPublic` 字段，**版本 1→2 需 Migration**。
- **DataStore**：当前未存储 `user_id`，需扩展以支持"自己/他人"判断。
- **SyncWorker**：`syncTags` 逻辑需从"拉全局标签列表"改为"按笔记逐个拉取标签"。

---

## 两种实施策略

| 策略 | 说明 | 适用场景 |
|------|------|---------|
| **A. 完整实施（推荐）** | 按上述 5 大模块一次性改造完毕，同步更新 Model、API、Repository、ViewModel、Fragment、布局、导航图 | 希望直接对齐最新后端，功能完整可用 |
| **B. 最小化适配** | 仅修改 `ApiService`、`Repository`、`Model` 和数据库层以恢复前后端连通性；UI 改造（预览、推荐、抽屉）延后 | 急需恢复编译运行，UI 迭代后续再做 |

---

**请告诉我：**
1. 您希望按 **策略 A（完整实施）** 还是 **策略 B（最小化适配）** 进行？
2. 是否有特定模块需要优先处理或调整？
````



策略 A（完整实施） 其中如果搜索字段不包含is_public则后端返回的一定是用户自己的笔记(包含公开), 若~~不~~包含is_public字段则一定是公开笔记(不包含自己公开的). 对于自己的公开笔记, 可以点赞, 可以收藏, 可以显示点赞数, 收藏数(note/detail这个API会返回相关信息)


````markdown title:""
所有修改已完成，编译通过 ✅（`BUILD SUCCESSFUL`）。

---

## 修改总结

### 1. 标签系统重构
- **移除了**旧版全局标签 CRUD 接口（`createTag`/`updateTag`/`deleteTag`/`bindTags`/`getTagList`）
- **新增了** `GET /api/notes/tags` 和 `POST /api/notes/tags/ai` 接口
- `TagRepository` 已重构：不再维护全局标签表，改为按笔记拉取标签、支持 AI 重新生成
- 右抽屉中标签改为**只读展示**，提供"AI 生成标签"按钮

### 2. 搜索增强
- `ApiService.searchNotes` 新增 `is_public` 参数
- 搜索界面新增 **Switch "搜索公开笔记"**
- 搜索结果根据 `author_id` 与当前用户 ID 对比，自动判断跳转到：
  - **自己笔记** → `NoteEditorFragment`（编辑器）
  - **他人公开笔记** → `NotePreviewFragment`（预览界面）
- 搜索行为：
  - 关闭 Switch → 不传 `is_public`，返回**自己的所有笔记**
  - 开启 Switch → 传 `is_public=1`，返回**他人公开笔记**（不含自己的）

### 3. 公开笔记预览界面（新建）
- 新建 `NotePreviewFragment` + `ViewModel` + 布局
- 支持 **Markwon 只读渲染** Markdown 内容
- 进入时**自动发送 `view` 请求**
- 支持 **点赞/收藏** 及取消，实时显示点赞数/收藏数
- 显示作者 ID、创建/修改时间、标签

### 4. 右抽屉改为笔记信息界面
- `ToolPanelFragment` 从"标签+文件"双 Tab 改为"**笔记信息+文件**"双 Tab
- 对于自己笔记：
  - 显示标题、摘要（**可编辑，限 30 字**）、创建/修改时间
  - 可切换**公开/私有**状态
  - 显示标签 + AI 刷新按钮
  - 显示点赞/收藏数，支持点赞/收藏操作
  - 提供**删除按钮**
- 对于他人公开笔记：
  - 显示标题、摘要（只读）、创建/修改时间、**作者 ID**
  - 显示标签（只读）
  - 支持点赞/收藏

### 5. 推荐功能（新建）
- 底部导航新增 **"推荐"入口**
- 新建 `RecommendFragment` + `ViewModel` + 布局
- 调用 `GET /api/recommend/notes` 展示推荐笔记列表
- 点击条目跳转 `NotePreviewFragment`

### 6. 基础层同步升级
- **Room 数据库** `version 1 → 2`，新增 `authorId` 和 `isPublic` 字段，带 Migration
- **DataStore** 新增 `user_id` 存储，登录/注册时自动保存
- **SyncWorker** 标签同步逻辑改为按笔记逐个拉取
- `NoteDetail` 等数据模型新增 `author_id`、`is_public`、`like_count`、`collect_count`、`has_liked`、`has_collected` 等字段

---

## 涉及的主要文件变更

| 类别 | 文件 |
|------|------|
| **新建** | `NotePreviewFragment.kt`, `NotePreviewViewModel.kt`, `RecommendFragment.kt`, `RecommendViewModel.kt`, `RecommendModels.kt`, 3 个新布局 XML |
| **重写** | `ApiService.kt`, `TagRepository.kt`, `ToolPanelFragment.kt`, `ToolPanelViewModel.kt`, `fragment_tool_panel.xml` |
| **大幅修改** | `NoteRepository.kt`, `SearchFragment.kt`, `SearchViewModel.kt`, `MainActivity.kt`, `SyncWorker.kt`, `nav_graph.xml` |
| **其他修改** | `NoteModels.kt`, `SearchModels.kt`, `TagModels.kt`, `NoteEntity.kt`, `AppDatabase.kt`, `AppDataStore.kt`, `LoginViewModel.kt`, `RegisterViewModel.kt`, `RegisterFragment.kt`, `NoteListViewModel.kt`, `bottom_nav_menu.xml`, `fragment_search.xml` |

---

如需进一步调整任何细节（如 UI 样式、字段映射、交互逻辑），请随时告诉我。
````
