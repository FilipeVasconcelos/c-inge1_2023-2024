SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CC=gcc
CFLAGS := $(SDL_CFLAGS) -O3
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_ttf -lSDL2_image

OBJ=colors.o tools.o draw_circle.o board.o stone.o sgf.o goban.o
EXEC=goban


goban: $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -I./ $(CFLAGS) $(CPPDEFS) -o $@ -c $<

clean:  
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}

