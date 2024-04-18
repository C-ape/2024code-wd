#include "mysql.h"


#define STR_LEN 10  //salt的长度

int mysqlInit(MYSQL **conn)
{
    char server[]="localhost";
    char user[]="root";
    char password[]="root1234";
    char database[]="net_disk";//要访问的数据库名称
    *conn=mysql_init(NULL);
    if(!mysql_real_connect(*conn,server,user,password,database,0,NULL,0))
    {
        printf("Error connecting to database:%s\n",mysql_error(*conn));
        exit(0);
    }else{
        printf("mysql connected...\n");
    }
    return 0;
}

void get_salt(char *str)
{
    str[STR_LEN + 1] = 0;
    int i,flag;
    srand(time(NULL));
    for(i = 0; i < STR_LEN; i ++)
    {
        flag = rand()%3;
        switch(flag)
        {
        case 0:
            str[i] = rand()%26 + 'a';
            break;
        case 1:
            str[i] = rand()%26 + 'A';
            break;
        case 2:
            str[i] = rand()%10 + '0';
            break;
        }
    }
    *str='$';str[1]='5';str[2]='$';
    return;
}



//用于注册的时候查询用户名是否存在
int addUser(loginMsg_t login)
{
    MYSQL_ROW* row;
    char query[300] = {0};
    char salt[50] = {0};
    get_salt(salt);
    puts(salt);
    char *crypt_str;
    crypt_str = crypt(login.passwd, salt);
    puts(crypt_str);
    sprintf(query, "%s '%s', '%s', '%s', '%s')", "insert into user (username, salt, cryptpasswd, pwd) values (",
                                 login.name, salt, crypt_str, ORI_PWD);
    puts(query);
    int t;
    t = mysql_query(conn, query); //成功返回0，不成功返回非0值
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }else
    {
        printf("insert %s successful\n", login.name);
    }
    return 0;

}

//用于注册的时候查询用户名是否存在，不存在返回0
int findName(const char* name)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s%s%s", "select username from user where username = \"", name, "\"");
    puts(query);
    int t, ret;
    t = mysql_query(conn, query); //成功返回0，不成功返回非0值
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
        ret = -1;
    }else
    {
        res = mysql_use_result(conn);  //成功返回结果集，错误返回null
        if (res)
        {
            if ((row = mysql_fetch_row(res)) != NULL)  //如果没有要检索的行或者出现错误，返回NULL
            {
                //用户名已存在
                ret = 1;

            }else
            {   
                printf("don't find username\n");
                ret = 0;
            }
        }else
        {
            printf("find name query error\n");
            ret = -1;
        }
        mysql_free_result(res);
    }
    return ret;
}

//错误返回-1ï[MBG0没找到salt返回0，找到返回1
int findSaltByName(const char* name, char* salt)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s%s%s", "select salt from user where username = \"", name, "\"");
    puts(query);
    int t, ret;
    t = mysql_query(conn, query); //成功返回0，不成功返回非0值
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
        ret = -1;
    }else
    {
        res = mysql_use_result(conn);  //成功返回结果集，错误返回null
        if (res)
        {
            if ((row = mysql_fetch_row(res)) != NULL)  //如果没有要检索的行或者出现错误，返回NULL
            {
                //只会有一行，而且salt必须存在，直接赋值就行了
                strcpy(salt, row[0]);
                ret = 1;

            }else
            {   
                //没有找到salt
                printf("don't find salt\n");
                ret = -1;
            }
        }else {
             printf("find name query error\n");
             ret = -1;
        }
        mysql_free_result(res);
    }
    return ret;
}

int main()
{
    mysqlInit(&conn);
    loginMsg_t login;
    char name[20] = "haha";
    char salt[30] = "gggg";
    char passwd[100] = "root1234";
    int ret = findSaltByName(name, salt);
    printf("ret = %d\n", ret);
    ret = findName(name);
    strcpy(login.name, name);
    strcpy(login.passwd, passwd);
    if (ret == 0)
    {

        addUser(login);
    }
    return 0;
}
