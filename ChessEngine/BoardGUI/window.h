#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include "board.h"


class Window
{
private:
    const int rows = 8;
    sf::RenderWindow window;
    sf::RectangleShape squares[8][8];
    sf::IntRect squareShape;
    sf::Color squareColors[2];
    Board board;
    int width, height;
    sf::Sprite pieceSprites[64];
    sf::Texture pieceTextures[16];



    void drawBoard();
    void mapSquaresToSize();
    void drawPieces();
    void mapPieces();
    
public:
    Window(int size, const char *name, const char *pieceImg[16]);
    bool display();
};

#endif


