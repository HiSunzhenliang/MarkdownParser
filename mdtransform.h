/************************************ 
	* @file    : mdtransform.h
	* @brief   : 
	* @details : 
	* @author  : Alliswell
	* @date    : 2020-1-28
************************************/
#pragma once
#include <string>

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

class MarkdownTransform {
public:
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

private:
	string contents, TOC;
};

