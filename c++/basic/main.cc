#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;


int main()
{
    vector<int> nums;
    nums.reserve(10);
    for(int idx = 0;idx <nums.capacity(); ++idx)
    {
        nums[idx] = idx;
        cout << nums[idx] << endl;   
    }
    return 0;
}