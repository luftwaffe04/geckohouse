#include "main.h"

int main()
{
	unsigned long times = 1;
	// Initializing System
	if (UART_Init()) {
		printf("UART Initializing Error\n");
		return 0;
	}
	if (MariaDBInit()) {
		return 0;
	}

	// Loop
	while (1) {
		printf("<%ldth Process>\n", times++);

		if (ReadToAVR()) { break; }
		if (PushDataToDB()) { break; }
		if (ReadToMariaDB()) { break; }
		if (feed) {
			if (CaseFeed1()) { break; }
		}
		printf("Waiting 10 sec...\n\n\n");
		Sleep(10000); // 10초간 대기
	}

	// Quit Sequence
	ExitMariaDB();
	QuitUART();

	return 0;
}



int CaseFeed1()
{
	/*
		ReadToMariaDB에서 feed=1인 경우 AVR에 doservo라는 문자열을 전송하는 함수
		- 전송 후 DB의 feed값을 0으로 변경
	*/
	int endflag = 0;
	char data[100] = { 0, }; // 쿼리를 저장할 문자열 변수

	if (WriteUART("doservo\n")) {
		return 1;
	}

	// feed->0 Sequence
	feed = 0;
	// Query 수행
	sprintf_s(data, sizeof(data), "update sensordata set feed=0;");
	endflag = (int)MariaDBQuery(data);
	printf("Query : %s\n", data);

	return endflag;
}

int ReadToMariaDB()
{
	/*
		DB에서 feed를 select 하는 함수
		- select feed from sensordata;
		- 정상실행시 0 / 에러발생시 1
	*/
	char data[100] = { 0, }; // 쿼리를 저장할 문자열 변수
	MYSQL_RES *res; // 쿼리 결과를 저장할 변수
	MYSQL_ROW row; 	// 데이터 값을 저장할 변수

	// select 쿼리문 설정
	sprintf_s(data, sizeof(data), "select feed from sensordata;");

	// 쿼리문 실행
	res = MariaDBQuery(data);

	// 쿼리에 문제가 있을 시
	if (res == 1) { return 1; }

	// 쿼리 정상 실행 시
	while ((row = mysql_fetch_row(res)) != NULL) {
		printf("feed->%s\n", row[0]);
		feed = atoi(row[0]);
	}

	return 0;
}

int PushDataToDB()
{
	/*
		읽어온 데이터를 DB로 update 하는 함수
		- update sensordata set temperature=%d, humidity=%d, water=%d, food=%d;
		- 정상실행시 0 / 에러발생시 1
	*/
	int endflag = 0;
	char data[100] = { 0, }; // 쿼리를 저장할 문자열 변수

	// update 쿼리문 설정
	sprintf_s(data, sizeof(data), "update sensordata set temperature=%d, humidity=%d, water=%d, food=%d;", s_data.temp, s_data.humi, s_data.water, s_data.food);

	// 쿼리문 실행
	endflag = (int)MariaDBQuery(data);

	if (endflag) { return 1; }

	printf("Query : %s\n", data);

	return endflag;
}

int ReadToAVR()
{
	/*
		AVR로부터 데이터를 읽어오는 함수
		- "\ntemp:%04d_humi:%04d_water:%04d_food:%04d\n" 형태의 데이터를 쪼개서 각각 int형으로 변환 후, sensordata 구조체에 넣는다
		- 정상실행시 0 / 에러발생시 1
	*/
	char readdata[50];
	char *tokData[4]; //
	char *context; // strtok_s에서 다음 문자열의 주소를 저장할 변수

	if (WriteUART("senddata\n")) { return 1; }
	Sleep(1000);

	// UART에서 데이터 수신
	memset(readdata, 0, sizeof(readdata));
	if (ReadUART(readdata)) { return 1; }

	// 수신한 데이터를 "_"을 기준으로 토큰화
	tokData[0] = strtok_s(readdata, "_", &context) + 5;
	tokData[1] = strtok_s(NULL, "_", &context) + 5;
	tokData[2] = strtok_s(NULL, "_", &context) + 6;
	tokData[3] = strtok_s(NULL, "_", &context) + 5;

	// 토큰화한 데이터를 정수화
	s_data.temp = atoi(tokData[0]);
	s_data.humi = atoi(tokData[1]);
	s_data.water = atoi(tokData[2]);
	s_data.food = atoi(tokData[3]);

	// 데이터 출력
	printf("AVR Data : temp->%d humi->%d water->%d food->%d\n", s_data.temp, s_data.humi, s_data.water, s_data.food);

	return 0;
}
