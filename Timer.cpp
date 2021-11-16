#include "Timer.h"
#include "framework.h"
#include <time.h>


int start_time = time(NULL);
int is_started = 0;
const HFONT hFont = CreateFont(-32, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* label = TEXT("Время:");
RECT label_rect = { 0, 0, 100, 56 };
TCHAR timer_value[20];
RECT timer_rect = { 0, 0, 112, 56 };

int get_timer() {
	if (!is_started) return 0;
	return time(NULL) - start_time;
}

void set_timer(int seconds) {
	start_time = time(NULL) - seconds;
}

void start_timer() {
	start_time = time(NULL);
	is_started = 1;
}

void stop_timer() {
	is_started = 0;
}

void draw_timer(HDC hdc, int x, int y) {
	OffsetRect(&label_rect, x, y);
	OffsetRect(&timer_rect, x + 100, y);
	_stprintf_s(timer_value, TEXT("%04d"), get_timer());
	SetTextColor(hdc, RGB(255, 255, 255));
	SetBkMode(hdc, TRANSPARENT);
	SelectObject(hdc, hFont);
	DrawText(hdc, label, lstrlen(label), &label_rect, DT_CENTER);
	DrawText(hdc, timer_value, lstrlen(timer_value), &timer_rect, DT_CENTER);
	OffsetRect(&label_rect, -x, -y);
	OffsetRect(&timer_rect, -x - 100, -y);
}