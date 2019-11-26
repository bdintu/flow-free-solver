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

            createObstacleHueristicTable(this->heuristic_table[i], center);

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
            init_hueristic_table[i] = 0;

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


void FlowFree::createObstacleHueristicTable(array<int, TABLE_SIZE>& heuristic_table, const array<int, 2>& r) {

    bool is_first = true;

    for (int i=0; i<TABLE_SIZE; ++i) {
        array<int, 2> pos = {i/MASK_NUM, i%MASK_NUM};
        transform(r.begin(), r.end(), pos.begin(), pos.begin(), std::minus<int>());

        pos[0] = abs(pos[0]);
        pos[1] = abs(pos[1]);

        heuristic_table[i] = accumulate(pos.begin(), pos.end(), 0);
    }
}
