SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CPP=gcc
CFLAGS := $(SDL_CFLAGS) -O3
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_ttf

OBJ=draw_circle.o
EXEC=goban

all: ${EXEC}

${EXEC}: ${EXEC}.o $(OBJ)
	${CPP} $(CFLAGS) -o $@ $^ ${LDFLAGS}

%.o : %.c
	$(CPP) $(CFLAGS) -o $@ -c $<


clean:  
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}

