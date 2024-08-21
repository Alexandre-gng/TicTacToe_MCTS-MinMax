/*
 * if 0 => none
 * if -1 => black = 0
 * if  1 => white = X
 */

#include "StructMinMax.h"


// Creates the grid
Grid* createGrid(){
    Grid* ptrG = new Grid;
    Grid G = *ptrG;
    ptrG->grid ={{0, 0, 0},
                 {0, 0, 0},
                 {0, 0, 0}};
    return ptrG;
}

// Textual print of the grid
void printGrid(Grid* g) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (g->grid[i][j] == 0) {
                cout << "#";
            }
            if (g->grid[i][j] == -1) {
                cout << "O";
            }
            if (g->grid[i][j] == 1) {
                cout << "X";
            }
        }
        cout << "\n";
    }
}

// Detects if there is no more move left
bool isMovesLeft(Grid* g) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j ++) {
            if (g->grid[i][j] == 0) {
                return true;
            }
        }
    }
    return false;
}

// Detects if a player won
int isWinning(Grid* g) {
    for (int i = 0; i < 3; i++) {
        int cpt = 0;
        int cpt2 = 0;
        if (g->grid[0][0] == g->grid[1][1] && g->grid[1][1] == g->grid[2][2]) {
            return g->grid[1][1];
        }
        if (g->grid[0][2] == g->grid[1][1] && g->grid[1][1] == g->grid[2][0]) {
            return g->grid[1][1];
        }
        for (int j = 0; j < 3; j++) {
            if (g->grid[i][j] == -1) {
                cpt--;
            }
            if (g->grid[i][j] == 1) {
                cpt++;
            }
            if (cpt == 3) {
                return 1;
            }
            if (cpt == -3) {
                return -1;
            }

            // ===== OTHER THINGS =====
            if (g->grid[j][i] == -1) {
                cpt2--;
            }
            if (g->grid[j][i] == 1) {
                cpt2++;
            }
            if (cpt2 == 3) {
                return 1;
            }
            if (cpt2 == -3) {
                return -1;
            }
        }
    }
    return 0;
}

// Combining isWinning() and isMovesLeft()
bool isGameEnd(Grid* ptrG) {
    if ((isWinning(ptrG) != 0 || !isMovesLeft(ptrG))) {
        if (isWinning(ptrG) == -1) {
            return true;
        }
        if (isWinning(ptrG) == 1) {
            return true;
        }
        if (!isMovesLeft(ptrG)) {
            return true;
        }
    }
    return false;
}


// MinMax algorithm
// g = actual grid | depth = depth | if player = -1: black is playing, if player = 1: white is playing
int MinMax(Grid G, int depth, bool MAXIMISING, int alpha, int beta) {
    int score = isWinning(&G);
    if (score != 0) {
        return score*10 - depth;
    }
    if (!isMovesLeft(&G) || depth == 0) {
        return 0;
    }
    else {
        if(MAXIMISING) {
            int maxEval = -1000;
            for (int i = 0; i < 3; i ++) {
                for (int j = 0; j < 3; j++) {
                    if (G.grid[i][j] == 0) {
                        Grid newGrid = G;
                        Grid* ptrnewGrid = &newGrid;
                        ptrnewGrid->grid[i][j] = 1;
                        int eval = MinMax(newGrid, depth - 1, false, alpha, beta);
                        maxEval = max(maxEval, eval);
                        // Alpha-beta prunning
                        if (maxEval > beta) {
                            return maxEval;
                        }
                        alpha = max(alpha, maxEval);
                    }
                }
            }
            return maxEval;
        } else {
            int minEval = 1000;
            for (int i = 0; i < 3; i ++) {
                for (int j = 0; j < 3; j++) {
                    if (G.grid[i][j] == 0) {
                        Grid newGrid = G;
                        Grid* ptrnewGrid = &newGrid;
                        ptrnewGrid->grid[i][j] = -1;
                        int eval = MinMax(newGrid, depth - 1, true, alpha, beta);
                        minEval = min(minEval, eval);
                        // Alpha-beta prunning
                        if (minEval < alpha) {
                            return minEval;
                        }
                        beta = min(minEval, beta);
                    }
                }
            }
            return minEval;
        }
    }
}

// Doing things with the MiniMax (setting the first layer of possible grids)
Grid playMoveMinMax(Grid G, int depth) {
    Grid* ptrbestGrid = new Grid;
    int alpha = -1000;
    int beta = 1000;
    Grid bestGrid = *ptrbestGrid;
    int MAXeval = -1000;
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j++) {
            if (G.grid[i][j] == 0) {
                Grid newGrid = G;
                newGrid.grid[i][j] = 1;
                if (isWinning(&newGrid)) {
                    return newGrid;
                }
                int TEMPscore = MinMax(newGrid, depth, false, alpha, beta);
                if (TEMPscore >= MAXeval) {
                    MAXeval = TEMPscore;
                    bestGrid = newGrid;
                }
            }
        }
    }
    return bestGrid;
}
