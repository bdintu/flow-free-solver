#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <queue>

#define MASK_NUM 5
#define TABLE_SIZE 5*5
#define PROP_OF_EVENT 20

using namespace std;

struct Node{
	int table[TABLE_SIZE];
	Node* next[PROP_OF_EVENT];
};


class FlowFree{
public:
	FlowFree();
	~FlowFree();

    void readFile(const char* path);
    void selectMask();
    void createTree();

private:
	void newTree();
	void deleteTree();

	Node* root;
    queue<Node*> q;
};


FlowFree::FlowFree(){
    newTree();
}

FlowFree::~FlowFree(){
	deleteTree();
}

void FlowFree::newTree(){
	this->root = NULL;
}

void FlowFree::deleteTree(){
}


void FlowFree::readFile(const char* path) {
    Node* node = new Node;
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

    this->root = node;
}

bool canwalkSort(const vector<int>& v1, const vector<int>& v2) { 
    return v1[2] > v2[2]; 
} 

void FlowFree::selectMask() {
    vector<vector<int>> canwalk(2*MASK_NUM, vector<int>(3, 0));
    auto canwalk_itr = canwalk.begin();

    Node* cur = root;

    clog << endl << "# select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++) {
        if (cur->table[i] != 0) {
            (*canwalk_itr) = {i, cur->table[i], 0};

            if (i%MASK_NUM !=0 && cur->table[i-1] == 0)
                (*canwalk_itr)[2]++;

            if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=0 && cur->table[i-5] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+5] == 0)
                (*canwalk_itr)[2]++;

            clog << "index: " << (*canwalk_itr)[0]
                << ", mask: " << (*canwalk_itr)[1]
                << ", canwalk: " << (*canwalk_itr)[2]
                << endl;

            advance(canwalk_itr, 1);
        }
    }

    sort(canwalk.begin(), canwalk.end(), canwalkSort);

    clog << endl << "# select mask after sorted" << endl;
    for (auto& canwalk_itr: canwalk)
        clog << "index: " << canwalk_itr[0]
            << ", mask: " << canwalk_itr[1]
            << ", canwalk: " << canwalk_itr[2]
            << endl;

    vector<int> mask_duplicate(5);

    for (auto& canwalk_itr: canwalk) {
        if (find(mask_duplicate.begin(), mask_duplicate.end(), canwalk_itr[1])  == mask_duplicate.end()) {
            mask_duplicate.push_back(canwalk_itr[1]);

            cur->table[canwalk_itr[0]] = canwalk_itr[1] + MASK_NUM;
        }
    }

    clog << endl << "# table after select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++)
        clog << "index: " << i << " mask: " << cur->table[i] << endl;
}


void FlowFree::createTree() {

}


int main(int argc, char** argv) {
    if (!argc) {
        cerr << "no find arg";
        return 0;
    }

    FlowFree* flowfree = new FlowFree();
    flowfree->readFile(argv[1]);
    flowfree->selectMask();
    flowfree->createTree();

    return 0;
}
