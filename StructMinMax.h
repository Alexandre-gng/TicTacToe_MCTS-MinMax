//
// Created by alexa on 30/04/2024.
//

#ifndef TICTACTOE_STRUCTMINMAX_H
#define TICTACTOE_STRUCTMINMAX_H

#include <iostream>
#include <vector>
#include <string>
// MTCS ln
#include <math.h>
// Random for MTCS
#include <random>
#include <time.h>

using namespace std;


typedef struct Grid {
    vector<vector<int>> grid;
} Grid ;


// ================ Grid things  =================
// Creates a grid
Grid* createGrid();
// Prints grid
void printGrid(Grid*);


// ================ Checks ======================
// Checks if any player is winning
int isWinning(Grid*);
// Checks if there is still a possible move
bool isMovesLeft(Grid*);

// Combining both of the previous checking functions
bool isGameEnd(Grid*);


// ================== MiniMax ======================
// The AI with the MiniMax algorithm
// (last two int are for Alpha–beta pruning)
int MiniMax(Grid, int, bool, int, int);
// Doing things with the MiniMax (setting the first layer of possible grids)
Grid playMoveMinMax(Grid, int);


#endif //TICTACTOE_STRUCTMINMAX_H