#include "board.h"

static Board *allocate_board(){
    Board *b = malloc(sizeof(*b));
    if(b == NULL){
        return NULL;
    }

    b->board = calloc(120, sizeof(*(b->board)));
    if(b->board == NULL){
        free(b);
        return NULL;
    }

    return b;
}

Board *create_board_default(){
    Board *b = allocate_board();
    if(b == NULL){
        return NULL;
    }

    b->turn = white;
    b->b_castle_king = 1;
    b->b_castle_queen = 1;
    b->w_castle_king = 1;
    b->w_castle_queen = 1;
    b->white_king_sq = e1;
    b->black_king_sq = e8;
    b->en_passant_sq = -1;

    //initalize pieces onto board
    for(int i = 31; i < 39; i++){
        b->board[i] = bPawn;
    }

    for(int i = 81; i < 89; i++){
        b->board[i] = wPawn;
    }

    b->board[b8] = bKnight;
    b->board[g8] = bKnight;
    b->board[b1] = wKnight;
    b->board[g1] = wKnight;

    b->board[c8] = bBishop;
    b->board[f8] = bBishop;
    b->board[c1] = wBishop;
    b->board[f1] = wBishop;

    b->board[a8] = bRook;
    b->board[h8] = bRook;
    b->board[a1] = wRook;
    b->board[h1] = wRook;

    b->board[d8] = bQueen;
    b->board[d1] = wQueen;
    
    b->board[e8] = bKing;
    b->board[e1] = wKing;


    return b;
}

void reset_board_default(Board *b){

    b->turn = white;
    b->b_castle_king = 1;
    b->b_castle_queen = 1;
    b->w_castle_king = 1;
    b->w_castle_queen = 1;
    b->white_king_sq = e1;
    b->black_king_sq = e8;
    b->en_passant_sq = -1;

    for(int i = 31; i < 39; i++){
        b->board[i] = bPawn;
    }

    for(int i = 81; i < 89; i++){
        b->board[i] = wPawn;
    }

    b->board[b8] = bKnight;
    b->board[g8] = bKnight;
    b->board[b1] = wKnight;
    b->board[g1] = wKnight;

    b->board[c8] = bBishop;
    b->board[f8] = bBishop;
    b->board[c1] = wBishop;
    b->board[f1] = wBishop;

    b->board[a8] = bRook;
    b->board[h8] = bRook;
    b->board[a1] = wRook;
    b->board[h1] = wRook;

    b->board[d8] = bQueen;
    b->board[d1] = wQueen;
    
    b->board[e8] = bKing;
    b->board[e1] = wKing;

    for(int rank = 4; rank < 8; rank++){
        for(int file = 1; file < 9; file++){
            b->board[rank * 10 + file] = EMPTY;
        }
    }
}

static const char* piece_to_string(int piece) {
    switch(piece) {
        case EMPTY:   return " ";
        case wPawn:   return "♙";
        case wKnight: return "♘";
        case wBishop: return "♗";
        case wRook:   return "♖";
        case wQueen:  return "♕";
        case wKing:   return "♔";
        case bPawn:   return "♟";
        case bKnight: return "♞";
        case bBishop: return "♝";
        case bRook:   return "♜";
        case bQueen:  return "♛";
        case bKing:   return "♚";
        default:      return "?";
    }
}

void print_board(Board *b) {
    for(int rank = 2; rank <= 9; rank++) {
        printf("%d ", 10 - rank);  // Print rank number
        for(int file = 1; file <= 8; file++) {
            int sq = rank * 10 + file;
            printf("%s ", piece_to_string(b->board[sq]));
        }
        printf("\n");
    }
    printf("  a b c d e f g h\n");
}

void destroy_board(Board *b){
    if(b != NULL){
        free(b->board);
        free(b);
    }
}