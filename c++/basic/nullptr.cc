#include <iostream>

using std::cout;
using std::endl;


class A
{
public:
    void func(){ std::cout << "A::func()" << endl; }
};

class B : public A
{
public:
    void func(){ std::cout << "B::func()"<< endl; }
};

class C : public A
{
public:
    void func(){ std::cout << "C::func()" << endl; }
};

int main()
{
	C * pc = nullptr;
	pc->func();
}