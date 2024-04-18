#include"func.h"


char* mode_to_str(mode_t mode, char* buf)
{
    mode_t type = mode & S_IFMT;//通过man 7 inode可以查阅相关信息
    switch (type) 
    {
        case S_IFBLK:  *buf = 'b';  break;
        case S_IFSOCK: *buf = 's'; break;
        case S_IFCHR:  *buf = 'c';  break;
        case S_IFDIR:  *buf = 'd';  break;
        case S_IFIFO:  *buf = 'F';  break;
        case S_IFLNK:  *buf = 'l';  break;
        case S_IFREG:  *buf = '-'; break;
        default:       *buf = '?';  break;
    }
    //&操作确定文件的权限"rwxrwxrwx",
    //1-9位，按位与，每三位确定一种类型权限，
    //第一位结果为1说明有读权限，第二位为写权限，第三位为执行权限
    for (int i = 0; i < 3; i++)
    {
        if (mode & (1<<(8-i*3)))
        {
            buf[1+i*3] = 'r';
        }else
        {
            buf[1+i*3] = '-';
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (mode & ((1<<7-i*3)))
        {
            buf[2+i*3] = 'w';
        }else
        {
            buf[2+i*3] = '-';
        }
    }
    for (int i = 0; i < 3; i++)
    {
        if (mode & (1<<(6-i*3)))
        {
            buf[3+i*3] = 'x';
        }else
        {
            buf[3+i*3] = '-';
        }
    }
    return buf;
}

//将时间装换为ls显示的字符串模式，ctime转换的结果自带换行，需要重新写这个函数
char* time_to_str(time_t *time, char* buf)
{
    struct tm* time_str = localtime(time);
    const char * month[13] = {NULL,"Jan", "Feb", "Mar", "Apr", "May", "Jun", 
                            "Jul","Aug", "Sep", "Oct", "Nov", "Dec"};
    sprintf(buf, "%s %02d %02d:%02d", 
            month[time_str->tm_mon + 1], 
            time_str->tm_mday,
            time_str->tm_hour,
            time_str->tm_min);
    return buf;
}


int main(int argc, char* argv[])
{
    ARGS_CHECK(argc, 2);
    DIR* dp = opendir(argv[1]);
    ERROR_CHECK(dp, NULL, "opendir");
    struct dirent* dirent;
    struct stat statbuf;
    char mode_buf[12] = {0};
    char time_buf[20] = {0};
    while(NULL != (dirent = readdir(dp)))
    {
        char buf[512] = {0};
        sprintf(buf, "%s%s", argv[1], dirent->d_name);
        int ret = stat(buf, &statbuf); //直接使用目录的名字(dirent->name)切换目录了找不到文件，要使用绝对路径
        ERROR_CHECK(ret, -1, "stat");
        //类型 权限 硬连接数 用户名 组名 大小 修改时间 文件名字
        printf("%s %ld %s %s %6ld %s %s\n", 
            mode_to_str(statbuf.st_mode, mode_buf),
            statbuf.st_nlink, 
            getpwuid(statbuf.st_uid)->pw_name, //获取用户名
            getgrgid(statbuf.st_gid)->gr_name, 
            statbuf.st_size,
            time_to_str(&statbuf.st_mtime, time_buf),
            dirent->d_name);
    }
    closedir(dp);
    return 0;
}