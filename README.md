# Minesweeper in C

## Overview
This is a command-line implementation of Minesweeper written in C. The game generates a grid-based minefield where players can uncover cells, flag potential mines, and attempt to clear the board without hitting a mine.

## Features
- Dynamically allocated grid-based board.
- Random mine placement with calculated adjacent mine counts.
- User commands to uncover cells, flag/unflag mines, and display the board.
- Recursive uncovering of empty spaces.
- Simple command-line interface for game interaction.

## Compilation & Execution
### Compile the program using:
```
gcc -o minesweeper minesweeper.c -lm
```
### Run the program:
```
./minesweeper
```

## Commands
The game accepts the following commands:
- `new <rows> <cols> <mines>` - Creates a new board with the specified dimensions and number of mines.
- `show` - Displays the current board state.
- `coverall` - Covers all cells.
- `uncoverall` - Uncovers all cells (debugging purposes).
- `flag <row> <col>` - Flags a cell as a potential mine.
- `unflag <row> <col>` - Removes a flag from a cell.
- `uncover <row> <col>` - Uncovers a cell, revealing its contents.
- `quit` - Exits the game.

## Code Structure
- `cell** struct**`: Represents each cell in the Minesweeper grid.
- `command_new`: Initializes a new game board.
- `calculate_adjacency_counts`: Computes the number of adjacent mines for each cell.
- `processcommand`: Handles user inputs and executes corresponding game functions.
- `rungame`: Main game loop that processes player input.

## Future Improvements
- Implement a graphical user interface (GUI) version.
- Add difficulty settings with preset grid sizes.
