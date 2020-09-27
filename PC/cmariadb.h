#include <stdio.h>
#include <mysql.h>
// You Need Header Setting and Linker Setting on your SDK

#define HOST "localhost"
#define ID "root"
#define PASSWD "1234"
#define DATABASE "geckohouse"

MYSQL *con;

int MariaDBInit();
MYSQL_RES* MariaDBQuery(char *query);
void ExitMariaDB();
