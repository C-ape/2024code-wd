#include <iostream>
#include <vector>
#include <unordered_map>

using std::cout;
using std::endl;
using std::vector;
using std::unordered_map;
using std::string;

class Solution {
public:
    vector<int> findAnagrams(string s, string p) {
        vector<int> nums;
        //一个窗口的hash表，一个匹配串的hash表
        unordered_map<char, int> need, window;
        for(char c : p)
        {
            need[c]++;
        }
        int left = 0, right = 0;
        int valid = 0; //window和need里对应字符数量相等的有效数量
        while(right < s.size())
        {
            char c = s[right];
            right++;    //右移窗口
            //字符在need中存在，加入Windows中, 同时valid++
            if (need.count(c))
            {
                window[c]++;
                //当对应字符的数量相等时，有效数量加1
                if (window[c] == need[c])
                    valid++;
            }

            printf("window: [%d, %d), valid = %d\n", left, right, valid);
            //当valid满足need的长度时，就说明已经包含对应的字符
            //判断左窗口是否要收缩
            while(valid == need.size())
            {
                //当窗口的长度等于p的长度时，就是目标字符串
                if (right - left == p.size())
                {  
                    nums.push_back(left);
                    cout << nums[0] << endl;
                }
                char d = s[left];   //d是即将移除的字符
                left++;  //左移窗口
                if (need.count(d))
                {
                    if (window[d] == need[d])
                        valid--;
                    window[d]--;
                    
                }
            }
        }
        return nums;
    }
};


int main()
{
    string s1 = "cbaebabacd";
    string s2 = "abc";
    Solution s;
    s.findAnagrams(s1, s2);
    return 0;
}
