#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const static int HEIGHT = 30, WIDTH = 50;

void init_board(char* board) {
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            char temp = j == 0 || j == WIDTH-1 ? '|' : i == 0 || i == HEIGHT-1 ? '-' : ' ';
            board[(WIDTH * i) + j] = temp;
        }
    }
}

void print_board(char* board) {
    for(int i = 0; i < HEIGHT; i++) {
        for(int j = 0; j < WIDTH; j++) {
            printf("%c", board[(WIDTH * i) + j]);
        }
        printf("\n");
    }
}

int main() {
    char* board = (char*) malloc(sizeof(char) * HEIGHT * WIDTH);
    init_board(board);
    for (int i = 0; i < 100; ++i) {
        system("cls");
        print_board(board);
        sleep(1);
    }
    free(board);
    return 0;
}
