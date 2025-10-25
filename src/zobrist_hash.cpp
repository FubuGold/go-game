#include <random>

#include "../include/zobrist_hash.h"


Zobrist_hash::Zobrist_hash() {
    current_hash = 0;
    std::mt19937_64 rng(998244353727);
    for (int i = 0; i < 19; i++) {
        for (int j = 0; j < 19; j++) {
            zobrist[i][j][0] = rng(); //Black stone
            zobrist[i][j][1] = rng(); //White stone
        }
    }
}

void Zobrist_hash::reset() {
    current_hash = 0;
}

uint64_t Zobrist_hash::get_hash() const {
    return current_hash;
}

void Zobrist_hash::update_hash(const int &pos_x, const int &pos_y, const char &stone_type) {
    if (stone_type == 'X') current_hash ^= zobrist[pos_x][pos_y][0];
    else if (stone_type == 'O') current_hash ^= zobrist[pos_x][pos_y][1];
}