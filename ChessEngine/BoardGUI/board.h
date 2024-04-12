#ifndef BOARD_H
#define BOARD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "piece.h"
#include <stdio.h>
#include <iostream>
#include <vector>


struct Move{
    int startingSquare, targetSquare;
    bool qCastle = false;
    bool kCastle = false;

    bool isProm = false;
    bool qProm = false;
    bool bProm = false;
    bool kProm = false;
    bool rProm = false;
    int Capture = 0;

    Move(int starting, int target){
        startingSquare = starting;
        targetSquare = target;
    }
    bool operator==(const Move& first){
        bool start = startingSquare == first.startingSquare;
        bool end = targetSquare == first.targetSquare;

        return start && end;
    }

    bool operator!=(const Move& comp){
        bool start = startingSquare != comp.startingSquare;
        bool end = targetSquare != comp.targetSquare;

        return start || end;
    }
};
class Board
{

public:
    Piece piece;
    static const int movingOffsets[8];
    static const int knightOffsets[8];
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

    
    Board();

    bool WQueenSideCastlingRights = true;
    bool WKingSideCastlingRights = true;
    bool BQueenSideCastlingRights = true;
    bool BKingSideCastlingRights = true;


    bool isSlidingPiece(int type);
    std::vector<Move> generateLegalMoves(bool black = false);
    std::vector<Move> generateMoves(bool black = false);
    unsigned short calculateSquarestoEdge(int currPosition);

    bool makeMove(Move move);
    bool unmakeMove();
    std::vector<Move> movesPlayed;
    int moveCount;
    bool fenToBoard(char* fenString, int *position);
    
};

#endif