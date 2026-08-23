# 开发周志 —— 第16周：三栏布局框架与文件夹管理

## 本周 TODO
- [ ] 实现 Home.vue 三栏布局（左侧文件树 / 中间编辑区 / 右侧标签文件）
- [ ] 实现 ElSplitter 可拖拽分栏与侧边栏折叠（左右可收起）
- [ ] 实现 FileTree.vue 递归树形组件（文件夹展开/折叠/选中）
- [ ] 实现 FolderDialog（新建 / 重命名 / 删除文件夹）
- [ ] 实现 NoteDialog（新建笔记）
- [ ] 集成 Pinia Store（folderStore、noteStore、layoutStore）

## 工作内容概括

本周构建 Web 端最核心的主界面框架。三栏布局使用 Element Plus 的 `ElSplitter` 实现左右两栏可拖拽宽度，并支持完全折叠（通过 `leftCollapsed` / `rightCollapsed` 状态控制，持久化至 `localStorage`）。左侧边栏展示递归文件树，区分文件夹与笔记节点，支持点击选中、展开/折叠、右键菜单操作。

组件开发：
- `FileTree.vue`：递归渲染文件夹层级，Props 传递 `folders`、`notes`、`expandedFolders` 等状态，Emits 向上冒泡点击/创建/重命名/删除事件；
- `FolderDialog.vue`：基于 `ElDialog` + `el-form`，支持新建子文件夹、重命名、删除确认；
- `NoteDialog.vue`：新建笔记时选择所属文件夹。

状态管理引入 Pinia，按业务域拆分 Store：`folderStore` 管理文件夹数据与选中状态，`noteStore` 管理笔记列表与当前编辑笔记，`layoutStore` 管理侧边栏折叠与主题。通过 Store 的响应式更新驱动三栏联动。

## 关键产出
- Home.vue 三栏主界面框架
- FileTree 递归组件
- FolderDialog / NoteDialog 对话框
- Pinia 状态管理模块

---

## 工作记录（精简版）

### 一、三栏布局

`Home.vue` 采用 `ElSplitter` 实现左右分栏：

```
┌──────────┬──────────────┬──────────┐
│ LeftSidebar│  CenterArea │RightSidebar│
│ (文件树)  │ (编辑器/列表)│ (标签/文件)│
└──────────┴──────────────┴──────────┘
```

- 左右栏宽度可拖拽，最小宽度 200px；
- `leftCollapsed` / `rightCollapsed` 控制完全折叠，状态持久化至 `localStorage`；
- 折叠时显示悬浮按钮，点击展开。

### 二、FileTree 递归组件

**Props**：`folders`、`notes`、`parentFolderId`、`expandedFolders`、`selectedFolderId`、`selectedNoteId`

**递归逻辑**：遍历当前层级 folders，若文件夹 id 在 `expandedFolders` 中，则递归渲染子级（传入 `parentFolderId = 当前folder.id`）。

**性能优化**：扁平数据转树时预计算 `children` 数组，避免渲染时重复 filter。

### 三、对话框组件

| 组件 | 功能 | 关键字段 |
|------|------|----------|
| FolderDialog | 新建/重命名/删除文件夹 | `name`（必填）、`parent_id` |
| NoteDialog | 新建笔记 | `title`、`folder_id`（下拉选择）|

对话框使用 `ElDialog` + `el-form`，Props 定义 `visible`，Emits 定义 `update:visible` 与 `confirm`。

### 四、Pinia Store 拆分

| Store | 状态 | Actions |
|-------|------|---------|
| `folderStore` | `folders`、`selectedFolderId` | `fetchFolders`、`createFolder`、`deleteFolder` |
| `noteStore` | `notes`、`selectedNoteId`、`editingNote` | `fetchNotes`、`saveNote`、`deleteNote` |
| `layoutStore` | `leftCollapsed`、`rightCollapsed`、`theme` | `toggleSidebar`、`setTheme` |
| `tagStore` | `tags`、`noteTags` | `fetchTags`、`bindTag`、`unbindTag` |
| `fileStore` | `files` | `fetchFiles`、`uploadFile`、`deleteFile` |

### 五、本周问题

- **递归组件事件冒泡复杂**：FileTree 嵌套层级深时，子级点击事件需层层 `$emit` 至 Home.vue。优化：使用 Pinia Store 直接管理 `selectedFolderId` 与 `selectedNoteId`，子组件直接 commit mutation，减少事件链。
- **ElSplitter 折叠后拖拽失效**：折叠时隐藏 Splitter 手柄，展开后恢复，通过 `v-show` 控制 DOM 显隐而非 `v-if`（保留拖拽状态）。

### 六、会议备忘

自查：三栏布局渲染正常，FileTree 支持 5 级嵌套不卡顿，Pinia 状态响应式更新正常。下一步实现编辑器与文件上传。
