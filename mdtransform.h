/************************************ 
	* @file    : mdtransform.h
	* @brief   : 
	* @details : 
	* @author  : Alliswell
	* @date    : 2020-1-28
************************************/
#pragma once
#include <string>

// * 0: null | ��ʼ
// * 1 : <p> | ����
// * 2 : <a href = " ">...< / a> | ������
// * 3 : <ul> | �����б�
// * 4 : <ol> | �����б�
// * 5 : <li> | �б�
// * 6 : <em> | б��
// * 7 : <strong> | �Ӵ�
// * 8 : <hr / > | ˮƽ�ָ���
// * 9 : <br / > | ����
// * 10 : <img alt = "" src = "" / > | ͼƬ
// * 11 : <blockquote> | ����
// * 12 : <h1> | h1
// * 13 : <h2> | h2
// * 14 : <h3> | h3
// * 15 : <h4> | h4
// * 16 : <h5> | h5
// * 17 : <h6> | h6
// * 18 : <pre><code> | �����
// * 19 : <code> | ���ڴ���
//�ʷ��ؼ���ö��
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

//HTMLǰ�ñ�ǩ
const std::string frontTag[] = {
	"","<p>","","<ul>","<ol>","<li>","<em>","<strong>",
	"<hr color=#CCCCCC size=1 />","<br />",
	"","<blockquote>",
	"<h1 ","<h2 ","<h3 ","<h4 ","<h5 ","<h6 ", // �ұߵļ�����Ԥ������������ı�ǩ����, �� id
	"<pre><code>","<code>"
};
// HTML ���ñ�ǩ
const std::string backTag[] = {
	"","</p>","","</ul>","</ol>","</li>","</em>","</strong>",
	"","","","</blockquote>",
	"</h1>","</h2>","</h3>","</h4>","</h5>","</h6>",
	"</code></pre>","</code>"
};

class MarkdownTransform {
public:
	//���캯��
	MarkdownTransform(const std::string &filename);

	// ��� Markdown Ŀ¼
	std::string getTableOfContents() {
		return TOC;
	}

	// ��� Markdown ����
	std::string getContents() {
		return contents;
	}

	//��������
	~MarkdownTransform();

private:
	string contents, TOC;
};

