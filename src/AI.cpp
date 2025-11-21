#include "../include/AI.h"

#include <random>
#include <chrono>

const char AI_STONE = 'O';

std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());

/**
 * @brief Random AI
 * 
 * @return Move 
 */
Move easy_ai();

/**
 * @brief Minimax algorithm, with depth = 
 * 
 * @return Move 
 */
Move medium_ai();

/**
 * @brief Minimax algorithm + Alpha-Beta pruning, with depth = 
 * 
 * @return Move 
 */
Move hard_ai();

Move ai_move(Difficulty difficulty) {
    if (difficulty == Difficulty::EASY) return easy_ai();
    if (difficulty == Difficulty::MEDIUM) return medium_ai();
    if (difficulty == Difficulty::HARD) return hard_ai();
}

Move easy_ai() {
    std::vector<std::pair<int,int>> vec;
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,AI_STONE))) {
                current_board.undo_move();
                vec.push_back({i,j});
            }
        }
    }
    int rnd = std::uniform_int_distribution<int>(0,vec.size()-1)(rnd);
    return Move(vec[rnd].first,vec[rnd].second,AI_STONE);
}