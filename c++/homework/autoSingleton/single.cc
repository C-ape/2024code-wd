#include <iostream>
#include <assert.h>

using std::cout;
using std::endl;

 //单例模式：
//该对象不能是一个栈对象, 全局/静态对象
//该对象只能是堆对象
class Singleton
{
public:
    //3、定义一个返回值为类指针的静态成员函数
    static Singleton* getInstance()
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
            delete _pInstance;   //delete会自动调用析构函数
            _pInstance = nullptr;
        }
    }
private:
    Singleton()    //1、将构造函数私有化
    {
        cout << "Singleton()" << endl;
    }
    ~Singleton()
    {
        cout << "~Singleton()" << endl;
    }
    static Singleton* _pInstance;   //2、在类中定义一个静态的指向本类型的指针变量
};

Singleton * Singleton::_pInstance = nullptr;    //静态成员的初始化必须放在类外


int main()
{
    Singleton* test1 = Singleton::getInstance();
    Singleton* test2 = Singleton::getInstance();
    assert(test1 == test2);
    Singleton::destory();
}
