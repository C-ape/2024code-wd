#include <iostream>

using std::cout;
using std::endl;

#define STACK_SIZE 10

class Stack
{
public:
    Stack()
    :_capacity(STACK_SIZE)
    ,_stackSize(0)
    ,_stack(new int[_capacity]())
    {
        cout << "Stack()" << endl;
    }

    //三合成原则，拷贝构造函数，赋值运算符函数也要显示定义
    ~Stack()
    {
        if(_stack)
        {
            delete [] _stack;
            _stack = nullptr;
        }
        cout << "~Stack()" << endl;
    }

    bool empty()
    {
        if (_stackSize == 0)
        {
            return true;
        }else
        {
            return false;
        }
    }

    bool full()
    {
        if (_stackSize == _capacity)
        {
            return true;
        }else
        {
            return false;
        }
    }

    void push(int num)
    {
        if (!full())
        {
            _stack[_stackSize] = num;
            _stackSize++;
        }else
        {
            cout << "Stack is full" << endl;
        }
        return;
    }

    void pop()
    {
        if(!empty())
            _stackSize--;
        else
            cout << "stact is empty" << endl;
    }

    int top()
    {
        if(!empty())
            return _stack[_stackSize - 1];
        else
            cout << "stack is empty" << endl;
    }

    int capacity()
    {
        return _capacity;
    }

    int stackSize()
    {
        return _stackSize;
    }
private:
    int _capacity;      //栈的容量
    int _stackSize;    //栈的元素个数
    int* _stack;


};

int main()
{
    Stack s1;
    cout << "s1.capacity is " << s1.capacity() << endl;
    cout << "s1.size is " << s1.stackSize() << endl;
    s1.push(1);
    s1.push(2);
    s1.push(3);
    s1.push(4);
    s1.push(5);
    s1.push(6);
    s1.push(7);
    s1.push(8);
    s1.push(9);
    s1.push(10);
    s1.push(3);
    cout << "s1.top()" << s1.top() << endl;

    s1.pop();

    cout << "s1.top()" << s1.top() << endl;

    return 0;
}