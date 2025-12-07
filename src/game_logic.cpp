#include <queue>
#include <cstring>
#include <iostream> //Added to debug

#include "../include/game_logic.h"
#include "../include/custom_util.h"

Board current_board;
bool ko_rule_violated = false;

void reset_game() {
    current_board.reset();
}

CustomQueue q;

std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> find_captured_stone(int stx,int sty) {
    q.clear();
    std::vector<std::pair<int, int>> captured_stone[2]; //0 = white, 1 = black
    bool visited[BOARD_SIZE][BOARD_SIZE];
    std::memset(visited, false, sizeof visited);

    for (int k=0;k<=4;k++) {
        int i = stx,j = sty;

        if (k < 4) i += direction_x[k], j += direction_y[k];
        if (std::min(i, j) < 0 || std::max(i, j) >= BOARD_SIZE) continue;

        char current_stone_state = current_board.get_state(i, j);
        if (!visited[i][j] && current_stone_state != '.') {
            // std::cerr << i << ' ' << j << '\n';
            visited[i][j] = true;
            bool has_liberty = false;
            std::vector<std::pair<int, int>> stone_list;

            q.emplace(i,j);

            while (!q.empty()) {
                auto cur = q.front();
                q.pop();

                stone_list.push_back(cur);
                for (int x, y, t = 0; t < 4; t++) {
                    x = cur.first + direction_x[t];
                    y = cur.second + direction_y[t];
                    if (std::min(x, y) >= 0 && std::max(x, y) < BOARD_SIZE) {
                        char state = current_board.get_state(x, y);
                        if (state == current_stone_state && !visited[x][y]) {
                            visited[x][y] = true;
                            q.emplace(x, y);
                        }
                        else if (state == '.') {
                            has_liberty = true;
                        }
                    }
                }
            }

            if (!has_liberty) {
                captured_stone[current_stone_state == 'X'].insert(
                    captured_stone[current_stone_state == 'X'].end(),
                    stone_list.begin(),stone_list.end()
                );
            }
        }
    }
    
    return {captured_stone[0], captured_stone[1]};
}

bool add_move(Move new_move) {
    if (std::min(new_move.pos_x, new_move.pos_y) < 0 || std::max(new_move.pos_x, new_move.pos_y) >= 19) return false; //Checking if this move is inside the board
    if (current_board.get_state(new_move.pos_x, new_move.pos_y) != '.') return false; //Checking if the current cell is empty
    
    current_board.add_move(new_move);

    //Checking liberty
    auto captured_list = find_captured_stone(new_move.pos_x,new_move.pos_y); //.first = white, .second = black
    char current_stone_state = current_board.get_state(new_move.pos_x, new_move.pos_y);

    if (current_stone_state == 'X') { //If the current move is black
        if (captured_list.first.empty() && !captured_list.second.empty()) { //Didn't capture any white stone and blocked liberty of the current black stone
            current_board.undo_move(false);
            return false;
        }
        else {
            new_move.captured_stones = captured_list.first;
        }
    }
    else {
        if (captured_list.second.empty() && !captured_list.first.empty()) { //Didn't capture any black stone and blocked liberty of the current white stone
            current_board.undo_move(false);
            return false;
        }
        else {
            new_move.captured_stones = captured_list.second;
        }
    }
    
    //Need to do this in order to add captured stone to new_move state
    current_board.undo_move(false);
    current_board.add_move(new_move);

    //Checking KO rule
    if (current_board.check_existed_state()) {
        ko_rule_violated = true;
        current_board.undo_move(false);
        return false;
    }
    
    current_board.clear_undo_list();
    
    return true;
}

bool check_ko_rule() {
    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            ko_rule_violated = false;

            if (add_move(Move(i, j, current_board.get_turn() ? 'X' : 'O'))) {
                current_board.undo_move(false);
            }
            else if (ko_rule_violated) {
                ko_rule_violated = false;
                return true;
            }
        }
    }
    return false;
}

bool check_ko_threat(Move new_move) {
    if (!check_ko_rule()) return false;
    if (!add_move(new_move)) return false;
    
    bool visited[BOARD_SIZE][BOARD_SIZE];

    char oppo = opposite_stone(current_board.get_turn() ? 'X' : 'O');
    for (int k = 0; k < 4; k++) {
        int x = new_move.pos_x + direction_x[k], y = new_move.pos_y + direction_y[k];

        if (std::min(x, y) < 0 || std::max(x, y) >= BOARD_SIZE || current_board.get_state(x, y) != oppo) continue;

        q.clear();
        std::memset(visited, false, sizeof visited);

        int liberties = 0;
        visited[x][y] = true;
        q.emplace(x, y);

        while (!q.empty()) {
            int u, v;
            std::tie(u, v) = q.front();
            q.pop();

            for (int _k = 0; _k < 4; _k++) {
                int _x = u + direction_x[_k], _y = v + direction_y[_k];

                if (std::min(_x, _y) >= 0 && std::max(_x, _y) < BOARD_SIZE) {
                    char state = current_board.get_state(_x, _y);
                    if (state == oppo && !visited[_x][_y]) {
                        visited[_x][_y] = true;
                        q.emplace(_x, _y);
                    }
                    else if (state == '.') {
                        ++liberties;
                        if (liberties > 1) {
                            q.clear();
                            break;
                        }
                    }
                }
            }
        }

        if (liberties == 1) {
            current_board.undo_move(false);
            return true;
        }
    }

    current_board.undo_move(false);
    return false;
}