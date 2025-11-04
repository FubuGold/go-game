#include <iostream>
#include <cstdlib>

#include "../include/game_logic.h"

void debug() {
    std::cerr << current_board.save_game() << std::endl;
    std::cerr << current_board.load_game() << std::endl;
    current_board.add_move(Move(0,0,'X'));
    std::cerr << current_board.save_game();
}

int main() {
    bool turn = 1;

    // while (true) {
    //     current_board.print_board();
        
    //     if (!current_board.check_empty_move_list()) {
    //         std::cerr << "Hash value of the previous move: " << current_board.get_board_hash() << '\n';
    //         std::cout << "Do you want to undo the last move? 1 - Yes, 0 - No ";
    //         bool undo_or_not;
    //         std::cin >> undo_or_not;
    //         if (undo_or_not) {
    //             current_board.undo_move();
    //             system("cls");
    //             continue;
    //         }
    //     }
        
    //     std::cout << "Current turn: " << (turn ? "Black" : "White") << std::endl;
    //     int pos_x, pos_y;
    //     do {
    //         std::cout << "Input the coordinate where you want to place your stone: ";
    //         std::cin >> pos_x >> pos_y;
    //         if (!check_vaild_move(Move(pos_x, pos_y, turn ? 'X' : 'O'))) {
    //             std::cout << "Your move is invaild, please try again!" << std::endl;
    //         }
    //         else break;
    //     } while (true);

    //     turn ^= 1;
    //     system("cls");
    // }
    return 0;
}