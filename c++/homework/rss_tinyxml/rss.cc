#include "tinyxml2.h"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <regex>

using std::cout;
using std::endl;
using namespace tinyxml2;
using std::string;
using std::vector;
using std::ofstream;

struct RssItem
{
    string _title;
    string _link;
    string _description;
    string _content;
};

class RssReader
{
public:
    RssReader(size_t capa)
    {
        _rss.reserve(capa);     //先预留空间，后面push_back效率太低
    }
    void parseRss(const char * filename);    //解析
    void dump(const string & filename);    //输出到文件

private:
    vector<RssItem> _rss;
};

void RssReader::parseRss(const char * filename)
{
    XMLDocument xmlDoc;
    XMLError err = xmlDoc.LoadFile(filename);
    if (err != XML_SUCCESS)
    {
        cout  <<  "read xml failed"  <<  endl;
        return;
    }

    XMLElement * rss = xmlDoc.FirstChildElement("rss");
    if (rss == nullptr)
    {
        cout  <<  "read xml rss failed"  <<  endl;
        return;
    }
    XMLElement * channel = rss->FirstChildElement("channel");
    if (channel == nullptr)
    {
        cout  <<  "read xml channel failed"  <<  endl;
        return;
    }
    XMLElement * node = channel->FirstChildElement("item");
    if (node == nullptr)
    {
        cout  <<  "read xml item failed"  <<  endl;
        return;
    }
    size_t idx = 0;
    while(node)
    {
        //严格来说，这里的FirstChildElement()的结果需要判空才能下一步操作,不然会core，繁琐不写了
        string title = node->FirstChildElement("title")->GetText();
        string link = node->FirstChildElement("link")->GetText();
        string description= node->FirstChildElement("description")->GetText();
        string content = node->FirstChildElement("content:encoded")->GetText();

        std::regex reg("<[^>]+>");//通用正则表达式
        description = regex_replace(description, reg, "");
        content = regex_replace(content, reg, "");

        RssItem rssItem;

        rssItem._title = title;
        rssItem._link = link;
        rssItem._description = description;
        rssItem._content = content;

        _rss.push_back(rssItem);

        ++idx;
        node = node->NextSiblingElement("item");
    } 
}

void RssReader::dump(const string & filename)
{
    ofstream ofs(filename);
    if(!ofs)
    {
        cout  <<  "open file error"  <<  endl;
        return;
    }
    for (size_t idx = 0; idx < _rss.size(); ++idx)
    {
        ofs  <<  "<doc>" << "\n"
         << "\t" << "<docid>" <<  idx+1  << "</docid>" << "\n"
         << "\t" << "<title>" << _rss[idx]._title << "</title>" << "\n"

         << "\t" << "<link>" << _rss[idx]._link  <<  "</link>" << "\n"
         << "\t" << "<description>"  <<  _rss[idx]._description  <<  "</description>"  <<  "\n"
         << "\t" << "content" << _rss[idx]._content << "</content>"  <<  endl;
    }
    ofs.close();
} 


int main()
{
    RssReader rss(20);
    rss.parseRss("coolshell.xml");
    rss.dump("pagelib.dat");
    return 0;
}