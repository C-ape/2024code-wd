#include <iostream>
#include <stdlib.h>

using std::cout;
using std::endl;

//atexit + 饿汉模式
class Singleton  
{
public:
    static Singleton * getInstance()
    {
        if(_pInstance == nullptr)
        {
            _pInstance = new Singleton();
            //注册函数，程序结束后执行
            //需要放里面，只注册一次，如果放if外面，多线程的情况下会执行多次destory，会double free
            //这里的destory需要使用static的，如果不是静态成员函数，会有this指针，不满足atexit的参数
            atexit(destory);
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

int main()
{
    Singleton * p1 = Singleton::getInstance();
    Singleton * p2 = Singleton::getInstance();
    cout << p1 << endl;
    cout << p2 << endl;
    return 0;
}