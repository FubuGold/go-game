#include "../include/game_logic.h"

Board current_board;

void reset_game() {
    current_board.reset();
}

bool check_vaild_move(Move &new_move) {
    if (std::min(new_move.pos_x, new_move.pos_y) < 0 || std::max(new_move.pos_x, new_move.pos_y) >= 19) return false;
    if (current_board.get_state(new_move.pos_x, new_move.pos_y) != '.') return false;
    
    return true;
}

