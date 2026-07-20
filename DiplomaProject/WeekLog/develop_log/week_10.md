# 开发周志 —— 第10周：后端框架搭建与用户认证模块实现

## 本周 TODO
- [ ] 完成 Drogon 服务配置（config.json：端口、DB、线程池、日志）
- [ ] 集成 MariaDB 数据库连接池与 ORM 异步操作
- [ ] 实现 JwtUtil 工具类（Token 生成与校验）
- [ ] 实现 PasswordUtil（密码哈希，bcrypt）
- [ ] 实现 AuthController（注册 / 登录 / 退出）与 AuthService

## 工作内容概括

本周正式进入后端编码阶段。首先完善 `config.json`，配置监听端口 `8888`、MariaDB 连接参数、线程数与日志级别。引入 `libjsoncpp`、`libuuid`、`OpenSSL`、`libcurl` 等依赖，确保 CMake 构建通过。

用户认证模块作为系统入口，优先实现：
- `PasswordUtil`：基于 bcrypt 对密码进行哈希存储，杜绝明文保存；
- `JwtUtil`：封装 JWT 的生成与校验逻辑，支持从 Header 或 URL 参数读取 Token；
- `AuthService`：处理注册（用户名唯一性校验）、登录（密码比对 + Token 签发）、退出（Token 删除）；
- `AuthController`：暴露 `/api/auth/register`、`/api/auth/login`、`/api/auth/logout`，完成参数解析、调用 Service、组装统一响应。

同步编写了基础测试用例，验证注册-登录-退出全链路可用。

## 关键产出
- 可运行的 Drogon 后端服务（端口 8888）
- 用户认证模块（注册 / 登录 / 退出）
- JwtUtil / PasswordUtil 工具类
- calcite_test 测试子目标通过编译

---

## 工作记录（精简版）

### 一、Drogon 服务配置

`config.json` 核心参数：

| 参数 | 取值 | 说明 |
|------|------|------|
| listen_port | 8888 | 服务监听端口 |
| db_client | mariadb | 连接池大小 10 |
| threads_num | 4 | IO 线程数 |
| log | async_log | 异步日志，级别 info |

依赖确认：`libjsoncpp`、`libuuid`、`OpenSSL`、`libcurl` 均已链接，CMake 构建通过。

### 二、认证模块实现

| 类 | 职责 | 关键实现 |
|----|------|----------|
| PasswordUtil | 密码哈希 | bcrypt，cost factor = 10 |
| JwtUtil | Token 生成/校验 | HS256，过期时间 7 天，支持 Header/URL 参数提取 |
| AuthService | 业务逻辑 | 注册查重、登录比对、退出删表记录 |
| AuthController | HTTP 入口 | `/api/auth/register` `/login` `/logout` |

**注册流程**：校验用户名唯一 → bcrypt 哈希 → 插入 user 表 → 生成 JWT → 写 user_token 表 → 返回 token。

**登录流程**：查用户 → bcrypt 校验 → 生成 JWT → 写 user_token → 返回 token。

### 三、测试验证

```bash
cd build && make calcite_test
./test/calcite_test
# 输出：All tests passed (5 assertions in 3 test cases)
```

测试覆盖：注册成功、用户名重复、登录密码错误、Token 校验、退出失效。

### 四、本周问题

- **bcrypt 库选型**：`libbcrypt` 与 Drogon 的 `libpq` 存在符号冲突；改用 `openssl/evp.h` 实现 PBKDF2 替代，后确认 Drogon 自带 `drogon::utils::getMd5` 不满足安全需求，最终引入独立 `bcrypt.cpp` 单文件库，隔离命名空间后解决。
- **config.json 热加载**：Drogon 不支持配置热更新，修改端口或 DB 参数需重启服务；毕业设计阶段可接受，生产环境需配合进程管理工具（systemd/supervisor）。

### 五、会议备忘

后端自评：Auth 模块为系统入口，需确保健壮性。下一步优先实现 Note/Folder 核心 CRUD，为前端/Android 提供可联调的接口。
