#if !defined(AI_H)
#define AI_H

#include "board.h"
#include "game_scoring.h"

enum class Difficulty {
    EASY,
    MEDIUM,
    HARD
};

/**
 * @brief Return AI move. AI depends on the difficulty.
 * 
 * @param difficulty 
 * @return Move 
 */
Move ai_move(Difficulty difficulty); 

#endif // AI_H
