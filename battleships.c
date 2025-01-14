#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 5          // Size of the board
#define NUM_SHIPS 3     // Number of ships

// Ship sizes
const int SHIP_SIZES[NUM_SHIPS] = {1, 2, 3};

// Structure to represent each ship
typedef struct {
    int size;
    int hits;
    int row;
    int col;
    int direction; // 0 for horizontal, 1 for vertical
} Ship;

// Function declarations
void initializeBoard(char board[SIZE][SIZE]);
void placeShips(char board[SIZE][SIZE], Ship ships[NUM_SHIPS]);
void printBoard(char board[SIZE][SIZE]);
int checkHit(char board[SIZE][SIZE], int row, int col, Ship ships[NUM_SHIPS]);
int allShipsSunk(char board[SIZE][SIZE]);

int main() {
    char board[SIZE][SIZE];
    Ship ships[NUM_SHIPS];
    int row, col;
    int attempts = 0;

    srand(time(0));
    initializeBoard(board);
    placeShips(board, ships);

    printf("Welcome to Battleships! Try to sink all enemy ships.\n");

    while (!allShipsSunk(board)) {
        printBoard(board);
        printf("Enter coordinates to fire (row and column): ");
        scanf("%d %d", &row, &col);

        // Validate coordinates
        if (row < 0 || row >= SIZE || col < 0 || col >= SIZE) {
            printf("Invalid coordinates. Please try again.\n");
            continue;
        }

        // Check if the spot has already been chosen
        if (board[row][col] == 'X' || board[row][col] == '-') {
            printf("You already fired at this location. Choose another spot.\n");
            continue;
        }

        attempts++;
        int hitResult = checkHit(board, row, col, ships);
        if (hitResult == 1) {
            printf("Hit!\n");
            board[row][col] = 'X';
        } else if (hitResult == 2) {
            printf("Hit! You sunk a ship!\n");
            board[row][col] = 'X';
        } else {
            printf("Miss.\n");
            board[row][col] = '-';
        }
    }

    printf("Congratulations! You sank all the ships in %d attempts.\n", attempts);
    return 0;
}

// Initialize the game board with empty water ('~')
void initializeBoard(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = '~';
        }
    }
}

// Place ships of varying sizes randomly on the board without overlapping
void placeShips(char board[SIZE][SIZE], Ship ships[NUM_SHIPS]) {
    for (int i = 0; i < NUM_SHIPS; i++) {
        int shipSize = SHIP_SIZES[i];
        int placed = 0;

        while (!placed) {
            int row = rand() % SIZE;
            int col = rand() % SIZE;
            int direction = rand() % 2;  // 0 for horizontal, 1 for vertical

            // Check if ship can be placed without going out of bounds or overlapping
            int canPlace = 1;
            if (direction == 0) {  // Horizontal
                if (col + shipSize <= SIZE) {
                    for (int j = 0; j < shipSize; j++) {
                        if (board[row][col + j] != '~') {
                            canPlace = 0;
                            break;
                        }
                    }
                    if (canPlace) {
                        for (int j = 0; j < shipSize; j++) {
                            board[row][col + j] = 'S';
                        }
                        ships[i] = (Ship){shipSize, 0, row, col, direction};
                        placed = 1;
                    }
                }
            } else {  // Vertical
                if (row + shipSize <= SIZE) {
                    for (int j = 0; j < shipSize; j++) {
                        if (board[row + j][col] != '~') {
                            canPlace = 0;
                            break;
                        }
                    }
                    if (canPlace) {
                        for (int j = 0; j < shipSize; j++) {
                            board[row + j][col] = 'S';
                        }
                        ships[i] = (Ship){shipSize, 0, row, col, direction};
                        placed = 1;
                    }
                }
            }
        }
    }
}

// Print the board for the player (hide ships)
void printBoard(char board[SIZE][SIZE]) {
    printf("  ");
    for (int i = 0; i < SIZE; i++) printf("%d ", i);
    printf("\n");

    for (int i = 0; i < SIZE; i++) {
        printf("%d ", i);
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 'S') {
                printf("~ ");
            } else {
                printf("%c ", board[i][j]);
            }
        }
        printf("\n");
    }
}

// Check if player's guess is a hit, miss, or if a ship is sunk
int checkHit(char board[SIZE][SIZE], int row, int col, Ship ships[NUM_SHIPS]) {
    if (board[row][col] != 'S') {
        return 0;  // Miss
    }

    // Check which ship was hit and update its hit count
    for (int i = 0; i < NUM_SHIPS; i++) {
        Ship *ship = &ships[i];
        int hit = 0;

        // Check if the hit coordinates match this ship's position
        if (ship->direction == 0) {  // Horizontal
            if (row == ship->row && col >= ship->col && col < ship->col + ship->size) {
                hit = 1;
            }
        } else {  // Vertical
            if (col == ship->col && row >= ship->row && row < ship->row + ship->size) {
                hit = 1;
            }
        }

        if (hit) {
            ship->hits++;
            if (ship->hits == ship->size) {
                return 2;  // Ship sunk
            }
            return 1;  // Ship hit but not sunk
        }
    }
    return 0;  // Should not reach here
}

// Check if all ships are sunk
int allShipsSunk(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == 'S') {
                return 0;
            }
        }
    }
    return 1;
}
