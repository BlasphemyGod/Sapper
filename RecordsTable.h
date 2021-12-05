#pragma once
#include "framework.h"

typedef struct {
	int win_time;
	TCHAR name[21];
	int date;
} Record;


typedef struct {
	Record records[11];
	int last_record;
} RecordsTable;

