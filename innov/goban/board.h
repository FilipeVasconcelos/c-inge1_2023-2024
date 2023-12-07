#ifndef BOARD_H_
#define BOARD_H_
#define N 19 // NxN board 
#define NMAX_SEQUENCE N*N // NxN board 
#define GOBAN_COLOR 0xEB,0xC6,0x41,0xFF
#define OFFSET 36 
#define RHOSHI 4 
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
void draw_background(SDL_Renderer *renderer, 
                     Board *board, 
                     SDL_Texture* Text_row[N], 
                     SDL_Texture* Text_col[N],
                     SDL_Texture* Score);
#endif /* BOARD_H_ */
