gcc -Wall -g -O2 -std=c99 -o autoPlayer autoPlayer.c OpGrilleV2.c

#####Launch comparason between 2 algos
./launch.sh ./algo1 ./algo2

Ex:
./launch.sh ./autoPlayer ./labyrinth-10
