# 将本地文件夹 `rpGit` 作为 Git 仓库上传到 GitHub，并同时创建远程仓库、添加 `README.md` 文件


# 1. 在 GitHub 上创建远程仓库
1. 登录到 [GitHub](https://github.com/)
2. 在页面右上角点击 **+**，然后选择 **New repository**
3. 填写仓库名称（比如 `rpGit`）和描述，并选择初始化仓库的选项：
    - **Initialize this repository with a README**: 不勾选，因为我们将本地文件夹中的 `README.md` 上传
    - **Add .gitignore**: 如果需要可以选择相应的模板
    - **Choose a license**: 选择合适的开源协议，比如 `MIT License`
4. 点击 **Create repository**

----

# 2. 将本地文件夹初始化为 Git 仓库
1. 打开终端或命令行工具，进入到本地文件夹 `rpGit` 目录：`{bash}cd /path/to/rpGit`
2. 初始化本地 Git 仓库：`{bash}git init`
3. 添加远程仓库 URL（在 GitHub 上创建仓库时会提供）：
```bash
git remote add origin https://github.com/nanno35578/rpGit.git

git fetch # 拉取最新状态
git switch main # 切换到默认main分支
```
4. 拉取远程仓库的内容到本地：`{bash}git pull origin main`. 其实`{bash}git pull`即可, 在切换到main之后.

> 注意：这里假设远程仓库的默认分支是 `main`，如果是 `master`，则需要使用 `git pull origin master`

- 如果在创建仓库时选中License, pull后就会拉取到本地仓库.

# 3. 创建 `README.md` 文件
1. 创建 `README.md` 文件并编辑：`{bash}echo "# rpGit" > README.md`

----

# 4. 提交文件并上传到 GitHub
2. 添加所有文件到 Git 暂存区：`{bash}git add .`
3. 提交文件：`{bash}git commit -m "Initial commit with README"`
4. 上传到 GitHub 远程仓库：`{bash}git push -u origin master`
如果使用的是 GitHub 新的默认 `main` 分支而不是 `master`，则需要使用：`{bash}git push -u origin main` 

#####  5. 验证上传
打开你 GitHub 上的仓库页面，应该可以看到上传的 `README.md`文件。

-------

# 其他问题

> [!abstract] 若步骤2 出现如下输出:
> ```bash
> usr24@Thatchy linux ≡ƒÿâ $ git commit -m "add Linux nvim tmux bash"
> Author identity unknown
> 
> *** Please tell me who you are.
> 
> Run
> 
> git config --global user.email "you@example.com"
> git config --global user.name "Your Name"
> 
> to set your account's default identity.
> Omit --global to set the identity only in this repository.
> 
> fatal: empty ident name (for <usr24@Thatchy.>) not allowed
> ```

你这个报错意思很明确：
Git 不知道你是谁（`Author identity unknown`），而且你当前 Linux 环境的用户名（`usr24`）和主机名（`Thatchy`）拼出来的 email `<usr24@Thatchy.>` 也不合法（域名部分少了个东西），所以它拒绝提交

##### 说明git用户信息未设置. 
即在向Github提交的时候用到的用户信息等, 至少要有：
 `user.name`：你的名字（可以是昵称）
 `user.email`：你的邮箱（随便写个合法格式就行）

- *全局设置*（所有仓库生效）
```bash
git config --global user.name "hhhhnxsm"
git config --global user.email "3273108824@example.com"
```
- *仅当前仓库生效* 如果只想给当前项目设置：不加`--global`选项
- 验证是否生效: `{bash}git config user.name; git config user.email`


> [!bug] 若步骤3出现如下输出, 是你的身份还未经过认证, GitHub不知道你是谁, 有没有权限对仓库内容修改:
> 
> ```bash
> usr24@Thatchy linux ≡ƒÿà $ git branch -m main
> usr24@Thatchy linux ≡ƒÿâ $ git status
> On branch main
> nothing to commit, working tree clean
> usr24@Thatchy linux ≡ƒÿâ $ git push -u origin main
> Missing or invalid credentials.
> Error: connect ECONNREFUSED /run/user/1000/vscode-git-e0e88eb351.sock
>     at PipeConnectWrap.afterConnect [as oncomplete] (node:net:1636:16) {
>   errno: -111,
>   code: 'ECONNREFUSED',
>   syscall: 'connect',
>   address: '/run/user/1000/vscode-git-e0e88eb351.sock'
> }
> Missing or invalid credentials.
> Error: connect ECONNREFUSED /run/user/1000/vscode-git-e0e88eb351.sock
>     at PipeConnectWrap.afterConnect [as oncomplete] (node:net:1636:16) {
>   errno: -111,
>   code: 'ECONNREFUSED',
>   syscall: 'connect',
>   address: '/run/user/1000/vscode-git-e0e88eb351.sock'
> }
> remote: No anonymous write access.
> fatal: Authentication failed for 'https://github.com/NANNO35578/Configs.git/'
> ```

##### 这是典型的 *Git 远程认证失败*.
 这个错误其实分两部分:
1. visual studio Code 内置 Git 代理连接失败`{bash}Error: connect ECONNREFUSED /run/user/1000/vscode-git-xxxx.sock` 
	* 这是 visual studio Code 的 Git 扩展试图通过它的 socket 代理做身份认证，但这个进程没运行（可能是 WSL / 远程开发环境）
	* 这个不影响你用命令行直接推送，只是 visual studio Code 里的 Git 功能暂时用不了
	
2. GitHub 拒绝匿名写入 (最后两行)
	* 说明你没有提供任何有效的用户名/密码（或 token）
	* HTTPS 模式下，GitHub 不再接受账号密码登录，必须用 **Personal Access Token (PAT)**
	* 也可能是你还没在本地保存过认证信息


 ##### 解决方法 1：用 SSH推荐
建议用 **SSH 方式**，配一次之后所有仓库都能直接推，不用每次输 token
1. 生成 SSH key（如果没有的话）： `{bash}ssh-keygen -t ed25519 -C "你的GitHub邮箱"`
   一路回车即可（默认保存在 `~/.ssh/id_ed25519`）

2. 将公钥添加到 GitHub： `{bash}cat ~/.ssh/id_ed25519.pub`
   复制输出内容 → [GitHub Settings → SSH and GPG keys → New SSH key](https://github.com/settings/keys)

3. 把远程 URL 改成 SSH 形式：`{bash}git remote set-url origin git@github.com:NANNO35578/Configs.git`

4. 测试： `{bash}ssh -T git@github.com`
   如果显示：`{bash}Hi NANNO35578! You've successfully authenticated, but GitHub does not provide shell access.`就成功了

5. 推送：`{bash}git push -u origin main`

 ##### 方法 2：HTTPS + PAT
1. 在 GitHub 上生成一个 PAT：
   * 进入 [GitHub Settings → Developer settings → Personal access tokens → Tokens (classic)](https://github.com/settings/tokens)
   * 选择 **Generate new token (classic)**
   * 选中 `repo` 权限，设置有效期
   * 生成后复制 token（**只会显示一次**）

2. 本地执行：`{bash}git config --global credential.helper store`   这样 Git 会记住你输入的凭据

3. 推送时输入：
   * Username: 你的 GitHub 用户名
   * Password: 刚生成的 token（不是你的 GitHub 密码）

----


---

1. 切换分支
2. git checkout code
3. 添加分支
4. git checkout -b code1
5. 更改本地分支名
6. git branch -m hahaha





# 删除 Git 分支有两种情况：删除本地分支和删除远程分支。

### 1. 删除本地分支
*使用* `git branch -d` *删除本地分支* :  如果该分支已经合并到当前分支（或者你想确保分支已合并），可以使用 `-d`（小写）选项
- 删除本地分支：`{bash}git branch -d <branch_name>`
- **`-d`**：会确保分支已经被合并到当前分支，如果分支未合并，会提示你不能删除

*强制删除本地分支*（即使未合并） :  如果你确定要删除一个未合并的分支，可以使用 `-D`（大写）选项强制删除该分支：
- `{bash}git branch -D <branch_name>`

 ### 2. 删除远程分支
远程分支需要通过 `git push` 删除
*使用* `git push` *删除远程分支* : 删除远程分支的命令如下：`{bash}git push origin --delete <branch_name>`
- **`origin`**：远程仓库的名称（通常是 `origin`）。
- **`--delete`**：表示删除远程分支。
- **`<branch_name>`**：你要删除的远程分支名称。

> [!quote] 删除远程仓库的 `feature` 分支
`{bash}git push origin --delete feature`

 ### 3. 删除远程分支的本地追踪分支
当你删除远程分支后，本地仓库中仍然可能会保留该分支的追踪信息
你可以使用 `git fetch` 删除本地的远程追踪分支信息： `{bash}git fetch -p`
- **`-p`**（或 **`--prune`**）：删除所有已被删除的远程分支的追踪信息

 ### 4. 检查是否成功删除分支
*查看本地分支*：`{bash}git branch` 
*查看远程分支*：`{bash}git branch -r` 
### 总结 
- 删除 **本地分支**：
    - 合并过的分支：`git branch -d <branch_name>`
    - 强制删除未合并分支：`git branch -D <branch_name>`
- 删除 **远程分支**：
    - `git push origin --delete <branch_name>`
- 清理 **本地远程追踪分支**：
    - `git fetch -p`



----

# `git` 添加子模块

现有仓库calcite和calcite_server两个仓库, 要将calcite_server作为子模块添加到calcite, 命令如下

```sh
# 在calcite的本地仓库:
git submodule add https://github.com/NANNO35578/calcite_server.git calcite_server
git commit -m "Add module: server"
# 在calcite拉取最新的server:
git submodule update --init --recursive
```

----

# `git` 撤销一次commit

```sh
git add .
git commit -m "haha"
```

现在改了一些东西, 然后commit了, 但是发现改错了, 想撤销一次, 重新commit

```sh
git reset --soft HEAD~1
git add .
git commit -m "haha"
```