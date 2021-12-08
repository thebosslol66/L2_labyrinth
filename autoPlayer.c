#include "autoPlayer.h"

#include "OpGrille.h"

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
    board_destroy(&board);
    return 0;
}