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

RecordsTable leaders_table = {};
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

int addRecord(int win_time, TCHAR name[]) {
	Record record = { win_time, {}, time(NULL) };
	lstrcpyn(record.name, name, 21);
	leaders_table.records[leaders_table.last] = record;
	int result = 0;
	for (int i = leaders_table.last; i > 0; i--) {
		if (compareRecords(leaders_table.records[i], leaders_table.records[i - 1]) == -1) {
			Record temp = leaders_table.records[i];
			leaders_table.records[i] = leaders_table.records[i - 1];
			leaders_table.records[i - 1] = temp;
			result = 1;
		}
	}
	leaders_table.last = min(leaders_table.last + 1, 11);
	return result;
}

void encodeRecord(TCHAR str[]) {
	for (int i = 0; str[i] != L'\0'; i++) {
		str[i] += 10;
	}
}

void decodeRecord(TCHAR str[]) {
	for (int i = 0; str[i] != L'\0'; i++) {
		str[i] -= 10;
	}
}

void saveRecords() {
	FILE* output_file;
	fopen_s(&output_file, "leaders_table.txt", "wt");
	if (output_file == NULL) {
		fprintf(stderr, "Something gone wrong during saving leaders table");
		return;
	}
	fputwc(L'0', output_file);
	fprintf(output_file, "%d\n", leaders_table.last);
	for (int i = 0; i < leaders_table.last; i++) {
		fwprintf(output_file, L"%s %d %d\n", leaders_table.records[i].name, leaders_table.records[i].win_time, leaders_table.records[i].date);
	}
}

void saveRecordsSafe() {
	FILE* output_file;
	fopen_s(&output_file, "leaders_table.txt", "wt");
	if (output_file == NULL) {
		fprintf(stderr, "Something gone wrong during saving leaders table");
		return;
	}
	fputwc(L'1', output_file);
	fwprintf(output_file, L"%d\n", leaders_table.last);
	TCHAR encoded_record[60] = {};
	for (int i = 0; i < leaders_table.last; i++) {
		swprintf_s(encoded_record, 60, L"%s %d %d", leaders_table.records[i].name, leaders_table.records[i].win_time, leaders_table.records[i].date);
		encodeRecord(encoded_record);
		fwprintf(output_file, L"%s\n", encoded_record);
	}
}

void loadRecords() {
	FILE* input_file;
	fopen_s(&input_file, "leaders_table.txt", "wt");
	if (input_file == NULL) {
		fprintf(stderr, "Something gone wrong during saving leaders table");
		return;
	}
	TCHAR is_encoded_mark = fgetwc(input_file);
	if (is_encoded_mark == '0') {
		fwscanf(input_file, L"%d", &leaders_table.last);
		for (int i = 0; i < leaders_table.last; i++) {
			fwscanf(input_file, L"%s %d %d", &leaders_table.records[i].name, &leaders_table.records[i].win_time, &leaders_table.records[i].date);
		}
	}
	else if (is_encoded_mark == '1') {
		fwscanf(input_file, L"%d", &leaders_table.last);
		TCHAR buffer[60] = {};
		for (int i = 0; i < leaders_table.last; i++) {
			fgetws(buffer, 60, input_file);
			decodeRecord(buffer);
			swscanf(buffer, L"%s %d %d", &leaders_table.records[i].name, &leaders_table.records[i].win_time, &leaders_table.records[i].date);
		}
	}
}

void drawLeadersTableButton(HWND hWnd, HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hFont);
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&button_rect, mouse)) {
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			SelectObject(hdc, activeButtonBrush);
			RoundRect(hdc, button_rect.left, button_rect.top, button_rect.right, button_rect.bottom, 12, 12);
		}
		else {
			SelectObject(hdc, casualButtonBrush);
			RoundRect(hdc, button_rect.left, button_rect.top, button_rect.right, button_rect.bottom, 12, 12);
		}
	}
	else {
		SelectObject(hdc, casualButtonBrush);
		RoundRect(hdc, button_rect.left, button_rect.top, button_rect.right, button_rect.bottom, 12, 12);
	}
	DrawText(hdc, record_label, lstrlen(record_label), &button_rect, DT_CENTER);
}