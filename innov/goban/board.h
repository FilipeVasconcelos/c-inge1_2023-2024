#ifndef BOARD_H_
#define BOARD_H_
#define N 19 // NxN board 
#define NMAX_SEQUENCE N*N // NxN board 
#define GOBAN_COLOR 0xEB,0xC6,0x41,0xFF
#define OFFSET 36 
#define RHOSHI 4 

#define TEXTMOVE_POSX 10 
#define TEXTMOVE_POSY 100 
//Some x y 2D vector type
typedef struct Vec2D{
    int x,y;
} Vec2D;

//Some width,height 2D vector type 
typedef struct Size{
    int w;
    int h;
}Size;
typedef struct Board{
    Size screen;
    int size;
    SDL_Rect outer;
    SDL_Rect inner;
    Vec2D offset;
    Size cellsize;
    Size half_cellsize;
    int maxcellsize;
    int rhoshi;
} Board;
int get_pos(size_t line, char key, Board board);
size_t get_line(int pos, char key, Board board);
void set_board(Board * board, int w, int h);
void hoshi(SDL_Renderer *renderer, Board *board, int i, int n);
void render_text_from_int(SDL_Renderer *renderer,TTF_Font* font,SDL_Color textColor, int i, int x, int y);
void draw_background(SDL_Renderer *renderer, 
                     Board *board, 
                     SDL_Texture* Text_row[], 
                     SDL_Texture* Text_col[],
                     SDL_Texture* Move);
#endif /* BOARD_H_ */
