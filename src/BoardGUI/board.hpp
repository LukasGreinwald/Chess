#ifndef BOARD_H
#define BOARD_H

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "piece.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <string_view>
#include <ranges>
#include <cctype>
#include <cstring>
#include <charconv>

struct Move
{
    int startingSquare, targetSquare;
    bool qCastle = false;
    bool kCastle = false;

    bool isProm = false;
    bool qProm = false;
    bool bProm = false;
    bool kProm = false;
    bool rProm = false;
    bool enPassant = false;
    int Capture = 0;

    Move(int starting, int target)
    {
        startingSquare = starting;
        targetSquare = target;
    }
    bool operator==(const Move &first)
    {
        bool start = startingSquare == first.startingSquare;
        bool end = targetSquare == first.targetSquare;

        return start && end;
    }

    bool operator!=(const Move &comp)
    {
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
        12, 11, 10, 13, 14, 10, 11, 12};

    Board();
    
    int fiftyMoveRule = 0;
    bool WQueenSideCastlingRights = true;
    bool WKingSideCastlingRights = true;
    bool BQueenSideCastlingRights = true;
    bool BKingSideCastlingRights = true;

    bool isWhiteToMove = true;
    bool isSlidingPiece(int type);
    std::vector<Move> generateLegalMoves(bool black = false);
    std::vector<Move> generateMoves(bool black = false);
    unsigned short calculateSquarestoEdge(int currPosition);

    bool makeMove(Move move);
    bool unmakeMove();
    std::vector<Move> movesPlayed;
    int moveCount;
    int fenToBoard(const std::string_view fenStr);

    static const int FEN_SUCCESS = 0;
    static const int FEN_GROUPS_SIZE = 1;
    static const int FEN_FIRST_GROUP = 2;
    static const int FEN_SEC_GROUP = 3;
    static const int FEN_THIRD_GROUP = 4;
    static const int FEN_FOURTH_GROUP = 5;
    static const int FEN_FIFTH_GROUP = 6;
    static const int FEN_SIXTH_GROUP = 7;
};

#endif