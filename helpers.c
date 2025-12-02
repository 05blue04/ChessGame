#include "board.h"

const char* piece_to_string(int piece) {
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