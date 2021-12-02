#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define abs(a)(a<0?-a:a)

struct Case {
    int cout;
    int heuristique;
    char type; // ' ' = vide 
};

struct Board {
    struct Case * data; //Contain al information about plate
    int width;
    int height;

    int tresorX;
    int tresorY;

    int sortieX;
    int sortieY;
};
