/************************************ 
	* @file    : mdtransform.h
	* @brief   : 
	* @details : 
	* @author  : Alliswell
	* @date    : 2020-1-28
************************************/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <cctype>

using namespace std;

#define maxLength 10000

// * 0: null | 开始
// * 1 : <p> | 段落
// * 2 : <a href = " ">...< / a> | 超链接
// * 3 : <ul> | 无序列表
// * 4 : <ol> | 有序列表
// * 5 : <li> | 列表
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
enum Token
{
	maxLength = 1000,
	nul       = 0,
	paragraph = 1,
	href	  = 2,
	ul		  = 3,
	ol		  = 4,
	li		  = 5,
	em		  = 6,
	strong	  = 7,
	hr		  = 8,
	br		  = 9,
	image	  = 10,
	quote	  = 11,
	h1		  = 12,
	h2		  = 13,
	h3	      = 14,
	h4		  = 15,
	h5		  = 16,
	h6		  = 17,
	blockcode = 18,
	code	  = 19,
};

//HTML前置标签
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

//保存目录结构
struct Cnode {
	vector<Cnode*> ch;
	string heading;
	string tag;

	Cnode(const string &hd) :heading(hd) {}
};

//保存正文内容
struct node {
	//节点类型
	int type;
	vector<node*> ch;
	//存三个重要属性，elem[0]保存显示内容、elem[1]保存链接、elem[2]保存title
	string elem[3];

	node(int _type) :type(_type) {}
};

class MarkdownTransform {
private:
	//内容
	string contents;
	//目录
	string TOC;
	
	node *root, *now;
	Cnode *Croot;

	//让目录能够正确的索引到 HTML 的内容位置， cntTag 的进行记录
	int cntTag = 0;
	char s[maxLength];

public:

	//************************************
	// Method:    start
	// FullName:  MarkdownTransform::start
	// Access:    public 
	// Returns:   std::pair<int, char*> 由空格数和有内容处的 char* 指针组成的 std::pair
	// Qualifier:
	// Parameter: char * src 源串
	// details :  解析一行中开始处的空格和Tab
	//************************************
	pair<int, char*> start(char *src) {
		//如果行空，则返回
		if (!strlen(src)){
			return make_pair(0, nullptr);
		}

		//行前有空格和Tab
		//统计空格数和Tab数
		int cntspace = 0;
		int cnttab = 0;
		//从该行的第一个字符读其, 统计空格键和 Tab 键,
		// 当遇到不是空格和 Tab 时，立即停止
		for (int i = 0; src[i] != '\0';i++) {
			if (src[i]==' '){
				cntspace++;
			}else if (src[i]=='\t'){
				cnttab++;
			}
			// 如果内容前有空格和 Tab，那么将其统一按 Tab 的个数处理,
			// 其中, 一个 Tab = 四个空格
			else return make_pair(cnttab + cnttab / 4, src + i);
		
		}
		//行前无空格和Tab
		return make_pair(0, src);
	}


	//************************************
	// Method:    JudgeType
	// FullName:  MarkdownTransform::JudgeType
	// Access:    public 
	// Returns:   std::pair<int, char*> 当前行的类型和除去行标志性关键字的正是内容的 char* 指针组成的 std::pair
	// Qualifier:
	// Parameter: char * src 源串
	// details :  判断当前行类型，MD语法关键字位于行首
	//************************************
	pair<int, char*> JudgeType(char *src) {
		char *ptr = src;

		//跳过'#'
		while (*ptr=='#'){
			ptr++;
		}

		//接着出现空格，则是'<h>'标签
		if (ptr - src > 0 && *ptr == ' ') {
			return make_pair(ptr - src + h1 - 1, ptr + 1);//累加判断几级标题
		}

		//重置分析位置
		ptr = src;

		//出现 ```则是代码块
		if (!strncmp(ptr, "```", 3)) {
			return make_pair(blockcode, ptr + 3);
		}

		//如果出现 (* +) -, 并且他们的下一个字符为空格，则说明是无序列表
		if (!strncmp(ptr, "- ", 2)) {
			return make_pair(ul, ptr + 1);
		}
		// 如果出现的是数字, 且下一个字符是 '.',下下个空格，则说明是是有序列表
		char *ptr1 = ptr;
		while (*ptr1 && (isdigit(*ptr1))) {
			ptr1++;
		}
		if (ptr1 != ptr && *ptr1 == '.'&&ptr1[1] == ' ') {
			return make_pair(ol, ptr1 + 1);
		}

		//如果出现 > 且下一个字符为空格，则说明是引用
		if (!strncmp(ptr, ">  ", 2)) {
			return make_pair(quote, ptr + 1);
		}

		//否则就是普通段落
		return make_pair(paragraph, ptr);
	}
	
	//************************************
	//类型获取
	//************************************
	// 判断是否为标题
	bool isHeading(node *v) {
		return (v->type >= h1 && v->type <= h6);
	}
	// 判断是否为图片
	bool isImage(node *v) {
		return (v->type == image);
	}
	//判断为超链接
	bool isHref(node *v) {
		return (v->type == href);
	}




	//构造函数
	MarkdownTransform(const std::string &filename);

	// 获得 Markdown 目录
	std::string getTableOfContents() {
		return TOC;
	}

	// 获得 Markdown 内容
	std::string getContents() {
		return contents;
	}

	//析构函数
	~MarkdownTransform();
};

