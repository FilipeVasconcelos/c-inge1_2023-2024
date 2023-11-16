#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include "SDL_ttf.h"

#include "draw_circle.h"

// Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

// Some colors
#define WHITE 0xFF,0xFF,0xFF,0xFF
#define BLACK 0x00,0x00,0x00,0xFF
// Define screen dimensions
//#define SCREEN_WIDTH    800
//#define SCREEN_HEIGHT   600
#define MIN_SCREEN_W   800
#define MIN_SCREEN_H   600
int screen_width=800;
int screen_height=600;

typedef struct Vec2D{
    int x,y;
} Vec2D;

//SIZE of STONE 
#define STONE_SIZE 0.45 
#define BLACK_STONE_COLOR 0x16,0x15,0x1b,0xFF
#define WHITE_STONE_COLOR 0xEF,0xED,0xEF,0xFF
#define BLACK_STONE_STROKE 0x36,0x25,0x2b,0xFF
#define WHITE_STONE_STROKE 0xAE,0xBD,0xAF,0xFF
typedef struct Stone{
    int x;
    int y;
    int linex;
    int liney;
    int color;
} Stone;

typedef struct Size{
    int w;
    int h;
}Size;

#define N 19 // NxN goban 
#define GOBAN_COLOR 0xEB,0xC6,0x41,0xFF
#define OFFSET 36
#define RHOSHI 3 
typedef struct Goban{
    int size;
    SDL_Rect outer;
    SDL_Rect inner;
    Vec2D offset;
    Size cellsize;
    int rhoshi;
} Goban;

void set_goban(Goban * goban,int w, int h)
{
    goban->size    = N;
    goban->outer.w = MIN(w,h) * 0.75 ;
    goban->outer.h = MIN(w,h) * 0.75;
    goban->outer.x = w / 2 - goban->outer.w / 2;
    goban->outer.y = h / 2 - goban->outer.h / 2;
    goban->offset.x=36;
    goban->offset.y=36;
    goban->rhoshi=3;
    goban->cellsize.w=(goban->outer.w-2*goban->offset.x)/(N-1);
    goban->cellsize.h=(goban->outer.h-2*goban->offset.y)/(N-1);
    goban->inner.w=goban->cellsize.w*(N-1);
    goban->inner.h=goban->cellsize.h*(N-1);
    goban->inner.x=goban->outer.x+goban->offset.x;
    goban->inner.y=goban->outer.y+goban->offset.y;
}

int get_pos(int line, char key,Goban goban){
    return (key=='x') ? goban.inner.x+line*goban.cellsize.w : 
                        goban.inner.y+line*goban.cellsize.h;
}

// get line from position key if 'x' or 'y' direction
// check the position compare to half of the cell
int get_line(int pos,char key,Goban goban) {
    int line,diff,half;
    diff=(key=='x') ? (pos-goban.inner.x) : (pos-goban.inner.y);
    half=(key=='x') ? (diff%goban.cellsize.w > goban.cellsize.w*0.5) ? 1 : 0 :
                      (diff%goban.cellsize.h > goban.cellsize.h*0.5) ? 1 : 0 ;
    line=(key=='x') ? diff/goban.cellsize.w :
                      diff/goban.cellsize.h ;
 
    return ((line>=0)&&(line<goban.size)) ? line+half : -1;
}

// return false if linex and liney was already played
int linefree(int n,Stone st[],int linex,int liney)
{
    for (size_t i=0;i<n;++i){
        if ( ( linex == st[i].linex) && ( liney == st[i].liney) ){
            return 0;
        }
    }
    return 1;
}


#define NMAX_SEQUENCE N*N // NxN goban 

int main(int argc, char* argv[])
{
    // Unused argc, argv
    (void) argc;
    (void) argv;

    // Initialize SDL
    if(SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        fprintf(stderr,"SDL failed to initialise: %s\n",SDL_GetError());
        return 1;
    }

    if (TTF_Init()<0) 
    {
        fprintf(stderr, "Couldn't initialize TTF: %s\n",SDL_GetError());
        SDL_Quit();
        return 2;
    }
    // Create window
    SDL_Window *window = SDL_CreateWindow("CGoban v0.1",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          screen_width,screen_height,
                                          SDL_WINDOW_RESIZABLE);
    if(!window)
    {
       fprintf(stderr, "SDL window failed to initialise: %s\n", SDL_GetError());
       return 1;
    }
    SDL_SetWindowMinimumSize(window, MIN_SCREEN_W, MIN_SCREEN_H);
	// setting minimum size might move the window, so set position again
	SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)
    {
        fprintf(stderr,"SDL_Create_Rendeder failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //this opens a font style and sets a size
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeMono.ttf", 14);
    if(!font) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    SDL_Color textColor = {0,0,0};
    char row[20] = "ABCDEFGHJKLMNOPQRST"; 
    SDL_Surface* Surf_row[sizeof(row)];
    SDL_Surface* Surf_col[sizeof(row)];
    char *ptr=malloc(2*sizeof(char));
    char str[3];
    for (size_t i=0;i<N;i++) {
        ptr[0]=row[i];
        ptr[1]='\0';
        Surf_row[i] = TTF_RenderText_Solid(font, ptr,textColor) ;
        sprintf(str, "%d", i+1);
        Surf_col[i] = TTF_RenderText_Solid(font, str,textColor) ;
    }

    // now you can convert it into a texture
    SDL_Texture* Text_row[sizeof(row)];
    SDL_Texture* Text_col[sizeof(row)];
    for (size_t i =0; i<N;i++) {
        Text_row[i]=SDL_CreateTextureFromSurface(renderer, Surf_row[i]);
        Text_col[i]=SDL_CreateTextureFromSurface(renderer, Surf_col[i]);
    }
    
    SDL_Rect textRect;    
    // Declare a square as Rect 
    Goban board;
    set_goban(&board,screen_width,screen_height);

    Stone st[NMAX_SEQUENCE];
    Vec2D mousepos;
    Vec2D line;
    int played=0;
    bool quit = false;
    SDL_Event e;
    while(!quit)
    {

        SDL_PollEvent(&e);

        // check for messages
        switch (e.type) {
            case SDL_MOUSEBUTTONDOWN:
                 switch (e.button.button) {
                    case SDL_BUTTON_LEFT:
                        line.x=get_line(e.motion.x,'x',board);
                        line.y=get_line(e.motion.y,'y',board);
                        //printf("mouse pos : %d %d line predicted: %d %d coord px of line (0,0) %d %d\n",e.motion.x,e.motion.y,line.x,line.y,get_pos(0,'x',board),get_pos(0,'y',board));
                        if ( (line.x>=0) && (line.y>=0) && (linefree(played,st,line.x,line.y)) ) {
                            st[played].linex=line.x;
                            st[played].liney=line.y;
                            st[played].x=get_pos(line.x,'x',board);
                            st[played].y=get_pos(line.y,'y',board);
                            printf("line predicted : %d %d st inside %d %d\n",line.x,line.y,st[played].x,st[played].y);
                            played++;
                        }
                        //printf("test %d %d %d %d %d\n",e.motion.x,e.motion.y,line.x,line.y,played);
                        //SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", window);
                        break;
                    case SDL_BUTTON_RIGHT:
                        SDL_ShowSimpleMessageBox(0, "Mouse", "Right button was pressed!", window);
                        break;
                  }
                break;
            case SDL_WINDOWEVENT:
                if (e.window.event == SDL_WINDOWEVENT_RESIZED) {
                    SDL_GetWindowSize(window,&screen_width,&screen_height);
                    if (screen_width<MIN_SCREEN_W) screen_width=MIN_SCREEN_W;
                    if (screen_height<MIN_SCREEN_H) screen_height=MIN_SCREEN_H;
                    SDL_SetWindowSize(window,screen_width,screen_height);
                    set_goban(&board,screen_width,screen_height);
                    for (size_t i=0;i<played;++i){
                        st[i].x=get_pos(st[i].linex,'x',board);
                        st[i].y=get_pos(st[i].liney,'y',board);
                    }
                    printf("here event %d %d\n",board.outer.w,board.outer.h);
                    }
                break;
            case SDL_QUIT :
                quit = 1; 
                break;
            case SDL_KEYDOWN:
                switch(e.key.keysym.sym) {
                    case SDLK_q:
                        quit = 1;
                        break;
                    }
            	default:
                	break;
        }
        // Initialize renderer color white for the background
        SDL_SetRenderDrawColor(renderer, WHITE);
        // Clear screen
        SDL_RenderClear(renderer);
        // Set renderer color for the goban  
        SDL_SetRenderDrawColor(renderer, GOBAN_COLOR);
        // Draw filled goban 
        SDL_RenderFillRect(renderer, &board.outer);

        // Draw lines, hoshis stars and coordinates
        SDL_SetRenderDrawColor(renderer, BLACK);
        for (size_t i=0;i<N;i++){
            SDL_RenderDrawLine(renderer,get_pos(0,'x',board),
                                        get_pos(i,'y',board),
                                        get_pos(0,'x',board)+board.inner.w,
                                        get_pos(i,'y',board));

            SDL_RenderDrawLine(renderer,get_pos(i,'x',board),
                                        get_pos(0,'y',board),
                                        get_pos(i,'x',board),
                                        get_pos(0,'y',board)+board.inner.h);
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(Text_row[i], NULL, NULL, &texW, &texH);
            textRect.x=get_pos(i,'x',board)-texW/2;
            textRect.y=get_pos(0,'y',board)-texH*2;
            textRect.w=texW;
            textRect.h=texH;
            SDL_RenderCopy(renderer, Text_row[i], NULL, &textRect);
            SDL_QueryTexture(Text_col[18-i], NULL, NULL, &texW, &texH);
            textRect.x=get_pos(0,'x',board)-texH*2;
            textRect.y=get_pos(i,'y',board)-texW/2;
            textRect.w=texW;
            textRect.h=texH;
            SDL_RenderCopy(renderer, Text_col[18-i], NULL, &textRect);
            //Points Hoshi 3x3 points
            if ( (i==3) || (i==9) || (i==15) )
            {
                SDL_RenderFillCircle(renderer,get_pos(i,'x',board),get_pos(3 ,'y',board),board.rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',board),get_pos(9 ,'y',board),board.rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',board),get_pos(15,'y',board),board.rhoshi);
            }
        }
         for (size_t i=0;i<played;++i){
            //printf("outside %d %d %d\n",i,st[i].x,st[i].y);
            (i%2==0) ? SDL_SetRenderDrawColor(renderer, BLACK_STONE_COLOR) : SDL_SetRenderDrawColor(renderer, WHITE_STONE_COLOR);
            SDL_RenderFillCircle(renderer,st[i].x,st[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
            (i%2==0) ? SDL_SetRenderDrawColor(renderer, BLACK_STONE_STROKE) : SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
            SDL_RenderDrawCircle(renderer,st[i].x,st[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
         }
            (played%2==0) ? SDL_SetRenderDrawColor(renderer, WHITE_STONE_STROKE):SDL_SetRenderDrawColor(renderer,BLACK_STONE_STROKE);
            SDL_RenderDrawCircle(renderer,st[played].x,st[played].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE-10);
        // Update screen
        SDL_RenderPresent(renderer);
    }
    //free memory
    for (size_t i=0; i<N;i++) {
        SDL_DestroyTexture(Text_row[i]);
        SDL_DestroyTexture(Text_col[i]);
        SDL_FreeSurface(Surf_row[i]);
        SDL_FreeSurface(Surf_col[i]);
    }

    //Quit TTF related
    TTF_CloseFont(font);
    TTF_Quit();
    // Destroy renderer
    SDL_DestroyRenderer(renderer);
    // Destroy window
    SDL_DestroyWindow(window);
    // Quit SDL
    SDL_Quit();
    return 0;
}
