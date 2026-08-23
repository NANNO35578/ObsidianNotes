# 开发周志 —— 第21周：Android 主界面与核心功能实现

## 本周 TODO
- [ ] 实现 MainActivity（Single Activity + DrawerLayout + BottomNavigationView）
- [ ] 实现 NoteListFragment（左抽屉笔记列表）与树形文件夹适配器 TreeAdapter
- [ ] 实现 NoteEditorFragment（Markdown 编辑与预览，Markwon 渲染）
- [ ] 实现 SearchFragment（ES 全文搜索）
- [ ] 实现文件上传与 OCR 功能（调用后端异步接口 + 轮询）
- [ ] 实现标签绑定/解绑与 AI 生成标签

## 工作内容概括

本周完成 Android 端最核心的主界面与业务功能。主界面采用 **Single Activity** 架构：`MainActivity` 集成 `DrawerLayout`（左侧抽屉为笔记列表，右侧抽屉为工具面板）与 `BottomNavigationView`（5 个底部标签：笔记、搜索、推荐、工具、我的），`NavHostFragment` 管理各页面路由。

核心 Fragment 实现：
- `NoteListFragment`：左抽屉内容，展示树形文件夹与笔记列表，`TreeAdapter` 实现可展开/折叠的多级列表；
- `NoteEditorFragment`：核心编辑页，使用 Markwon 渲染 Markdown 预览，支持内容编辑、自动保存、图片上传；
- `SearchFragment`：输入关键词调用后端 ES 搜索，结果列表展示标题与摘要高亮；
- `RecommendFragment`：展示个性化推荐笔记卡片，支持点击进入预览。

文件与 OCR 延续 Web 端的异步轮询模式：上传后通过 WorkManager 或协程定时轮询状态，完成后更新 UI。AI 标签生成在笔记保存后可选触发，展示推荐标签供用户一键添加。

## 关键产出
- MainActivity 主界面框架
- NoteListFragment + TreeAdapter 树形列表
- NoteEditorFragment（Markwon 渲染）
- SearchFragment / RecommendFragment
- 文件上传与 OCR 移动端适配

---

## 工作记录（精简版）

### 一、主界面框架

`MainActivity` 采用 Single Activity + Navigation Component：

```
DrawerLayout
 ├── 主内容：FragmentContainerView (NavHost) + BottomNavigationView
 ├── 左抽屉：NoteListFragment
 └── 右抽屉：工具面板（预留）
```

底部 5 个菜单项：笔记、搜索、推荐、工具、我的。`NavHost` 管理 `nav_graph.xml`，`BottomNavigationView` 与导航图绑定 `setupWithNavController()`。

### 二、TreeAdapter 树形列表

继承 `BaseExpandableListAdapter`，实现两级结构：

- **Group**：文件夹（`FolderEntity`），支持展开/折叠；
- **Child**：笔记（`NoteEntity`），点击打开编辑器。

**数据组装**：Repository 从 Room 读取扁平列表，ViewModel 按 `parentId` 分组为 `Map<FolderEntity, List<NoteEntity>>`，适配器直接消费分组结果。

### 三、NoteEditorFragment

| 功能 | 实现 |
|------|------|
| Markdown 预览 | Markwon `Markwon.create(context)` 渲染 `TextView` |
| 编辑模式 | `EditText` 输入 Markdown 原文 |
| 模式切换 | 底部工具栏「预览/编辑」按钮切换可见性 |
| 自动保存 | 内容变化后 3 秒协程延迟保存，取消时重置 Job |
| 图片上传 | 调用 `FileRepository.upload()`，协程轮询状态 |

### 四、SearchFragment

- `SearchView` 输入关键词，`TextWatcher` 500ms 防抖；
- ViewModel 调用 `NoteRepository.search(keyword)`；
- 结果 `RecyclerView` 展示标题高亮（`SpannableString` 设置 `BackgroundColorSpan` 模拟 `<mark>`）；
- 点击进入 `NotePreviewFragment`（只读）。

### 五、OCR 与 AI 标签移动端适配

| 功能 | 移动端差异 |
|------|------------|
| OCR | 拍照/选图 → 先上传文件 → 提交 OCR 任务 → 协程轮询 → 生成笔记 |
| AI 标签 | 保存笔记后弹窗提示「是否生成标签」，用户确认后调用 `/api/notes/{id}/tags/ai` |

### 六、本周问题

- **Markwon 图片加载**：默认不加载网络图片。解决：集成 `MarkwonImageCoilPlugin`，使用 Coil 加载图片并缓存。
- **DrawerLayout 与 NavController 手势冲突**：侧滑打开抽屉与 Fragment 返回手势冲突。解决：锁定右边缘滑出抽屉，左边缘留给系统返回手势（Android 10+ 全面屏手势）。

### 七、会议备忘

自查：主界面框架稳定，TreeAdapter 支持 3 级展开不卡顿，Markwon 渲染正常，搜索高亮展示正常。OCR 端到端链路（拍照→上传→识别→生成笔记）已跑通。
