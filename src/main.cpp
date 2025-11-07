#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>

#include "../include/game_logic.h"

void debug() {
    std::cerr << current_board.save_game() << std::endl;
    std::cerr << current_board.load_game() << std::endl;
    current_board.add_move(Move(0,0,'X'));
    std::cerr << current_board.save_game();
}

void to_lower_all(std::string &s) {
    for (int i=0;i<s.size();i++) s[i] = tolower(s[i]);
}

bool validate_user_input(std::string s) {
    to_lower_all(s);
    if (s == "m" || s == "u" || s == "s" || s == "l") return true;
    return false; 
}

int main() {
    bool turn = 1;

    while (true) {
        current_board.print_board();

        std::cout << "Current turn: " << (turn ? "Black" : "White") << std::endl;
        std::cout << "Enter one of the command below (lowercase or uppercase):\n";
        std::cout << "m: play move for current player\n";
        std::cout << "u: undo previous move\n";
        std::cout << "s: current game\n";
        std::cout << "l: load game in save file\n";
        std::cout << "Input: ";
        std::cout.flush();

        std::string inp;
        std::cin >> inp;
        
        std::cin.ignore();

        while (!validate_user_input(inp)) {
            std::cout << "Invalid input. Please try again.\n";
            std::cout << "Input: ";
            std::cout.flush();
        }

        to_lower_all(inp);

        if (inp == "m") {
            std::cout << "Input coordinate: ";
            int pos_x,pos_y; std::cin >> pos_x >> pos_y;
            while (!check_vaild_move(Move(pos_x,pos_y))) {
                std::cout << "Invaild move. Try again\n";
                std::cout << "Input coordinate: ";
                std::cout.flush();
            }
            std::cin.ignore();
            turn ^= 1;
        }
        else if (inp == "u") {

        }
        else if (inp == "s") {
            if (current_board.save_game()) continue;
            std::cout << "Save game failed\n";
            std::cout << "Try again after 5 second" << std::endl;
            Sleep(5000);
        }
        else if (inp == "l") {
            if(current_board.load_game()) continue;
            std::cout << "Load game failed\n";
            std::cout << "Try again after 5 seconds" << std::endl;
            Sleep(5000);
        }

        system("cls");
    }
    return 0;
}