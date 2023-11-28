#include <stdbool.h>
#include <SDL2/SDL.h>
#include "draw_circle.h"
#include "board.h"
#include "stone.h"

// set/reset stone from linex,liney and board size
void set_stone(Stone *st, size_t linex, size_t liney , Board board, int color){
    st->linex=linex;
    st->liney=liney;
    st->x=get_pos(linex,'x',board);
    st->y=get_pos(liney,'y',board);
    st->sprite.x=st->x-board.half_cellsize.w;
    st->sprite.y=st->y-board.half_cellsize.h;
    st->sprite.w=board.maxcellsize;
    st->sprite.h=board.maxcellsize;
    if ( color != -1 ) st->color=color;
}
// return false if linex and liney is already occupied (played)
bool linefree(size_t n,Stone stones[],size_t linex,size_t liney)
{
    for (size_t i=0;i<n;++i){
        if ( ( linex == stones[i].linex) && 
             ( liney == stones[i].liney)    ){
            return false;
        }
    }
    return true;
}

// render marker on stone usually used on last stone
void render_marker_on_stone(SDL_Renderer *renderer,Board board, Stone st, size_t i){
    (i%2==0) ? SDL_SetRenderDrawColor(renderer,BLACK_STONE_STROKE):
               SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
    SDL_RenderDrawCircle(renderer,st.x,st.y,board.maxcellsize*MARK_SIZE);
    SDL_RenderDrawCircle(renderer,st.x,st.y,board.maxcellsize*MARK_SIZE+1);
}

