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
    Move tmp;
    if (difficulty == Difficulty::EASY) tmp = easy_ai();
    else if (difficulty == Difficulty::MEDIUM) tmp = medium_ai();
    else tmp = hard_ai();
    std::cerr << tmp.pos_x << ' ' << tmp.pos_y << ' ' << tmp.stone_type << '\n';
    if (tmp.pos_x >= BOARD_SIZE || tmp.pos_y >= BOARD_SIZE || tmp.pos_x < 0 || tmp.pos_y < 0 || tmp.stone_type == '.') tmp = Move();
    std::cerr << tmp.pos_x << ' ' << tmp.pos_y << ' ' << tmp.stone_type << '\n';
    return tmp;
}

Move easy_ai() {
    std::vector<std::pair<int,int>> vec;
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,AI_STONE))) {
                current_board.undo_move(false);
                vec.push_back({i,j});
            }
        }
    }
    vec.push_back({-1,-1});
    int rnd = rng(0,vec.size()-1);
    sf::sleep(sf::milliseconds(1500)); // Artifical delay
    return Move(vec[rnd].first,vec[rnd].second,vec[rnd].first < 0 ? '.' : AI_STONE);
}

// AI is try to minimize black - white
Node minimax(bool ai_turn,int depth) {

    std::pair<int,int> tmp = scoring(current_board);
    if (depth == 0) return {tmp.first - tmp.second,{-1,-1}};

    int best_move = tmp.first - tmp.second;
    std::vector<std::pair<int,int>> vec;
    char op = opposite_stone(AI_STONE);
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,ai_turn ? AI_STONE : op))) {
                Node cur = minimax(!ai_turn,depth-1);
                cur.second = {i,j};
                // std::cerr << cur.first << '\n';
                if (ai_turn) { // minimize
                    if (best_move > cur.first) {
                        vec.clear();
                        best_move = cur.first;
                    }
                    if (best_move >= cur.first) {
                        vec.push_back(cur.second);
                    }
                }
                else {
                    if (best_move < cur.first) {
                        vec.clear();
                        best_move = cur.first;
                    }
                    if (best_move <= cur.first) {
                        vec.push_back(cur.second);
                    }
                }

                // std::cerr << "AI board pass end 1.1: " << current_board.pass << '\n';
                current_board.undo_move(false);
                // std::cerr << "AI board pass end 1.2: " << current_board.pass << '\n';
            }
        }
    }
    if (vec.size() == 0) return Node(best_move,{BOARD_SIZE,BOARD_SIZE});
    return Node(best_move,vec[rng(0,std::min(19*4,(int)vec.size()-1))]);
}

Move medium_ai() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    Node res = minimax(1,1);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cerr << "AI have thought for: " << ms_double.count() << "ms\n";
    sf::sleep(sf::milliseconds(1500)); // Artifical delay
    // std::cerr << res.first << '\n';
    return Move(res.second.first,res.second.second,AI_STONE);
}

// AI is minimizing black - white
Node minimax_pruning(bool ai_turn,int depth,int alpha,int beta) {
    // std::cerr << ai_turn << ' ' << depth << ' ' << alpha << ' ' << beta << '\n';
    std::pair<int,int> tmp = scoring(current_board);
    if (depth == 0) {
        return {tmp.first - tmp.second,{-1,-1}};
    }
    // Speed up by trying to play move that capture stone
    int pre = ai_turn ? current_board.captured_black : current_board.captured_white;
    Node best_move = {ai_turn ? INT_MAX : INT_MIN, {BOARD_SIZE,BOARD_SIZE}};
    char op = opposite_stone(AI_STONE);
    std::vector<Node> vec;
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (add_move(Move(i,j,ai_turn ? AI_STONE : op))) {
                if (ai_turn) {
                    vec.push_back({pre - current_board.captured_black, {i,j}});
                }
                else {
                    vec.push_back({pre - current_board.captured_white, {i,j}});
                }
                current_board.undo_move(false);
            }
        }
    }
    sort(vec.begin(),vec.end());
    for (const Node &p : vec) {
        int i = p.second.first, j = p.second.second;
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
    return best_move;
}

Move hard_ai() {
    using std::chrono::high_resolution_clock;
    using std::chrono::duration;
    using std::chrono::milliseconds;
    auto t1 = high_resolution_clock::now();

    Node res = minimax_pruning(1,2,INT_MIN,INT_MAX);

    auto t2 = high_resolution_clock::now();
    duration<double, std::milli> ms_double = t2 - t1;
    std::cerr << "AI have thought for: " << ms_double.count() << "ms\n";
    sf::sleep(sf::milliseconds(1500)); // Artifical delay
    return Move(res.second.first,res.second.second,AI_STONE);
}