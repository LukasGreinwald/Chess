#include "window.hpp"
#include "board.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <stdio.h>

void Window::drawPieces() {
  for (int sq = 0; sq < 64; sq++) {
    if (board.position[sq] != 0) {
      window.draw(pieceSprites[sq]);
    }
  }
}

void Window::drawBoard() {
  for (int row = 0; row < rows; row++) {
    for (int coloumn = 0; coloumn < rows; coloumn++) {
      window.draw(squares[row][coloumn]);
    }
  }
}

void Window::mapSquaresToSize() {
  for (int row = 0; row < rows; row++) {
    for (int coloumn = 0; coloumn < rows; coloumn++) {
      int x = squareShape.width / 8;
      squares[row][coloumn].setSize(sf::Vector2f(x, x));
      squares[row][coloumn].setPosition(sf::Vector2f(
          (squareShape.left + (row * x)), (squareShape.top + (coloumn * x))));
    }
  }
}

void Window::mapPieces() {
  for (int sq = 0; sq < 64; sq++) {
    if (board.position[sq] != 0) {
      int x = sq % 8;
      int y = sq / 8;
      int length = squareShape.width / 8;
      pieceSprites[sq].setPosition(sf::Vector2f(
          squareShape.left + (x * length), squareShape.top + (y * length)));

      pieceSprites[sq].setScale(
          sf::Vector2f(squareShape.width / 3400.f, squareShape.width / 3400.f));
    }
  }
}

void Window::setTextures() {
  for (int sq = 0; sq < 64; sq++) {
    if (board.position[sq] != 0) {
      pieceSprites[sq].setTexture(pieceTextures[board.position[sq]], true);
    }
  }
}

bool Window::loadFen(std::string_view fen) {
  switch (board.fenToBoard(fen)) {
  case Board::FEN_GROUPS_SIZE:
    std::cerr << "FEN_GROUPS_SIZE" << std::endl;
    return false;
  case Board::FEN_FIRST_GROUP:
    std::cerr << "FEN_FIRST_GROUP" << std::endl;
    return false;
  case Board::FEN_SEC_GROUP:
    std::cerr << "FEN_SEC_GROUP" << std::endl;
    return false;
  case Board::FEN_THIRD_GROUP:
    std::cerr << "FEN_THIRD_GROUP" << std::endl;
    return false;
  case Board::FEN_FOURTH_GROUP:
    std::cerr << "FEN_FOURTH_GROUP" << std::endl;
    return false;
  case Board::FEN_FIFTH_GROUP:
    std::cerr << "FEN_FIFTH_GROUP" << std::endl;
    return false;
  case Board::FEN_SIXTH_GROUP:
    std::cerr << "FEN_SIXTH_GROUP" << std::endl;
    return false;
  default:
    return true;
  }
}

Window::Window(int size, const char *name, const char *pieceImg[16]) {
  squareColors[0] = sf::Color(255, 228, 196, 155);
  squareColors[1] = sf::Color(160, 82, 45, 235);
  width = size;
  height = size;
  squareShape.width = size;
  squareShape.height = size;

  for (int row = 0; row < rows; row++) {
    for (int coloumn = 0; coloumn < rows; coloumn++) {
      squares[row][coloumn].setFillColor(squareColors[(row + coloumn) % 2]);
    }
  }

  for (int i = 0; i < 16; i++) {
    if (pieceImg[i]) {
      if (pieceTextures[i].loadFromFile(pieceImg[i])) {
        std::cout << "loaded succesfully at position: " << i << std::endl;
        std::cout << pieceTextures[i].getSize().x
                  << pieceTextures[i].getSize().y << std::endl;
      }
    }
  }
  mapSquaresToSize();
  sf::IntRect blank;
  setTextures();

  mapPieces();
  window.create(sf::VideoMode(size, size), name);
}

bool Window::display() {
  std::cout << "Do you want to add a FEN string? (y/n)" << std::endl;
  char answer;
  std::cin >> answer;

  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  if (answer == 'y') {
    std::string fen;
    std::cout << "Enter the FEN string: ";
    getline(std::cin, fen);
    std::cout << fen << std::endl;
    if (loadFen(fen)) {
      std::cout << "FEN loaded succesfully" << std::endl;
      setTextures();
      mapPieces();

    } else {
      std::cout << "FEN not loaded" << std::endl;
    }
  }
  for (size_t i = 0; i < 8; i++) {
    for (size_t j = 0; j < 8; j++) {
      std::cout << board.position[i * 8 + j] << " ";
    }
    std::cout << std::endl;
  }

  int indexPiece, posX, posY;
  bool isDragging = false;
  bool isInBound = false;
  sf::Vector2f startingPosition;
  sf::Vector2f dragPieceStartPosition;
  bool black = !board.isWhiteToMove;

  while (window.isOpen()) {

    sf::Event event;
    while (window.pollEvent(event)) {

      std::vector<Move> legal = board.generateLegalMoves(black);

      switch (event.type) {
      case sf::Event::Closed:
        window.close();
        return false;

      case sf::Event::Resized:
        width = window.getSize().x;
        height = window.getSize().y;
        window.setView(sf::View(sf::FloatRect(0, 0, width, height)));
        if (width > height) {
          squareShape.width = height;
          squareShape.height = height;
          squareShape.left = (width - squareShape.width) / 2;
          squareShape.top = 0;
        } else {
          squareShape.width = width;
          squareShape.height = width;
          squareShape.top = (height - squareShape.height) / 2;
          squareShape.left = 0;
        }

        mapPieces();
        mapSquaresToSize();
        break;
      case sf::Event::MouseButtonPressed:
        posX = (event.mouseButton.x - squareShape.left) /
               squares[0][0].getSize().x;
        posY =
            (event.mouseButton.y - squareShape.top) / squares[0][0].getSize().x;
        indexPiece = posY * 8 + posX;
        isInBound = posX < 8 && posY < 8 && posX >= 0 && posY >= 0;
        if (board.position[indexPiece] != 0 && isInBound) {
          startingPosition =
              sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
          dragPieceStartPosition = pieceSprites[indexPiece].getPosition();
          isDragging = true;
        }
        break;
      case sf::Event::MouseMoved:
        if (isDragging) {
          sf::Vector2f delta =
              sf::Vector2f(event.mouseMove.x, event.mouseMove.y) -
              startingPosition;
          pieceSprites[indexPiece].setPosition(dragPieceStartPosition + delta);
        }
        break;
      case sf::Event::MouseButtonReleased:
        if (isDragging) {
          int newPosX = (event.mouseButton.x - squareShape.left) /
                        squares[0][0].getSize().x;
          int newPosY = (event.mouseButton.y - squareShape.top) /
                        squares[0][0].getSize().x;
          int newIndex = newPosY * 8 + newPosX;

          isInBound =
              newPosX < 8 && newPosY < 8 && newPosX >= 0 && newPosY >= 0;

          Move movePlayed = Move(indexPiece, newIndex);
          for (size_t i = 0; i < legal.size(); i++) {
            if ((board.position[legal[i].startingSquare] &
                 board.piece.pieceMask) == board.piece.king) {
              std::cout << legal[i].startingSquare << " : "
                        << legal[i].targetSquare << std::endl;
            }
          }

          std::cout << std::endl;

          auto findElement = std::find(legal.begin(), legal.end(), movePlayed);
          bool assertLegalMove = findElement != legal.end();
          if (assertLegalMove) {
            movePlayed = legal[findElement - legal.begin()];
            if (board.makeMove(movePlayed)) {
              setTextures();
              mapPieces();
              black = !black;
            }
          } else {
            pieceSprites[indexPiece].setPosition(dragPieceStartPosition);
          }
          isDragging = false;
        }
        break;

      case sf::Event::KeyPressed:
        if (event.key.code == sf::Keyboard::Left) {
          if (board.unmakeMove()) {
            for (int sq = 0; sq < 64; sq++) {
              if (board.position[sq] != 0) {

                pieceSprites[sq].setTexture(pieceTextures[board.position[sq]],
                                            true);
              }
            }
            mapPieces();
            black = !black;
          }
        }
      default:
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
