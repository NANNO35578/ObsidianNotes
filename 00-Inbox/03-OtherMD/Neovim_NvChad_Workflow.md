# 安装Neovim
## 从源码编译安装
`[!!quote:Requirements]` : `cmake`, `git`  

```sh title:"安装命令"
# 克隆到当前文件夹下
git clone https://github.com/neovim/neovim.git
cd neovim
git checkout release-0.11  # 切换到0.11版本分支
make CMAKE_BUILD_TYPE=RelWithDebInfo
sudo make install
# 检查是否成功安装
nvim --version
```


# 安装其他工具
## `tree-sitter-cli`

- 若没有cargo, 可使用npm, npm还没有就得装个npm. npm用到还挺多的.

```sh title:"cargo 安装tree-sitter-cli"
cargo install --locked tree-sitter-cli

# 或者使用npm安装
npm install tree-sitter-cli
```

## 下载一个Nerd Font
推荐[Maple Mono](https://github.com/subframe7536/maple-font). GitHub右侧栏有个release, 点进去可以看见下载链接.

![[Pasted image 20260124151013.png]]


# 安装NvChad

[NvChad官网](https://nvchad.com/). [GitHub主页](https://github.com/NvChad/NvChad). [Wiki](https://nvchad.com/docs/quickstart/install).

一行命令:
`{sh}git clone https://github.com/NvChad/starter ~/.config/nvim && nvim`

然后就进入了neovim, 这时候等加载好了(需要下载一些neovim插件), 再输入两个neovim命令, 如下:
- Run `:MasonInstallAll` and `:TSInstallAll` command after lazy.nvim finishes downloading plugins.



# 修改主题

有个leader键, 相当于win, command键, 可以快捷启动一些东西. 默认是space也就是空格键.

切换主题: `<leader>th`
打开文件浏览器: `<leader>e`
