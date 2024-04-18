#include "mysql.h"

#define STR_LEN 10  //salt的长度

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


int addUser(loginMsg_t login)
{
    MYSQL_ROW* row;
    char query[300] = {0};
    char salt[50] = {0};
    get_salt(salt);
    puts(salt);
    char *crypt_str = NULL;
    crypt_str = crypt(login.passwd, salt);
    puts(crypt_str);
    //根目录设置为用户名
    sprintf(query, "%s '%s', '%s', '%s', '%s')", "insert into user (username, salt, cryptpasswd, pwd) values (",
                                 login.name, salt, crypt_str, login.name); 
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


//每次注册成功插入虚拟用户表根目录信息
int addRootDir(loginMsg_t login)
{
    MYSQL_ROW* row;
    char query[300] = {0};

    int owner_id = 0;
    findIdArgByName(login.name, &owner_id, ID);

    //根目录设置为用户名
    sprintf(query, "%s %d, '%s', %d, %d, %d, %d)", 
            "insert into virtual_file (parent_id, filename, owner_id, md5, filesize, type) value (",
            0, login.name, owner_id, 0, 0, 0);
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

//用于注册的时候查询用户名是否存在，不存在返回0, 失败返回-1，存在返回1
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
                printf("username exist\n");
                ret = 1;

            }else
            {   
                if (mysql_errno(conn))  //出现错误
                {
                    fprintf(stderr, "Error: %s\n", mysql_error(conn));
                    ret = -1;
                }else
                {
                    printf("don't find username\n");
                    ret = 0;
                }
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

//严谨点用mysql_errno,不想改了，没啥影响
//错误返回-1，没找到salt返回1，找到返回0
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
                ret = 0;

            }else
            {   
                //没有找到salt
                printf("don't find salt\n");
                ret = 1;
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

//通过name，查找数据库中的crypt认证,
//crypt相等返回0，不相等返回1，错误返回-1
int findCryptByName(char* name,  char* token)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s%s%s", "select cryptpasswd from user where username = \"", name, "\"");
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
                printf("crypt is %s, token is %s\n", row[0], token);
                if (strcmp(token, row[0]) == 0)
                {
                    ret = 0;
                }else
                {
                    //密文不一样，密码错误
                    ret = 1;
                }

            }else
            {   
                //没有找到crypt
                printf("don't find crypt\n");
                ret = -1;
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

//公共接口，通过名字查询数据库内容
//str:返回的字符串，arg：命令参数
//失败返回-1，没找到返回1，成功返回0
int findStrArgByName(char* name, char* str, const char* arg)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s %s %s%s%s", "select", arg, "from user where username = \"", name, "\"");
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
                strcpy(str, row[0]);
                ret = 0;

            }else
            {   
                //没有找到
                printf("don't find %s\n", arg);
                ret = 1;
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


//将查询语句的结果存入fileinfo
int findAllByDir(char* name, int owner_id, int pre_id, fileInfo_t* fileinfo)
{
    MYSQL_RES* res;

    char query[300] = {0};
    sprintf(query, "%s%d%s%d", "select filename, type from virtual_file where owner_id = ", owner_id, "and parent_id = ", pre_id);
    puts(query);
    int t, ret = 0;
    t = mysql_query(conn, query); //成功返回0，不成功返回非0值
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
        ret = -1;
    }else
    {
        res = mysql_store_result(conn);
        if (res)
        {
            MYSQL_ROW row;  //typedef char** MYSQL_ROW
            int i = 0;
            while((row = mysql_fetch_row(res)))
            {
                //遍历当前的每一列,每一列都只有filename和type两个参数
                printf("row[0] is %s, row[1] is %s\n", row[0], row[1]);
                strcpy(fileinfo[i].filename, row[0]);
                strcpy(&fileinfo[i].type, row[1]);
                i++;
            }
            mysql_free_result(res);
        }else
        {
            printf("Error making query:%s\n",mysql_error(conn));
            ret = -1;
        }

    }
    return ret;
}


//公共接口，通过名字查询数据库内容
//str:返回的字符串，arg：命令参数
//失败返回-1，没找到返回1，成功返回0
int findIdArgByName(char* name, int* id, const char* arg)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s %s %s%s%s", "select", arg, "from user where username = \"", name, "\"");
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
                *id = atoi(row[0]);
                ret = 0;

            }else
            {   
                //没有找到
                printf("don't find %s\n", arg);
                ret = 1;
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


//公共接口，通过名字查询数据库虚拟文件表内容
//str:返回的字符串，arg：命令参数
//失败返回-1，没找到返回1，成功返回0
//根目录直接是用户名
int findRootIdByNameFromVir(char* filename, int* id, const char* arg, int pre_id)
{
    MYSQL_RES* res;
    MYSQL_ROW row;
    char query[300] = {0};
    sprintf(query, "%s %s %s '%s' %s %d", 
                "select", arg, "from virtual_file where filename = ", filename, "and parent_id = ", pre_id);
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
                *id = atoi(row[0]);
                ret = 0;

            }else
            {   
                //没有找到
                printf("don't find %s\n", arg);
                ret = 1;
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


int insertDirVir(char* dirname, int pre_id, int owner_id)
{
    MYSQL_ROW* row;
    char query[300] = {0};

    //type为0表示目录
    sprintf(query, "%s %d, '%s', %d, %d, %d, %d)", 
            "insert into virtual_file (parent_id, filename, owner_id, md5, filesize, type) value (",
            pre_id, dirname, owner_id, 0, 0, 0);
    puts(query);
    int t;
    t = mysql_query(conn, query); //成功返回0，不成功返回非0值
    if(t)
    {
        printf("Error making query:%s\n",mysql_error(conn));
        return -1;
    }else
    {
        printf("insert %s successful\n", dirname);
    }
    return 0;
}