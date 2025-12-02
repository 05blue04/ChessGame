#include "move.h"


static int is_square_valid(Square sq){
    if(sq < 21)
        return 0;
    
    if(sq > 98)
        return 0;

    if((sq % 10 < 1) || (sq % 10 > 8))
        return 0;

    return 1;
}


int is_legal_move(Board *b, Square src, Square dst){
    
    if (b == NULL){
        fprintf(stderr, "is_legal_move: board must be initialized\n");
        return 0;
    }

    if (!is_square_valid(src))
        return 0;

    if (!is_square_valid(dst))
        return 0;

    Piece p = b->board[src];
    Piece dst_p = b->board[dst];

    //check if a piece exists
    if(p == EMPTY)
        return 0;

    //ensure turn order 
    if(b->turn == white && p > wKing)
        return 0;

    if(b->turn ==  black && p <= wKing)
        return 0;

    //check if capturing not capturing own piece
    if (dst_p != EMPTY){
        if(b->turn == white && (dst_p <= wKing)){
            return 0;
        }else if(b->turn == black && (dst_p > wKing)){
            return 0;
        }
    }


    //case analysis
    switch (p)
    {
    case wPawn:
        int check = src - dst;
        
        if(check == 10){ // move straight up
            return (dst_p == EMPTY);
        }

        if(check == 9 || check == 11){ // capture diagonaly 
            return (dst_p != EMPTY);
        }

        if(check == 20 && (src >= 81 && src <= 88)){ //move up 2 ONLY on starting move
            return (dst_p == EMPTY && b->board[src - 10] == EMPTY);
        }

        return 0;
    
    case bPawn:
        int check = dst - src;
        
        if(check == 10){ // move straight up
            return (dst_p == EMPTY);
        }

        if(check == 9 || check == 11){ // capture diagonaly to the right
            return (dst_p != EMPTY);
        }

        if(check == 20 && (src >= 31 && src <= 38)){ //move up 2 ONLY on starting move
            return (dst_p == EMPTY && b->board[src + 10] == EMPTY);
        }

        return 0;

    default:
        break;
    }

}