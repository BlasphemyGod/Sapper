#pragma once

void draw_save_load_buttons(HWND hWnd, HDC hdc);

void on_save_load_click(HWND hWnd, Field* field);

int save_game(Field* field);

int load_game(Field* field);
