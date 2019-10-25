#include <array>
#include <vector>

#include "config.h"
using namespace std;


struct Node {
    array<int, TABLE_SIZE> table = {0};
    vector<Node*> next;

    vector<int>::iterator mask_itr;
    int estimate;
    int cost_to_come;

    int depth = 0;
    int id = 0;
};
