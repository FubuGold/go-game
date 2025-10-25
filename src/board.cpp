#include <cstring>
#include <iostream>

#include "../include/board.h"

char opposite_stone(const char &cur_stone) {
    if (cur_stone == 'X') return 'O';
    return 'X';
}

Move::Move(int pos_x = 0, int pos_y = 0, char stone_type = '.', std::vector<std::pair<int, int>> captured_stones = {}) {
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
    hash_move_list.clear();
}

char Board::get_state(const int &pos_x, const int &pos_y) const {
    return board[pos_x][pos_y];
}

void Board::print_board() const {
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            std::cout << board[i][j];
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void Board::add_move(const Move &new_move) {
    //Assumed this move is vaild
    update_cell(new_move.pos_x, new_move.pos_y, '.', new_move.stone_type);
    hash_move_list.insert(zobrist_hash.get_hash());
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

void Board::undo_move() {
    //Assumed move_list is not empty
    auto last_move = move_list.back();
    move_list.pop_back();
    hash_move_list.erase(zobrist_hash.get_hash());
    
    update_cell(last_move.pos_x, last_move.pos_y, last_move.stone_type, '.');
    char cur_opposite_stone = opposite_stone(last_move.stone_type);

    for (auto &[pos_x, pos_y] : last_move.captured_stones) {
        update_cell(pos_x, pos_y, '.', cur_opposite_stone);
    }

    undo_list.push_back(last_move);
}

void Board::redo_move() {
    //Assumed undo_list is not empty
    auto last_move = undo_list.back();
    undo_list.pop_back();

    update_cell(last_move.pos_x, last_move.pos_y, '.', last_move.stone_type);
    for (auto &[pos_x, pos_y] : last_move.captured_stones) {
        update_cell(pos_x, pos_y, board[pos_x][pos_y], '.');
    }

    hash_move_list.insert(zobrist_hash.get_hash());
    move_list.push_back(last_move);
}

bool Board::check_existed_state(const uint64_t &new_state) const {
    return hash_move_list.count(new_state);
}