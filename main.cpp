/************************************ 
	* @file    : main.cpp
	* @brief   : 
	* @details : 
	* @author  : Alliswell
	* @date    : 2020-1-28
************************************/

#include <fstream>
#include "mdtransform.h"


int main() {
	//װ��MD�ļ�
	MarkdownTransform transformer("test.md");
	
	//ʹ��getTableOfContens()��ȡMD�ļ�HTML��ʽĿ¼
	std::string table = transformer.getTableOfContents();

	//ʹ��getContens()��ȡת�����HTML����
	std::string contens = transformer.getContents();

	//д��HTML�ļ���ͷβ��Ϣ
	std::string head = "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <link rel=\"stylesheet\" href=\"github-markdown.css\">\
        </head><body><article class=\"markdown-body\">";
	std::string end = "</article></body></html>";

	//д���ļ�
	std::ofstream out;
	out.open("output/index.html");
	//ת��������д��HTML��<article></article> ��ǩ�ڲ�
	out << head + table + contens + end;
	out.close();

	return 0;
}