#include "cuart.h"

void QuitUART()
{
	if (CloseHandle(hSerial) == 0) {
		fprintf(stderr, "Error : Close Handle\n");
	}
}

int ReadUART(char *_string) {
	/*
		C <- UART : UART를 통해 데이터를 읽어오는 함수
		- 인터럽트나 쓰레드를 통해 데이터를 지속적으로 읽어오는 것이 아니기 때문에 시간차에 의한 데이터 깨짐이 발생.
		- 따라서 광범위하게 데이터를 읽어들여 그 안에서 유효한 데이터를 찾아 저장.
	*/
	char data[MAX_BUFFER] = { 0, };
	DWORD bytes_read, event_Mask;

	// UART 입력 이벤트가 발생하면 실행
	if (WaitCommEvent(hSerial, &event_Mask, NULL)) {
		// 입력 이벤트
		ReadFile(hSerial, data, sizeof(data), &bytes_read, NULL); // Read Comport Data
		if (bytes_read) {
			// Cut Real Data
			printf("ReadData : %s\n", data);
			DataVarification(data, MAX_BUFFER, _string);
		}
		else {
			fprintf(stderr, "Reading Error\n");
			return 1;
		}
	}
	else {
		fprintf(stderr, "Error waiting for comm event\n");
	}

	return 0;
}

void DataVarification(char *_data, int _data_length, char *_string) {
	/*
		UART로 Read한 데이터에서 유효한 데이터를 추출하는 함수
		- 유효한 데이터의 길이를 매크로 CORRECT로 선언하고 CORRECT만큼 문자를 읽으면 유효한 데이터로 간주
		- 유효한 데이터는 "\n(CORRECT의 길이만큼 입력된 문자열)\n"의 형태를 가지고 있음
	*/
	int start = 0; //
	int cnt = 0;
	int flag = 0;
	int ptr = 0;

	while (start < _data_length) { // 0부터 마지막 까지 데이터를 읽음
		int i = start;
		do { // i부터 마지막까지 데이터를 읽음
			if (_data[i] == '\n') { // 시작지점의 값이 \n이 아니면 탈출
				break;
			}
			else { // 유효한 문자인지 판단
				if (_data[i] >= '0' && _data[i] <= 'z') {
					cnt++;
				}
				else {
					break;
				}
			}
			i++;
		} while (i < _data_length);
		if (cnt == CORRECT) { // 유효한 문자가 CORRECT와 같으면 루프 탈출
			break;
		}

		start++; // 유효한 문자가 아니면 탈출
		cnt = 0;
	}

	// 유효한 문자의 시작값부터 CORRECT까지 문자열을 복사
	for (int i = start; i < start + CORRECT; i++) {
		_string[ptr] = _data[i];
		ptr++;
	}

	return;
}

int WriteUART(char *_string) {
	/*
		C -> UART : UART를 통해 데이터를 전달하는 함수
		전송할 문자열은 SEND_STRING에 선언되어있음
	*/
	DWORD bytes_written = 0;

	// UART에 데이터를 전송
	if (!WriteFile(hSerial, _string, strlen(_string), &bytes_written, NULL)) {
		fprintf(stderr, "Writing Error\n");
		return 1;
	}

	// 성공시 몇 바이트를 전송했는지 print
	printf("%d bytes written\n", bytes_written);
	return 0;
}

int UART_Init() {
	/*
		UART 통신을 초기화하는 함수
		main.c 에서 hSerial을 선언해야 한다.
	*/
	DCB dcbSerialParams = { 0 };
	COMMTIMEOUTS timeouts = { 0 };

	// Serial Port를 Open
	printf("Opening serial port...\n");
	hSerial = CreateFile("\\\\.\\COM3", // COM3 포트
		GENERIC_READ | GENERIC_WRITE, // 읽기 | 쓰기 가능
		0, NULL, // 다른 프로세스의 접근 허용안함 / 보안속성 구조체 사용안함
		OPEN_EXISTING, // 파일이 존재할 경우에만 open
		0, NULL); // 생성될 파일의 속성 지정안함 / 템플릿파일 핸들러 사용 안함

	if (hSerial == INVALID_HANDLE_VALUE) {
		fprintf(stderr, "Error\n");
		return 1;
	}

	// Open할 장치를 설정
	memset(&dcbSerialParams, 0, sizeof(dcbSerialParams));
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
	if (GetCommState(hSerial, &dcbSerialParams) == 0) {
		fprintf(stderr, "Error getting device state\n");
		CloseHandle(hSerial);
		return 1;
	}

	dcbSerialParams.BaudRate = CBR_9600; // buadrate 9600
	dcbSerialParams.ByteSize = 8; // 8비트씩 읽기
	dcbSerialParams.StopBits = ONESTOPBIT; // 스탑비트 1개
	dcbSerialParams.Parity = NOPARITY; // 패리티비트 사용안함

	if (SetCommState(hSerial, &dcbSerialParams) == 0) {
		fprintf(stderr, "Error setting device parameters\n");
		CloseHandle(hSerial);
		return 1;
	}

	// 타임아웃 설정
	timeouts.ReadIntervalTimeout = 1;
	timeouts.ReadTotalTimeoutConstant = 1;
	timeouts.ReadTotalTimeoutMultiplier = 1;
	timeouts.WriteTotalTimeoutConstant = 1;
	timeouts.WriteTotalTimeoutMultiplier = 1;

	if (SetCommTimeouts(hSerial, &timeouts) == 0) {
		fprintf(stderr, "Error setting timeouts\n");
		CloseHandle(hSerial);
		return 1;
	}

	// 이벤트 마스크 설정
	if (SetCommMask(hSerial, EV_RXCHAR) == 0)
	{
		fprintf(stderr, "Error setting comm mask\n");
		CloseHandle(hSerial);
		return 1;
	}

	return 0;
}
