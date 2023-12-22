#ifndef COLORS_H_
#define COLORS_H_
//Some colors
#define WHITE 0xFF,0xFF,0xFF,0xFF
#define SNOW 0xFF,0xFA,0xFA,0xFF
#define HONEYDREW 0xF0,0xFF,0xF0,0xFF
#define BLACK 0x00,0x00,0x00,0xFF
extern SDL_Color black;
void set_color(SDL_Color* c , uint8_t r, uint8_t g, uint8_t b, uint8_t a );
void set_colors();
#endif /* COLORS _H_ */
