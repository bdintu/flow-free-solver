#include <iostream>

#include "flowfree.cc"
#include "select_mask.cc"
#include "create_tree.cc"
#include "create_heuristic_table.cc"

using namespace std;


int main(int argc, char** argv) {
    if (!argc) {
        cerr << "no find arg";
        return 0;
    }

    FlowFree* flowfree = new FlowFree();
    flowfree->readFile(argv[1]);
    flowfree->setAlgorithm(argv[2]);
    flowfree->selectMask();

    if (flowfree->algorithm == 2) {
        flowfree->createHueristicTable();
    }

    flowfree->createTree();

    return 0;
}
