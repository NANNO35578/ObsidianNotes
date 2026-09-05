让 Yarn 走代理，通常有几种方法。你用的 Yarn 版本不同（比如 Yarn 1.x 还是 Yarn 2/3/4），命令会有点区别，需要注意一下。

### ⚙️ 方法一：通过 `yarn config` 命令设置（推荐）

这是最直接的方法，设置后会保存在用户级配置文件里。

1.  **首先，明确你的 Yarn 版本**：
    在终端执行 `yarn --version`。如果版本号以 `1.` 开头（如 `1.22.19`），它是 Yarn 1.x（Classic）；如果是 `2.`、`3.`、`4.` 等，则是 Yarn 2+（Berry）。

2.  **根据版本，执行对应的命令**：

    *   **如果你使用的是 Yarn 1.x (Classic)**：
        代理设置使用 `proxy` 和 `https-proxy` 这两个配置项。
        ```bash
        yarn config set proxy http://你的代理地址:端口号
        yarn config set https-proxy http://你的代理地址:端口号
        ```
        例如：`yarn config set proxy http://127.0.0.1:7890`。
        如果代理需要用户名和密码，可以这样写：`http://用户名:密码@host:port`。

    *   **如果你使用的是 Yarn 2+ (Berry)**：
        代理设置使用 `httpProxy` 和 `httpsProxy` 这两个配置项。
        ```bash
        yarn config set httpProxy http://你的代理地址:端口号
        yarn config set httpsProxy http://你的代理地址:端口号
        ```
        例如：`yarn config set httpProxy http://127.0.0.1:7890`。

### 🌐 方法二：通过环境变量设置（对 Yarn 2+ 可能无效）

设置 `HTTP_PROXY` 和 `HTTPS_PROXY` 环境变量是通用做法。

```bash
# 临时设置（仅当前终端会话有效）
export HTTP_PROXY="http://你的代理地址:端口号"
export HTTPS_PROXY="http://你的代理地址:端口号"
```

**需要注意**：有报告指出，Yarn 4 可能会**忽略**这些环境变量。因此，如果此方法无效，请改用方法一。

### 📁 方法三：通过项目配置文件设置（适合团队共享）

在项目根目录下创建或修改 `.yarnrc.yml` 文件（Yarn 2+ 使用）。

```yaml
# .yarnrc.yml
httpProxy: "http://你的代理地址:端口号"
httpsProxy: "http://你的代理地址:端口号"
```

### 🔍 验证与排查

*   **查看当前代理设置**：
    *   Yarn 1.x: `yarn config get proxy`
    *   Yarn 2+: `yarn config get httpProxy`
*   **删除错误的代理设置**：
    *   Yarn 1.x: `yarn config delete proxy`
    *   Yarn 2+: `yarn config delete httpProxy`

### 💎 其他提示

*   **关于镜像源**：如果代理速度不理想，也可以考虑直接更换为国内镜像源，如淘宝镜像 `https://registry.npmmirror.com`。
*   **关于 `strict-ssl`**：如果代理使用自签名证书，可能导致 SSL 错误。可通过 `yarn config set strict-ssl false` 关闭 SSL 严格检查（**不推荐在生产环境使用**）。

建议先确认你的 Yarn 版本，然后使用方法一（`yarn config` 命令）来设置代理，这通常是最直接有效的方式。



-------------

这个错误 `self-signed certificate in certificate chain` 意味着 **Yarn 在通过 HTTPS 请求 npm 仓库时，遇到了无法验证的自签名证书**。通常是因为你的网络环境（如公司内网、代理工具）对 HTTPS 流量进行了拦截或替换了证书。

### ✅ 最快速的解决方案（适合开发环境）

在终端执行以下命令，**关闭 Yarn 的 SSL 严格校验**：

```bash
yarn config set strict-ssl false
```

这会生成一个用户级配置，让 Yarn 不再验证远程仓库的 SSL 证书。**注意**：这会在安全性上打折扣（可能存在中间人攻击风险），但对于本地开发且处于可信网络环境时，可以接受。

---

### 🛠️ 备选方案（如果不想降低安全性）

#### 1. 添加自签名证书到系统信任链（推荐）
如果你知道代理使用的 CA 证书文件（例如 `ca.crt`），可以设置 `NODE_EXTRA_CA_CERTS` 环境变量：

```bash
export NODE_EXTRA_CA_CERTS=/路径/到/你的/ca.crt
yarn install
```

或者把证书添加到 Node.js 的全局信任库。

#### 2. 使用 `npm` 的 `cafile` 配置（Yarn 也会读取）
在用户目录的 `.npmrc` 文件中添加：
```
cafile=/路径/到/你的/ca.crt
```
或者通过命令设置：
```bash
npm config set cafile /路径/到/你的/ca.crt
```
Yarn 1.x 会继承 npm 的部分配置，因此可能有效。

#### 3. 更换为可信镜像源（跳过代理限制）
如果你访问官方 registry（`https://registry.yarnpkg.com`）被代理拦截，可以改用国内镜像源，它通常使用受信任的证书：

```bash
yarn config set registry https://registry.npmmirror.com
```
这样就不需要走代理了（如果镜像源可直接访问），也无需处理证书问题。

---

### 📌 验证与重置

- 查看当前 `strict-ssl` 状态：`yarn config get strict-ssl`
- 如果需要恢复默认（`true`），执行：`yarn config delete strict-ssl`

---

### 💡 建议
- **优先尝试更换镜像源**（方案3），因为既解决了证书问题，又可能提升下载速度。
- 如果必须使用代理且无法获取证书，则采用 `strict-ssl false` 临时解决。
- 打包发布时，建议不要将 `strict-ssl false` 写入项目配置文件（`.yarnrc`），以免影响其他同事。

执行完上述任一操作后，再次运行 `yarn install` 即可。若仍有问题，请检查代理设置是否正确（特别是 `http-proxy` 和 `https-proxy` 是否指向了代理地址）。










---------