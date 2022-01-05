#include "OpGrilleV2.h"

void raiseErrorIfOutOfBounds(const struct Board * board, const int x, const int y){
    assert(x>=0||x<board->width);
    assert(y>=0||y<board->height);
}

void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt) {
    int size = width*height;
    self->data = calloc(size, sizeof(struct Case));
    self->player = malloc(sizeof(struct Player));
    for (int i = 0; i < size; i++)
    {
        self->data[i].type = ' ';
    }
    self->width = width;
    self->height = height;
    fprintf(stderr,"Tresure x:%d, y:%d\n", xt, yt);
    self->tresorX = xt;
    self->tresorY = yt;
    self->sortieX = x;
    self->sortieY = y;
    self->player->x = x;
    self->player->y = y;
    self->player->state = 0;
    self->player->lastInstruction = " ";

    self->moves = NULL;
    self->movesRemaining = 0;
    self->nbMoveInSameDirection = 0;
    self->step = 0;
}

void board_destroy(struct Board *self) {
    free(self->data);
    free(self->player);
}

void board_print(const struct Board * self) {
    for (int i = 0; i < self->width + 1; i++)
    {
        fprintf(stderr,"W");
    }
    int size = self->width*self->height;
    for (int i = 0; i < size; i++)
    {
        if (i % self->width == 0)
        {
            fprintf(stderr,"W\nW");
        }
        if (self->tresorX == i%self->width && self->tresorY == i/self->width){
            fprintf(stderr,"T");
        } else if (self->player->x == i%self->width && self->player->y == i/self->width){
            fprintf(stderr,"@");
        } else {
            fprintf(stderr,"%c", self->data[i].type);
        }
    }
    fprintf(stderr,"W\nW");
    for (int i = 0; i < self->width + 1; i++)
    {
        fprintf(stderr,"W");
    }
    fprintf(stderr,"\n");
}

void board_print2(const struct Board * self, const int * self2) {
    for (int i = 0; i < self->width + 1; i++)
    {
        fprintf(stderr,"W");
    }
    int size = self->width*self->height;
    for (int i = 0; i < size; i++)
    {
        if (i % self->width == 0)
        {
            fprintf(stderr,"W\nW");
        }
        fprintf(stderr,"%d",self2[i]);
        
    }
    fprintf(stderr,"W\nW");
    for (int i = 0; i < self->width + 1; i++)
    {
        fprintf(stderr,"W");
    }
    fprintf(stderr,"\n");
}

//ajouter les bords
void board_update(struct Board *self, char *status) {
    //fprintf(stderr,status);
    for (int i =0; i < 8; i++){
        //left
        if (self->player->x == 0 && (i == 0 || i == 3 || i == 5)){
            continue;
        }
        if (self->player->x == self->width-1 && (i == 2 || i == 4 || i == 7)){
            continue;
        }
        if (self->player->y == 0 && (i == 0 || i == 1 || i == 2)){
            continue;
        }
        if (self->player->y == self->height-1 && (i == 5 || i == 6 || i == 7)){
            continue;
        }
        if (i>3){
            self->data[(self->player->x+((i+1)%3-1))+((self->player->y+((i+1)/3)-1)*self->width)].type = status[i];
            //fprintf(stderr,"%c, pos: %d\n",status[i], ((self->player->x+((i+1)%3-1))+((self->player->y+((i+1)/3-1))*self->width)));
        }
        else{
            self->data[(self->player->x+(i%3-1))+((self->player->y+(i/3-1))*self->width)].type = status[i];
            //fprintf(stderr,"%c, pos: %d\n",status[i], (self->player->x+(i%3-1))+((self->player->y+(i/3-1))*self->width));
        }
        
    }
}
 
char get_type(const struct Board * self, const int x, const int y){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return 'W';
    }
    return self->data[x+(y*self->width)].type;
} 

// || x == h-1 && y == w-1 ||x == 0 && y == 0 || x == 0 && y == w-1 
//Verify if it don't cross wall
int calculate_heuristique(const int departX, const int departY, const int arriveX, const int arriveY){
    int dx = arriveX - departX;
    int dy = arriveY - departY;
    return abs(dx) + abs(dy);
}

int calculate_heuristique_with_wall(const struct Board *self, int departX, int departY){

}
//faire fonctrion pour recalculer le cout de toutes les cases seulement quand trésor trouvé


void movePlayer(struct Board * self){
    switch(self->player->lastInstruction[0]){
            case 'N':{
                self->player->y--;
            }break;
            case 'W':{
                self->player->x--;
            }break;
            case 'E':{
                self->player->x++;
            }break;
            case 'S':{
                self->player->y++;
            }break;
            default:{
            }
        }
}

char * directionWithInt(const int x){
    switch(x){
        case -2:{ //North
            return "NORTH";
        }break;
        case -1:{ //WEST
            return "WEST";
        }break;
        case 1:{ //East
            return "EAST";
        }break;
        case 2:{ //South
            return "SOUTH";
        }break;
        default:{//North
            return "NORTH";
        }
    }
}

void changerLastPositionWithInt(struct Board * self, const int x){
    self->player->lastInstruction = directionWithInt(x);
}

int lastPositionToInt(struct Board * self){
    switch(self->player->lastInstruction[0]){
            case 'N':{
                return -2;
            }break;
            case 'W':{
                return -1;
            }break;
            case 'E':{
                return 1;
            }break;
            case 'S':{
                return 2;
            }break;
            default:{
                return -2;
            }
        }
}

//bonus if it decrease the longest difference
//improve direction choice
int heuristique_for_direction(struct Board * self, int posX, int posY, int * alreadyView, int * smokeBoard){
    int minHeuristique = 1000000;
    for (int i =-1; i <= 1; i++){
        int jtt = i==0?-1:0;
        for (int j = jtt; j <= 1; j+=2){
            if (posX+i < 0 || posX+i >= self->width || posY+j < 0 || posY+j >= self->height){
                continue;
            }
            if (alreadyView[(posX+i)+(posY+j)*self->width]){
                continue;
            }
            char typeCase = get_type(self, posX+i, posY+j);
            if (typeCase == ' '){
                if (smokeBoard[(posX+i)+(posY+j)*self->width] == 0){
                    continue;
                }
                int k = calculate_heuristique(posX+i, posY+j, self->tresorX, self->tresorY);
                minHeuristique = min(minHeuristique, k);
            }
            else if (typeCase == 'T'){
                minHeuristique = 0;
            }
            else if (typeCase != 'W'){
                alreadyView[(posX+i)+(posY+j)*self->width] = 1;
                int k = heuristique_for_direction(self, posX+i, posY+j, alreadyView, smokeBoard)+1;
                alreadyView[(posX+i)+(posY+j)*self->width] = 0;
                minHeuristique = min(minHeuristique, k);
            }
        }
    }
    return minHeuristique;
}

void addFrontMoves(struct Board * self, const int dir){
    struct Moves * next = self->moves;
    struct Moves * node = malloc(sizeof(struct Moves));
    node->dir = dir;
    node->next = next;
    self->moves = node;
    self->movesRemaining++;
}

int popFrontMoves(struct Board * self){
    assert(self->moves != NULL);

    struct Moves * next = self->moves->next;
    int moveDir = self->moves->dir;
    free(self->moves);
    self->moves = next;
    self->movesRemaining--;
    return moveDir;
}

void deleteMoves(struct Board * self){
    struct Moves * next = NULL;
    struct Moves * node = self->moves;
    while (node != NULL){
        next = node->next;
        free(node);
        node = next;
    }
    self->movesRemaining = 0;
}

void createPath(struct Board * self, int * alreadyView){
    int actualPosX = self->sortieX;
    int actualPosY = self->sortieY;
    int nextPosX = actualPosX;
    int nextPosY = actualPosY;
    deleteMoves(self);
    while (actualPosX != self->tresorX || actualPosY != self->tresorY){
        int minCost = 100000;
        int minDir = 0;
        for (int i =-1; i <= 1; i++){
            int jtt = i==0?-1:0;
            for (int j = jtt; j <= 1; j+=2){
                if (actualPosX+i < 0 || actualPosX+i >= self->width || actualPosY+j < 0 || actualPosY+j >= self->height){
                    continue;
                }
                if (minCost > alreadyView[(actualPosX+i)+(actualPosY+j)*self->width] && alreadyView[(actualPosX+i)+(actualPosY+j)*self->width] != 0){
                    raiseErrorIfOutOfBounds(self, actualPosX+i, actualPosY+j);
                    minCost = alreadyView[(actualPosX+i)+(actualPosY+j)*self->width];
                    nextPosX = actualPosX+i;
                    nextPosY = actualPosY+j;
                    minDir = i+2*j;
                }
            }
        }
        addFrontMoves(self, -minDir);
        actualPosX = nextPosX;
        actualPosY = nextPosY;
        
    }
    assert(actualPosX == self->tresorX);
    assert(actualPosY == self->tresorY);
    fprintf(stderr, "Moves to return #%d \n", self->movesRemaining);

}
void coutCaseToGoBack(struct Board *self, int posX, int posY, const int cost, int * alreadyView){
    for (int i =-1; i <= 1; i++){
        int jtt = i==0?-1:0;
        for (int j = jtt; j <= 1; j+=2){
            char typeCase = get_type(self, posX+i, posY+j);
            if (typeCase != 'W' && typeCase != ' '){
                if (self->sortieX == posX+i && self->sortieY == posY+j){
                    createPath(self, alreadyView);
                    return;
                }
                if (posX+i < 0 || posX+i >= self->width || posY+j < 0 || posY+j >= self->height){
                    continue;
                }
                if (cost < alreadyView[(posX+i)+(posY+j)*self->width] || alreadyView[(posX+i)+(posY+j)*self->width] == 0){
                    raiseErrorIfOutOfBounds(self, posX+i, posY+j);
                    alreadyView[(posX+i)+(posY+j)*self->width] = cost;
                    coutCaseToGoBack(self, posX+i, posY+j, cost+1, alreadyView);
                }
            }
        }
    }
}
void calclateReturningPath(struct Board * self){
    int * alreadyView = calloc(self->width * self->height, sizeof(int));
    alreadyView[self->tresorX+self->tresorY*self->width] = 1 ;
    coutCaseToGoBack(self, self->tresorX, self->tresorY, 2, alreadyView);
    free(alreadyView);
}

void propagateSmoke(const struct Board * self, int * smokeBoard, const int x, const int y, const int isFirst){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return;
    }
    if (smokeBoard[x+y*self->width] == 1){
        return;
    }
    if (get_type(self, x, y) != ' ' && !isFirst){
        return ;
    }
    smokeBoard[x+y*self->width] = 1;
    propagateSmoke(self, smokeBoard, x+1, y, 0);
    propagateSmoke(self, smokeBoard, x-1, y, 0);
    propagateSmoke(self, smokeBoard, x, y+1, 0);
    propagateSmoke(self, smokeBoard, x, y-1, 0);

}

char * think(struct Board * self){
    int x = self->player->x;
    int y = self->player->y;
    self->step++;
    fprintf(stderr, "--- Step #%d \n", self->step);
    fprintf(stderr, "x : %d,   y:%d \n", x, y);
    raiseErrorIfOutOfBounds(self, x, y);
    if (self->movesRemaining != 0){
        fprintf(stderr, "Moves left: %d\n", self->movesRemaining);
        changerLastPositionWithInt(self, popFrontMoves(self));
        movePlayer(self);
        return self->player->lastInstruction;
    }
    fprintf(stderr, "No more moves, preparing new moves.\n");
    //board_print(self);
    //evaluer toutes les cases autours
    int minHeuristique = 1000000;
    int numberMinDir = 0;

    //create a board for memorise case which are already viewed
    int * alreadyView = calloc(self->width * self->height, sizeof(int));
    alreadyView[x+y*self->width] = 1;

    //TODO:Erreur lorsqu'on connait toutes lers cases autour d'une case vide elle est supprimé
    int * smokeBoard = calloc(self->width * self->height, sizeof(int));
    //verifier si autour du trésor c'est libre sinon allez a la case vide la plus proche ou avant un cout positif
    propagateSmoke(self, smokeBoard, self->tresorX, self->tresorY, 1);

    //board_print2(self, smokeBoard);

    int dx = self->tresorX - x;
    int dy = self->tresorY - y;

    for (int i =-1; i <= 1; i++){
        int jtt = i==0?-1:0;
        for (int j = jtt; j <= 1; j+=2){
            if (get_type(self, x+i, y+j) == 'T'){
                minHeuristique = 0;
                numberMinDir = i+2*j;
                fprintf(stderr, "Calculate Returning Path\n");
                calclateReturningPath(self);
            }
            else if (get_type(self, x+i, y+j) != 'W'){
                int heuristiqueForDirection = heuristique_for_direction(self, x+i, y+j, alreadyView, smokeBoard);
                //Ajoute une priorité sur la ligne droite a partir du deuxième déplacement
                if (lastPositionToInt(self) != i+2*j || self->nbMoveInSameDirection%2 == 0){
                    heuristiqueForDirection = heuristiqueForDirection+2;
                }
                if (abs(dx) < abs(dy)){
                    if(abs(dy-j) >= abs(dy)){
                        heuristiqueForDirection = heuristiqueForDirection+1;
                    }

                }
                if (abs(dx) > abs(dy)){
                    if(abs(dx-i) >= abs(dx)){
                        heuristiqueForDirection = heuristiqueForDirection+1;
                    }
                }
                fprintf(stderr, "Heuristique dir: %s, i:%d, j:%d, h:%d\n", directionWithInt(i+2*j), x+i, y+j, heuristiqueForDirection);
                if (minHeuristique > heuristiqueForDirection){
                    minHeuristique = heuristiqueForDirection;
                    numberMinDir = i+2*j;
                }
            }
        }
    }
    free(alreadyView);
    if (lastPositionToInt(self) == numberMinDir){
        self->nbMoveInSameDirection++;
    }
    else {
        self->nbMoveInSameDirection = 0;
    }
    fprintf(stderr, "Nb move in the same line: %d\n", self->nbMoveInSameDirection);
    fprintf(stderr, "Last direction: %s\n", self->player->lastInstruction);
    changerLastPositionWithInt(self, numberMinDir);
    fprintf(stderr, "Chosen direction: %s\n", self->player->lastInstruction);
    movePlayer(self);
    return self->player->lastInstruction;

}