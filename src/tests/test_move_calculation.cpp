#include "../BoardGUI/board.hpp"
#include <gtest/gtest.h>

// Returns true if the move list contains a castling move on the given side.
static bool hasCastle(const std::vector<Move> &moves, bool kingSide) {
  for (const Move &m : moves) {
    if (kingSide ? m.kCastle : m.qCastle)
      return true;
  }
  return false;
}

// Baseline: with both rooks and the king home and no obstruction, both castles
// are legal for White.
TEST(Castling, BothSidesAvailable) {
  Board board;
  board.fenToBoard("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  std::vector<Move> moves = board.generateLegalMoves(false);
  EXPECT_TRUE(hasCastle(moves, true));
  EXPECT_TRUE(hasCastle(moves, false));
}

// Moving the king and returning it home must forfeit castling permanently.
TEST(Castling, KingMoveForfeitsRights) {
  Board board;
  board.fenToBoard("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  board.makeMove(Move(60, 52)); // Ke1-e2
  EXPECT_FALSE(board.WKingSideCastlingRights);
  EXPECT_FALSE(board.WQueenSideCastlingRights);

  board.makeMove(Move(52, 60)); // Ke2-e1 (back home)

  std::vector<Move> moves = board.generateLegalMoves(false);
  EXPECT_FALSE(hasCastle(moves, true));
  EXPECT_FALSE(hasCastle(moves, false));
}

// Moving a rook forfeits only that side's right; the other side is untouched.
TEST(Castling, RookMoveForfeitsOneSide) {
  Board board;
  board.fenToBoard("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  board.makeMove(Move(56, 57)); // Ra1-b1
  EXPECT_FALSE(board.WQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
}

// make/unmake must restore castling rights exactly (regression for the
// unmakeMove copy-paste bug that wrote the wrong rights variable).
TEST(Castling, UnmakeRestoresRights) {
  Board board;
  board.fenToBoard("r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1");

  board.makeMove(Move(56, 57)); // Ra1-b1 revokes White queenside
  EXPECT_FALSE(board.WQueenSideCastlingRights);

  board.unmakeMove();
  EXPECT_TRUE(board.WQueenSideCastlingRights);
  EXPECT_TRUE(board.WKingSideCastlingRights);
  EXPECT_TRUE(board.BQueenSideCastlingRights);
  EXPECT_TRUE(board.BKingSideCastlingRights);
}

// A rook captured on its home square forfeits that castling right, and no
// phantom rook may be conjured by a later castle attempt.
TEST(Castling, CapturingRookForfeitsRight) {
  Board board;
  board.fenToBoard("r3k3/8/8/8/8/6n1/8/R3K2R w KQ-- - 0 1");

  board.makeMove(Move(46, 63)); // Ng3xh1 captures the kingside rook
  EXPECT_FALSE(board.WKingSideCastlingRights);

  std::vector<Move> moves = board.generateLegalMoves(false);
  EXPECT_FALSE(hasCastle(moves, true));  // no kingside castle / phantom rook
  EXPECT_TRUE(hasCastle(moves, false));  // queenside is unaffected
}

// The king may not castle across a square attacked only by a pawn (the square
// is empty during the transit, so pawn attacks are easy to miss).
TEST(Castling, CannotCastleThroughPawnAttack) {
  Board board;
  board.fenToBoard("r3k2r/8/8/8/8/8/6p1/R3K2R w KQ-- - 0 1");

  std::vector<Move> moves = board.generateLegalMoves(false);
  EXPECT_FALSE(hasCastle(moves, true)); // g2 pawn attacks f1, the transit square
  EXPECT_TRUE(hasCastle(moves, false)); // queenside path is clear
}

// The king may not castle out of check.
TEST(Castling, CannotCastleOutOfCheck) {
  Board board;
  board.fenToBoard("r3k3/4r3/8/8/8/8/8/R3K2R w KQ-- - 0 1");

  std::vector<Move> moves = board.generateLegalMoves(false);
  EXPECT_FALSE(hasCastle(moves, true));
  EXPECT_FALSE(hasCastle(moves, false));
}
