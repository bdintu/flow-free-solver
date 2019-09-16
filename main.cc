#include <iostream>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <array>
#include <vector>

#define MASK_NUM 5
#define TABLE_SIZE MASK_NUM * MASK_NUM

/*  define DFS if you choose DFS
 * or define BFS if you choose BFS
*/
//#define DFS j
#define BFS j

#ifdef DFS
#define STACK_QUEUE j
#endif

#ifdef STACK_QUEUE
#include <stack>
#else
#include <queue>
#endif

using namespace std;


struct Node{
    array<int, TABLE_SIZE> table = {0};
    vector<Node*> next;

    vector<int>::iterator mask_itr;
    int depth = 0;
    int id = 0;
};


class FlowFree{
public:
	FlowFree();
	~FlowFree();

    void readFile(const char* path);
    void printNode(Node* node);
    void selectMask();
    void createTreeAllFlow();
    void createTreeOneFlow();

private:
    Node* newNode(Node*, int, int, int);

	Node* root;
#ifdef STACK_QUEUE
    stack<Node*> fstack;
#else
    queue<Node*> fqueue;
#endif
    vector<int> priority;
    int node_id;
};


FlowFree::FlowFree(){
	this->root = NULL;
    this->node_id = 0;
}


FlowFree::~FlowFree(){

}


void FlowFree::readFile(const char* path) {
    Node* node = new Node;

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


void FlowFree::printNode(Node* node) {
    for (int i=0; i<TABLE_SIZE; i++) {
        cout << setw(2) << node->table[i] << ' ';

        if (i%MASK_NUM==MASK_NUM-1)
            cout << endl;
    } 

    cout << endl;
}
 

void FlowFree::selectMask() {
    vector<vector<int>> canwalk(2*MASK_NUM, vector<int>(3));
    vector<vector<int>>::iterator canwalk_itr = canwalk.begin();

    array<int, MASK_NUM> walksum = {0};

    Node* cur = this->root;

    clog << endl << "# select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++) {
        if (cur->table[i] != 0) {
            (*canwalk_itr) = {i, cur->table[i]};

            if (i%MASK_NUM !=0 && cur->table[i-1] == 0)
                (*canwalk_itr)[2]++;

            if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0)
                (*canwalk_itr)[2]++;

            if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0)
                (*canwalk_itr)[2]++;

            clog << "index: " << (*canwalk_itr)[0]
                << ", mask: " << (*canwalk_itr)[1]
                << ", canwalk: " << (*canwalk_itr)[2]
                << endl;

            walksum[(*canwalk_itr)[1] -1] += (*canwalk_itr)[2];
            advance(canwalk_itr, 1);
        }
    }

    clog << endl << "# walk sum" << endl;
    for (const auto i: walksum)
        clog << i << ' ';
    clog << endl;

    sort(canwalk.begin(), canwalk.end(), [walksum](const vector<int>& v1, const vector<int>& v2){
        if (v1[2] == v2[2])
            return walksum[v1[1] -1] < walksum[v2[1] -1];

        return v1[2] < v2[2];
    });

    clog << endl << "# select mask after sorted" << endl;
    for (const auto& canwalk_itr: canwalk)
        clog << "index: " << canwalk_itr[0]
            << ", mask: " << canwalk_itr[1]
            << ", canwalk: " << canwalk_itr[2]
            << endl;


    for (const auto& canwalk_itr: canwalk) {
        if (find(this->priority.begin(), this->priority.end(), canwalk_itr[1])  == this->priority.end()) {
            priority.push_back(canwalk_itr[1]);
            cur->table[canwalk_itr[0]] = canwalk_itr[1] + MASK_NUM;
        }
    }

    cur->mask_itr = this->priority.begin();

    clog << endl << "# mask priority" << endl;
    for (const auto i: this->priority)
        clog << i << ' ';
    clog << endl;

    clog << endl << "# table after select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++)
        clog << "index: " << i << " mask: " << cur->table[i] << endl;
}


Node* FlowFree::newNode(Node* cur, int cur_index, int new_index, int node_id) {
    Node* node = new Node;

    node->table = cur->table;
    node->table[new_index] = cur->table[cur_index];
    node->table[cur_index] = cur->table[cur_index] + 2*MASK_NUM;

//    node->is_success = cur->is_success;
//    if (success_mask)
//        node->is_success[success_mask -1] = true;

    node->mask_itr = cur->mask_itr;
    node->depth = cur->depth +1;
    node->id = node_id;

    return node;
}


void FlowFree::createTreeOneFlow() {

    Node* cur = this->root;

#ifdef STACK_QUEUE
    this->fstack.push(this->root);
    
    while ( !this->fstack.empty() ) {
        cur = this->fstack.top();
        this->fstack.pop();

#else
    this->fqueue.push(this->root);
    
    while ( !this->fqueue.empty() ) {
        cur = this->fqueue.front();
        this->fqueue.pop();

#endif

        cout << "cur mask: " << *(cur->mask_itr) << endl;
        cout << "success:";

        cout << "cur node:" << endl;
        this->printNode(cur);

        if (!*(cur->mask_itr))
            break;

        for (int i=0; i<TABLE_SIZE; i++) {
            if (cur->table[i] == *(cur->mask_itr)) {

                if (i%MASK_NUM !=0 && cur->table[i-1] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-1, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    clog << "<- !! " << *(node->mask_itr) << endl;
                    this->printNode(node);

                    break;
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+1, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    clog << "-> !! " << *(node->mask_itr) << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-MASK_NUM, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    clog << "^ !! " << *(node->mask_itr) << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == cur->table[i] + MASK_NUM) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+MASK_NUM, this->node_id);
                    advance(node->mask_itr, 1);
                    cur->next.push_back(node);

#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    clog << "V !! " << *(node->mask_itr) << endl;
                    this->printNode(node);

                    break;
                }



                if (i%MASK_NUM !=0 && cur->table[i-1] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-1, this->node_id);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "<-" << endl;
                    this->printNode(node);
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+1, this->node_id);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "->" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i-MASK_NUM, this->node_id);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "^" << endl;
                    this->printNode(node);
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0) {
                    this->node_id++;
                    Node* node = this->newNode(cur, i, i+MASK_NUM, this->node_id);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "V" << endl;
                    this->printNode(node);
                }
            }
        }

        cout << "node depth: " << cur->depth
            << ", node id: " << cur->id
            << ", node child: " << cur->next.size()
#ifdef STACK_QUEUE
            << ", stack size: " << this->fstack.size()
#else
            << ", queue size: " << this->fqueue.size()
#endif
            << endl;
    }
}


int main(int argc, char** argv) {
    if (!argc) {
        cerr << "no find arg";
        return 0;
    }

    FlowFree* flowfree = new FlowFree();
    flowfree->readFile(argv[1]);
    flowfree->selectMask();
    flowfree->createTreeOneFlow();

    return 0;
}
