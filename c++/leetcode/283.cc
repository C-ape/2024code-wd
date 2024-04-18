#include <iostream>
#include <vector>
#include <unordered_map>

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::string;

void moveZeroes(vector<int>& nums) {
    int size = nums.size();
    int j = 0;
    for (int idx = 0; idx < size; idx++)
    {
        if (nums[idx] != 0)
        {
            nums[j] = nums[idx];
            j++;
        }
    }
    for (; j < size; j++)
    {
        nums[j] = 0;
    }
}

int main()
{
    vector<int> nums = {0,1,0,3,12};
    moveZeroes(nums);
    for (auto num : nums)
    {
        cout << num << endl;
    }
}