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

int get_cell(Field* field, int row, int col) {
	if (correct_coord(row, col)) {

	}
}

int calculate_cells(Field* field) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			field->cells[y][x] = field->cells[]
		}
	}
}