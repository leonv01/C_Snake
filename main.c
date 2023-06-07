#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>

static int HEIGHT, WIDTH;
const static int REFRESH = 250000;
const static char WALL = '|', CEILING = '_', SPACE = '.', FOOD = '+', HEAD = '@', TAIL = '#';
char endGame = ' ';
char* board;
int* player;
int length;
int* foodPos;
WINDOW* win;

enum direction {
    UP, DOWN, RIGHT, LEFT
};
enum direction current_dir = UP;

_Noreturn void *read_input(void *arg) {
    char temp = ' ';
    while (temp != 'q'||endGame != 'q') {
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

void spawn_food(char *board){
    srand(time(NULL));
    foodPos[0] = (1 + random()) % (WIDTH - 1);
    foodPos[1] = (1 + random()) % (HEIGHT - 1);

    board[(WIDTH * foodPos[1]) + foodPos[0]] = FOOD;
}

void move_player(char *board, int *playerPos){
    int deltaHeadX = playerPos[0];
    int deltaHeadY = playerPos[1];
   
    switch (current_dir) {
        case UP:
            playerPos[1]--;
            break;
        case DOWN:
            playerPos[1]++;
            break;
        case LEFT:
            playerPos[0]--;
            break;
        case RIGHT:
            playerPos[0]++;
            break;
        default:
            break;
    }
    if(playerPos[0] == foodPos[0] && playerPos[1] == foodPos[1]){
	   spawn_food(board);
	   length++;
    }
    
    
    board[(WIDTH * playerPos[1]) + playerPos[0]] = HEAD;
    board[(WIDTH * deltaHeadY) + deltaHeadX] = SPACE;
    int deltaTailX;	
    int deltaTailY;
    //Tail render
    for(int i = 1; i <= length; i++){
	int temp = (i * 2);
	deltaTailX = playerPos[temp];
	deltaTailY = playerPos[temp + 1];

	playerPos[temp] = deltaHeadX;
	playerPos[temp+1] = deltaHeadY;

	if(playerPos[0] == playerPos[temp] && playerPos[1] == playerPos[temp + 1]){
		endGame = 'q';		
	}

	board[(WIDTH * deltaHeadY) + deltaHeadX] = TAIL;
	board[(WIDTH * deltaTailY) + deltaTailX] = SPACE;

	deltaHeadX = deltaTailX;
	deltaHeadY = deltaTailY;
    }

    
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
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            mvwprintw(win, i, j, "%c", board[(WIDTH * i) + j]);
        }
    }
    mvwprintw(win, HEIGHT, 0, "Length: %d", (length * 10));
    wrefresh(win);
}

_Noreturn void *graph_update(void *arg){
    initscr();
    nodelay(stdscr, TRUE);

    while(endGame != 'q'){
        print_board();
        usleep(REFRESH);
    }
    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if(argc > 3){
	HEIGHT = atoi(argv[0]);
	WIDTH = atoi(argv[1]);
    }
    else{
	HEIGHT = 20;
	WIDTH = 40;
    }

    initscr();
    win = newwin(HEIGHT + 1, WIDTH, 0, 0);
    cbreak();
    noecho();
    nodelay(win, TRUE);
    
    
    length = 0;

    board = (char *) malloc(sizeof(char) * HEIGHT * WIDTH);
    player = (int *) malloc((sizeof(int) * 2) * HEIGHT * WIDTH);
    pthread_t input_thread, graph_thread;
    int ret;

    player[0] = HEIGHT / 2;
    player[1] = WIDTH / 2;

    ret = pthread_create(&input_thread, NULL, read_input, NULL);
    if (ret != 0) {
        printf("Error\n");
        return 1;
    }
    ret = pthread_create(&graph_thread, NULL, graph_update, NULL);
    if (ret != 0){
	printf("Error\n");
	return 1;
    }	

    init_board();

    char in;
    int i = 0;
    foodPos = (int*) malloc(sizeof (int) * 2);

    spawn_food(board);
    while (endGame != 'q') {
        i++;
        move_player(board, player);
        usleep(REFRESH);
    };

    pthread_join(input_thread, NULL);
    pthread_join(graph_thread, NULL);

    delwin(win);
    endwin();

    free(board);
    free(player);

    return 0;
}
