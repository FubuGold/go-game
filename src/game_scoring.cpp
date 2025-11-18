#include "../include/game_scoring.h"
#include <iostream>

const int KOMI = 6;

bool check(int x,int y) {
    return x >= 0 && y >= 0 && x < BOARD_SIZE && y < BOARD_SIZE;
}

void dfs(int x,int y,int setter,int vst[BOARD_SIZE][BOARD_SIZE], const Board &board) {
    vst[x][y] = setter;
    for (int i=0;i<4;i++) {
        int vx = x + direction_x[i], vy = y + direction_y[i];
        if (check(vx,vy) && board.get_state(vx,vy) == '.' && vst[vx][vy] != setter) dfs(vx,vy,setter,vst,board);
    }
}

std::pair<int,int> scoring(const Board &board) {
    int black_point = board.get_captured_white(), white_point = KOMI + board.get_captured_black();
    int vst[BOARD_SIZE][BOARD_SIZE];
    for (int i=0;i<BOARD_SIZE;i++) for (int j=0;j<BOARD_SIZE;j++) vst[i][j] = 0;
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            if (board.get_state(i,j) == 'X') {
                for (int d = 0; d < 4; d++) {
                    int vx = i + direction_x[d],
                        vy = j + direction_y[d];
                    if (!check(vx,vy)) continue;
                    if (vst[vx][vy] == 1 || vst[vx][vy] == 3) continue;
                    if (vst[vx][vy] == 2) dfs(i,j,3,vst,board);
                    else dfs(i,j,1,vst,board);
                }
            }
            else if (board.get_state(i,j) == 'O') {
                for (int d = 0; d < 4; d++) {
                    int vx = i + direction_x[d],
                        vy = j + direction_y[d];
                    if (!check(vx,vy) || board.get_state(vx,vy) != '.') continue;
                    if (vst[vx][vy] == 2 || vst[vx][vy] == 3) continue;
                    if (vst[vx][vy] == 1) dfs(i,j,3,vst,board);
                    else dfs(i,j,2,vst,board);
                }
            }
        }
    }
    for (int i=0;i<BOARD_SIZE;i++) {
        for (int j=0;j<BOARD_SIZE;j++) {
            black_point += vst[i][j] == 1;
            white_point += vst[i][j] == 2;
        }
    }
    return {black_point,white_point};
}