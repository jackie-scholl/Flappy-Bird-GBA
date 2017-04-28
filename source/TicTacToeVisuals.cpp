//
// second.c
// Cleaner version of 'first.c'
//
// (20060211 - 20060922, cearn)

#include "TicTacToeVisuals.hpp"

#include <stdlib.h>

#define BLANK_FILL ((SCREEN_WIDTH-SCREEN_HEIGHT)/2 + 1)

void screen_fill(COLOR color) {
	for (int x = 0; x < SCREEN_WIDTH; x++) {
		for (int y=0; y < SCREEN_HEIGHT; y++) {
			m3_plot(x, y, CLR_WHITE);
		}
	}
}

void horizontal_line(int y, COLOR color) {
	for (int x=0; x < SCREEN_WIDTH; x++) {
		m3_plot(x, y, color);
	}
}

void vertical_line(int x, COLOR color) {
	for (int y=0; y < SCREEN_HEIGHT; y++) {
		m3_plot(x, y, color);
	}
}

void draw_x(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=4; i<48; i++) {
		m3_plot(base_x+i, base_y+i, color);
		m3_plot(base_x+52-i, base_y+i, color);
	}
}

void draw_o(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=0; i<52; i++) {
		for (int j=0; j<52; j++) {
			int t = (i-26)*(i-26) + (j-26)*(j-26);
			int radius = 20;
			int radiusSquared = radius*radius;
			int allowance = radius;
			int diff = abs(t - radiusSquared);
			if (diff < allowance) {
				m3_plot(base_x+i, base_y+j, color);
			}
		}
	}
}

void highlight(int i, int j, COLOR color) {
	int base_x = BLANK_FILL + 54*i;
	int base_y = 54*j;
	for (int i=0; i<52; i++) {
		m3_plot(base_x+i, base_y, color);
		m3_plot(base_x+i, base_y+51, color);
		m3_plot(base_x, base_y+i, color);
		m3_plot(base_x+51, base_y+i, color);
	}
}

void init_screen() {
	REG_DISPCNT= DCNT_MODE3 | DCNT_BG2;

	screen_fill(CLR_WHITE);

	int blank_fill = BLANK_FILL; // 40
	for (int i=0; i<blank_fill; i++) {
		vertical_line(i, CLR_BLACK);
		vertical_line(SCREEN_WIDTH - i, CLR_BLACK);
	}

	int lines[] = {52, 53, 106, 107};
	// lines at 52, 53, 106, 107
	for (int i : lines) {
		horizontal_line(i, CLR_BLACK);
		vertical_line(blank_fill + i, CLR_BLACK);
	}
}

void game_end(COLOR color) {
	//int blank_fill = (SCREEN_WIDTH-SCREEN_HEIGHT)/2 + 1; // 40
	for (int i=0; i<BLANK_FILL; i++) {
		vertical_line(i, color);
		vertical_line(SCREEN_WIDTH - i, color);
	}
}

int main_() {
	init_screen();
	//horizontal_line(80, CLR_BLACK);

	//vertical_line(blank_fill_left + SCREEN_WIDTH, CLR_BLACK);
	//vertical_line(160, CLR_BLACK);
	/*for (int i=blank_fill; i < SCREEN_HEIGHT;;) {
		i += 52;
		horizontal_line(i, CLR_BLACK);
		horizontal_line()
	}*/
	//horizontal_line(blank_fill+52, CLR_BLACK);
	//horizontal_line(100, CLR_BLACK);

	/*m3_plot( 120, 80, RGB15(31, 0, 0) );	// or CLR_RED
	m3_plot( 136, 80, RGB15( 0,31, 0) );	// or CLR_LIME
	m3_plot( 120, 96, RGB15( 0, 0,31) );	// or CLR_BLUE*/
	//m3_plot( 121, 96, RGB15( 0, 0,31) );	// or CLR_BLUE

	draw_x(0, 0, CLR_BLUE);
	draw_x(1, 1, CLR_BLUE);
	draw_x(2, 2, CLR_BLUE);
	draw_x(1, 2, CLR_BLUE);

	draw_o(0, 1, CLR_RED);
	draw_o(0, 2, CLR_RED);
	draw_o(1, 0, CLR_RED);

	highlight(2, 0, CLR_LIME);
	highlight(1, 1, CLR_LIME);

	while(1);

	return 0;
}
