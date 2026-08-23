
# Open
- [ ] 数据设置为不为空, 可能导致空指针异常
- [ ] 标签绑定
- [ ] 登陆后不是显示笔记列表而是新建一篇笔记, 导致每次登录都新建了笔记
- [ ] 没有图标
- [ ] 文件树没有图标, 没有根目录笔记
- [ ] 依旧activity问题

按照下方列表修复
1. 实现离线模式, 获取token后存储到本地, 设置过期时间(7天). 若应用打开无网络时并且token有效即可离线模式使用.
2. 离线模式要求: 在连接到服务器时存储所有笔记, 已上传文件列表, 标签列表, 文件夹列表等信息. 同时增加存储当前打开笔记id这一选项, 当用户下次打开应用时打开这个笔记. 离线模式每10min尝试与服务器连接, 若token有效, 则自动更新笔记等信息, 若无效则退出登录.
3. 登录后主页修复: 若登陆后上次存储的当前打开笔记id这一选项有效则打开该笔记. 否则登录后根目录笔记列表, 而不是显示为新建笔记页面, 因为每次登陆后都会新建空笔记. 
4. 左抽屉文件树没有图标, 不易区分文件夹与笔记. 同时文件树不显示根目录笔记. 修改为添加长按文件夹从下方弹出抽屉菜单, 菜单内容: 可在文件夹下新建笔记, 可更改文件夹名称, 删除文件夹等, 有API(./docs/api.md)
5. 右抽屉设计要求: 分标签/已上传文件 两栏, 顶部按钮切换. 标签管理页面: 若当前打开笔记id这一选项无效, 则显示所有标签, 同样长按可对标签进行改名/删除(同样有API), 若id有效, 则标签管理页面上半部分显示当前打开笔记的标签, 下半部分显示全部标签, 点击下半部分标签为笔记添加标签, 长按可改名, 点击上半部分已绑定标签可取消该标签绑定.





````markdown title:""

你是一个资深 Android 工程师，请在【已有项目基础上】实现离线模式 + 数据同步 + UI 修复。

后端 API 文档位于：./docs/api.md（必须严格遵守）

--------------------------------
# 一、核心要求（必须理解）

本阶段重点：

1. 构建完整“离线优先”架构
2. 所有数据必须支持本地存储 + 同步
3. UI 必须基于本地数据驱动（不能直接依赖网络）

--------------------------------
# 二、本地存储（强制要求）

❗必须使用 Room 数据库
表：
- NoteEntity
- TagEntity
- FolderEntity
- FileEntity

## 状态存储（DataStore）
存储：
- token
- token_expire_time（7天）
- current_note_id

--------------------------------

# 三、离线模式（必须实现）
条件：
- 无网络
- token 未过期
👉 行为：
- 所有页面使用本地 Room 数据
- 禁止发起网络请求

--------------------------------

# 四、数据同步机制（必须实现）
❗必须使用 WorkManager
## 同步策略：
每 10 分钟执行：
1. 检查 token 是否有效
   - 无效 → 清除 token → 跳转登录
1. 拉取服务端数据：
   - 笔记
   - 标签
   - 文件
   - 文件夹
1. 更新本地数据库（覆盖或合并）

--------------------------------

# 五、数据流（必须遵守）
UI → ViewModel → Repository → Room
👉 网络数据必须先写入 Room，再由 UI 观察
禁止：
UI 直接使用网络数据

--------------------------------

# 六、登录后行为修复
逻辑：
1. 读取 current_note_id
2. 如果存在：
   → 打开 NoteEditorFragment
3. 否则：
   → 打开根目录笔记列表
❗禁止自动新建空笔记

--------------------------------

# 七、左抽屉优化
## 1. 文件树
必须：
- 区分：
  - 文件夹（图标）
  - 笔记（图标）
- 显示根目录笔记

## 2. 长按菜单（必须）
使用：
BottomSheetDialog
菜单内容：
文件夹：
- 新建笔记
- 重命名
- 删除

笔记：
- 删除
- 重命名

--------------------------------

# 八、右抽屉优化

## 顶部切换
- 标签
- 已上传文件

## 标签管理（复杂逻辑）
### 情况1：无 current_note_id
- 显示全部标签
- 支持：
  - 长按 → 重命名 / 删除

### 情况2：有 current_note_id
UI：
上半部分：
- 当前笔记标签（可点击取消绑定）
下半部分：
- 全部标签（点击绑定）

## 标签操作要求
点击：
- 调用 /api/tag/bind
- 更新 Room
- UI 立即刷新

--------------------------------

# 九、重要补充（必须实现）

## 1. token 过期
- 自动退出登录
- 清空本地数据
## 2. 当前笔记记录
- 每次打开笔记时更新 current_note_id
## 3. 数据一致性

- 本地更新后必须同步远端
- 远端更新后必须刷新本地

--------------------------------

# 十、代码要求

概括输出下方实现思路：
1. Room 数据库设计（Entity + DAO）
2. Repository（含同步逻辑）
3. WorkManager 实现
4. UI 修改代码
5. DataStore 实现
将你的操作在 ./docs 生成260413_offline_sync.md文档



````


# ANS

````markdown title:""
所有任务已完成，项目编译通过（`BUILD SUCCESSFUL`）。下面是本次改动的核心总结：

---

## ✅ 已完成内容

### 1. 基础设施
- **依赖**：`gradle/libs.versions.toml` 与 `app/build.gradle.kts` 中新增 **Room** + **WorkManager** + **KSP** 插件。
- **Room**：创建了 `NoteEntity`、`TagEntity`、`FolderEntity`、`FileEntity`、`NoteTagCrossRef` 及对应 5 个 DAO + `AppDatabase`。
- **DataStore**：将旧的 `TokenDataStore` 升级为 `AppDataStore`，新增存储 `token_expire_time`（7天）和 `current_note_id`。

### 2. Repository 层重构（离线优先）
所有 Repository 同时持有 **ApiService + DAO**：
- **读**：有网络时先调 API → 回写 Room；无网络时直接读 Room（Flow 驱动 UI）。
- **写**：有网络时先调 API → 成功后更新 Room；无网络直接报错，禁止离线伪造。
- `NetworkUtils.isNetworkAvailable(context)` 统一判断网络状态。

### 3. WorkManager 同步
- `SyncWorker` 每 **10 分钟** 执行一次。
- 流程：校验 Token 有效期 → 拉取全量 **笔记（含 content）/ 标签 / 文件夹 / 文件** → 覆盖/合并写入 Room。
- Token 过期时自动清空 DataStore 与所有 Room 表。

### 4. 登录后行为修复
- 登录成功 → 读取 `current_note_id`。
  - `> 0`：携带 `noteId` 跳转 `NoteEditorFragment`。
  - `== 0`：跳转 `HomeFragment`（主界面，左抽屉显示根目录笔记树）。
- **禁止**登录后自动新建空笔记。

### 5. 左抽屉优化（NoteListFragment）
- **图标区分**：文件夹使用 `ic_folder_24`，笔记使用 `ic_note_24`。
- **根目录笔记**：根目录同时展示文件夹 + 笔记。
- **长按菜单**：使用 `BottomSheetDialog`。
  - 文件夹：新建笔记 / 重命名 / 删除
  - 笔记：重命名 / 删除

### 6. 右抽屉优化（ToolPanelFragment）
- **顶部 Tab 切换**：标签 / 已上传文件。
- **标签管理**：
  - **无 `current_note_id`**：显示全部标签，长按支持重命名/删除。
  - **有 `current_note_id`**：上半部分显示"当前笔记标签"（点击取消绑定），下半部分显示"全部标签"（点击绑定），操作均先调 `/api/tag/bind` 再更新 Room。

### 7. Token 过期与数据一致性
- `MainActivity` 启动时校验 Token，无效则踢回登录页。
- `NoteEditorFragment` 打开时自动更新 `current_note_id`；离开编辑器时清 0。
- 退出登录时清空 DataStore + Room 全部数据。

---

## 📄 文档

实现思路与详细架构已写入：

**`./docs/260413_offline_sync.md`**

包含 Room 设计、Repository 同步策略、WorkManager 实现、UI 修改说明、DataStore 实现、关键文件清单等。
````

----


# 离线bug

1. 每次打开应用都要登录, 无论是否能连接到服务器. 也就是说, token存7天有效没有处理好. 再说一遍:若打开应用, token过期了, 让用户重新登录. 若token有效, 则不用登录, 此时如果用户打开应用后后台尝试连接服务器, 若离线, 读取本地数据库构想相应内容, 期间每10min尝试连接一次服务器, 若连接成功, 则更新笔记/文件夹/标签/已上传文件等列表, 并弹窗已更新. 
2. 新设备登录, 不会请求服务器获取当前笔记列表与文件夹列表. 同时, 由文件列表与笔记列表构建成的文件树有问题. 比如:根目录笔记不显示(父文件夹为空或者父文件夹id为0). 
3. 为左侧抽屉添加一个下拉刷新的功能, 通过/api/note/list与/api/folder/list两个API重新构建文件树
4. 用户依旧需要退出多次才能正常回到桌面. 退出设置为这样的: 登录注册界面可以直接退出, 笔记编辑界面可以直接退出. 若已登录, 其余界面均返回到笔记编辑界面退出.

````markdown title:""
你是一个资深 Android 工程师，请在【已有项目基础上】修复以下问题。

必须严格参考：
./docs/api.md

--------------------------------
# 一、启动与 token 逻辑（必须修复）

启动时读取 DataStore：
- token
- token_expire_time（7天）

规则：

1. token不存在或过期 → 进入登录页  
2. token有效 → 直接进入主界面（无需登录）

进入主界面后：

- 有网：请求并更新数据（note/folder/tag/file），提示“已同步”
- 无网：进入离线模式，仅使用 Room 数据

--------------------------------

# 二、定时同步（必须）

使用 WorkManager：
- 每10分钟同步一次
- 若 token 失效 → 清空登录状态并跳转登录页

--------------------------------

# 三、登录后数据加载（必须修复）

登录成功后必须调用：
- /api/note/list
- /api/folder/list

写入 Room 后再进入主界面

--------------------------------

# 四、文件树修复（重点）

规则：

- folder_id == 0 或 null → 根目录
- 根目录笔记必须显示
- 先构建文件夹树，再挂载笔记

UI：
- 文件夹/笔记必须有不同图标

--------------------------------

# 五、左抽屉下拉刷新

使用 SwipeRefreshLayout：

刷新流程：
1. 请求 note + folder
2. 更新 Room
3. 重建文件树
4. 停止刷新动画

--------------------------------

# 六、返回逻辑（必须统一）

- 登录/注册页 → 直接退出
- NoteEditor → 直接退出
- 其他页面 → 返回 NoteEditor

禁止多层返回

--------------------------------

# 七、数据流（必须）

UI → ViewModel → Repository → Room

禁止 UI 直接使用网络数据

--------------------------------

# 八、输出要求

概括实现思路：

1. 启动流程
2. 文件树构建逻辑
3. 返回逻辑实现

在 ./docs 生成260414_bugfix.md

````
# ANS

````markdown title:""
所有修复已完成，项目编译通过（`BUILD SUCCESSFUL`）。以下是改动概要：

---

### 一、启动与 token 逻辑
- `MainActivity` 启动时通过 `UserRepository.isTokenValid()` 校验 DataStore 中的 `token` 与 `token_expire_time`。
- **有效**：清空回退栈并直达 `homeFragment`，同时启动 `SyncWorker`。
- **无效/过期**：停留在 `loginFragment`。
- 进入主界面后，`NoteListViewModel.init` 自动检测网络：有网则调用 `refresh()` 同步并提示“已同步”；无网则仅使用 Room 离线数据。

### 二、定时同步
- `SyncWorker` 保持每 10 分钟执行一次。
- 若 token 失效，Worker 会清空本地数据并发送广播 `com.calcite.notes.ACTION_TOKEN_EXPIRED`。
- `MainActivity` 注册 `BroadcastReceiver`，收到广播后跳转登录页。

### 三、登录后数据加载
- `LoginViewModel.login()` 在登录成功后，先调用：
  - `noteRepository.syncAllNotes()`（`/api/note/list?folder_id=0`）
  - `folderRepository.syncAllFolders()`（BFS 递归 `/api/folder/list`）
- 数据写入 Room 后再导航到主界面。

### 四、文件树修复
- `NoteListViewModel` 改为**全量观察** `folderDao.getAll()` + `noteDao.getAll()`。
- `buildFullTree()` 递归构建：**先文件夹（按名称排序），后笔记（按标题排序）**。
- `folder_id == 0 || null` 统一视为根目录，根目录笔记正常显示。
- 文件夹与笔记的 item 布局已使用不同图标（`ic_folder_24` / `ic_note_24`）。

### 五、左抽屉下拉刷新
- `fragment_note_list.xml` 中用 `SwipeRefreshLayout` 包裹 `RecyclerView`。
- `NoteListFragment` 绑定 `setOnRefreshListener` → 调用 `viewModel.refresh()`。
- 刷新流程：请求 note + folder → 更新 Room → 重建文件树 → 停止动画。

### 六、返回逻辑统一
- `MainActivity` 中统一拦截返回键：
  - **登录/注册页、NoteEditor** → `finish()` 直接退出。
  - **其他页面** → `navigate(R.id.noteEditorFragment, popUpTo=nav_graph)`，清空中间栈，禁止多层返回。

### 七、数据流
- 新增 `MainViewModel` 供 `MainActivity` 使用。
- 底部导航“新建笔记”与 OCR 流程全部收敛到 `MainViewModel` → `Repository` → Room。
- 禁止 UI 直接发起网络请求。

---

### 文档输出
详细实现思路已写入：
- `./docs/260414_bugfix.md`
````


# Open
1. 依旧文件树问题

````markdown title:""
编译通过（`BUILD SUCCESSFUL`）。两个问题均已修复：

### 1. 登录后同步不拉取子文件夹笔记
**根因**：之前 `LoginViewModel` 先 `syncAllFolders` 再读本地 `folderIds` 去拉笔记，但由于时序/Flow 取值问题，实际上后续子文件夹笔记请求没有发出。

**修复方案（按你的思路）**：
- 把笔记拉取直接整合进 `FolderRepository.syncAllFolders()` 的 BFS 过程中：
  - 遍历每个 `pid` 时，**先调用 `apiService.getNoteList(pid)` 拉取该文件夹下的笔记并写入 Room**，再调用 `getFolderList(pid)` 拉取子文件夹。
- `LoginViewModel` 登录成功后只需调用 `folderRepository.syncAllFolders(context)` 即可同时同步完整文件夹树和所有笔记。
- `NoteListViewModel.refresh()` 下拉刷新也复用同一逻辑。

### 2. 返回逻辑退到“新建”的编辑界面
**根因**：`MainActivity` 的 `addOnDestinationChangedListener` 会在**离开任意页面**（包括从编辑器跳转到搜索页）时把 `currentNoteId` 设为 `0L`；且返回拦截跳转 `noteEditorFragment` 时没有带 `noteId` bundle，导致打开了一个空编辑器。

**修复方案**：
- 移除了离开页面就清空 `currentNoteId` 的逻辑（现在只有在明确退出登录或创建新笔记时才会重置）。
- 返回拦截改为读取 `appDataStore.currentNoteId`：
  - 若 `noteId > 0`，携带该 `noteId` 跳转到 `noteEditorFragment`，即回到**最近编辑的笔记**。
  - 若 `noteId == 0`，说明没有最近编辑的笔记，直接 `finish()` 退出应用。



编译通过（`BUILD SUCCESSFUL`）。两个问题均已定位并修复：

### 1. 子文件夹下笔记已拉取但 UI 不更新
**根因**：`NoteListViewModel` 中使用了 `MutableStateFlow(Unit)` 作为 `refreshTrigger`。由于 `Unit` 是单例对象，当 `toggleFolder()` 里执行 `refreshTrigger.value = Unit` 时，新值与旧值相同，`StateFlow` 不会触发 emit，导致 `combine` 不会重新收集，文件树 UI 不会刷新。

**修复**：将 `refreshTrigger` 改为 `MutableStateFlow(0)` 计数器，`toggleFolder()` 中执行 `refreshTrigger.value += 1`。每次文件夹展开/折叠都能确保触发 `combine` 重建文件树并刷新 RecyclerView。

### 2. 登录后未请求标签与文件列表
**根因**：`LoginViewModel` 只注入了 `NoteRepository` 和 `FolderRepository`，登录成功后仅调用了 `folderRepository.syncAllFolders()`，缺少 `TagRepository.syncAllTags()` 和 `FileRepository.syncAllFiles()`。

**修复**：
- 扩展 `LoginViewModel` 构造函数与 `Factory`，注入 `TagRepository` 和 `FileRepository`。
- `LoginViewModel.login()` 登录成功后依次调用：
  1. `folderRepository.syncAllFolders(context)`（含笔记）
  2. `tagRepository.syncAllTags(context)`
  3. `fileRepository.syncAllFiles(context)`
- `LoginFragment` 的 `ViewModel.Factory` 同步传入这两个 Repository 实例。
````

````markdown title:""
编译通过（`BUILD SUCCESSFUL`）。三个问题均已修复，核心原因和修复如下：

---

### 1. 笔记详情 UI 不更新（最严重）

**根因**：
- `NoteEditorViewModel.loadNote()` 中，`noteRepository.getNoteDetail()` 虽然网络请求成功，但其内部的 `noteDao.insert()` 若抛出异常（如数据库竞态），会导致整个协程崩溃，`when` 分支无法执行，`_noteDetail.value` 永远是 null。
- `observeLocalNote()` 原来只在 `_noteDetail.value == null` 时更新，一旦本地数据库先占坑（例如同步时插入了 content 为空的记录），后续网络数据回写 Room 后，Flow 再次发射时因为 `_noteDetail != null` 而跳过更新，UI 就卡在空状态。
- `NoteEditorFragment` 中 `isUserEditing` 标志位和 `TextWatcher` 存在微小竞态风险。

**修复**：
- `loadNote()` 用 `try-catch` 包裹整个网络请求，任何异常都会回退到本地数据，确保 `_noteDetail` 一定会被赋值。
- `observeLocalNote()` 改为：**只要用户没有未保存的修改，就始终用本地数据库最新数据更新 `_noteDetail`**。这样网络写入 Room 后的 Flow 发射能正确刷新界面。
- `NoteEditorFragment` 中将 `TextWatcher` 提取为属性，在 `setText` 前先 `removeTextChangedListener`，设置完后再恢复，彻底消除竞态。

---

### 2. 文件树 UI 不更新子文件夹下的笔记

**根因**：
- `NoteListViewModel` 中 `refreshTrigger` 原来是 `MutableStateFlow(Unit)`，而 `Unit` 是单例，赋值相同值时 `StateFlow` 不会重新 emit，导致 `combine` 无法触发文件树重建。
- 另外，BFS 同步文件夹时只同步了文件夹本身，没有同步各文件夹下的笔记。

**修复**：
- `refreshTrigger` 改为 `MutableStateFlow(0)` 计数器，`toggleFolder` 时递增，确保每次展开/折叠都能触发 `combine` 重建树。
- `FolderRepository.syncAllFolders()` 在 BFS 遍历每个 `pid` 时，**一并调用 `apiService.getNoteList(pid)` 并将该文件夹下的笔记写入 Room**。`NoteListViewModel` 通过 `combine` 观察 `noteDao.getAll()` 的 Flow，数据库变化会自动刷新 RecyclerView。

---

### 3. 登录后未请求标签与已上传文件列表

**修复**：
- `LoginViewModel.login()` 登录成功后已加入完整同步：
  ```kotlin
  folderRepository.syncAllFolders(context)  // 含笔记
  tagRepository.syncAllTags(context)        // GET /api/tag/list
  fileRepository.syncAllFiles(context)      // GET /api/file/list
  ```
- 若你仍然没有在日志中看到 tag/file 请求，**请确认安装的是最新编译的 APK**。代码层面这三个请求已按顺序串行发出，只要网络可用就会执行。
````

````markdown title:""
编译通过（`BUILD SUCCESSFUL`）。三个问题均已修复，核心原因和修复如下：

---

### 1. 笔记详情 UI 不更新（最严重）

**根因**：
- `NoteEditorViewModel.loadNote()` 中，`noteRepository.getNoteDetail()` 虽然网络请求成功，但其内部的 `noteDao.insert()` 若抛出异常（如数据库竞态），会导致整个协程崩溃，`when` 分支无法执行，`_noteDetail.value` 永远是 null。
- `observeLocalNote()` 原来只在 `_noteDetail.value == null` 时更新，一旦本地数据库先占坑（例如同步时插入了 content 为空的记录），后续网络数据回写 Room 后，Flow 再次发射时因为 `_noteDetail != null` 而跳过更新，UI 就卡在空状态。
- `NoteEditorFragment` 中 `isUserEditing` 标志位和 `TextWatcher` 存在微小竞态风险。

**修复**：
- `loadNote()` 用 `try-catch` 包裹整个网络请求，任何异常都会回退到本地数据，确保 `_noteDetail` 一定会被赋值。
- `observeLocalNote()` 改为：**只要用户没有未保存的修改，就始终用本地数据库最新数据更新 `_noteDetail`**。这样网络写入 Room 后的 Flow 发射能正确刷新界面。
- `NoteEditorFragment` 中将 `TextWatcher` 提取为属性，在 `setText` 前先 `removeTextChangedListener`，设置完后再恢复，彻底消除竞态。

---

### 2. 文件树 UI 不更新子文件夹下的笔记

**根因**：
- `NoteListViewModel` 中 `refreshTrigger` 原来是 `MutableStateFlow(Unit)`，而 `Unit` 是单例，赋值相同值时 `StateFlow` 不会重新 emit，导致 `combine` 无法触发文件树重建。
- 另外，BFS 同步文件夹时只同步了文件夹本身，没有同步各文件夹下的笔记。

**修复**：
- `refreshTrigger` 改为 `MutableStateFlow(0)` 计数器，`toggleFolder` 时递增，确保每次展开/折叠都能触发 `combine` 重建树。
- `FolderRepository.syncAllFolders()` 在 BFS 遍历每个 `pid` 时，**一并调用 `apiService.getNoteList(pid)` 并将该文件夹下的笔记写入 Room**。`NoteListViewModel` 通过 `combine` 观察 `noteDao.getAll()` 的 Flow，数据库变化会自动刷新 RecyclerView。

---

### 3. 登录后未请求标签与已上传文件列表

**修复**：
- `LoginViewModel.login()` 登录成功后已加入完整同步：
  ```kotlin
  folderRepository.syncAllFolders(context)  // 含笔记
  tagRepository.syncAllTags(context)        // GET /api/tag/list
  fileRepository.syncAllFiles(context)      // GET /api/file/list
  ```
- 若你仍然没有在日志中看到 tag/file 请求，**请确认安装的是最新编译的 APK**。代码层面这三个请求已按顺序串行发出，只要网络可用就会执行。
````

````markdown title:""
````


````markdown title:""
````


````markdown title:""
````

````markdown title:""
````


````markdown title:""
````



````markdown title:""
````




````markdown title:""
````



````markdown title:""
````
