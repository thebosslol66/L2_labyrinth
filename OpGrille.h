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

struct File {
    
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
