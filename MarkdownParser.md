# MarkdownParser

### 一、实验介绍

#### 实验内容

Markdown 已经是程序员的标配，其语法简单的特点让我们能够更加专注于内容输出与写作。本节实验我们完成对于项目的`基本设计和原理`
本次项目我们将针对 Markdown 的一些最常用语法，手动实现一个 Markdown 解析器，作为展示，还将为文档生成目录，如图所示：

![此处输入图片的描述](https://doc.shiyanlou.com/document-uid29879labid1958timestamp1469785718093.png/wm)

#### 知识点

- 词法分析技术
- 语法树
- DFS 深度优先搜索
- C++11
- 使用指针进行字符流处理

#### 适合人群

适合人群：本课程适合有C++语言，了解markdown，然后有动手能力的同学学习。

#### 代码获取

```
# 本节实验完整代码
http://labfile.oss.aliyuncs.com/courses/569/MarkdownParser.zip
# 在这里下载好之后，后面的不用一个个下载了
```

我们的整个项目目录结构树为：

```
MarkdownParser
├── main.cpp
├── output
│   └── github-markdown.css
└── test.md
```

### 二、实验原理

#### Markdown 常用语法

Markdown 的语法非常简单，下面是一些常用的语法：

```
## 标题

# 一级标题
## 二级标题
### 三级标题

## 强调
- **粗体**
- _斜体_

## 引用
> 引用内容

## 列表
- 列表内容7
- 列表内容8
- 列表内容9
1. 列表内容10
2. 列表内容11
3. 列表内容12

## 代码

代码的插入方式 `printf('\n');`
​```
function method()
{
    alert("javascript");
}
​```
## 链接
行内连接 [github](https://github.com/) 的链接

## 图片
![shiyanlou logo](https://static.shiyanlou.com/img/logo_03.png)
```

可以看到，Markdown 的语法之所以不复杂，是因为它的大部分词法规则都位于行首。这就为我们快速编写一个简单的 Markdown 解析器提供了方便之处。

唯一需要考虑的，就是诸如链接、图片、代码、粗体、斜体等这些特殊的标记。

### 三、测试驱动



为了更加专注于编写 Markdown 解析器本身，我们先部署好相关测测试文件。

首先你可以输入：

```bash
http://labfile.oss.aliyuncs.com/courses/569/test.md
```

来获取 Markdown 的测试文件。

其次，为了测试 Markdown 的解析结果，我们很自然的想到把解析结果直接转换到 HTML 进行展示，所以，这里已经为你准备好了好看的 CSS 文件，你可以先下载：

```bash
wget http://labfile.oss.aliyuncs.com/courses/569/github-markdown.css
```

为了让我们的代码便于测试，我们先编写好主函数：

```cpp
//
//  main.cpp
//  MarkdownParser
//

#include <fostream>         // std::ofstream
#include "mdtransform.hpp"  // 需要实现的 Markdown 解析类

int main() {
    // 装载构造 Markdown 文件
    MarkdownTransform transformer("test.md");

    // 编写一个 `getTableOfContents()` 方法来获取 Markdown 文件 HTML 格式的目录
    std::string table = transformer.getTableOfContents();

    // 编写一个 `getContents()` 方法来获取 Markdown 转成 HTML 后的内容
    std::string contents = transformer.getContents();

    // 准备要写入的 HTML 文件头尾信息
    std::string head = "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <link rel=\"stylesheet\" href=\"github-markdown.css\">\
        </head><body><article class=\"markdown-body\">";
    std::string end = "</article></body></html>";

    // 将结果写入到文件
    std::ofstream out;
    out.open("output/index.html");
    // 将转换后的内容构造到 HTML 的 <article></article> 标签内部
    out << head+table+contents+end;
    out.close();
    return 0;
}
```

> 生成这个目录树的工具叫做 `tree`, 如果你感兴趣可以使用
>
> ```bash
> sudo apt-get install tree
> ```
>
> 来获取这个命令工具，使用方法如图所示：
>
> ![此处输入图片的描述](https://doc.shiyanlou.com/document-uid29879labid1958timestamp1469785260448.png/wm)



























































