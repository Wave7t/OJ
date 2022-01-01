#include <cstdio>
#include <cstdlib>
#include <list>
#include <queue>

#define OJ_H
#define INF 2147483640

#define WEIGHT_MARKER  1073741824
#define IS_MARKER_W(weight) ((weight) & WEIGHT_MARKER)
#define MARK_W(weight) (weight) |= WEIGHT_MARKER
#define WEIGHT(weight) ((weight) & (~WEIGHT_MARKER))

int EXPLORE_MARKER = 1073741824;
FILE* g_file_in;

struct Link {
    int ind;
    int edge_ind;
    Link(int _ind, int _edge_ind) :ind(_ind), edge_ind(_edge_ind) {}
};

struct Node {
    std::list<Link> path_prevs;
    std::list<Link> links;

    int ind;
    int dist;

    Node() :dist(INF) {}

    inline bool operator<(Node& _r) {
        if(is_explored())return true;
        if(_r.is_explored())return true;
        return dist < _r.dist;
    }

    inline bool is_explored() {
        return ind & EXPLORE_MARKER;
    }

    inline void mark_explored() {
        ind |= EXPLORE_MARKER;
    }
};

struct Elem {
    int dist;
    int node;

    bool operator<(const Elem& _r)const {
        return dist > _r.dist;
    }

    Elem(int _node) :node(_node), dist(INF) {}
    Elem(int _node, int _dist) :node(_node), dist(_dist) {}
};

class Map {
public:

    Map(int _N, int _M, FILE* f) :nodeNum(_N), edgeNum(_M), file_in(f) {
        nodes = new Node[nodeNum];
        edges = new int[edgeNum];
        for(int i = 0; i < nodeNum; ++i) {
            nodes[i].ind = i;
        }
    }// constructor

    ~Map() {
        delete[] nodes;
        delete[] edges;
    }

    void getLinks() {
        int ind1, ind2;
        for(int i = 0; i < edgeNum; ++i) {
            fscanf(file_in, "%d%d%d", &ind1, &ind2, &edges[i]);
            nodes[ind1].links.emplace_back(Link(ind2, i));
            nodes[ind2].links.emplace_back(Link(ind1, i));
        }
    }// get input

    void query(int start, int end) {
        findMinPath(start, end);
        printPath(end);
        traceBack(end);
        clear();
        EXPLORE_MARKER >>= 1;
        findMinPath(start, end);
        printPath(end);
    }// query

private:
    FILE* file_in;
    int nodeNum;// node num
    int edgeNum;// edge num
    Node* nodes;
    int* edges;

    void clear() {
        for(int i = 0; i < nodeNum; ++i) {
            nodes[i].path_prevs.clear();
            nodes[i].dist = INF;
        }
    }

    void findMinPath(int start, int end) {

        // construct the V set
        std::priority_queue<Elem> V;
        for(int i = 0; i < nodeNum; ++i) {
            if(i != start) V.push(Elem(i));
        }

        nodes[start].dist = 0;
        nodes[start].mark_explored();
        int cur = start;

        for(int cnt = 0; cnt < nodeNum - 1; ++cnt) {
            for(auto it = nodes[cur].links.begin(); it != nodes[cur].links.end(); ++it) {
                if(nodes[it->ind].is_explored() || IS_MARKER_W(edges[it->edge_ind]))continue;

                int newDist = nodes[cur].dist + WEIGHT(edges[it->edge_ind]);
                if(newDist < nodes[it->ind].dist) {
                    V.push(Elem(it->ind, newDist));
                    nodes[it->ind].dist = newDist;
                    nodes[it->ind].path_prevs.clear();
                    nodes[it->ind].path_prevs.emplace_front(Link(cur, it->edge_ind));
                }
                else if(newDist == nodes[it->ind].dist) {
                    nodes[it->ind].path_prevs.emplace_front(Link(cur, it->edge_ind));
                }
            }// for each adjacent node
            cur = V.top().node;
            nodes[cur].mark_explored();
            V.pop();
        }// loop
    }

    void traceBack(int root) {
        for(auto it = nodes[root].path_prevs.begin(); it != nodes[root].path_prevs.end(); ++it) {
            MARK_W(edges[it->edge_ind]);
            traceBack(it->ind);
        }
    }// trace back

    void printPath(int root) {
        // 这一块的写法：path中的每一个节点包含了这条边本身（mark和weight）的信息，
        // 同时包含了“上一个节点”的信息。这样，遍历的顺序就是从末到头。
        printf("start\n");
        _printPath_r(nodes[root].path_prevs.front());
        printf("%d\nend\n%d\n", root, nodes[root].dist);
    }

    void _printPath_r(Link& prev) {
        if(!nodes[prev.ind].path_prevs.empty())
            _printPath_r(nodes[prev.ind].path_prevs.front());
        printf("%d\n", prev.ind);
    }
};


int main() {
#ifdef OJ_H
    g_file_in = stdin;
#else// OJ_H
    g_file_in = fopen("data2.txt", "r");
#endif// OJ_H
    int N, M;
    fscanf(g_file_in, "%d%d", &N, &M);
    Map m(N, M, g_file_in);
    m.getLinks();
    int S, D;
    fscanf(g_file_in, "%d%d", &S, &D);
    m.query(S, D);
#ifndef OJ_H
    fclose(g_file_in);
#endif// OJ_H
    return 0;
}