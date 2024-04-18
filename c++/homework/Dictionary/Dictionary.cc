#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

// 输入：某篇文章的绝对路径
// 输出：词典（词典中的内容为**每一行**都是一个“**单词 词频**”）

using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;


struct Record
{
    string _word;
    int _frequency;
};


bool operator<(const Record & rhs, const Record &lhs)
{
    return rhs._word < lhs._word;
} 

class Dictionary
{
public:

    void read(const string &filename);
    void store(const string &filename);
    int findWord(const string &word);
    void print()
    {
        for(auto &wd: _dict)
        {
            cout << wd._word << "    " << wd._frequency << endl;
        }
    }
private:
    vector<Record> _dict;
};


int checkword(const string & word)
{
    for (auto & ch: word)
    {
        //cout << "ch is :" << ch << endl;
        // if (ch >= 'A' && ch <= 'Z' || ch >= 'a' && ch <= 'z')
        // {
        //     continue;
        // }else
        // {
        //     cout << "word is not ok" << endl;
        //     return -1;
        // }
        if (!isalpha(ch))   //判断一个字符是否为字母
        {
            //cout << "word is not ok" << endl;
            return -1;   
        }
    }
    //cout << "word is ok" << endl;
    return 0;
}

int Dictionary::findWord(const string & word)
{
    if (_dict.size() == 1)
    {
        return -1;   //没有找到
    }
    for (int i = 0; i < _dict.size(); ++i)
    {
        if (_dict[i]._word.compare(word) == 0)
        {
            return i;   //找到了,返回下标
        }
    }
    return -1;
}


void Dictionary::read(const string & filename)
{
    ifstream ifs(filename);
    if (!ifs.good())
    {
        cout << "ifstream open file error!\n" << endl;
        return;
    }
    string word;
    while(ifs >> word)      //默认以空格，制表符和换行符作为分隔符
    {   
        //cout << word << endl;
        if (checkword(word) == -1)
            continue;
        int ret = findWord(word);
        if (ret == -1)
        {
            Record wd;
            wd._word = word;
            wd._frequency = 1;
            _dict.push_back(wd);
        }else
        {
            _dict[ret]._frequency++;
        }
    }
    sort(_dict.begin(), _dict.end());
    ifs.close();
}

void Dictionary::store(const string & filename)
{
    ofstream ofs(filename);
    if(!ofs)
    {
        cout << "open file failed" << endl;
    }
    for(auto &wd: _dict)
    {
        ofs << wd._word << "    " << wd._frequency << endl;
    }

}


int main()
{
    Dictionary dic;
    string filename("/root/c++/homework/The_Holy_Bible.txt");
    dic.read(filename);
    //dic.print();
    string resultFile("Result.txt");
    dic.store(resultFile);
    return 0;
}