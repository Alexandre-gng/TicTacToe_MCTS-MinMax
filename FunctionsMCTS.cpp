#include "StructMinMax.h"
#include "ClassNode.h"

// Init the first node
void Node::InitFirstNode() {
    Grid *ptrG = createGrid();

    this->isLeaf = true;
    this->isAIPlaying = true;
    this->VisitCount = 0;
    this->NodeValue = 0;
    this->UCTValue = 0;
    this->g = *ptrG;
    this->ParentNode = nullptr;
    this->isFirstNode = true;

    this->GenerateInitialsChildsNodes();

    return;
}


// Init a first new node
void Node::InitNewFirstNode(Grid LastGrid) {

    this->isLeaf = true;
    this->isAIPlaying = true;
    this->VisitCount = 0;
    this->NodeValue = 0;
    this->UCTValue = 0;
    this->g = LastGrid;
    this->ParentNode = nullptr;
    this->isFirstNode = true;

    this->GenerateInitialsChildsNodes();

    return;
}


// Chooses the best final Node regarding its node value
Node* Node::SelectFinalChildNode() {
    int MAXNODEVALUE = -1000;
    Node* ptr;
    for (auto o: this->ChildNodes) {
        if (o->NodeValue > MAXNODEVALUE) {
            ptr = o;
            MAXNODEVALUE = o->NodeValue;
        }
    }
    return ptr;
}

// Compares UCT score of every child node to select from wich one expands
Node* Node::SelectChildNode() {
    double TEMPMaxUCT = -1000;
    Node* TEMPMaxChildNode;
    for (auto nChild: this->ChildNodes) {
        if(nChild->UCT() > TEMPMaxUCT) {
            TEMPMaxUCT = nChild->UCT();
            TEMPMaxChildNode = nChild;
        }
    }
    TEMPMaxChildNode->isAIPlaying = !this->isAIPlaying;
    return TEMPMaxChildNode;
}


// Returns how many child nodes are possible (before expansion)
bool Node::ChildNodesLeft() {
    int TEMPcpt = 0;
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j++) {
            if (this->g.grid[i][j] == 0) {
                TEMPcpt++;
            }
        }
    }
    for (auto TEMPvar: this->ChildNodes) {
        TEMPcpt--;
    }
    if (TEMPcpt == 0) {
        return false;
    }
    else {
        return true;
    }
}

bool Node::IsThereChild(int i, int j) {
    for (Node *N: this->ChildNodes) {
        if (N->g.grid[i][j] != 0) {
            return true;
        }
    }
    return false;
}

// Generate a random child node (2. General expansion)
Node* Node::GenerateRandomChildNode() {
    if (isGameEnd(&this->g)) {
        this->NodeValue = isWinning(&this->g);
        return this;
    } else {
        this->isLeaf = false;
        int i;
        int j;
        bool TEMP;
        do {
            i = rand() % 3;
            j = rand() % 3;

        } while (this->g.grid[i][j] != 0 || this->IsThereChild(i, j));

        Node *newChildNode = new Node(this);
        if (this->isAIPlaying && this->g.grid[i][j] == 0) {
            newChildNode->g.grid[i][j] = 1;
            this->ChildNodes.push_back(newChildNode);
            newChildNode->ParentNode = this;
            newChildNode->VisitCount = 0;
            return newChildNode;
        }
        if (!this->isAIPlaying && this->g.grid[i][j] == 0) {
            newChildNode->g.grid[i][j] = -1;
            this->ChildNodes.push_back(newChildNode);
            newChildNode->ParentNode = this;
            newChildNode->VisitCount = 0;
            return newChildNode;
        }
    }
}

// Generates the first layer of child nodes (0. Initial expansion)
void Node::GenerateInitialsChildsNodes() {
    this->isLeaf = false;
    for (int i = 0; i < 3; i ++) {
        for (int j = 0; j < 3; j++) {
            if (this->g.grid[i][j] == 0) {
                Node* newChildNode = new Node(this);
                if (this->isAIPlaying) {
                    newChildNode->g.grid[i][j] = 1;
                }
                else {
                    newChildNode->g.grid[i][j] = -1;
                }
                if (this->isAIPlaying) {
                    newChildNode->isAIPlaying = false;
                }
                else {
                    newChildNode->isAIPlaying = true;
                }
                this->ChildNodes.push_back(newChildNode);
                int RES = newChildNode->Rollout();

                newChildNode->BackPropagation(RES);
            }
        }
    }
    return;
}

// UCT calcul and things
double Node::UCT() {
    // Case of a division by 0
    if (this->VisitCount == 0) {
        this->UCTValue = 1000.0;
    }
    else {
        this->UCTValue = ((this->NodeValue / this->VisitCount) + sqrt(2) * sqrt(log(ParentNode->VisitCount) / VisitCount));
    }
    return this->UCTValue;
}

// Backpropagates NodeValue and VisitCount (4. backpropagation)
void Node::BackPropagation(int a) {
    Node *PN;
    if (this->isLeaf) {
        this->VisitCount++;
    }
    if (this->ParentNode != nullptr) {
        PN = this->ParentNode;
        PN->VisitCount++;
    }
    else {
        return;
    }
    this->ParentNode->NodeValue += a;
    for (auto ChildNode: PN->ChildNodes) {
        int res = ChildNode->UCT();
    }
    if (this->ParentNode != nullptr) {
        PN->BackPropagation(a);
    }
    return;
}

// Random rollout from a specific node (3. Simulation)
int Node::Rollout() {
    if (isGameEnd(&this->g)) {
        this->NodeValue = isWinning(&this->g);
        return this->NodeValue;
    }
    Node RolloutChild = new Node(this);
    RolloutChild.isAIPlaying = this->isAIPlaying;
    //bool SAVEisAiPlaying = this->isAIPlaying;
    while (!isGameEnd(&RolloutChild.g)) {
        int i, j;

        i = rand() % 3;
        j = rand() % 3;
        // If MCTS is playing
        if (RolloutChild.isAIPlaying) {
            while (RolloutChild.g.grid[i][j] != 0) {
                i = rand() % 3;
                j = rand() % 3;
            }
            RolloutChild.g.grid[i][j] = 1;
            RolloutChild.isAIPlaying = false;
        }
            // If human is playing
        else {
            while (RolloutChild.g.grid[i][j] != 0) {
                i = rand() % 3;
                j = rand() % 3;
            }
            RolloutChild.g.grid[i][j] = -1;
            RolloutChild.isAIPlaying = true;
        }
    }
    this->NodeValue = isWinning(&RolloutChild.g);
    // NEW THING:
    //double resUCT = this->UCT();
    return this->NodeValue;
}

Node* Node::playMoveMCTS(int ITERATIONS, Node* FNode) {
    int TEMPRESCur;
    Node* CurentNode = FNode;
    for (int i = 0; i < ITERATIONS; i++) {
        if (CurentNode->isLeaf) {
            if (CurentNode->VisitCount == 0) {
                TEMPRESCur = CurentNode->Rollout();
                CurentNode->BackPropagation(TEMPRESCur);
                CurentNode = FNode;
            }
            else {
                if (CurentNode->ParentNode->ChildNodesLeft()) {
                    CurentNode = CurentNode->ParentNode->GenerateRandomChildNode();
                    TEMPRESCur = CurentNode->Rollout();
                    CurentNode->BackPropagation(TEMPRESCur);
                    CurentNode = FNode;
                }
                else {
                    CurentNode = CurentNode->GenerateRandomChildNode();

                    TEMPRESCur = CurentNode->Rollout();
                    CurentNode->BackPropagation(TEMPRESCur);
                    CurentNode = FNode;
                }
            }
        }
        else {
            CurentNode = CurentNode->SelectChildNode();
        }
    }
    Node* ptr = FNode->SelectFinalChildNode();
    return ptr;
}