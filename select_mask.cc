#include <iostream>
#include <algorithm>

#include "flowfree.h"


void FlowFree::selectMask() {
    vector<vector<int>> canwalk(2*MASK_NUM, vector<int>(3));
    vector<vector<int>>::iterator canwalk_itr = canwalk.begin();

    array<int, MASK_NUM> walksum = {0};

    Node* cur = this->root;

    clog << endl << "# select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++) {
        if (cur->table[i] != 0) {
            (*canwalk_itr) = {i, cur->table[i]};

            if (i%MASK_NUM !=0 && cur->table[i-1] == 0)
                (*canwalk_itr)[2]++;

            if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0)
                (*canwalk_itr)[2]++;

            clog << "index: " << (*canwalk_itr)[0]
                << ", mask: " << (*canwalk_itr)[1]
                << ", canwalk: " << (*canwalk_itr)[2]
                << endl;

            walksum[(*canwalk_itr)[1] -1] += (*canwalk_itr)[2];
            advance(canwalk_itr, 1);
        }
    }

    clog << endl << "# walk sum" << endl;
    for (const auto i: walksum)
        clog << i << ' ';
    clog << endl;

    sort(canwalk.begin(), canwalk.end(), [walksum](const vector<int>& v1, const vector<int>& v2){
        if (v1[2] == v2[2])
            return walksum[v1[1] -1] < walksum[v2[1] -1];

        return v1[2] < v2[2];
    });

    clog << endl << "# select mask after sorted" << endl;
    for (const auto& canwalk_itr: canwalk)
        clog << "index: " << canwalk_itr[0]
            << ", mask: " << canwalk_itr[1]
            << ", canwalk: " << canwalk_itr[2]
            << endl;


    for (const auto& canwalk_itr: canwalk) {
        if (find(this->priority.begin(), this->priority.end(), canwalk_itr[1])  == this->priority.end()) {
            priority.push_back(canwalk_itr[1]);
            cur->table[canwalk_itr[0]] = canwalk_itr[1] + MASK_NUM;
        }
    }

    cur->mask_itr = this->priority.begin();

    clog << endl << "# mask priority" << endl;
    for (const auto i: this->priority)
        clog << i << ' ';
    clog << endl;

    clog << endl << "# table after select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++)
        clog << "index: " << i << " mask: " << cur->table[i] << endl;

    if (this->algorithm == 2)
        cur->cost_to_come = 0;
}
