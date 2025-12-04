#ifndef BOARD_H
#define BOARD_H

#include<stdio.h>
#include<stdlib.h>

typedef enum{ EMPTY, wPawn, wKnight, wBishop, wRook, wQueen, wKing, bPawn, bKnight, bBishop, bRook, bQueen, bKing } Piece;  

enum{white,black};

typedef enum {
    a8 = 21, b8, c8, d8, e8, f8, g8, h8,  // 21-28
    a7 = 31, b7, c7, d7, e7, f7, g7, h7,  // 31-38
    a6 = 41, b6, c6, d6, e6, f6, g6, h6,  // 41-48
    a5 = 51, b5, c5, d5, e5, f5, g5, h5,  // 51-58
    a4 = 61, b4, c4, d4, e4, f4, g4, h4,  // 61-68
    a3 = 71, b3, c3, d3, e3, f3, g3, h3,  // 71-78
    a2 = 81, b2, c2, d2, e2, f2, g2, h2,  // 81-88
    a1 = 91, b1, c1, d1, e1, f1, g1, h1   // 91-98
} Square;


typedef struct _Board {
    int *board;
    int turn;
    Square white_king_sq;
    Square black_king_sq;
    Square en_passant_sq; //-1 if none, otherwise the square that can be captured


    int w_castle_king; //can white king side castle?
    int w_castle_queen; //can white queen side castle?
    int b_castle_king;
    int b_castle_queen;
} Board;

//allocate memory for board
Board *create_board_default();


Board *create_board_from_fen(const char *fen);


//print the board
void print_board(Board *b);

//free the board
void destroy_board(Board *b);

void reset_board_default(Board *b);

#endif //BOARD_H