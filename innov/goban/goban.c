#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_ttf.h"
#include "SDL2/SDL_image.h"
#include "draw_circle.h"

#define BLOCK_SIZE 24
#define BLOCK_COUNT 10
#define MAX_LEN 300
//Define MAX and MIN macros
#define MAX(X, Y) (((X) > (Y)) ? (X) : (Y))
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

//Some colors
#define WHITE 0xFF,0xFF,0xFF,0xFF
#define SNOW 0xFF,0xFA,0xFA,0xFF
#define HONEYDREW 0xF0,0xFF,0xF0,0xFF
#define BLACK 0x00,0x00,0x00,0xFF
//Define screen dimensions
//#define SCREEN_WIDTH    800
//#define SCREEN_HEIGHT   600
#define MIN_SCREEN_W   800
#define MIN_SCREEN_H   600
int screen_width=800;
int screen_height=600;

//  
typedef struct Vec2D{
    int x,y;
} Vec2D;

// return line index 0 to N-1 
// from char c, 'a' is the reference  
size_t sgftoline(char c){
    return (size_t) c - (size_t) 'a';
}

//STONE related 
#define STONE_SIZE 0.45 
#define MARK_SIZE 0.21
#define BLACK_STONE_COLOR 0x16,0x15,0x1b,0xFF
#define WHITE_STONE_COLOR 0xEE,0xEF,0xEE,0xFF
#define BLACK_STONE_STROKE 0xFE,0xFD,0xFF,0xFF
#define WHITE_STONE_STROKE 0x26,0x15,0x2b,0xFF
typedef struct Stone{
    int x;
    int y;
    size_t linex;
    size_t liney;
} Stone;
//width,height size
typedef struct Size{
    int w;
    int h;
}Size;

// Goban related
#define N 19 // NxN goban 
#define NMAX_SEQUENCE N*N // NxN goban 
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

// goban :
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
void set_goban(Goban * goban, int w, int h)
{
    goban->size    = N;
    goban->outer.w = MIN(w,h) * 0.75 ;
    goban->outer.h = MIN(w,h) * 0.80;
    goban->outer.x = w / 2 - goban->outer.w / 2;
    goban->outer.y = h / 2 - goban->outer.h / 2;
    goban->offset.x=OFFSET;
    goban->offset.y=OFFSET;
    goban->rhoshi=RHOSHI;
    goban->cellsize.w=(goban->outer.w-2*goban->offset.x)/(N-1);
    goban->cellsize.h=(goban->outer.h-2*goban->offset.y)/(N-1);
    goban->inner.w=goban->cellsize.w*(N-1);
    goban->inner.h=goban->cellsize.h*(N-1);
    goban->inner.x=goban->outer.x+goban->offset.x;
    goban->inner.y=goban->outer.y+goban->offset.y;
}

// get pixel position from line :
// line is a size_t between 1 and 19 
// key is 'x' or 'y'
int get_pos(size_t line, char key, Goban goban){
    return (key=='x') ? goban.inner.x+line*goban.cellsize.w : 
                        goban.inner.y+line*goban.cellsize.h;
}

// get line from position key if 'x' or 'y' direction
// check the position compare to half of the cell
size_t get_line(int pos, char key, Goban goban) {
    size_t line,diff,half;
    diff=(key=='x') ? (pos-goban.inner.x) : (pos-goban.inner.y);
    half=(key=='x') ? (diff%goban.cellsize.w > goban.cellsize.w*0.5) ? 1 : 0 :
                      (diff%goban.cellsize.h > goban.cellsize.h*0.5) ? 1 : 0 ;
    line=(key=='x') ? diff/goban.cellsize.w :
                      diff/goban.cellsize.h ;
 
    return ((line>=0)&&(line<goban.size)) ? line+half : -1;
}

//reset line and pos from linex,liney and goban size
void set_line_and_pos(Stone *st, size_t linex, size_t liney , Goban goban){
    st->linex=linex;
    st->liney=liney;
    st->x=get_pos(linex,'x',goban);
    st->y=get_pos(liney,'y',goban);
}

// return false if linex and liney was already played
bool linefree(size_t n,Stone stones[],size_t linex,size_t liney)
{
    for (size_t i=0;i<n;++i){
        if ( ( linex == stones[i].linex) && ( liney == stones[i].liney) ){
            return false;
        }
    }
    return true;
}
// render hoshi stars at line i for any n goban size
void hoshi(SDL_Renderer *renderer, Goban *goban, int i, int n) {
    if ( n==9 )  {
        if ( (i==2) || (i==6) ) {
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(2 ,'y',*goban),goban->rhoshi);
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(6 ,'y',*goban),goban->rhoshi);
        }
        if (i==4)  SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(4 ,'y',*goban),goban->rhoshi);  
    }
    if ( n==13 ) {
        if ( (i==3) || (i==9) ) {
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(3 ,'y',*goban),goban->rhoshi);
            SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(9 ,'y',*goban),goban->rhoshi);
        }
        if (i==6)  SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(6 ,'y',*goban),goban->rhoshi);  
    }
    if ( n==19 ) {
        if ( (i==3) || (i==9) || (i==15) ) {
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(3 ,'y',*goban),goban->rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(9 ,'y',*goban),goban->rhoshi);
                SDL_RenderFillCircle(renderer,get_pos(i,'x',*goban),get_pos(15,'y',*goban),goban->rhoshi);
        }
    }
}

/******************************************************************************
 * draw all background, lines, coordinates, hoshis
 *****************************************************************************/
void draw_background(SDL_Renderer *renderer, Goban *goban, SDL_Texture* Text_row[N], SDL_Texture* Text_col[N])
{
    SDL_Rect textRect;    
    // Initialize renderer color white for the background
    SDL_SetRenderDrawColor(renderer, HONEYDREW);
    // Clear screen
    SDL_RenderClear(renderer);
    // Set renderer color for the goban  
    SDL_SetRenderDrawColor(renderer, GOBAN_COLOR);
    // Draw filled goban 
    SDL_RenderFillRect(renderer, &(goban->outer));
    // Draw lines, hoshis stars and coordinates
    SDL_SetRenderDrawColor(renderer, BLACK);
    for (size_t i=0;i<N;i++){
        // Draw lines
        // boucle sur -1 0 1 epaisseur de 3
        for (int ep=-1;ep<1;++ep)
        {
            //lignes horizontales
            SDL_RenderDrawLine(renderer,get_pos(0,'x',*goban)+ep,
                                        get_pos(i,'y',*goban)+ep,
                                        get_pos(0,'x',*goban)+goban->inner.w+ep,
                                        get_pos(i,'y',*goban)+ep);
            //lignes verticales
            SDL_RenderDrawLine(renderer,get_pos(i,'x',*goban)+ep,
                                        get_pos(0,'y',*goban)+ep,
                                        get_pos(i,'x',*goban)+ep,
                                        get_pos(0,'y',*goban)+goban->inner.h+ep);
        }
            //draw text 
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(Text_row[i], NULL, NULL, &texW, &texH);
            textRect.x=get_pos(i,'x',*goban)-texW/2;
            textRect.y=get_pos(0,'y',*goban)-texH*2;
            textRect.w=texW;
            textRect.h=texH;
            SDL_RenderCopy(renderer, Text_row[i], NULL, &textRect);
            SDL_QueryTexture(Text_col[N-1-i], NULL, NULL, &texW, &texH);
            textRect.x=get_pos(0,'x',*goban)-texW-10;
            textRect.y=get_pos(i,'y',*goban)-texH/2;
            textRect.w=texW;
            textRect.h=texH;
            SDL_RenderCopy(renderer, Text_col[N-1-i], NULL, &textRect);

            //Draw all Hoshi points
            hoshi(renderer, goban, i, N);
        }
}


int main(int argc, char* argv[])
{
    Goban board;
    set_goban(&board,screen_width,screen_height);
    Stone stones[NMAX_SEQUENCE];
    size_t played=0;
    size_t showed=0;
    bool readingfromfile=false;
    Vec2D line;
    FILE* fptr;
    char sgfilename[MAX_LEN];
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

    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSansBold.ttf", 16);
    if(!font) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    SDL_Color textColor = {0,0,0};
    char row[26] = "ABCDEFGHJKLMNOPQRSTUVWXYZ"; 
    SDL_Surface* Surf_row[N];
    SDL_Surface* Surf_col[N];
    char *ptr=malloc(2*sizeof(char));
    char str[3];
    for (size_t i=0;i<N;i++) {
        ptr[0]=row[i];
        ptr[1]='\0';
        Surf_row[i] = TTF_RenderText_Solid(font, ptr,textColor) ;
        sprintf(str, "%ld", i+1);
        Surf_col[i] = TTF_RenderText_Solid(font, str,textColor) ;
    }
    // now you can convert it into a texture
    SDL_Texture* Text_row[N];
    SDL_Texture* Text_col[N];
    for (size_t i =0; i<N;i++) {
        Text_row[i]=SDL_CreateTextureFromSurface(renderer, Surf_row[i]);
        Text_col[i]=SDL_CreateTextureFromSurface(renderer, Surf_col[i]);
    }
 
    SDL_Surface* wpng = IMG_Load("img/white.png");
    SDL_Surface* bpng = IMG_Load("img/black.png");
    if ((!wpng)||(!bpng))
    {
        printf("Erreur de chargement de l'image : %s",SDL_GetError());
        return -1;
    }

    SDL_Rect src1={0, 0, 0, 0};
    SDL_Texture* wstone = SDL_CreateTextureFromSurface(renderer,wpng); 
    SDL_Texture* bstone = SDL_CreateTextureFromSurface(renderer,bpng); 
    SDL_MapRGBA( wpng->format, 0, 0, 0, SDL_ALPHA_TRANSPARENT );
    SDL_QueryTexture(wstone, NULL, NULL, &src1.w, &src1.h);
    SDL_QueryTexture(bstone, NULL, NULL, &src1.w, &src1.h);


    // background
    draw_background(renderer,&board,Text_row,Text_col);
    // if reading from file
    if (argc > 1 ) {
        printf("reading sgf file %s\n",*(argv+1));
        readingfromfile=true;
        strcpy(sgfilename,*(argv+1));
        //open SGF file
        fptr=fopen(sgfilename,"r");
        if ( ! fptr ) {
            fprintf( stderr, "Cannot open file for reading\n" );
            exit(-1);
        }
        char * rline = NULL;
        size_t len = 0;
        while(getline(&rline, &len, fptr) != -1) {
            if (strlen(rline) == 7 ){
                stones[played].linex=sgftoline(rline[3]);
                stones[played].liney=sgftoline(rline[4]);
                stones[played].x=get_pos(stones[played].linex,'x',board);
                stones[played].y=get_pos(stones[played].liney,'y',board);
                played++;
            }
        }
        printf("played %ld",played);
    
        int returnCode = fclose( fptr );
        free(rline);
        if ( returnCode == EOF ) {
            fprintf( stderr, "Cannot close file\n" );
            exit( -1 );
        }
    }

    Vec2D mousepos;
    bool quit = false;
    SDL_Event e;
    while(!quit)
    {
        if (SDL_WaitEvent(&e) != 0) {
            // check for messages
            switch (e.type) {
                case SDL_MOUSEBUTTONDOWN:
                     switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            if (readingfromfile) {
                                break;
                            }
                            line.x=get_line(e.motion.x,'x',board);
                            line.y=get_line(e.motion.y,'y',board);
                            if ( (line.x>=0)                && 
                                 (line.y>=0)                && 
                                 (line.x<board.size)        && 
                                 (line.y<board.size)        && 
                                 (linefree(played,stones,line.x,line.y)) ) {

                                set_line_and_pos(stones+played,line.x,line.y,board);
                                played++;
                                showed++;
                            }
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
                        draw_background(renderer,&board,Text_row,Text_col);
                        //reset position in pixel from new board size
                        for (size_t i=0;i<played;++i){
                            set_line_and_pos(stones+i,stones[i].linex,stones[i].liney,board);
                        }
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
                        case SDLK_SPACE:
                            (showed+1<=played) ? showed++ : showed;
                            //printf("showed %ld",showed);
                            break;
                        case SDLK_r:
                            (showed>0) ? showed-- : showed;
                            draw_background(renderer,&board,Text_row,Text_col);
                            //printf("showed %ld",showed);
                            break;
                    }
               	default:
                   	break;
            }
        }
        //draw stones
        for (size_t i=0;i<showed;++i){
        //    (i%2==0) ? SDL_SetRenderDrawColor(renderer, BLACK_STONE_COLOR) : SDL_SetRenderDrawColor(renderer, WHITE_STONE_COLOR);
         //   SDL_RenderFillCircle(renderer,stones[i].x,stones[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
            SDL_Rect dst1={ stones[i].x-board.cellsize.w*0.5, stones[i].y-board.cellsize.h*0.5, 2*MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE, 2*MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE };
//            S(i%2==0) ? DL_SetTextureBlendMode(wstone, SDL_BLENDMODE_NONE) : 
            (i%2==0) ? SDL_RenderCopy(renderer, wstone, &src1, &dst1) : SDL_RenderCopy(renderer, bstone, &src1, &dst1);
//            SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
 //           SDL_RenderDrawCircle(renderer,stones[i].x,stones[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
//            SDL_RenderDrawCircle(renderer,stones[i].x,stones[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE-1);
        }
        ((showed-1)%2==0) ? SDL_SetRenderDrawColor(renderer,BLACK_STONE_STROKE):
                            SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
//        SDL_RenderDrawCircle(renderer,stones[showed-1].x,stones[showed-1].y,MAX(board.cellsize.w,board.cellsize.h)*MARK_SIZE);
 //       SDL_RenderDrawCircle(renderer,stones[showed-1].x,stones[showed-1].y,MAX(board.cellsize.w,board.cellsize.h)*MARK_SIZE+1);


        // Update screen
        SDL_RenderPresent(renderer);
    } // quit loop
    

    // free memory Texture and Surfaces
    for (size_t i=0; i<N;i++) {
        SDL_DestroyTexture(Text_row[i]);
        SDL_DestroyTexture(Text_col[i]);
        SDL_FreeSurface(Surf_row[i]);
        SDL_FreeSurface(Surf_col[i]);
    }
    SDL_DestroyTexture(wstone); 

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
