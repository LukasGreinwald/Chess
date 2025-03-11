#include "board.hpp"
#include <ranges>
int const Board::movingOffsets[8] = {-1, 1, -8, 8, -7, 7, -9, 9};
int const Board::knightOffsets[8] = {-10, 10, 6, -6, -15, 15, 17, -17};

unsigned short
Board::calculateSquarestoEdge(int currPosition) { // XXXXBBBTTTRRRLLL
  unsigned short left, right, top, bottom, value;

  left = currPosition % 8;

  right = 7 - left;
  right <<= 3;

  top = currPosition / 8;
  bottom = 7 - top;
  top <<= 6;

  bottom <<= 9;
  value = left | right | top | bottom;

  return value;
}

std::vector<Move> Board::generateMoves(bool black) {
  std::vector<Move> some;
  for (int sq = 0; sq < 64; sq++) {
    int curr = position[sq];
    if (curr != 0 && (piece.isWhite(curr) != black)) {
      unsigned short borders = calculateSquarestoEdge(sq);
      unsigned short bordersMask = 0b111;

      int left = borders & bordersMask;
      borders >>= 3;

      int right = borders & bordersMask;
      borders >>= 3;

      int top = borders & bordersMask;
      borders >>= 3;

      int bottom = borders & bordersMask;

      if ((curr & piece.pieceMask) == piece.pawn) {
        if (black) {
          int moveIndex = sq + 8;

          if (position[moveIndex] == 0) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 7) {
              move.isProm = true;
            }
            some.push_back(move);
          }
          moveIndex = sq + 7;

          if ((left != 0) && (position[moveIndex] != 0) &&
              piece.isWhite(position[moveIndex])) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 7) {
              move.isProm = true;
            }
            move.Capture = position[moveIndex];
            some.push_back(move);
          }

          moveIndex = sq + 9;

          if ((right != 0) && (position[moveIndex] != 0) &&
              piece.isWhite(position[moveIndex])) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 7) {
              move.isProm = true;
            }
            move.Capture = position[moveIndex];
            some.push_back(move);
          }
          moveIndex = sq + 16;

          if (top == 1) {
            if ((position[moveIndex] == 0) && (position[sq + 8] == 0)) {
              Move move = Move(sq, moveIndex);
              some.push_back(move);
            }
          }

          if (movesPlayed.size() != 0) {
            Move last = movesPlayed[movesPlayed.size() - 1];
            bool checkCondition =
                (position[last.targetSquare] == piece.whitePawn) &&
                (std::abs(last.targetSquare - last.startingSquare) == 16);

            bool leftEnPassant =
                ((sq % 8) != 0) && ((sq - 1) == last.targetSquare);
            bool rightEnPassant =
                ((sq % 8) != 7) && ((sq + 1) == last.targetSquare);

            if (checkCondition) {
              if (leftEnPassant) {
                std::cout << "left en passant" << std::endl;
                Move move = Move(sq, (sq + 7));
                move.enPassant = true;

                some.push_back(move);
              }
              if (rightEnPassant) {
                std::cout << "right en passant" << std::endl;
                Move move = Move(sq, (sq + 9));
                move.enPassant = true;

                some.push_back(move);
              }
            }
          }
        } else {
          int moveIndex = sq - 8;

          if (position[moveIndex] == 0) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 0) {
              move.isProm = true;
            }
            some.push_back(move);
          }
          moveIndex = sq - 7;

          if ((right != 0) && (position[moveIndex] != 0) &&
              !piece.isWhite(position[moveIndex])) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 0) {
              move.isProm = true;
            }
            move.Capture = position[moveIndex];
            some.push_back(move);
          }

          moveIndex = sq - 9;

          if ((left != 0) && (position[moveIndex] != 0) &&
              !piece.isWhite(position[moveIndex])) {
            Move move = Move(sq, moveIndex);
            if ((moveIndex / 8) == 0) {
              move.isProm = true;
            }
            move.Capture = position[moveIndex];
            some.push_back(move);
          }
          moveIndex = sq - 16;

          if (bottom == 1) {
            if ((position[moveIndex] == 0) && (position[sq - 8] == 0)) {
              Move move = Move(sq, moveIndex);
              some.push_back(move);
            }
          }

          if (movesPlayed.size() != 0) {
            Move last = movesPlayed[movesPlayed.size() - 1];
            bool checkCondition =
                (position[last.targetSquare] == piece.blackPawn) &&
                (std::abs(last.targetSquare - last.startingSquare) == 16);

            bool leftEnPassant =
                ((sq % 8) != 0) && ((sq - 1) == last.targetSquare);
            bool rightEnPassant =
                ((sq % 8) != 7) && ((sq + 1) == last.targetSquare);

            if (checkCondition) {
              if (leftEnPassant) {
                Move move = Move(sq, (sq - 9));
                move.enPassant = true;

                some.push_back(move);
              }
              if (rightEnPassant) {
                Move move = Move(sq, (sq - 7));
                move.enPassant = true;

                some.push_back(move);
              }
            }
          }
        }
      }

      if ((curr & piece.pieceMask) == piece.king) {
        for (int i : movingOffsets) {
          bool isAtBorder = ((top == 0) && (i == -7 || i == -8 || i == -9)) ||
                            ((bottom == 0) && (i == 7 || i == 8 || i == 9)) ||
                            ((left == 0) && (i == -9 || i == -1 || i == 7)) ||
                            ((right == 0) && (i == 9 || i == 1 || i == -7));
          int moveIndex = sq + i;
          Move move = Move(sq, moveIndex);

          if (!isAtBorder &&
              (position[moveIndex] == 0 ||
               (piece.hasDiffColor(curr, position[moveIndex])))) {
            move.Capture = position[moveIndex];
            some.push_back(move);
          }
        }
        if (black) {
          if (BKingSideCastlingRights) {
            if ((position[sq + 1] == 0) && (position[sq + 2] == 0)) {
              Move move = Move(sq, sq + 2);
              move.kCastle = true;
              some.push_back(move);
            }
          }
          if (BQueenSideCastlingRights) {
            if ((position[sq - 1] == 0) && (position[sq - 2] == 0) &&
                (position[sq - 3] == 0)) {
              Move move = Move(sq, sq - 2);
              move.qCastle = true;
              some.push_back(move);
            }
          }
        } else {
          if (WKingSideCastlingRights) {
            if ((position[sq + 1] == 0) && (position[sq + 2] == 0)) {
              Move move = Move(sq, sq + 2);
              move.kCastle = true;
              some.push_back(move);
            }
          }
          if (WQueenSideCastlingRights) {
            if ((position[sq - 1] == 0) && (position[sq - 2] == 0) &&
                (position[sq - 3] == 0)) {
              Move move = Move(sq, sq - 2);
              move.qCastle = true;
              some.push_back(move);
            }
          }
        }
      }

      if ((curr & piece.pieceMask) == piece.knight) {
        for (int i : knightOffsets) {
          bool isAtBorder =
              ((top == 0) && (i < 0)) || ((top == 1) && (i < -10)) ||
              ((bottom == 0) && (i > 0)) || ((bottom == 1) && (i > 10)) ||
              ((left == 0) && (i == -10 || i == 6 || i == 15 || i == -17)) ||
              ((left == 1) && (i == -10 || i == 6)) ||
              ((right == 0) && (i == 10 || i == -6 || i == -15 || i == 17)) ||
              ((right == 1) && (i == 10 || i == -6));
          int moveIndex = sq + i;
          Move move = Move(sq, moveIndex);

          if (!isAtBorder &&
              (position[moveIndex] == 0 ||
               (piece.hasDiffColor(curr, position[moveIndex])))) {
            move.Capture = position[moveIndex];
            some.push_back(move);
          }
        }
      }
      if (isSlidingPiece(curr)) {

        if ((curr & piece.pieceMask) != piece.bishop) {

          for (int i = 1; i <= left; i++) {

            int moveIndex = sq + i * movingOffsets[0];
            Move move = Move(sq, moveIndex);

            if (position[moveIndex] != 0) {
              if (piece.hasDiffColor(curr, position[moveIndex])) {
                move.Capture = position[moveIndex];
                some.push_back(move);
              }
              break;
            }

            some.push_back(move);
          }

          for (int i = 1; i <= right; i++) {
            int moveIndex = sq + i * movingOffsets[1];
            Move move = Move(sq, moveIndex);
            if (position[moveIndex] != 0) {
              if (piece.hasDiffColor(curr, position[moveIndex])) {
                move.Capture = position[moveIndex];
                some.push_back(move);
              }
              break;
            }
            some.push_back(move);
          }

          for (int i = 1; i <= top; i++) {
            int moveIndex = sq + i * movingOffsets[2];
            Move move = Move(sq, moveIndex);
            if (position[moveIndex] != 0) {
              if (piece.hasDiffColor(curr, position[moveIndex])) {
                move.Capture = position[moveIndex];
                some.push_back(move);
              }
              break;
            }
            some.push_back(move);
          }

          for (int i = 1; i <= bottom; i++) {
            int moveIndex = sq + i * movingOffsets[3];
            Move move = Move(sq, moveIndex);
            if (position[moveIndex] != 0) {
              if (piece.hasDiffColor(curr, position[moveIndex])) {
                move.Capture = position[moveIndex];
                some.push_back(move);
              }
              break;
            }
            some.push_back(move);
          }
        }

        if ((curr & piece.pieceMask) != piece.rook) {
          int maxDistL = std::max(top, bottom);
          int maxDistV = std::max(left, right);
          int maxDist = std::max(maxDistL, maxDistV);

          if (right && top) {
            for (int i = 1; i <= maxDist; i++) { // go right up
              int moveIndex = sq + i * movingOffsets[4];
              Move move = Move(sq, moveIndex);
              move.Capture = position[moveIndex];
              if (position[moveIndex] != 0) {
                if (piece.hasDiffColor(curr, position[moveIndex])) {

                  some.push_back(move);
                }
                break;
              }
              some.push_back(move);

              if ((moveIndex % 8 == 7) || (moveIndex / 8 == 0)) {
                break;
              }
            }
          }
          if (right && bottom) {
            for (int i = 1; i <= maxDist; i++) { // go right down
              int moveIndex = sq + i * movingOffsets[7];
              Move move = Move(sq, moveIndex);
              move.Capture = position[moveIndex];
              if (position[moveIndex] != 0) {
                if (piece.hasDiffColor(curr, position[moveIndex])) {

                  some.push_back(move);
                }
                break;
              }
              some.push_back(move);

              if ((moveIndex % 8 == 7) || (moveIndex / 8 == 7)) {
                break;
              }
            }
          }
          if (left && bottom) {
            for (int i = 1; i <= maxDist; i++) { // go left down
              int moveIndex = sq + i * movingOffsets[5];
              Move move = Move(sq, moveIndex);
              move.Capture = position[moveIndex];
              if (position[moveIndex] != 0) {
                if (piece.hasDiffColor(curr, position[moveIndex])) {

                  some.push_back(move);
                }
                break;
              }
              some.push_back(move);

              if ((moveIndex % 8 == 0) || (moveIndex / 8 == 7)) {
                break;
              }
            }
          }

          if (left && top) {
            for (int i = 1; i <= maxDist; i++) { // go left up
              int moveIndex = sq + i * movingOffsets[6];
              Move move = Move(sq, moveIndex);
              move.Capture = position[moveIndex];
              if (position[moveIndex] != 0) {
                if (piece.hasDiffColor(curr, position[moveIndex])) {
                  some.push_back(move);
                }
                break;
              }
              some.push_back(move);

              if ((moveIndex % 8 == 0) || (moveIndex / 8 == 0)) {
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

std::vector<Move> Board::generateLegalMoves(bool black) {
  std::vector<Move> moves = generateMoves(black);
  for (size_t i = 0; i < moves.size(); ++i) {
    makeMove(moves[i]);

    std::vector<Move> enemyMoves = generateMoves(!black);
    bool kingCaptured = false;

    int kingSquare = black ? 4 : 60;
    int kcSquare = black ? 5 : 61;

    int qcSquare1 = black ? 3 : 59;
    int qcSquare2 = black ? 2 : 58;

    for (const Move &enmv : enemyMoves) {
      if ((position[enmv.targetSquare] & piece.pieceMask) == piece.king) {
        kingCaptured = true;
        break;
      }

      if ((moves[i].kCastle || moves[i].qCastle) &&
          (enmv.targetSquare == kingSquare)) {
        kingCaptured = true;
        break;
      }
      if ((moves[i].kCastle) && (enmv.targetSquare == kcSquare)) {
        kingCaptured = true;
        break;
      }
      if ((moves[i].qCastle) &&
          (enmv.targetSquare == qcSquare1 || enmv.targetSquare == qcSquare2)) {
        kingCaptured = true;
        break;
      }
    }

    if (kingCaptured) {
      // Erase the move that leads to the king being captured
      moves.erase(moves.begin() + i);
      // Adjust the index to account for the erased element
      --i;
    }

    unmakeMove();
  }

  return moves;
}

bool Board::makeMove(Move move) {
  bool black = !piece.isWhite(position[move.startingSquare]);
  position[move.targetSquare] = position[move.startingSquare];
  position[move.startingSquare] = 0;
  movesPlayed.push_back(move);

  if (move.qCastle) {
    position[black ? 3 : 59] = black ? piece.blackRook : piece.whiteRook;
    position[black ? 0 : 56] = 0;
    BQueenSideCastlingRights = black ? false : BQueenSideCastlingRights;
    WQueenSideCastlingRights = black ? WQueenSideCastlingRights : false;
  }
  if (move.kCastle) {
    position[black ? 5 : 61] = black ? piece.blackRook : piece.whiteRook;
    position[black ? 7 : 63] = 0;
    BKingSideCastlingRights = black ? false : BKingSideCastlingRights;
    WKingSideCastlingRights = black ? WKingSideCastlingRights : false;
  }
  if (move.isProm) {
    position[move.targetSquare] = black ? piece.blackQueen : piece.whiteQueen;
  }

  if (move.enPassant) {
    position[black ? (move.targetSquare - 8) : (move.targetSquare + 8)] = 0;
  }
  return true;
}

bool Board::unmakeMove() {

  if (movesPlayed.size() == 0) {
    return false;
  }
  Move play = movesPlayed[movesPlayed.size() - 1];
  bool black = !piece.isWhite(position[play.targetSquare]);
  movesPlayed.pop_back();

  int pc = position[play.targetSquare];
  position[play.targetSquare] = play.Capture;

  if (play.isProm) {
    position[play.startingSquare] = black ? piece.blackPawn : piece.whitePawn;
  } else {
    position[play.startingSquare] = pc;
  }

  if (play.qCastle) {
    position[black ? 3 : 59] = 0;
    position[black ? 0 : 56] = black ? piece.blackRook : piece.whiteRook;

    WQueenSideCastlingRights = black ? WQueenSideCastlingRights : true;
    BQueenSideCastlingRights = black ? true : BKingSideCastlingRights;
  }
  if (play.kCastle) {
    position[black ? 5 : 61] = 0;
    position[black ? 7 : 63] = black ? piece.blackRook : piece.whiteRook;

    WKingSideCastlingRights = black ? WKingSideCastlingRights : true;
    BKingSideCastlingRights = black ? true : BKingSideCastlingRights;
  }

  if (play.enPassant) {
    position[black ? (play.targetSquare - 8) : (play.targetSquare + 8)] =
        black ? piece.whitePawn : piece.blackPawn;
  }

  return true;
}

bool Board::isSlidingPiece(int movingPiece) {

  if (((movingPiece & piece.pieceMask) == piece.bishop) ||
      ((movingPiece & piece.pieceMask) == piece.rook) ||
      ((movingPiece & piece.pieceMask) == piece.queen)) {
    return true;
  } else {
    return false;
  }
}

int Board::fenToBoard(const std::string_view fenStr) {
  using std::operator""sv;
  auto str = fenStr | std::views::split(' ');

  int i = 0;
  for (auto group : str) {
    switch (i) {
    case 0: {
      constexpr auto row_delim{"/"sv};
      int j = 0;
      for (const auto row : std::views::split(group, row_delim)) {

        int start = 8 * j;
        int k = 0;
        for (auto ch : row) {
          if (std::isdigit(ch)) {

            for (int i = 0; i < (ch - 48); i++, k++) {
              position[start + k] = 0;
            }
          } else {
            switch (ch) {
            case 'k':
              position[start + k] = piece.blackKing;
              break;
            case 'K':
              position[start + k] = piece.whiteKing;

              break;
            case 'q':
              position[start + k] = piece.blackQueen;
              break;
            case 'Q':
              position[start + k] = piece.whiteQueen;
              break;
            case 'r':
              position[start + k] = piece.blackRook;
              break;
            case 'R':
              position[start + k] = piece.whiteRook;
              break;
            case 'b':
              position[start + k] = piece.blackBishop;
              break;
            case 'B':
              position[start + k] = piece.whiteBishop;
              break;
            case 'n':
              position[start + k] = piece.blackKnight;
              break;
            case 'N':
              position[start + k] = piece.whiteKnight;
              break;
            case 'p':
              position[start + k] = piece.blackPawn;
              break;
            case 'P':
              position[start + k] = piece.whitePawn;
              break;
            default:
              return FEN_FIRST_GROUP;
            }
            ++k;
          }
        }
        ++j;
      }
      if (j != 8)
        return FEN_FIRST_GROUP;
      ++i;
      break;
    }
    case 1: {
      std::string_view value(group.begin(), group.end());
      if (value == "w") {
        isWhiteToMove = true;
      } else if (value == "b") {
        isWhiteToMove = false;
      } else {
        return FEN_SEC_GROUP;
      }
      ++i;
      break;
    }
    case 2: {
      std::string_view value(group.begin(), group.end());
      if (value == "-") {
        WQueenSideCastlingRights = false;
        WKingSideCastlingRights = false;
        BQueenSideCastlingRights = false;
        BKingSideCastlingRights = false;
      } else if (value.size() != 4) {
        return FEN_THIRD_GROUP;
      } else {
        WKingSideCastlingRights = value[0] == 'K';
        WQueenSideCastlingRights = value[1] == 'Q';
        BKingSideCastlingRights = value[2] == 'k';
        BQueenSideCastlingRights = value[3] == 'q';
      }
      ++i;
      break;
    }
    case 3: {
      std::string_view value(group.begin(), group.end());
      if (value == "-") {
      } else if (value.size() != 2) {
        std::cout << "size:" << value.size() << std::endl;
        return FEN_FOURTH_GROUP;
      } else {
        // we add this move to the movesPlayed vector
        // because int the move calculation function we check if
        // an en passant move is possible by checking the last move
        int enPassantSquare =
            (group.data()[0] - 97) + (8 - (group.data()[1] - 48)) * 8;
        std::cout << group.data()[0] << group.data()[1] << std::endl;
        std::cout << enPassantSquare << std::endl;
        int start = isWhiteToMove ? enPassantSquare - 8 : enPassantSquare + 8;
        int end = isWhiteToMove ? enPassantSquare + 8 : enPassantSquare - 8;
        movesPlayed.push_back(Move(start, end));
      }

      ++i;
      break;
    }
    case 4: {
      std::string_view value(group.begin(), group.end());
      if (value == "-") {
        fiftyMoveRule = 0;
      } else {
        auto res = std::from_chars(group.begin(), group.end(), fiftyMoveRule);
        if (res.ec != std::errc()) {
          return FEN_FIFTH_GROUP;
        }
      }
      ++i;
      break;
    }
    case 5:
      // we don't need to do anything with this group
      // as this just gives the number of moves played

      ++i;
      break;
    default:
      return FEN_GROUPS_SIZE;
    }
  }
  if (i != 6)
    return FEN_GROUPS_SIZE;

  return FEN_SUCCESS;
}

Board::Board() {}
