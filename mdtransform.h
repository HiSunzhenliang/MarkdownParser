/************************************
 * @file    : mdtransform.h
 * @brief   :
 * @details :
 * @author  : Alliswell
 * @date    : 2020-1-28
 ************************************/
#pragma once
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

using namespace std;

#define maxLength 10000

// * 0: null | 开始
// * 1 : <p> | 段落
// * 2 : <a href = " ">...< / a> | 超链接
// * 3 : <ul> | 无序列表
// * 4 : <ol> | 有序列表
// * 5 : <li> | 列表 使用 <ul> 和 <ol> 进行包裹，最后将整个内容使用 <li>
// 进行包装。
// * 6 : <em> | 斜体
// * 7 : <strong> | 加粗
// * 8 : <hr / > | 水平分割线
// * 9 : <br / > | 换行
// * 10 : <img alt = "" src = "" / > | 图片
// * 11 : <blockquote> | 引用
// * 12 : <h1> | h1
// * 13 : <h2> | h2
// * 14 : <h3> | h3
// * 15 : <h4> | h4
// * 16 : <h5> | h5
// * 17 : <h6> | h6
// * 18 : <pre><code> | 代码段
// * 19 : <code> | 行内代码
//词法关键词枚举
enum Token {
    nul = 0,
    paragraph = 1,
    href = 2,
    ul = 3,
    ol = 4,
    li = 5,
    em = 6,
    strong = 7,
    hr = 8,
    br = 9,
    image = 10,
    quote = 11,
    h1 = 12,
    h2 = 13,
    h3 = 14,
    h4 = 15,
    h5 = 16,
    h6 = 17,
    blockcode = 18,
    code = 19,
};

// HTML前置标签
const std::string frontTag[] = {
    "",
    "<p>",
    "",
    "<ul>",
    "<ol>",
    "<li>",
    "<em>",
    "<strong>",
    "<hr color=#CCCCCC size=1 />",
    "<br />",
    "",
    "<blockquote>",
    "<h1 ",
    "<h2 ",
    "<h3 ",
    "<h4 ",
    "<h5 ",
    "<h6 ",  // 右边的尖括号预留给添加其他的标签属性, 如 id
    "<pre><code>",
    "<code>"};
// HTML 后置标签
const std::string backTag[] = {
    "",          "</p>",  "",      "</ul>", "</ol>",         "</li>",  "</em>",
    "</strong>", "",      "",      "",      "</blockquote>", "</h1>",  "</h2>",
    "</h3>",     "</h4>", "</h5>", "</h6>", "</code></pre>", "</code>"};

//保存目录结构
struct Cnode {
    vector<Cnode *> ch;
    string heading;
    string tag;

    Cnode(const string &hd) : heading(hd) {}
};

//保存正文内容
struct node {
    //节点类型
    int type;

    vector<node *> ch;

    //存三个重要属性，elem[0]保存显示内容、elem[1]保存链接、elem[2]保存title
    string elem[3];

    node(int _type) : type(_type) {}
};

class MarkdownTransform {
   private:
    //正文内容
    string contents;
    //目录
    string TOC;

    node *root, *now;
    Cnode *Croot;

    //让目录能够正确的索引到 HTML 的内容位置， cntTag 的进行记录
    int cntTag = 0;
    char s[maxLength];  //读取一行内容buf

   public:
    //************************************
    //类型获取
    //************************************
    // 判断是否为标题
    bool isHeading(node *v) { return (v->type >= h1 && v->type <= h6); }
    // 判断是否为图片
    bool isImage(node *v) { return (v->type == image); }
    //判断为超链接
    bool isHref(node *v) { return (v->type == href); }

    //************************************
    // Method:    start
    // FullName:  MarkdownTransform::start
    // Access:    public
    // Returns:   std::pair<int, char*> 由空格数和有内容处的 char* 指针组成的
    // std::pair Qualifier: Parameter: char * src 源串 details :
    // 解析一行中开始处的空格和Tab
    //************************************
    pair<int, char *> start(char *src) {
        //如果行空，则返回
        if ((int)strlen(src) == 0) {
            return make_pair(0, nullptr);
        }

        //行前有空格和Tab
        //统计空格数和Tab数
        int cntspace = 0;
        int cnttab = 0;
        //从该行的第一个字符读其, 统计空格键和 Tab 键,
        // 当遇到不是空格和 Tab 时，立即停止
        for (int i = 0; src[i] != '\0'; i++) {
            if (src[i] == ' ') {
                cntspace++;
            } else if (src[i] == '\t') {
                cnttab++;
            }
            // 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
            // 其中, 一个 Tab = 四个空格
            return make_pair(cnttab + cnttab / 4, src + i);
        }
        //行前无空格和Tab
        return make_pair(0, nullptr);
    }

    //************************************
    // Method:    JudgeType
    // FullName:  MarkdownTransform::JudgeType
    // Access:    public
    // Returns:   std::pair<int, char*>
    // 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
    // Qualifier:
    // Parameter: char * src 源串
    // details :  判断当前行类型，MD语法关键字位于行首
    //************************************
    pair<int, char *> JudgeType(char *src) {
        char *ptr = src;

        //跳过'#'
        while (*ptr == '#') {
            ptr++;
        }

        //接着出现空格，则是'<h>'标签
        if (ptr - src > 0 && *ptr == ' ') {
            return make_pair(ptr - src + h1 - 1, ptr + 1);  //累加判断几级标题
        }

        //重置分析位置
        ptr = src;

        //出现 ```则是代码块
        if (strncmp(ptr, "```", 3) == 0) {
            return make_pair(blockcode, ptr + 3);
        }

        //如果出现 (* +) -, 并且他们的下一个字符为空格，则说明是无序列表
        if (strncmp(ptr, "- ", 2) == 0) {
            return make_pair(ul, ptr + 1);
        }

        //如果出现 > 且下一个字符为空格，则说明是引用
        if (*ptr == '>' && (ptr[1] == ' ')) {
            return make_pair(quote, ptr + 1);
        }

        // 如果出现的是数字, 且下一个字符是 '.',下下个空格，则说明是是有序列表
        char *ptr1 = ptr;
        while (*ptr1 && (isdigit((unsigned char)*ptr1))) {
            ptr1++;
        }
        if (ptr1 != ptr && *ptr1 == '.' && ptr1[1] == ' ') {
            return make_pair(ol, ptr1 + 1);
        }
        //否则就是普通段落
        return make_pair(paragraph, ptr);
    }

    //************************************
    // 树操作
    //************************************
    //************************************//************************************//************************************
    //************************************
    // Method:    findnode
    // FullName:  MarkdownTransform::findnode
    // Access:    public
    // Returns:   node* 找到的节点指针
    // Qualifier:
    // Parameter: int depth 深度
    // details :  给定树的深度寻找节点
    //************************************
    node *findnode(int depth) {
        node *ptr = root;
        while (!ptr->ch.empty() && depth != 0) {
            ptr = ptr->ch.back();  //最后一个元素
            if (ptr->type == li) {
                depth--;
            }
        }
        return ptr;
    }

    //************************************//************************************//************************************

    //************************************
    // Method:    Cins
    // FullName:  MarkdownTransform::Cins
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: Cnode * v
    // Parameter: int x 目录级别序号
    // Parameter: const string & hd
    // Parameter: int tag  tag序号 标签来标记这个目录所指向的内容
    // details :  递归Cnode节点插入
    //************************************
    void Cins(Cnode *v, int x, const string &hd, int tag) {
        int n = (int)v->ch.size();
        if (x == 1) {
            v->ch.push_back(new Cnode(hd));
            v->ch.back()->tag = "tag" + to_string(tag);
            return;
        }
        if (!n || v->ch.back()->heading.empty()) {
            v->ch.push_back(new Cnode(""));
        }
        Cins(v->ch.back(), x - 1, hd, tag);
    }

    //************************************
    // Method:    insert
    // FullName:  MarkdownTransform::insert
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: node * v 指定节点
    // Parameter: const string & src
    // details :  向指定的node节点中插入要处理的串
    //************************************
    void insert(node *v, const string &src) {
        int n = (int)src.size();
        bool incode = false, inem = false, instrong = false, inautolink = false;
        v->ch.push_back(new node(nul));

        for (int i = 0; i < n; i++) {
            char ch = src[i];
            if (ch == '\\') {
                ch = src[++i];
                v->ch.back()->elem[0] += string(1, ch);  //保存内容
                continue;
            }

            //处理行内代码 `code`
            if (ch == '`' && !inautolink) {
                incode ? v->ch.push_back(new node(nul))
                       : v->ch.push_back(new node(code));
                incode = !incode;
                continue;
            }

            //处理加粗 **加粗**
            if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode &&
                !inautolink) {
                ++i;
                instrong ? v->ch.push_back(new node(nul))
                         : v->ch.push_back(new node(strong));
                instrong = !instrong;
                continue;
            }

            //处理斜体 _斜体_
            if (ch == '_' && !incode && !inautolink && !instrong) {
                inem ? v->ch.push_back(new node(nul))
                     : v->ch.push_back(new node(em));
                inem = !inem;
                continue;
            }

            //处理图片 ![image](https://web/image.png)
            //![image](https://web/image.png "title")
            if (ch == '!' && (i < n - 1 && src[i + 1] == '[') && !incode &&
                !instrong && !inem && !inautolink) {
                //获取图片显示内容
                v->ch.push_back(new node(image));
                for (i += 2; i < n - 1 && src[i] != ']'; i++) {
                    v->ch.back()->elem[0] += string(1, src[i]);
                }
                i++;
                //获取图片链接
                for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
                    v->ch.back()->elem[1] += string(1, src[i]);
                }
                //获取title
                if (src[i] != ')') {
                    for (i++; i < n - 1 && src[i] != ')'; i++) {
                        if (src[i] != '"') {
                            v->ch.back()->elem[2] += string(1, src[i]);
                        }
                    }
                }
                v->ch.push_back(new node(nul));
                continue;
            }

            //处理超链接  [github](https://github.com/)
            //[github](https://github.com/ "title")
            if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
                //获取链接显示内容
                v->ch.push_back(new node(href));
                for (i++; i < n - 1 && src[i] != ']'; i++) {
                    v->ch.back()->elem[0] += string(1, src[i]);
                }
                i++;
                //获取链接
                for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
                    v->ch.back()->elem[1] += string(1, src[i]);
                }
                //获取title
                if (src[i] != ')') {
                    for (i++; i < n - 1 && src[i] != ')'; i++) {
                        if (src[i] != '"') {
                            v->ch.back()->elem[2] += string(1, src[i]);
                        }
                    }
                }
                v->ch.push_back(new node(nul));
                continue;
            }

            //普通文本
            v->ch.back()->elem[0] += string(1, ch);
            if (!inautolink) {
                v->ch.back()->elem[1] += string(1, ch);
            }
        }

        //结尾两个空格，换行
        if (src.size() >= 2) {
            if (src.at(src.size() - 1) == ' ' &&
                src.at(src.size() - 2) == ' ') {
                v->ch.push_back(new node(br));
            }
        }
    }

    //************************************
    // Method:    isCutline
    // FullName:  MarkdownTransform::isCutline
    // Access:    public
    // Returns:   bool
    // Qualifier:
    // Parameter: char * src
    // details :  判断是否换行,Markdown 支持使用 ---进行人为分割
    //************************************
    bool isCutline(char *src) {
        int cnt = 0;
        char *ptr = src;
        while (*ptr) {
            // 如果不是 空格、tab、- 或 *，那么则不需要换行
            if (*ptr != ' ' && *ptr != '\t' && *ptr != '-') {
                return false;
            }
            if (*ptr == '-') {
                cnt++;
            }
            ptr++;
        }
        // 如果出现 --- 则需要增加一个分割线, 这时需要换行
        return (cnt >= 3);
    }

    //************************************
    // Method:    mkpara
    // FullName:  MarkdownTransform::mkpara
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: node * v
    // details :  拿到一个段落文本
    //************************************
    void mkpara(node *v) {
        if (v->ch.size() == 1u &&
            v->ch.back()->type == paragraph) {  // 1u 1 unsigned int
            return;
        }
        if (v->type == paragraph) {
            return;
        }
        if (v->type == nul) {
            v->type = paragraph;
            return;
        }
        node *x = new node(paragraph);
        x->ch = v->ch;
        v->ch.clear();
        v->ch.push_back(x);
    }

    //遍历与生成
    //语法树的遍历是需要深度优先的，而对目录的深度遍历和正文内容的深度遍历逻辑并不一样

    //************************************
    // Method:    dfs
    // FullName:  MarkdownTransform::dfs
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: node * v
    // details :  深度优先遍历正文
    //************************************
    void dfs(node *v) {
        if (v->type == paragraph && v->elem[0].empty() && v->ch.empty()) {
            return;
        }

        contents += frontTag[v->type];
        bool flag = true;

        //处理标题，支持目录进行跳转
        if (isHeading(v)) {
            contents += "id=\"" + v->elem[0] + "\">";
            flag = false;
        }

        //处理超链接
        if (isHref(v)) {
            contents += "<a href=\"" + v->elem[1] + "\" title=\"" + v->elem[2] +
                        "\">" + v->elem[0] + "</a>";
            flag = false;
        }

        //处理图片
        if (isImage(v)) {
            contents += "<img alt=\"" + v->elem[0] + "\" src=\"" + v->elem[1] +
                        "\" title=\"" + v->elem[2] + "\" />";
            flag = false;
        }

        // 如果上面三者都不是, 则直接添加内容
        if (flag) {
            contents += v->elem[0];
            flag = false;
        }

        // 递归遍历所有
        for (int i = 0; i < (int)v->ch.size(); i++) {
            dfs(v->ch[i]);
        }

        // 拼接结束标签
        contents += backTag[v->type];
    }

    //目录的遍历和正文内容的遍历差别在于，目录的展示方式是需要使用无序列表的形式展示在
    // HTML 中
    //************************************
    // Method:    Cdfs
    // FullName:  MarkdownTransform::Cdfs
    // Access:    public
    // Returns:   void
    // Qualifier:
    // Parameter: Cnode * v
    // Parameter: string index
    // details :  遍历目录
    //************************************
    void Cdfs(Cnode *v, string index) {
        TOC += "<li>\n";
        TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading +
               "</a>\n";
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

    //************************************
    // Method:    MarkdownTransform
    // FullName:  MarkdownTransform::MarkdownTransform
    // Access:    public
    // Returns:
    // Qualifier:
    // Parameter: const std::string & filename
    // details :  构造函数对文档树结构处理，生成内容与目录
    //************************************
    MarkdownTransform(const std::string &filename) {
        // 首先为文档的树结构进行初始化，并将当前指针 now 指向根节点
        Croot = new Cnode("");
        root = new node(nul);
        now = root;

        //从文件流中读取文件
        std::ifstream fin(filename);

        //默认不是新段落，默认不在代码块内
        bool newpara = false;
        bool inblock = false;

        //读取到eof为止
        while (!fin.eof()) {
            //从文件读取一行
            fin.getline(s, maxLength);

            //处理不在代码块且需要换行
            if (!inblock && isCutline(s)) {
                now = root;
                now->ch.push_back(new node(hr));
                newpara = false;
                continue;
            }

            // std::pair 实质上是一个结构体, 可以将两个数据组合成一个数据
            // 计算一行中开始的空格和 Tab 数
            // 由空格数和有内容处的 char* 指针组成的
            auto ps = start(s);

            // 如果没有位于代码块中, 且没有统计到空格和 Tab, 则直接读取下一行
            if (!inblock && ps.second == nullptr) {
                now = root;
                newpara = true;
                continue;
            }

            // 分析该行文本的类型
            // 当前行的类型和除去行标志性关键字的正文内容的 char* 的pair
            auto tj = JudgeType(ps.second);

            // 如果是代码块类型
            if (tj.first == blockcode) {
                // 如果已经位于代码块中, 则 push 一个空类型的节点，否则push
                // 新代码块节点
                inblock ? now->ch.push_back(new node(nul))
                        : now->ch.push_back(new node(blockcode));
                inblock = !inblock;
                continue;
            }
            //如果已经在代码块中，内容拼接到当前节点
            if (inblock) {
                now->ch.back()->elem[0] += string(s) + '\n';
                continue;
            }

            //普通段落？
            if (tj.first == paragraph) {
                if (now == root) {  //?
                    now = findnode(ps.first);
                    now->ch.push_back(new node(paragraph));
                    now = now->ch.back();
                }
                bool flag = false;
                if (newpara && !now->ch.empty()) {
                    node *ptr = nullptr;
                    for (auto i : now->ch) {
                        if (i->type == nul) {
                            ptr = i;
                        }
                    }
                    if (ptr != nullptr) {
                        mkpara(ptr);
                    }
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

            //标题行，标签中插入属性tag
            if (tj.first >= h1 && tj.first <= h6) {
                now->ch.push_back(new node(tj.first));
                now->ch.back()->elem[0] = "tag" + to_string(++cntTag);
                insert(now->ch.back(), string(tj.second));
                Cins(Croot, tj.first - h1 + 1, string(tj.second), cntTag);
            }

            //如果是无序列表
            if (tj.first == ul) {
                if (now->ch.empty() || now->ch.back()->type != ul) {
                    now->ch.push_back(new node(ul));
                }
                now = now->ch.back();
                bool flag = false;
                if (newpara && !now->ch.empty()) {
                    node *ptr = nullptr;
                    for (auto i : now->ch) {
                        if (i->type == li) {
                            ptr = i;
                        }
                    }
                    if (ptr != nullptr) {
                        mkpara(ptr);
                    }
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

            //如果是有序列表
            if (tj.first == ol) {
                if (now->ch.empty() || now->ch.back()->type != ol) {
                    now->ch.push_back(new node(ol));
                }
                now = now->ch.back();
                bool flag = false;
                if (newpara && !now->ch.empty()) {
                    node *ptr = nullptr;
                    for (auto i : now->ch) {
                        if (i->type == li) {
                            ptr = i;
                        }
                    }
                    if (ptr != nullptr) {
                        mkpara(ptr);
                    }
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

            //如果是引用
            if (tj.first == quote) {
                if (now->ch.empty() || now->ch.back()->type != quote) {
                    now->ch.push_back(new node(quote));
                }
                now = now->ch.back();
                if (newpara || now->ch.empty()) {
                    now->ch.push_back(new node(paragraph));
                }
                insert(now->ch.back(), string(tj.second));
            }

            newpara = false;
        }

        //文件读取分析完毕
        fin.close();

        //深度遍历语法树
        dfs(root);

        //构造目录
        TOC += "<ul>";
        for (int i = 0; i < (int)Croot->ch.size(); i++) {
            Cdfs(Croot->ch[i], to_string(i + 1) + ".");
        }
        TOC += "</ul>";
    }

    //递归销毁节点
    template <typename T>
    void destroy(T *v) {
        for (int i = 0; i < (int)v->ch.size(); i++) {
            destroy(v->ch[i]);
        }
        delete v;
    }

    // 获得 Markdown 目录
    std::string getTableOfContents() { return TOC; }

    // 获得 Markdown 内容
    std::string getContents() { return contents; }

    //析构函数
    ~MarkdownTransform() {
        destroy<node>(root);
        destroy<Cnode>(Croot);
    }
};
