#include <array>
#include <numeric>
#include <algorithm>
#include <limits>
#include <stack>

#include "flowfree.h"


void FlowFree::createHueristicTable() {

    this->createInitHueristicTable();

    Node* cur = this->root;
    clog << endl << "# center of heuristic_table" << endl;

    for (int i=0; i<MASK_NUM; ++i) {
        array<int, TABLE_SIZE>::iterator center_itr = find(cur->table.begin(), cur->table.end(), i + MASK_NUM+1);
        if (center_itr != cur->table.end()) {
            int center_index = distance(cur->table.begin(), center_itr);
            array<int, 2> center = {center_index/5, center_index%5};

            createObstacleHueristicTable(this->heuristic_table[i], center_index);

            clog << "center of heuristic_table " << i+1 << " ("<< center[0] << ", " << center[1] << ")" << endl;
            for (int j=0; j<TABLE_SIZE; ++j) {
                clog << this->heuristic_table[i][j] << " ";

                if (j!=0 && j%MASK_NUM == MASK_NUM-1)
                    clog << endl;
            }
        }
    }
}


void FlowFree::createInitHueristicTable() {

    Node* cur = this->root;
    array<int, TABLE_SIZE> init_hueristic_table;

    for (int i=0; i<TABLE_SIZE; ++i)
        if ( cur->table[i] != 0)
            init_hueristic_table[i] = numeric_limits<int>::max();
        else
            init_hueristic_table[i] = 1;

    for (int i=0; i<MASK_NUM; ++i)
        heuristic_table[i] = init_hueristic_table;
}


void FlowFree::createManhattanHueristicTable(array<int, TABLE_SIZE>& heuristic_table, const array<int, 2>& r) {

    for (int i=0; i<TABLE_SIZE; ++i) {
        array<int, 2> pos = {i/MASK_NUM, i%MASK_NUM};
        transform(r.begin(), r.end(), pos.begin(), pos.begin(), std::minus<int>());

        pos[0] = abs(pos[0]);
        pos[1] = abs(pos[1]);

        heuristic_table[i] = accumulate(pos.begin(), pos.end(), 0);
    }
}


int FlowFree::min_neighbors_plus(array<int, TABLE_SIZE>& heuristic_table, array<int, TABLE_SIZE>& visit, int cur) {
    int min = numeric_limits<int>::max();

    if (visit[cur-1] == 1 && cur%MASK_NUM != 0 && heuristic_table[cur-1] < min)
        min = heuristic_table[cur-1];

    if (visit[cur+1] == 1 && cur%MASK_NUM != MASK_NUM-1 && heuristic_table[cur+1] < min)
        min = heuristic_table[cur+1];

    if (visit[cur-MASK_NUM] == 1 && cur/MASK_NUM != 0 && heuristic_table[cur-MASK_NUM] < min)
        min = heuristic_table[cur-MASK_NUM];

    if (visit[cur+MASK_NUM] == 1 && cur/MASK_NUM != MASK_NUM-1 && heuristic_table[cur+MASK_NUM] < min)
        min = heuristic_table[cur+MASK_NUM];

    return min == numeric_limits<int>::max() ? 0 : min;
}


void FlowFree::createObstacleHueristicTable(array<int, TABLE_SIZE>& heuristic_table, const int center_index) {

    heuristic_table[center_index] = 0;

    stack<int> stack;
    stack.push(center_index);

    int cur;
    array<int, TABLE_SIZE> visit = {0};

    while (!stack.empty()) {
        cur = stack.top();
        stack.pop();

        if (cur%MASK_NUM != 0 && heuristic_table[cur-1] != numeric_limits<int>::max() && visit[cur-1] == 0)
            stack.push(cur-1);

        if (cur%MASK_NUM != MASK_NUM-1 && heuristic_table[cur+1] != numeric_limits<int>::max() && visit[cur+1] == 0)
            stack.push(cur+1);

        if (cur/MASK_NUM != 0 && heuristic_table[cur-MASK_NUM] != numeric_limits<int>::max() && visit[cur-MASK_NUM] == 0)
            stack.push(cur-MASK_NUM);

        if (cur/MASK_NUM != MASK_NUM-1 && heuristic_table[cur+MASK_NUM] != numeric_limits<int>::max() && visit[cur+MASK_NUM] == 0)
            stack.push(cur+MASK_NUM);

        if (cur != center_index) 
            heuristic_table[cur] = min_neighbors_plus(heuristic_table, visit, cur) +1;

        visit[cur] = 1;
    }

    heuristic_table[center_index] = numeric_limits<int>::max();

    for (int i=0; i<TABLE_SIZE; ++i)
        if (heuristic_table[i] == numeric_limits<int>::max())
            heuristic_table[i] = 0;
}
