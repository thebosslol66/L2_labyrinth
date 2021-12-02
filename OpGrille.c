#include "OpGrille.h"

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt) {
    int size = width*height;
    self->data = calloc(size, sizeof(struct Case));
    for (int i = 0; i < size; i++)
    {
        self->data[i]->type = ' ';
        self->data[i]->heuristique = calculate_heuristique(x, y, xt, yt);
    }
    self->width = width;
    self->height = height;
    self->tresorX = xt;
    self->tresorY = yt;
    self->sortieX = x;
    self->sortieY = y;
}




char * array_to_line(int w, int h) {
    int size = w * h;
    char *line = calloc(size, sizeof(size_t));
    // for (size_t i = 0; i < size; i++)
    // {
    //     line[i] = i;
    // }
    return line;
}

 void board_update(struct Board *self, int x, int y, char *status) {
    for (int r = 0; r < self->width; r++)
    {
        for (int c = 0; r < self->height; c++)
        {
            int imin = c > 0? c-1 : c;
            int imax = c < self->height-1? c+1 : c;
            int jmin = r > 0? r-1 : r;
            int jmax = r < self->height-1? r+1 : r;
        }
    }
    
    
    
    
    // int i = 0;
    // if (x == h-1 || x == 0 || y == w-1 || y == 0){
    //     if (x == h-1)
    //     {
    //         if (y == 0)
    //         {
    //             line[(x-1)+(y)*w] = status[1];
    //             line[(x-1)+(y)*w] = status[2];
    //             line[(x-1)+(y)*w] = status[4];
    //         }else if (y == w-1)
    //         {
    //             line[(x-1)+(y-1)*w] = status[0];
    //             line[(x-1)+(y)*w] = status[1]
    //             line[(x)+(y-1)*w] = status[3];
    //         }else
    //         {
    //             line[(x-1)+(y-1)*w] = status[0];.
    //             line[(x-1)+(y)*w] = status[1];
    //             line[(x-1)+(y+1)*w] = status[2];
    //             line[(x)+(y-1)*w] = status[3];
    //             line[(x)+(y+1)*w] = status[4];
    //         }
    //     }else
    //     if (x == 0)
    //     {
    //         if (y == 0)
    //         {
    //             line[(x)+(y+1)*w] = status[4];
    //             line[(x+1)+(y+1)*w] = status[6];
    //             line[(x+1)+(y)*w] = status[7];
    //         }else if (y == w-1)
    //         {
    //             line[(x)+(y-1)*w] = status[3];
    //             line[(x+1)+(y-1)*w] = status[5];
    //             line[(x+1)+(y)*w] = status[6];
    //         }else
    //         {
    //             line[(x)+(y-1)*w] = status[3];
    //             line[(x)+(y+1)*w] = status[4];
    //             line[(x-1)+(y-1)*w] = status[5];
    //             line[(x-1)+(y)*w] = status[6];
    //             line[(x-1)+(y+1)*w] = status[7];
    //         }
    //     }else
    //     if (y == w-1)
    //     {
    //         line[(x-1)+(y-1)*w] = status[0];
    //         line[(x-1)+(y)*w] = status[1];
    //         line[(x)+(y-1)*w] = status[3];
    //         line[(x+1)+(y-1)*w] = status[5];
    //         line[(x+1)+(y)*w] = status[6];
    //     }else
    //     {
    //         line[(x-1)+(y)*w] = status[1];
    //         line[(x-1)+(y+1)*w] = status[2];
    //         line[(x)+(y+1)*w] = status[4];
    //         line[(x+1)+(y)*w] = status[6];
    //         line[(x+1)+(y+1)*w] = status[7];
    //     }

    // }else{
    //     for (i = 0; i < 8; i++)
    //     {
    //         switch(i){
    //             case 0 :
    //                 line[(x-1)+(y-1)*w] = status[i];
    //             break;

    //             case 1 :
    //                 line[(x-1)+(y)*w] = status[i];
    //             break;

    //             case 2 :
    //                 line[(x-1)+(y+1)*w] = status[i];
    //             break;

    //             case 3 :
    //                 line[(x)+(y-1)*w] = status[i];
    //             break;

    //             case 4 :
    //                 line[(x)+(y+1)*w] = status[i];
    //             break;

    //             case 5 :
    //                 line[(x+1)+(y-1)*w] = status[i];
    //             break;

    //             case 6 :
    //                 line[(x+1)+(y)*w] = status[i];
    //             break;

    //             case 7 :
    //                 line[(x+1)+(y+1)*w] = status[i];
    //             break;
    //         }
    //     }
    // }
}

// || x == h-1 && y == w-1 ||x == 0 && y == 0 || x == 0 && y == w-1 
int calculate_heuristique(const int departX, const int departY, const int arriveX, const int arriveY){
    return abs(arriveX - departX) + abs(arriveY - departY);
}