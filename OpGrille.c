#include "OpGrille.h"

void raiseErrorIfOutOfBounds(const struct Board * board, const int x, const int y){
    assert(x>=0||x<board->width);
    assert(y>=0||y<board->height);
}

void board_unexplorecase_push_front(struct Board * self, const int x, const int y){
    struct UnexploreCase * new = malloc(sizeof(struct UnexploreCase));
    new->next = self->unexplorePath;
    new->x = x;
    new->y = y;
    self->unexplorePath = new;
}


struct UnexploreCase * board_unexplorecase_remove_front(struct Board * self){
    struct UnexploreCase * other = self->unexplorePath;
    if (other == NULL){
        return other;
    }
    self->unexplorePath = self->unexplorePath-> next;
    other->next = NULL;
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
    self->player = malloc(sizeof(struct Position));
    for (int i = 0; i < size; i++)
    {
        self->data[i].type = ' ';
        self->data[i].heuristique = calculate_heuristique(i%width, i/width, xt, yt);
        self->data[i].cout = -1;
    }
    self->unexplorePath = NULL;
    self->width = width;
    self->height = height;
    self->tresorX = xt;
    self->tresorY = yt;
    self->sortieX = x;
    self->sortieY = y;
    self->player->x = x;
    self->player->y = y;
    self->player->state = 0;
    self->player->lastInstruction = " ";
    self->data[self->player->x+(self->player->y*self->width)].cout = 0;
}

void board_destroy(struct Board *self) {
    board_unexplorecase_destroy(self);
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
        if (self->player->x == i%self->width && self->player->y == i/self->width){
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
    
void set_heuristique(struct Board * self, const int x, const int y, const int heuristique){
    self->data[x+(y*self->width)].heuristique = heuristique;
}

int get_heuristique(const struct Board * self, const int x, const int y){
    //simplement pour le test
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return -1;
    }
    return self->data[x+(y*self->width)].heuristique;
}

void set_cout(struct Board * self, const int x, const int y, const int cout){
    self->data[x+(y*self->width)].cout = cout;
}
int get_cout(const struct Board * self, const int x, const int y){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return -1;
    }
    return self->data[x+(y*self->width)].cout;
} 

char get_type(const struct Board * self, const int x, const int y){
    if (x >= self->width || x < 0 || y >= self->height || y < 0)
    {
        return 'W';
    }
    return self->data[x+(y*self->width)].type;
} 

// || x == h-1 && y == w-1 ||x == 0 && y == 0 || x == 0 && y == w-1 
int calculate_heuristique(const int departX, const int departY, const int arriveX, const int arriveY){
    int dx = arriveX - departX;
    int dy = arriveY - departY;
    return abs(dx) + abs(dy);
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
            if ((x == xt && y == yt) || get_cout(board, xt, yt) <0|| xt != x && yt != y){
                continue;
            }
            int coutPotential = get_cout(board, xt, yt)+1;
            coutActual = min(coutActual,coutPotential);
        }
    }

    return coutActual;
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

void changerLastPositionWithInt(struct Board * self, const int x){
    switch(x){
        case -2:{ //North
            self->player->lastInstruction =  "NORTH";
        }break;
        case -1:{ //WEST
            self->player->lastInstruction =  "WEST";
        }break;
        case 1:{ //East
            self->player->lastInstruction =  "EAST";
        }break;
        case 2:{ //South
            self->player->lastInstruction =  "SOUTH";
        }break;
        default:{//North
            self->player->lastInstruction =  "NORTH";
        }
    }
}

struct Position * projectionOnWall(const struct Board * self, const int xDepart, const int yDepart){
    return NULL;
}

bool canAccessToTreasure(const struct Board * self, const int xDepart, const int yDepart, const int xProjection, const int yProjection){
    return true;
}

bool havePathToTreasure(const struct Board * self, const int xDepart, const int yDepart){
    return true;
}

char * think(struct Board * self){
    int x = self->player->x;
    int y = self->player->y;
    fprintf(stderr, "width : %d,   height:%d \n", self->width, self->height);
    fprintf(stderr, "x : %d,   y:%d \n", x, y);
    raiseErrorIfOutOfBounds(self, x, y);
    board_print(self);
    int k = 0;
    if (self->player->state == 0){
        struct Position ** possible_direction;
        possible_direction = calloc(4, sizeof(struct Position));
        //TODO: Aller tout droit jusqu'a ce qu'on ne puisse plus(pas sur)
        //TODO: Ajout de l'algo A*
        //TODO: Supprimer de la pile les chemins qui sont dans une boucle

        //verifier si on es plus proche en x ou y
        int dx = x - self->tresorX;
        int dy = y - self->tresorX;

        for (int i =-1; i <= 1; i++){
            int jtt = i==0?-1:0;
            for (int j = jtt; j <= 1; j+=2){
                fprintf(stderr, "i: %d, j: %d, type: %c, cout: %d, heuristique: %d\n", i, j, get_type(self, x+i, y+j), get_cout(self, x+i, y+j), get_heuristique(self, x+i, y+j));
                if (get_type(self, x+i, y+j) != 'W' && get_cout(self, x+i, y+j) == -1){
                    //TODO: modifier la condition d'acces au tresor
                    if (get_type(self, x+i, y+j) == 'T'){
                        fprintf(stderr, "Treasure found!\n");
                        self->player->state = 3;
                    }
                    
                    int nbWall = 0;
                    for (int i2 =-1; i2 <= 1; i2++){
                        int jtt2 = i2==0?-1:0;
                        for (int j2 = jtt2; j2 <= 1; j2+=2){
                            if (get_type(self, x+i+i2, y+j+j2) == 'W'){
                                nbWall++;
                            }
                        }
                    }
                    set_cout(self, x+i, y+j,get_cout(self,x, y)+1);
                    if(nbWall == 3 && get_type(self, x+i, y+j) != 'T'){
                        continue;
                    }

                    int l = 0;
                    //tri insertion
                    while (l < k && get_heuristique(self, possible_direction[l]->x, possible_direction[l]->y) <= get_heuristique(self, x+i, y+j)){
                        if((get_heuristique(self, possible_direction[l]->x, possible_direction[l]->y)*(100-POURCENTAGE_HUERISTIQUE_DIFFERENTE_PREFERE_CHEMIN_PLUS_PROCHE_X_ET_Y))/100 >= get_heuristique(self, x+i, y+j) &&
                        ((get_heuristique(self, possible_direction[l]->x, possible_direction[l]->y)*(100+POURCENTAGE_HUERISTIQUE_DIFFERENTE_PREFERE_CHEMIN_PLUS_PROCHE_X_ET_Y))/100) <= get_heuristique(self, x+i, y+j)){
                            if (abs(dx) > abs(dy)){
                                int dxt1 = x+i - self->tresorX;
                                int dxt2 = possible_direction[l]->x - self->tresorX;
                                if (abs(dxt1) <= abs(dxt2)){
                                    //dtx1 est le plus proche donc on le place avant
                                    break;
                                }
                            }
                            int dyt1 = y+j - self->tresorY;
                            int dyt2 = possible_direction[l]->y - self->tresorY;
                            if (abs(dyt1) <= abs(dyt2)){
                                //dty1 est le plus proche donc on le place avant
                                break;
                            }
                        }
                        l++;
                    }
                    fprintf(stderr, "\nl:%d\n", l);
                    for (int m = k-1; m>=l; m--){
                        fprintf(stderr, "\nl:%d, m:%d\n", l, m);
                        possible_direction[m+1] = possible_direction[m];
                    }
                    possible_direction[l] = malloc(sizeof(struct Position));
                    possible_direction[l]->x = x+i;
                    possible_direction[l]->y = y+j;
                    k++;
                }
            }
        }

        //Unexplore path without first because we go into
        for (int i = k-1; i> 0; i--){
            fprintf(stderr, "Yeh\n");
            board_unexplorecase_push_front(self, possible_direction[i]->x, possible_direction[i]->y);
            set_heuristique(self, possible_direction[i]->x, possible_direction[i]->y, get_cout(self,possible_direction[i]->x, possible_direction[i]->y)+calculate_heuristique(possible_direction[i]->x, possible_direction[i]->y, self->tresorX, self->tresorY));
            free(possible_direction[i]);
        }
        if (self->unexplorePath != NULL && self->unexplorePath->next != NULL){
            assert(self->unexplorePath->x != self->unexplorePath->next->x || self->unexplorePath->y != self->unexplorePath->next->y);
        }
        
        if (possible_direction[0] != NULL){
            int xt = possible_direction[0]->x -x;
            int yt = possible_direction[0]->y -y;
            set_heuristique(self, possible_direction[0]->x, possible_direction[0]->y, get_cout(self,possible_direction[0]->x, possible_direction[0]->y)+calculate_heuristique(possible_direction[0]->x, possible_direction[0]->y, self->tresorX, self->tresorY));
            free(possible_direction[0]);
            changerLastPositionWithInt(self, xt+2*yt);
            free(possible_direction);
            movePlayer(self);
            fprintf(stderr, "Direction %s\n", self->player->lastInstruction);
            return self->player->lastInstruction;

        }
        //C'est un cul de sac tout a déja été exploré
        self->player->state = 1;
        free(possible_direction);
    }
    if (self->player->state == 1){
        fprintf(stderr, "\n3\n");
        //TODO: Verifier si la case sur laquelle on est est dans la file (si c'est le cas le supprimer car chemin déja visité)
        switch(self->player->lastInstruction[0]){
            case 'N':{
                self->player->lastInstruction =  "SOUTH";
            }break;
            case 'W':{
                self->player->lastInstruction =  "EAST";
            }break;
            case 'E':{
                self->player->lastInstruction =  "WEST";
            }break;
            case 'S':{
                self->player->lastInstruction =  "NORTH";
            }break;
            default:{
                self->player->lastInstruction =  "NORTH";
            }
        }
        movePlayer(self);
        self->player->state = 2;//Demi-tour éffectué maintenant on rebrousse chemin
        return self->player->lastInstruction;
    }
    if (self->player->state == 2){
        int coutMin = self->width*self->height;
        int xmin, ymin;
        for (int i =-1; i <= 1; i++){
            int jtt = i==0?-1:0;
            for (int j = jtt; j <= 1; j+=2){
                fprintf(stderr, "state 2 i: %d, j: %d, type: %c, cout: %d\n", i, j, get_type(self, x+i, y+j), get_cout(self, x+i, y+j));
                if (get_type(self, x+i, y+j) != 'W'){
                    if (x+i == self->unexplorePath->x && y+j == self->unexplorePath->y){
                        fprintf(stderr, "i: %d, j: %d\n", i, j);
                        changerLastPositionWithInt(self, i+2*j);
                        movePlayer(self);
                        
                        free(board_unexplorecase_remove_front(self));
                        self->player->state = 0;
                        //Retour a la situation initiale et découvrons un nouveau chemin
                        return self->player->lastInstruction;
                    }
                    int actualCout = get_cout(self, x+i, y+j);
                    if (coutMin > actualCout){
                        coutMin = actualCout;
                        xmin = x+i;
                        ymin = y+j;
                    }
                }
            }
        }
        int xt = xmin -x;
        int yt = ymin -y;
        fprintf(stderr, "xt+2*yt: %d\n", xt+2*yt);
        changerLastPositionWithInt(self, xt+2*yt);
        movePlayer(self);
        return self->player->lastInstruction;
        
    }
    
    if (self->player->state == 3){
        int coutMin = self->width*self->height;
        int xmin = 0;
        int ymin = 0;
        for (int i =-1; i <= 1; i++){
            int jtt = i==0?-1:0;
            for (int j = jtt; j <= 1; j+=2){
                fprintf(stderr, "state 3 i: %d, j: %d, type: %c, cout: %d\n", i, j, get_type(self, x+i, y+j), get_cout(self, x+i, y+j));
                if (get_type(self, x+i, y+j) != 'W'){
                    int actualCout = get_cout(self, x+i, y+j);
                    
                    if (coutMin > actualCout && actualCout >=0){
                        coutMin = actualCout;
                        xmin = x+i;
                        ymin = y+j;
                    }
                }
            }
        }
        int xt = xmin -x;
        int yt = ymin -y;
        fprintf(stderr, "xt+2*yt: %d\n", xt+2*yt);
        changerLastPositionWithInt(self, xt+2*yt);
        movePlayer(self);
        return self->player->lastInstruction;
    }

    fprintf(stderr, "4");

    //Quatre état cherche/demi-tour/rebrousser chemin/petit pousset

    //Si aucune issu dans le sens inverse du dernier choix (empeche de prendre le chemin déja visité si croisement.)
    //Supprimer la possibilité de la pile


    return "NORTH";
    


}
//si on touche un bord horizontal ou vertical tout ce qui est du coté oposé est inutile a vérifier
//si on est a 1 case avant le mur cela ne sert a rien d'aller voir ,(on gagne deux déplacement a chaque fois)