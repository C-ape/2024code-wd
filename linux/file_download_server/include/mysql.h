#include "func.h"


MYSQL* conn; //全局的数据库连接

void get_salt(char *str);
int mysqlInit(MYSQL **conn);
int findName(const char* name);
int findSaltByName(const char* name, char* salt);
int addUser(loginMsg_t login);
int addRootDir(loginMsg_t login);
int findCryptByName(char* name,  char* token);
int findStrArgByName(char* name, char* str, const char* arg);
int findIdArgByName(char* name, int* id, const char* arg);
int findRootIdByNameFromVir(char* filename, int* id, const char* arg, int pre_id);
int insertDirVir(char* dirname, int pre_id, int owner_id);