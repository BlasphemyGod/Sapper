#include <stdio.h>
#include <time.h>
#include "framework.h"
#include "Timer.h"
#include "Sapper.h"
#include "RecordsTable.h"
#define _CRT_SECURE_NO_WARNINGS

const HFONT hFont = CreateFont(-20, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const HFONT hLeaderFont = CreateFont(-12, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const HFONT hInsertFont = CreateFont(-12, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* record_label = TEXT("Таблица рекордов");
const TCHAR* return_label = TEXT("Вернуться");
const TCHAR* insert_label = TEXT("Сохранить");
RECT button_rect = { 10, 60, 212, 90 };
RECT return_rect = { 118, 40, 318, 70 };
RECT insert_rect = { 315, 90, 400, 110 };
const HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
const HBRUSH casualButtonBrush = CreateSolidBrush(RGB(255, 255, 255));
const HBRUSH activeButtonBrush = CreateSolidBrush(RGB(222, 222, 222));
static HWND leaders_name_field;

RecordsTable leaders_table = {};
int is_on_records_screen = 0;

HWND get_leaders_name_field() {
	return leaders_name_field;
}

void set_leaders_name_field(HWND hWnd) {
	leaders_name_field = hWnd;
}

int on_records_screen() {
	return is_on_records_screen;
}

void change_records_screen() {
	is_on_records_screen = 1 - is_on_records_screen;
}

int compare_records(Record first, Record second) {
	if (first.win_time < second.win_time)
		return -1;
	if (first.win_time > second.win_time)
		return 1;
	return 0;
}

int add_record(int win_time, TCHAR name[]) {
	Record record = { win_time, {}, time(NULL) };
	lstrcpyn(record.name, name, 21);
	leaders_table.records[leaders_table.last] = record;
	int result = 0;
	for (int i = leaders_table.last; i > 0; i--) {
		if (compare_records(leaders_table.records[i], leaders_table.records[i - 1]) == -1) {
			Record temp = leaders_table.records[i];
			leaders_table.records[i] = leaders_table.records[i - 1];
			leaders_table.records[i - 1] = temp;
			result = 1;
		}
	}
	leaders_table.last = min(leaders_table.last + 1, 11);
	return result;
}

void encode_record(TCHAR str[]) {
	for (int i = 0; str[i] != L'\0'; i++) {
		str[i] += 10;
	}
}

void decode_record(TCHAR str[]) {
	for (int i = 0; str[i] != L'\0'; i++) {
		str[i] -= 10;
	}
}

void save_records() {
	FILE* output_file;
	fopen_s(&output_file, "leaders_table.txt", "wt");
	if (output_file == NULL) {
		fprintf(stderr, "Something gone wrong during saving leaders table");
		return;
	}
	fputwc(L'0', output_file);
	fwprintf(output_file, L"%d\n", leaders_table.last);
	for (int i = 0; i < leaders_table.last; i++) {
		unsigned int date = leaders_table.records[i].date;
		fwprintf(output_file, L"%s %u %u\n", leaders_table.records[i].name, leaders_table.records[i].win_time, date);
	}
	fclose(output_file);
}

void save_records_safe() {
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
		unsigned int date = leaders_table.records[i].date;
		swprintf_s(encoded_record, 60, L"%s %u %u", leaders_table.records[i].name, leaders_table.records[i].win_time, date);
		encode_record(encoded_record);
		fwprintf(output_file, L"%s\n", encoded_record);
	}
	fclose(output_file);
}

void load_records() {
	FILE* input_file;
	fopen_s(&input_file, "leaders_table.txt", "rt");
	if (input_file == NULL) {
		fprintf(stderr, "Something gone wrong during saving leaders table");
		return;
	}
	TCHAR is_encoded_mark = fgetwc(input_file);
	if (is_encoded_mark == L'0') {
		fwscanf_s(input_file, L"%d", &leaders_table.last);
		for (int i = 0; i < leaders_table.last; i++) {
			unsigned int date = 0;
			fwscanf_s(input_file, L"%s%u%u", leaders_table.records[i].name, 21, &leaders_table.records[i].win_time, &date);
			leaders_table.records[i].date = date;
		}
	}
	else if (is_encoded_mark == L'1') {
		fwscanf_s(input_file, L"%d", &leaders_table.last);
		fgetwc(input_file);
		TCHAR buffer[60] = {};
		for (int i = 0; i < leaders_table.last; i++) {
			fgetws(buffer, 60, input_file);
			decode_record(buffer);
			unsigned int date = 0;
			swscanf_s(buffer, L"%s%u%u", leaders_table.records[i].name, 21, &leaders_table.records[i].win_time, &date);
			leaders_table.records[i].date = date;
		}
	}
	fclose(input_file);
}

void draw_leaders_table_button(HWND hWnd, HDC hdc) {
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

void draw_leaders_table_return_button(HWND hWnd, HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hFont);
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&return_rect, mouse)) {
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			SelectObject(hdc, activeButtonBrush);
			RoundRect(hdc, return_rect.left, return_rect.top, return_rect.right, return_rect.bottom, 12, 12);
		}
		else {
			SelectObject(hdc, casualButtonBrush);
			RoundRect(hdc, return_rect.left, return_rect.top, return_rect.right, return_rect.bottom, 12, 12);
		}
	}
	else {
		SelectObject(hdc, casualButtonBrush);
		RoundRect(hdc, return_rect.left, return_rect.top, return_rect.right, return_rect.bottom, 12, 12);
	}
	DrawText(hdc, return_label, lstrlen(return_label), &return_rect, DT_CENTER);
}

void draw_leaders_table_insert_button(HWND hWnd, HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	SelectObject(hdc, hInsertFont);
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&insert_rect, mouse)) {
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			SelectObject(hdc, activeButtonBrush);
			RoundRect(hdc, insert_rect.left, insert_rect.top, insert_rect.right, insert_rect.bottom, 12, 12);
		}
		else {
			SelectObject(hdc, casualButtonBrush);
			RoundRect(hdc, insert_rect.left, insert_rect.top, insert_rect.right, insert_rect.bottom, 12, 12);
		}
	}
	else {
		SelectObject(hdc, casualButtonBrush);
		RoundRect(hdc, insert_rect.left, insert_rect.top, insert_rect.right, insert_rect.bottom, 12, 12);
	}
	DrawText(hdc, insert_label, lstrlen(insert_label), &insert_rect, DT_CENTER);
}

void draw_leaders_table(HDC hdc) {
	TCHAR buffer[100];
	RECT text_rect = { 5, 120, 416, 140 };
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hPen);
	SelectObject(hdc, hLeaderFont);
	for (int i = 0; i < leaders_table.last; i++) {
		swprintf(buffer, 100, L"%2d место | %3d секунд | %21s | %21s", i + 1, leaders_table.records[i].win_time, leaders_table.records[i].name, _wctime64(&leaders_table.records[i].date));
		DrawText(hdc, buffer, lstrlen(buffer), &text_rect, DT_CENTER);
		OffsetRect(&text_rect, 0, 30);
	}
}

void on_leaders_table_click(HWND hWnd) {
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&button_rect, mouse)) {
		switch_on_leaders_table();
	}
}

void on_leaders_table_return_click(HWND hWnd) {
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&return_rect, mouse)) {
		switch_off_leaders_table();
	}
}

void on_leaders_table_insert_click(HWND hWnd) {
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&return_rect, mouse)) {
		TCHAR name_buffer[21];
		GetWindowText(leaders_name_field, name_buffer, 21);
		add_record(get_timer(), name_buffer);
		restart_game();
	}
}