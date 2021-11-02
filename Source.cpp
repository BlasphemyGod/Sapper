#include <stdlib.h>
#define WIDTH 14
#define HEIGHT 14

struct Field {
	int cells[HEIGHT][WIDTH];
	int opened[HEIGHT][WIDTH];
	int width;
	int height;
	int bombs;
};

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

int put_bomb(Field* field, int row, int col) {
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