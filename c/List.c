//链表相关的操作

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct ListNode
{
    int value;
    struct ListNode* next;
}Node, *pNode;

void printList(pNode phead)
{
    while(phead)
    {
        printf("phead->value = %d\n", phead->value);
        phead = phead->next;
    }
    return;
}


//头插法
void headInsert(pNode* pphead, pNode* pptail, int value)
{
    pNode newNode = (pNode)malloc(sizeof(Node));
    memset(newNode, 0, sizeof(Node));
    newNode->value = value;

    if (NULL == *pphead)
    {
        *pphead = newNode;
        *pptail = newNode;
    }else
    {
        newNode->next = *pphead;
        *pphead = newNode;
    }
}

//尾插法
void tailInsert(pNode* pphead, pNode* pptail, int value)
{
    pNode newNode = (pNode)malloc(sizeof(Node));
    memset(newNode, 0, sizeof(Node));
    newNode->value = value;

    if (NULL == *pphead)
    {
        *pphead = newNode;
        *pptail = newNode;
    }else
    {
        (*pptail)->next = newNode;
        *pptail = newNode;
    }
}

void destroy_list(pNode* pphead) {
	// 释放结点空间
	pNode curr = *pphead;
	while (curr != NULL) {
		// 保存curr后继结点
		Node* next = curr->next;
		free(curr);
		curr = next;
	}
}

//有序插入,升序
void orderInsert(pNode* pphead, pNode* pptail, int value)
{
    pNode newNode = (pNode)malloc(sizeof(Node));
    memset(newNode, 0, sizeof(Node));
    newNode->value = value;

    pNode pcur = NULL;
    pNode ppre = NULL;     //用ppre吧，这样就可以直接使用while(pcur)，不用考虑其他边界情况
    if(NULL == *pphead)
    {
        *pphead = newNode;
        *pptail = newNode;
    }else
    {
        if (newNode->value <= (*pphead)->value)
        {
            //头插法
            newNode->next = *pphead;
            *pphead = newNode;
        }else if (newNode->value >= (*pptail)->value)
        {
            //尾插法
            (*pptail)->next = newNode;
            *pptail = newNode;
        }else
        {
            //中间插入
            pcur = *pphead;
            while (pcur)
            {
                if (newNode->value < pcur->value)
                {
                    ppre->next = newNode;
                    newNode->next = pcur;
                    return;
                }
                ppre = pcur;
                pcur = pcur->next;
            }  
        }
    }
}

void deleteNode(pNode* pphead, int val)
{
    pNode pcur = *pphead;
    pNode ppre = NULL;
    pNode tmp = NULL;
    while(pcur)
    {
        if ((*pphead)->value == val)
        {
            *pphead = (*pphead)->next;
            pcur = *pphead;
            continue;
        }
        if (pcur->value == val)
        {
            tmp = pcur;
            ppre->next = pcur->next;
            pcur = pcur->next;
            free(tmp);
            tmp = NULL;
        }else
        {
            ppre = pcur;
            pcur = pcur->next;
        }

    }
}


int main()
{
    pNode phead = NULL;
    pNode ptail = NULL;

    for (int i = 0; i < 5; i++)
    {
        //tailInsert(&phead, &ptail, i);
        //headInsert(&phead, &ptail, i);
        //orderInsert(&phead, &ptail, i);
    }
    orderInsert(&phead, &ptail, 6);
    orderInsert(&phead, &ptail, 1);
    orderInsert(&phead, &ptail, 5);
    orderInsert(&phead, &ptail, 1);
    orderInsert(&phead, &ptail, 3);
    orderInsert(&phead, &ptail, 1);
    printList(phead);
    deleteNode(&phead, 1);
    printList(phead);
    return 0;
}
