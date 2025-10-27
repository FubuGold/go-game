#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.h"

const int direction_x[] = {0, 0, 1, -1};
const int direction_y[] = {1, -1, 0, 0};

extern Board current_board;

void reset_game();

bool check_vaild_move(const Move &new_move);

#endif