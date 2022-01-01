/* OJ10
 * Time Limit: 1000ms
 * Memory Limit: 40MB
 * Algorithm: depth-first search */

#include <cstdio>
#include <cstdlib>
#include <list>

#define OJ_H

FILE* g_file_in;
typedef unsigned char BYTE;
typedef unsigned short DBYTE;
#define FOR_RANGE(begin, end, cnt) for(int cnt = begin; cnt < end; ++cnt)

#define UP 1
#define LEFT 2
#define DOWN 3
#define RIGHT 0

#define L_BACK 0
#define R_BACK 1
#define BACK 2
#define BACK2 3

#define R 0
#define L 1
#define F 2
#define F2 3

/* element access rule: [ind_x][ind_y] */
DBYTE** bmap;
int Lx, Ly;


struct node {
    node(int _x, int _y, int _d) :x(_x), y(_y), dir(_d) {}
    node() = default;

    unsigned int x : 10;
    unsigned int dir : 3;
    unsigned int y : 10;
};
typedef std::list<node> nl;
node begin, end;

inline void mark(const node& n) {
    bmap[n.x][n.y] |= (1 << (n.dir + 1));
}

inline bool is_marked(const node& n) {
    return bmap[n.x][n.y] & (1 << (n.dir + 1));
}

inline void mark_prev(const node& n, BYTE action) {
    bmap[n.x][n.y] |= ((action) << (8 + (n.dir << 1)));
}

inline BYTE get_prev(const node& n) {
    return (bmap[n.x][n.y] & (3 << (8 + (n.dir << 1)))) >> (8 + (n.dir << 1));
}

inline node trace_back(const node& n, BYTE action) {
    switch(action) {
    case BACK: {
        switch(n.dir) {
        case UP:
            return node(n.x, n.y + 1, UP);
        case DOWN:
            return node(n.x, n.y - 1, DOWN);
        case LEFT:
            return node(n.x + 1, n.y, LEFT);
        case RIGHT:
            return node(n.x - 1, n.y, RIGHT);
        }
    }

    case BACK2: {
        switch(n.dir) {
        case UP:
            return node(n.x, n.y + 2, UP);
        case DOWN:
            return node(n.x, n.y - 2, DOWN);
        case LEFT:
            return node(n.x + 2, n.y, LEFT);
        case RIGHT:
            return node(n.x - 2, n.y, RIGHT);
        }
    }

    case R_BACK: {
        return node(n.x, n.y, (n.dir + 3) & 3);
    }

    case L_BACK: {
        return node(n.x, n.y, (n.dir + 1) & 3);
    }
    }
    return node();
}

inline bool is_accessable(const node& n) {
    return n.x >= 0 && n.x < Lx
        && n.y >= 0 && n.y < Ly
        && !(bmap[n.x][n.y] & 1);
}

inline bool is_destination(const node& n) {
    return n.x == end.x
        && n.y == end.y
        && n.dir == end.dir;
}

inline node _F(const node& n) {
    switch(n.dir) {
    case UP:
        return node(n.x, n.y - 1, UP);
    case DOWN:
        return node(n.x, n.y + 1, DOWN);
    case LEFT:
        return node(n.x - 1, n.y, LEFT);
    case RIGHT:
        return node(n.x + 1, n.y, RIGHT);
    }
    return node();
}

inline node _F2(const node& n) {
    switch(n.dir) {
    case UP:
        return node(n.x, n.y - 2, UP);
    case DOWN:
        return node(n.x, n.y + 2, DOWN);
    case LEFT:
        return node(n.x - 2, n.y, LEFT);
    case RIGHT:
        return node(n.x + 2, n.y, RIGHT);
    }
    return node();
}

inline node _L(const node& n) {
    return node(n.x, n.y, (n.dir + 1) & 3);
}

inline node _R(const node& n) {
    return node(n.x, n.y, (n.dir + 3) & 3);
}



void init() {
    fscanf(g_file_in, "%d%d", &Lx, &Ly);
    bmap = new DBYTE * [Lx];
    FOR_RANGE(0, Lx, i)bmap[i] = new DBYTE[Ly];

    int temp;
    FOR_RANGE(0, Ly, i) {
        FOR_RANGE(0, Lx, j) {
            fscanf(g_file_in, "%d", &temp);
            bmap[j][i] = temp;
        }
    }

    fscanf(g_file_in, "%d", &temp);
    begin.x = temp;
    fscanf(g_file_in, "%d", &temp);
    begin.y = temp;
    fscanf(g_file_in, "%d", &temp);
    begin.dir = temp & 3;

    fscanf(g_file_in, "%d", &temp);
    end.x = temp;
    fscanf(g_file_in, "%d", &temp);
    end.y = temp;
    fscanf(g_file_in, "%d", &temp);
    end.dir = temp & 3;
}


void search() {
    // initialize
    nl l1;
    nl l2;
    nl* cur = &l1;
    nl* nxt = &l2;
    l1.emplace_back(begin);
    int layer = 0;
    node temp[4];

    while(true) {
        ++layer;
        for(auto it = cur->begin(); it != cur->end(); ++it) {
            temp[L_BACK] = _R(*it);
            temp[R_BACK] = _L(*it);
            temp[BACK] = _F(*it);
            temp[BACK2] = _F2(*it);

            FOR_RANGE(0, 2, i) {
                if(!is_marked(temp[i])) {
                    mark(temp[i]);
                    mark_prev(temp[i], i);
                    nxt->emplace_back(temp[i]);
                    if(is_destination(temp[i])) {
                        goto found;
                    }
                }
            }

            if(is_accessable(temp[BACK])) {
                if(!is_marked(temp[BACK])) {
                    mark(temp[BACK]);
                    mark_prev(temp[BACK], BACK);
                    nxt->emplace_back(temp[BACK]);
                    if(is_destination(temp[BACK])) {
                        goto found;
                    }
                }

                if(is_accessable(temp[BACK2]) && !is_marked(temp[BACK2])) {
                    mark(temp[BACK2]);
                    mark_prev(temp[BACK2], BACK2);
                    nxt->emplace_back(temp[BACK2]);
                    if(is_destination(temp[BACK2])) {
                        goto found;
                    }
                }
            }
        }// for each node in the current depth

        // exchange
        nl* temp_nl = cur;
        cur = nxt;
        nxt = temp_nl;
        *nxt = nl();
    }// while the destination is not found
found:
    /* here "layer" represents the number of steps taken */
    BYTE* seq = new BYTE[layer];

    node t = end;
    BYTE action;
    for(int i = layer - 1; i != -1; --i) {
        action = get_prev(t);
        t = trace_back(t, action);
        seq[i] = action;
    }

    printf("%d\n", layer);
    FOR_RANGE(0, layer, i) {
        switch(seq[i]) {
        case F: printf("F\n"); break;
        case F2: printf("F2\n"); break;
        case L: printf("L\n"); break;
        case R: printf("R\n"); break;
        }
    }
    delete[] seq;
}


void destroy() {
    FOR_RANGE(0, Lx, i) {
        delete[] bmap[i];
    }
    delete[] bmap;
}



int main() {
#ifdef OJ_H
    g_file_in = stdin;
#else
    g_file_in = fopen("test2.txt", "r");
#endif

    init();
    search();
    destroy();

    return 0;
}