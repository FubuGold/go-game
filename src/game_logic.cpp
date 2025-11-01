#include <queue>
#include <cstring>
#include <iostream> //Added to debug

#include "../include/game_logic.h"

Board current_board;

void reset_game() {
    current_board.reset();
}

std::pair<std::vector<std::pair<int, int>>, std::vector<std::pair<int, int>>> find_captured_stone() {
    std::queue<std::pair<int, int>> q;
    std::vector<std::pair<int, int>> captured_stone[2]; //0 = white, 1 = black
    bool visited[BOARD_SIZE][BOARD_SIZE];
    std::memset(visited, false, sizeof visited);

    for (int i = 0; i < BOARD_SIZE; i++) {
        for (int j = 0; j < BOARD_SIZE; j++) {
            char current_stone_state = current_board.get_state(i, j);
            if (!visited[i][j] && current_stone_state != '.') {
                q.emplace(i, j);
                visited[i][j] = true;
                bool has_liberty = false;
                std::vector<std::pair<int, int>> stone_list;

                while (!q.empty()) {
                    auto cur = q.front();
                    q.pop();

                    stone_list.push_back(cur);
                    for (int x, y, k = 0; k < 4; k++) {
                        x = cur.first + direction_x[k];
                        y = cur.second + direction_y[k];
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
                    captured_stone[current_stone_state == 'X'] = stone_list;
                }
            }
        }
    }
    
    return {captured_stone[0], captured_stone[1]};
}

bool check_vaild_move(Move new_move) {
    if (std::min(new_move.pos_x, new_move.pos_y) < 0 || std::max(new_move.pos_x, new_move.pos_y) >= 19) return false; //Checking if this move is inside the board
    if (current_board.get_state(new_move.pos_x, new_move.pos_y) != '.') return false; //Checking if the current cell is empty
    
    //Checking liberty
    current_board.add_move(new_move);
    auto captured_list = find_captured_stone(); //.first = white, .second = black
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
        current_board.undo_move(false);
        return false;
    }

    return true;
}