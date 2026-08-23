你是资深前端开发人员，技术栈为Vue3+Element Plus及相关生态，专注问题排查与代码重构。
## 需求
修复笔记系统中**笔记与文件夹关联构建文件树**的异常问题，将现有文件树组件重构为el-tree懒加载模式，解决请求参数传递错误、接口重复请求的问题。
## 问题现象
web端登录后控制台日志异常：
```log
请求: GET /user/profile undefined             request.js:28
请求: GET /folder/list Object             request.js:28
请求: GET /note/list undefined             request.js:28
请求: GET /tag/list undefined             request.js:28
请求: GET /file/list Object             request.js:45
响应: /user/profile 请求参数: undefined 响应数据: Object             request.js:45
响应: /note/list 请求参数: undefined 响应数据: Object             request.js:45
响应: /tag/list 请求参数: undefined 响应数据: Object             request.js:45
响应: /folder/list 请求参数: Object 响应数据: Object             request.js:28
请求: GET /folder/list Object             request.js:45
响应: /file/list 请求参数: Object 响应数据: Object             request.js:45
响应: /folder/list 请求参数: Object 响应数据: Object             request.js:28
请求: GET /folder/list Object             request.js:45
响应: /folder/list 请求参数: Object 响应数据: Object             request.js:28
请求: GET /folder/list Object             request.js:45
响应: /folder/list 请求参数: Object 响应数据: Object
```
核心问题：`/folder/list`、`/note/list` 接口**重复频繁请求**，**folder_id参数未正确传递**，无法正常构建树形结构。

## 需求规范
### 设计规则
用户登录后正常请求流程：
1. 固定请求：GET /user/profile、GET /file/list、GET /tag/list
2. 仅初始化请求**根节点数据**：
   - 根文件夹列表：GET /folder/list?folder_id=0
   - 根笔记列表：GET /note/list?folder_id=0
3. 懒加载规则：**点击文件夹时**，才请求当前文件夹下的子数据，示例（文件夹id=34）：
   - GET /folder/list?folder_id=34
   - GET /note/list?folder_id=34
4. 文件夹+笔记合并渲染为el-tree文件树，文件夹可展开，文件为叶子节点

### 处理范围
1. 核心修改文件：`./calcite-web/src/components/FileTree.vue`
2. 检查并修复：前端请求API层的folder_id参数传递逻辑
3. 废弃原有的递归全量加载逻辑，替换为el-tree原生懒加载

## 输出要求
1. 精准定位问题点（FileTree.vue组件、API参数传递）
2. 清晰概括完整解决思路
3. 输出规范文档：在`./docs`目录下生成**260415_**为前缀的修复文档
4. 提供可直接运行的Vue3+Element Plus代码实现


````markdown title:"good job"
修复已完成，构建验证通过。以下是完整总结：

---

## 一、精准定位的问题点

### 1. `src/views/Home.vue` — 递归全量加载 + 参数缺失
- `fetchAllFolders()` 使用 **BFS while 循环** 逐层递归请求所有文件夹，导致登录时产生大量重复的 `GET /folder/list` 请求
- `fetchAllNotes()` 调用 `getNoteList()` **未传任何参数**，导致 `folder_id` 缺失，控制台打印 `undefined`
- `fetchAllTags()` 同样未传参数，日志显示 `undefined`

### 2. `src/components/FileTree.vue` — 自定义递归组件不支持懒加载
- 采用递归自调用方式渲染树（组件内嵌套 `<FileTree>`）
- 依赖父组件预先全量注入 `allFolders` / `notes`，无法按需加载

### 3. `src/components/sidebar/LeftSidebar.vue` — 中间层透传冗余
- 向 `FileTree` 透传大量全量数据 props，与懒加载架构不匹配

---

## 二、完整解决思路

1. **废弃递归全量加载**：删除 `Home.vue` 中的 BFS 递归逻辑
2. **重构为 el-tree 原生懒加载**：`FileTree.vue` 改用 `<el-tree lazy :load="loadNode">`
   - 根节点自动请求 `folder_id=0`
   - 点击展开文件夹时，才请求对应 `folder_id` 的子文件夹和笔记
3. **修复参数传递**：所有 `getNoteList` / `getFolderList` 调用均显式传入 `{ folder_id: xxx }`
4. **建立数据同步机制**：
   - `FileTree.vue` 懒加载获得的数据通过 `folders-loaded` / `notes-loaded` 事件回写 `Home.vue` 缓存
   - `FileTree.vue` 暴露 `refreshNode(folderId)` 方法，`Home.vue` 在 CRUD 后定向刷新受影响的树节点

---

## 三、已修改文件

| 文件 | 修改内容 |
|------|----------|
| `calcite-web/src/components/FileTree.vue` | **完全重写**：替换为 `el-tree` 懒加载；实现 `loadNode`、节点构建、展开状态同步、暴露 `refreshNode` |
| `calcite-web/src/views/Home.vue` | 删除递归 `fetchAllFolders`；新增 `fetchRootFolders` / `fetchRootNotes` / `handleFoldersLoaded` / `handleNotesLoaded` / `refreshFolderData` / `refreshTreeNode`；CRUD 后定向刷新 |
| `calcite-web/src/components/sidebar/LeftSidebar.vue` | 移除冗余 props 透传；新增事件透传；暴露 `fileTreeRef` |
| `calcite-web/src/utils/request.js` | 无需修改（参数已修正后日志自然正常） |

---

## 四、Build 验证结果

```bash
cd calcite-web && npm run build
# ✅ 构建成功，3584 modules transformed，无编译错误
```

---

## 五、规范文档

已生成至：
```
./docs/260415_filetree_lazyload_fix.md
```

文档包含完整的问题定位、根因分析、代码变更详情、请求行为预期、Build 验证结果及后续优化建议。
````


# Open
- [x] 修改笔记编辑界面返回按钮为el-tooltip
- [ ] 