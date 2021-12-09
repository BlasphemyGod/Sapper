#include <stdio.h>
#include "Sapper.h"
#include "Field.h"
#include "Timer.h"
#include "SaveManager.h"

const HFONT hFont = CreateFont(-20, 0, 0, 0, 0, ANSI_CHARSET, 0, 0, 0, 0, 0, 0, 0, TEXT("Arial"));
const TCHAR* save_label = TEXT("Сохранить");
const TCHAR* load_label = TEXT("Загрузить");
RECT save_rect = {232, 10, 400, 36};
RECT load_rect = {232, 46, 400, 72};
const HPEN hPen = CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
const HBRUSH casualButtonBrush = CreateSolidBrush(RGB(255, 255, 255));
const HBRUSH activeButtonBrush = CreateSolidBrush(RGB(222, 222, 222));

void draw_save_load_buttons(HWND hWnd, HDC hdc) {
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(0, 0, 0));
    SelectObject(hdc, hPen);
    SelectObject(hdc, hFont);
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(hWnd, &mouse);
    if (PtInRect(&save_rect, mouse)) {
        if (GetKeyState(VK_LBUTTON) & 0x8000) {
            SelectObject(hdc, activeButtonBrush);
            RoundRect(hdc, save_rect.left, save_rect.top, save_rect.right, save_rect.bottom, 12, 12);
        }
        else {
            SelectObject(hdc, casualButtonBrush);
            RoundRect(hdc, save_rect.left, save_rect.top, save_rect.right, save_rect.bottom, 12, 12);
        }
    }
    else {
        SelectObject(hdc, casualButtonBrush);
        RoundRect(hdc, save_rect.left, save_rect.top, save_rect.right, save_rect.bottom, 12, 12);
    }
    DrawText(hdc, save_label, lstrlen(save_label), &save_rect, DT_CENTER);
    if (PtInRect(&load_rect, mouse)) {
        if (GetKeyState(VK_LBUTTON) & 0x8000) {
            SelectObject(hdc, activeButtonBrush);
            RoundRect(hdc, load_rect.left, load_rect.top, load_rect.right, load_rect.bottom, 12, 12);
        }
        else {
            SelectObject(hdc, casualButtonBrush);
            RoundRect(hdc, load_rect.left, load_rect.top, load_rect.right, load_rect.bottom, 12, 12);
        }
    }
    else {
        SelectObject(hdc, casualButtonBrush);
        RoundRect(hdc, load_rect.left, load_rect.top, load_rect.right, load_rect.bottom, 12, 12);
    }
    DrawText(hdc, load_label, lstrlen(load_label), &load_rect, DT_CENTER);
}

void on_save_load_click(HWND hWnd, Field* field) {
    POINT mouse;
    GetCursorPos(&mouse);
    ScreenToClient(hWnd, &mouse);
    if (PtInRect(&save_rect, mouse)) {
        int result = save_game(field);
        if (result == 0) {
            MessageBox(hWnd, TEXT("Failed to save game"), TEXT("Fail"), MB_OK);
        }
    }
    if (PtInRect(&load_rect, mouse)) {
        int result = load_game(field);
        if (result == 0) {
            MessageBox(hWnd, TEXT("Failed to load game"), TEXT("Fail"), MB_OK);
        }
    }
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
    fclose(file);
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
    fclose(file);
    start_timer();
    set_timer(time);
    return 1;
}