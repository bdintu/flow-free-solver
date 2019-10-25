#include <iostream>
#include <chrono>

#include "flowfree.h"

typedef std::chrono::high_resolution_clock Clock;

Node* FlowFree::newNodeAndPushToArrayTree(Node* cur, Node* node, int cur_index, int new_index, int node_id) {
    if (this->algorithm != 2) {
        node = this->newNode(cur, cur_index, new_index, node_id);
    } else {
        int heuristic = this->heuristic_table[*cur->mask_itr][new_index];
        node = this->newNode(cur, cur_index, new_index, node_id, heuristic);
    }

    cur->next.push_back(node);

    if (this->algorithm == 0)
        this->fqueue.push(node);
    else if (this->algorithm == 1)
        this->fstack.push(node);
    else
        this->pqueue.push(node);

    return node;
}

void FlowFree::createTree() {

    auto t1 = Clock::now();

    Node* cur = this->root;

    if (this->algorithm == 0)
        this->fqueue.push(this->root);
    else if (this->algorithm == 1)
        this->fstack.push(this->root);
    else
        this->pqueue.push(this->root);

    while(1) {

        if (this->algorithm == 0) {
            if ( this->fqueue.empty() )
                break;

            cur = this->fqueue.front();
            this->fqueue.pop();

        } else if (this->algorithm == 1) {
            if ( this->fstack.empty() )
                break;

            cur = this->fstack.top();
            this->fstack.pop();
        } else {
            if ( this->pqueue.empty() )
                break;

            cur = this->pqueue.top();
            this->pqueue.pop();
        }

        cout << "cur mask: " << *(cur->mask_itr) << endl;
        cout << "cur node:" << endl;
        this->printNode(cur);

        if (!*(cur->mask_itr)) {
            auto t2 = Clock::now();
            this->time_usage = chrono::duration_cast<chrono::microseconds>(t2 - t1).count() / 1000.0;

            this->printSummary(cur);
            break;
        }

        for (int i=0; i<TABLE_SIZE; i++) {
            if (cur->table[i] == *(cur->mask_itr)) {

                if (i%MASK_NUM !=0 && cur->table[i-1] == cur->table[i] + MASK_NUM) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i-1, this->node_id++);
                    this->printNode(node, '<');

                    advance(node->mask_itr, 1);
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == cur->table[i] + MASK_NUM) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i+1, this->node_id++);
                    this->printNode(node, '>');

                    advance(node->mask_itr, 1);
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i-MASK_NUM, this->node_id++);
                    this->printNode(node, '^');

                    advance(node->mask_itr, 1);
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i+MASK_NUM, this->node_id++);
                    this->printNode(node, 'V');

                    advance(node->mask_itr, 1);
                }



                if (i%MASK_NUM !=0 && cur->table[i-1] == 0) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i-1, this->node_id++);
                    this->printNode(node, '<');
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i+1, this->node_id++);
                    this->printNode(node, '>');
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i-MASK_NUM, this->node_id++);
                    this->printNode(node ,'^');
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0) {
                    Node* node;
                    this->newNodeAndPushToArrayTree(cur, node, i, i+MASK_NUM, this->node_id++);
                    this->printNode(node, '^');
                }
            }
        }

        cout << "node depth: " << cur->depth
            << ", node id: " << cur->id
            << ", node child: " << cur->next.size();

        if (this->algorithm == 0)
            cout << ", queue size: " << this->fqueue.size();
        else
            cout << ", stack size: " << this->fstack.size();

        cout << endl;
    }
}
