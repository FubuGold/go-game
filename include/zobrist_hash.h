#ifndef ZOBRIST_HASH_H
#define ZOBRIST_HASH_H

#include <cstdint>
#include "../lib/json.hpp"
#include "custom_util.h"

using json = nlohmann::json;

class Zobrist_hash {
private:
    uint64_t zobrist[BOARD_SIZE][BOARD_SIZE][2];
    uint64_t current_hash;

public:
    Zobrist_hash();

    void reset(); //Reset current_hash to 0

    /**
     * @brief Set the hash object
     * 
     * Set object's hash from input (use for json parse)
     * 
     * @param hash 
     */
    void set_hash(uint64_t hash);
    
    /**
     * @brief Set the zobrist object
     * 
     * Set object's zobrist table from input (use for json parse)
     * 
     * @param zobrist_table 
     */
    void set_zobrist(uint64_t zobrist_table[BOARD_SIZE][BOARD_SIZE][2]);

    /**
     * @brief Copy zobrist table to another
     * 
     * Copy the current zobrist table to another table (use for json parse)
     * 
     * @param table 
     */
    void copy_zobrist(uint64_t table[BOARD_SIZE][BOARD_SIZE][2]) const;

    /**
     * @brief Get the hash object
     * 
     * @return uint64_t 
     */
    uint64_t get_hash() const;

    void update_hash(const int &pos_x, const int &pos_y, const char &stone_type); //Update current_hash value after add/remove a stone
};

void to_json(json &j, const Zobrist_hash &hash);

void from_json(const json &j, Zobrist_hash &hash);

#endif