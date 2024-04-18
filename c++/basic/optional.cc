#include <optional>
#include<iostream>

using std::cout;
using std::endl;


//编译方法：g++ optional.cc -std=c++17
int main()
{
    bool T = true;
    std::optional<bool> i = T;
    cout << sizeof(i) << endl;
    return 0;
}
