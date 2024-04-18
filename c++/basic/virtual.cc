#include <iostream>

using std::cout;
using std::endl;

//虚函数带默认参数的例子


class A
{ 
public:
    virtual
    void func(int val = 1)
    {
        cout << "A->" << val << endl;
    } 

    void print1()
    {}

private:
    long _a;
};

class B
: public A
{ 
public:
    virtual
    void func(int val = 10)
    {
        val += 1;
        cout << "B->" << val << endl;
    }

    void print()
    {
        cout << _b << endl;
    }
    int _c;
private:
    long _b;
};

void test1()
{
    B b;
    //这里执行的是派生类到基类的隐式转换，p1是类A的指针，指向的是A的对象的空间，不能访问B的数据成员
    //但是能够访问func函数，因为func是虚函数，A和B中都有定义
    //
    A *p1 = &b;     
    B *p2 = &b;
    //p1->_c = 10;  //不能访问              
    //p1->print();    //非虚函数不能访问

    //执行的是B的函数体，参数使用的是A的默认参数
    p1->func();
    //这个肯定执行的是B的
    p2->func();
}

void test3()
{
    int a = 125;
    float *pa =NULL;
    float b;

    pa = (float *)&a;
    b = *pa;
    //打印结果是乱码,因为类型转换的是a的指针，并不是a的空间，所以a的空间还是int型存储，赋值给float会乱码
    printf("%f\n",b);
}
int main(void)
{
    test1();
    test3();
    return 0;
}