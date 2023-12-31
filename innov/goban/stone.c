#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
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
// render last stone + marker on stone
void render_last_stone(SDL_Renderer *renderer, Board board,  SDL_Rect img, 
                       SDL_Texture* wstone, SDL_Texture* bstone, Stone st, size_t showed){
    render_stone(renderer,st,img,wstone,bstone,showed);
    render_marker_on_stone(renderer,board,st,showed);
}

//render stone
void render_stone(SDL_Renderer *renderer, Stone st, SDL_Rect img, SDL_Texture* wstone, SDL_Texture* bstone,  size_t showed ){
    (showed%2==0) ? SDL_RenderCopy(renderer, bstone, &img, &(st.sprite)) : 
                    SDL_RenderCopy(renderer, wstone, &img, &(st.sprite));
}
//render stones
void render_stones(SDL_Renderer *renderer, Stone st[], SDL_Rect img, SDL_Texture* wstone, SDL_Texture* bstone,  size_t showed ){
    if (showed == 0 ) return ;
    //display stones sprite
    for (size_t i=0;i<showed-1;++i){
        render_stone(renderer,st[i],img,wstone,bstone,i);
    }
}

// render marker on stone usually used on last stone
void render_marker_on_stone(SDL_Renderer *renderer,Board board, Stone st, size_t i){
    (i%2==0) ? SDL_SetRenderDrawColor(renderer,BLACK_STONE_STROKE):
               SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
    SDL_RenderDrawCircle(renderer,st.x,st.y,board.maxcellsize*MARK_SIZE);
    SDL_RenderDrawCircle(renderer,st.x,st.y,board.maxcellsize*MARK_SIZE+1);
}

