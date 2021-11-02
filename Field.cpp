#include <stdlib.h>
#include <time.h>
#include "Field.h"
#include "Cell.h"

int correct_coord(int row, int col) {
	if (row >= 0 && row < FIELD_HEIGHT && col >= 0 && col < FIELD_WIDTH) {
		return 1;
	}
	return 0;
}

int check_field(const Field* field) {
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			if (field->opened[y][x] && field->cells[y][x] == -1) {
				return -1;
			}
			else if (!field->opened[y][x] && field->cells[y][x] != -1) {
				return 0;
			}
		}
	}
	return 1;
}

int set_bomb(Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		field->cells[row][col] = -1;
		return 0;
	}
	return -1;
}

int is_bomb(const Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		return field->cells[row][col] == -1;
	}
	return 0;
}

int is_opened(const Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		return field->opened[row][col];
	}
	return 0;
}

int is_flag(const Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		if (field->flags[row][col]) {
			return 1;
		}
	}
	return 0;
}

void swap_flag(Field* field, int row, int col) {
	if (correct_coord(row, col) && !is_opened(field, row, col)) {
		field->flags[row][col] = 1 - field->flags[row][col];
	}
}

int get_cell(const Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		return field->cells[row][col];
	}
	return -2;
}

int* get_cell_ptr(Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		return &field->cells[row][col];
	}
	return NULL;
}

int set_cell(Field* field, int row, int col, int value) {
	if (correct_coord(row, col)) {
		field->cells[row][col] = value;
		return 0;
	}
	return -1;
}

void calculate_cells(Field* field) {
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			if (get_cell(field, y, x) != -1) {
				set_cell(
					field, y, x,
					is_bomb(field, y - 1, x - 1) + is_bomb(field, y - 1, x) + is_bomb(field, y - 1, x + 1) +
					is_bomb(field, y, x + 1) + is_bomb(field, y + 1, x + 1) + is_bomb(field, y + 1, x) +
					is_bomb(field, y + 1, x - 1) + is_bomb(field, y, x - 1)
				);
			}
		}
	}
}

void open_cell(Field* field, int row, int col) {
	if (correct_coord(row, col) && !is_opened(field, row, col) && !is_flag(field, row, col)) {
		int cell = get_cell(field, row, col);
		field->opened[row][col] = 1;
		if (cell == 0) {
			open_cell(field, row - 1, col - 1);
			open_cell(field, row - 1, col);
			open_cell(field, row - 1, col + 1);
			open_cell(field, row, col + 1);
			open_cell(field, row + 1, col + 1);
			open_cell(field, row + 1, col);
			open_cell(field, row + 1, col - 1);
			open_cell(field, row, col - 1);
		}
	}
}

void close_cell(Field* field, int row, int col) {
	if (correct_coord(row, col)) {
		field->opened[row][col] = 0;
	}
}

void clear_field(Field* field) {
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			field->cells[y][x] = 0;
			field->opened[y][x] = 0;
			field->flags[y][x] = 0;
		}
	}
}

void restart_field(Field* field) {
	srand(time(NULL));
	clear_field(field);
	int linear[FIELD_WIDTH * FIELD_HEIGHT] = {};
	for (int i = 0; i < field->bombs; i++) {
		linear[i] = -1;
	}
	for (int i = FIELD_WIDTH * FIELD_HEIGHT - 1; i > 0; i--) {
		int temp = linear[i];
		int j = rand() % (i + 1);
		linear[i] = linear[j];
		linear[j] = temp;
	}
	for (int i = 0; i < FIELD_WIDTH * FIELD_HEIGHT; i++) {
		set_cell(field, i / FIELD_WIDTH, i % FIELD_WIDTH, linear[i]);
	}
	calculate_cells(field);
}

Field new_field(int x, int y, int bombs) {
	return { {}, {}, {}, x, y, FIELD_WIDTH, FIELD_HEIGHT, bombs };
}

void draw_field(HDC hdc, Field* field) {
	POINT cursor;
	GetCursorPos(&cursor);
	cursor.x -= field->x;
	cursor.y -= field->y;
	for (int x = 0; x < FIELD_WIDTH; x++) {
		for (int y = 0; y < FIELD_HEIGHT; y++) {
			if (!is_opened(field, y, x)) {
				RECT cell = { x * CELL_WIDTH, y * CELL_HEIGHT, x * CELL_WIDTH + CELL_WIDTH, y * CELL_HEIGHT + CELL_HEIGHT };
				if (PtInRect(&cell, cursor)) {
					draw_active_cell(hdc, field->x + x * CELL_WIDTH, field->y + y * CELL_HEIGHT, is_flag(field, y, x));
					continue;
				}
			}
			if ((x + y) % 2 == 0) {
				draw_even_cell(hdc, field->x + x * CELL_WIDTH, field->y + y * CELL_HEIGHT, get_cell(field, y, x), is_opened(field, y, x), is_flag(field, y, x));
			}
			else {
				draw_odd_cell(hdc, field->x + x * CELL_WIDTH, field->y + y * CELL_HEIGHT, get_cell(field, y, x), is_opened(field, y, x), is_flag(field, y, x));
			}
		}
	}
}

void mouse_button_click(Field* field, int mouse_button, int x, int y) {
	x -= field->x;
	y -= field->y;
	if (mouse_button == 1) {
		for (int col = 0; col < FIELD_WIDTH; col++) {
			for (int row = 0; row < FIELD_HEIGHT; row++) {
				RECT cell = { col * CELL_WIDTH, row * CELL_HEIGHT, col * CELL_WIDTH + CELL_WIDTH, row * CELL_HEIGHT + CELL_HEIGHT };
				if (PtInRect(&cell, { x, y })) {
					open_cell(field, row, col);
				}
			}
		}
	}
	else if (mouse_button == 2) {
		for (int col = 0; col < FIELD_WIDTH; col++) {
			for (int row = 0; row < FIELD_HEIGHT; row++) {
				RECT cell = { col * CELL_WIDTH, row * CELL_HEIGHT, col * CELL_WIDTH + CELL_WIDTH, row * CELL_HEIGHT + CELL_HEIGHT };
				if (PtInRect(&cell, { x, y })) {
					swap_flag(field, row, col);
				}
			}
		}
	}
}