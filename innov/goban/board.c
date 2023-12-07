// ----------------------------------------------------------------------------
//                           Board related
// ----------------------------------------------------------------------------
#include <stdlib.h>
#include <SDL2/SDL.h>
#include "draw_circle.h"
#include "colors.h"
#include "board.h"
#include "tools.h"

// get pixel position from line :
// line is a size_t between 1 and 19 
// key is 'x' or 'y'
int get_pos(size_t line, char key, Board board){
    return (key=='x') ? board.inner.x+line*board.cellsize.w : 
                        board.inner.y+line*board.cellsize.h;
}

// get line from position key if 'x' or 'y' direction
// check the position compare to half of the cell
size_t get_line(int pos, char key, Board board) {
    size_t line,diff,half;
    diff=(key=='x') ? (pos-board.inner.x) : (pos-board.inner.y);
    half=(key=='x') ? (diff%board.cellsize.w > board.half_cellsize.w) ? 1 : 0 :
                      (diff%board.cellsize.h > board.half_cellsize.h) ? 1 : 0 ;
    line=(key=='x') ? diff/board.cellsize.w :
                      diff/board.cellsize.h ;
 
    return ((line>=0)&&(line<board.size)) ? line+half : -1;
}

// board :
//  w : screen_width
//  h : screen_height
//
//     <---------------- w ---------------->
// ^   +-----------------------------------+
// |   |            ^    
// |   |            | outer.h 
// |   |            | 
// h   |            v 
// |   | <------->  +----------------------+
// |   |  outer.w   |         ^
//                  |         | inner.h 
//                  | <---->  v
// v                | inner.w +---+---+---+
//                            |   |   |   |
//                            +---+---+---+
void set_board(Board * board, int w, int h) {
    board->size    = N;
    board->outer.w = MIN(w,h) * 0.75 ;
    board->outer.h = MIN(w,h) * 0.75;
    board->outer.x = w / 2 - board->outer.w / 2;
    board->outer.y = h / 2 - board->outer.h / 2;
    board->offset.x=OFFSET;
    board->offset.y=OFFSET;
    board->rhoshi=RHOSHI;
    board->cellsize.w=(board->outer.w-2*board->offset.x)/(N-1);
    board->cellsize.h=(board->outer.h-2*board->offset.y)/(N-1);
    board->maxcellsize = MAX(board->cellsize.w,board->cellsize.h);
    board->half_cellsize.w=board->cellsize.w/2;
    board->half_cellsize.h=board->cellsize.h/2;
    board->inner.w=board->cellsize.w*(N-1);
    board->inner.h=board->cellsize.h*(N-1);
    board->inner.x=board->outer.x+board->offset.x;
    board->inner.y=board->outer.y+board->offset.y;
}

// render hoshi stars at line i for any n board size
void hoshi(SDL_Renderer *renderer, Board *board, int i, int n) {
    if ( n==9 )  {
        if ( (i==2) || (i==6) ) {
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(2 ,'y',*board),board->rhoshi);
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(6 ,'y',*board),board->rhoshi);
        }
        if (i==4)  SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(4 ,'y',*board),board->rhoshi);  
    }
    if ( n==13 ) {
        if ( (i==3) || (i==9) ) {
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(3 ,'y',*board),board->rhoshi);
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(9 ,'y',*board),board->rhoshi);
        }
        if (i==6)  SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(6 ,'y',*board),board->rhoshi);  
    }
    if ( n==19 ) {
        if ( (i==3) || (i==9) || (i==15) ) {
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(3 ,'y',*board),board->rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(9 ,'y',*board),board->rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*board),get_pos(15,'y',*board),board->rhoshi);
        }
    }
}
// draw all background, lines, coordinates, hoshis
void draw_background(SDL_Renderer *renderer, 
                     Board *board, 
                     SDL_Texture* Text_row[N], 
                     SDL_Texture* Text_col[N],
                     SDL_Texture* Text_move,
                     size_t showed) {
    SDL_Rect textRect;    
    // Initialize renderer color white for the background
    SDL_SetRenderDrawColor(renderer, HONEYDREW);
    // Clear screen
    SDL_RenderClear(renderer);
    // Set renderer color for the board  
    SDL_SetRenderDrawColor(renderer, GOBAN_COLOR);
    // Draw filled board 
    SDL_RenderFillRect(renderer, &(board->outer));
    // Draw lines, hoshis stars and coordinates
    SDL_SetRenderDrawColor(renderer, BLACK);
    for (size_t i=0;i<N;i++){
        // Draw lines
        // boucle sur -1 0 1 epaisseur de 3
        for (int ep=-1;ep<1;++ep)
        {
            //lignes horizontales
            SDL_RenderDrawLine(renderer,get_pos(0,'x',*board)+ep,
                                        get_pos(i,'y',*board)+ep,
                                        get_pos(0,'x',*board)+board->inner.w+ep,
                                        get_pos(i,'y',*board)+ep);
            //lignes verticales
            SDL_RenderDrawLine(renderer,get_pos(i,'x',*board)+ep,
                                        get_pos(0,'y',*board)+ep,
                                        get_pos(i,'x',*board)+ep,
                                        get_pos(0,'y',*board)+board->inner.h+ep);
        }
        int texW = 0;
        int texH = 0;
        sprintf(str, "%ld", showed);
        SDL_Surface* Surf_moveValue=TTF_RenderText_Solid(font, str ,textColor);
        SDL_Texture* Text_moveValue=SDL_CreateTextureFromSurface(renderer, Surf_moveValue);
        //draw Text_move 
        SDL_QueryTexture(Text_moveValue, NULL, NULL, &texW, &texH);
        textRect.x=10;
        textRect.y=120;
        textRect.w=texW;
        textRect.h=texH;
        SDL_RenderCopy(renderer, Text_moveValue, NULL, &textRect);
        //draw Text_move 
        SDL_QueryTexture(Text_move, NULL, NULL, &texW, &texH);
        textRect.x=10;
        textRect.y=100;
        textRect.w=texW;
        textRect.h=texH;
        SDL_RenderCopy(renderer, Text_move, NULL, &textRect);
        //draw Text_row 1 2 3 ... 19
        SDL_QueryTexture(Text_row[i], NULL, NULL, &texW, &texH);
        textRect.x=get_pos(i,'x',*board)-texW/2;
        textRect.y=get_pos(0,'y',*board)-texH*2;
        textRect.w=texW;
        textRect.h=texH;
        SDL_RenderCopy(renderer, Text_row[i], NULL, &textRect);
        //draw Text_col A B C D ... S T
        SDL_QueryTexture(Text_col[N-1-i], NULL, NULL, &texW, &texH);
        textRect.x=get_pos(0,'x',*board)-texW-10;
        textRect.y=get_pos(i,'y',*board)-texH/2;
        textRect.w=texW;
        textRect.h=texH;
        SDL_RenderCopy(renderer, Text_col[N-1-i], NULL, &textRect);

        //Draw all Hoshi points
        hoshi(renderer, board, i, N);
    }
}
