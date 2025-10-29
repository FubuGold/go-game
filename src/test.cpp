#include "../include/game_logic.h"
#include "../include/game_scoring.h"
#include <iostream>
#include <cassert>

bool test_scoring_v1() {
    for (int i=0;i<4;i++) {
        for (int j=0;j<3;j++) {
            if ((i == 0 && j == 0) || (i == 1 && j == 0)) continue;
            current_board.add_move(Move(i,j,'O'));
        }
    }
    for (int i=0;i<4;i++) current_board.add_move(Move(4,i,'X'));
    for (int i=0;i<5;i++) current_board.add_move(Move(i,3,'X'));
    for (int i=0;i<3;i++) current_board.add_move(Move(8,i,'O'));
    for (int i=5;i<8;i++) current_board.add_move(Move(i,2,'O'));
    for (int i=5;i<8;i++) current_board.add_move(Move(i,1,'X'));
    current_board.print_board();
    std::pair<int,int> tmp = scoring(current_board);
    std::cout << tmp.first << ' ' << tmp.second << std::endl;
    return tmp.first - tmp.second == 8;
}

int main() {
    // std::cout << "Test" << std::endl;
    assert(test_scoring_v1());
}
