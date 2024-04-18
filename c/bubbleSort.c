#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void bubbleSort(int* buf, int len)
{
    for(int i = 0; i < len - 1; i++)    //外层控制比较的次数
    {
        bool flag = false;              //表示数据是否有交换，没有交换的话直接退出
        for (int j = 1; j < len - i; j++)   //内层控制比较的元素
        {
            if (buf[j] < buf[j - 1])
            {
                int tmp = buf[j - 1];
                buf[j - 1] = buf[j];
                buf[j] = tmp;
                flag = true;
            }
        }
        if (!flag)
        {
            break;      //没有数据交换，提前退出
        }
    }
}

//指针实现
void bubbleSort1(int* buf, int len)
{
    for(int i = 0; i < len - 1; i++)    //外层控制比较的次数
    {
        for (int j = 1; j < len - i; j++)   //内层控制比较的元素
        {
            if (*(buf + j) < *(buf + j - 1))
            {
                int tmp = *(buf + j - 1);
                *(buf + j - 1) = *(buf + j);
                *(buf + j) = tmp;
            }
        }
    }
}


void swap(int* buf, int a, int b)
{
    int tmp = buf[a];
    buf[a] = buf[b];
    buf[b] = tmp;
}

int partition(int* buf, int left, int right)
{
    //left的左边是小于buf[povit]的值，右边是大于buf[povit]的值
    int povit = right;      //比较值先选为最右边的值
    int tmp = 0;
    int idx = left;
    while (idx < right)
    {
        if (buf[idx] < buf[povit])
        {
            swap(buf, idx, left);
            left++;
        }
        idx++;
    }
    swap(buf, left, povit);
    return left;        //返回的是下标值
}

void quickSortPart(int* buf, int left, int right)
{
    //递归跳出条件，不要忘记写了
    if (left >= right)
    {
        return;
    }
    int povit = partition(buf, left, right);
    quickSortPart(buf, 0, povit - 1);
    quickSortPart(buf, povit + 1, right);
}

//快速排序
void quickSort(int* buf, int len)
{
    quickSortPart(buf, 0, len - 1);
}

void print(int * buf, int len)
{
    for (int i = 0; i < len; i++)
    {
        printf("buf[%d] = %d\n", i, buf[i]);
    }
}

//有序数组，二分查找，返回查找到的下标
int binSearch(int* nums, int numsSize, int target) {
    int left = 0, right = numsSize - 1;
    int mid = 0;
    while (left <= right)   //注意这里是<=，不是<
    {
        mid = left + (right - left) / 2;
        //mid = low + ((high - low) >> 1);  //位运算更快
        if (nums[mid] < target)
        {
            left = mid + 1;
        }else if(nums[mid] > target)
        {
            right = mid - 1;
        }else
        {
            return mid;
        }
    }
    return -1;
}


int main()
{
    int buf[] = {1, 13, 14, 5, 11, 12, 6, 8, 4, 5};
    int len = sizeof(buf)/sizeof(int); 
    //bubbleSort(buf, len);
    quickSort(buf, len);
    print(buf, len);
}