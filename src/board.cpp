#include <cstring>
#include <iostream>
#include <fstream>
#include <string>
#include "../lib/json.hpp"

#include "../include/board.h"

using json = nlohmann::json;

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
    
    if (new_move.stone_type == 'X') captured_white += new_move.captured_stones.size();
    else if (new_move.stone_type == 'O') captured_black += new_move.captured_stones.size();

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

int Board::get_captured_black() const {
    return captured_black;
}
    
int Board::get_captured_white() const {
    return captured_white;
}

uint64_t Board::get_board_hash() const {
    return zobrist_hash.get_hash();
}

bool Board::check_existed_state() const {
    if ((int)move_list.size() < 3) return false;
    return zobrist_hash.get_hash() == move_list[(int)move_list.size() - 3].hash_val;
}

void to_json(json &j,const Move &move) {
    j["pos_x"] = move.pos_x;
    j["pos_y"] = move.pos_y;
    j["stone_type"] = move.stone_type;
    j["hash_val"] = move.hash_val;
    j["captured_stones"] = move.captured_stones;
}

void from_json(const json &j, Move &move) {
    j.at("pos_x").get_to(move.pos_x);
    j.at("pos_y").get_to(move.pos_y);
    j.at("hash_val").get_to(move.hash_val);
    j.at("captured_stones").get_to(move.captured_stones);
}

bool Board::save_game(std::filesystem::path filepath) const {
    try {
        json j;

        j["board"] = this->board;
        j["move_list"] = this->move_list;
        j["undo_list"] = this->undo_list;
        j["zobrist_hash"] = this->zobrist_hash;

        std::ofstream o(filepath);
        o << j << std::endl;
        o.close();

        return true;
    }
    catch (...){
        return false;
    }
}

bool Board::load_game(std::filesystem::path filepath) {
    try {
        json j;

        std::ifstream i(filepath);
        if (!i.is_open()) {
            std::cerr << "Open file error. Check the path again\n";
            return false;
        }
        i >> j;
        i.close();

        std::cerr << "Json loaded" << std::endl;
        
        std::vector<std::string> vec_board;
        j.at("board").get_to(vec_board);
        for (int i=0;i<BOARD_SIZE;i++) for (int j=0;j<BOARD_SIZE;j++) 
        this->board[i][j] = vec_board[i][j];
        
        std::cerr << "Board loaded" << std::endl;
        j.at("move_list").get_to(this->move_list);
        std::cerr << "Move list loaded" << std::endl;
        j.at("undo_list").get_to(this->undo_list);
        std::cerr << "Undo list loaded" << std::endl;
        j.at("zobrist_hash").get_to(this->zobrist_hash);
        std::cerr << "Hash loaded" << std::endl;

        return true;
    }
    catch (...) {
        return false;
    }
}
