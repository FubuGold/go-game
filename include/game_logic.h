#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include "board.h"

const int direction_x[] = {0, 0, 1, -1};
const int direction_y[] = {1, -1, 0, 0};

extern Board current_board;
extern bool ko_rule_violated;

void reset_game();

std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> find_captured_stone(int stx,int sty); //Return two vectors which contain captured white and black stone 

bool add_move(Move new_move);

/**
 * @brief Check if there exists a way to violate the KO rule
 * 
 * @return true 
 * @return false 
 */
bool check_ko_rule();

/**
 * @brief Check if the input move will make a KO Thread. Assume that this move hasn't placed yet
 * 
 * @param new_move 
 * @return true 
 * @return false 
 */
bool check_ko_threat(Move new_move);

#endif