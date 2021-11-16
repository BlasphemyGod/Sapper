#pragma once
#include "framework.h"

int get_timer();

void set_timer(int seconds);

void start_timer();

void stop_timer();

void draw_timer(HDC hdc, int x, int y);
