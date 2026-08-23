# 开发周志 —— 第17周：Markdown 编辑器、自动保存与文件上传

## 本周 TODO
- [ ] 集成 md-editor-v3，实现 NoteEditor.vue Markdown 编辑与实时预览
- [ ] 实现笔记自动保存（2 秒防抖，saveStatus 状态提示）
- [ ] 实现图片/文件粘贴上传，调用 uploadFile + pollFileStatus 轮询
- [ ] 实现 RightSidebar（TagList / FileList / UserProfile / SearchResults）
- [ ] 实现标签绑定/解绑、文件列表展示与删除

## 工作内容概括

本周聚焦中间编辑区与右侧边栏的功能实现。`NoteEditor.vue` 基于 `md-editor-v3` 封装，支持 Markdown 编辑、实时预览、代码高亮与主题适配（通过 `:deep()` 覆盖编辑器内部样式为 Everforest 配色）。自动保存机制：编辑器内容变化时触发 `handleNoteChange`，设置 2 秒防抖定时器，超时后调用 `updateNote` 保存，状态栏实时显示「已保存 / 有未保存的更改 / 保存中…」。

文件上传支持拖拽与粘贴两种触发方式：调用 `uploadFile` 获取 `file_id` 后，通过 `pollFileStatus` 每秒轮询一次、最多 60 秒，获取最终 URL 并插入编辑器光标位置。

右侧边栏功能分区：
- `TagList`：展示当前笔记的标签，支持解绑；展示所有标签，支持绑定；
- `FileList`：展示当前笔记附件，支持删除；
- `UserProfile`：展示用户信息；
- `SearchResults`：输入关键词后展示 ES 搜索结果，含高亮片段。

## 关键产出
- Markdown 编辑器组件（含主题适配）
- 笔记自动保存机制
- 图片/文件上传与轮询插入
- 右侧边栏四大面板

---

## 工作记录（精简版）

### 一、NoteEditor 组件

基于 `md-editor-v3` 封装，核心 Props：`v-model:content`、`noteId`、`theme`。

**主题覆盖**：

```vue
<style scoped>
.note-editor :deep(.md-editor) {
  background-color: var(--bg-secondary) !important;
  color: var(--text-primary) !important;
}
.note-editor :deep(.md-editor-preview) {
  /* 覆盖预览区代码块背景 */
}
</style>
```

### 二、自动保存机制

```javascript
// Home.vue
let saveTimer = null;
const handleNoteChange = () => {
  saveStatus.value = '有未保存的更改...';
  if (saveTimer) clearTimeout(saveTimer);
  saveTimer = setTimeout(() => {
    saveCurrentNote(); // 调用 POST /api/note/update
    saveStatus.value = '已保存';
  }, 2000);
};
```

- 2 秒防抖，减少频繁保存请求；
- 状态栏实时显示保存状态；
- 页面关闭前通过 `beforeunload` 事件提示未保存内容。

### 三、文件上传与轮询

```javascript
const onUploadImg = async (files, callback) => {
  const formData = new FormData();
  formData.append('file', files[0]);
  formData.append('note_id', noteId);
  
  const { file_id } = await uploadFile(formData);
  const { url } = await pollFileStatus(file_id, { interval: 1000, maxAttempts: 60 });
  callback([url]);
};
```

`pollFileStatus` 封装：定时器轮询，成功返回 URL，失败或超时报错。

### 四、RightSidebar 四大面板

| 面板 | 数据来源 | 交互 |
|------|----------|------|
| TagList | `/api/notes/{id}/tags` + `/api/tag/list` | 当前笔记标签可解绑，全部标签可绑定 |
| FileList | `/api/file/list?note_id=` | 展示附件，支持删除 |
| UserProfile | `/api/user/profile` | 展示头像、用户名，提供退出登录 |
| SearchResults | `/api/note/search?keyword=` | 输入防抖 300ms，结果高亮展示 |

### 五、本周问题

- **自动保存与手动保存冲突**：用户点击「保存」按钮时，若防抖定时器未触发，可能导致重复保存。解决：手动保存时 `clearTimeout(saveTimer)`，并立即执行保存。
- **轮询未清除导致内存泄漏**：组件卸载时未清理 `pollFileStatus` 的定时器。解决：在 `onUnmounted` 中统一清除所有 pending 定时器。

### 六、会议备忘

自查：编辑器 Markdown 渲染正常，图片上传后可插入光标位置，自动保存 2 秒防抖工作稳定，右侧边栏四大面板数据刷新正常。
