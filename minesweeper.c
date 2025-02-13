#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// structure of each cell in minesweeper board
struct cell {
   int position;
   int adjcount;
   int mined;
   int covered;
   int flagged;
};

typedef struct cell cell;

// intialize each cell with default values
void initalize_cell(cell *c, int p) {
    c->position = p;
    c->adjcount = 0;
    c->mined = 0;
    c->covered = 1;
    c->flagged = 0;
}

// global variables
cell **board;
int rows;
int cols;
int mines;

// creates random number within the given range
int get_random(int range) {
    return ((int)floor((float)range * rand() / RAND_MAX)) % range;
}

// calculates number of adjacent cells containing mines 
void calculate_adjacency_counts(int row, int col) {
    for (int i = row - 1; i <= row + 1; i++) {
        for (int j = col - 1; j <= col + 1; j++) {
            if (i >= 0 && i < rows && j >= 0 && j < cols && !(i == row && j == col)) {
                if (board[i][j].mined) {
                    board[row][col].adjcount++;
                }
            }
        }
    }
}

// initializes new board
void command_new(void) {
    // creates new board
    board = (cell **) malloc(sizeof(cell *) * rows);
    for (int i=0; i < rows; i++) {
        board[i] = (cell *) malloc( sizeof(cell) * cols);
    }
    // initializes each cell
    for (int i=0; i < rows; i++) {
        for (int j=0; j < cols; j++) {
            initalize_cell(&board[i][j], i * cols + j);
        }
    }
    int mineCount = 0;
    while (mineCount < mines) {
        // randomly distribute mines
        int randRow = get_random(rows);
        int randCol = get_random(cols);

        if (!board[randRow][randCol].mined) {
            board[randRow][randCol].mined = 1;
            mineCount++;
        }
    }
    // calculates all mines in adjacent cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (!board[i][j].mined) {
                calculate_adjacency_counts(i, j);
            }
        }
    }
}

void cover_all_cells(void) {
    // covers all cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].covered = 1;
        }
    }
}

void uncover_all_cells(void) {
    // uncovers all cells
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            board[i][j].covered = 0;
        }
    }
}

void command_flag(int r, int c) {
    // flag cell
    if (board[r][c].flagged == 0) {
        board[r][c].flagged = 1;
        printf("Cell at (%d, %d) flagged.\n", r, c);
    } else {
        printf("Cell at (%d, %d) is already flagged.\n", r, c);
    }
}

void command_unflag(int r, int c) {
    // unflag cell
    if (board[r][c].flagged == 1) {
        board[r][c].flagged = 0;
        printf("Cell at (%d, %d) unflagged.\n", r, c);
    } else {
        printf("Cell at (%d, %d) is not flagged.\n", r, c);
    }
}

void uncover_recursive(int row, int col);

// uncovers a cell and reveals its contents
void command_uncover(int row, int col) {
    // checks if cell is covered or flagged
    if (board[row][col].covered == 0 || board[row][col].flagged == 1) {
        printf("Cell (%d, %d) is flagged or already open.\n", row, col);
        return;
    }
    // check if cell has mine
    if (board[row][col].mined) {
        printf("Game over! You uncovered a mine at (%d, %d).\n", row, col);
        return;
    }
    // uncovers selected cell
    board[row][col].covered = 0;

    // uncover surrounding cells if thier adjcount is 0
    if (board[row][col].adjcount == 0) {
        uncover_recursive(row, col);
    }
}

// uncovers adjacent empty cells
void uncover_recursive(int row, int col) {
    // uncover the selected cell
    board[row][col].covered = 0;

    if (board[row][col].adjcount == 0) {
        for (int i = row - 1; i <= row + 1; i++) {
            for (int j = col - 1; j <= col + 1; j++) {
                if (i >= 0 && i < rows && j >= 0 && j < cols && !(i == row && j == col) &&
                    board[i][j].covered == 1) {
                    uncover_recursive(i, j);
                }
            }
        }
    }
}

// displays a single cell
void display_cell(cell *c) {
    if (c->covered) {
        printf("%3s", "/");
    } else {
        if (c->mined == 1) {
            printf("%3s", "*");
        } else if (c->adjcount == 0) {
            printf("%3s", ".");
        } else {
            printf("%3d", c->adjcount);
        }
    }
}

// displays current board state (**kept for development process)
void command_show(void) {
   for (int i = 0; i < rows; i ++) {
      for (int j = 0; j < cols; j++) {
         display_cell(&board[i][j]);
      }
      printf("\n");
   }
}


#define MAXCHARLENGTH 80
#define MAXTOKENS 20
#define MAXTOKENCOUNT 20
#define MAXTOKENLENGTH 20
#define MAXLINELENGTH 400


// executes user's inputs
int processcommand(char tokens[][MAXTOKENLENGTH], int tokencount) {
    if (strcmp(tokens[0], "new") == 0) {
        printf("New Board Command\n");
        rows = atoi(tokens[1]);
        cols = atoi(tokens[2]);
        mines = atoi(tokens[3]);
        command_new();
        
    } else if (strcmp(tokens[0], "show") == 0) {
        printf("Show Board Command\n");
        command_show();
        
    } else if (strcmp(tokens[0], "coverall") == 0) {
        printf("Cells Covered Command\n");
        cover_all_cells();
        
    } else if (strcmp(tokens[0], "uncoverall") == 0) {
        printf("Cells Uncovered Command\n");
        uncover_all_cells();
        
    } else if (strcmp(tokens[0], "flag") == 0) {
        int row = atoi(tokens[1]);
        int col = atoi(tokens[2]);
        command_flag(row, col);
        
    } else if (strcmp(tokens[0], "unflag") == 0) {
        int row = atoi(tokens[1]);
        int col = atoi(tokens[2]);
        command_unflag(row, col);
        
    } else if (strcmp(tokens[0], "uncover") == 0) {
        int row = atoi(tokens[1]);
        int col = atoi(tokens[2]);
        command_uncover(row, col);
        
    } else if (strcmp(tokens[0], "quit") == 0) {
        printf("Bye!\n");
        return 0;
    }
    return 1;
}

// reads user's input
void get_line(char line[], int maxlinelen) {
    fgets( line, maxlinelen, stdin);
    int linelen = strlen(line);
    if (line[linelen-1] == '\n') {
        line[linelen-1] = '\ ';
    }
}

// tokenize user's inputs
void get_tokens(char line[], char tokens[][MAXTOKENLENGTH], int *count) {
   char linecopy[MAXCHARLENGTH];
   strcpy(linecopy, line);

   char *tok;
   tok = strtok(linecopy, " ");
   *count = 0;
   while (tok != NULL && *count < MAXTOKENS) {
      strcpy(tokens[*count], tok);
      (*count)++;
      tok = strtok(NULL, " ");
   }
}

// loops game
int rungame(void) {
    char line[MAXCHARLENGTH];
    char tokens[MAXTOKENS][MAXCHARLENGTH];
    int tokencount;
    char *tok;
    
    while (1) {
        int tokencount;
        int result;
        printf(">> ");
        get_line(line, MAXLINELENGTH);
        get_tokens(line, tokens, &tokencount);
        result = processcommand(tokens,tokencount);
        if (result == 0) {
            break;
        }
    }
    return 1;
}


int main(void) {
   rungame();
   return 0;
}
         
