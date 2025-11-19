#include "../include/game_logic.h"
#include "../include/game_scoring.h"
#include <iostream>
#include <cassert>
#include "../include/test.h"

bool save_load_test();
bool valid_move_test();
bool invalid_move_test();
bool scoring_test();

void testing() {
    if (save_load_test()) std::cerr << "Save load test succeeded\n";
    else std::cerr << "Save load test failed\n";
    if (valid_move_test()) std::cerr << "Valid move test succedded\n";
    else std::cerr << "Valid move test failed\n";
    if (invalid_move_test()) std::cerr << "Invalid test succeeded\n";
    else std::cerr << "Invalid test failed\n";
    if (scoring_test()) std::cerr << "Scoring test succeeded\n";
    else std::cerr << "Scoring test failed\n";
}


bool save_load_test() {
    try
    {
        current_board.reset();
        bool flag = current_board.save_game();
        add_move(Move(2,3,'X'));
        flag |= current_board.load_game();
        return current_board.get_state(2,3) == '.' && flag;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Save load test error" << '\n';
        std::cerr << e.what() << '\n';
        return false;
    }
    
}

bool valid_move_test() {
    try
    {
        current_board.reset();
        bool turn = 1;
        for (int i=0;i<BOARD_SIZE;i++) {
            if(!add_move(Move(0,i,turn ? 'X' : 'O'))) std::cerr << i << '\n';
            turn ^= 1;
        }
        turn = 1;
        for (int i=0;i<BOARD_SIZE;i++) {
            char stone = turn ? 'X' : 'O';
            if (current_board.get_state(0,i) != stone) {
                return false;
            }
            turn ^= 1;
        }
        return true;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Valid move test error" << '\n';
        std::cerr << e.what() << '\n';
        return false;
    }   
}

bool invalid_move_test() {
    try
    {
        current_board.reset();
        bool flag = 0,combine_flag = 0;
        // Outside the board test
        flag |= add_move(Move(-1,0,'X'));
        flag |= add_move(Move(0,-1,'X'));
        flag |= add_move(Move(-1,-1,'X'));
        flag |= add_move(Move(19,0,'X'));
        flag |= add_move(Move(0,19,'X'));
        flag |= add_move(Move(19,19,'X'));
        combine_flag |= flag;
        if (flag) std::cerr << "Invalid move subtest - Out of bound: Failed\n";
        
        // Suicide 
        flag = 0;
        add_move(Move(0,1,'X'));
        add_move(Move(0,2,'O'));
        add_move(Move(1,0,'X'));
        flag |= add_move(Move(0,0,'O'));
        combine_flag |= flag;
        if (flag) std::cerr << "Invalid move subtest - Suicide: Failed\n";
        
        current_board.reset();
        
        // Ko rule
        flag = 0;
        add_move(Move(0,1,'X'));
        add_move(Move(1,1,'O'));
        add_move(Move(1,0,'X'));
        add_move(Move(0,2,'O'));
        add_move(Move(3,2,'X'));
        add_move(Move(2,2,'O'));
        add_move(Move(3,1,'X'));
        
        add_move(Move(0,0,'O'));
        flag |= add_move(Move(0,1,'X'));
        combine_flag |= flag;
        if (flag) std::cerr << "Invalid move subtest - Ko rule: Failed\n";
        
        return !flag;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Invalid move test error\n";
        std::cerr << e.what() << '\n';
        return false;
    }
    
}

bool scoring_test() {
    try
    {
        current_board.load_game("data/game1.json");
        std::pair<int,int> result = scoring(current_board);
        bool flag = result == std::pair<int,int>(3,6);
        if (!flag) std::cerr << "Scoring game 1 failed\n";

        current_board.load_game("data/game2.json");
        result = scoring(current_board);
        flag &= result == std::pair<int,int>(8,6);
        if (!flag) std::cerr << "Scoring game 2 failed\n";

        return flag;
    }
    catch(const std::exception& e)
    {
        std::cerr << "Scoring test error\n";
        std::cerr << e.what() << '\n';
        return false;
    }
    
}