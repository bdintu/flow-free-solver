#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <vector>
#include <stack>
#include <queue>

#include "flowfree.h"


using namespace std;


FlowFree::FlowFree(){
	this->root = NULL;
    this->node_id = 0;
}


FlowFree::~FlowFree(){
}


void FlowFree::readFile(const char* path) {
    Node* node = new Node;

    ifstream file(path) ;
    if (file) {
        clog << endl << "# read file " << path << endl;
        for (int i=0; i<TABLE_SIZE; i++) {
            file >> node->table[i];
            clog << "index: " << i << " mask: " << node->table[i] << endl;
        }
    } else {
        cerr << "failed to open file " << path << endl;
    }

    this->root = node;
}


void FlowFree::printNode(Node* node) {
    for (int i=0; i<TABLE_SIZE; i++) {
        cout << setw(2) << node->table[i] << ' ';

        if (i%MASK_NUM==MASK_NUM-1)
            cout << endl;
    } 

    cout << endl;
}

void FlowFree::printNode(Node* node, char d) {
    cout << d << endl;
    this->printNode(node);

    if (this->algorithm == 2)
        cout << "estimate: " << node->estimate << " = "
            << "cost to come: " << node->cost_to_come << " + "
            << "heuristic: " << node->heuristic << endl;

    for (int i=0; i<32; ++i)
        cout << '-';
    cout << endl;
}


Node* FlowFree::newNode(Node* cur, int cur_index, int new_index, int node_id) {
    Node* node = new Node;

    node->table = cur->table;
    node->table[new_index] = cur->table[cur_index];
    node->table[cur_index] = cur->table[cur_index] + 2*MASK_NUM;

    node->mask_itr = cur->mask_itr;
    node->depth = cur->depth +1;
    node->id = node_id;

    return node;
}


Node* FlowFree::newNode(Node* cur, int cur_index, int new_index, int node_id, int heuristic) {
    Node* node = this->newNode(cur, cur_index, new_index, node_id);

    node->cost_to_come = cur->cost_to_come +1;
    node->heuristic = heuristic;
    node->estimate = node->cost_to_come + heuristic;

    return node;
}


void FlowFree::setAlgorithm(const char* algorithm) {
    this->algorithm = atoi(algorithm);

    cout << endl << "# select algorithm ";
    switch (this->algorithm) {
        case 0 :
            cout << "Breadth-first search" << endl;
        break;

        case 1 :
            cout << "Depth-first search" << endl;
        break;

        case 2 :
            cout << "Heuristic search" << endl;
        break;
    }
}

void FlowFree::printSummary(Node* node) {
    for (int i=0; i<64; i++)
        cout << "-";
    cout << endl;
    for (int i=0; i<28; i++)
        cout << " ";
    cout << "Summary" << endl;
    for (int i=0; i<64; i++)
        cout << "-";
    cout << endl;

    cout << "time usage: "
      << this->time_usage
      << " ms"
      << endl

      << "node created: "
      << this->node_id
      << " node"
      << endl

      << "tree depth: "
      << node->depth
      << " level"
      << endl
      << endl;

    cout << "active node priority:" << endl;
        for (const auto i: this->priority)
            cout << i << ' ';
        cout << endl;

    cout << "Problem:" << endl;
    this->printNode(this->root);

    cout << "Slove:" << endl;
    this->printNode(node);
}
