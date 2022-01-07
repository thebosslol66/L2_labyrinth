#include "OpGrilleV2.h"

/**
 * @brief raiseErrorIfOutOfBounds permit to keep 'x' and 'y' on the context.
 * @param board Board game
 * @param x an abscissa
 * @param y an ordinate
 */
void raiseErrorIfOutOfBounds(const struct Board * board, const int x, const int y){
    assert(x>=0&&x<board->width);
    assert(y>=0&&y<board->height);
}

/**
 * @brief board_create create the board.
 * @param self Board game
 * @param width Width of the board
 * @param height Height of the board
 * @param x player's abscissa
 * @param y player's ordonate
 * @param xt treasure's abscissa
 * @param yt treasure's ordonate
 * 
 * From the parameters, it assigned the diferents board's value structure.
 */
void board_create(struct Board *self, int width, int height, int x, int y, int xt, int yt) {
    int size = width*height;
    self->data = calloc(size, sizeof(struct Case));
    self->player = malloc(sizeof(struct Player));
    for (int i = 0; i < size; i++)
    {
        self->data[i].type = ' ';
        self->data[i].hasVisited = 0;
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

    self->xmax = x + 2;
    self->xmin = x - 2;
    self->ymax = y + 2;
    self->ymin = y - 2;

    self->moves = NULL;
    self->movesRemaining = 0;
    self->nbMoveInSameDirection = 0;
    self->step = 0;
}

/**
 * @brief board_destroy Destroy the board.
 * @param self The board to desroy
 */
void board_destroy(struct Board *self) {
    free(self->data);
    free(self->player);
}

/**
 * @brief board_print Print the board knew for the player.
 * @param slef The board
 * 
 * Represent the board only with ' ', 'W', '@', '_' and 'T'.
 */
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

/**
 * @brief board_print2 Print the values of the second board 
 * @param slef The board
 */
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
            fprintf(stderr,"W\nW\t");
        }
        fprintf(stderr,"%d\t",self2[i]);
        
    }
    fprintf(stderr,"W\nW");
    for (int i = 0; i < self->width + 1; i++)
    {
        fprintf(stderr,"W");
    }
    fprintf(stderr,"\n");
}

/**
 * @brief board_update update the board
 * @param self the board
 * @param status string of the status case arount the player
 */
void board_update(struct Board *self, char *status) {
    //fprintf(stderr,status);

    self->data[self->player->x+(self->player->y*self->width)].hasVisited = 1;

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

/**
 * @brief get_type Get the char in a position.
 * @param self the board
 * @param x the abscissa's position
 * @param y the ordonate's position
 * @return the char of the position
 * 
 * if the position is oput of the board, it return a wall
 */
char get_type(const struct Board * self, const int x, const int y){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return 'W';
    }
    return self->data[x+(y*self->width)].type;
} 

/**
 * @brief calculate_heuristique Calculate the coste of a way to the tresure
 * @param departX the abscissa's initial position
 * @param departY the ordonate's initial position
 * @param arriveX the abscissa's goal position
 * @param arriveY the ordonate's goal position
 * @return the heuristique into two positions
 */
int calculate_heuristique(const int departX, const int departY, const int arriveX, const int arriveY){
    int dx = arriveX - departX;
    int dy = arriveY - departY;
    return abs(dx) + abs(dy);
}

/**
 * @brief movePlayer Do a move from a direction
 * @param self the board
 */
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

/**
 * @brief directionWithInt associate an int to a directon
 * @param x An int
 * @return string whitch represent a direction
 */
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

/**
 * @brief changerLastPositionWithInt assigned the last position of the player
 * @param self the board
 * @param x the int last direction
 */
void changerLastPositionWithInt(struct Board * self, const int x){
    self->player->lastInstruction = directionWithInt(x);
}

/**
 * @brief lastPositionToInt pass a char direction to an int direction
 * @param self the board
 * @return int direction
 */
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

/**
 * @brief Get the best cost from the position to the tresure.
 *
 * Calculate all move cost to all cases from the position.
 * If we don't have information about the case we use his value in the smokeBoad.
 * We return the best value to get the tresure.
 *
 * @param self the board of the game
 * @param posX the position in x for calculate the minimum possible move to reach treasure
 * @param posY the position in y for calculate the minimum possible move to reach treasure
 * @param alreadyView store cases already visited to avoid infinite loop
 * @param smokeBoard list with all unvisited cases with his move cost to tresure
**/
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
                int k = smokeBoard[(posX+i)+(posY+j)*self->width]-1;
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

/**
 * @brief Add a move to the structure board
 *
 * @param self the board of the game
 * @param dir the diretion to add to the structure
**/
void addFrontMoves(struct Board * self, const int dir){
    struct Moves * next = self->moves;
    struct Moves * node = malloc(sizeof(struct Moves));
    node->dir = dir;
    node->next = next;
    self->moves = node;
    self->movesRemaining++;
}

/**
 * @brief Get the first move from board and delete it in the list.
 *
 * @param self the board of the game
**/
int popFrontMoves(struct Board * self){
    assert(self->moves != NULL);

    struct Moves * next = self->moves->next;
    int moveDir = self->moves->dir;
    free(self->moves);
    self->moves = next;
    self->movesRemaining--;
    return moveDir;
}

/**
 * @brief Delete all moves present in the board
 *
 * @param self the board of the game
**/
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

/**
 * @brief Create a path between treasure and exit.
 *
 * It use a board already fullfilled and follow cases wich cost the less mouves to return to exit.
 * It record each moves in attribute moves in board for use it later.
 *
 * @param self the board of the game
 * @param alreadyView the board filled with mouve cost
**/
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

/**
 * @brief Add cost for all cases we discover and put it into a list.
 *
 * For every cases add the cost to go to posX and posY with the less moves.
 *
 * @param self the board of the game
 * @param posX the position in x of the case to add a move cost
 * @param posY the position in y of the case to add a move cost
 * @param cost the cost to add to the case
 * @param alreadyView the board to fill with mouve cost
**/
void coutCaseToGoBack(struct Board *self, int posX, int posY, int cost, int * alreadyView){
    for (int i =-1; i <= 1; i++){
        int jtt = i==0?-1:0;
        for (int j = jtt; j <= 1; j+=2){
            char typeCase = get_type(self, posX+i, posY+j);
            if (typeCase != 'W' && typeCase != ' '){
                if (self->sortieX == posX+i && self->sortieY == posY+j){
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

/**
 * @brief Calculate the best path from treasure to the exit.
 *
 * Create a new empty board and fill with move cost every path we disdcover from the exit position.
 * Then create a path and put it in the board folling the lowest move cost from the treasure.
 *
 * @param self the board of the game
**/
void calclateReturningPath(struct Board * self){
    int * alreadyView = calloc(self->width * self->height, sizeof(int));
    alreadyView[self->tresorX+self->tresorY*self->width] = 1 ;
    coutCaseToGoBack(self, self->tresorX, self->tresorY, 2, alreadyView);
    createPath(self, alreadyView);
    free(alreadyView);
}


/**
 * @brief Put in smokeBoard the move cost from each case we don't visit.
 *
 * We begin in the fist position and put a 1 move, then for cases around put a cost of 1 more move.
 * We do this for all cases not visited and wich are not wall.
 *
 * @param self the board of the game
 * @param smokeBoard the board to fullfill with the cost to the position
 * @param x the position in x of the case to add a move cost
 * @param y the position in y of the case to add a move cost
 * @param cost the cost of the case
 *
**/
void propagateSmoke(const struct Board * self, int * smokeBoard, const int x, const int y, const int cost, const int isFirst){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return;
    }
    if (smokeBoard[x+y*self->width] <= cost && smokeBoard[x+y*self->width] != 0){
        return;
    }
    if (get_type(self, x, y) == 'W'){
        return ;
    }
    if (self->data[x+(y*self->width)].hasVisited == 1){
        return ;
    }
    smokeBoard[x+y*self->width] = cost;
    propagateSmoke(self, smokeBoard, x+1, y, cost+1, 0);
    propagateSmoke(self, smokeBoard, x-1, y, cost+1, 0);
    propagateSmoke(self, smokeBoard, x, y+1, cost+1, 0);
    propagateSmoke(self, smokeBoard, x, y-1, cost+1, 0);
}

/** 
 * @brief Calculate the best move to do with actual board
 *
 * If we search tresure we calculate the minimum cost of each case arround and then we go to the minimum.
 * We made aa smal priority for case wich reduce the longest distance between dx and dy.
 * If we find the tresure we calculate the path to return to initial position.
 * If we have a path to follow, we use it in priority. It is use to return to entry.
 * @param self actual board for the game
 * @return string for thr best next direction
**/
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
    propagateSmoke(self, smokeBoard, self->tresorX, self->tresorY, 1, 1);

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
                if (lastPositionToInt(self) != i+2*j){
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
    free(smokeBoard);
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