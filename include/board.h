#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <unordered_set>

#include "zobrist_hash.h"

#define BOARD_SIZE 19

char opposite_stone(const char &cur_stone); //Return the opposite stone of the current stone

struct Move {
    int pos_x, pos_y;
    char stone_type; //Black 'X' or white 'O'
    std::vector<std::pair<int, int>> captured_stones; //Store which stone has been captured in this move

    Move(int pos_x = 0, int pos_y = 0, char stone_type = '.', std::vector<std::pair<int, int>> captured_stones = {});
};

class Board {
private:
    char board[BOARD_SIZE][BOARD_SIZE]; //Use this array to show the board state on the screen
    std::vector<Move> move_list; //Store all the moves players have made so far
    std::vector<Move> undo_list; //Store all the undo moves for redo function
    Zobrist_hash zobrist_hash;
    std::unordered_set<uint64_t> hash_move_list;

public:
    Board();
    
    void reset(); //Reset the entire board to its initial state
    
    char get_state(const int &pos_x, const int &pos_y) const; //Return the current state of cell (pos_x, pos_y)

    void print_board() const; //Print the board array to the screen

    void add_move(const Move &new_move); //Add new move to move_list, need to check vaild move before we call this function

    void update_cell(const int &pos_x, const int &pos_y, const char &prev_stone_type, const char &new_stone_type); //Update one cell state and update the new hash value

    bool check_empty_move_list() const; //Return true/false if the move_list array is empty

    bool check_empty_undo_list() const; //Return true/false if the undo_list array is empty

    void undo_move(); //Need to check if we could undo the move

    void redo_move(); //Need to check if we could redo the move

    bool check_existed_state(const uint64_t &new_state) const; //Return true if the input state is already existed
};

#endif