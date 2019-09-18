#ifndef FLOWFREE_H
#define FLOWFREE_H

#include <vector>
#include <stack>
#include <queue>

#include "config.h"
#include "node.h"


class FlowFree{

public:
	FlowFree();
	~FlowFree();

    void readFile(const char*);
    void printNode(Node* node);
    void selectMask();
    void setAlgorithm(const char*);
    void createTree();
    void printSummary(Node* node);

private:
    Node* newNode(Node*, int, int, int);

    stack<Node*> fstack;
    queue<Node*> fqueue;

	Node* root;
    vector<int> priority;

    int algorithm;
    int node_id;
    double time_usage;
};

#endif
