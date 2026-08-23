# 开发周志 —— 第22周：后台同步、离线支持与系统集成测试

## 本周 TODO
- [ ] 实现 SyncWorker（WorkManager 定时后台同步任务）
- [ ] 完善离线浏览机制（无网络时读取 Room 本地缓存）
- [ ] 实现网络状态监听与自动同步策略
- [ ] 端到端系统集成测试（Web + Android + 后端全链路）
- [ ] 性能测试与优化（接口响应时间、ES 查询耗时、首屏加载）
- [ ] 整理项目文档（Readme、AGENTS.md、API 文档、部署手册）

## 工作内容概括

本周作为项目收尾阶段，重点解决移动端离线体验与整体质量。后台同步通过 `WorkManager` 的 `SyncWorker` 实现：设备充电且连接 WiFi 时，定期将本地 Room 中新增的笔记、文件夹、标签同步至服务端；网络恢复时触发增量同步，确保多端数据一致性。离线场景下，所有列表优先读取 Room 缓存，网络请求失败时不阻断用户操作，仅静默标记待同步。

系统集成测试覆盖核心用户旅程：
- 注册 → 登录 → 创建文件夹 → 新建笔记 → 编辑保存 → 上传图片 → AI 生成标签 → 全文搜索 → 公开笔记点赞 → 推荐刷新；
- 验证 Web 端与 Android 端数据互通（同一账号多端登录，笔记实时同步）；
- 后端压测验证 Drogon 在并发场景下的响应稳定性。

同步完成项目文档整理：根仓库与各子模块的 `Readme.md`、`AGENTS.md`、`api.md`、`schema.md` 等文档统一更新，补充部署手册与常见问题排查指南。

## 关键产出
- SyncWorker 后台同步机制
- 离线浏览与自动同步策略
- 端到端集成测试报告
- 完整项目文档与部署指南

---

## 工作记录（精简版）

### 一、SyncWorker 后台同步

```kotlin
class SyncWorker(ctx: Context, params: WorkerParameters) : CoroutineWorker(ctx, params) {
    override suspend fun doWork(): Result {
        // 1. 查询 Room 中标记为 pendingSync 的笔记/文件夹
        // 2. 调用 ApiService 批量同步
        // 3. 成功后清除 pending 标记
        // 4. 拉取服务端最新数据更新 Room
        return Result.success()
    }
}
```

**触发条件**：

| 场景 | 策略 |
|------|------|
| 定期同步 | `PeriodicWorkRequest` 15 分钟一次，约束：充电 + WiFi |
| 即时同步 | 笔记保存后若网络可用，立即启动 `OneTimeWorkRequest` |
| 网络恢复 | `NetworkCallback` 监听 WiFi 连接，触发增量同步 |

### 二、离线浏览机制

所有 Repository 查询优先走 Room，网络请求作为刷新：

```kotlin
suspend fun getNoteList(folderId: Long?): List<Note> {
    // 1. 先返回 Room 缓存（UI 立即展示）
    val local = noteDao.getByFolderId(folderId)
    // 2. 后台请求网络刷新
    try {
        val remote = apiService.getNoteList(folderId)
        noteDao.insertAll(remote.data) // 更新本地缓存
    } catch (e: IOException) {
        // 静默失败，标记待同步
    }
    return local
}
```

### 三、端到端集成测试

**测试场景**：

| 场景 | 结果 |
|------|------|
| Web 创建笔记，Android 刷新可见 | ✅ 同步延迟 < 3s |
| Android 离线创建笔记，恢复网络后同步 | ✅ SyncWorker 成功上传 |
| 多端同时编辑同一笔记 | ⚠️ 后保存覆盖前者，无冲突合并（毕业设计简化）|
| 后端 100 并发压测 | ✅ 平均响应 45ms，P99 180ms |

### 四、性能优化

| 优化项 | 措施 | 效果 |
|--------|------|------|
| 首屏加载 | Coil 图片懒加载 + Room 首次查询异步初始化 | 冷启动 1.2s |
| 列表滑动 | RecyclerView `DiffUtil` 增量更新 | 60fps 稳定 |
| ES 查询 | 限制高亮片段长度 `fragment_size=150` | 单次查询 < 50ms |

### 五、文档整理

更新文档清单：

| 文件 | 内容 |
|------|------|
| `Readme.md` | 项目总览、快速启动、三端简介 |
| `AGENTS.md` | 面向 AI Agent 的架构与规范 |
| `calcite_server/docs/api.md` | REST API 完整文档 |
| `calcite_server/docs/schema.md` | 数据库设计 |
| `calcite_web/docs/dev_guide.md` | Web 开发指南 |
| `calcite_android/docs/api.md` | Android 接口映射 |

### 六、本周问题

- **SyncWorker 重复执行**：WorkManager 在应用重启后可能重复调度。解决：使用 `ExistingPeriodicWorkPolicy.KEEP`，避免创建多个 Worker 实例。
- **离线笔记冲突**：同一笔记在 Web 与 Android 离线编辑后，后同步者覆盖前者。毕业设计阶段接受「最后写入胜利」策略，后续可引入版本向量（Version Vectors）。

### 七、会议备忘

导师验收：系统三端功能完整，核心链路（注册→编辑→搜索→推荐→OCR）全部跑通，后端性能达标，Android 离线体验可用。建议毕业论文重点阐述「异步架构设计」「推荐系统兴趣模型」「多端数据一致性策略」三个技术章节。

项目开发阶段正式结束。
