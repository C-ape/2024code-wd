#include <stdio.h>
#include <stdlib.h>


int main()
{
    char* pc;
    int* pi;
    char ** pcc;
    char arr1[] = "hello";
    char arr2[][5] = {"asdf", "qwer"};
    int arri[] ={12,2,1,4};
    int (*p)[4];
    p = &arri; //&arri是指向数组的指针，不能用arri，arri是数组的首地址

    printf("sizeof(pc) = %ld\n", sizeof(pc));
    printf("sizeof(pi) = %ld\n", sizeof(pi));
    printf("sizeof(pcc) = %ld\n", sizeof(pcc));
    printf("arr1 = %p, sizeof(arr1) = %ld\n", arr1, sizeof(arr1));
    printf("arr1+1 = %p, *(arr1+1) = %c\n", arr1+1, *(arr1+1));
    printf("&arr1 = %p, sizeof(&arr1) = %ld\n", &arr1, sizeof(&arr1));
    printf("&arr1+1 = %p, *(&arr1+1) = %d\n", &arr1+1, *(arr1+1));
    printf("arr2 = %p\n", arr2);
    printf("arr2+1 = %p, *(arr2+1) = %s\n", arr2+1, *(arr2+1));

    printf("p = %p, arri = %p\n", p, arri);
    printf("sizeof(*p) = %ld, p+1 = %p\n", sizeof(*p), p+1);

}