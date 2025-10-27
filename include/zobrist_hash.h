#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <cstdint>

class Zobrist_hash {
private:
    uint64_t zobrist[19][19][2];
    uint64_t current_hash;

public:
    Zobrist_hash();

    void reset(); //Reset current_hash to 0

    uint64_t get_hash() const; //Return current_hash

    void update_hash(const int &pos_x, const int &pos_y, const char &stone_type); //Update current_hash value after add/remove a stone
};

#endif