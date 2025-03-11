#include "../BoardGUI/board.hpp"
#include <algorithm>
#include <gtest/gtest.h>

TEST(Chess, Standard) {
  Board board;
  std::string_view fen =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

  int expPosition[64] = {
      4, 3, 2, 5, 6, 2, 3, 4, 1, 1, 1, 1, 1,  1,  1,  1,  0,  0,  0,  0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 12, 11, 10, 13, 14, 10, 11, 12};

  board.fenToBoard(fen);

  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  for (int i = 0; i < 64; i++) {
    EXPECT_EQ(board.position[i], expPosition[i]);
  }
}

TEST(CHESS, ranOpening) {
  Board board;
  std::string_view fen =
      "r1bqk2r/pp3ppp/2nppn2/2p5/2PP4/2PBPN2/P4PPP/R1BQK2R w KQkq - 0 1";
  int expPosition[64] = {4, 0, 2, 5, 6, 0, 0, 4, 1,  1, 0,  0,  0,  1,  1, 1,
                         0, 0, 3, 1, 1, 3, 0, 0, 0,  0, 1,  0,  0,  0,  0, 0,
                         0, 0, 9, 9, 0, 0, 0, 0, 0,  0, 9,  10, 9,  11, 0, 0,
                         9, 0, 0, 0, 0, 9, 9, 9, 12, 0, 10, 13, 14, 0,  0, 12};

  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  for (int i = 0; i < 64; i++) {
    EXPECT_EQ(board.position[i], expPosition[i]);
  }
}

TEST(CHESS, withEnPassant) {
  Board board;
  std::string_view fen =
      "rnbqkbnr/pp2pppp/2p5/8/3pP3/5P2/PPPP2PP/RNBQKBNR b KQkq e3 0 1";
  int expPosition[64] = {
      4, 3, 2, 5, 6, 2, 3, 4, 1, 1, 0, 0, 1,  1,  1,  1,  0,  0,  1,  0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  1,  9,  0,  0,  0,  0,  0, 0, 0,
      0, 9, 0, 0, 9, 9, 9, 9, 0, 0, 9, 9, 12, 11, 10, 13, 14, 10, 11, 12};

  board.fenToBoard(fen);
  EXPECT_FALSE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  for (int i = 0; i < 64; i++) {
    EXPECT_EQ(board.position[i], expPosition[i]);
  }

  std::vector<Move> legalMoves = board.generateLegalMoves(!board.isWhiteToMove);

  bool containsEnPassant = false;
  for (Move move : legalMoves) {
    if (move.enPassant) {
      containsEnPassant = true;
      break;
    }
  }

  EXPECT_TRUE(containsEnPassant);
}

TEST(CHESS, CastlingRights) {
  Board board;
  std::string_view fen =
      "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQk- - 0 1";

  int expPosition[64] = {
      4, 3, 2, 5, 6, 2, 3, 4, 1, 1, 1, 1, 1,  1,  1,  1,  0,  0,  0,  0, 0, 0,
      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0, 0, 0,
      0, 0, 0, 0, 9, 9, 9, 9, 9, 9, 9, 9, 12, 11, 10, 13, 14, 10, 11, 12};

  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_FALSE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  for (int i = 0; i < 64; i++) {
    EXPECT_EQ(board.position[i], expPosition[i]);
  }

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQ-- - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_FALSE(board.BKingSideCastlingRights);
  EXPECT_FALSE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -Qkq - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w -Q-- - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_FALSE(board.BKingSideCastlingRights);
  EXPECT_FALSE(board.BQueenSideCastlingRights);
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w --kq - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_TRUE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_FALSE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w ---q - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_FALSE(board.BKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_FALSE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);

  fen = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1";
  board.fenToBoard(fen);
  EXPECT_TRUE(board.isWhiteToMove);
  EXPECT_FALSE(board.BKingSideCastlingRights);
  EXPECT_FALSE(board.BQueenSideCastlingRights);
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_FALSE(board.WQueenSideCastlingRights);
  EXPECT_EQ(board.fiftyMoveRule, 0);
}
