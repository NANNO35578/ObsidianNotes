# 开发周志 —— 第11周：笔记 CRUD 与文件夹树形管理

## 本周 TODO
- [ ] 实现 NoteController 与 NoteService（创建 / 更新 / 删除 / 列表 / 详情）
- [ ] 实现笔记软删除与 note_history 版本记录
- [ ] 实现 FolderController 与 FolderService（多级目录 CRUD）
- [ ] 实现文件夹循环引用检测与级联删除策略

## 工作内容概括

本周完成两大核心内容模块。笔记模块支持完整的 CRUD 操作：创建时自动生成 `created_at`；更新时同步写入 `note_history` 表保存旧版本内容；删除采用软删除（`is_deleted = 1`），保留数据以便恢复。`getNoteList` 支持按 `folder_id` 筛选与关键词过滤，`getNoteDetail` 返回完整内容与关联标签。

文件夹模块实现多级树形目录管理：
- `createFolder` 支持指定 `parent_id`，根目录为 `null`；
- `updateFolder` 支持重命名与移动父目录，移动前检测是否形成循环引用（避免将父文件夹移入其子树）；
- `deleteFolder` 采用级联删除：先递归删除子文件夹与其中笔记，再删除自身；
- `getFolderList` 返回扁平列表，由前端/Android 根据 `parent_id` 自行组装为树形结构。

## 关键产出
- 笔记模块完整 CRUD API
- 文件夹模块树形管理 API
- note_history 版本回溯机制

---

## 工作记录（精简版）

### 一、笔记模块 CRUD

| 接口 | 方法 | 关键逻辑 |
|------|------|----------|
| `/api/note/create` | POST | ORM 插入，返回 note_id；触发后台 ES 索引与 AI 标签（异步） |
| `/api/note/update` | POST | 增量更新；更新前将旧内容写入 note_history；更新 ES 索引 |
| `/api/note/delete` | POST | `is_deleted = 1` 软删除；同步删除 ES 索引；保留历史版本 |
| `/api/note/list` | GET | 支持 folder_id 筛选、is_deleted=0 过滤、created_at 倒序 |
| `/api/note/detail` | GET | 返回笔记基础字段；tags/files 通过独立接口获取 |

**版本历史**：每次 update 时，将当前 content 与 version 号写入 `note_history`，version 自增。最多保留最近 20 条，超限时删除最旧记录。

### 二、文件夹模块

| 接口 | 关键逻辑 |
|------|----------|
| create | parent_id 为空时归入根目录 |
| update | 支持重命名与移动；移动前检测循环引用（DFS 遍历祖先） |
| delete | 级联删除：递归删子文件夹 → 软删其中笔记 → 删自身 |
| list | 返回扁平列表，前端/Android 按 parent_id 组装树 |

**循环引用检测**：移动文件夹前，从目标 parent_id 向上 DFS 遍历祖先链，若遇到待移动文件夹自身 id，则拒绝并返回 code=2002。

### 三、事务边界

笔记创建/更新与 note_history 写入需保证原子性，采用 Drogon ORM 事务：

```cpp
auto trans = app().getDbClient()->newTransaction();
try {
    orm::Note(note).insert(trans);
    orm::NoteHistory(history).insert(trans);
    trans->commit();
} catch (...) {
    trans->rollback();
}
```

### 四、本周问题

- **级联删除性能**：文件夹层级深时递归删除耗时。优化：先批量标记子文件夹 id 列表，再批量软删笔记，最后批量删文件夹，减少单条循环。
- **note_history 表膨胀**：高频编辑导致历史记录激增。限制单笔记保留 20 个版本，超限时异步清理最旧记录。

### 五、会议备忘

自查：笔记/文件夹 API 已通过 Postman 基础测试，循环引用检测边界场景（移动至自身、移动至子节点）全部覆盖。
