要在 Neovim 中使用 Lazy 包管理器实现 C/C++ 代码补全，可以按照以下步骤进行配置：

# 1: 安装 Neovim 和 Lazy 包管理器
确保你已安装 Neovim 和 Lazy 包管理器。如果还没安装，可以按照以下步骤进行安装。
## 安装 Neovim
你可以通过包管理器或从源代码安装 Neovim。请参考 [Neovim 官方文档](https://neovim.io/)。
1. 下载`appimage` 
2. 在Linux中找到`appimage`文件，打开终端
3. 执行以下命令
```bash
sudo mv nvim-linux-x86_64.appimage /usr/local/bin/nvim
sudo chmod +x /usr/local/bin/nvim
```

- 这样就可以全局使用neovim了
## 安装 Lazy 包管理器
在你的 Neovim 配置目录（通常是 `~/.config/nvim`）中创建一个 `init.lua` 文件，然后添加以下内容：

```lua
-- bootstrap lazy.nvim
local lazypath = vim.fn.stdpath("data") .. "/lazy/lazy.nvim"
if not vim.loop.fs_stat(lazypath) then
  vim.fn.system({
    "git",
    "clone",
    "--filter=blob:none",
    "https://github.com/folke/lazy.nvim.git",
    "--branch=stable",
    lazypath,
  })
end
vim.opt.rtp:prepend(lazypath)

require("lazy").setup({
  -- 这里将添加你的插件
})
```

# 2: 安装补全插件

在 `require("lazy").setup({})` 中添加 C/C++ 代码补全相关的插件。例如，可以使用 `nvim-cmp` 和 `clangd` 作为补全引擎。

```lua
require("lazy").setup({
  -- nvim-cmp
  {
    "hrsh7th/nvim-cmp",
    dependencies = {
      "hrsh7th/cmp-nvim-lsp",
      "hrsh7th/cmp-buffer",
      "hrsh7th/cmp-path",
      "hrsh7th/cmp-cmdline",
    },
  },
  -- LSP
  {
    "neovim/nvim-lspconfig",
    config = function()
      require("lspconfig").clangd.setup{}
    end,
  },
  -- 其他你需要的插件
})
```

# 3: 配置 nvim-cmp
在你的 `init.lua` 中添加以下配置以设置 `nvim-cmp`：

```lua
local cmp = require'cmp'

cmp.setup({
  snippet = {
    expand = function(args)
      require('luasnip').lsp_expand(args.body) -- 使用 luasnip 作为片段引擎
    end,
  },
  mapping = {
    ['<C-n>'] = cmp.mapping.select_next_item(),
    ['<C-p>'] = cmp.mapping.select_prev_item(),
    ['<C-Space>'] = cmp.mapping.complete(),
    ['<C-e>'] = cmp.mapping.close(),
    ['<CR>'] = cmp.mapping.confirm({ select = true }),
  },
  sources = {
    { name = 'nvim_lsp' },
    { name = 'buffer' },
  },
})
```

# 4: 安装 Clangd

确保你已安装 `clangd`，可以通过包管理器进行安装：
```bash
sudo apt install clangd  # Ubuntu
brew install llvm        # macOS
```

# 5: 启动 Neovim 并测试
在终端中启动 Neovim，打开一个 C/C++ 文件，检查补全是否正常工作。

## 额外提示

- 确保在你的 C/C++ 项目中有 `compile_commands.json` 文件，`clangd` 会利用它来提供更好的补全体验。
- 你可以根据需要调整 `nvim-cmp` 和 `lspconfig` 的配置。

这样，你就可以在 Neovim 中使用 Lazy 包管理器实现 C/C++ 代码补全了！


----

# `SQL` 补全


1. 配置好LSP的C/C++补全
2. **nvim-cmp** 只是补全前端，SQL 和 Java 都需要对应的 **LSP 服务器**

## 安装SQL的LSP服务器

##### 需要go环境
1. 从[这个网站](https://go.dev/dl/go1.22.5.linux-amd64.tar.gz)下载go WSL下载**AMD64** 不是**ARM64** 
2. 解压到 `/usr/local`

`{bash}sudo tar -C /usr/local -xzf go1.22.5.linux-amd64.tar.gz`

4. 配置环境变量. 编辑 `~/.bashrc` 或 `~/.zshrc`，加入：

`{bash}export PATH=$PATH:/usr/local/go/bin export GOPATH=$HOME/go export PATH=$PATH:$GOPATH/bin`

让它生效：`{bash}saource ~/.bashrc`

5. 检查版本 `{bash}go version # 应该看到 go1.22.5 或更高`


##### 安装SQL的LSP服务器

- `{bash}go install github.com/sqls-server/sqls@latest` 
- 运行完, 在**Go 1.17 及以上版本的 `go install` 默认把可执行文件安装到 `$GOPATH/bin` 或 `$HOME/go/bin`**，而这个目录可能没有加到你的 `PATH`
- 添加到`PATH`: 
```bash
export GOPATH=$HOME/go
export PATH=$PATH:$GOPATH/bin
```

然后输入`sqls --version`应该就有输出了.

##### 配置LSP
在`plugins.lua`中, nvim-cmp与lsp插件:
```lua

    -- nvim-cmp 自动补全
    {
        "hrsh7th/nvim-cmp",
        dependencies = {
            "hrsh7th/cmp-nvim-lsp",
            "hrsh7th/cmp-buffer",
            "hrsh7th/cmp-path",
            "hrsh7th/cmp-cmdline",
        },
    },

    -- LSP
    {

	    ---
    "neovim/nvim-lspconfig",
    config = function()
        local lspconfig = require("lspconfig")
        local capabilities = require("cmp_nvim_lsp").default_capabilities()

        -- C/C++
        lspconfig.clangd.setup({
            capabilities = capabilities,
        })

        -- SQL
        lspconfig.sqls.setup({
            capabilities = capabilities,
            on_attach = function(client, bufnr)
                -- 如果你想用 sqls 内置命令，比如查询当前表结构
                local function buf_set_keymap(...) vim.api.nvim_buf_set_keymap(bufnr, ...) end
                buf_set_keymap("n", "<leader>se", "<cmd>SqlsExecuteQuery<CR>", { noremap = true, silent = true })
            end,
        })
    end,

	    ---

    -- "neovim/nvim-lspconfig",
    --    config = function()
    --       require("lspconfig").clangd.setup({
    --	cmd = { "clangd", "--compile-commands-dir=build", "-I../include" }
    --    })
    --    end,
    },


```

就可以完成基本补全了. 
- **限制数据库范围**  在 `.sqls.yml` 里配置 `schemas` 只加载你要用的几个库，减少不必要的元数据扫描
- **启用缓存**  `sqls` 会缓存表结构信息，不用每次补全都去数据库查询
- **必要时才启动**  在 Neovim 配置里，让 `sqls` 只在 `.sql` 文件打开时启动，避免全局常驻

```yml
datasources:
  - alias: local
    driver: mysql
    proto: tcp
    user: root
    passwd: password
    host: 127.0.0.1
    port: 3306
    dbname: mydb
    schemas:
      - mydb
```

如果只是写 SQL 脚本或偶尔连数据库，**sqls 占用很低**，可以放心常驻。  
但如果你连的是**超大数据库**，最好限制 schema 范围并开启缓存，否则启动和内存占用会明显上去



----

# go代理超时

```bash
go install github.com/sqls-server/sqls@latest
go: github.com/sqls-server/sqls@latest: module github.com/sqls-server/sqls: Get "https://proxy.golang.org/github.com/sqls-server/sqls/@v/list": dial tcp 142.251.215.241:443: i/o timeout
```

你这个错误是 **Go 在访问官方模块代理 `proxy.golang.org` 时超时**，
在国内（或者某些网络环境）这是常见的，因为 Google 服务器被限制访问。

解决方法有三类：

---

## **方法 1：换国内 Go 模块代理（最简单）**

Go 从 1.13 起支持配置 `GOPROXY` 环境变量。
你可以直接换成 **Goproxy.cn** 或 **七牛云代理**：

```bash
# 临时生效（当前终端）
export GOPROXY=https://goproxy.cn,direct

# 永久生效（加到 ~/.bashrc 或 ~/.zshrc）
echo 'export GOPROXY=https://goproxy.cn,direct' >> ~/.bashrc
source ~/.bashrc
```

然后再执行：

```bash
go install github.com/sqls-server/sqls@latest
```

---

## **方法 2：直接从 GitHub 拉源码手动编译**

```bash
git clone https://github.com/sqls-server/sqls.git
cd sqls
go build
# 把编译好的 sqls 放到 PATH 里
sudo mv sqls /usr/local/bin/
```

这样跳过了 Go 模块代理，直接从 GitHub 下载。

---

## **方法 3：用代理工具（全局网络代理）**

如果你有 Clash、V2Ray、白鲸等代理，可以：

```bash
export https_proxy=http://127.0.0.1:7890
export http_proxy=http://127.0.0.1:7890
```

（把端口换成你实际代理端口）
然后再 `go install ...`。

---

我建议你直接用 **方法 1** 改 `GOPROXY`，是最省事的。

---

如果你愿意，我可以帮你写一个 **Debian 一键脚本**，
直接完成：

1. 安装最新版 Go
2. 配置 `GOPROXY`
3. 安装 `sqls` 并加到 PATH
   这样就不用一步步处理了。



----