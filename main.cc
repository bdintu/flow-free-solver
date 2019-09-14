#include <iostream>
#include <fstream>
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
    array<int, TABLE_SIZE> table;
    vector<Node*> next;
    array<bool, 5> is_success;
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
};


FlowFree::FlowFree(){
	this->root = NULL;
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
        cout << node->table[i] << ' ';

        if (i%MASK_NUM==MASK_NUM-1)
            cout << endl;
    } 

    cout << endl;
}


bool canwalkSort(const vector<int>& v1, const vector<int>& v2) { 
    if (v1[2] < v2[2])
        return true;
    if (v1[2] == v2[2])
        return v1[1] < v1[2];

    return false;
} 

void FlowFree::selectMask() {
    vector<vector<int>> canwalk(2*MASK_NUM, vector<int>(3, 0));
    auto canwalk_itr = canwalk.begin();

    Node* cur = this->root;

    clog << endl << "# select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++) {
        if (cur->table[i] != 0) {
            (*canwalk_itr) = {i, cur->table[i], 0};

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


    for (auto& canwalk_itr: canwalk) {
        if (find(this->priority.begin(), this->priority.end(), canwalk_itr[1])  == this->priority.end()) {
            priority.push_back(canwalk_itr[1]);
            cur->table[canwalk_itr[0]] = canwalk_itr[1] + MASK_NUM;
        }
    }

    clog << endl << "# mask priority" << endl;
    for (auto i: this->priority)
        clog << i << ' ';
    clog << endl;

    clog << endl << "# table after select mask" << endl;
    for (int i=0; i<TABLE_SIZE; i++)
        clog << "index: " << i << " mask: " << cur->table[i] << endl;
}


Node* FlowFree::newNode(Node* cur, int cur_index, int new_index, int success_mask) {
    Node* node = new Node;

    node->table = cur->table;
    node->table[new_index] = cur->table[cur_index];
    node->table[cur_index] = cur->table[cur_index] + 2*MASK_NUM;

    node->is_success = cur->is_success;
    if (success_mask)
        node->is_success[success_mask -1] = true;

    return node;
}


void FlowFree::createTreeOneFlow() {

    Node* cur = this->root;
    int depth = 0;

    auto mask_itr = this->priority.begin();

    while ( !all_of(cur->is_success.begin(), cur->is_success.end(), [](bool i){ return i; }) ) {

        cout << "success:";
        for (const auto i: cur->is_success)
            cout << i << ' ';
        cout << endl;

        cout << "cur node:" << endl;
        this->printNode(cur);


        for (int i=0; i<TABLE_SIZE; i++) {
            if (cur->table[i] == *mask_itr) {

                if (i%MASK_NUM !=0 && cur->table[i-1] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i-1, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    advance(mask_itr, 1);
                    clog << "<- !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i+1, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    advance(mask_itr, 1);
                    clog << "-> !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i-MASK_NUM, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    advance(mask_itr, 1);
                    clog << "^ !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i+MASK_NUM, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    advance(mask_itr, 1);
                    clog << "V !!" << endl;
                    this->printNode(node);

                    break;
                }



                if (i%MASK_NUM !=0 && cur->table[i-1] == 0) {
                    Node* node = this->newNode(cur, i, i-1, 0);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "<-" << endl;
                    this->printNode(node);

                    break;
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == 0) {
                    Node* node = this->newNode(cur, i, i+1, 0);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "->" << endl;
                    this->printNode(node);
                    break;
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == 0) {
                    Node* node = this->newNode(cur, i, i-MASK_NUM, 0);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "^" << endl;
                    this->printNode(node);
                    break;
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == 0) {
                    Node* node = this->newNode(cur, i, i+MASK_NUM, 0);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "V" << endl;
                    this->printNode(node);
                    break;
                }
            }
        }

        cout << "depth: " << depth
            << ", child node: " << cur->next.size()
#ifdef STACK_QUEUE
            << ", stack size: " << this->fstack.size()
#else
            << ", queue size: " << this->fqueue.size()
#endif
            << endl;

#ifdef STACK_QUEUE
        cur = this->fstack.top();
        this->fstack.pop();
#else
        cur = this->fqueue.front();
        this->fqueue.pop();
#endif

        depth++;
//        if (depth == 10)
//            break;
    }
}


void FlowFree::createTreeAllFlow() {

    Node* cur = this->root;
    int depth = 0;

    while ( !all_of(cur->is_success.begin(), cur->is_success.end(), [](bool i){ return i; }) ) {

        cout << "success:";
        for (const auto i: cur->is_success)
            cout << i << ' ';
        cout << endl;

        cout << "cur node:" << endl;
        this->printNode(cur);


        for (int i=0; i<TABLE_SIZE; i++) {
            if (cur->table[i] > 0 && cur->table[i] < MASK_NUM +1) {

                if (i%MASK_NUM !=0 && cur->table[i-1] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i-1, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else                    
                    this->fqueue.push(node);
#endif

                    clog << "<- !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i%MASK_NUM !=MASK_NUM-1 && cur->table[i+1] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i+1, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "-> !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=0 && cur->table[i-MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i-MASK_NUM, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "^ !!" << endl;
                    this->printNode(node);

                    break;
                }

                if (i/MASK_NUM !=MASK_NUM-1 && cur->table[i+MASK_NUM] == cur->table[i] + MASK_NUM) {
                    Node* node = this->newNode(cur, i, i+MASK_NUM, cur->table[i]);

                    cur->next.push_back(node);
#ifdef STACK_QUEUE
                    this->fstack.push(node);
#else
                    this->fqueue.push(node);
#endif

                    clog << "V !!" << endl;
                    this->printNode(node);

                    break;
                }



                if (i%MASK_NUM !=0 && cur->table[i-1] == 0) {
                    Node* node = this->newNode(cur, i, i-1, 0);

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
                    Node* node = this->newNode(cur, i, i+1, 0);

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
                    Node* node = this->newNode(cur, i, i-MASK_NUM, 0);

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
                    Node* node = this->newNode(cur, i, i+MASK_NUM, 0);

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

        cout << "depth: " << depth
            << ", child node: " << cur->next.size()
#ifdef STACK_QUEUE
            << ", stack size: " << this->fstack.size()
#else
            << ", queue size: " << this->fqueue.size()
#endif
            << endl;

#ifdef STACK_QUEUE
        cur = this->fstack.top();
        this->fstack.pop();
#else
        cur = this->fqueue.front();
        this->fqueue.pop();
#endif

        depth++;
        //if (depth == 1)
        //    break;
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
//    flowfree->createTreeAllFlow();
    flowfree->createTreeOneFlow();

    return 0;
}
