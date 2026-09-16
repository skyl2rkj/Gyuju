# Github

github是一个项目托管网站，大量的企业级工程和开源工程在网站云端托管，聚集海量项目，软件研发工程师的工具网站。

## 一、仓库与分支

- 仓库：是github中项目（工程）的存储单位，一个用户可以创建多个仓库，每个仓库存一个项目
- 分支：是资源的存储单位，一般一个仓库中有一个默认的主分支（master），上传代码数据默认放到主分支中，仓库包含分支，一个仓库可以有多个分支，主分支只有一个。

### 关于查询

按用户关键字进行全站的模糊查询，注意下面的语言标签
标签查询，(样本) `xxx sample` (资料) `xxxx tutorial`

### 关于仓库中的项目

- code：存储工程资源数据，代码、配置文件等等
- README.md：markdown文本修饰语言，后缀统一为`.md`，使用修饰符与正文结合成一个好的视觉效果。
- LISTENSE：关于许可证的，GPL3.0 APHACHE2.0 MIT 给使用者最大使用权利，最小的限制，如果不是知名机构许可证，要详细阅读条款（避免法务问题）
- issues：问答板块，提问和解决问题

## 二、git基本配置

用个人电脑，向云端（github仓库）上传数据，需要进行设备认证，让github账户认为此设备是受信任的，后续完成加密传输

### 1.设备认证

- 创建一个本地仓库 `git init`
    当前位置有 (Master) 表示再仓库所在位置，后续的所有操作都是在这个位置
- `git confit --list` 查看git配置文件信息
- 在配置文件中添加两条配置，关于email和username

```bash
 git config --global user.name "用户名"
 git config --global user.email "邮箱"
```

- 查看是否配置成功

```bash
ssh -T git@github.com
```

- 密钥生成，创建本机RSA 非对称加密密钥，并且传给账户，后续使用此密钥完成数据加密传输。

```bash
ssh-leygen -t rsa -C "邮箱"
```

- 记录一下密钥文件的存储位置，等会去复制密钥串
    - 粘贴位置，点击头像->menu->Settings->ssh and GPG keys-> new ssh key->填写密钥名->粘贴密钥->add ssh key
        *2.为仓库地址创建别名

```bash
git remote add origin "ssh地址" #为云端仓库ssh地址创建别名,叫origin
git remote remove origin #删除origin地址别名
```

### 关于项目管理(内容的上传下载)

- 使用git bash 进行本地内容的上传,通过命令方式,数据更新/版本更新

### 工作流程示意

- 1.资源文件(如test.c)->git缓冲区`git add source` `git rm source` `git restore`
- 2.git缓冲区->本地仓库`git commit -吗"提交说明"`
- 3.本地仓库->云端仓库 `git push origin master` 版本更新,本地的新版本,同步合并云端的旧版本
    将本地仓库数据push推到云端
    - 上传的分支与云端仓库主分支同名,进行合并替换.
    - 不同名,云端仓库创建新分支,保存用户上传内容.

```bash
git add file #资源添加到缓冲区
git rm file #资源从缓冲区删除同时删除文件
git restore file #将资源从缓冲区还原到磁盘中
git status #查看缓冲区状态
git commit -m "提交说明" # 将缓冲区数据提交到本地仓库，可以附加说明信息
git push origin master 将本地仓库主分支数据 推到origin 指向的云端仓库中
```

## markdown 文本修饰语言

使用修饰符对正文进行修饰，让正文内容附带各种效果

1. 标题修饰符，多级标题
2. 正文及换行符
3. 文本修饰符
4. 列表（无序列表，有序列表）
5. 引用修饰符
6. 超链接，图片（本地、和网络图片）
7. 表格
8. 插入代码片段