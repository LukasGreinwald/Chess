#include "board.h"
#include "piece.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

int const Board::movingOffsets[8] = {-1,1,-8,8,-7,7,-9,9};
int const Board::knightOffsets[8] = {-10, 10, 6, -6, -15, 15, 17, -17};

unsigned short Board::calculateSquarestoEdge(int currPosition){       //XXXXBBBTTTRRRLLL
    unsigned short left,right, top, bottom, value;

    left = currPosition%8;

    right = 7-left;
    right <<= 3;

    top = currPosition/8;
    bottom = 7-top;
    top <<= 6;

    
    
    bottom <<= 9;
    value = left | right | top | bottom;
    
    return value;
}


std::vector<Move> Board::generateLegalMoves(bool black){
    std::vector<Move> some;
    for(int sq = 0; sq < 64; sq++){
        int curr = position[sq];
        if(curr != 0 && (piece.isWhite(curr) != black)){
            unsigned short borders = calculateSquarestoEdge(sq);
            unsigned short bordersMask = 0b111;

            int left = borders&bordersMask;
            borders >>= 3;
            
            int right = borders&bordersMask;
            borders >>= 3;
            
            int top = borders&bordersMask;
            borders >>= 3;

            int bottom = borders&bordersMask;
            
            
            if((curr&piece.pieceMask) == piece.king){
                for(int i : movingOffsets){
                    bool isAtBorder = ((top == 0) && (i == -7 || i == -8 || i == -9)) 
                                        || ((bottom == 0) && (i == 7 || i == 8 || i == 9))
                                        || ((left == 0) && (i == -9 || i == -1 || i == 7)) 
                                        || ((right == 0) && (i == 9 || i == 1 || i == -7));
                    int moveIndex = sq + i;
                    Move move = Move(sq, moveIndex);

                    if(!isAtBorder &&(position[moveIndex] == 0 || (piece.hasDiffColor(curr, position[moveIndex])))){
                        some.push_back(move);
                    }
                }
            }

            if((curr&piece.pieceMask) == piece.knight){
                for(int i : knightOffsets){
                    bool isAtBorder = ((top == 0) && (i < 0)) 
                                        || ((top == 1) && (i < -10))
                                        || ((bottom == 0) && (i > 0))
                                        || ((bottom == 1) && (i > 10))
                                        || ((left == 0) && (i == -10 || i == 6 || i == 15 || i == -17)) 
                                        || ((left == 1) && (i == -10 || i == 6))
                                        || ((right == 0) && (i == 10 || i == -6 || i == -15 || i == 17))
                                        || ((right == 1) && (i == 10 || i == -6));
                    int moveIndex = sq + i;
                    Move move = Move(sq, moveIndex);

                    if(!isAtBorder &&(position[moveIndex] == 0 || (piece.hasDiffColor(curr, position[moveIndex])))){
                        some.push_back(move);
                    }
                }
            }
            if(isSlidingPiece(curr)){
                
                if((curr&piece.pieceMask) != piece.bishop){
                    
                    for(int i = 1; i <= left; i++){
                        
                        int moveIndex = sq + i*movingOffsets[0];
                        Move move = Move(sq, moveIndex);

                        
                        if(position[moveIndex] != 0){
                            if(piece.hasDiffColor(curr, position[moveIndex] )){
                                some.push_back(move);
                            }
                            break;
                        }
                        
                        some.push_back(move);
                    }

                    for(int i = 1; i <= right; i++){
                        int moveIndex = sq + i*movingOffsets[1];
                        Move move = Move(sq, moveIndex);
                        if(position[moveIndex] != 0){
                            if(piece.hasDiffColor(curr, position[moveIndex] )){
                                some.push_back(move);
                            }
                            break;
                        }
                        some.push_back(move);
                    }

                    for(int i = 1; i <= top; i++){
                        int moveIndex = sq + i*movingOffsets[2];
                        Move move = Move(sq, moveIndex);
                        if(position[moveIndex] != 0){
                            if(piece.hasDiffColor(curr, position[moveIndex] )){
                                some.push_back(move);
                            }
                            break;
                        }
                        some.push_back(move);
                    }   

                    for(int i = 1; i <= bottom; i++){
                        int moveIndex = sq + i*movingOffsets[3];
                        Move move = Move(sq, moveIndex);
                        if(position[moveIndex] != 0){
                            if(piece.hasDiffColor(curr, position[moveIndex] )){
                                some.push_back(move);
                            }
                            break;
                        }
                        some.push_back(move);
                    }


                }
                
                if((curr&piece.pieceMask) != piece.rook){
                    int maxDistL = std::max(top, bottom);
                    int maxDistV = std::max(left, right);
                    int maxDist = std::max(maxDistL, maxDistV);

                    

                    if(right && top){
                        for(int i = 1; i <= maxDist; i++){                  //go right up
                            int moveIndex = sq + i*movingOffsets[4];
                            Move move = Move(sq, moveIndex);
                            if(position[moveIndex] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndex] )){
                                    some.push_back(move);
                                }
                                break;
                            }
                            some.push_back(move);

                            if((moveIndex%8 == 7) || (moveIndex/8 == 0)){
                                break;
                            }
                        }
                    }
                    if(right && bottom){
                        for(int i = 1; i <= maxDist; i++){                  //go right down
                            int moveIndex = sq + i*movingOffsets[7];
                            Move move = Move(sq, moveIndex);
                            if(position[moveIndex] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndex])){
                                    some.push_back(move);
                                }
                                break;
                            }
                            some.push_back(move);

                            if((moveIndex%8 == 7) || (moveIndex/8 == 7)){
                                break;
                            }
                        }
                    }
                    if(left && bottom){
                        for(int i = 1; i <= maxDist; i++){                  //go left down
                            int moveIndex = sq + i*movingOffsets[5];
                            Move move = Move(sq, moveIndex);
                            if(position[moveIndex] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndex] )){
                                    some.push_back(move);
                                }
                                break;
                            }
                            some.push_back(move);

                            if((moveIndex%8 == 0) || (moveIndex/8 == 7)){
                                break;
                            }
                        }
                    }

                    if(left && top){
                        for(int i = 1; i <= maxDist; i++){                  //go left up
                            int moveIndex = sq + i*movingOffsets[6];
                            Move move = Move(sq, moveIndex);
                            if(position[moveIndex] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndex] )){
                                    some.push_back(move);
                                }
                                break;
                            }
                            some.push_back(move);

                            if((moveIndex%8 == 7) || (moveIndex/8 == 0)){
                                break;
                            }
                        }
                    }

                }
            }
        }
        
    }
    return some;
}



bool Board::isSlidingPiece(int movingPiece){
    
    if(((movingPiece&piece.pieceMask) == piece.bishop) || ((movingPiece&piece.pieceMask) == piece.rook) || ((movingPiece&piece.pieceMask) == piece.queen)){
        return true;
    }else{
        return false;
    }
}

bool Board::fenToBoard(char* fenString, int *position){
    return false;
}

Board::Board(){
   
    
}



