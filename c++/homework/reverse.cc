#include <iostream>

using std::cout;
using std::endl;

class Solution {
public:
    bool isPalindrome(int x) {
        cout << "x is :" << x << endl;
        if (x < 0 || (x % 10 == 0 && x != 0))
        {
            return false;
        }

        int revertedNumber = 0;
        //跳出循环的条件是翻转的数大于等于剩下的数
        while(revertedNumber < x)
        {
            revertedNumber = revertedNumber * 10 + x % 10;
            x = x / 10;
        }
        cout << "x is :" << x << endl;
        cout << "nums is :" << revertedNumber << endl;
        //有数字个数是奇数的情况和偶数的情况
        return revertedNumber == x || x == revertedNumber / 10;
    }
};

int main()
{
    Solution s;
    if (s.isPalindrome(121))
    {
        cout << "ture"  << endl;
    }else
    {
        cout << "false" << endl;
    }


}
