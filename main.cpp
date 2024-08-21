#include "StructMinMax.h"
#include "ClassNode.h"

using namespace std;

int main() {
    // Generates new seed with current time
    srand(time(NULL));

    int x, y;
    int depth = 3;
    int ChoiceAlg;
    int ITERATIONS = 10000;

    cout << "Wich algorithm to use: \n- MinMax (1)\n- MTCS with UCB1 (2)" << endl;
    cin >> ChoiceAlg;

    // MCTS with UCB1
    if (ChoiceAlg == 2) {
        string firstToPlay;
        cout << "Do you want to start playing (y/n): ";
        cin >> firstToPlay;

        cout << "How many iterations (1000 recommended): ";
        cin >> ITERATIONS;
        Node *FNode = new Node();
        if (firstToPlay == "y" || firstToPlay == "Y" || firstToPlay == "yes") {
            FNode->InitFirstNode();
            do {
                cout << "Where to put 0 (x): ";
                cin >> x;
                cout << "Where to put 0 (y): ";
                cin >> y;

                FNode->g.grid[x][y] = -1;

                Node* ptr = FNode->playMoveMCTS(ITERATIONS, FNode);
                printGrid(&ptr->g);
                FNode->g = ptr->g;
            } while (!isWinning(&FNode->g) && isMovesLeft(&FNode->g));
        }
        else {
            cout << "============ MCTS play ===========" << endl;
            FNode->InitFirstNode();
            FNode = FNode->playMoveMCTS(ITERATIONS, FNode);
            printGrid(&FNode->g);
            cout << "Where to put 0 (x): ";
            cin >> x;
            cout << "Where to put 0 (y): ";
            cin >> y;
            FNode->g.grid[x][y] = -1;
            cout << "============ Your play: ===========" << endl;
            printGrid(&FNode->g);
            Grid TEMPg;
            bool firstMove = true;
            do {
                Node *SecondNode = new Node();
                if (firstMove) {
                    SecondNode->InitNewFirstNode(FNode->g);
                    SecondNode->g = FNode->g;
                    firstMove = false;
                }
                else {
                    SecondNode->InitNewFirstNode(TEMPg);
                }
                SecondNode = SecondNode->playMoveMCTS(ITERATIONS, SecondNode);
                cout << "============ MCTS play ===========" << endl;
                printGrid(&SecondNode->g);

                if (isGameEnd(&SecondNode->g)) {
                    break;
                }

                // Human playing
                cout << "Where to put 0 (x): ";
                cin >> x;
                cout << "Where to put 0 (y): ";
                cin >> y;
                SecondNode->g.grid[x][y] = -1;
                cout << "============ Your play: ===========" << endl;
                printGrid(&SecondNode->g);
                TEMPg = SecondNode->g;
            } while (!isWinning(&FNode->g) && isMovesLeft(&FNode->g));
        }
    }
    // MinMax alpha-beta
    else {
        do {
            Grid *ptrG = createGrid();

            string firstToPlay;
            cout << "Do you want to start playing (y/n): ";
            cin >> firstToPlay;

            cout << "========= INITAL GRID ==========" << endl;
            printGrid(ptrG);

            do {
                cout << "Wich level of search (1-9): ";
                cin >> depth;
                depth = depth-1;
            } while (depth < 0 || depth > 9);

            if (firstToPlay == "y" || firstToPlay == "Y" || firstToPlay == "yes") {
                do {
                    cout << "Where to put 0 (x): ";
                    cin >> x;
                    cout << "Where to put 0 (y): ";
                    cin >> y;

                    ptrG->grid[x][y] = -1;

                    printGrid(ptrG);

                    if (isGameEnd(ptrG)) {
                        break;
                    }

                    cout << "========== MinMax play ==========" << endl;

                    *ptrG = playMoveMinMax(*ptrG, depth);

                    printGrid(ptrG);

                    if (isGameEnd(ptrG)) {
                        break;
                    }
                } while (!isWinning(ptrG) && isMovesLeft(ptrG));
            } else {
                do {
                    cout << "========== MinMax play ==========" << endl;

                    *ptrG = playMoveMinMax(*ptrG, depth);

                    printGrid(ptrG);

                    if (isGameEnd(ptrG)) {
                        break;
                    }

                    cout << "Where to put 0 (x): ";
                    cin >> x;
                    cout << "Where to put 0 (y): ";
                    cin >> y;

                    ptrG->grid[x][y] = -1;

                    printGrid(ptrG);
                    if (isGameEnd(ptrG)) {
                        break;
                    }

                } while (!isWinning(ptrG) && isMovesLeft(ptrG));
            }
        } while (1);
    }
    cout << "ENF OD THE PROGRAM" << endl;
    return 0;
}