#include <iostream>

using std::cout;
using std::endl;

//内部类 + 静态成员形式
//利用栈对象会自动释放的方式，静态成员程序结束后会自动释放
//单例模式不能自动释放原因是new的堆对象，需要手动delete，所以用栈对象，析构函数中手动delete就行
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
    class AutoRelease
    {
    public:
        AutoRelease()
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
    };

private:
    Singleton()
    {
        cout << "Singleton()" << endl;
    }
    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }

    static AutoRelease ar;
    //指针被声明为全局或静态，就会在程序启动时被初始化为 NULL 
    static Singleton * _pInstance;
};

//默认构造函数初始化
//ar不能在堆上，否则死锁
Singleton::AutoRelease Singleton::ar;
//饿汉模式，解决线程不安全的问题，静态成员函数与静态数据成员共用了作用域限定符
Singleton* Singleton::_pInstance = getInstance();


int main()
{
    Singleton * ps1 = Singleton::getInstance();
    printf("ps1'address = %p\n", ps1);
    return 0;
}