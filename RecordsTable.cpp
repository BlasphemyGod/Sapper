#include <stdio.h>
#include <time.h>
#include "framework.h"
#include "RecordsTable.h"

const HFONT hFont = CreateFont(-20, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* record_label = TEXT("Таблица рекордов");
RECT button_rect = { 10, 90, 212, 116 };
const HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
const HBRUSH casualButtonBrush = CreateSolidBrush(RGB(255, 255, 255));
const HBRUSH activeButtonBrush = CreateSolidBrush(RGB(222, 222, 222));

RecordsTable records_table = {};
int is_on_records_screen = 0;

int onRecordsScreen() {
	return is_on_records_screen;
}

void changeRecordsScreen() {
	is_on_records_screen = 1 - is_on_records_screen;
}

int compareRecords(Record first, Record second) {
	if (first.win_time < second.win_time)
		return -1;
	if (first.win_time > second.win_time)
		return 1;
	return 0;
}

void addRecord(int win_time, TCHAR name[]) {
	Record record = { win_time, {}, time(NULL) };
	lstrcpyn(record.name, name, 21);
	records_table.records[records_table.last_record] = record;
	for (int i = records_table.last_record; i > 0; i--) {
		if (compareRecords(records_table.records[i], records_table.records[i - 1]) == -1) {
			Record temp = records_table.records[i];
			records_table.records[i] = records_table.records[i - 1];
			records_table.records[i - 1] = temp;
		}
	}
	records_table.last_record = min(records_table.last_record + 1, 11);
}