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
    //装载MD文件
    MarkdownTransform transformer("test.md");

    //使用getTableOfContens()获取MD文件HTML格式目录
    std::string table = transformer.getTableOfContents();

    //使用getContens()获取转换后的HTML内容
    std::string contents = transformer.getContents();

    //写入HTML文件的头尾信息
    std::string head =
        "<!DOCTYPE html><html><head>\
        <meta charset=\"utf-8\">\
        <title>Markdown</title>\
        <link rel=\"stylesheet\" href=\"github-markdown.css\">\
        </head><body><article class=\"markdown-body\">";
    std::string end = "</article></body></html>";

    //写入文件t
    std::ofstream out;
    out.open("output/index.html");
    //转换后内容写入HTML的<article></article> 标签内部
    out << head + table + contents + end;
    out.close();

    return 0;
}