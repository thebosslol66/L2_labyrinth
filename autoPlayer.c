#include "autoPlayer.h"

#include "OpGrilleV2.h"

/**
 * @brief The main function of bot
 *
 * It get all information from the server and use it to respond and find the fastest way to solve labirynth.
 *
**/
int main() {
    srand(time(NULL));
    setbuf(stdout, NULL);

    struct Board board;

    char buf[BUFSIZE];
    char * t;
    // get the width
    t = fgets(buf, BUFSIZE, stdin);
    int width = atoi(buf);
    // get the height
    t = fgets(buf, BUFSIZE, stdin);
    int height = atoi(buf);
    // get the initial x
    t = fgets(buf, BUFSIZE, stdin);
    int x = atoi(buf);
    // get the initial y
    t = fgets(buf, BUFSIZE, stdin);
    int y = atoi(buf);
    // get the treasure x
    t = fgets(buf, BUFSIZE, stdin);
    int xt = atoi(buf);
    // get the treasure y
    t = fgets(buf, BUFSIZE, stdin);
    int yt = atoi(buf);



    // initialize the state of the game
    board_create(&board, width, height, x, y, xt, yt);

    for (;;) {
    // get the adjacent cells
    t = fgets(buf, BUFSIZE, stdin);
    // update the state of the game
    // send the new direction
    board_update(&board, buf);

    puts(think(&board)); // or "SOUTH" or "EAST" or "WEST"
    // get the result
    t = fgets(buf, BUFSIZE, stdin);
    if (strcmp(buf, "END\n") == 0) {
    break;
    }
    }
    board_destroy(&board);
    free(t);
    return 0;
}