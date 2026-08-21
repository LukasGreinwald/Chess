# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Overview

A chess GUI written in **C++23** using **SFML 2.6.1**. It renders an 8x8 board, loads
positions from FEN strings, generates legal moves (castling, en passant, promotion,
check filtering — no 50-move or threefold-repetition draw enforcement), and lets the
user play by dragging pieces.

## Build & Run

Two independent build systems coexist:

**Makefile** (repo root) — builds the GUI executable `board`:
```bash
make          # debug build -> ./board
make release  # release build
make run      # NOTE: this target does not exist in the current Makefile; use ./board
make clean
```
- Expects a local SFML build at `../lib/SFML/SFML/` (outside the repo). Adjust `SFML_PATH`
  in the Makefile if SFML is installed elsewhere.
- Compiler auto-selected: `clang++` if present, else `g++`.
- **Run from the repo root**: piece textures are loaded via paths relative to the CWD
  (`./src/res/pieces/*.png` in `src/BoardGUI/main.cpp`).

**CMake** (`src/CMakeLists.txt`) — builds the `tests` executable only. Use `src/eval.sh`:
```bash
cd src && ./eval.sh    # clean-configures with clang-18, builds + runs all tests
```
`eval.sh` wipes `src/build/`, configures with `clang++-18`/`clang-18`, exports
`compile_commands.json`, builds the `tests` target, and reports pass/fail. Requires
system-installed **GTest** and **SFML 2.5+**.

Run a single test (after `eval.sh` has built it):
```bash
src/build/tests --gtest_filter='CHESS.withEnPassant'
```

## Architecture

Three layers, all under `src/BoardGUI/`:

- **`Piece`** (`piece.{hpp,cpp}`) — piece encoding constants and color helpers. Pieces are
  ints: low 3 bits = type (`pawn`=1 … `king`=6, `pieceMask`=0b111), bit `0b1000` = white.
  So black pieces are 1–6, white pieces 9–14, empty = 0. `isWhite(pc)` / `hasDiffColor(a,b)`.

- **`Board`** (`board.{hpp,cpp}`) — pure game logic, no SFML rendering. Owns `int position[64]`
  where **index 0 = a8 (top-left), index 63 = h1** (rank 8 first, matching FEN order).
  Key methods:
  - `fenToBoard(fen)` — parses all 6 FEN fields; returns 0 on success or a `FEN_*` error code.
    The en-passant field is encoded as a synthetic prior move pushed onto `movesPlayed` so
    move generation can detect the capture.
  - `generateMoves(black)` — pseudo-legal moves for the side.
  - `generateLegalMoves(black)` — calls `generateMoves`, then make/unmake each move and
    discards any that leave the king capturable (also guards castling path squares). This
    is how check is detected — there is no separate attack-map function.
  - `makeMove(move)` / `unmakeMove()` — apply/undo; `movesPlayed` is the undo stack and also
    drives en-passant detection.
  - `calculateSquarestoEdge(sq)` — packs distance-to-each-edge into one `unsigned short`,
    3 bits per direction, layout `XXXX BBB TTT RRR LLL`. Sliding-piece loops use these counts;
    `movingOffsets` / `knightOffsets` are the direction deltas.
  - `Move` struct carries flags for castling (`kCastle`/`qCastle`), promotion (`isProm`, always
    promotes to queen in `makeMove`), `enPassant`, and `Capture` (the captured piece, for undo).

- **`Window`** (`window.{hpp,cpp}`, `main.cpp`) — SFML rendering + input. Holds a `Board`,
  draws squares/sprites, and runs the event loop. On startup it prompts on **stdin** for an
  optional FEN string. Interaction: drag a piece to move (validated against `generateLegalMoves`),
  **Left arrow** = undo. Piece image paths are wired in `main.cpp`'s `pieceImg[16]` array,
  indexed by the piece encoding.

## Tests

`src/tests/` uses GoogleTest. `test_fenToBoard.cpp` covers FEN parsing, castling-rights
flags, and en-passant move generation. `test_move_calculation.cpp` is currently a stub
(only includes the header).
