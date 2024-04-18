#include <iostream>

using std::cout;
using std::endl;

class Queue
{
public:
    Queue(int capacity = 10)
    :_capacity(capacity)
    ,_front(0)
    ,_rear(0)
    ,_base(new int[_capacity]())
    {
        cout << "Queue(int)" << endl;
    }

    ~Queue()
    {
        if(_base)
        {
            delete [] _base;
            _base = nullptr;
        }
        cout << "~Queue()" << endl;
    }

    bool empty() const;
	bool full() const;
	void push(int number);
	void pop();
	int front() const;
	int back() const;
    int capacity() const
    {
        return _capacity;
    }
    int queueSize() const
    {
        return (_rear - _front + _capacity)%_capacity;
    }
private:
    int _capacity; //队列容量
    int _front; //队首元素
    int _rear;  //队尾元素下一个位置
    int * _base;
};

bool Queue::empty() const
{
    return _front == _rear;
}

bool Queue::full() const
{
    return (_rear + 1)%_capacity == _front;
}

//环形队列预留一个空间不使用，用于判断队列是否满，如果_rear指向的是队尾元素，那么判空和判满的条件都是_front==_rear，不易区分
//所以_rear指向队尾元素的下一个位置，预留一个空间判断
//也可以使用另一个解决方法，再用一个变量表示队列长度，用于判断队列空或者满
void Queue::push(int num)
{
    if (!full())
    {
        _base[_rear] = num;
        _rear++;
        _rear = _rear % _capacity;
    }else
    {
        cout << "queue is full" << endl;
    }
}

void Queue::pop()
{
    if (!empty())
    {
        _front++;
        _front = _front % _capacity;
    }else
    {
        cout << "queue is empty" << endl;
    }
}

int Queue::front() const
{
    return _base[_front];
}

int Queue::back() const
{
    return _base[(_rear - 1 + _capacity) % _capacity];
}

void test0() 
{
	Queue que;
	cout << "que is empty? " << que.empty() << endl;
	que.push(1);
    cout << "que's front element: " << que.front() << endl;
	cout << "que is empty? " << que.empty() << endl;
	for(int idx = 2; idx < 12; ++idx) {
		que.push(idx);
	}
    cout << "que size is  " << que.queueSize() << endl;
    cout << "que capacity is  " << que.capacity() << endl;
    cout << "que is full? " << que.full() << endl;

	cout << "pop an element "<< endl;
	que.pop();
	cout << "que's front element: " << que.front() << endl;
	cout << "push an element " << endl;
	que.push(10);
	cout << "que's back element: " << que.back() << endl;
	cout << "que is full? " << que.full() << endl;

	while(!que.empty()) {
		cout << que.front() << " ";
		que.pop();
	}

	cout << "\nque is empty? " << que.empty() << endl;
} 
 
int main(void)
{
	test0();
	return 0;
}
