#include <array>
#include <numeric>
#include <algorithm>

#include "config.h"

using namespace std;


void create_hueristic_table(array<int, TABLE_SIZE>& heuristic_table, const array<int, 2>& r) {

    for (int i=0; i<TABLE_SIZE; ++i) {
        array<int, 2> pos = {i/MASK_NUM, i%MASK_NUM};
        transform(r.begin(), r.end(), pos.begin(), pos.begin(), std::minus<int>());

        pos[0] = abs(pos[0]);
        pos[1] = abs(pos[1]);

        heuristic_table[i] = accumulate(pos.begin(), pos.end(), 0);
    }
}
