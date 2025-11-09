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

/**
 * @brief Change input string to lowercase (using tolower)
 * 
 * @param s 
 */
void to_lower_all(std::string &s) {
    for (int i=0;i<s.size();i++) s[i] = tolower(s[i]);
}

/**
 * @brief Validate user input for action
 * 
 * @param s 
 * @return true 
 * @return false 
 */
bool validate_user_action_input(std::string s) {
    to_lower_all(s);
    if (s == "m" || s == "p" || s == "u") return true;
    if (s == "s" || s == "l" || s == "r") return true;
    if (s == "rs" || s == "e") return true;
    return false; 
}

/**
 * @brief Input coordinate from user
 * 
 * Take input position in x and y and return the coordinate via the argument
 * 
 * @param pos_x 
 * @param pos_y 
 */
void input_coord(int &pos_x,int &pos_y) {
    std::cout << "Input number ranging from 0 to 18\n";
    std::cout << "Input coordinate: ";
    while (!(std::cin >> pos_x >> pos_y)) {
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        std::cout << "Invaild input. Please try again.\n";
        std::cout << "Input coordinate: ";
        std::cout.flush();
    }
}

int main() {
    bool turn = 1;

    std::string prev_resp = "";

    while (true) {
        current_board.print_board();

        std::cout << "Current turn: " << (turn ? "Black" : "White") << std::endl;
        std::cout << "Enter one of the command below (lowercase or uppercase):\n";
        std::cout << "m: play move for current player\n";
        std::cout << "p: pass move\n";
        std::cout << "u: undo previous move\n";
        std::cout << "r: redo move\n";
        std::cout << "s: current game\n";
        std::cout << "l: load game in save file\n";
        std::cout << "rs: reset game\n";
        std::cout << "e: exit gamne\n";
        std::cout << prev_resp << '\n';
        std::cout << "Input: ";
        std::cout.flush();

        prev_resp = "";

        std::string inp;
        std::cin >> inp;
        
        std::cin.ignore();

        while (!validate_user_action_input(inp)) {
            std::cin.ignore();
            std::cout << "Invalid input. Please try again.\n";
            std::cout << "Input: ";
            std::cout.flush();
            std::cin >> inp;
        }

        to_lower_all(inp);

        if (inp == "m") {
            int pos_x,pos_y;
            input_coord(pos_x,pos_y);
            while (!check_vaild_move(Move(pos_x,pos_y, turn ? 'X' : 'O'))) {
                std::cout << "Invalid move. Please try another space." << std::endl;
                input_coord(pos_x,pos_y);
            }
            std::cin.ignore();
            turn ^= 1;
        }
        else if (inp == "p") {
            current_board.add_move(Move());
            turn ^= 1;
        }
        else if (inp == "u") {
            if (current_board.check_empty_move_list()) {
                prev_resp = "There is no move to redo";
            }
            else {
                current_board.undo_move();
            }
        }
        else if (inp == "r") {
            if (current_board.check_empty_undo_list()) {
                prev_resp = "There is no move to redo";
            }
            else {
                current_board.redo_move();
            }
        }
        else if (inp == "s") {
            if (!current_board.save_game()) prev_resp = "Fail to save game";
            else prev_resp = "Saved game successfully";
        }
        else if (inp == "l") {
            if (!current_board.load_game()) prev_resp = "Load game failed";
            else prev_resp = "Loaded game successfully";
        }
        else if (inp == "rs") {
            current_board.reset();
        }
        else if (inp == "e") {
            break;
        }

        system("cls");
    }
    return 0;
}