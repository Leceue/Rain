### Git Basic

---

| 指令                          | 说明                                      |
| :---------------------------- | :---------------------------------------- |
| `git init <directory>`        | 初始化git，即创建一个新的git              |
| `git clone <repo>`            | 克隆项目到本地（HTTP或者SSH）             |
| `git config user.name <name>` | 配置用户名                                |
| `git add <directory>`         | 将修改加入缓冲区                          |
| `git commit -m "<message>"`   | 提交缓存区的修改                          |
| `git status`                  | 显示文件状态，哪些文件被staged和untracked |
| `git log`                     | 显示commit记录                            |



### Git Diff

---

| 指令                | 说明                             |
| ------------------- | -------------------------------- |
| `git giff`          | 比较缓存区和工作区的修改         |
| `git diff HEAD`     | 比较工作区和上一次commit后的修改 |
| `git diff --cached` | 比较缓存区和上一次commit         |



### Undong Changes

---

| 指令                  | 说明                  |
| --------------------- | --------------------- |
| `git revert <commit>` | 撤销上次commit        |
| `git reset <file>`    | 将\<file>从缓存区移除 |



### Git Branches

---

| 指令                     | 说明                 |
| ------------------------ | -------------------- |
| `git branch`             | 显示所有分支         |
| `git switch -c <branch>` | 创建分支并切换到当前 |
| `git merge <branch>`     | 合并分支至当前       |



### Remote Repositories

---

| 指令                          | 说明                                   |
| ----------------------------- | -------------------------------------- |
| `git remote add <name> <url>` | 添加新的远程连接                       |
| `git fetch <remote> <branch>` | 从指定remote中抓取branch               |
| `git pull <remote>`           | 从指定remote抓取所有commit并合并到分支 |
| `git push <remote> <branch>`  | 将branch推送到指定remote               |



### 多人协作

----

1. 用 `git push origin <branch-name> `推送修改；

2. 推送失败可能是远程分支比本地更新，需要用 `git pull` 试图合并；
3. 如果合并冲突，解决后本地提交；
4. 再次推送即可

用建立本地分支和远程分支的链接 `git branch --set-upstream-to <branch-name> origin/<branch-name>`

​	远程库信息`git remote -v`



`git rebase` 可以将本地未push的分叉整理为直线

