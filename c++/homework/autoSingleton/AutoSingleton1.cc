#include <iostream>

using std::cout;
using std::endl;

//单例模式自动释放
//栈对象自动释放
class Singleton  
{
public:
    static Singleton * getInstance()
    {
        if(_pInstance == nullptr)
        {
            _pInstance = new Singleton();
        }
        return _pInstance;
    }

    static void destory()
    {
        if(_pInstance)
        {
            delete _pInstance;
            _pInstance = nullptr;
        }
    }
    friend class AutoRelease;
private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }
    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
    //指针被声明为全局或静态，就会在程序启动时被初始化为 NULL 
    static Singleton * _pInstance;
};

//饱汉模式：线程不安全，多线程的情况下会多次new
//Singleton* Singleton::_pInstance = nullptr;
//getInstance()前面的Singleton::省略了
//饿汉模式，解决线程不安全的问题，静态成员函数与静态数据成员共用了作用域限定符
Singleton* Singleton::_pInstance = getInstance();

//利用类对象在栈上创建单例实现自动释放
class AutoRelease
{
public:
    AutoRelease()
    :_p(Singleton::getInstance())
    {
        cout << "AutoRelease()" << endl;
    }

    ~AutoRelease()
    {
        cout << "~AutoRelease()" << endl;
        if(Singleton::_pInstance)
        {
            delete Singleton::_pInstance;
            Singleton::_pInstance = nullptr;
        }
    }

    void print()
    {
        cout << _p << endl;
    }
private:
    Singleton * _p;
};

int main()
{
    // Singleton * p1 = Singleton::getInstance();
    // Singleton * p2 = Singleton::getInstance();
    // cout << p1 << endl;
    // cout << p2 << endl;
    AutoRelease p1;
    p1.print();
    AutoRelease p2;
    p2.print();
    return 0;
}