#include <array>
#include <vector>

#include "config.h"
using namespace std;


struct Node{
    array<int, TABLE_SIZE> table = {0};
    vector<Node*> next;

    vector<int>::iterator mask_itr;
    int heuristic;
    int cons;

    int depth = 0;
    int id = 0;
};

struct Cmp {
    bool operator ()(const Node* lhs, const Node* rhs) const {
        return lhs->heuristic <= rhs->heuristic;
    }
};
