#### 离谱的事情

二叉树的链表存储的确离谱，结构大概是这样的：

```c++
class TwoDTree {
    		 ///////////////////////////////////////////////////////////////////////////////////////////////////////////
// constructed from an array
public:
    TwoDTree(int _size) :size(_size) {}
private:
    int size;// the number of nodes in the tree
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////
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
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the nearest k
public:
    int get_k_near(int targetX, int targetY, int k);
private:
    int targetX, targetY, k;
    void _set_target(int tx, int ty, int _k) { targetX = tx; targetY = ty; k = _k; }
    void _search_k_near_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_near>& _pq);

///////////////////////////////////////////////////////////////////////////////////////////////////////////
// get the farrest k
public:
    int get_k_far(int targetX, int targetY, int k);
private:
    void _search_k_far_r(Node* rt, std::priority_queue<Node*, std::vector<Node*>, nodeptrCmp_far>& _pq);
    inline ll _get_estimate_dist(Node* n);
   ///////////////////////////////////////////////////////////////////////////////////////////////////////////
};// TwoDTree
```

所有的数据都来自于`new`的动态分配，依靠指针来handle。为了支持顺序访问，还加了指针表。这很离谱，但不完全离谱。

另外一种离谱的思想就是：一个算法要用的公共数据，就存放在类的里面：算法是数据和方法的结合，类就是算法。

#### 栈的递归和循环实现

递归本身就是栈。如果不用函数调用栈，就不得不用外部的栈——这个栈应该存储什么信息，很重要。栈本身不会很深，所以多存点东西也无所谓。如果为了省事，只存一个指针，也不是不行。

#### 估值和剪枝

基于树搜索的算法应该有以下的共同点：

- 基于估值函数来决定搜索的优先级——哪个方向有更大的概率找到目标值；
- 基于严格的计算来决定剪枝：尤其是这种搜索算法，不容许有遗漏。

但是，不是所有的算法都要求严格。在搜索的规模大到一定程度的时候，连搜索都只有用估值进行。一个例子就是下棋。像minimax和$\alpha-\beta$剪枝这样的算法只有死路一条。

