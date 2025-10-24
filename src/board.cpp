#include "../include/board.h"

Board_State::Board_State(int pos_x, int pos_y, int cur_val) : pos_x(pos_x), pos_y(pos_y), cur_val(cur_val) {}

void Board::Board_Reset() {
    Board::Board_State_Storer.clear();
}

void Board::Add_Board_State(const Board_State &new_Board_State) {
    Board::Board_State_Storer.push_back(new_Board_State);
}
