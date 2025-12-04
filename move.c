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
        return 0;
    }

    if (!is_square_valid(src)){
        return 0;
    }

    if (!is_square_valid(dst)){
        return 0;
    }

    if(src == dst){
        return 0;
    }

    Piece p = b->board[src];
    Piece dst_p = b->board[dst];

    //check if a piece exists
    if(p == EMPTY){
        return 0;
    }

    //ensure turn order 
    if(b->turn == white && p > wKing){
        return 0;
    }

    if(b->turn ==  black && p <= wKing){
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
            valid = (dst_p != EMPTY) || (dst == b->en_passant_sq);
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
            valid = (dst_p != EMPTY) || (dst == b->en_passant_sq);
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
            break;
        }

        int enemy = 1 ^ b->turn; // flipping color

        //check king side castle e1 to g1 **WHITE
        if(p == wKing && src == e1){
            if(check == 2 && b->w_castle_king){
                if(b->board[f1] == EMPTY && b->board[g1] == EMPTY && b->board[h1] == wRook
                    && !is_square_attacked(b,e1, enemy) && !is_square_attacked(b,f1,enemy) && !is_square_attacked(b,g1,enemy)){
                        valid = 1;
                }
            }
        }

        // check queen side castle e1 to c1 **WHITE
        if(p == wKing && src == e1){
            if(check == -2 && b->w_castle_queen){
                if(b->board[d1] == EMPTY && b->board[c1] == EMPTY && b->board[b1] == EMPTY && b->board[a1] == wRook
                    && !is_square_attacked(b,e1, enemy) && !is_square_attacked(b,d1,enemy) && !is_square_attacked(b,c1,enemy)){
                        valid = 1;
                }
            }
        }
        
        // check king side castle e8 to g8 ** BLACK
        if(p == bKing && src == e8){
            if(check == 2 && b->b_castle_king){
                if(b->board[f8] == EMPTY && b->board[g8] == EMPTY && b->board[h8] == bRook
                    && !is_square_attacked(b,e8, enemy) && !is_square_attacked(b,f8,enemy) && !is_square_attacked(b,g8,enemy)){
                        valid = 1;
                }
            }
        }
        // check queen side castle e8 to c8 **BLACK
        if(p == bKing && src == e8){
            if(check == -2 && b->b_castle_queen){
                if(b->board[d8] == EMPTY && b->board[c8] == EMPTY && b->board[b8] == EMPTY && b->board[a8] == bRook
                    && !is_square_attacked(b,e8, enemy) && !is_square_attacked(b,d8,enemy) && !is_square_attacked(b,c8,enemy)){
                        valid = 1;
                }
            }
        }
        break;
    }

    case wBishop:
    case bBishop:{
        check = dst - src;
        valid = 1;
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

        break;
    }

    case wRook:
    case bRook:{
        check = dst - src;
        int dir = 0;
        valid = 1;

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


        break;
    }

    case wQueen:
    case bQueen:{
        check = dst - src;
        int dir = 0;
        valid = 1;

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
        return 0;
    }
 //-1 if none, otherwise the square that can be captured
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

int has_legal_moves(Board *b, int color) {
    // Loop through all squares
    for (int rank = 2; rank <= 9; rank++) {
        for (int file = 1; file <= 8; file++) {
            Square src = rank * 10 + file;
            Piece p = b->board[src];
            
            // Skip empty squares and opponent pieces
            if (p == EMPTY) continue;
            if (color == white && p > wKing) continue;
            if (color == black && p <= wKing) continue;
            
            // Try all possible destination squares
            for (int dst_rank = 2; dst_rank <= 9; dst_rank++) {
                for (int dst_file = 1; dst_file <= 8; dst_file++) {
                    Square dst = dst_rank * 10 + dst_file;
                    
                    // Temporarily suppress error messages
                    if (is_legal_move(b, src, dst)) {
                        return 1;  // Found a legal move!
                    }
                }
            }
        }
    }
    
    return 0;  // No legal moves
}

int is_checkmate(Board *b, int color) {
    return is_in_check(b, color) && !has_legal_moves(b, color);
}

int is_stalemate(Board *b, int color) {
    return !is_in_check(b, color) && !has_legal_moves(b, color);
}

void make_move(Board *b, Square src, Square dst){
    if(!is_legal_move(b,src,dst)){
        fprintf(stderr,"move %d to %d is illegal \n",src,dst);
        return;
    }

   Piece p = b->board[src];

    // check if enpassant possible 
    if (p == wPawn && dst == b->en_passant_sq) {
        b->board[dst + 10] = EMPTY;  // Black pawn was one rank below target
    } else if (p == bPawn && dst == b->en_passant_sq) {
        b->board[dst - 10] = EMPTY;  // White pawn was one rank above target
    }

    // Reset en passant at start of each move
    b->en_passant_sq = -1;

    // if pawn moved 2 set enpassant target
    if(p == wPawn && src - dst == 20){
        b->en_passant_sq = src - 10;
    } else if (p == bPawn && dst - src == 20){
        b->en_passant_sq = src + 10;
    }
    

    // Handle castling - move the rook too
    if (p == wKing && abs((int)dst - (int)src) == 2) {
        if (dst == g1) {  // Kingside
            b->board[f1] = wRook;
            b->board[h1] = EMPTY;
        } else if (dst == c1) {  // Queenside
            b->board[d1] = wRook;
            b->board[a1] = EMPTY;
        }
    }
    if (p == bKing && abs((int)dst - (int)src) == 2) {
        if (dst == g8) {  // Kingside
            b->board[f8] = bRook;
            b->board[h8] = EMPTY;
        } else if (dst == c8) {  // Queenside
            b->board[d8] = bRook;
            b->board[a8] = EMPTY;
        }
    }
    
    // Update castling rights
    if (p == wKing) {
        b->w_castle_king = 0;
        b->w_castle_queen = 0;
    } else if (p == bKing) {
        b->b_castle_king = 0;
        b->b_castle_queen = 0;
    } else if (p == wRook) {
        if (src == a1) b->w_castle_queen = 0;
        if (src == h1) b->w_castle_king = 0;
    } else if (p == bRook) {
        if (src == a8) b->b_castle_queen = 0;
        if (src == h8) b->b_castle_king = 0;
    }

   //update king position
   if(p == wKing){
        b->white_king_sq = dst;
    } else if(p == bKing){
        b->black_king_sq = dst;
    }

    b->board[src] = EMPTY; 
    b->board[dst] = p;

    // Pawn promotion
    if (p == wPawn && dst >= a8 && dst <= h8) {
        printf("White pawn promoting at %d\n", dst);
        b->board[dst] = wQueen;
    } else if (p == bPawn && dst >= a1 && dst <= h1) {
        printf("Black pawn promoting at %d\n", dst);
        b->board[dst] = bQueen;
    }

   b->turn ^= 1;
}