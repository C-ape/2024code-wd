#include <iostream>
#include <string.h>
using std::cout;
using std::endl;


//只能生成栈对象：operator new函数私有化

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

    ~Student()
    {
        if(_name)
        {
            delete [] _name;
            _name = nullptr;
        }
        cout << "~Student()" << endl;
    }

    void print()
    {
        cout << "id is:" << _id << endl
             << "name is:" << _name << endl;
    }

private:
    void* operator new(size_t size);
    void operator delete(void *p);
    
    int _id;    //数据成员
    char* _name;
};


int main()
{
    Student s1(1, "haha");
    s1.print();
}