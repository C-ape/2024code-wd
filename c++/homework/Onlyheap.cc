#include <iostream>
#include <string.h>
using std::cout;
using std::endl;


//只能生成堆对象：析构函数私有化，delete在类里面调用
// new 表达式的步骤
// 1、调用operator new 标准库函数申请空间
// 2、在该空间上调用构造函数初始化对象
// 3、返回一个指向对象的指针
// delete表达式的步骤
// 1、调用析构函数，回收数据成员申请的资源
// 2、调用operator delete标准库函数释放该对象所用的内存
class Student
{
public:
    Student(int id, const char* name)
    :_id(id)
    ,_name(new char[strlen(name) + 1]())
    {
        cout << "Student(int, const char*)" << endl;
        strcpy(_name, name);
    }

    void destory()
    {
        //直接调用析构函数，并不能释放对象
		/* this->~Student(); */
        delete this;    //this是指向整个对象的指针，delete this才是将对象释放
    }

    void* operator new(size_t sz)   //申请新对象时调用
    {
        cout << "operator new" << endl;
        return malloc(sz);
    }

    void operator delete(void* p)   //释放对象时调用
    {
        cout << "operator delete" << endl;
        free(p);
    }

    void print()
    {
        cout << "id is:" << _id << endl
             << "name is:" << _name << endl;
    }

private:
    ~Student()
    {
        if (_name)  
        {
            //这里是释放构造函数中new的空间，不是类自己的方法
            delete [] _name;
            _name = nullptr;
        }
        cout << "~Student()" << endl;
    }

    int _id;    //数据成员
    char* _name;
};


int main()
{
    Student * s1 = new Student(1, "asdf");
    s1->print();
    s1->destory();
}