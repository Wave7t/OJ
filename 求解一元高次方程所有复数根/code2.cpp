/* 重根判断的水深 */
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <complex>
#include <algorithm>
#include <cmath>
#include <list>
#include <vector>


typedef std::complex<double> cdouble;
typedef std::list<int> dir;
typedef std::vector<std::list<int>> table;
#define EPS             1e-50

#define OJ_H
#define ITER_1          3500
#define ITER_2          3000
#define THRESHOLD       1e-3

FILE* g_file_in;
int N;// order
int N_cur;
#define FOR_N(i) for(int i = 0; i < N_cur; ++i)
double* A;// coefficients
double* B;// coefficients of the derivative: 0 to N_cur - 1
double* A_base;
double* B_base;
cdouble* Z;// approximations: 0 to N_cur - 1
cdouble* C;// approximations: 0 to N_cur - 1
cdouble* cur, * nxt;
int stable_cnt = 0;
int max_order = 0;
table order_table;

struct Root {
    Root() = default;
    void add() { ++occur; }
    void set(cdouble num, int o) {
        n = num;
        occur = o;
    }

    void set_value(cdouble v) {
        n = v;
    }

    void print() {
        printf("%.4lf %.4lf\n", n.real(), n.imag());
    }

    cdouble n;
    int occur;
};
Root* roots;
int rootnum = 0;

inline double ABS(double x) {
    return x > 0 ? x : -x;
}

inline double MOD(cdouble x) {
    return x.imag() * x.imag() + x.real() * x.real();
}

inline double ROUND(double x) {
    double temp = x * 1e4;
    temp = round(temp) * 1e-4;
    if(temp == -0.0)temp = 0;
    return temp;
}

inline bool comp(cdouble a, cdouble b) {
    if(a.real() == b.real()) {
        if(ABS(a.imag()) == ABS(b.imag())) {
            return a.imag() > b.imag();
        }
        return ABS(a.imag()) > ABS(b.imag());
    }
    return a.real() > b.real();
}

inline int MAX(int a, int b) {
    return a > b ? a : b;
}

void init() {
    // read in
    fscanf(g_file_in, "%d", &N);
    N_cur = N;
    A_base = new double[N_cur + 1];
    B_base = new double[N_cur];
    Z = new cdouble[N_cur];
    C = new cdouble[N_cur];
    A = A_base;
    B = B_base;
    for(int i = N_cur; i != -1; --i) { fscanf(g_file_in, "%lf", &A[i]); }

    for(int i = 0; i < N_cur; ++i) {
        B[i] = (i + 1) * A[i + 1];
    }

    // initialize solutions
    double angle_unit = 2 * 3.14159265 / double(N_cur);
    double angle = 0;
    for(int i = 0; i < N_cur; ++i) {
        Z[i] = cdouble(cos(angle) * 100, sin(angle) * 100);
        angle += angle_unit;
    }
    cur = C;
    nxt = Z;
}

void destroy() {
    delete[] A_base;
    delete[] B_base;
    delete[] Z;
    delete[] roots;
    delete[] C;
}

cdouble evauluate(cdouble x) {
    cdouble ret = A[N_cur];
    for(int i = N_cur - 1; i != -1; --i) {
        ret *= x;
        ret += A[i];
    }
    return ret;
}

cdouble evaluate_d(cdouble x) {
    cdouble ret = B[N_cur - 1];
    for(int i = N_cur - 2; i != -1; --i) {
        ret *= x;
        ret += B[i];
    }
    return ret;
}

void derivative() {
    // exchange A and B
    double* temp = A;
    A = B;
    B = temp;
    --N_cur;

    for(int i = 0; i < N_cur; ++i) {
        B[i] = (i + 1) * A[i + 1];
    }
}

void step() {
    // exchange
    cdouble* temp_p = nxt;
    nxt = cur;
    cur = temp_p;

    for(int i = 0; i < N_cur; ++i) {
        nxt[i] = 0;
    }
    cdouble temp;
    for(int i = 0; i < N_cur; ++i) {
        for(int j = i + 1; j < N_cur; ++j) {
            temp = 1.0 / (Z[i] - Z[j] + EPS);
            nxt[i] += temp;
            nxt[j] -= temp;
        }
    }

    for(int i = 0; i < N_cur; ++i) {
        temp = 1.0 / (evaluate_d(cur[i]) / (evauluate(cur[i]) + EPS) - nxt[i] + EPS);
        nxt[i] = cur[i] - temp;
    }
}

void truncate() {
    for(int i = 0; i < N_cur; ++i) {
        nxt[i] = cdouble(ROUND(nxt[i].real()), ROUND(nxt[i].imag()));
    }
}

void solve() {
    for(int i = 0; i < ITER_1; ++i)step();
}


void find_duplicates() {
    sort(nxt, nxt + N_cur, comp);
    roots = new Root[N_cur];
    int ref = 0;
    int rootcnt = 0;
    for(int i = 0; i <= N_cur; ++i) {
        if(i == N_cur || MOD(nxt[i] - nxt[ref]) > THRESHOLD) {
            int order = i - ref;
            roots[rootcnt].set(nxt[ref], order);
            ref = i;
            // renew max-order found
            if(order > max_order) {
                for(int k = max_order; k < order; ++k)order_table.push_back(dir());
                max_order = order;
            }
            order_table[order - 1].emplace_back(rootcnt);
            ++rootcnt;
        }
    }
    rootnum = rootcnt;
}

void newtonian_search() {
    // order-based

    // deal with the ordinaries first
    for(auto it = order_table[0].begin(); it != order_table[0].end(); ++it) {
        cdouble temp = roots[*it].n;
        for(int i = 0; i < ITER_2; ++i) {
            temp = temp - evauluate(temp) / (EPS + evaluate_d(temp));
        }
        roots[*it].set_value(temp);
    }

    // the ACTUAL order should be added by one
    for(int order = 1; order < max_order; ++order) {
        derivative();
        for(auto it = order_table[order].begin(); it != order_table[order].end(); ++it) {
            cdouble temp = roots[*it].n;
            for(int i = 0; i < ITER_2; ++i) {
                temp = temp - evauluate(temp) / (EPS + evaluate_d(temp));
            }
            roots[*it].set_value(temp);
        }
    }
}


void print_final() {
    int a = 0;
    while(a != rootnum) {
        if(a != rootnum - 1 && roots[a].n.real() == roots[a + 1].n.real()) {
            // conjugate
            for(int i = 0; i < MAX(roots[a].occur, roots[a + 1].occur); ++i) {
                if(i < roots[a].occur)roots[a].print();
                if(i < roots[a + 1].occur)roots[a + 1].print();
            }
            a += 2;
        }
        else {
            for(int i = 0; i < roots[a].occur; ++i) {
                roots[a].print();
            }
            a += 1;
        }
    }
}

int main() {

#ifdef OJ_H
    g_file_in = stdin;
#else
    g_file_in = fopen("test2.txt", "r");
#endif

    init();
    solve();
    truncate();
    find_duplicates();
    newtonian_search();
    print_final();
    destroy();

#ifndef OJ_H
    fclose(g_file_in);
#endif
    return 0;
}