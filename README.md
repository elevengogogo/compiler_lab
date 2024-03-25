# compiler_lab
编译原理实验作业用于团队协作

目前已提交第一次实验



实验概述：

第一次课上讨论决定，我们组选择MIPS，使用c++



github仓库：

仓库HTTPS：https://github.com/elevengogogo/compiler_lab

仓库SSH：git@github.com:elevengogogo/compiler_lab.git



仓库结构：

每次实验一个文件夹，其中放有实验说明、实验代码和实验分工（我会根据每次实验内容制定分工）以及实验报告（虽然每个人都要提交一份但方便大家彼此共同完成报告还是可以共享一下）

因为老师之前说每次实验可能会在上一次实验基础上完善，所以版本记录每次代码命名为compilerlab1、lab2...

![截屏2024-03-11 14.22.41](/Users/shiyihang/Library/Application Support/typora-user-images/截屏2024-03-11 14.22.41.png)



使用方法：

由于大家都有github账户，并在电脑上配置过git。

**如果没有设置过SSH的话**还需要在本地计算机上设置相应的 SSH 公钥并具有相应的权限：

要使用 SSH 进行 Git 操作，需要完成以下步骤：

1. 在本地计算机上生成 SSH 密钥对，其中包括公钥和私钥。可以使用以下命令生成 SSH 密钥对：

   ```
   ssh-keygen -t rsa -b 4096 -C "your_email@example.com"
   ```

   在生成 SSH 密钥对时，会提示输入一个密码以保护私钥。如果不想设置密码，可以直接按回车键跳过。

2. 将生成的 SSH 公钥添加到 GitHub 账户中。可以复制并粘贴公钥内容到 GitHub 设置页面中的 SSH and GPG keys 部分。

**如果已经设置过**只需要提供给我SSH公钥，就可以给大家添加到仓库：

可以通过以下命令来获取：

```
cat ~/.ssh/id_rsa.pub
```

这会打印 SSH 公钥到终端或命令行窗口中。



添加后，就可以使用我们的仓库的 SSH URL 进行拉取和上传操作

1. 通过 SSH URL 克隆仓库：

   ```
   git clone git@github.com:elevengogogo/compiler_lab.git
   ```

   其中，`git@github.com:elevengogogo/compiler_lab.git` 是我们的仓库的 SSH URL。

2. 可以使用类似以下的命令来拉取更新或推送变更到仓库中：

   ```
   git pull origin main
   git push origin main
   ```



（以下步骤粘贴自chatgpt：

要将本地计算机上的文件夹与 GitHub 仓库远程连接，并能够拉取、上传和同步文件，你可以按照以下步骤进行操作：

1. 首先，确保你已经在本地计算机上安装了 Git，并且已经在 GitHub 上创建了一个仓库。

2. 在本地计算机上，打开命令行或终端，并导航到你希望将仓库克隆到的文件夹位置。

3. 使用 `git clone` 命令将 GitHub 仓库克隆到本地计算机上。命令格式如下：

   ```
   git clone 仓库地址
   ```

   其中，仓库地址是你在 GitHub 上创建的仓库的 URL。

4. 进入克隆下来的仓库文件夹，将你的文件复制或移动到该文件夹下。

5. 使用以下命令添加你的文件到 Git 中：

   ```
   git add .
   ```

   这会将所有变更添加到 Git 缓存区。

6. 使用以下命令提交你的变更到本地仓库：

   ```
   git commit -m "提交说明"
   ```

   请将提交说明替换为你对这次提交的简短描述。

7. 使用以下命令将你的变更推送到 GitHub 上的远程仓库：

   ```
   git push origin main
   ```

   这会将你的本地变更推送到 GitHub 上的主分支（假设你使用的是 `main` 分支，如果你的默认分支是其他名称，请将命令中的 `main` 替换为你的默认分支名称）。

8. 如果你想从 GitHub 上的远程仓库拉取最新的变更到本地，可以使用以下命令：

   ```
   git pull origin main
   ```

   这会将 GitHub 上的最新变更拉取到你的本地仓库中。

通过执行以上步骤，你就可以将本地计算机上的文件夹与 GitHub 仓库远程连接，并能够进行拉取、上传和同步文件。

