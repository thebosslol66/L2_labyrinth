#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define abs(a)(a<0?-a:a)
#define max(a,b)(a>=b?a:b)
#define min(a,b)(a<=b?a:b)


/**
 * @struct Case
 * @param type contain the type of the case of the board
 * @param hasVisited say if the case has been visit (0 for no and 1 fir yess)
 */
struct Case {
    char type; // ' ' = vide
    int hasVisited;
};

/**
 * @struct Case
 * @param type contain the type of the case of the board
 * @param hasVisited say if the case has been visit (0 for no and 1 fir yess)
 */
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
    int nbMoveInSameDirection;

    // 4 points calcul heuristique
    int xmin;
    int xmax;
    int ymin;
    int ymax;

    int step;

    int width;
    int height;

    int tresorX;
    int tresorY;

    int sortieX;
    int sortieY;
};

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt);
void board_update(struct Board *self, char *status);
void board_destroy(struct Board *self);
char * think(struct Board * self);