#include "OpGrille.h"

void raiseErrorIfOutOfBounds(const struct Board * board, const int x, const int y){
    assert(x<0||x>=board->width);
    assert(y<0||y>=board->height);
}

void board_unexplorecase_pop_front(struct Board * self, const int x, const int y){
    struct UnexploreCase * new = malloc(sizeof(struct UnexploreCase));
    new->next = self->unexplorePath;
    new->x = x;
    new->y = y;
    self->unexplorePath = new;
}

struct UnexploreCase * board_unexplorecase_remove_front(struct Board * self){
    struct UnexploreCase * other = self-> unexplorePath;
    if (other == NULL){
        return other;
    }
    self-> unexplorePath = self-> unexplorePath-> next;
    other-> next = NULL;
    return other;
}

void board_unexplorecase_destroy(struct Board * self){
    struct UnexploreCase * res = board_unexplorecase_remove_front(self);
    while(res != NULL){
        free(res);
        res = board_unexplorecase_remove_front(self);
    }
}

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt) {
    int size = width*height;
    self->data = calloc(size, sizeof(struct Case));
    for (int i = 0; i < size; i++)
    {
        self->data[i].type = ' ';
        self->data[i].heuristique = calculate_heuristique(x, y, xt, yt);
    }
    self->width = width;
    self->height = height;
    self->tresorX = xt;
    self->tresorY = yt;
    self->sortieX = x;
    self->sortieY = y;
}

// char * array_to_line(int w, int h) {
//     int size = w * h;
//     char *line = calloc(size, sizeof(size_t));
//     // for (size_t i = 0; i < size; i++)
//     // {
//     //     line[i] = i;
//     // }
//     return line;
// }

 void board_update(struct Board *self, int x, int y, char *status) {
    for (int r = 0; r < self->width; r++)
    {
        for (int c = 0; r < self->height; c++)
        {
            int imin = c > 0? c-1 : c;
            int imax = c < self->height-1? c+1 : c;
            int jmin = r > 0? r-1 : r;
            int jmax = r < self->height-1? r+1 : r;
            for (int i = imin; i <= imax; i++)
            {
                for (int j = jmin; j <= jmax; j++)
                {
                    if (i == c && j == r)
                    {
                        continue;
                    }
                    self->data[i+(j*self->width)].type = status[i+j];
                }
                
            }
        }
    }
}
    
int get_heuristique(const struct Board * self, const int x, const int y){

    return self->data[x+(y*self->width)].heuristique;
}

int get_cout(const struct Board * self, const int x, const int y){
    return self->data[x+(y*self->width)].cout;
} 

char get_type(const struct Board * self, const int x, const int y){
    return self->data[x+(y*self->width)].type;
} 

// || x == h-1 && y == w-1 ||x == 0 && y == 0 || x == 0 && y == w-1 
int calculate_heuristique(const int departX, const int departY, const int arriveX, const int arriveY){
    return abs(arriveX - departX) + abs(arriveY - departY);
}

int calculate_cout(const struct Board * board, const int x, const int y){
    raiseErrorIfOutOfBounds(board, x, y);
    int minX = max(x-1, 0);
    int maxX = min(x+1, board->width-1);
    int minY = max(y-1, 0);
    int maxY = min(x+1, board->height-1);

    int coutActual = board->width*board->height;

    for (int xt = minX; xt <= maxX; xt++){
        for (int yt = minY; yt <= maxY; yt++){
            if (x == xt && y == yt){
                continue;
            }
            coutActual = min(coutActual,get_heuristique(board, xt, yt)+1);
        }
    }

    return coutActual;
}

char * think(char * position){
    fprintf(stderr, "Read %s.", position);
    int * possible_direction;
    possible_direction = calloc(4, sizeof(char));
    possible_direction[0] = (position[1] == '_')?1:0;
    possible_direction[1] = (position[3] == '_')?1:0;
    possible_direction[2] = (position[4] == '_')?1:0;
    possible_direction[3] = (position[6] == '_')?1:0;
    int numberPosition = 0;
    for (int i = 0; i < 4; i++){
        numberPosition = numberPosition + possible_direction[i];
    }
    assert(numberPosition > 0);
    int returnValue = rand()%numberPosition;
    for (int i = 0; i < 4; i++){
        if (possible_direction[i] && returnValue <=0){
            switch(i){
                case 0:{
                    return "NORTH";
                }
                case 1:{
                    return "WEST";
                }
                case 2:{
                    return "EAST";
                }
                case 3:{
                    return "SOUTH";
                }
            }
        }
        if (possible_direction[i]){
            returnValue--;
        }
    }
    return "NORTH";
}

char * think(struct Board * self, int x, int y){
    raiseErrorIfOutOfBounds(board, x, y);
    board_print(struct Board * self);
    int * possible_direction;
    possible_direction = calloc(4, sizeof(char));
    possible_direction[0] = (get_type(self, x, y-1) != 'W')?1:0; //Haut
    possible_direction[1] = (get_type(self, x-1, y) != 'W')?1:0; //Gauche
    possible_direction[2] = (get_type(self, x+1, y) != 'W')?1:0; //Droite
    possible_direction[3] = (get_type(self, x, y+1) != 'W')?1:0; //Bas

    


}
//si on touche un bord horizontal ou vertical tout ce qui est du coté oposé est inutile a vérifier