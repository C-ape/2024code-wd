#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <unordered_map>
#include <algorithm>


using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::string;
using std::unordered_map;

vector<vector<string>> groupAnagrams(vector<string>& strs) {
    int size = strs.size();
    vector<vector<string>> ret;
    //预留vector最大可能的空间
    ret.resize(size);  
    int mapIdx = 0;
    //申请一个map来存放同组字符串的键，值为vector中的下标
    unordered_map<string, int> hash_map;
    for (auto & str: strs)
    {
        string tmp = str;
        //先排序
        sort(tmp.begin(), tmp.end());
        cout << "tmp is " << tmp << "    str is " << str << endl;
        //然后在map中查找
        auto search = hash_map.find(tmp);
        if (search != hash_map.end())
        {
            cout << search->first << "   "  << search->second << endl;
            //找到的话就插入vector对应的vector中
            ret[search->second].emplace_back(str);
        }else
        {   
            //没有找到先在map中添加，然后插入vector中
            hash_map[tmp] = mapIdx;
            ret[mapIdx].push_back(str);     //为什么这里这样使用会是错的
            ++mapIdx;
            //ret.push_back({str});     //只能用这个
        }
    } 
    return ret;
}

vector<vector<string>> groupAnagrams1(vector<string>& strs) {
    unordered_map<string, vector<string>> mp;
    for (string& str: strs) {
        string key = str;
        sort(key.begin(), key.end());
        mp[key].emplace_back(str);
    }
    vector<vector<string>> ans;
    for (auto it = mp.begin(); it != mp.end(); ++it) {
        ans.emplace_back(it->second);
    }
    return ans;
}


vector<vector<string>> groupAnagrams2(vector<string>& strs) {
    vector<vector<string>> out;
    unordered_map<string, int> hashtable;
    string str;
    vector<string>::iterator it_begin = strs.begin();
    int j = 0;
    for (auto it = it_begin; it != strs.end(); it++)
    {
        str = (*it);
        sort(str.begin(), str.end());

        auto it_hash = hashtable.find(str);

        if (it_hash == hashtable.end())
        {
            hashtable[str] = j;
            j++;
            out.push_back({ *it });
        }
        else
        {
            out[it_hash->second].push_back(*it);		
        }
    }
    return out;
}

void test()
{
    vector<vector<string>> num;
    num.reserve(10);
    num[0].push_back("asdf");
    num[0].push_back("aaaa");
    num[1].push_back("qwer");
    cout << "num[0] = " << num[0][0] << endl;
    cout << "num[0] = " << num[0][1] << endl;
    cout << "num[0] = " << num[1][0] << endl;
}


int main()
{
    vector<string> strs = {"eat", "tea", "tan", "ate", "nat", "bat"};
    vector<vector<string>> ans;
    ans = groupAnagrams(strs);
    for (int idx = 0; idx < ans.size(); ++idx)
    {
        for (int j = 0; j < ans[idx].size(); ++j)
        {
            cout << ans[idx][j] << "    ";
        }
        cout << endl;
    }
    //test();
}


