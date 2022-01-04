#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define abs(a)(a<0?-a:a)
#define max(a,b)(a>=b?a:b)
#define min(a,b)(a<=b?a:b)

struct Case {
    char type; // ' ' = vide
};

struct Position {
    int x;
    int y;
};

struct PossiblePosition{
    struct Position ** pos;
    int n;
};

struct Player {
    int x;
    int y;
    int state; //état du personnage dans sa quete
    char * lastInstruction;
};

struct Moves {
    int dir;
    struct Moves * next;
};

struct Board {
    struct Case * data; //Contain al information about plate
    struct Player * player;
    struct Moves * moves;
    int movesRemaining;

    int step;

    int width;
    int height;

    int tresorX;
    int tresorY;

    int sortieX;
    int sortieY;
};

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt);
char * think(struct Board * self);