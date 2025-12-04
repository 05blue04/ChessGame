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

static Square algebraic_to_square(char file, char rank) {
    // 'a'-'h' -> 1-8, '1'-'8' -> ranks
    int f = file - 'a' + 1;
    int r = '8' - rank + 2;  // '8' -> rank 2 (top), '1' -> rank 9 (bottom)
    return r * 10 + f;
}

static Piece char_to_piece(char c) {
    switch (c) {
        case 'P': return wPawn;
        case 'N': return wKnight;
        case 'B': return wBishop;
        case 'R': return wRook;
        case 'Q': return wQueen;
        case 'K': return wKing;
        case 'p': return bPawn;
        case 'n': return bKnight;
        case 'b': return bBishop;
        case 'r': return bRook;
        case 'q': return bQueen;
        case 'k': return bKing;
        default:  return EMPTY;
    }
}


Board *create_board_from_fen(const char *fen) {
    Board *b = allocate_board();
    if (b == NULL){
        return b;
    }

    // Initialize defaults
    b->w_castle_king = 0;
    b->w_castle_queen = 0;
    b->b_castle_king = 0;
    b->b_castle_queen = 0;
    b->en_passant_sq = -1;
    b->white_king_sq = -1;
    b->black_king_sq = -1;
    
    int rank = 2;  // Start at rank 8 (top of board)
    int file = 1;
    
    while (*fen && *fen != ' ') {
        if (*fen == '/') {
            rank++;
            file = 1;
        } else if (*fen >= '1' && *fen <= '8') {
            file += (*fen - '0');
        } else {
            Square sq = rank * 10 + file;
            Piece p = char_to_piece(*fen);
            b->board[sq] = p;
            
            // Track king positions
            if (p == wKing) 
                b->white_king_sq = sq;
            if (p == bKing) 
                b->black_king_sq = sq;
            
            file++;
        }
        fen++;
    }
    
    if (*fen == ' ') fen++;
    
    b->turn = (*fen == 'w') ? white : black;
    fen++;
    
    if (*fen == ' ') fen++;
    
    // Field 3: Castling rights
    while (*fen && *fen != ' ') {
        switch (*fen) {
            case 'K': b->w_castle_king = 1; break;
            case 'Q': b->w_castle_queen = 1; break;
            case 'k': b->b_castle_king = 1; break;
            case 'q': b->b_castle_queen = 1; break;
            case '-': break;
        }
        fen++;
    }
    
    if (*fen == ' ') fen++;
    
    // Field 4: En passant square
    if (*fen && *fen != '-') {
        char ep_file = *fen++;
        char ep_rank = *fen++;
        b->en_passant_sq = algebraic_to_square(ep_file, ep_rank);
    }
    
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