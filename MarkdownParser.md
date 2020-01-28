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



### 四、mdtransform 设计

我们要解析 Markdown 文件，自然就要先在程序中定义好相关的词法信息、语法规则，这样才能后为后续的转换工作提供铺垫。

如果你对编译原理有所了解，就能够立刻明白对 Markdown 的解析，其实就是编译过程中的词法分析和语法分析阶段，而最后的到 HTML 的转换，实际上就是『目标代码的生成』这个阶段。

幸运的是，Markdown 的语法是在是太过于简单，它所支持的这些语法，几乎完全不需要我们对它的语义进行分析，这就省去了语义分析、代码优化等这些阶段。使得整个 Markdown 解析器变得非常的简单。

首先，我们来进行词法分析。我们要规划好需要实现的相关词法，也就是常说的设计关键字 Token。

1. `#` 就是 Markdown 的标志性关键字，它的个数直接决定了标题的类型，因此，我们可以非常简单的直接将其转换层 `` 类型的 HTML 标签；
2. 正常的段落，没有任何标志性的说明，可以直接用 `` 进行包裹；
3. `[]()`, `![]()` 这两个语法能够让我们在 Markdown 文件中插入超链接和图片，因此可以分别使用 `[...]( )` 和 ``
4. 两个 ` 可以包裹一个行内的代码，而三个 ` 则可以包裹一块代码段，可以分别使用 `` 和 `` 来进行替换
5. `>` 这个关键字用来处理引用的内容，可以使用 `` 标签
6. `-`, `1.` 这些列表形式的关键字，可以使用 `` 和 `` 进行包裹，最后将整个内容使用 `` 进行包装。
7. `_` 和 `*` 这两个关键字分别有进行斜体和加粗强调的作用，在 HTML 中对应 `` 和 `` 两个标签。

至此，以上七点的分析足以涵盖最常用的 Markdown 语法，我们将对这部分内容进行解析，定义 Token 如下：

```
* 0: null                          |开始
* 1: <p>                           |段落
* 2: <a href=" ">...</a>           |超链接
* 3: <ul>                          |无序列表
* 4: <ol>                          |有序列表
* 5: <li>                          |列表
* 6: <em>                          |斜体
* 7: <strong>                      |加粗
* 8: <hr />                        |水平分割线
* 9: <br />                        |换行
* 10: <img alt="" src="" />        |图片
* 11: <blockquote>                 |引用
* 12: <h1>                         |h1
* 13: <h2>                         |h2
* 14: <h3>                         |h3
* 15: <h4>                         |h4
* 16: <h5>                         |h5
* 17: <h6>                         |h6
* 18: <pre><code>                  |代码段
* 19: <code>                       |行内代码
```

根据前面的测试代码，我们可以将 `MarkdownTransform` 类设计成如下的形式：

```cpp
#ifndef MD2HTML
#define MD2HTML

#include <string>

// 词法关键字枚举
enum{
    maxLength       = 10000,
    nul             = 0,
    paragraph       = 1,
    href            = 2,
    ul              = 3,
    ol              = 4,
    li              = 5,
    em              = 6,
    strong          = 7,
    hr              = 8,
    br              = 9,
    image           = 10,
    quote           = 11,
    h1              = 12,
    h2              = 13,
    h3              = 14,
    h4              = 15,
    h5              = 16,
    h6              = 17,
    blockcode       = 18,
    code            = 19
};
// HTML 前置标签
const std::string frontTag[] = {
    "","<p>","","<ul>","<ol>","<li>","<em>","<strong>",
    "<hr color=#CCCCCC size=1 />","<br />",
    "","<blockquote>",
    "<h1 ","<h2 ","<h3 ","<h4 ","<h5 ","<h6 ", // 右边的尖括号预留给添加其他的标签属性, 如 id
    "<pre><code>","<code>"
};
// HTML 后置标签
const std::string backTag[] = {
    "","</p>","","</ul>","</ol>","</li>","</em>","</strong>",
    "","","","</blockquote>",
    "</h1>","</h2>","</h3>","</h4>","</h5>","</h6>",
    "</code></pre>","</code>"
};

class MarkdownTransform{
private:
    string content, TOC;
public:
    // 构造函数
    MarkdownTransform(const std::string &filename);

    // 获得 Markdown 目录
    std::string getTableOfContents() { return TOC; }
    // 获得 Markdown 内容
    std::string getContents() { return content; }

    // 析构函数
    ~MarkdownTransform();
};
#endif
```

下一节实验中，我们将实现这个 Markdown 解析器。

### 五、解析

#### 5.1 解析的实现

在上节实验中，我们已经确定好了 `getTableOfContents()` 和 `getContents()` 这两个获取解析内容的方法——它们直接从 `TOC` 和 `content` 这两个变量中获取被解析成 HTML 的内容。所以整个 Markdown 的解析工作就落在了构造函数的身上。

如同往常一样，我们的构造函数接口给定的是需要解析的文件名，那么我们自然就需要从文件流中读取。

在正式实现之前，我们需要思考一个问题，那就是如何来存储我们解析出来的内容。

#### 5.2 基本的结构及成员

首先我们应该明确的一个概念就是，Markdown 其实和 HTML 类似，他们都有类似 DOM 树的结构，这也就不可避免的让我们需要去实现一个树结构，为此：

```cpp
// 
// mdtransform.hpp
//

// 保存目录的结构
typedef struct Cnode {
    vector <Cnode *> ch;
    string heading;
    string tag;
    Cnode (const string &hd): heading(hd) {}
} Cnode;

// 保存正文内容的结构
typedef struct node {
    int type;                           // 节点代表的类型
    vector <node *> ch;
    string elem[3];                     // 用来存放三个重要的属性, elem[0] 保存了要显示的内容
                                        // elem[1] 保存了链接, elem[2] 则保存了 title
    node (int _type): type(_type) {}
} node;
```

毫无疑问，为了让整个解析顺利的进行，我们需要在 `MarkdownTransform` 类 中增加如下私有属性：

```cpp
private:
    node  *root, *now;
    Cnode *Croot;
```

同时，为了让目录能够正确的索引到 HTML 的内容位置，我们不妨引入一个 `cntTag` 的变量进行记录：

```cpp
int cntTag = 0;
```

并与此同时，设置一个缓存数组，来缓存要处理的行：

```cpp
char s[maxLength];
```

其中，`#define maxLength 10000`。

### 六、具体实现

#### 6.1 处理每行开始处的一些空格和 Tab

空格其实并不影响我们整个 Markdown 的解析，所以最好的办法就是统一将每行开始处的空格和 Tab 进行一个预先处理。

```cpp
// 
// mdtransform.hpp
//

// 开始解析一行中开始的空格和 Tab
// src: 源串
// 返回值: 由空格数和有内容处的 char* 指针组成的 std::pair
inline pair<int, char *> start(char *src) {
    // 如果该行内容为空，则直接返回
    if ((int)strlen(src) == 0)
        return make_pair(0, nullptr);
    // 统计空格键和 Tab 键的个数
    int cntspace = 0, cnttab = 0;
    // 从该行的第一个字符读其, 统计空格键和 Tab 键,
    // 当遇到不是空格和 Tab 时，立即停止
    for (int i = 0; src[i] != '\0'; i++) {
        if (src[i] == ' ') cntspace++;
        else if (src[i] == '\t') cnttab++;
        // 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
        // 其中, 一个 Tab = 四个空格
        else return make_pair(cnttab + cntspace / 4, src + i);
    }
    return make_pair(0, nullptr);
}
```

> 此处感谢 wangerrr 在问答区中

> https://www.shiyanlou.com/questions/5507

> 指出原代码的错误, 现已更新.

#### 6.2 行类型判断

Markdown 的解析简单，就简单在此。关键性的语法关键词其实都是位于每行的行首的。 比如标题的 `#`，又比如列表的 `-` 符号。

```cpp
// 
// mdtransform.hpp
//

// 判断当前行的类型
// src: 源串
// 返回值: 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
inline pair<int, char *> JudgeType(char *src) {
    char *ptr = src;

    // 跳过 `#`
    while (*ptr == '#') ptr++;

    // 如果出现空格, 则说明是 `<h>` 标签
    if (ptr - src > 0 && *ptr == ' ')
        return make_pair(ptr - src + h1 - 1, ptr + 1);

    // 重置分析位置
    ptr = src;

    // 如果出现 ``` 则说明是代码块
    if (strncmp(ptr, "```", 3) == 0)
        return make_pair(blockcode, ptr + 3);

    // 如果出现 * + -, 并且他们的下一个字符为空格，则说明是列表
    if (strncmp(ptr, "- ", 2) == 0)
        return make_pair(ul, ptr + 1);

    // 如果出现 > 且下一个字符为空格，则说明是引用
    if (*ptr == '>' && (ptr[1] == ' '))
        return make_pair(quote, ptr + 1);

    // 如果出现的是数字, 且下一个字符是 . 则说明是是有序列表
    char *ptr1 = ptr;
    while (*ptr1 && (isdigit(*ptr1))) ptr1++;
    if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ')
        return make_pair(ol, ptr1 + 1);

    // 否则，就是普通段落
    return make_pair(paragraph, ptr);
}
```

#### 6.3 类型获取

```cpp
// 
// mdtransform.hpp
//

// 判断是否为标题
inline bool isHeading(node *v) {
    return (v->type >= h1 && v->type <= h6);
}
// 判断是否为图片
inline bool isImage(node *v) {
    return (v->type == image);
}
// 判断是否为超链接
inline bool isHref(node *v) {
    return (v->type == href);
}
```

#### 6.4 节点搜索

当我们发现一个新的语法结构时，在造树的过程中，是有必要对我们需要的节点进行搜索的，通过给定一个深度，来找到我们想要查找的节点，所以我们需要实现这个逻辑：

```cpp
// 
// mdtransform.hpp
//

// 给定树的深度寻找节点
// depth: 树的深度
// 返回值: 找到的节点指针
inline node* findnode(int depth) {
    node *ptr = root;
    while (!ptr->ch.empty() && depth != 0) {
        ptr = ptr->ch.back();
        if (ptr->type == li)
            depth--;
    }
    return ptr;
}
```

#### 6.5 新增节点的插入

节点的插入分为两种类型，一种是向 Cnode 结构的树种插入目录，另一种则是向 `node` 结构的树种插入解析内容，插入这些结构时，为了让目录能够正确跳转到正文所对应的内容，所以我们设置了 `tag` 标签来标记这个目录所指向的内容

```cpp
// 
// mdtransform.hpp
//

void Cins(Cnode *v, int x, const string &hd, int tag) {
    int n = (int)v->ch.size();
    if (x == 1) {
        v->ch.push_back(new Cnode(hd));
        v->ch.back()->tag = "tag" + to_string(tag);
        return ;
    }

    if (!n || v->ch.back()->heading.empty())
        v->ch.push_back(new Cnode(""));
    Cins(v->ch.back(), x - 1, hd, tag);
}
```

而对于正文的部分，需要处理的复杂逻辑并不多，难度较大的属于如何在一个段落中处理图片和超链接的情况。这就意味着我们需要一个字符一个字符的进行判断和处理。这也是我们为什么在处理解析代码的时候，都是使用 C 风格的字符串以及字符指针来进行处理，而不是简单的使用 `std::string`。

```cpp
// 
// mdtransform.hpp
//

// 向指定的节点中插入要处理的串
// v: 节点
// src: 要处理的串
void insert(node *v, const string &src) {
    int n = (int)src.size();
    bool incode = false,
         inem = false,
         instrong = false,
         inautolink = false;
    v->ch.push_back(new node(nul));

    for (int i = 0; i < n; i++) {
        char ch = src[i];
        if (ch == '\\') {
            ch = src[++i];
            v->ch.back()->elem[0] += string(1, ch);
            continue;
        }

        // 处理行内代码
        if (ch == '`' && !inautolink) {
            incode ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(code));
            incode = !incode;
            continue;
        }

        // 处理加粗
        if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink) {
            ++i;
            instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
            instrong = !instrong;
            continue;
        }
        if (ch == '_' && !incode && !instrong && !inautolink) {
            inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
            inem = !inem;
            continue;
        }

        // 处理图片
        if (ch == '!' && (i < n - 1 && src[i + 1] == '[')
           && !incode && !instrong && !inem && !inautolink) {
            v->ch.push_back(new node(image));
            for (i += 2; i < n - 1 && src[i] != ']'; i++)
                v->ch.back()->elem[0] += string(1, src[i]);
            i++;
            for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                v->ch.back()->elem[1] += string(1, src[i]);
            if (src[i] != ')')
                for (i++; i < n - 1 && src[i] != ')'; i++)
                    if (src[i] != '"')
                        v->ch.back()->elem[2] += string(1, src[i]);
            v->ch.push_back(new node(nul));
            continue;
        }

        // 处理超链接
        if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
            v->ch.push_back(new node(href));
            for (i++; i < n - 1 && src[i] != ']'; i++)
                v->ch.back()->elem[0] += string(1, src[i]);
            i++;
            for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++)
                v->ch.back()->elem[1] += string(1, src[i]);
            if (src[i] != ')')
                for (i++; i < n - 1 && src[i] != ')'; i++)
                    if (src[i] != '"')
                        v->ch.back()->elem[2] += string(1, src[i]);
            v->ch.push_back(new node(nul));
            continue;
        }

        v->ch.back()->elem[0] += string(1, ch);
        if (inautolink) v->ch.back()->elem[1] += string(1, ch);
    }
    if (src.size() >= 2)
        if (src.at(src.size() - 1) == ' ' && src.at(src.size() - 2) == ' ')
            v->ch.push_back(new node(br));
}
```

#### 6.6 换行的处理和段落生成

是否换行在 Markdown 中十分重要，因为Markdown 支持使用 `---`进行人为分割，同时，在代码块中又不能破坏代码的属性，所以一行内容是否在 HTML 中需要换行，就变得十分的重要：

```cpp
// 
// mdtransform.hpp
//

// 判断是否换行
inline bool isCutline(char *src) {
    int cnt = 0;
    char *ptr = src;
    while (*ptr) {
        // 如果不是 空格、tab、- 或 *，那么则不需要换行
        if (*ptr != ' ' && *ptr != '\t' && *ptr != '-')
            return false;
        if (*ptr == '-')
            cnt++;
        ptr++;
    }
    // 如果出现 --- 则需要增加一个分割线, 这时需要换行
    return (cnt >= 3);
}
```

大部分情况下，我们都会拿到一个段落文本，所以我们不妨将这个逻辑抽离出来：

```cpp
// 
// mdtransform.hpp
//

inline void mkpara(node *v) {
    if (v->ch.size() == 1u && v->ch.back()->type == paragraph)
        return;
    if (v->type == paragraph)
        return;
    if (v->type == nul) {
        v->type = paragraph;
        return;
    }
    node *x = new node(paragraph);
    x->ch = v->ch;
    v->ch.clear();
    v->ch.push_back(x);
}
```

#### 6.7 遍历与生成

语法树的遍历是需要深度优先的，而对目录的深度遍历和正文内容的深度遍历逻辑并不一样，所以我们还需要分开对这两部分逻辑进行实现：

```cpp
// 
// mdtransform.hpp
//

void dfs(node *v) {
    if (v->type == paragraph && v->elem[0].empty() && v->ch.empty())
        return ;

    content += frontTag[v->type];
    bool flag = true;

    // 处理标题, 支持用目录进行跳转
    if (isHeading(v)) {
        content += "id=\"" + v->elem[0] + "\">";
        flag = false;
    }

    // 处理超链接
    if (isHref(v)) {
        content += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
        flag = false;
    }

    // 处理图片
    if (isImage(v)) {
        content += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] + "\" title=\"" + v->elem[2] + "\" />";
        flag = false;
    }

    // 如果上面三者都不是, 则直接添加内容
    if (flag) {
        content += v->elem[0];
        flag = false;
    }

    // 递归遍历所有
    for (int i = 0; i < (int)v->ch.size(); i++)
        dfs(v->ch[i]);

    // 拼接为结束标签
    content += backTag[v->type];
}
```

目录的遍历和正文内容的遍历差别在于，目录的展示方式是需要使用无需列表的形式被展示在 HTML 中，因此：

```cpp
// 
// mdtransform.hpp
//

void Cdfs(Cnode *v, string index) {
    TOC += "<li>\n";
    TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
    int n = (int)v->ch.size();
    if (n) {
        TOC += "<ul>\n";
        for (int i = 0; i < n; i++) {
            Cdfs(v->ch[i], index + to_string(i + 1) + ".");
        }
        TOC += "</ul>\n";
    }
    TOC += "</li>\n";
}
```

#### 6.8 关键构造逻辑

有了上面的基本操作的铺垫，下面我们来开始实现关键性的构造函数：

```cpp
// 
// mdtransform.hpp
//

// 构造函数
MarkdownTransform(const std::string &filename) {
    // 首先为文档的树结构进行初始化，并将当前指针 not 指向根节点
    Croot = new Cnode("");
    root = new node(nul);
    now = root;

    // 从文件流中读取文件
    std::ifstream fin(filename);

    // 默认不是新段落、默认不在代码块内
    bool newpara = false;
    bool inblock = false;

    // 直到读取到 eof 为止
    while (!fin.eof()) {
        // 从文件中获取一行
        fin.getline(s, maxLength);

        // 处理不在代码块且需要换行的情况
        if (!inblock && isCutline(s)) {
            now = root;
            now->ch.push_back(new node(hr));
            newpara = false;
            continue;
        }

        // std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
        // 计算一行中开始的空格和 Tab 数
        std::pair<int, char *> ps = start(s);

        // 如果没有位于代码块中, 且没有统计到空格和 Tab, 则直接读取下一行
        if (!inblock && ps.second == nullptr) {
            now = root;
            newpara = true;
            continue;
        }

        // 分析该行文本的类型
        std::pair<int, char *> tj = JudgeType(ps.second);

        // 如果是代码块类型
        if (tj.first == blockcode) {
            // 如果位于代码块中, 则 push 一个空类型的节点
            inblock ? now->ch.push_back(new node(nul)) : now->ch.push_back(new node(blockcode));
            inblock = !inblock;
            continue;
        }

        // 如果在代码块中, 直接将内容拼接到当前节点中
        if (inblock) {
            now->ch.back()->elem[0] += string(s) + '\n';
            continue;
        }

        // 如果是普通段落
        if (tj.first == paragraph) {
            if (now == root) {
                now = findnode(ps.first);
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            bool flag = false;
            if (newpara && !now->ch.empty()) {
                node* ptr = nullptr;
                for (auto i: now->ch) {
                    if (i->type == nul)
                        ptr = i;
                }
                if (ptr != nullptr)
                    mkpara(ptr);
                flag = true;
            }
            if (flag) {
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            now->ch.push_back(new node(nul));
            insert(now->ch.back(), string(tj.second));
            newpara = false;
            continue;
        }

        now = findnode(ps.first);

        // 如果是标题行, 则向其标签中插入属性 tag
        if (tj.first >= h1 && tj.first <= h6) {
            now->ch.push_back(new node(tj.first));
            now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
            insert(now->ch.back(), string(tj.second));
            Cins(Croot, tj.first - h1 + 1, string(tj.second), cntTag);
        }

        // 如果是无序列表
        if (tj.first == ul) {
            if (now->ch.empty() || now->ch.back()->type != ul) {
                now->ch.push_back(new node(ul));
            }
            now = now->ch.back();
            bool flag = false;
            if (newpara && !now->ch.empty()) {
                node* ptr = nullptr;
                for (auto i: now->ch) {
                    if (i->type == li) ptr = i;
                }
                if (ptr != nullptr) mkpara(ptr);
                flag = true;
            }
            now->ch.push_back(new node(li));
            now = now->ch.back();
            if (flag) {
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            insert(now, string(tj.second));
        }

        // 如果是有序列表
        if (tj.first == ol) {
            if (now->ch.empty() || now->ch.back()->type != ol) {
                now->ch.push_back(new node(ol));
            }
            now = now->ch.back();
            bool flag = false;
            if (newpara && !now->ch.empty()) {
                node* ptr = nullptr;
                for (auto i: now->ch) {
                    if (i->type == li) ptr = i;
                }
                if (ptr != nullptr) mkpara(ptr);
                flag = true;
            }
            now->ch.push_back(new node(li));
            now = now->ch.back();
            if (flag) {
                now->ch.push_back(new node(paragraph));
                now = now->ch.back();
            }
            insert(now, string(tj.second));
        }

        // 如果是引用
        if (tj.first == quote) {
            if (now->ch.empty() || now->ch.back()->type != quote) {
                now->ch.push_back(new node(quote));
            }
            now = now->ch.back();
            if (newpara || now->ch.empty()) now->ch.push_back(new node(paragraph));
            insert(now->ch.back(), string(tj.second));
        }

        newpara = false;

    }

    // 文件读取分析完毕
    fin.close();

    // 深入优先遍历整个语法树
    dfs(root);

    // 构造目录
    TOC += "<ul>";
    for (int i = 0; i < (int)Croot->ch.size(); i++)
        Cdfs(Croot->ch[i], to_string(i + 1) + ".");
    TOC += "</ul>";
}
```

#### 6.9 销毁逻辑

我们在创建整个语法树的过程中，都是使用了动态创建的手段，因此我们必须在析构函数中释放相关的内存。我们有 Cnode 和 node 两个结构，就意味着我们需要事先两套不同的释放机制吗？事实上并不需要，利用模板我们可以在一个函数内解决这个问题，这得益于我们在实现这两个结构的时候，都使用了同样的成员`ch`：

```cpp
// 
// mdtransform.hpp
//

// 递归销毁数节点
template <typename T>
void destroy(T *v) {
    for (int i = 0; i < (int)v->ch.size(); i++) {
        destroy(v->ch[i]);
    }
    delete v;
}
```

至此，我们已经实现了上一节实验中设计的测试驱动的全部需求，我们可以通过

```bash
g++ main.cpp -std=c++11
./a.out
```

这样的话，我们就可以打开 `index.html` 来查看最后生成的效果了：

![此处输入图片的描述](https://doc.shiyanlou.com/document-uid29879labid1959timestamp1470285206108.png/wm)

同时，我们可以点击目录中的标签，跳转到相应的内容位置。













































