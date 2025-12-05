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
    Node best_move = {ai_turn ? INT_MAX : INT_MIN, {BOARD_SIZE,BOARD_SIZE}};
    char op = opposite_stone(AI_STONE);
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,ai_turn ? AI_STONE : op))) {
                Node cur = minimax(!ai_turn,depth-1);
                cur.second = {i,j};
                if (ai_turn) { // minimize
                    best_move = std::min(best_move,cur);
                }
                else {
                    best_move = std::max(best_move,cur);
                    
                }

                current_board.undo_move(false);
            }
        }
    }
    return best_move;
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
Node minimax_pruning(bool ai_turn,int depth,int alpha,int beta) {
    // std::cerr << ai_turn << ' ' << depth << ' ' << alpha << ' ' << beta << '\n';
    if (depth == 0) {
        std::pair<int,int> tmp = scoring(current_board);
        return {tmp.first - tmp.second,{-1,-1}};
    }
    // Speed up by trying to play move that capture stone
    int pre = ai_turn ? current_board.captured_black : current_board.captured_white;
    Node best_move = {ai_turn ? INT_MAX : INT_MIN, {BOARD_SIZE,BOARD_SIZE}};
    char op = opposite_stone(AI_STONE);
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,ai_turn ? AI_STONE : op))) {
                Node cur = minimax_pruning(!ai_turn,depth-1,alpha,beta);
                cur.second = {i,j};
                if (ai_turn) {
                    best_move = std::min(best_move,cur);
                    beta = std::min(beta,cur.first);
                    // std::cerr << best_move.first << ' ' << best_move.second.first << ' ' << best_move.second.second << '\n';
                    if (beta <= alpha) {
                        // std::cerr << "Break: " << ai_turn << ' ' << depth << ' ' << alpha << ' ' << beta << '\n';
                        current_board.undo_move(false);
                        return best_move;
                    }
                }
                else {
                    best_move = std::max(best_move,cur);
                    alpha = std::max(alpha,cur.first);
                    // std::cerr << best_move.first << ' ' << best_move.second.first << ' ' << best_move.second.second << '\n';
                    if (beta <= alpha) {
                        // std::cerr << "Break: " << ai_turn << ' ' << depth << ' ' << alpha << ' ' << beta << '\n';
                        current_board.undo_move(false);
                        return best_move;
                    }
                }
                current_board.undo_move(false);
            }
        }
    }
    return best_move;
}

Move hard_ai() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    Node res = minimax_pruning(1,3,INT_MIN,INT_MAX);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cerr << ms_double.count() << "ms\n";

    return Move(res.second.first,res.second.second,AI_STONE);
}