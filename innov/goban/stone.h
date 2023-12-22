#ifndef STONE_H_
#define STONE_H_
// ----------------------------------------------------------------------------
//                           Stone related constants
// ----------------------------------------------------------------------------
//#define STONE_SIZE 0.5 //half cellsize
#include "board.h"
#define MARK_SIZE 0.24
#define BLACK_STONE_COLOR 0x16,0x15,0x1b,0xFF
#define WHITE_STONE_COLOR 0xEE,0xEF,0xEE,0xFF
#define BLACK_STONE_STROKE 0xEE,0xED,0xEF,0xFF
#define WHITE_STONE_STROKE 0x26,0x25,0x2b,0xFF
typedef struct Stone{
    SDL_Rect sprite;
    int x;
    int y;
    size_t linex;
    size_t liney;
    int color;
} Stone;

void set_stone(Stone *st, size_t linex, size_t liney , Board board, int color);
bool linefree(size_t n,Stone stones[],size_t linex,size_t liney);
void render_stone(SDL_Renderer *renderer, Stone st, SDL_Rect img, SDL_Texture* wstone, SDL_Texture* bstone,  size_t showed );
void render_last_stone(SDL_Renderer *renderer, Board board,  SDL_Rect img, 
                       SDL_Texture* wstone, SDL_Texture* bstone, Stone st, size_t showed);
void render_stones(SDL_Renderer *renderer, Stone st[], SDL_Rect img, SDL_Texture* wstone, SDL_Texture* bstone,  size_t showed );
void render_marker_on_stone(SDL_Renderer *renderer,Board board, Stone st, size_t i);
#endif /* STONE_H_ */
