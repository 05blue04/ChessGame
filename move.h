#ifndef MOVE_H
#define MOVE_H

#include "board.h"

//function that checks if move from src to dst is valid on the chess board
int is_legal_move(Board *b, Square src, Square dst);

void make_move(Board *b, Square src, Square dst);

//check if king is in check
int is_in_check(Board *b, int color);

int is_stalemate(Board *b, int color);

int is_checkmate(Board *b, int color);

int has_legal_moves(Board *b, int color);

//check if square is attacked
int is_square_attacked(Board *b, Square sq, int by_color);

void test_movement();

#endif