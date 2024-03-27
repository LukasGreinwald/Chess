#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <stdio.h>
#include <SFML/System.hpp>
#include "window.h"

const char *pieceImg[16] = {
    nullptr,
    "./res/pieces/bp.png",          //blackPawn
    "./res/pieces/bb.png",          //blackBishop
    "./res/pieces/bn.png",          //blackKnight   
    "./res/pieces/br.png",          //blackRook 
    "./res/pieces/bq.png",          //blackQueen
    "./res/pieces/bk.png",          //blackKing
    nullptr,
    nullptr,
    "./res/pieces/wp.png",          //whitePawn
    "./res/pieces/wb.png",          //whiteBishop
    "./res/pieces/wn.png",          //whiteKnight
    "./res/pieces/wr.png",          //whiteRook
    "./res/pieces/wq.png",          //whiteQueen
    "./res/pieces/wk.png",          //whiteKing
    nullptr
};


int main(){
    Window win(640, "ChessWindow", pieceImg);
    win.display();

    return 0;
}