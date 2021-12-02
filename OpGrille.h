#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <time.h>

struct Case {
    int cout;
    int heuristique;
    char type; // ' ' = vide 
};

struct Board {
    Case * data;
    int width;
    int height;
};
