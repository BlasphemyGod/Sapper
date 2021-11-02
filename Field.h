#pragma once
#include "framework.h"
#define FIELD_WIDTH 14
#define FIELD_HEIGHT 14


typedef struct {
	int cells[FIELD_HEIGHT][FIELD_WIDTH];
	int opened[FIELD_HEIGHT][FIELD_WIDTH];
	int flags[FIELD_HEIGHT][FIELD_WIDTH];
	int x;
	int y;
	int width;
	int height;
	int bombs;
} Field;

int correct_coord(int row, int col);

int check_field(const Field* field);

int set_bomb(Field* field, int row, int col);

int is_bomb(const Field* field, int row, int col);

int is_opened(const Field* field, int row, int col);

int is_flag(const Field* field, int row, int col);

void swap_flag(Field* field, int row, int col);

int get_cell(const Field* field, int row, int col);

int* get_cell_ptr(Field* field, int row, int col);

int set_cell(Field* field, int row, int col, int value);

void calculate_cells(Field* field);

void open_cell(Field* field, int row, int col);

void close_cell(Field* field, int row, int col);

void clear_field(Field* field);

void restart_field(Field* field);

Field new_field(int x, int y, int bombs);

void draw_field(HDC hdc, Field* field);

void mouse_button_click(Field* field, int mouse_button, int x, int y);
