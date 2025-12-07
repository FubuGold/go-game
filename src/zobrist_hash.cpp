#include <random>
#include <vector>

#include "../include/zobrist_hash.h"


Zobrist_hash::Zobrist_hash() {
    current_hash = 0;
    std::mt19937_64 rng(998244353727);
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            zobrist[i][j][0] = rng(); //Black stone
            zobrist[i][j][1] = rng(); //White stone
        }
    }
}

void Zobrist_hash::reset() {
    current_hash = 0;
}

void Zobrist_hash::set_hash(uint64_t hash) {
    this->current_hash = hash;
}

void Zobrist_hash::set_zobrist(std::vector<std::vector<std::vector<uint64_t>>> &table) {
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            this->zobrist[i][j][0] = table[i][j][0];
            this->zobrist[i][j][1] = table[i][j][1];
        }
    }
}

void Zobrist_hash::copy_zobrist(std::vector<std::vector<std::vector<uint64_t>>> &table) const {
    table.assign(BOARD_SIZE,std::vector<std::vector<uint64_t>>(BOARD_SIZE,std::vector<uint64_t>(2)));
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            table[i][j][0] = this->zobrist[i][j][0];
            table[i][j][1] = this->zobrist[i][j][1];
        }
    }
}


uint64_t Zobrist_hash::get_hash() const {
    return current_hash;
}

void Zobrist_hash::update_hash(const int &pos_x, const int &pos_y, const char &stone_type) {
    if (stone_type == 'X') current_hash ^= zobrist[pos_x][pos_y][0];
    else if (stone_type == 'O') current_hash ^= zobrist[pos_x][pos_y][1];
}

void to_json(json &j, const Zobrist_hash &hash) {
    std::vector<std::vector<std::vector<uint64_t>>> table;
    hash.copy_zobrist(table);
    j["zobrist"] = table;
    j["current_hash"] = hash.get_hash();
}

void from_json(const json &j,Zobrist_hash &hash) {
    std::vector<std::vector< std::vector<uint64_t> > > vec_table;
    int hs;
    j.at("zobrist").get_to(vec_table);
    j.at("current_hash").get_to(hs);
    hash.set_hash(hs);
    hash.set_zobrist(vec_table);
}