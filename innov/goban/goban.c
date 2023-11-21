#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include <SDL.h>
#include "SDL_ttf.h"

#include "draw_circle.h"

#define BLOCK_SIZE 24
#define BLOCK_COUNT 10

#define MAX_LEN 300
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

size_t sgftoline(char c){
    return (size_t) c - (size_t) 'a';
}

//SIZE of STONE 
#define STONE_SIZE 0.45 
#define MARK_SIZE 0.21
#define BLACK_STONE_COLOR 0x16,0x15,0x1b,0xFF
#define WHITE_STONE_COLOR 0xEF,0xED,0xEF,0xFF
#define BLACK_STONE_STROKE 0xFE,0xFD,0xFF,0xFF
#define WHITE_STONE_STROKE 0x36,0x25,0x2b,0xFF
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
#define RHOSHI 5 
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

void draw_background(SDL_Renderer *renderer, Goban * goban){
    // Initialize renderer color white for the background
    SDL_SetRenderDrawColor(renderer, WHITE);
    // Clear screen
    SDL_RenderClear(renderer);
    // Set renderer color for the goban  
    SDL_SetRenderDrawColor(renderer, GOBAN_COLOR);
    // Draw filled goban 
    SDL_RenderFillRect(renderer, &(goban->outer));

}
void draw_lines(SDL_Renderer *renderer, Goban board, SDL_Texture* Text_row[N], SDL_Texture* Text_col[N]){
    // Draw lines, hoshis stars and coordinates
    SDL_Rect textRect;    
    SDL_SetRenderDrawColor(renderer, BLACK);
    for (size_t i=0;i<N;i++){
        // Draw lines
        // boucle sur -1 0 1 epaisseur de 3
        for (int ep=-1;ep<1;++ep)
        {
            //lignes horizontales
            SDL_RenderDrawLine(renderer,get_pos(0,'x',board)+ep,
                                        get_pos(i,'y',board)+ep,
                                        get_pos(0,'x',board)+board.inner.w+ep,
                                        get_pos(i,'y',board)+ep);
            //lignes verticales
            SDL_RenderDrawLine(renderer,get_pos(i,'x',board)+ep,
                                        get_pos(0,'y',board)+ep,
                                        get_pos(i,'x',board)+ep,
                                        get_pos(0,'y',board)+board.inner.h+ep);
        }
            //draw text 
            int texW = 0;
            int texH = 0;
            SDL_QueryTexture(Text_row[i], NULL, NULL, &texW, &texH);
            textRect.x=get_pos(i,'x',board)-texW/2;
            textRect.y=get_pos(0,'y',board)-texH*2;
            textRect.w=texW;
            textRect.h=texH;
            SDL_RenderCopy(renderer, Text_row[i], NULL, &textRect);
            SDL_QueryTexture(Text_col[18-i], NULL, NULL, &texW, &texH);
                textRect.x=get_pos(0,'x',board)-texW-10;
                textRect.y=get_pos(i,'y',board)-texH/2;
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
}

#define NMAX_SEQUENCE N*N // NxN goban 

int main(int argc, char* argv[])
{
    Goban board;
    set_goban(&board,screen_width,screen_height);
    Stone st[NMAX_SEQUENCE];
    size_t played=0;
    size_t showed=0;
    int readingfromfile=0;
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
    //SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    //this opens a font style and sets a size
    // /usr/share/fonts/truetype/freefont/FreeMono.ttf
	// /usr/share/fonts/truetype/freefont/FreeMonoBold.ttf
	// /usr/share/fonts/truetype/freefont/FreeMonoBoldOblique.ttf
	// /usr/share/fonts/truetype/freefont/FreeMonoOblique.ttf
	// /usr/share/fonts/truetype/freefont/FreeSans.ttf
	// /usr/share/fonts/truetype/freefont/FreeSansBold.ttf
	// /usr/share/fonts/truetype/freefont/FreeSansBoldOblique.ttf
	// /usr/share/fonts/truetype/freefont/FreeSansOblique.ttf
	// /usr/share/fonts/truetype/freefont/FreeSerif.ttf
	// /usr/share/fonts/truetype/freefont/FreeSerifBold.ttf
	// /usr/share/fonts/truetype/freefont/FreeSerifBoldItalic.ttf
	// /usr/share/fonts/truetype/freefont/FreeSerifItalic.ttf
    TTF_Font* font = TTF_OpenFont("/usr/share/fonts/truetype/freefont/FreeSansBold.ttf", 16);
    if(!font) 
    {
        fprintf(stderr,"TTF_OpenFont failed to initialise: %s\n",SDL_GetError());
        return 1;
    }
    SDL_Color textColor = {0,0,0};
    char row[20] = "ABCDEFGHJKLMNOPQRST"; 
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
    // if reading from file
    if (argc > 1 ) {
        printf("reading sgf file %s\n",*(argv+1));
        readingfromfile=1;
        strcpy(sgfilename,*(argv+1));
        fptr=fopen(sgfilename,"r");
        if ( ! fptr ) {
            fprintf( stderr, "Cannot open file for reading\n" );
            exit(-1);
        }
        char * rline = NULL;
        size_t len = 0;
        while(getline(&rline, &len, fptr) != -1) {
            printf("line length: %zd\n", strlen(rline));
            printf("%s",rline);
            if (strlen(rline) == 7 ){
                st[played].linex=sgftoline(rline[3]);
                st[played].liney=sgftoline(rline[4]);
                st[played].x=get_pos(st[played].linex,'x',board);
                st[played].y=get_pos(st[played].liney,'y',board);
                printf("%ld %d %d %d %d\n",played,st[played].linex,st[played].linex,st[played].x,st[played].y);
                played++;
            }
        }

    
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
    // background
    draw_background(renderer,&board);
    //lines+hoshis+coordinates
    draw_lines(renderer,board,Text_row,Text_col);
    while(!quit)
    {
        if (SDL_WaitEvent(&e) != 0) {
            // check for messages
            switch (e.type) {
                case SDL_MOUSEBUTTONDOWN:
                     switch (e.button.button) {
                        case SDL_BUTTON_LEFT:
                            printf("readingformfile %d",readingfromfile);
                            if (readingfromfile) break;
                            line.x=get_line(e.motion.x,'x',board);
                            line.y=get_line(e.motion.y,'y',board);
                            if ( (line.x>=0)                && 
                                 (line.y>=0)                && 
                                 (line.x<board.size)        && 
                                 (line.y<board.size)        && 
                                 (linefree(played,st,line.x,line.y)) ) {
                                st[played].linex=line.x;
                                st[played].liney=line.y;
                                st[played].x=get_pos(line.x,'x',board);
                                st[played].y=get_pos(line.y,'y',board);
                                played++;
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
                        draw_background(renderer,&board);
                        //lines+hoshis+coordinates
                        draw_lines(renderer,board,Text_row,Text_col);
                        //reset position in pixel from new board size
                        for (size_t i=0;i<played;++i){
                            st[i].x=get_pos(st[i].linex,'x',board);
                            st[i].y=get_pos(st[i].liney,'y',board);
                        }
                        //printf("here event %d %d\n",board.outer.w,board.outer.h);
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
                            (showed+1<played) ? showed++ : showed;
                            //printf("showed %ld",showed);
                            break;
                        case SDLK_r:
                            (showed>0) ? showed-- : showed;
                            //printf("showed %ld",showed);
                            break;
                    }
               	default:
                   	break;
            }
        }
        //draw stones
        for (size_t i=0;i<showed;++i){
            (i%2==0) ? SDL_SetRenderDrawColor(renderer, BLACK_STONE_COLOR) : SDL_SetRenderDrawColor(renderer, WHITE_STONE_COLOR);
            SDL_RenderFillCircle(renderer,st[i].x,st[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
            SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
            SDL_RenderDrawCircle(renderer,st[i].x,st[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE);
            SDL_RenderDrawCircle(renderer,st[i].x,st[i].y,MAX(board.cellsize.w,board.cellsize.h)*STONE_SIZE-1);
        }
        ((showed-1)%2==0) ? SDL_SetRenderDrawColor(renderer,BLACK_STONE_STROKE):
                            SDL_SetRenderDrawColor(renderer,WHITE_STONE_STROKE);
        SDL_RenderDrawCircle(renderer,st[showed-1].x,st[showed-1].y,MAX(board.cellsize.w,board.cellsize.h)*MARK_SIZE);
        SDL_RenderDrawCircle(renderer,st[showed-1].x,st[showed-1].y,MAX(board.cellsize.w,board.cellsize.h)*MARK_SIZE+1);
        // Update screen
        SDL_RenderPresent(renderer);
    } // quit loop
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
