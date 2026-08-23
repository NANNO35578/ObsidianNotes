# 开发周志 —— 第6周：数据库概念与逻辑设计

## 本周 TODO
- [ ] 绘制 E-R 图，梳理实体关系（用户、笔记、文件夹、标签、文件、行为）
- [ ] 设计 13 张核心表的字段、类型与约束
- [ ] 定义主键、外键、唯一索引与联合主键
- [ ] 完成数据库逻辑结构设计文档

## 工作内容概括

本周围绕数据持久化层展开设计。通过 E-R 图梳理出五大主题域：用户域（user / user_token）、笔记域（note / note_history）、组织域（note_folder / tag / note_tag）、文件域（file_resource）、行为域（search_history / user_action / user_tag_stat / note_like / note_collect）。

核心设计决策包括：
- 笔记采用**软删除**（`is_deleted` 标记），保留历史版本（`note_history`）；
- 文件夹通过 `parent_id` 自关联实现**多级树形目录**；
- 标签全局去重（`uk_tag_name`），通过 `note_tag` 关联表实现笔记与标签的多对多关系；
- 文件资源表引入 `status` 枚举（processing / done / failed），支持异步上传状态追踪；
- 行为统计表 `user_tag_stat` 为推荐系统预留，记录用户对各标签的浏览/点赞/收藏计数。

## 关键产出
- E-R 图与逻辑结构文档
- 13 张核心表的 CREATE TABLE 脚本初稿
- 字段命名规范与索引策略说明

---

## 工作记录（精简版）

### 一、E-R 图核心实体关系

```
User ||--o{ Note : writes
User ||--o{ NoteFolder : owns
User ||--o{ FileResource : uploads
User ||--o{ UserAction : generates
Note }o--o{ Tag : has
Note ||--o{ NoteHistory : versions
NoteFolder ||--o{ NoteFolder : parent
Note ||--o{ FileResource : contains
Note ||--o{ NoteLike : receives
Note ||--o{ NoteCollect : receives
```

**关系说明**：用户与笔记一对多；笔记与标签多对多（`note_tag` 关联）；文件夹自关联树形；行为表桥接用户与标签统计。

### 二、13 张核心表设计要点

| 表名 | 核心设计决策 |
|------|-------------|
| `user` | `username` UNIQUE，`password_hash` VARCHAR(255) 存 bcrypt 结果 |
| `user_token` | 支持多端登录，`expired_at` 控制 Token 生命周期 |
| `note` | `is_deleted` TINYINT 软删除；`is_public` 控制可见性；计数器字段（view/like/collect）|
| `note_history` | `version` INT 递增，保留最近编辑内容，用于版本回溯 |
| `note_folder` | `parent_id` BIGINT 自关联，NULL 为根目录 |
| `tag` | `name` UNIQUE 全局去重，避免重复标签碎片 |
| `note_tag` | `(note_id, tag_id)` 联合主键，无外键冗余 |
| `file_resource` | `status` ENUM('processing','done','failed') 追踪异步上传；`object_key` 存 MinIO 路径 |
| `note_like` / `note_collect` | `(user_id, note_id)` 联合主键防重复，无独立 ID |
| `search_history` | `idx_user_time` 索引支撑用户搜索习惯分析 |
| `user_action` | `action_type` TINYINT（1=浏览/2=点赞/3=收藏），三索引支撑统计 |
| `user_tag_stat` | 推荐系统核心表，记录用户对各标签的行为计数与最后操作时间 |

### 三、命名与类型规范

- 表名：小写 + 下划线，复数形式（`note_folders` 未采用，保持 `note_folder` 与 Drogon ORM 生成习惯一致）
- 主键：统一 `id BIGINT AUTO_INCREMENT`
- 时间字段：`created_at` DEFAULT CURRENT_TIMESTAMP，`updated_at` ON UPDATE CURRENT_TIMESTAMP
- 布尔字段：TINYINT(1)，0/1 表示，避免 MySQL BOOL 实际映射为 TINYINT 的歧义
- 外键：开发阶段启用约束保证完整性，线上高并发场景可视情况禁用外键检查

### 四、本周问题

- **note_history 版本号策略**：自增 vs 时间戳。选自增 INT，更轻量且语义清晰（第几版），时间戳作为辅助字段存在 `created_at`。
- **tag 全局去重 vs 用户隔离**：全局去重可减少标签碎片，但不同用户对同一名称的标签可能有不同语义。选全局去重，因系统面向个人笔记，标签语义由内容决定而非用户主观定义。

### 五、会议备忘

导师意见：ER 图需补充Cardinality标注；`user_tag_stat` 的 `last_action_time` 应考虑时区问题，统一使用服务器 UTC 时间存储，前端转换本地时区显示。
