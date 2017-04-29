#include <stdio.h>

#include "tonc_demo_lib/toolbox.h"
#include "tonc_demo_lib/input.h"

#include "TicTacToe.h"
#include "TicTacToeVisuals2.hpp"

#include "TicTacToeInput.hpp"

int cursor_x = 0, cursor_y = 0;

short getUserInputSquare(TicTacToe board) {
  //printf("hello");

  key_poll();
  while (!key_hit(KEY_A)) {
  //while (!KEY_DOWN_NOW(KEY_A)) {

    //int old_cursor_x = cursor_x, old_cursor_y = cursor_y;
    // cursor_x += key_tri_horz();
    // cursor_y += key_tri_vert();
    if (key_hit(KEY_LEFT))
      cursor_x -= 1;
    if (key_hit(KEY_RIGHT))
      cursor_x += 1;
    if (key_hit(KEY_DOWN))
      cursor_y += 1;
    if (key_hit(KEY_UP))
      cursor_y -= 1;

    cursor_x = (cursor_x + 3) % 3;
    cursor_y = (cursor_y + 3) % 3;


    board.printState();
    //highlight(old_cursor_x, old_cursor_y, CLR_WHITE);
    //highlight(2, 2, CLR_CYAN);
    draw_highlight2(cursor_x, cursor_y);

    key_poll();
  }
  //highlight(cursor_x, cursor_y, CLR_WHITE);
  //highlight(2, 2, CLR_WHITE);
  return 3*cursor_x + cursor_y;
  //highlight(cursor_x, cursor_y, CLR_LIME);
}
