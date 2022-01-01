#include <cstdio>
#include <vector>
#include <queue>
#include <cstdlib>

// #define OJ_H
FILE* file_in;
#define X_LIM 100000000
#define Y_LIM 100000000
typedef long long int ll;

inline ll MAX(ll x, ll y) { return x > y ? x : y; }
inline ll SQUARE(ll x) { return x * x; }

class Node {
public:
    int x;// x coordinate
    int y;// y coordinate
    int ind;// index of this point
    int direction;// '0' means that the seperation line is facing the x direction, and '1' for facing y direction
    ll d_sqr;// distance
    Node* lchild;
    Node* rchild;
    int range_x[2];
    int range_y[2];

    void set_d(int x_tar, int y_tar) {
        ll temp1 = x_tar - x,
            temp2 = y_tar - y;
        d_sqr = temp1 * temp1 + temp2 * temp2;
    }
};// Node

struct nodeptrCmp_near {
    bool operator()(Node* n1, Node* n2) {
        return (n1->d_sqr < n2->d_sqr) || ((n1->d_sqr == n2->d_sqr) && n1->ind > n2->ind);
    }
};
nodeptrCmp_near CMP;

struct nodeptrCmp_far {
    bool operator()(Node* n1, Node* n2) {
        return (n1->d_sqr > n2->d_sqr) || ((n1->d_sqr == n2->d_sqr) && n1->ind < n2->ind);
    }
};
nodeptrCmp_far CMP1;

class TwoDTree {
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // constructed from an array
public:
    TwoDTree(int _size) :size(_size) {}
private:
    int size;// the number of nodes in the tree


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // construct the tree
public:
    void buildTree();
private:
    // recursive function
    Node* _build_tree_r(int parity, int start, int end);
    void _init_range_r(Node* _rt);
    // partition algorithms
    void _rand_select(int start, int end, int parity, int targe);
    int _rand_partition(int start, int end, int parity);
    // the root node that starts the tree
    Node* root;
    // temp data
    int* x_data;
    int* y_data;
    int* access_order;


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // get the nearest k
public:
    int get_k_near(int targetX, int targetY, int k);
private:
    int targetX, targetY, k;
    void _set_target(int tx, int ty, int _k) { targetX = tx; targetY = ty; k = _k; }
    void _search_k_near_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_near>& _pq);


    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // get the farrest k
public:
    int get_k_far(int targetX, int targetY, int k);
private:
    void _search_k_far_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_far>& _pq);
    inline ll _get_estimate_dist(Node* n);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////
};// TwoDTree

void TwoDTree::buildTree() {
    x_data = new int[size];
    y_data = new int[size];
    access_order = new int[size];

    for(int i = 0; i < size; ++i) {
        // read coordinate data: x & y
        fscanf(file_in, "%d%d", &(x_data[i]), &(y_data[i]));
        access_order[i] = i;
    }
    root = _build_tree_r(0, 0, size);
    root->range_x[0] = 0;
    root->range_x[1] = X_LIM;
    root->range_y[0] = 0;
    root->range_y[1] = Y_LIM;
    _init_range_r(root);

    delete[] x_data;
    delete[] y_data;
    delete[] access_order;
}// build tree


Node* TwoDTree::_build_tree_r(int parity, int start, int end) {
    // deal with trivial case
    if(start == end) {
        return nullptr;
    }
    Node* ret = new Node();
    int mid = (start + end) / 2;
    _rand_select(start, end, parity, mid);

    ret->x = x_data[access_order[mid]];
    ret->y = y_data[access_order[mid]];
    ret->ind = access_order[mid];
    ret->direction = parity;
    ret->lchild = _build_tree_r(!parity, start, mid);
    ret->rchild = _build_tree_r(!parity, mid + 1, end);
    return ret;
}


void TwoDTree::_init_range_r(Node* _rt) {
    if(_rt->direction) {
        // y
        if(_rt->lchild != nullptr) {
            // lower
            _rt->lchild->range_x[0] = _rt->range_x[0];
            _rt->lchild->range_x[1] = _rt->range_x[1];
            _rt->lchild->range_y[0] = _rt->range_y[0];
            _rt->lchild->range_y[1] = _rt->y;
            _init_range_r(_rt->lchild);
        }
        if(_rt->rchild != nullptr) {
            // upper
            _rt->rchild->range_x[0] = _rt->range_x[0];
            _rt->rchild->range_x[1] = _rt->range_x[1];
            _rt->rchild->range_y[1] = _rt->range_y[1];
            _rt->rchild->range_y[0] = _rt->y;
            _init_range_r(_rt->rchild);
        }
    }
    else {
        // x
        if(_rt->lchild != nullptr) {
            // left
            _rt->lchild->range_y[0] = _rt->range_y[0];
            _rt->lchild->range_y[1] = _rt->range_y[1];
            _rt->lchild->range_x[0] = _rt->range_x[0];
            _rt->lchild->range_x[1] = _rt->x;
            _init_range_r(_rt->lchild);
        }
        if(_rt->rchild != nullptr) {
            // upper
            _rt->rchild->range_y[0] = _rt->range_y[0];
            _rt->rchild->range_y[1] = _rt->range_y[1];
            _rt->rchild->range_x[1] = _rt->range_x[1];
            _rt->rchild->range_x[0] = _rt->x;
            _init_range_r(_rt->rchild);
        }
    }
}// _init_range_r


void TwoDTree::_rand_select(int start, int end, int parity, int target) {
    int par = _rand_partition(start, end, parity);
    if(target == par) return;
    else if(par > target) {
        _rand_select(start, par, parity, target);
        return;
    }
    _rand_select(par + 1, end, parity, target);
}// _findMidValue 


int TwoDTree::_rand_partition(int start, int end, int parity) {
    int selection = end - 1;
    int end_val = access_order[selection];
    access_order[selection] = access_order[end - 1];
    access_order[end - 1] = end_val;

    int* target = parity == 0 ? x_data : y_data;

    int ret_cnt = start;
    int temp;
    for(int cnt = start; cnt < end - 1; ++cnt) {
        if(target[access_order[cnt]] < target[end_val]) {
            temp = access_order[cnt];
            access_order[cnt] = access_order[ret_cnt];
            access_order[ret_cnt] = temp;
            ++ret_cnt;
        }
    }
    access_order[end - 1] = access_order[ret_cnt];
    access_order[ret_cnt] = end_val;
    return ret_cnt;
}// _rand_partition

int TwoDTree::get_k_near(int targetX, int targetY, int k) {
    _set_target(targetX, targetY, k);
    std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_near> pq;
    _search_k_near_r(root, pq);
    return pq.top()->ind;
}

inline void TwoDTree::_search_k_near_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_near>& _pq) {

    Node* first_search;
    Node* second_search;
    if(rt->direction) {
        if(targetY < rt->y) {
            first_search = rt->lchild;
            second_search = rt->rchild;
        }
        else {
            first_search = rt->rchild;
            second_search = rt->lchild;
        }
    }
    else {
        if(targetX < rt->x) {
            first_search = rt->lchild;
            second_search = rt->rchild;
        }
        else {
            first_search = rt->rchild;
            second_search = rt->lchild;
        }
    }
    if(first_search != nullptr)_search_k_near_r(first_search, _pq);

    rt->set_d(targetX, targetY);
    if(_pq.size() < k || CMP(rt, _pq.top()))_pq.push(rt);
    if(_pq.size() > k)_pq.pop();

    // look at the other direction
    if(second_search == nullptr)return;
    if(_pq.size() < k) {
        _search_k_near_r(second_search, _pq);
        return;
    }
    ll dis = (rt->direction) ? (targetY - rt->y) : (targetX - rt->x);
    dis = dis * dis;
    if(dis <= _pq.top()->d_sqr)_search_k_near_r(second_search, _pq);
}

void TwoDTree::_search_k_far_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_far>& _pq) {

    rt->set_d(targetX, targetY);
    if(_pq.size() < k || CMP1(rt, _pq.top()))_pq.push(rt);
    if(_pq.size() > k) _pq.pop();

    ll esti_dis_l = 0, esti_dis_r = 0;
    if(rt->lchild != nullptr) {
        esti_dis_l = _get_estimate_dist(rt->lchild);
    }
    if(rt->rchild != nullptr) {
        esti_dis_r = _get_estimate_dist(rt->rchild);
    }

    if(esti_dis_l > esti_dis_r) {
        if(rt->lchild != nullptr && esti_dis_l > _pq.top()->d_sqr) _search_k_far_r(rt->lchild, _pq);
        if(rt->rchild != nullptr && esti_dis_r > _pq.top()->d_sqr) _search_k_far_r(rt->rchild, _pq);
    }
    else {
        if(rt->rchild != nullptr && esti_dis_r > _pq.top()->d_sqr) _search_k_far_r(rt->rchild, _pq);
        if(rt->lchild != nullptr && esti_dis_l > _pq.top()->d_sqr) _search_k_far_r(rt->lchild, _pq);
    }
}

inline ll TwoDTree::_get_estimate_dist(Node* n) {
    return MAX(SQUARE(targetX - n->range_x[0]), SQUARE(targetX - n->range_x[1])) +
        MAX(SQUARE(targetY - n->range_y[0]), SQUARE(targetY - n->range_y[1]));
}

int TwoDTree::get_k_far(int targetX, int targetY, int k) {
    _set_target(targetX, targetY, k);
    std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_far> pq;
    _search_k_far_r(root, pq);
    return pq.top()->ind;
}

int main() {
#ifdef OJ_H
    file_in = stdin;
#else
    file_in = fopen("datain2.txt", "r");
#endif
    int M, N;
    fscanf(file_in, "%d%d", &N, &M);

    TwoDTree t(N);
    t.buildTree();

    int targetX, targetY, k;
    for(int i = 0; i < M; ++i) {
        fscanf(file_in, "%d%d%d", &targetX, &targetY, &k);
        printf("%d\n", t.get_k_far(targetX, targetY, k));
        // printf("%d\n", k > N / 2 ? t.get_k_near(targetX, targetY, N + 1 - k) : t.get_k_far(targetX, targetY, k));
    }
    return 0;
}