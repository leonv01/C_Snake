#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

const static int HEIGHT = 20, WIDTH = 70;
enum direction {
    UP, DOWN, RIGHT, LEFT
};
enum direction current_dir = UP;

_Noreturn void *read_input(void *arg) {
    while (1) {
        char temp = (char) getchar();
        switch (temp) {
            case 'w':
                current_dir = UP;
                break;
            case 'a':
                current_dir = LEFT;
                break;
            case 'd':
                current_dir = RIGHT;
                break;
            case 's':
                current_dir = DOWN;
                break;
        }
        usleep(500);


    }
    pthread_exit(NULL);
}

void set_direction() {

}

void init_board(char *board) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char wall;
            if (j == 0 || j == WIDTH - 1) {
                wall = '|';
            } else if (i == 0 || i == HEIGHT - 1) {
                wall = '-';
            } else {
                wall = ' ';
            }
            board[(WIDTH * i) + j] = wall;
        }
    }
}

void print_board(char *board) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[(WIDTH * i) + j]);
        }
        printf("\n");
    }
}

int main() {
    char *board = (char *) malloc(sizeof(char) * HEIGHT * WIDTH);
    pthread_t pthread;
    int ret;

    ret = pthread_create(&pthread, NULL, read_input, NULL);
    if (ret != 0) {
        printf("Error\n");
        return 1;
    }

    init_board(board);
    char in;
    int i = 0;

    while (i < 100) {
        i++;
        system("cls");
        print_board(board);
        printf("%d\n", current_dir);
        sleep(1);
    };
    pthread_join(pthread, NULL);

    free(board);
    return 0;
}
