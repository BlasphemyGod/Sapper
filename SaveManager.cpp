#include <stdio.h>
#include "Sapper.h"
#include "Field.h"
#include "Timer.h"

const HFONT hFont = CreateFont(-32, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* save_label = TEXT("Сохранить");
const TCHAR* load_label = TEXT("Загрузить");
RECT save_rect = {262, 10, 462, 66};
RECT load_rect = {262, 86, 462, 142};
const HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
const HBRUSH casualButtonBrush = CreateSolidBrush(RGB(255, 255, 255));
const HBRUSH activeButtonBrush = CreateSolidBrush(RGB(222, 222, 222));

void draw_save_load_buttons(HWND hWnd, HDC hdc) {
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	SelectObject(hdc, hPen);
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(hWnd, &mouse);
	if (PtInRect(&save_rect, mouse)) {
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			SelectObject(hdc, activeButtonBrush);
			RoundRect(hdc, save_rect.left, save_rect.top, save_rect.bottom, save_rect.right, 12, 12);
		}
	}
	DrawText(hdc, save_label, lstrlen(save_label), &save_rect, DT_CENTER);
	if (PtInRect(&load_rect, mouse)) {
		if (GetKeyState(VK_LBUTTON) & 0x8000) {
			SelectObject(hdc, activeButtonBrush);
			RoundRect(hdc, load_rect.left, load_rect.top, load_rect.bottom, load_rect.right, 12, 12);
		}
	}
	DrawText(hdc, load_label, lstrlen(load_label), &load_rect, DT_CENTER);
}

int save_game(Field* field) {
	FILE* file;
	fopen_s(&file, "save.txt", "w");
	if (file == NULL) {
		return 0;
	}
	fprintf_s(file, "%d\n", get_timer());
	fprintf_s(file, "%d %d %d %d %d %d\n", field->width, field->height, field->x, field->y, field->bombs, field->is_first_cell);
	for (int row = 0; row < field->height; row++) {
		for (int col = 0; col < field->width; col++) {
			fprintf_s(file, "%d %d %d ", get_cell(field, row, col), is_opened(field, row, col), is_flag(field, row, col));
		}
		fprintf_s(file, "\n");
	}
	return 1;
}

int load_game(Field* field) {
	FILE* file;
	fopen_s(&file, "save.txt", "r");
	if (file == NULL) {
		return 0;
	}
	int time;
	fscanf_s(file, "%d", &time);
	fscanf_s(file, "%d %d %d %d %d %d\n", &field->width, &field->height, &field->x, &field->y, &field->bombs, &field->is_first_cell);
	for (int row = 0; row < field->height; row++) {
		for (int col = 0; col < field->width; col++) {
			fscanf_s(file, "%d %d %d", &field->cells[row][col], &field->opened[row][col], &field->flags[row][col]);
		}
	}
	start_timer();
	set_timer(time);
	return 1;
}