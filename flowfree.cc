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


void FlowFree::setAlgorithm(const char* algorithm) {
    this->algorithm = atoi(algorithm);

    cout << "select Algorithm";
    if (this->algorithm == 0)
        cout << "Breadth-first search" << endl;
    else
        cout << "Depth-first search" << endl;
}
