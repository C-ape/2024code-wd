#include <iostream>
#include <pthread.h>
#include <stdlib.h>

using std::cout;
using std::endl;

//pthread_once + atexit
//pthread_once:多线程的情况下只执行一次，制作库的时候可能会用到
class Singleton  
{
public:
    static Singleton * getInstance()
    {
        //只会执行一次，就不用判断是否为空了
        pthread_once(&_once, init);
        return _pInstance;
    }

    static void init()
    {
        _pInstance = new Singleton();
        atexit(destory);
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

    static pthread_once_t _once;
    //指针被声明为全局或静态，就会在程序启动时被初始化为 NULL 
    static Singleton * _pInstance;
};

//getInstance()前面的Singleton::省略了
//饿汉模式，解决线程不安全的问题，静态成员函数与静态数据成员共用了作用域限定符
Singleton* Singleton::_pInstance = getInstance();
pthread_once_t Singleton::_once = PTHREAD_ONCE_INIT;

int main()
{
    Singleton * p1 = Singleton::getInstance();
    Singleton * p2 = Singleton::getInstance();
    cout << p1 << endl;
    cout << p2 << endl;
    return 0;
}