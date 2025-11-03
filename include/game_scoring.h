#ifndef GAME_SCORING
#define GAME_SCORING

#include "game_logic.h"
#include <utility>

/**
 * @brief Calculate current score from a board state
 * 
 * This function take a board state and calculate the score
 * 
 * @param board Current board state to calculate
 * 
 * @return std::pair<int,int> 
 * 
 * {black score, white score}
 */
std::pair<int,int> scoring(const Board &board);

#endif // GAME_SCORING
