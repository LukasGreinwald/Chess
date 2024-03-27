#ifndef PIECE_H
#define PIECE_H
#include <stdio.h>
#include <iostream>
#include <vector>



class Piece
{
    
public:
    static const int empty = 0;
    static const int pawn = 1;
    static const int bishop = 2;
    static const int knight = 3;
    static const int rook = 4;
    static const int queen = 5;
    static const int king = 6;
    
    
    static const int white = 0b1000;

    static const int blackPawn = pawn | 0;         //1
    static const int blackBishop = bishop | 0;     //2
    static const int blackKnight = knight | 0;     //3
    static const int blackRook = rook | 0;         //4
    static const int blackQueen = queen | 0;       //5
    static const int blackKing = king | 0;         //6


    static const int whitePawn = pawn | white;     //9
    static const int whiteBishop = bishop | white; //10
    static const int whiteKnight = knight | white; //11
    static const int whiteRook = rook | white;     //12
    static const int whiteQueen = queen | white;   //13
    static const int whiteKing = king | white;     //14



    Piece();
    bool isWhite(int pc);
    bool hasDiffColor(int pc1, int pc2);
    
};


#endif


