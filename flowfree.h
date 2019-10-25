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
    void printNode(Node* node, char);
    void selectMask();
    void setAlgorithm(const char*);
    void pushToArrayAndTree(Node*, Node*);
    void createTree();
    void createHueristicTable();
    void createHueristicTable1D(array<int, TABLE_SIZE>&, const array<int, 2>&);
    void printSummary(Node* node);

    int algorithm;

private:
    Node* newNode(Node*, int, int, int);

    stack<Node*> fstack;
    queue<Node*> fqueue;
    priority_queue<Node*> pqueue;

	Node* root;
    vector<int> priority;
    array<array<int, TABLE_SIZE>, MASK_NUM> heuristic_table;

    int node_id;
    double time_usage;
};

#endif
