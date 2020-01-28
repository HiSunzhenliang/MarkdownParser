# MarkDown书写规范

写文档好帮手，轻量级标记语言 Markdown

# 概述

## 起源

Markdown是[Daring Fireball](http://daringfireball.net/projects/markdown/syntax)的作者John Gruber发明，身为Blogger，可能他也觉得传统的HTML有些复杂吧，退一步来讲，若是一个毫无计算机基础的Blogger，根本也没必要去掌握那些复杂的标签语言知识。

## 宗旨

Markdown 的目标是实现「易读易写」，成为一种适用于网络的书写语言。

可读性，无论如何，都是最重要的。一份使用 Markdown 格式撰写的文件应该可以直接以纯文本发布，并且看起来不会像是由许多标签或是格式指令所构成。Markdown 语法受到一些既有 text-to-HTML 格式的影响，包括 Setext、atx、Textile、reStructuredText、Grutatext 和 EtText，而最大灵感来源其实是纯文本电子邮件的格式。

总之， Markdown 的语法全由一些符号所组成，这些符号经过精挑细选，其作用一目了然。比如：在文字两旁加上星号，看起来就像*强调*。Markdown 的列表看起来，嗯，就是列表。Markdown 的区块引用看起来就真的像是引用一段文字，就像你曾在电子邮件中见过的那样。


# 全局规范

MarkDown 文件均使用```.md```作为后缀 *(小写字母)*

普通文本换行，使用行末尾```2空格```触发


## 发展

越来越多的软件和服务支持Markdown语法，应该说，Mac上大多数的写作软件都支持它。在线工具同样有很多，如果你的博客基于Wordpress或是blogger，它同样支持发布。

不仅仅是写博客，一切文章都可以用Markdown语法来写，比如说你想将一个标题加大字体，只需要在相应文字前面加上一个#或是在它的下一行加上一些等号即可，还有比这更简单的调整格式方式吗？


# 语法参见

[官方语法说明](http://daringfireball.net/projects/markdown/syntax)

# 示例

---

## 标题

# 一级标题

## 二级标题

### 三级标题

#### 四级标题

##### 五级标题

###### 六级标题

## 强调

- **粗体**

- _斜体_

## 引用

上面的横线用三个-表示

> 区块引用

## 列表

* red
* green
* blue

+ red
+ green
+ blue

- red
- green
- blue


1. red
2. green
3. blue


* 一个列表包含一个区块
> 区块内容
> 更多内容

## 代码

代码的插入方式 `printf('\n');`

```
function method()
{
    alert("javascript");
}
```

```
class Test{
    public static void main(String argc[]){
        System.out.println("java");
    }
}
```

```
class Test{
    public static void main()
    {
        Console.WriteLine("C#");
    }
}
```

## 链接

行内连接 [GitHub](https://github.com/) 的链接

## 图片

![shiyanlou logo](https://static.shiyanlou.com/img/logo_03.png)
