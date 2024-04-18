/*
题目描述
一只贪吃的猴子，来到一个果园，发现许多串香蕉排成一行，每串香蕉上有若干根香蕉。每串香蕉的根数由数组numbers给出。猴子获取香蕉，每次都只能从行的开头或者末尾获取，并且只能获取N次，求猴子最多能获取多少根香蕉。

输入描述
第一行为数组numbers的长度

第二行为数组numbers的值每个数字通过空格分开

第三行输入为N，表示获取的次数

输出描述
按照题目要求能获取的最大数值

输入
7
1 2 2 7 3 6 1
3
输出
10

输入
3
1 2 3
3
输出
6
说明
全部获取所有的香蕉，因此最终根数为1+2+3 = 6

*/

#include <iostream>
#include <vector>

using namespace std;

/*
左右指针的方式：
先计算前面N个数的和，左指针指向N-1的位置，右指针指向n-1的位置
同时向左偏移，计算max即为所求

前缀和的方式：
前缀和preSum[i] + 后缀和laterSum[j]的最大值，i+j=N
preSum[i] = nums[0]+...+nums[i] = preSum[i-1] + nums[i]
laterSum[j] = nums[n-1]+...+nums[n-j] = laterSum[j+1] + nums[n-j]
求max(preSum[i] = laterSum[N-i])
*/

int main() {
    int n;
    cin >> n;
    vector<int> numbers(n);
    for(int i=0; i<n; i++) cin >> numbers[i];

    int N;
    cin >> N;
    int tot = 0;
    //先计算前N个，然后左指针往左移，右指针也往右移，计算total的最大值    
    for(int i=0; i<N; i++)
        tot += numbers[i];

    int rs = tot;

    int l = N-1, r = n-1;
    while(l >= 0) {
        //total + numbers[right] - numbers[left]相当于N次总和从加number[left]变为加number[right]
        tot += numbers[r] - numbers[l];
        r--;
        l--;
        rs = max(rs, tot);
    }

    cout << rs << endl;

    return 0;
}
