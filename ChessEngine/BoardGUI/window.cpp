#include <SFML/Graphics.hpp>
#include "window.h"
#include <SFML/System.hpp>
#include "board.h"
#include <stdio.h>
#include <iostream>


void Window::drawPieces(){
    for(int sq = 0; sq < 64; sq++){
        if(board.position[sq] != 0){
            window.draw(pieceSprites[sq]);
        }
    }
    
}

void Window::drawBoard(){
    for(int row = 0; row < rows; row++){
        for(int coloumn = 0; coloumn < rows; coloumn++){
            window.draw(squares[row][coloumn]);
        }
    }
}

void Window::mapSquaresToSize(){
    for(int row = 0; row < rows; row++){
        for(int coloumn = 0; coloumn < rows; coloumn++){
            int x = squareShape.width/8;
            squares[row][coloumn].setSize(sf::Vector2f(x, x));
            squares[row][coloumn].setPosition(sf::Vector2f((squareShape.left +(row * x)), (squareShape.top + (coloumn * x))));
        }
    }
}

void Window::mapPieces(){
    for(int sq = 0; sq < 64; sq++){
        if(board.position[sq] != 0){
            int x = sq%8;
            int y = sq/8;
            int length = squareShape.width/8;
            pieceSprites[sq].setPosition(sf::Vector2f(squareShape.left+ (x*length), squareShape.top + (y*length)));
            std::cout << pieceSprites[sq].getScale().x <<" "<< pieceSprites[sq].getScale().y << std::endl;
            pieceSprites[sq].setScale(sf::Vector2f(squareShape.width/3400.f, squareShape.width/3400.f));
        
        }
    }
}


Window::Window(int size,const char *name,const char *pieceImg[16])
{
    squareColors[0] = sf::Color(255,228,196, 155);
    squareColors[1] = sf::Color(160,82,45, 235);
    width = size;
    height = size;
    squareShape.width = size; 
    squareShape.height = size;


    for(int row = 0; row < rows; row++){
        for(int coloumn = 0; coloumn < rows; coloumn++){
            squares[row][coloumn].setFillColor(squareColors[(row+coloumn)%2]);
        }
    }

    for(int i = 0; i<16; i++){
        if(pieceImg[i]){
            if(pieceTextures[i].loadFromFile(pieceImg[i])){
                std::cout << "loaded succesfully at position: "<< i << std::endl;
                std::cout << pieceTextures[i].getSize().x << pieceTextures[i].getSize().y << std::endl;
                
            }
        }
    }
    mapSquaresToSize();
    sf::IntRect blank;
    for(int sq = 0; sq < 64; sq++){
    if(board.position[sq] != 0){
          
         pieceSprites[sq].setTexture(pieceTextures[board.position[sq]], true);
    }
    }

    mapPieces();
    window.create(sf::VideoMode(size, size), name);
}


bool Window::display(){
    int indexPiece, posX, posY;
    bool isDragging = false;
    sf::Vector2f startingPosition;
    sf::Vector2f dragPieceStartPosition;
    while(window.isOpen()){
        

        sf::Event event;
        while(window.pollEvent(event)){

            switch(event.type){
                case sf::Event::Closed:
                    window.close();
                    return false;


                case sf::Event::Resized:
                    width = window.getSize().x;
                    height = window.getSize().y;
                    window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
                    if(width > height){
                        squareShape.width = height;
                        squareShape.height = height;
                        squareShape.left = (width - squareShape.width)/2;
                        squareShape.top = 0;
                    }else{
                        squareShape.width = width ;
                        squareShape.height = width;
                        squareShape.top = (height - squareShape.height)/2;
                        squareShape.left = 0;
                    }
                    
                    mapPieces();
                    mapSquaresToSize();
                    break;
                case sf::Event::MouseButtonPressed:
                    posX = (event.mouseButton.x-squareShape.left)/squares[0][0].getSize().x;
                    posY = (event.mouseButton.y-squareShape.top)/squares[0][0].getSize().x;
                    indexPiece = posY*8 + posX;
                     
                    if(board.position[indexPiece] != 0){
                        startingPosition = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                        dragPieceStartPosition = pieceSprites[indexPiece].getPosition();
                        isDragging = true;
                    }
                    break;
                case sf::Event::MouseMoved:   
                    if(isDragging){
                        sf::Vector2f delta = sf::Vector2f(event.mouseMove.x, event.mouseMove.y) - startingPosition;
                        pieceSprites[indexPiece].setPosition(dragPieceStartPosition + delta);
                    } 
                    break;
                case sf::Event::MouseButtonReleased:
                    if(isDragging){
                        int newPosX = (event.mouseButton.x-squareShape.left) / squares[0][0].getSize().x;
                        int newPosY = (event.mouseButton.y-squareShape.top) / squares[0][0].getSize().x;
                        int newIndex = newPosY * 8 + newPosX;

                        std::cout << newPosX << ", " << newPosY << ", "<< newIndex << std::endl;
                        if(newIndex != indexPiece){
                            pieceSprites[newIndex] = pieceSprites[indexPiece];
                            pieceSprites[newIndex].setPosition(sf::Vector2f(squareShape.left + newPosX * squares[0][0].getSize().x, squareShape.top + newPosY * squares[0][0].getSize().x));
                            board.position[newIndex] = board.position[indexPiece];
                            board.position[indexPiece] = 0;
                        }else{
                            pieceSprites[indexPiece].setPosition(dragPieceStartPosition);
                        }
                        isDragging = false;
                    }
                    break;

            }
        }

        window.clear(sf::Color::White);
        drawBoard();
        drawPieces();
    
        window.display();
        

    }

    return true;
}

