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

    void createTree();
    void createHueristicTable();
    void createInitHueristicTable();
    void createManhattanHueristicTable(array<int, TABLE_SIZE>&, const array<int, 2>&);
    void createObstacleHueristicTable(array<int, TABLE_SIZE>&, const int);
    int min_neighbors_plus(array<int, TABLE_SIZE>&, array<int, TABLE_SIZE>&, int);
    void printSummary(Node* node);

    int algorithm;

private:
    Node* newNode(Node*, int, int, int);
    Node* newNode(Node*, int, int, int, int);
    Node* newNodeAndPushToArrayTree(Node*, Node*, int , int , int);

    queue<Node*> fqueue;
    stack<Node*> fstack;
    priority_queue<Node*, vector<Node*>, Cmp> pqueue;

	Node* root;
    vector<int> priority;
    array<array<int, TABLE_SIZE>, MASK_NUM> heuristic_table;

    int node_id;
    double time_usage;
};

#endif
