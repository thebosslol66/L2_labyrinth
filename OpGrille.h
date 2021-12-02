#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

#define int abs(int a){if(a<0){return -a;}return a;};

struct Case {
    int cout;
    int heuristique;
    char type; // ' ' = vide 
};

struct Board {
    Case * data; //Contain al information about plate
    int width;
    int height;

    int tresorX;
    int tresorY;

    int sortieX;
    int sortieY;
};
