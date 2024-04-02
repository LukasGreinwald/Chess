#include "board.h"
#include "piece.h"

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <stdio.h>
#include <iostream>
#include <vector>

int const Board::movingOffsets[8] = {-1,1,-8,8,-7,7,-9,9};

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
                    
                    int sideMin = std::min(left, right);
                    int latMin = std::min(top, bottom);
                    bool hitPieceUP = false;
                    bool hitPieceDown = false;
                    bool side = right > left;


                    if(sideMin >= latMin){
                        int osUp = side ? movingOffsets[6] : movingOffsets[4];
                        int osDown = side ? movingOffsets[5] : movingOffsets[7];

                        for(int i = 1; i <= sideMin; i++){
                            int moveIndexUP = sq + i*osUp;
                            int moveIndexDown = sq + i*osDown;
                            Move moveUp = Move(sq, moveIndexUP);
                            Move moveDown = Move(sq, moveIndexDown);

                            if(!hitPieceUP){
                                if(position[moveIndexUP] == 0){
                                some.push_back(moveUp);
                                }else{
                                    if(piece.hasDiffColor(curr, position[moveIndexUP])){
                                        some.push_back(moveUp);
                                    }
                                    hitPieceUP = true;
                                }
                            }
                            if(!hitPieceDown){
                                if(position[moveIndexDown] == 0){
                                    some.push_back(moveDown);
                                }else{
                                    if(piece.hasDiffColor(curr, position[moveIndexDown])){
                                        some.push_back(moveDown);
                                    }
                                    hitPieceDown = true;
                                }
                            }
                        }

                        osUp = side ? movingOffsets[4] : movingOffsets[6];
                        osDown = side ?  movingOffsets[7] : movingOffsets[5];
                        
                        for(int i = 1; i <= top; i++){
                            int moveIndexUP = sq + i*osUp;
                            Move move = Move(sq, moveIndexUP);
                            if(position[moveIndexUP] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndexUP] )){
                                    some.push_back(move);
                            }
                            break;
                            }
                            some.push_back(move);
                        }

                        for(int i = 1; i <= bottom; i++){
                            int moveIndexDown = sq + i*osDown;
                            Move move = Move(sq, moveIndexDown);
                            if(position[moveIndexDown] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndexDown] )){
                                    some.push_back(move);
                            }
                            break;
                            }
                            some.push_back(move);
                        }
                          
                    }else{
                        side = bottom > top;
                        int osUp = side ? movingOffsets[6] : movingOffsets[4];
                        int osDown = side ? movingOffsets[5] : movingOffsets[7];

                        for(int i = 1; i <= latMin; i++){
                            int moveIndexUP = sq + i*osUp;
                            int moveIndexDown = sq + i*osDown;
                            Move moveUp = Move(sq, moveIndexUP);
                            Move moveDown = Move(sq, moveIndexDown);

                            if(!hitPieceUP){
                                if(position[moveIndexUP] == 0){
                                some.push_back(moveUp);
                                }else{
                                    if(piece.hasDiffColor(curr, position[moveIndexUP])){
                                        some.push_back(moveUp);
                                    }
                                    hitPieceUP = true;
                                }
                            }
                            if(!hitPieceDown){
                                if(position[moveIndexDown] == 0){
                                    some.push_back(moveDown);
                                }else{
                                    if(piece.hasDiffColor(curr, position[moveIndexDown])){
                                        some.push_back(moveDown);
                                    }
                                    hitPieceDown = true;
                                }
                            }
                        }

                        osUp = side ? movingOffsets[5] : movingOffsets[6];
                        osDown = side ?  movingOffsets[7] : movingOffsets[4];
                        
                        for(int i = 1; i <= left; i++){
                            int moveIndexUP = sq + i*osUp;
                            Move move = Move(sq, moveIndexUP);
                            if(position[moveIndexUP] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndexUP])){
                                    some.push_back(move);
                            }
                            break;
                            }
                            some.push_back(move);
                        }

                        for(int i = 1; i <= right; i++){
                            int moveIndexDown = sq + i*osDown;
                            Move move = Move(sq, moveIndexDown);
                            if(position[moveIndexDown] != 0){
                                if(piece.hasDiffColor(curr, position[moveIndexDown] )){
                                    some.push_back(move);
                            }
                            break;
                            }
                            some.push_back(move);
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



