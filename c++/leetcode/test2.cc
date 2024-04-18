#include <iostream>
#include <vector>

using std::vector;
using namespace std;

int main() {
    vector<int> costs;
    vector<int> days;

    int cost;
    int cnt = 4;
    int count = 0;
    while(cnt)
    {
        cin >> cost;
        costs.push_back(cost);
        cnt--;
    }
    while(cin.get() == '\n');
    while(cin >> cost)
    {
        days.push_back(cost);
        if (cin.get() == '\n')
        {
            cout << count;
        }
    }

}
