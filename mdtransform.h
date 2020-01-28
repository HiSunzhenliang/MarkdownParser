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

//����Ŀ¼�ṹ
struct Cnode {
	vector<Cnode*> ch;
	string heading;
	string tag;

	Cnode(const string &hd) :heading(hd) {}
};

//������������
struct node {
	//�ڵ�����
	int type;
	vector<node*> ch;
	//��������Ҫ���ԣ�elem[0]������ʾ���ݡ�elem[1]�������ӡ�elem[2]����title
	string elem[3];

	node(int _type) :type(_type) {}
};

class MarkdownTransform {
private:
	//����
	string contents;
	//Ŀ¼
	string TOC;
	
	node *root, *now;
	Cnode *Croot;

	//��Ŀ¼�ܹ���ȷ�������� HTML ������λ�ã� cntTag �Ľ��м�¼
	int cntTag = 0;
	char s[maxLength];

public:

	//************************************
	// Method:    start
	// FullName:  MarkdownTransform::start
	// Access:    public 
	// Returns:   std::pair<int, char*> �ɿո����������ݴ��� char* ָ����ɵ� std::pair
	// Qualifier:
	// Parameter: char * src Դ��
	// details :  ����һ���п�ʼ���Ŀո��Tab
	//************************************
	pair<int, char*> start(char *src) {
		//����пգ��򷵻�
		if (!strlen(src)){
			return make_pair(0, nullptr);
		}

		//��ǰ�пո��Tab
		//ͳ�ƿո�����Tab��
		int cntspace = 0;
		int cnttab = 0;
		//�Ӹ��еĵ�һ���ַ�����, ͳ�ƿո���� Tab ��,
		// ���������ǿո�� Tab ʱ������ֹͣ
		for (int i = 0; src[i] != '\0';i++) {
			if (src[i]==' '){
				cntspace++;
			}else if (src[i]=='\t'){
				cnttab++;
			}
			// �������ǰ�пո�� Tab����ô����ͳһ�� Tab �ĸ�������,
			// ����, һ�� Tab = �ĸ��ո�
			else return make_pair(cnttab + cnttab / 4, src + i);
		
		}
		//��ǰ�޿ո��Tab
		return make_pair(0, src);
	}


	//************************************
	// Method:    JudgeType
	// FullName:  MarkdownTransform::JudgeType
	// Access:    public 
	// Returns:   std::pair<int, char*> ��ǰ�е����ͺͳ�ȥ�б�־�Թؼ��ֵ��������ݵ� char* ָ����ɵ� std::pair
	// Qualifier:
	// Parameter: char * src Դ��
	// details :  �жϵ�ǰ�����ͣ�MD�﷨�ؼ���λ������
	//************************************
	pair<int, char*> JudgeType(char *src) {
		char *ptr = src;

		//����'#'
		while (*ptr=='#'){
			ptr++;
		}

		//���ų��ֿո�����'<h>'��ǩ
		if (ptr - src > 0 && *ptr == ' ') {
			return make_pair(ptr - src + h1 - 1, ptr + 1);//�ۼ��жϼ�������
		}

		//���÷���λ��
		ptr = src;

		//���� ```���Ǵ����
		if (!strncmp(ptr, "```", 3)) {
			return make_pair(blockcode, ptr + 3);
		}

		//������� (* +) -, �������ǵ���һ���ַ�Ϊ�ո���˵���������б�
		if (!strncmp(ptr, "- ", 2)) {
			return make_pair(ul, ptr + 1);
		}
		// ������ֵ�������, ����һ���ַ��� '.',���¸��ո���˵�����������б�
		char *ptr1 = ptr;
		while (*ptr1 && (isdigit(*ptr1))) {
			ptr1++;
		}
		if (ptr1 != ptr && *ptr1 == '.'&&ptr1[1] == ' ') {
			return make_pair(ol, ptr1 + 1);
		}

		//������� > ����һ���ַ�Ϊ�ո���˵��������
		if (!strncmp(ptr, ">  ", 2)) {
			return make_pair(quote, ptr + 1);
		}

		//���������ͨ����
		return make_pair(paragraph, ptr);
	}
	
	//************************************
	//���ͻ�ȡ
	//************************************
	// �ж��Ƿ�Ϊ����
	bool isHeading(node *v) {
		return (v->type >= h1 && v->type <= h6);
	}
	// �ж��Ƿ�ΪͼƬ
	bool isImage(node *v) {
		return (v->type == image);
	}
	//�ж�Ϊ������
	bool isHref(node *v) {
		return (v->type == href);
	}




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
};

