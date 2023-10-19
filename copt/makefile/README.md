# Objectif du Makefile

Le makefile est un fichier qui permet de définir les règles
de compilation d'un projet. Ce petit tuto permet d'appréhender
son utilisation ainsi que la modularité d'un projet en C

# Contexte

On se donne un projet composé de deux fichiers f.c et g.c qui 
permettent la déclaration de deux fonctions de prototypes :

`int f(int x)`
`float g(float x)`

présents dans les fichiers headers f.h et g.h respectivement.
La fonction `main()`, déclarée dans `main.c`, appelle ces deux 
fonctions.

# Format simple du Makefile

```
all: main 

f.o: f.h f.c
	gcc -c f.c
g.o: g.h g.c
	gcc -c g.c

main: main.c f.o g.o
	gcc main.c f.o g.o -o main
```
