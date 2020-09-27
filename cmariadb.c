#include "cmariadb.h"

void ExitMariaDB()
{
	printf("MariaDB : Bye!\n");
	mysql_close(con);
}

MYSQL_RES* MariaDBQuery(char *query)
{
	/*
		쿼리를 인자로 받아 그 결과값을 리턴하는 함수
		- 정상 실행시 쿼리 결과 리턴 / 에러발생시 1
	*/
	MYSQL_RES *_res; // 쿼리 결과값을 저장하기 위한 변수

	// 쿼리문 실행
	if (mysql_query(con, query)) {
		fprintf(stderr, "%s\n", mysql_error(con));
		return 1;
	}

	// 쿼리 결과를 res에 저장
	_res = mysql_store_result(con);

	return _res; // 결과값 리턴
}

int MariaDBInit()
{
	/*
		MariaDB 초기화 및 접속
		- 정상 실행시 0 / 에러발생시 1
	*/
	con = mysql_init(NULL); // db커넥터 초기화

	if (con == NULL) {
		// db 커넥터 초기화에 실패한 경우
		fprintf(stderr, "%s\n", mysql_error(con));
		return 1;
	}

	// MariaDB 접속
	if (mysql_real_connect(con, HOST, ID, PASSWD, DATABASE, 0, NULL, 0) == NULL) {
		// MariaDB 접속이 안 된 경우 에러구문을 출력후 커넥터 닫기
		fprintf(stderr, "%s\n", mysql_error(con));
		mysql_close(con);
		return 1;
	}

	return 0;
}
