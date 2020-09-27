#include "cmariadb.h"
#include "cuart.h"

// TYPEDEFINITION AREA
typedef struct _SENSORDATA {
	int temp;
	int humi;
	int water;
	int food;
} SENSORDATA;

// GLOBAL VARIABLE AREA
SENSORDATA s_data = { 0 };
short feed = 0;

// FUNCTION AREA
int ReadToAVR();
int PushDataToDB();
int ReadToMariaDB();
int CaseFeed1();
