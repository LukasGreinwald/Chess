#include "piece.h"


Piece::Piece(){ 
}

bool Piece::isWhite(int pc){
    return pc&white;
}

bool Piece::hasDiffColor(int pc1, int pc2){
    return (isWhite(pc1) ^ isWhite(pc2));
}