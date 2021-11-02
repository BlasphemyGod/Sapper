#include <stdlib.h>
#include <time.h>
#include "Field.h"

int correct_coord(int row, int col) {
	if (row >= 0 && row < HEIGHT && col >= 0 && col < WIDTH) {
		return 1;
	}
	return 0;
}

int check_field(const Field* field) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
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

int calculate_cells(Field* field) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
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
	if (correct_coord(row, col) && !is_opened(field, row, col)) {
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
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			field->cells[y][x] = 0;
			field->opened[y][x] = 0;
		}
	}
}

void restart_field(Field* field) {
	srand(time(NULL));
	clear_field(field);
	int linear[WIDTH * HEIGHT] = {};
	for (int i = 0; i < field->bombs; i++) {
		linear[i] = -1;
	}
	for (int i = WIDTH * HEIGHT - 1; i > 0; i--) {
		int temp = linear[i];
		int j = rand() % (i + 1);
		linear[i] = linear[j];
		linear[j] = temp;
	}
	for (int i = 0; i < WIDTH * HEIGHT; i++) {
		set_cell(field, i / WIDTH, i % WIDTH, linear[i]);
	}
	calculate_cells(field);
}

Field field(int bombs) {
	return { {}, {}, WIDTH, HEIGHT, bombs };
}