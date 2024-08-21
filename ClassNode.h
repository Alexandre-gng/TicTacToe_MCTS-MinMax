/*
 * Node Class by A.G for MCTS and MinMax algorithm
 *
 */
#ifndef TICTACTOE_CLASSNODE_H
#define TICTACTOE_CLASSNODE_H

#include "StructMinMax.h"

using namespace std;

class Node {
public:
    Grid g;
    vector<Node*> ChildNodes;
    Node* ParentNode;

    int NodeValue;
    int VisitCount;
    double UCTValue;
    bool isAIPlaying;
    bool isLeaf;
    bool isFirstNode;


    Node() {
    }
    Node(Node* ParentN) {
        g = ParentN->g;
        ParentNode = ParentN;
        NodeValue = 0;
        VisitCount = 0;
        UCTValue = 0;
        isLeaf = true;
        isFirstNode = false;
    }

    // Play a single move using MCTS algorithm
    Node* playMoveMCTS(int, Node*);


    // =============== Initialisation ===============
    // Generates the first layer of the first node
    void GenerateInitialsChildsNodes();
    // Initialisation of the first ever node and its childs
    void InitFirstNode();
    // Initialisation of the first node, after a play, and its childs
    void InitNewFirstNode(Grid);

    // =============== MCTS ===============
    Node* SelectFinalChildNode();
    // 1. Selection
    double UCT();
    Node* SelectChildNode();
    // 2. Expansion
    Node* GenerateRandomChildNode();
    // 3. Simulation
    int Rollout();
    // 4. Backpropagation
    void BackPropagation(int);


    // =============== Random checks ===============
    // Checks if a Node has all of its child nodes
    bool ChildNodesLeft();
    // Checks if
    bool IsThereChild(int, int);
};

// Initialisation of the first node and its childs
Node* InitFirstNode();

#endif //TICTACTOE_CLASSNODE_H
