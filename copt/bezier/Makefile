SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CC=gcc
CFLAGS := -O3
LDFLAGS := -lm

bezier: bezier.c
	$(CC) $(CFLAGS) bezier.c -o bezier $(LDFLAGS) 

bezier_sdl: bezier_sdl.c
	$(CC) $(CFLAGS) $(SDL_CFLAGS) bezier_sdl.c -o bezier_sdl $(SDL_LDFLAGS) $(LDFLAGS) 

