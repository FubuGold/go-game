#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board_State {
private:
    int pos_x;
    int pos_y;
    int cur_val;

public:
    Board_State(int pos_x, int pos_y, int cur_val);

    int Get_pos_x() const {
        return pos_x;
    }

    int Get_pos_y() const {
        return pos_y;
    }

    int Get_cur_val() const {
        return cur_val;
    }
};

class Board {
private:
    std::vector<Board_State> Board_State_Storer; //To store all the current board states
    std::vector<Board_State> Undo_State_Storer;

public:
    Board(){}
    void Board_Reset(); //Reset the entire board

    void Add_Board_State(const Board_State &new_Board_State); //Add new board state

    void Undo_Board_State();

    void Redo_Board_State();
};

#endif

