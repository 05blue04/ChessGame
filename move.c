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


void debug_is_legal_move(Board *b, Square src, Square dst){
    fprintf(stderr,"Checking to see if move %d to %d is valid\n",src,dst);
    is_legal_move(b,src,dst);
}

int is_legal_move(Board *b, Square src, Square dst){
    
    if (b == NULL){
        fprintf(stderr, "is_legal_move: board must be initialized\n");
        return 0;
    }

    if (!is_square_valid(src)){

        fprintf(stderr, "square %d is an invalid square\n",src);
        return 0;
    }

    if (!is_square_valid(dst)){
        fprintf(stderr, "square %d is an invalid square\n",dst);
        return 0;
    }

    if(src == dst){
        fprintf(stderr, "cant move piece to same square\n");
        return 0;
    }

    Piece p = b->board[src];
    Piece dst_p = b->board[dst];

    //check if a piece exists
    if(p == EMPTY){
        fprintf(stderr,"piece at src:%d is empty\n",src);
        return 0;
    }

    //ensure turn order 
    if(b->turn == white && p > wKing){
        fprintf(stderr,"moving out of order whites turn\n");
        return 0;
    }

    if(b->turn ==  black && p <= wKing){
        fprintf(stderr,"moving out of order blacks turn\n");
        return 0;
    }

    //check if capturing not capturing own piece
    if (dst_p != EMPTY){
        if(b->turn == white && (dst_p <= wKing)){
            return 0;
        }else if(b->turn == black && (dst_p > wKing)){
            return 0;
        }
    }


    //case analysis
    int check = 0;
    int valid = 0;
    switch (p)
    {
    case wPawn:
        check = src - dst;
        
        if(check == 10){ // move straight up
            valid = (dst_p == EMPTY);
            break;
        }

        if(check == 9 || check == 11){ // capture diagonaly 
            valid = (dst_p != EMPTY);
            break;
        }

        if(check == 20 && (src >= 81 && src <= 88)){ //move up 2 ONLY on starting move
            valid = (dst_p == EMPTY && b->board[src - 10] == EMPTY);
        }

        break;
    
    case bPawn:
        check = dst - src;
        
        if(check == 10){ // move straight down
            valid = (dst_p == EMPTY);
            break;
        }

        if(check == 9 || check == 11){ // capture diagonaly
            valid = (dst_p != EMPTY);
            break;
        }

        if(check == 20 && (src >= 31 && src <= 38)){ //move up 2 ONLY on starting move
            valid = (dst_p == EMPTY && b->board[src + 10] == EMPTY);
        }
        break;

    case wKnight:
    case bKnight:{
        check = dst - src;

        if(check == 21 || check == 19 || check == 12 || check == 8 ||
            check == -21 || check == -19 || check == -12 || check == -8){
                valid = 1;
        }

        break;            
    }

    case wKing:
    case bKing: {
        
        int check = dst - src;

        if (check == 1  || check == -1  ||  // horizontal
            check == 10 || check == -10 ||  // vertical
            check == 11 || check == -11 ||  // diagonal
            check == 9  || check == -9) {   // diag 
            valid = 1;
        }

        break;
    }

    case wBishop:
    case bBishop:{
        check = dst - src;
        int dir = 0;

        // determining diagonal direction
        if(check % 11 == 0){
            if(check > 0 ){
                dir = 11;
            }else{
                dir = -11;
            }
        } else if (check % 9 == 0){
            if( check > 0 ){
                dir = 9;
            }else{
                dir = -9;
            }
        } else{
            valid = 0;
            break;
        }

        //traverse the path of the diagonal to ensure nothing is blocking the move
        int square = src + dir; 
        while(square != (int)dst){
            if(b->board[square] != EMPTY){ //check if path is occupied
                valid = 0;
                break;
            }
            square += dir;
        }

        valid = 1;
        break;
    }

    case wRook:
    case bRook:{
        check = dst - src;
        int dir = 0;

        if(check % 10  == 0){
            if(check > 0){
                dir = 10;
            }else{
                dir = -10;
            }
        } else if(src / 10 == dst / 10){
            if(check > 0 ){
                dir = 1;
            } else{
                dir = -1;
            }
        } else{
            valid = 0;
            break;
        }

        //traverse path of rook to ensure no pieces are blocking its path
        int square = src + dir;
        while(square != (int)dst){
            if (b->board[square] != EMPTY){
                valid = 0;
                break;
            }
            square += dir;
        }

        valid = 1;
        break;
    }

    case wQueen:
    case bQueen:{
        check = dst - src;
        int dir = 0;

        //lowk light just a combo of rook + bishop moves
        if(check % 10  == 0){
            if(check > 0){
                dir = 10;
            }else{
                dir = -10;
            }
        } else if(src / 10 == dst / 10){
            if(check > 0 ){
                dir = 1;
            } else{
                dir = -1;
            }
        } else if(check % 11 == 0){
            if(check > 0 ){
                dir = 11;
            }else{
                dir = -11;
            }
        } else if (check % 9 == 0){
            if( check > 0 ){
                dir = 9;
            }else{
                dir = -9;
            }
        } else{
            valid = 0;
            break;
        }

        // traverse the path
        int square = src + dir;
        while(square != (int)dst){
            if(b->board[square] != EMPTY){
                valid = 0;
                break;
            }
            square += dir;
        }

        valid = 1;
        break;
    }
        
    default:
        break;
    }

    if(!valid){
        return 0;
    }
    
    // determine if the move leaves king in check
    b->board[dst] = p;
    b->board[src] = EMPTY;
    
    // Update king position if king moved
    Square old_king_sq = -1;
    if (p == wKing) {
        old_king_sq = b->white_king_sq;
        b->white_king_sq = dst;
    } else if (p == bKing) {
        old_king_sq = b->black_king_sq;
        b->black_king_sq = dst;
    }

    int in_check = is_in_check(b, b->turn);

    // Undo move
    b->board[src] = p;
    b->board[dst] = dst_p;
    
    if (p == wKing) {
        b->white_king_sq = old_king_sq;
    } else if (p == bKing) {
        b->black_king_sq = old_king_sq;
    }

    if (in_check) {
        fprintf(stderr, "move leaves king in check\n");
        return 0;
    }

    return 1;

}

int is_square_attacked(Board *b, Square sq, int by_color){
    // knight attacks
    int knight_offsets[] = {21, 19, 12, 8, -21, -19, -12, -8};
    int enemy_knight = (by_color == white) ? wKnight : bKnight; 

    for(int i = 0; i < 8; i++){
        Square s = sq + knight_offsets[i];
        if(is_square_valid(s) && (b->board[s] == enemy_knight))
            return 1;
    }

    // pawn attacks
    if (by_color == white) {
        // White pawns attack from below 
        if (is_square_valid(sq + 11) && b->board[sq + 11] == wPawn){
            return 1;
        } 
        if (is_square_valid(sq + 9) && b->board[sq + 9] == wPawn){
            return 1;
        }
    } else {
        // Black pawns attack from above 
        if (is_square_valid(sq - 11) && b->board[sq - 11] == bPawn){
            return 1;
        } 
        if (is_square_valid(sq - 9) && b->board[sq - 9] == bPawn){
            return 1;
        }
    }

    // King attacks
    int king_offsets[] = {1, -1, 10, -10, 11, -11, 9, -9};
    int enemy_king = (by_color == white) ? wKing : bKing;
    
    for (int i = 0; i < 8; i++) {
        Square s = sq + king_offsets[i];
        if (is_square_valid(s) && b->board[s] == enemy_king)
            return 1;
    }

    // Bishop/queen attacks (diagonals)
    int diagonals[] = {11, -11, 9, -9};
    int enemy_bishop = (by_color == white) ? wBishop : bBishop;
    int enemy_queen = (by_color == white) ? wQueen : bQueen;
    
    for (int i = 0; i < 4; i++) {
        Square s = sq + diagonals[i];
        while (is_square_valid(s)) {
            if (b->board[s] != EMPTY) {
                if (b->board[s] == enemy_bishop || b->board[s] == enemy_queen)
                    return 1;
                break;  // Blocked by some piece
            }
            s += diagonals[i];
        }
    }

    // Rook/queen attacks (lines)
    int verticals[] = {10, -10, 1, -1};
    int enemy_rook = (by_color == white) ? wRook : bRook;
    
    for (int i = 0; i < 4; i++) {
        Square s = sq + verticals[i];
        while (is_square_valid(s)) {
            if (b->board[s] != EMPTY) {
                if (b->board[s] == enemy_rook || b->board[s] == enemy_queen)
                    return 1;
                break;
            }
            s += verticals[i];
        }
    }

    return 0;
}

int is_in_check(Board *b, int color){
    Square king_sq = (color == white) ? b->white_king_sq : b->black_king_sq;
    int enemy = (color == white) ? black : white;
    return is_square_attacked(b, king_sq, enemy);
}

void make_move(Board *b, Square src, Square dst){
    if(!is_legal_move(b,src,dst)){
        fprintf(stderr,"move is illegal \n");
        return;
    }

   Piece p = b->board[src];

   if(p == wKing){
        b->white_king_sq = dst;
   } else if(p == bKing){
        b->black_king_sq = dst;
   }

   b->board[src] = EMPTY; 
   b->board[dst] = p;
   b->turn ^= 1;
}