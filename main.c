#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <ncurses.h>
#include <locale.h>

struct Player{
    int* x;
    int* y;
};
struct Food{
    int x;
    int y;
};
enum direction {
    UP, DOWN, RIGHT, LEFT
};


static int HEIGHT, WIDTH;
const int REFRESH = 250000;
const char WALL = '|', CEILING = '_', SPACE = '.', FOOD = '+', HEAD = '@', TAIL = '#', EMPTY = '\t';
char endGame = ' ';
char* board;
int length;
WINDOW* win;
struct Food* food;
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
    food->x = (1 + random()) % (WIDTH - 1);
    food->y = (1 + random()) % (HEIGHT - 1);

    board[(WIDTH * food->y) + food->x] = FOOD;
}

void move_player(char *board, struct Player* playerPos){
    int deltaHeadX = playerPos->x[0];
    int deltaHeadY = playerPos->y[0];
   
    switch (current_dir) {
        case UP:
            playerPos->y[0]--;
            break;
        case DOWN:
            playerPos->y[0]++;
            break;
        case LEFT:
            playerPos->x[0]--;
            break;
        case RIGHT:
            playerPos->x[0]++;
            break;
        default:
            break;
    }
    if(playerPos->x[0] == food->x && playerPos->y[0] == food->y){
	   spawn_food(board);
	   length++;
    }
    
    
    board[(WIDTH * playerPos->y[0]) + playerPos->x[0]] = HEAD;
    board[(WIDTH * deltaHeadY) + deltaHeadX] = SPACE;
    int deltaTailX;	
    int deltaTailY;
    //Tail render
    for(int i = 1; i <= length; i++){
	    deltaTailX = playerPos->x[i];
	    deltaTailY = playerPos->y[i];

	    playerPos->x[i] = deltaHeadX;
	    playerPos->y[i] = deltaHeadY;

	    if(playerPos->x[0] == playerPos->x[i] && playerPos->y[0] == playerPos->y[i]){
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
            board[(WIDTH * i) + j] = SPACE;
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
    setlocale(LC_ALL, "");
    initscr();
    win = newwin(HEIGHT + 1, WIDTH, 0, 0);
    cbreak();
    noecho();
    nodelay(win, TRUE);
    
    struct Player player;
    food = malloc(sizeof(uint8_t) * 2);

    length = 0;

    board = (char *) malloc(sizeof(char) * HEIGHT * WIDTH);
    pthread_t input_thread, graph_thread;
    int ret;

    player.x = (int*) malloc((sizeof(int) * HEIGHT * WIDTH));
    player.y = (int*) malloc((sizeof(int) * HEIGHT * WIDTH));

    player.x[0] = WIDTH / 2;
    player.y[0] = HEIGHT / 2;

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

    spawn_food(board);
    while (endGame != 'q') {
        i++;
        move_player(board, &player);
        usleep(REFRESH);
    };

    pthread_join(input_thread, NULL);
    pthread_join(graph_thread, NULL);

    delwin(win);
    endwin();

    free(board);
    free(food);

    return 0;
}
