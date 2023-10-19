all: main

%.o: %.c %.h
	gcc -o $@ -c $<

main: main.c f.o g.o
	gcc main.c f.o g.o -o main
