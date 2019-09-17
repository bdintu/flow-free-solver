#include <iostream>

#include "flowfree.cc"
#include "select_mask.cc"
#include "create_tree.cc"

using namespace std;


/*
 * argv[1]: path of matrix
 * argv[2]: 0 -> DFS
 *          1 -> BFS
*/
int main(int argc, char** argv) {
    if (!argc) {
        cerr << "no find arg";
        return 0;
    }

    FlowFree* flowfree = new FlowFree();
    flowfree->readFile(argv[1]);
    flowfree->selectMask();
    flowfree->setAlgorithm(argv[2]);
    flowfree->createTree();

    return 0;
}
