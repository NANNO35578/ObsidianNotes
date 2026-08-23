# 开发周志 —— 第12周：标签系统、文件上传与 MinIO 集成

## 本周 TODO
- [ ] 实现 TagController / TagService（创建 / 列表 / 更新 / 删除 / 绑定 / 解绑）
- [ ] 集成 MinIO 客户端，实现文件异步上传至对象存储
- [ ] 实现 FileController / FileService（上传 / 列表 / 删除 / 状态查询）
- [ ] 设计文件上传状态机（processing → done / failed）与轮询机制

## 工作内容概括

本周实现标签与文件两大辅助模块。标签系统支持全局去重创建、笔记-标签多对多绑定与解绑；`getTagList` 返回用户所有标签，供右侧边栏展示。文件模块引入 MinIO 作为对象存储，bucket 命名为 `notes-files`，设为公开只读，通过 URL 直接访问。

文件上传采用异步架构：
1. 前端/Android 通过 `POST /api/file/upload` 提交文件；
2. 服务端先将元数据写入 `file_resource` 表，状态为 `processing`；
3. 后台线程异步将文件流上传至 MinIO，完成后更新 `status = done` 并写入 `url`；
4. 客户端通过 `GET /api/file/status?file_id={id}` 轮询查询上传结果。

该设计确保上传大文件时 HTTP 接口不会长时间阻塞，提升并发能力。同步实现文件删除 API，操作包含 MinIO 物理删除与数据库记录清理两步。

## 关键产出
- 标签系统完整 API
- MinIO 客户端工具类（MinioClient）
- 文件异步上传与状态轮询机制

---

## 工作记录（精简版）

### 一、标签系统

| 接口 | 说明 |
|------|------|
| `/api/tag/create` | 先查 `uk_tag_name`，存在则返回已有 tag_id |
| `/api/tag/update` | 更新名称，同步更新所有关联笔记的展示 |
| `/api/tag/delete` | 先删 `note_tag` 关联，再删 `tag` 记录 |
| `/api/notes/{id}/tags` | GET，返回笔记绑定的标签列表 |
| `/api/notes/{id}/tags/bind` | POST，绑定标签，幂等（联合主键防重复） |
| `/api/notes/{id}/tags/unbind` | POST，解绑标签 |

### 二、MinIO 集成与文件上传

**MinIO 配置**：
- Endpoint: `localhost:9000`
- Bucket: `notes-files`
- Access Policy: 公开只读（`public`），通过 `http://localhost:9000/notes-files/{object_key}` 直接访问

**上传状态机**：

```
前端上传 → DB 写入元数据（processing）→ 后台线程上传 MinIO
                                          ↓
                                    成功：status=done, 写入 url
                                    失败：status=failed, 记录日志
```

**MinioClient 工具类核心方法**：
- `upload(const std::string& localPath, const std::string& objectKey)` → 返回 URL
- `remove(const std::string& objectKey)` → 物理删除

### 三、文件删除策略

删除文件时执行两步：
1. MinIO 物理删除 `object_key`；
2. `file_resource` 表软删或直接删除记录。

两步非原子，若 MinIO 删除成功但 DB 失败，文件成为孤儿对象；通过定时扫描 `file_resource` 与 MinIO 实际对象差异进行补偿清理。

### 四、本周问题

- **大文件上传内存占用**：Drogon 默认将 multipart 文件读入内存，上传 100MB+ 文件时 RSS 激增；改用流式读取，边接收边写入临时文件，再转传 MinIO。
- **文件名重复**：用户上传同名文件时，MinIO 默认覆盖。解决：object_key 采用 `{user_id}/{uuid}_{filename}` 命名，确保唯一且可按用户隔离。

### 五、会议备忘

MinIO 公开桶安全确认：桶策略设为只读，禁止 ListBucket 与写操作，防止未授权遍历他人文件。上传/删除权限由后端 API 控制，不暴露 AccessKey/SecretKey 给客户端。
