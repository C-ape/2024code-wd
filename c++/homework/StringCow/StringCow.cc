#include <iostream>
#include <string.h>

using std::cout;
using std::endl;
using std::size_t;

// 实现字符串的写时复制：
// 1、写时复制的重要点就是引用计数+浅拷贝
// 2、所以申请字符串空间的时候前面四个字节存放引用计数，当没有写操作的时候进行浅拷贝，引用计数加1
// 3、当有写操作的时候，进行深拷贝，对于字符串来说，[]运算符遇到=是写操作，<<是读操作
// 4、所以需要一个中间类CharProxy来重载=和<<操作符，实现写时复制的作用
// 5、CharProxy的成员String类必须是引用，不然不能操作原字符串

class String
{
public:
    class CharProxy
    {
    public:
        //String的生命周期肯定比CharProxy的长，所以这里使用&没有问题，还可以直接操作原字符串
        CharProxy(size_t idx, String & string);
        CharProxy(const CharProxy & rhs);    //拷贝构造
        ~CharProxy();
        //s1[0] = 'a';
        String::CharProxy & operator=(const char &);
        //s1[0] = s2[1];
        String::CharProxy & operator=(const CharProxy & rhs);
        // 类型转换函数：向char类型转换，这样内部类不用暴露
        operator char();
    private:
        size_t _idx;
        String & _string;
    };
    void initRefcount();
    String();
	String(const char *);
	String(const String &);
	~String();

    void print();
    int getRefCount();
    void increaseRefCount();
    void decreaseRefCount();
    void release();
	String &operator=(const String &);
	
	CharProxy operator[](std::size_t index);
	
	std::size_t size() const;
	const char* c_str() const;
	
	friend std::ostream &operator<<(std::ostream &os, const String &rhs);


private:
    //指针前面4个字节存放引用计数
    char* _pstr;
};

String::CharProxy::CharProxy(size_t idx, String & string)
:_idx(idx)
,_string(string)
{
    cout << "CharProxy(size_t, String &)" << endl;
}
String::CharProxy::~CharProxy()
{
    cout << "~CharProxy()" << endl;
}

//拷贝构造函数，其引用的数据成员也要在初始化列表中初始化
String::CharProxy::CharProxy(const CharProxy & rhs)
:_string(rhs._string)
,_idx(rhs._idx)
{
    cout << "CharProxy(const String & rhs)" << endl;
}
//s1[0] = s2[1]
String::CharProxy & String::CharProxy::operator=(const CharProxy & rhs)
{
    if (_idx < _string.size())
    {
        //引用计数为1，直接修改，不用深拷贝
        if(_string.getRefCount() == 1)
        {
            _string._pstr[_idx] = rhs._string._pstr[rhs._idx];
            
        }else
        {
            //先将引用计数减一
            _string.decreaseRefCount();
            //进行深拷贝
            char * ptmp = new char[_string.size() + 5]() + 4; 
            strcpy(ptmp, _string._pstr);
            ptmp[_idx] = rhs._string._pstr[rhs._idx];
            _string._pstr = ptmp;            
        }
    }else
    {
        cout << "idx is error" << endl;
        _string = "\0";
    }

    return *this;
}

//s1[0] = 'a'
String::CharProxy & String::CharProxy::operator=(const char & ch)
{
    if (_idx < _string.size())
    {   
        //写时复制
        //引用计数为1，直接修改，不用深拷贝
        if(_string.getRefCount() == 1)
        {
            //_string._pstr[_idx] = ch;
            *(_string._pstr + _idx) = ch;
            
        }else
        {
            //先将引用计数减一
            _string.decreaseRefCount();
            //进行深拷贝
            char * ptmp = new char[_string.size() + 5]() + 4; 
            strcpy(ptmp, _string._pstr);
            ptmp[_idx] = ch;
            _string._pstr = ptmp;            
        }
    }else
    {
        cout << "idx is error" << endl;
        _string = "\0";
    }

    return *this;

}

String::CharProxy::operator char()
{
    cout << "operator char()" << endl;
    if (_idx < _string.size())
    {
        return _string._pstr[_idx];
    }else
    {
        cout << "idx is error" << endl;
        return '\0';
    }
}

// 重载中间类的输出流函数
// ostream &operator<<(std::ostream &os, const String::MidIdx &rhs)
// {
//     if(rhs._idx < rhs._str.size())
//     {
//         os << rhs._str.c_str()[rhs._idx];
//     }
//     else
//     {
//         os << "Wrong idx!";
//     }
//     return os;
// }

std::size_t String::size() const
{
    if(_pstr)
    {
        return strlen(_pstr);
    }
    return 0;
}

const char* String::c_str() const
{
    return _pstr;
}

int String::getRefCount()
{
    return *(int*)(_pstr - 4);
}

void String::increaseRefCount()
{
    ++*(int*)(_pstr - 4);
}

void String::decreaseRefCount()
{
    --*(int*)(_pstr - 4);
}
void String::initRefcount()
{
    *(int*)(_pstr - 4) = 1; //引用计数初始化为1
}
void String::print()
{   
    cout << "cnt = " << getRefCount() << "   " <<"pstr = " << _pstr << endl;
}
void String::release()
{
    decreaseRefCount();
    if (getRefCount() == 0)
    {
        delete (_pstr - 4);
        _pstr = nullptr;
    }
}
//_pstr 指向存放字符的地址，前面的是4个字节的引用计数
String::String()
:_pstr((new char[5]()) + 4)   //默认构造函数，一个int和一个'\0'
{
    cout << "String()" << endl;
    initRefcount();
}
String::String(const char * str)
:_pstr(new char[strlen(str) + 5] + 4)
{
    cout << "String(const char*)" << endl;
    initRefcount();
    strcpy(_pstr, str);
}

//拷贝构造函数
String::String(const String & rhs)
{
    cout << "String(const String & )" << endl;
    _pstr = rhs._pstr;
    increaseRefCount();
}

String::~String()
{
    cout << "~String()" << endl;
    release();
}

String & String::operator=(const String & rhs)
{
    cout << "String::operator=(const String &)" << endl;
    //自复制
    if (this != &rhs)
    {
        //左操作数释放
        release();
        _pstr = rhs._pstr;
        increaseRefCount();
    }
    return *this;
}

// 重载String类的输出流函数
std::ostream &operator<<(std::ostream &os, const String &rhs)
{
    os << rhs._pstr;
    return os;
}

String::CharProxy String::operator[](std::size_t index)
{
    //1、调用的是CharProxy(size_t, String &)创建临时对象
    return CharProxy(index, *this);
}

void test()
{
    String s1;
    s1.print();
    String s2("haha");
    s2.print();
    String s3 = s2;
    s3.print();
    s2.print();

    cout << endl << endl;
    s1 = s2;
    s1.print();
    s3 = "hello";   //这里有个临时对象的创建，有隐式转换，后面调用赋值运算符
    s2.print();
    s3.print();

    return;
}


void test1()
{
    String s1("hello");
    s1[2] = 'a';
    cout << s1[100] << endl;
    cout << s1[2] << endl;
    cout << s1 << endl;

    String s2 = "world";
    s1[0] = s2[2];
    //这里存在CharProxy到char的隐式转换，调用类型转换函数
    cout << s1[0] << endl;
    cout << s1 << endl;
}
int main()
{
    //test();
    test1();
    return 0;
}
