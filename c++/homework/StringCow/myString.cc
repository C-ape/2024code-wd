#include <string.h>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::cin;
using std::vector;

//重载字符串所有的运算符
class String 
{
public:
	String();
	String(const char *);
	String(const String &);
	~String();

    void print();
	String &operator=(const String &);
	String &operator=(const char *);

	String &operator+=(const String &);
	String &operator+=(const char *);
	
	char &operator[](std::size_t index);
	const char &operator[](std::size_t index) const;
	
	std::size_t size() const;
	const char* c_str() const;
	
    friend bool operator==(const String &, const String &);
    friend bool operator!=(const String &, const String &);

    friend bool operator<(const String &, const String &);
    friend bool operator>(const String &, const String &);
    friend bool operator<=(const String &, const String &);
    friend bool operator>=(const String &, const String &);

    friend std::ostream &operator<<(std::ostream &os, const String &s);
    friend std::istream &operator>>(std::istream &is, String &s);

private:
	char * _pstr;
};

String operator+(const String & lhs, const String & rhs)
{
    String tmp(lhs);
    tmp += rhs;
    return tmp;
}
String operator+(const String & lhs, const char * str)
{
    String tmp(lhs);
    tmp += str;
    return tmp;
}
String operator+(const char * str, const String & rhs)
{
    String tmp(str);
    tmp += rhs;
    return tmp;
}

std::size_t String::size() const
{
    if (_pstr)
        return strlen(_pstr);
    else
        return 0;
}

const char* String::c_str() const
{
    return _pstr;
}

void String::print()
{
    if(_pstr)
    {
        //cout输出空指针会直接程序崩溃
        cout << _pstr << endl;
    }
}
String::String()
:_pstr(nullptr)
{
    cout << "String()" << endl;
}
String::~String()
{
    cout << "~String()" << endl;
    if(_pstr)
    {
        delete _pstr;
        _pstr == nullptr;
    }
}
//有参构造函数
String::String(const char * str)
:_pstr(new char[strlen(str) + 1]())
{
    cout << "String(const char* str)" << endl;
    strcpy(_pstr, str);
}

//拷贝构造函数
// 1、实参到形参的赋值
// 2、return返回临时对象
// 3、初始化刚声明的对象
String::String(const String &rhs)
:_pstr(new char[strlen(rhs._pstr) + 1]())
{
    cout << "String(const String &)" << endl;
    strcpy(_pstr, rhs._pstr);
}

String & String::operator=(const String & rhs)
{
    if(this != &rhs)  //1、自复制
    {
        //2、释放左操作数空间
        if (_pstr)  //无参构造函数的pstr是nullptr的
            delete _pstr;
        //3、深拷贝
        _pstr = new char[strlen(rhs._pstr) + 1]();
        strcpy(_pstr, rhs._pstr);
    }
    cout << "String & operator=(const String &)" << endl;
    //4、返回对象
    return * this;
}

String & String::operator=(const char* str)
{
    if (_pstr)  //无参构造函数的pstr是nullptr的
        delete _pstr;

    _pstr = new char[strlen(str) + 1]();
    strcpy(_pstr, str);
    cout << "String & operator=(const char*)" << endl;
    return * this;
}

//s1 += s2; s1 = s1 + s2
String & String::operator+=(const String & rhs)
{
    //先删除左操作数的空间，再进行深拷贝
    String tmp(*this);
    if (_pstr)
        delete _pstr;
    _pstr = new char[strlen(tmp._pstr) + strlen(rhs._pstr) + 1]();
    strcpy(_pstr, tmp._pstr);
    strcat(_pstr, rhs._pstr);
    return *this;
}

String & String::operator+=(const char * str)
{
    String tmp(*this);
    if (_pstr)
        delete _pstr;
    _pstr = new char[strlen(tmp._pstr) + strlen(str) + 1]();
    strcpy(_pstr, tmp._pstr);
    strcat(_pstr, str);
    return *this;
}

char & String::operator[](std::size_t index)
{
    if (index < size())
    {
        return _pstr[index];
    }else
    {
        //返回的是一个引用，不能使用局部变量，用static
        static char charnull = '\0';
        return charnull;
    }
}
const char & String::operator[](std::size_t index) const
{
    if (index < size())
    {
        return _pstr[index];
    }else
    {
        //返回的是一个引用，不能使用局部变量，用static
        static char charnull = '\0';
        return charnull;
    }
}

bool operator==(const String & rhs, const String & lhs)
{
    return !strcmp(rhs._pstr, lhs._pstr);
}
bool operator!=(const String & rhs, const String & lhs)
{
    return strcmp(rhs._pstr, lhs._pstr);
}
bool operator<(const String &lhs, const String &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) < 0;
}

bool operator>(const String &lhs, const String &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) > 0;
}
bool operator<=(const String &lhs, const String &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) <= 0;
}
bool operator>=(const String &lhs, const String &rhs)
{
    return strcmp(lhs._pstr, rhs._pstr) >= 0;
}


std::ostream &operator<<(std::ostream &os, const String &s)
{
    if (s._pstr)
        os << s._pstr;
    return os;
}
std::istream &operator>>(std::istream &is, String & s)
{
    //删除原有空间，动态申请空间存放数据
    if (s._pstr)
        delete s._pstr;
        s._pstr = nullptr;
    
    vector<char> buffer;
    char ch;
    while ((ch = is.get()) != '\n')     //这里的分隔符可以选择空格或者换行符，具体看需求
    {
        buffer.push_back(ch);
    }
    
    s._pstr = new char[buffer.size() + 1]();
    strncpy(s._pstr, &buffer[0], buffer.size());
    return is;
}

void test0()
{
    // String s0();    //不能这样创建对象，这是个函数声明，不是对象创建
    // s0.print();
    String s1;
    s1.print();

    String s2("hello");
    s2.print();

    String s3 = s2; //拷贝构造
    s3.print();

    s1 = s2;    //赋值运算符
    s1.print();

    s1 = "asdf";
    s1.print();
}


void test1()
{
    String s1("hello");
    s1.print();
    
    String s2("world");
    s1 += s2;
    s1.print();
    s1 += "hahaha";
    s1.print();

    cout << s1[0] << "   "<< endl << s1[-1] << endl;

    String s4 = "heihei";
    String s5("hahah");
    String s6 = s4 + s5;
    s6.print();
    cout << endl;
    (s4 + "aaaa").print();      //临时对象调用print
    ("bbb" + s4).print();
}


int main()
{
    //test1();
    // String s3("he");
    // String s4("he");
    // cout << (s3 == s4) << endl;
    // cout << s3 << endl;
    
    String s1, s2;
    cin >> s1;
    s1.print();
    String s5 = s1;
    s5.print();
    return 0;
}
