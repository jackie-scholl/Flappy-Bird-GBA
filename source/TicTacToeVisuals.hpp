#ifndef TICTACTOEVISUALS_HPP
#define TICTACTOEVISUALS_HPP

#include "tonc_demo_lib/toolbox.h"

void draw_x(int i, int j, COLOR color);
void draw_o(int i, int j, COLOR color);
void highlight(int i, int j, COLOR color);
void init_screen();
void game_end(COLOR color);


#endif // TICTACTOEVISUALS_HPP
