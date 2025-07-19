# Chess GUI in C++ with SFML 2.6.1

A basic chess GUI built using **C++23** and **SFML 2.6.1**, supporting piece movement according to chess rules, custom board positions via **FEN** strings, and basic interaction through a graphical board.

## ✨ Features

- 🧩 **Chess logic** including legal moves, check detection, castling, en passant, and pawn promotion.
- 🖼️ **Graphical interface** using **SFML 2.6.1**.
- 🧠 Supports **FEN string input** to load custom starting positions.
- ✅ Validates moves and enforces most chess rules (excluding 50-move draw rule and threefold repetition).

## 📦 Dependencies

- **C++23** compiler (`clang++` or `g++`)
- **SFML 2.6.1**
  - `libsfml-graphics.so.2.6.1`
  - `libsfml-window.so.2.6.1`
  - `libsfml-system.so.2.6.1`

> Ensure SFML is either installed system-wide or placed in the `lib` directory (see below).

## 🛠️ Build Instructions

Assuming you have a local SFML build in `../lib/SFML/SFML/`, use the included `Makefile`.

```bash
make        # Build the project
make run    # Run with LD_LIBRARY_PATH set automatically
make clean  # Clean build artifacts

<img width="1850" height="1483" alt="image" src="https://github.com/user-attachments/assets/ecdd1ed1-50e3-4a9f-9394-237fccfbe00f" />
