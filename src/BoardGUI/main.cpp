#include "window.hpp"
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <stdio.h>

const char *pieceImg[16] = {nullptr,
                            "./src/res/pieces/bp.png", // blackPawn
                            "./src/res/pieces/bb.png", // blackBishop
                            "./src/res/pieces/bn.png", // blackKnight
                            "./src/res/pieces/br.png", // blackRook
                            "./src/res/pieces/bq.png", // blackQueen
                            "./src/res/pieces/bk.png", // blackKing
                            nullptr,
                            nullptr,
                            "./src/res/pieces/wp.png", // whitePawn
                            "./src/res/pieces/wb.png", // whiteBishop
                            "./src/res/pieces/wn.png", // whiteKnight
                            "./src/res/pieces/wr.png", // whiteRook
                            "./src/res/pieces/wq.png", // whiteQueen
                            "./src/res/pieces/wk.png", // whiteKing
                            nullptr};

int main() {
  Window win(640, "ChessWindow", pieceImg);
  win.display();

  return 0;
}
