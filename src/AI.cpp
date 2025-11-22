#include "../include/AI.h"

#include <random>
#include <chrono>
#include <iostream>

// For minimax algorithm
using Node = std::pair<int,std::pair<int,int>>;

const char AI_STONE = 'O';

std::mt19937 rnd(std::chrono::high_resolution_clock::now().time_since_epoch().count());
int rng(int l,int r) {
    return std::uniform_int_distribution<int>(l,r)(rnd);
}

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
    return hard_ai();
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
    int rnd = rng(0,vec.size()-1);
    return Move(vec[rnd].first,vec[rnd].second,AI_STONE);
}

// AI is try to minimize black - white
Node minimax(bool ai_turn,int depth) {
    if (depth == 0) {
        std::pair<int,int> tmp = scoring(current_board);
        return {tmp.first - tmp.second,{-1,-1}};
    }
    int best_score = ai_turn ? INT_MIN : INT_MAX;
    std::pair<int,int> best_move;
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,ai_turn ? AI_STONE : opposite_stone(AI_STONE)))) {
                Node cur = minimax(!ai_turn,depth-1);
                
                if (ai_turn) { // minimize
                    if (cur.first < best_score) {
                        best_score = cur.first;
                        best_move = {i,j};
                    }
                }
                else {
                    if (cur.first > best_score) {
                        best_score = cur.first;
                        best_move = {i,j};
                    }
                }

                current_board.undo_move(false);
            }
        }
    }
    return {best_score,best_move};
}

Move medium_ai() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    Node res = minimax(1,2);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cerr << ms_double.count() << "ms\n";
    return Move(res.second.first,res.second.second,AI_STONE);
}

// AI is minimizing black - white
Node minimax_pruning(bool ai_move,int depth) {

}

Move hard_ai() {
    return Move(0,0,AI_STONE);
}