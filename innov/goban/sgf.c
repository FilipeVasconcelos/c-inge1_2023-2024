#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<SDL2/SDL.h>
#include"SDL2/SDL_ttf.h"
#include"tools.h"
#include"board.h"
#include"stone.h"
#include"sgf.h"

void read_sgfile(char* sgfilename, Board board, Stone stones[], size_t* played){

    FILE* fptr;
    //open SGF file
    fptr=fopen(sgfilename,"r");
    if ( ! fptr ) {
        fprintf( stderr, "cannot acces %s: no such file or directory\n",sgfilename );
        exit(-1);
    }
    char * rline = NULL;
    size_t len = 0;
    while(getline(&rline, &len, fptr) != -1) {
        if (strlen(rline) == 7 ){
            stones[*played].linex=sgftoline(rline[3]);
            stones[*played].liney=sgftoline(rline[4]);
            stones[*played].x=get_pos(stones[*played].linex,'x',board);
            stones[*played].y=get_pos(stones[*played].liney,'y',board);
            stones[*played].sprite.x=stones[*played].x - board.half_cellsize.w;
            stones[*played].sprite.y=stones[*played].y - board.half_cellsize.h;
            stones[*played].sprite.w=board.maxcellsize;
            stones[*played].sprite.h=board.maxcellsize;
            (*played)++;
        }
    }
    printf("reading %ld moves\n",*played);
    int returnCode = fclose(fptr);
    free(rline);
    if ( returnCode == EOF ) {
        fprintf( stderr, "Cannot close file\n" );
        exit( -1 );
    }
    
}
