#pragma once
#include "framework.h"
#define CELL_WIDTH 30
#define CELL_HEIGHT 30

void draw_even_background(HDC hdc, int x, int y);

void draw_odd_background(HDC hdc, int x, int y);

void draw_number(HDC hdc, int x, int y, int number);

void draw_bomb(HDC hdc, int x, int y);

void draw_even_curtain(HDC hdc, int x, int y);

void draw_odd_curtain(HDC hdc, int x, int y);

void draw_active_curtain(HDC hdc, int x, int y);

void draw_flag(HDC hdc, int x, int y);

void draw_even_cell(HDC hdc, int x, int y, int value, int opened, int flagged);

void draw_odd_cell(HDC hdc, int x, int y, int value, int opened, int flagged);

void draw_active_cell(HDC hdc, int x, int y, int flagged);