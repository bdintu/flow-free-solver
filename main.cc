#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define MASK_NUM 5
#define TABLE_SIZE 5*5
#define PROP_OF_EVENT 20

using namespace std;

struct Node{
	int table[TABLE_SIZE];
	Node *next[PROP_OF_EVENT];
};


class FlowFree{
public:
	FlowFree();
	~FlowFree();

    void readFile(const char* path);
    void selectMask();

private:
	void createTree();
	void destroyTree();

	Node *root;
};


FlowFree::FlowFree(){
    createTree();
}

FlowFree::~FlowFree(){
	destroyTree();
}

void FlowFree::createTree(){
	root = NULL;
}

void FlowFree::destroyTree(){
}


void FlowFree::readFile(const char* path) {
    Node *node = new Node;
    for(int i=0; i<PROP_OF_EVENT; i++)
        node->next[i] = NULL;

    ifstream file(path) ;
    if (file) {
        clog << endl << "# read file " << path << endl;
        for (int i=0; i<TABLE_SIZE; i++) {
            file >> node->table[i];
            clog << "index: " << i << " mask: " << node->table[i] << endl;
        }
    } else {
        cerr << "failed to open file " << path << endl;
    }

    root = node;
}

bool canwalkSort(const vector<int>& v1, const vector<int>& v2) { 
    return v1[2] > v2[2]; 
} 

void FlowFree::selectMask() {
    vector<vector<int>> canwalk(2*MASK_NUM, vector<int>(3, 0));
    auto it = canwalk.begin();

    clog << endl << "# select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++) {
        if (root->table[i] != 0) {
            (*it) = {i, root->table[i], 0};

            if (i%MASK_NUM !=0 && root->table[i-1] == 0)
                (*it)[2]++;

            if (i%MASK_NUM !=MASK_NUM-1 && root->table[i+1] == 0)
                (*it)[2]++;

            if (i/MASK_NUM !=0 && root->table[i-5] == 0)
                (*it)[2]++;

            if (i/MASK_NUM !=MASK_NUM-1 && root->table[i+5] == 0)
                (*it)[2]++;

            clog << "index: " << (*it)[0]
                << ", mask: " << (*it)[1]
                << ", canwalk: " << (*it)[2]
                << endl;

            advance(it, 1);
        }
    }

    sort(canwalk.begin(), canwalk.end(), canwalkSort);

    clog << endl << "# select mask after sorted" << endl;
    for (auto& it: canwalk)
        clog << "index: " << it[0]
            << ", mask: " << it[1]
            << ", canwalk: " << it[2]
            << endl;



    vector<int> mask_duplicate(5);

    for (auto& it: canwalk) {
        if (find(mask_duplicate.begin(), mask_duplicate.end(), it[1])  == mask_duplicate.end()) {
            mask_duplicate.push_back(it[1]);

            root->table[it[0]] = it[1] + MASK_NUM;
        }
    }

    clog << endl << "# table after select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++)
        clog << "index: " << i << " mask: " << root->table[i] << endl;
}


int main(int argc, char** argv) {
    if (!argc) {
        cerr << "no find arg";
        return 0;
    }

    FlowFree *flowfree = new FlowFree();
    flowfree->readFile(argv[1]);
    flowfree->selectMask();

    return 0;
}
