#include <stdio.h>
#include <stdlib.h>
#include "board.h"

int main(int argc, char **argv){

    Board *b = create_board();
    if(b == NULL){
        fprintf(stderr,"Error allocating memory for the board\n");
        return EXIT_FAILURE;
    }

    print_board(b);

    return EXIT_SUCCESS;
}