#include <iostream>
#include <fstream>

#define TABLE_SIZE 5
#define PROP_OF_EVENT 20

using namespace std;

struct Node{
	int table[TABLE_SIZE*TABLE_SIZE];
	Node *next[PROP_OF_EVENT];
};


class FlowFree{
public:
	FlowFree();
	~FlowFree();

    void readFile(const char* path);

private:
	void create_tree();
	void destroy_tree();

	Node *root;
};


FlowFree::FlowFree(){
    create_tree();
}

FlowFree::~FlowFree(){
	destroy_tree();
}

void FlowFree::create_tree(){
	root = NULL;
}

void FlowFree::destroy_tree(){
}


void FlowFree::readFile(const char* path) {
    Node *node = new Node;
    for(int i=0; i<PROP_OF_EVENT; i++) {
        node->next[i] = NULL;
    }

    ifstream file(path) ;
    if (file) {
        int i = 0;
        while(file) {
            file >> node->table[i];
            i++;
        }
    } else {
        std::cerr << "failed to open file " << path << endl;
    }

    root = node;
}

int main(int argc, char** argv) {
    if (!argc) {
        return 0;
    }

    FlowFree *flowfree = new FlowFree();
    flowfree->readFile(argv[1]);

    return 0;
}
