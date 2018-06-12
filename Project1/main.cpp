/******************************
 * Author: Alperen Kantarcı   *
 * Id : 150140140             *
 *                            *
 *****************************/

 /*****************************************************
  *  IMPORTANT COMPILING NOTE:                        *
  * In order to compile the program use this command: *
  * g++ -std=c++11 main.cpp                           *
  *****************************************************/


#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <queue>
#include <stack>
#include <ctime>

using namespace std;


clock_t tStart, tFinish;
int numberOfBFSNode = 0;
int numberOfDFSNode = 0;

/*
 * This class shows the movement of certain state
 * True means that thing in the boat with farmer
 */

class Move {
public:
    bool Fox, Carrot, Rabbit;

    Move(bool, bool, bool);

    Move();

    string printBoatMove(bool side);
};

Move::Move(){

}

Move::Move(bool keyFox, bool keyRabbit, bool keyCarrot) {
    Fox = keyFox;
    Carrot = keyCarrot;
    Rabbit = keyRabbit;
}

// Prints movement beautifully
string Move::printBoatMove(bool side) {
    string tmp = "(Farmer";
    if (Fox)
        tmp += ", Fox";
    if (Rabbit)
        tmp += ", Rabbit";
    if (Carrot)
        tmp += ", Carrot";
    if (side)
        tmp += ", >";
    else
        tmp += ", <";
    tmp += ")\n";
    return tmp;
}

/*
 * This object shows the current positions of the elements such as farmer,fox ...
 * nullNode true means it's a dummy object and will be used as identifier of the not allowed state
 *
 */

class State {
public:
    bool nullNode = false;
    bool farmerSide = false; //If farmer is in the left side then false else true
    set<string> left;
    set<string> right;

    State(bool, set<string>, set<string>);

    State(bool);

    State();

    bool checkSolutionState();

    bool checkBothSides();

    bool checkIfSideAllows(set<string>);

    void printSides();

    bool equal(State);

    State *makeNewMoveState(Move);
};

State::State() {

}

State::State(bool keyFarmerSide, set<string> keyLeft, set<string> keyRight) {
    farmerSide = keyFarmerSide;
    left = keyLeft;
    right = keyRight;
}

State::State(bool isNull) {
    nullNode = isNull;
}

//Function controls both left and right sides if it's a valid state
bool State::checkBothSides() {
    return checkIfSideAllows(left) && checkIfSideAllows(right) ? true : false;
}

// This function tests if situation cause any problem between the animals
bool State::checkIfSideAllows(set<string> key) {

    //If Farmer is not together with Rabbit and Fox then Fox will eat Rabbit so return false
    if (key.find("Farmer") == key.end() && key.find("Rabbit") != key.end() && key.find("Fox") != key.end())
        return false;


    //If Farmer is not together with Rabbit and Carrot then Rabbit will eat Carrot so return false
    return !(key.find("Farmer") == key.end() && key.find("Rabbit") != key.end() && key.find("Carrot") != key.end());

}

bool State::checkSolutionState() {
    //If Farmer, Carrot, Rabbit and Fox all together on the right side, return true
    return left.empty() && right.find("Farmer") != right.end() && right.find("Carrot") != right.end() &&
           right.find("Rabbit") != right.end() && right.find("Fox") != right.end();
}


void State::printSides() {
    set<string>::iterator it;
    for (it = left.begin(); it != left.end(); it++) {
        string f = *it;
        cout << f << " ";
    }

    cout << "|| ";
    for (it = right.begin(); it != right.end(); it++) {
        string f = *it;
        cout << f << " ";
    }
}

/*
 * This function creates new State which is created from current state
 * to new state with move that has been passed.
 * If move is not possible(Fox in the right side but move tries to carry fox from left to right)
 * it will return nullNode=true so in graph creation we will control it easier.
 * Also this moving state always assume that farmer transporting to the other side.
 */
State *State::makeNewMoveState(Move move) {
    bool keyFarmerSide;
    set<string> keyLeft = left;
    set<string> keyRight = right;

    keyFarmerSide = !farmerSide;

    set<string>::iterator it;
    if (keyFarmerSide) { //Farmer went to the right

        if (keyLeft.find("Farmer") != keyLeft.end()) {
            keyLeft.erase("Farmer");
            keyRight.insert("Farmer");

        } else return new State(true);


        if (move.Fox) {
            if (keyLeft.find("Fox") != keyLeft.end()) {
                keyLeft.erase("Fox");
                keyRight.insert("Fox");
            } else return new State(true);
        }

        if (move.Rabbit) {
            if (keyLeft.find("Rabbit") != keyLeft.end()) {
                keyLeft.erase("Rabbit");
                keyRight.insert("Rabbit");
            } else return new State(true);
        }

        if (move.Carrot) {
            if (keyLeft.find("Carrot") != keyLeft.end()) {
                keyLeft.erase("Carrot");
                keyRight.insert("Carrot");
            } else return new State(true);
        }

    } else {              //Farmer went to the left

        if (keyRight.find("Farmer") != keyRight.end()) {
            keyRight.erase("Farmer");
            keyLeft.insert("Farmer");
        } else return new State(true);


        if (move.Fox) {
            if (keyRight.find("Fox") != keyRight.end()) {
                keyRight.erase("Fox");
                keyLeft.insert("Fox");
            } else return new State(true);
        }

        if (move.Rabbit) {
            if (keyRight.find("Rabbit") != keyRight.end()) {
                keyRight.erase("Rabbit");
                keyLeft.insert("Rabbit");
            } else return new State(true);
        }

        if (move.Carrot) {
            if (keyRight.find("Carrot") != keyRight.end()) {
                keyRight.erase("Carrot");
                keyLeft.insert("Carrot");
            } else return new State(true);
        }
    }


    return new State(keyFarmerSide, keyLeft, keyRight);
}

// If two states are equal return true else false
bool State::equal(State key) {
    set<string> temp;
    set<string>::iterator it;
    if (farmerSide != key.farmerSide)
        return false;

    temp = key.left;
    for (it = left.begin(); it != left.end(); ++it) {
        if (temp.find(*it) == temp.end())
            return false;
    }
    temp = key.right;
    for (it = right.begin(); it != right.end(); ++it) {
        if (temp.find(*it) == temp.end())
            return false;
    }

    return true;
}

/*
 * This object modelling graph nodes and nodes has parent pointers and child nodes
 * and also current state which models riddle state in this node of the graph.
 * currentMove string is just a variable for printing at the end of the program.So it changes
 * according to the move and state
 */
class Node {
public:
    int level;
    Node *parent = nullptr;
    State *currentState;
    vector<Node *> childList;
    string currentMove;

    Node();

    Node(State &);

    bool isParent();
};

Node::Node() {
    level = 0;
    currentState = nullptr;
}

Node::Node(State &key) {
    parent = nullptr;
    currentState = &key;
    level = 0;
}

/*
 * This function is cruel to prevent cycles in the graph
 * For example if farmer makes same move indefinitely then it will be in stuck between nodes
 * so if there is any parent which has same state with current state then it will return true and
 * it will be blocked in graph creation phase.
 */
bool Node::isParent() {
    Node *temp = parent;

    bool answer = false;
    while (temp != nullptr) {
        if (currentState->equal(*temp->currentState)) {
            answer = true;
            break;
        }
        temp = temp->parent;
    }

    return answer;
}

/*
 * This function prints solutions and measurements of the algorithms.
 */
void solutionTrace(Node *endNode, bool mode) {
    double time = (tFinish - tStart) / double(CLOCKS_PER_SEC) * 100;
    stack<Node *> solutionStack;
    Node *current = endNode;
    while (current != nullptr) {
        solutionStack.push(current);
        current = current->parent;
    }
    if (mode) {
        cout << "Algorithm: BFS" << endl;
        cout << "Number of the visited nodes: " << numberOfBFSNode << endl;
    } else {
        cout << "Algorithm: DFS" << endl;
        cout << "Number of the visited nodes: " << numberOfDFSNode << endl;
    }
    cout << "Maximum number of nodes kept in the memory: " << solutionStack.size() << endl;
    cout.precision(4);
    cout << "Running time: " << time << " milliseconds" << endl;
    cout << "Solution move count: " << solutionStack.size() - 1 << endl;
    while (!solutionStack.empty()) {
        Node *temp = solutionStack.top();
        solutionStack.pop();

        cout << temp->currentMove;
        temp->currentState->printSides();
        cout << endl;
    }
}


Node *rootOfGraph;  //Global variable for graph

/*
 * This function uses nearly same method with BFS but it is only
 * creates graph nodes.So at the end of the function there will be full graph
 * that contains all states except cycled nodes.Since cycles will create loops
 * i do not add the nodes.
 *
 * Function creates new nodes with every possible moves and adds to the graph structure
 * if any state has already occurred in graph before it is not added to the graph i ensure this
 * with isParent() function
 */

void createGraph() {
    State *initialState = new State();
    initialState->left.insert("Farmer");
    initialState->left.insert("Fox");
    initialState->left.insert("Rabbit");
    initialState->left.insert("Carrot");    //Initial state with all elements on the left side
    rootOfGraph = new Node(*initialState);
    queue<Node *> nodeQueue;
    nodeQueue.push(rootOfGraph);

    while (!nodeQueue.empty()) {
        Node *n = nodeQueue.front();
        nodeQueue.pop();

        for (int i = 0; i < 4; i++) {
            Move *currentMove;
            if (i == 0)
                currentMove = new Move(false, false, false);
            else if (i == 1)
                currentMove = new Move(true, false, false);
            else if (i == 2)
                currentMove = new Move(false, true, false);
            else if (i == 3)
                currentMove = new Move(false, false, true);

            State *s = n->currentState->makeNewMoveState(*currentMove);

            if (!s->nullNode) // Check if it is allowable state
            {
                Node *child = new Node(*s);
                child->parent = n;
                child->level = n->level + 1;
                child->currentMove = currentMove->printBoatMove(child->currentState->farmerSide);

                if (!child->isParent()) {
                    n->childList.push_back(child);
                    if (child->currentState->checkBothSides()) { //If it's allowed state add to the graph
                        nodeQueue.push(child);
                    }
                }
            } else delete s;
            delete[] currentMove;
        }
    }
}


Node *BFS() {
    tStart = clock();
    queue<Node *> nodeQueue; //Queue that will keep the nodes
    set<Node *> visited;      // Keeping visited nodes

    nodeQueue.push(rootOfGraph);    //Pushing the root to the queue
    numberOfBFSNode++;
    while (!nodeQueue.empty()) {
        Node *current = nodeQueue.front();            //Take from queue and pop it
        nodeQueue.pop();

        if (current->currentState->checkSolutionState()) {
            tFinish = clock();
            return current;
        }

        for (int i = 0; i < current->childList.size(); i++) {
            Node *child = current->childList.at(i);      //get the adjacent node


            if (visited.find(child) != visited.end())  //If already visited skip
                continue;

            visited.insert(current);                //Label as visited
            nodeQueue.push(child);                  //Push to the queue
            numberOfBFSNode++;                      //visiting number increase
        }
    }
}

/*
 * This is a iterative depth first search algorithm it's looking very familiar to the lecture slides
 * I used nearly same pseudo code in the lecture slides.
 */

Node *DFS() {
    tStart = clock();
    stack<Node *> nodeStack;    //Stack for dfs
    set<Node *> visited;         //Visited nodes

    nodeStack.push(rootOfGraph);
    numberOfDFSNode++;

    while (!nodeStack.empty()) {
        Node *current = nodeStack.top();
        nodeStack.pop();

        if (current->currentState->checkSolutionState()) { //If solution found
            tFinish = clock();
            return current;
        }

        if (visited.find(current) == visited.end()) { //If not visited
            visited.insert(current);                //Label as visited
            numberOfDFSNode++;

            for (int i = 0; i < current->childList.size(); i++) {
                Node *child = current->childList.at(i);      //get the adjacent node
                nodeStack.push(child);
            }
        }
    }
}

void cleanGraph(Node *current) {
    if (current == nullptr)
        return;

    for (int i = 0; i < current->childList.size(); i++) {
        cleanGraph(current->childList.at(i));      //Clean the child nodes
    }
    delete current->currentState;
    delete current;
}

int main(int argc, char const *argv[]) {

    if (argc != 2) {
        cerr << "You should give the search mode as argument" << '\n';
        return 0;
    }
    string mode;
    mode = argv[1];
    createGraph();      //Generates graph that contains riddle states
    if (mode == "bfs")
        solutionTrace(BFS(), true);
    else if (mode == "dfs")
        solutionTrace(DFS(), false);
    else cerr << "You have given invalid algorithm mode" << endl;

    cleanGraph(rootOfGraph);
    return 0;
}
