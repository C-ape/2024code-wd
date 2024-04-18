#include<iostream>
#include <string.h>
//实现一个自定义的string类

using std::cout;
using std::endl;

class String
{
public:
    String() //构造函数
    :_pstr(nullptr)
    {
        cout << "String();" << endl;
    }

    String(const char* pstr)    //带参数的构造函数
    :_pstr(new char[strlen(pstr) + 1]())
    {
        strcpy(_pstr, pstr);
        cout << "String(const char* pstr)" << endl;
    }

    String(const String & rhs)  //拷贝构造函数
    :_pstr(new char[strlen(rhs._pstr) + 1]())
    {
        strcpy(_pstr, rhs._pstr);
        cout << "String(const String & rhs)" << endl;
    }

    String & operator=(const String & rhs) //赋值运算符函数
    {
        if (this != &rhs)   //1、防止自复制
        {
            delete [] _pstr;   //2、释放原来的空间

            _pstr = new char[strlen(rhs._pstr) + 1](); //3、深拷贝
            strcpy(_pstr, rhs._pstr);
        }
        
        cout << "String & operator=(const String & rhs)" << endl;

        return *this;      //4、返回对象
    }

    ~String()
    {
        if (_pstr)
        {
            delete [] _pstr;
            _pstr = nullptr;
        }
        cout << "~String()" << endl;
    }

    void print()
    {
		//当cout输出一个char*类型的空指针，int* 的话会输出一个0
		//程序直接崩溃
		if(_pstr) {
			cout << _pstr << endl;
		}
    }

	size_t length() const {	
		if(_pstr) {
			return strlen(_pstr);	//strlen不能计算空指针  
		}else 
			return 0;
	}

private:
    char* _pstr;
};

 void test()
 {
    String str1;
    str1.print();
    String str2("hahahah");
    str2.print();
    String str3 = str2;
    String str4("hello");
    str4.print();

    cout << endl;
    str4 = str4;
    str4.print();

    str4 = str3;
    str4.print();

}

//查看拷贝构造函数第三种情况，要去掉编译优化g++ myString_bak.cc -fno-elide-constructors
String test1()
{
    String s1("asdf");
    return s1;
}

int main()
{

    //String s2 = test1();
    test1();
    return 0;
}