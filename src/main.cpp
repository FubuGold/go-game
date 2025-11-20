#include <iostream>
#include <cstdlib>
#include <string>
#include <windows.h>
#include "../include/test.h"
#include "../include/game_scoring.h"
#include "../include/game_logic.h"

void main_game();

int main() {
    // testing();
    main_game();
}

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
        std::cin.ignore();
        std::cout << "Invaild input. Please try again.\n";
        std::cout << "Input coordinate: ";
        std::cout.flush();
    }
    std::cin.ignore();
}

void main_game() {

    std::string prev_resp = "";
    int pass_cnt = 0;

    while (true) {
        current_board.print_board();

        std::cout << "Current turn: " << (current_board.get_turn() ? "Black" : "White") << std::endl;
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

        if (!validate_user_action_input(inp)) {
            prev_resp = "Invalid input. Please try again.";
            continue;
        }

        to_lower_all(inp);

        if (inp == "m") {
            int pos_x,pos_y;
            input_coord(pos_x,pos_y);
            while (!add_move(Move(pos_x,pos_y, current_board.get_turn() ? 'X' : 'O'))) {
                std::cout << "Invalid move. Please try another space." << std::endl;
                input_coord(pos_x,pos_y);
            }
            pass_cnt = 0;
            current_board.update_turn();
        }
        else if (inp == "p") {
            current_board.add_move(Move());
            pass_cnt++;
            if (pass_cnt == 2) {
                system("cls");
                break;
            }
            current_board.update_turn();
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
            reset_game();
        }
        else if (inp == "e") {
            return;
        }

        system("cls");
    }

    current_board.print_board();
    std::pair<int,int> score = scoring(current_board);
    std::cout << "Score: ";
    std::cout << "Black - " << score.first << ' ' << "White - " << score.second << '\n';
    if (score.first > score.second) std::cout << "Black win";
    else if (score.first < score.second) std::cout << "White win";
    else std::cout << "Tie";
    std::cout << '\n';
    system("pause");
}
