#include "Timer.h"
#include "framework.h"
#include <time.h>


int start_time = 0;
int end_time = 0;
int is_started = 0;
const HFONT hFont = CreateFont(-32, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* label = TEXT("Время:");
RECT label_rect = { 10, 10, 110, 66 };
TCHAR timer_value[20];
RECT timer_rect = { 110, 10, 212, 66 };

int get_timer() {
    if (!is_started) return end_time - start_time;
    return time(NULL) - start_time;
}

void set_timer(int seconds) {
    start_time = time(NULL) - seconds;
}

void start_timer() {
    start_time = time(NULL);
    is_started = 1;
}

void unpause_timer() {
    set_timer(get_timer());
    is_started = 1;
}

void stop_timer() {
    end_time = time(NULL);
    is_started = 0;
}

void draw_timer(HDC hdc) {
    _stprintf_s(timer_value, TEXT("%04d"), get_timer());
    SetTextColor(hdc, RGB(255, 255, 255));
    SetBkMode(hdc, TRANSPARENT);
    SelectObject(hdc, hFont);
    DrawText(hdc, label, lstrlen(label), &label_rect, DT_CENTER);
    DrawText(hdc, timer_value, lstrlen(timer_value), &timer_rect, DT_CENTER);
}