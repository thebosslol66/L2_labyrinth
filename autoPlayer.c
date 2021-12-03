#include "autoPlayer.h"

#include "OpGrille.h"

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

int main() {
    srand(time(NULL));
    setbuf(stdout, NULL);

    struct Board board;

    char buf[BUFSIZE];
    // get the width
    fgets(buf, BUFSIZE, stdin);
    int width = atoi(buf);
    // get the height
    fgets(buf, BUFSIZE, stdin);
    int height = atoi(buf);
    // get the initial x
    fgets(buf, BUFSIZE, stdin);
    int x = atoi(buf);
    // get the initial y
    fgets(buf, BUFSIZE, stdin);
    int y = atoi(buf);
    // get the treasure x
    fgets(buf, BUFSIZE, stdin);
    int xt = atoi(buf);
    // get the treasure y
    fgets(buf, BUFSIZE, stdin);
    int yt = atoi(buf);



    // initialize the state of the game
    board_create(&board, width, height, x, y, xt, yt);



    fgets(buf, BUFSIZE, stderr);
    fprintf(stderr, "Flush : %s\n", buf);

    for (;;) {
    // get the adjacent cells
    fgets(buf, BUFSIZE, stdin);
    // update the state of the game
    // send the new direction
    puts(think(buf)); // or "SOUTH" or "EAST" or "WEST"
    // get the result
    fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "END\n") == 0) {
    break;
    }
    }
    return 0;
}