#pragma once
#define WIDTH 14
#define HEIGHT 14


typedef struct {
	int cells[HEIGHT][WIDTH];
	int opened[HEIGHT][WIDTH];
	int width;
	int height;
	int bombs;
} Field;