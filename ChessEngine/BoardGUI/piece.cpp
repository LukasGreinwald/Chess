class Piece
{
    
public:
    const int empty = 0;
    const int pawn = 1;
    const int bishop = 2;
    const int knight = 3;
    const int rook = 4;
    const int queen = 5;
    const int king = 6;
    
    
    const int white = 0b1000;

    const int blackPawn = pawn | 0;         //1
    const int blackBishop = bishop | 0;     //2
    const int blackKnight = knight | 0;     //3
    const int blackRook = rook | 0;         //4
    const int blackQueen = queen | 0;       //5
    const int blackKing = king | 0;         //6


    const int whitePawn = pawn | white;     //9
    const int whiteBishop = bishop | white; //10
    const int whiteKnight = knight | white; //11
    const int whiteRook = rook | white;     //12
    const int whiteQueen = queen | white;   //13
    const int whiteKing = king | white;     //14
    Piece();
    
};

Piece::Piece()
{
}



