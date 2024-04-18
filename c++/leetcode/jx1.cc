/*
炬星科技第一道面试题
输入一个数组，输出仅出现一次的元素 (请使用c/c++语言实现)
样例1
输入：1 2 3 4 4 5 5 6 7 8 9 9
输出：1 2 3 6 7 8

样例2
输入：1 1 1 1 1 1 2 3 9 9 9 9 9 5 4
输出：2 3 5 4

*/

#include <iostream>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::vector;

void print(vector<int> & nums, int len)
{
    for (int idx = 0; idx < len; ++idx)
    {
        cout << nums[idx] << " ";
    }
    cout << endl;
}

/*
left存放符合条件的值，right遍历数组
只有下标为right-1，right，right+1三个数都不同的时候，才能插入left
注意：第一个和最后一个需要单独判断
*/
int removeElement(vector<int>& nums)
{
    int left = 0, right = 1;
    //第一个单独判断
    if (nums[right] != nums[left])
    {
        left++;
    }
    while(right < nums.size() - 1)
    {
        if (nums[right] != nums[right + 1] && nums[right] != nums[right - 1])
        {
            
            nums[left] = nums[right];
            left++;
        }
        right++;
    }
    //最后一个单独判断，因为while循环跳出的情况忽略了最后一个数
    if (nums[right] != nums[right - 1])
    {
        nums[left] = nums[right];
    }
    //print(nums, left + 1);
    return (left + 1);
}

int main()
{
    vector<int> nums;
    int num;
    //cin输入的有空格的话，就不会立即cout，等到下一个cin的值直接为换行才能输出
    while (cin >> num)
    {
        nums.push_back(num);
        if (cin.get() == '\n')
        {
            int len = removeElement(nums);
            print(nums, len);
            nums.clear();
        }
    }
}
