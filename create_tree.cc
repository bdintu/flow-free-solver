#include <iostream>
#include <chrono>

#include "flowfree.h"

typedef std::chrono::high_resolution_clock Clock;


void FlowFree::createTree() {

    auto t1 = Clock::now();

    Node* cur = this->root;

    if (this->algorithm == 0)
        this->fstack.push(this->root);
    else
        this->fqueue.push(this->root);

    while(1) {

        if (this->algorithm == 0) {
            if ( this->fstack.empty() )
                break;

            cur = this->fstack.top();
            this->fstack.pop();

        } else {
            if ( this->fqueue.empty() )
                break;

            cur = this->fqueue.front();
            this->fqueue.pop();
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
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-1, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "<- Found" << endl;
                    this->printNode(node);
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+1, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "-> Found" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-MASK_NUM, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "^ Found" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+MASK_NUM, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "V Found" << endl;
                    this->printNode(node);
                }



                if (i%MASK_NUM !=0 && cur->table[i-1] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-1, this->node_id);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "<-" << endl;
                    this->printNode(node);
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+1, this->node_id);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "->" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-MASK_NUM, this->node_id);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "^" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+MASK_NUM, this->node_id);
                    cur->next.push_back(node);

                    if (this->algorithm == 0)
                        this->fstack.push(node);
                    else
                        this->fqueue.push(node);

                    clog << "V" << endl;
                    this->printNode(node);
                }
            }
        }

        cout << "node depth: " << cur->depth
            << ", node id: " << cur->id
            << ", node child: " << cur->next.size();

        if (this->algorithm == 0)
            cout << ", stack size: " << this->fstack.size();
        else
            cout << ", queue size: " << this->fqueue.size();

        cout << endl;
    }
}
