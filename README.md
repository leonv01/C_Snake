# Snake Game with ncurses

This is a simple implementation of the classic Snake game using the ncurses library in C. It allows the player to control a snake using the WASD keys, with the objective pf eating food (+) and growing longer withouth hitting the walls or itself.

## Prerequisites
To compile and run this program, you need to have the ncurses library installed on your system. If you're using a Debian-based Linux distribution, you can install it by running the following command:

```
sudo apt-get install libncurses5-dev libncursesw5-dev
```


###Compilation
To compile the program, use the following command:
```
gcc main.c - main -lncurses
```

####Usage
Run the compiled program using the following command:
```
./main [HEIGHT WIDTH]
```
By default, if no height and width arguments are provided, the game will be displayed in a 20x40 terminal window.

#####Controls
Use the following keys to control the snake:
- W: Move up
- S: Move down
- A: Move left
- D: Move right
- Q: Quit the game

#######Gameplay
- The snake is represented by the `@` symbol, and it starts in the middle of the game board.
- Move the snake around the board using the control keys.
- The snake will grow in length whenever it eats a piece of food (`+`).
- The game ends if the snake hits the walls or collides with itself.
- The length of the snake is displayed at the bottom of the game board.

########Acknowledgements
This Snake game implementation is based on the ncurses library, which provides a convenient way to create terminal-based user interfaces in C.

########License
This program is licensed under the **MIT License**. Feel free to modify and distribute it according to the terms of the license.
