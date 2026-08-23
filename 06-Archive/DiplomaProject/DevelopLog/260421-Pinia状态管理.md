已完成使用 Pinia 重构项目数据管理。

### 创建的 Store 架构 (`src/stores/`)

| Store | 职责 |
|-------|------|
| `user.js` | 用户信息 (`userInfo`)、获取与清除 |
| `layout.js` | 侧边栏折叠状态 (`leftCollapsed` / `rightCollapsed`) |
| `folder.js` | 文件夹 CRUD、`selectedFolderId`、`expandedFolders`、树形数据 |
| `note.js` | 笔记 CRUD、编辑器状态、搜索、推荐、保存状态 |
| `file.js` | 文件列表、上传状态管理 |
| `dialog.js` | 文件夹/笔记对话框状态 |

### 核心修改

1. **注册 Pinia** (`main.js`)
   - 创建并注册 `pinia` 实例

2. **重写 Home.vue**
   - 从中央状态容器转变为 **视图编排层**
   - 所有状态迁移至 Pinia stores
   - 保留涉及 `ElMessageBox.confirm`、`router` 跳转和组件 ref（如 `refreshTreeNode`）的协调逻辑

3. **子组件直接使用 Store**
   - `CenterToolbar` → 直接调用 `layoutStore.toggleLeft/Right()` 和 `noteStore.setSearchKeyword/fetchRecommendNotes`
   - `NoteListView` → 直接读取 `noteStore.contentTitle`、`noteStore.displayNotes`
   - `NoteEditor` → 直接读写 `noteStore.editingNote`、`noteStore.saveStatus`，支持主题切换（`dark`/`light`）
   - `PublicNotePreview` → 直接读取 `noteStore.previewingNote`，支持主题切换
   - `SearchResults` → 直接读取 `noteStore.searchResults`、`noteStore.searchTotal` 等
   - `RightSidebar` → 直接读取 `noteStore` / `folderStore`，`FileList` 内嵌使用 `fileStore`
   - `FileList` → 完全独立，直接使用 `fileStore` 做列表、刷新、删除
   - `UserProfile` → 直接读取 `userStore.userInfo`

4. **主题适配补充**
   - `NoteEditor` 和 `PublicNotePreview` 中的 `md-editor-v3` 编辑器主题从硬编码 `theme="dark"` 改为响应式 `:theme="themeStore.isDark ? 'dark' : 'light'"`

项目构建验证通过 ✅。