SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CPP=gcc
CFLAGS := $(SDL_CFLAGS) -O3
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_ttf -lSDL2_image

OBJ=tools.o draw_circle.o board.o stone.o sgf.o
EXEC=goban

all: ${EXEC}

${EXEC}:  $(OBJ) ${EXEC}.o
	${CPP} $(CFLAGS) -o $@ $^ ${LDFLAGS}

%.o : %.c
	$(CPP) $(CFLAGS) -o $@ -c $<


clean:  
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}

