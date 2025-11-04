#include <cstring>
#include <iostream>

#include "../include/board.h"

char opposite_stone(const char &cur_stone) {
    if (cur_stone == 'X') return 'O';
    return 'X';
}

Move::Move(int pos_x, int pos_y, char stone_type, uint64_t hash_val, std::vector<std::pair<int, int>> captured_stones) {
    this->pos_x = pos_x;
    this->pos_y = pos_y;
    this->stone_type = stone_type;
    this->captured_stones = captured_stones;
}

Board::Board() {
    std::memset(board, '.', sizeof board);
}

void Board::reset() {
    std::memset(board, '.', sizeof board);
    move_list.clear();
    undo_list.clear();
    zobrist_hash.reset();
}

char Board::get_state(const int &pos_x, const int &pos_y) const {
    return board[pos_x][pos_y];
}

void Board::print_board() const {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Board::add_move(Move new_move) {
    //Assumed this move is vaild
    update_cell(new_move.pos_x, new_move.pos_y, '.', new_move.stone_type);

    char cur_opposite_stone = opposite_stone(new_move.stone_type);
    for (auto &[pos_x, pos_y] : new_move.captured_stones) {
        update_cell(pos_x, pos_y, cur_opposite_stone, '.');
    }
    
    new_move.hash_val = zobrist_hash.get_hash();
    move_list.push_back(new_move);
}

void Board::update_cell(const int &pos_x, const int &pos_y, const char &prev_stone_type, const char &new_stone_type) {
    zobrist_hash.update_hash(pos_x, pos_y, prev_stone_type);
    board[pos_x][pos_y] = new_stone_type;
    zobrist_hash.update_hash(pos_x, pos_y, new_stone_type);
}

bool Board::check_empty_move_list() const {
    return move_list.empty();
}

bool Board::check_empty_undo_list() const {
    return undo_list.empty();
}

void Board::undo_move(bool player_make_undo) {
    //Assumed move_list is not empty
    auto last_move = move_list.back();
    move_list.pop_back();
    
    update_cell(last_move.pos_x, last_move.pos_y, last_move.stone_type, '.');
    char cur_opposite_stone = opposite_stone(last_move.stone_type);

    for (auto &[pos_x, pos_y] : last_move.captured_stones) {
        update_cell(pos_x, pos_y, '.', cur_opposite_stone);
    }

    if (player_make_undo) undo_list.push_back(last_move); //Only redo-able if this undo action is made by player
}

void Board::redo_move() {
    //Assumed undo_list is not empty
    auto last_move = undo_list.back();
    undo_list.pop_back();

    update_cell(last_move.pos_x, last_move.pos_y, '.', last_move.stone_type);
    for (auto &[pos_x, pos_y] : last_move.captured_stones) {
        update_cell(pos_x, pos_y, board[pos_x][pos_y], '.');
    }

    move_list.push_back(last_move);
}

uint64_t Board::get_board_hash() const {
    return zobrist_hash.get_hash();
}

bool Board::check_existed_state() const {
    if ((int)move_list.size() < 3) return false;
    return zobrist_hash.get_hash() == move_list[(int)move_list.size() - 3].hash_val;
}