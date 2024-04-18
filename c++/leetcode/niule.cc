/*
疫情期间，小明隔离在家，百无聊赖，在纸上写数字玩。他发明了一种写法:

给出数字个数n和行数m (0 < n <= 999，0 < m <= 999)，从左上角的1开始，按照顺时针螺旋向内写方式，依次写出2,3...n，最终形成一个m行矩阵。

小明对这个矩阵有些要求:

每行数字的个数一样多
列的数量尽可能少
填充数字时优先填充外部
数字不够时，使用单个*号占位
输入描述
两个整数，空格隔开，依次表示n、m

输出描述
符合要求的唯一矩阵
输入：
9 4

输出：
1 2 3
* * 4
9 * 5
8 7 6

说明：
9个数字写成4行，最少需要3列


*/


#include <iostream>

#include <string>

using std::string;
using namespace std;

int main() {
    int size = 0;   //数字个数
    cin >> size;
    int line = 0;
    cin >> line;    //行数
    
    if (size <= line)
    {
        for (int idx = 1; idx <= line; idx++)
        {
            if (idx <= size)
            {
                cout << idx << endl;
            }else {
                cout << "*" << endl;
            }
        }
        return 0;
    }

    int colNum = size / line + 1;       //总列数
    int **array = new int*[line];
    for (int i = 0; i < line; i++)
    {
        array[i] = new int[colNum];
    }
    //创建一个二维数组来存放所有的元素，初始化全为0
    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < colNum; j++)
        {
            array[i][j] = 0;
        }
    }
   
    int num = 1;                        //填充的数从1开始
    int row = 0, col = 0;
    int dir = 0;                        //行
    //四个方向向量(0,1)->右移，(1,0)->下移,(0，-1)->左移，(-1,0)->上移
    int dirs[][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};
    while (num <= size)
    {
        //cout << "row =" << row << endl << "col =" << col << endl;

        array[row][col] = num;
        //cout << "array[][] = " << array[row][col] << endl; 
        num++;
        int newCol = col + dirs[dir][1];    //先更新行
        int newRow = row + dirs[dir][0];
        if (newRow < 0 || newRow >= line || newCol < 0 ||newCol >= colNum || array[newRow][newCol] != 0)
        {
            dir = (dir + 1) % 4;       //不能继续移动就更新移动的方式 
        }
        //更新行列索引
        col += dirs[dir][1];
        row += dirs[dir][0];
    }

    for (int i = 0; i < line; i++)
    {
        for (int j = 0; j < colNum; j++)
        {
            string s;
            if (array[i][j] == 0)
            {
                s = "*";
            }else
            {
                s = to_string(array[i][j]);
            }
            cout << s << " ";
        }
        cout << endl;
    }
    return 0;
}