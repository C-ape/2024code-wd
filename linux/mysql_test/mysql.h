#define _GNU_SOURCE
#include <mysql/mysql.h>

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>


#define ORI_PWD "~/"


//登录的用户名和密码
typedef struct loginMsg_s
{
    
    char name[50];
    char passwd[100];
    //char token[PASSWD_LENGTH];
}loginMsg_t;


MYSQL* conn; //全局的数据库连接

void get_salt(char *str);
int mysqlInit(MYSQL **conn);
