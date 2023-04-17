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
        switch (temp) {
            case 'w':
                current_dir = UP;
                break;
            case 's':
                current_dir = DOWN;
                break;
            case 'a':
                current_dir = LEFT;
                break;
            case 'd':
                current_dir = RIGHT;
                break;
            default:
                current_dir = current_dir;
                break;
        }

    }
    endGame = 'q';
    endwin();
    pthread_exit(NULL);
}

void move_player(char *board, int* pos, int* alt_pos){
    alt_pos[1] = pos[1];
    alt_pos[0] = pos[0];
    switch (current_dir) {
        case UP:
            pos[1]--;
            break;
        case DOWN:
            pos[1]++;
            break;
        case LEFT:
            pos[0]--;
            break;
        case RIGHT:
            pos[0]++;
            break;
        default:
            break;
    }
    if((pos[0] == 0 || pos[0] == WIDTH) || (pos[1] == 0 || pos[1] == HEIGHT)){
        endGame = 'q';
        return;
    }
    board[(WIDTH * pos[1]) + pos[0]] = '@';
    board[(WIDTH * alt_pos[1]) + alt_pos[0]] = ' ';

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
    int *pos = (int*) malloc(sizeof(int) * 2);
    int *alt_pos = (int*) malloc(sizeof (int)*2);
    pos[1] = HEIGHT / 2;
    pos[0] = WIDTH / 2;
    while (endGame != 'q') {
        i++;
        print_board(board);
        printw("%d\t%d\n", i, current_dir);
        spawn_food(board);
        refresh();
        usleep(500000);
        move_player(board, pos, alt_pos);
    };
    pthread_join(pthread, NULL);

    free(board);
    free(pos);
    return 0;
}
