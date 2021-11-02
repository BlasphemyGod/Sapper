#include <stdlib.h>
#define WIDTH 14
#define HEIGHT 14

struct Field {
	int actual[HEIGHT][WIDTH];
	int opened[HEIGHT][WIDTH];
};

int check_field(Field* field) {
	for (int x = 0; x < WIDTH; x++) {
		for (int y = 0; y < HEIGHT; y++) {
			if (field->opened[y][x] && field->actual[y][x] == -1) {
				return -1;
			}
			else if (!field->opened[y][x] && field->actual[y][x] != -1) {
				return 0;
			}
		}
	}
	return 1;
}