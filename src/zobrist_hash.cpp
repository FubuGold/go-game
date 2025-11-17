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

void Zobrist_hash::set_zobrist(uint64_t zobrist_table[BOARD_SIZE][BOARD_SIZE][2]) {
    std::copy(&(zobrist_table[0][0][0]),&(zobrist_table[0][0][0])+BOARD_SIZE*BOARD_SIZE*2,&(this->zobrist[0][0][0]));
}

void Zobrist_hash::copy_zobrist(uint64_t table[BOARD_SIZE][BOARD_SIZE][2]) const {
    std::copy(&(this->zobrist[0][0][0]),&(this->zobrist[0][0][0])+BOARD_SIZE*BOARD_SIZE*2,&(table[0][0][0]));
}


uint64_t Zobrist_hash::get_hash() const {
    return current_hash;
}

void Zobrist_hash::update_hash(const int &pos_x, const int &pos_y, const char &stone_type) {
    if (stone_type == 'X') current_hash ^= zobrist[pos_x][pos_y][0];
    else if (stone_type == 'O') current_hash ^= zobrist[pos_x][pos_y][1];
}

void to_json(json &j, const Zobrist_hash &hash) {
    uint64_t table[BOARD_SIZE][BOARD_SIZE][2];
    hash.copy_zobrist(table);
    j["zobrist"] = table;
    j["current_hash"] = hash.get_hash();
}

void from_json(const json &j,Zobrist_hash &hash) {
    std::vector<std::vector< std::vector<uint64_t> > > vec_table;
    uint64_t c_table[BOARD_SIZE][BOARD_SIZE][2];
    int hs;
    j.at("zobrist").get_to(vec_table);
    j.at("current_hash").get_to(hs);
    for (int i=0;i<BOARD_SIZE;i++) for (int j=0;j<BOARD_SIZE;j++) for (int k=0;k<2;k++)
        c_table[i][j][k] = vec_table[i][j][k];
    hash.set_hash(hs);
    hash.set_zobrist(c_table);
}