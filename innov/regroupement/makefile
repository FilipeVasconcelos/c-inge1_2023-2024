SDL_CFLAGS := $(shell sdl2-config --cflags)
SDL_LDFLAGS := $(shell sdl2-config --libs)

CPP=gcc
CFLAGS := $(SDL_CFLAGS) -O0
LDFLAGS := $(SDL_LDFLAGS) -lSDL2_ttf

EXEC= bougertexte

all: ${EXEC}

${EXEC}: ${EXEC}.o
	${CPP} $(CFLAGS) -o ${EXEC} ${EXEC}.o ${LDFLAGS}

${EXEC}.o: ${EXEC}.c
	${CPP} $(CFLAGS) -o ${EXEC}.o -c ${EXEC}.c


clean:  
	rm -fr *.o

mrproper: clean
	rm -fr ${EXEC}

