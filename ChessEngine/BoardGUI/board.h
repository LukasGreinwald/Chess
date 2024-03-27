#ifndef CHESS_BOARD_H
#define CHESS_BOARD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>


class ChessBoard
{

public:
    
    int position[64] = {
        4, 3, 2, 5, 6, 2, 3, 4,
        1, 1, 1, 1, 1, 1, 1, 1,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        9, 9, 9, 9, 9, 9, 9, 9,
        12,11,10,13,14,10,11,12
    };

    
    ChessBoard(){};




    bool fenToBoard(char* fenString, int *position){
        return false;
    }
    
};

#endif