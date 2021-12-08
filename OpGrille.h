#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define abs(a)(a<0?-a:a)
#define max(a,b)(a>=b?a:b)
#define min(a,b)(a<=b?a:b)

struct Case {
    int cout;
    int heuristique;
    char type; // ' ' = vide 
};

struct UnexploreCase {
    int x;
    int y;
    struct UnexploreCase * next;
};

struct Position {
    int x;
    int y;
};

struct Board {
    struct Case * data; //Contain al information about plate
    struct UnexploreCase * unexplorePath;
    struct Position * player;
    int width;
    int height;

    int tresorX;
    int tresorY;

    int sortieX;
    int sortieY;
};

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt);