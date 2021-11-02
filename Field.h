#pragma once
#define FIELD_WIDTH 14
#define FIELD_HEIGHT 14


typedef struct {
	int cells[FIELD_HEIGHT][FIELD_WIDTH];
	int opened[FIELD_HEIGHT][FIELD_WIDTH];
	int flags[FIELD_HEIGHT][FIELD_WIDTH];
	int width;
	int height;
	int bombs;
} Field;