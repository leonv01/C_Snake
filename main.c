#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>

const static int HEIGHT = 20, WIDTH = 70;
const char WALL = '|', CEILING = '_', SPACE = '.', FOOD = '+';
char endGame = ' ';
enum direction {
    UP, DOWN, RIGHT, LEFT
};
enum direction current_dir = UP;

_Noreturn void *read_input(void *arg) {


    char temp = ' ';
    while (temp != 'q') {
         temp = getch();
        printf("%c\n", temp);


    }
    endGame = 'q';
    endwin();
    pthread_exit(NULL);
}

void set_direction() {

}

void init_board(char *board) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            char temp;
            if (j == 0 || j == WIDTH - 1) {
                temp = WALL;
            } else if (i == 0 || i == HEIGHT - 1) {
                temp = CEILING;
            } else {
                temp = SPACE;
            }
            board[(WIDTH * i) + j] = temp;
        }
    }
}

void print_board(char *board) {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printw("%c", board[(WIDTH * i) + j]);
        }
        printw("\n");
    }
}

void spawn_food(char *board){
    srand(time(NULL));
    int xPos = (1 + random()) % (WIDTH - 1);
    int yPos = (1 + random()) % (HEIGHT - 1);

    board[(WIDTH * yPos) + xPos] = FOOD;
}

int main() {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int iteration = 0;
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

    while (endGame != 'q') {
        i++;
        print_board(board);
        printw("%d\n", i);
        sleep(1);
        spawn_food(board);
        refresh();

    };
    pthread_join(pthread, NULL);

    free(board);
    return 0;
}
