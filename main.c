#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>

const static int HEIGHT = 20, WIDTH = 70, REFRESH = 500000;
const char WALL = '|', CEILING = '_', SPACE = '.', FOOD = '+';
char endGame = ' ';
char* board;
int foodCount;
int* foodPos;
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


void player_grow(int* pos_alt, char* board){
    board[(WIDTH * pos_alt[1]) + pos_alt[0]] = '#';
}
void spawn_food(char *board){
    foodCount++;
    srand(time(NULL));
    foodPos[0] = (1 + random()) % (WIDTH - 1);
    foodPos[1] = (1 + random()) % (HEIGHT - 1);

    board[(WIDTH * foodPos[1]) + foodPos[0]] = FOOD;
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
    if(pos[0] == foodPos[0] && pos[1] == foodPos[1]){
        spawn_food(board);
    }
    board[(WIDTH * pos[1]) + pos[0]] = '@';
    board[(WIDTH * alt_pos[1]) + alt_pos[0]] = ' ';
    player_grow(alt_pos, board);
}

void init_board() {
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

void print_board() {
    clear();
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printw("%c", board[(WIDTH * i) + j]);
        }
        printw("\n");
    }
}




_Noreturn void *graph_update(void *arg){
    while(endGame != 'q'){
        print_board();
        refresh();
        usleep(REFRESH);
    }
    pthread_exit(NULL);
}

int main() {

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    board = (char *) malloc(sizeof(char) * HEIGHT * WIDTH);
    pthread_t input_thread, graph_thread;
    int ret;

    ret = pthread_create(&input_thread, NULL, read_input, NULL);
    if (ret != 0) {
        printf("Error\n");
        return 1;
    }
    ret = pthread_create(&graph_thread, NULL, graph_update, NULL);

    init_board();
    char in;
    int i = 0;
    int *pos = (int*) malloc(sizeof(int) * 2);
    int *alt_pos = (int*) malloc(sizeof (int)*2);
    foodPos = (int*) malloc(sizeof (int) * 2);
    pos[1] = HEIGHT / 2;
    pos[0] = WIDTH / 2;
    spawn_food(board);
    while (endGame != 'q') {
        i++;
        move_player(board, pos, alt_pos);
        usleep(REFRESH);
    };
    pthread_join(input_thread, NULL);
    pthread_join(graph_thread, NULL);
    free(alt_pos);
    free(board);
    free(pos);
    return 0;
}
