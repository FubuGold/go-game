#ifndef BOARD_H
#define BOARD_H

#include <vector>

#include "zobrist_hash.h"

#define BOARD_SIZE 19

char opposite_stone(const char &cur_stone); //Return the opposite stone of the current stone

struct Move {
    int pos_x, pos_y;
    char stone_type; //Black 'X' or white 'O'
    uint64_t hash_val; //The board's hash value after placed this move
    std::vector<std::pair<int, int>> captured_stones; //Store which stone has been captured in this move

    Move(int pos_x = 0, int pos_y = 0, char stone_type = '.', uint64_t hash_val = 0, std::vector<std::pair<int, int>> captured_stones = {});
};

class Board {
private:
    char board[BOARD_SIZE][BOARD_SIZE]; //Use this array to show the board state on the screen
    std::vector<Move> move_list; //Store all the moves players have made so far
    std::vector<Move> undo_list; //Store all the undo moves for redo function
    int captured_black,captured_white;
    Zobrist_hash zobrist_hash;

public:
    Board();
    
    void reset(); //Reset the entire board to its initial state
    
    char get_state(const int &pos_x, const int &pos_y) const; //Return the current state of cell (pos_x, pos_y)

    void print_board() const; //Print the board array to the screen

    void add_move(Move new_move); //Add new move to move_list, need to check vaild move before we call this function

    void update_cell(const int &pos_x, const int &pos_y, const char &prev_stone_type, const char &new_stone_type); //Update one cell state and update the new hash value

    bool check_empty_move_list() const; //Return true/false if the move_list array is empty

    bool check_empty_undo_list() const; //Return true/false if the undo_list array is empty

    void undo_move(bool player_make_undo = true); //Need to check if we could undo the move

    void redo_move(); //Need to check if we could redo the move

    uint64_t get_board_hash() const; //Return current board hash

    /**
     * @brief Get the number of captured black stone
     * 
     * @return int 
     */
    int get_captured_black() const;
    
    /**
     * @brief Get the number of captured white stone
     * 
     * @return int 
     */
    int get_captured_white() const;

    bool check_existed_state() const; //Return true if the input state is already existed

    /**
     * @brief Save game to file
     * 
     * Save current board state to a local file. Override previous file if exist.
     * 
     * Return if the process is success or not
     * 
     * @return true 
     * @return false 
     */
    bool save_game() const;

    /**
     * @brief Load game from file
     * 
     * Load board state from locally saved json file and override the current board
     * 
     * Return if the process is success or not
     * 
     * @return true 
     * @return false 
     */
    bool load_game();
};

void to_json(json &j,const Move &move);

void from_json(const json &j,Move &move);

#endif