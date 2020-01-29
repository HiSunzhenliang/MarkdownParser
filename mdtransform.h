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
// * 5 : <li> | �б� ʹ�� <ul> �� <ol> ���а����������������ʹ�� <li> ���а�װ��
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
	//��������
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


	//************************************
	// ������
	//************************************
	//************************************//************************************//************************************
	//************************************
	// Method:    findnode
	// FullName:  MarkdownTransform::findnode
	// Access:    public 
	// Returns:   node* �ҵ��Ľڵ�ָ��
	// Qualifier:
	// Parameter: int depth ���
	// details :  �����������Ѱ�ҽڵ�
	//************************************
	node* findnode(int depth) {
		node *ptr = root;
		while (!ptr->ch.empty() && depth) {
			ptr = ptr->ch.back();//���һ��Ԫ��
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
	// Parameter: int x ��
	// Parameter: const string & hd
	// Parameter: int tag  tag ��ǩ��������Ŀ¼��ָ�������
	// details :  �ݹ�Cnode�ڵ����
	//************************************
	void Cins(Cnode *v, int x, const string &hd, int tag) {
		int n = v->ch.size();
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
	// Parameter: node * v ָ���ڵ�
	// Parameter: const string & src
	// details :  ��ָ����node�ڵ��в���Ҫ����Ĵ�
	//************************************
	void insert(node *v, const string &src) {
		int n = src.size();
		bool incode = false, 
			inem = false, 
			instrong = false, 
			inautolink = false;
		v->ch.push_back(new node(nul));

		for (int i = 0; i < n; i++) {
			char ch = src[i];
			if (ch == '\\') {
				ch = src[++i];
				v->ch.back()->elem[0] += string(1, ch);//��������
				continue;
			}

			//�������ڴ��� `code`
			if (ch == '`' && !inautolink) {
				incode ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(code));
				incode = !incode;
				continue;
			}

			//����Ӵ� **�Ӵ�**
			if (ch == '*' && (i < n - 1 && (src[i + 1] == '*')) && !incode && !inautolink) {
				++i;
				instrong ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(strong));
				instrong = !instrong;
				continue;
			}

			//����б�� _б��_
			if (ch == '_' && !incode && !inautolink && !instrong) {
				inem ? v->ch.push_back(new node(nul)) : v->ch.push_back(new node(em));
				inem = !inem;
				continue;
			}

			//����ͼƬ ![image](https://web/image.png) ![image](https://web/image.png "title")
			if (ch == '!' && (i < n - 1 && src[i + 1] == '[') && !incode && !instrong && !inem && !inautolink) {
				//��ȡͼƬ��ʾ����
				for (i += 2; i < n - 1 && src[i] != ']'; i++) {
					v->ch.back()->elem[0] += string(1, src[i]);
				}
				i++;
				//��ȡͼƬ����
				for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
					v->ch.back()->elem[1] += string(1, src[i]);
				}
				//��ȡtitle
				if (src[i] != ')') {
					for (i++; i < n - 1 && src[i] != ')'; i++) {
						if (src[i] == '"') {
							v->ch.back()->elem[2] += string(1, src[i]);
						}
					}
				}
				v->ch.push_back(new node(nul));
				continue;
			}

			//��������  [github](https://github.com/) [github](https://github.com/ "title")
			if (ch == '[' && !incode && !instrong && !inem && !inautolink) {
				//��ȡ������ʾ����
				for (i++; i < n - 1 && src[i] != ']'; i++) {
					v->ch.back()->elem[0] += string(1, src[i]);
				}
				i++;
				//��ȡ����
				for (i++; i < n - 1 && src[i] != ' ' && src[i] != ')'; i++) {
					v->ch.back()->elem[1] += string(1, src[i]);
				}
				//��ȡtitle
				if (src[i] != ')') {
					for (i++; i < n - 1 && src[i] != ')'; i++) {
						if (src[i] == '"') {
							v->ch.back()->elem[2] += string(1, src[i]);
						}
					}
				}
				v->ch.push_back(new node(nul));
				continue;
			}

			//��ͨ�ı�
			v->ch.back()->elem[0] += string(1, ch);
			if (!inautolink) {
				v->ch.back()->elem[1] += string(1, ch);
			}
		}

		//��β�����ո񣬻���
		if (src.size() >= 2) {
			if (src.at(src.size() - 1) == ' '&&src.at(src.size() - 2) == ' ') {
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
	// details :  �ж��Ƿ���,Markdown ֧��ʹ�� ---������Ϊ�ָ�
	//************************************
	bool isCutline(char *src) {
		int cnt = 0;
		char *ptr = src;
		while (*ptr) {
			// ������� �ո�tab��- �� *����ô����Ҫ����
			if (*ptr != ' '&&*ptr != '\t'&&*ptr != '-') {
				return false;
			}
			if (*ptr == '-'){
				cnt++;
			}
			ptr++;
		}
		// ������� --- ����Ҫ����һ���ָ���, ��ʱ��Ҫ����
		return (cnt >= 3);
	}

	//************************************
	// Method:    mkpara
	// FullName:  MarkdownTransform::mkpara
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: node * v
	// details :  �õ�һ�������ı�
	//************************************
	void mkpara(node *v) {
		if (v->ch.size() == 1u && v->ch.back()->type == paragraph) { //1u 1 unsigned int
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

	//�﷨���ı�������Ҫ������ȵģ�����Ŀ¼����ȱ������������ݵ���ȱ����߼�����һ��

	//************************************
	// Method:    dfs
	// FullName:  MarkdownTransform::dfs
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: node * v
	// details :  ������ȱ�������
	//************************************
	void dfs(node *v) {
		if (v->type == paragraph && v->elem[0].empty() && v->ch.empty()) {
			return;
		}

		contents += frontTag[v->type];
		bool flag = true;

		//������⣬֧��Ŀ¼������ת
		if (isHeading(v)) {
			contents += "id=\"" + v->elem[0] + "\">";
			flag = false;
		}

		//��������
		if (isHref(v)) {
			contents += "<a href=\"" + v->elem[1] + "\"title=\"" + v->elem[2] + "\">" + v->elem[0] + "</a>";
			flag = false;
		}

		//����ͼƬ
		if (isImage(v)) {
			contents += "<img alt=\"" + v->elem[0] + "\"src=\"" + v->elem[1] + "\"title=\"" + v->elem[2] + "\"/>";
			flag = false;
		}

		// ����������߶�����, ��ֱ���������
		if (flag){
			contents += v->elem[0];
			flag = false;
		}

		// �ݹ��������
		for (int i = 0; i < v->ch.size(); i++) {
			dfs(v->ch[i]);
		}

		// ƴ�ӽ�����ǩ
		contents += backTag[v->type];
	}

	//Ŀ¼�ı������������ݵı���������ڣ�Ŀ¼��չʾ��ʽ����Ҫʹ�������б����ʽչʾ�� HTML ��
	//************************************
	// Method:    Cdfs
	// FullName:  MarkdownTransform::Cdfs
	// Access:    public 
	// Returns:   void
	// Qualifier:
	// Parameter: Cnode * v
	// Parameter: string index
	// details :  ����Ŀ¼
	//************************************
	void Cdfs(Cnode *v, string index) {
		TOC += "<li>\n";
		TOC += "<a href=\"#" + v->tag + "\">" + index + " " + v->heading + "</a>\n";
		int n = v->ch.size();
		if (n){
			TOC += "<ul>\n";
			for (int i = 0; i < n; i++) {
				Cdfs(v->ch[i], index + to_string(i + 1) + ".");
			}
			TOC += "</ul>\n";
		}
		TOC += "</li>\n";
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

