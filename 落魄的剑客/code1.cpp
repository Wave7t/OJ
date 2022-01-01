#include <cstdio>
#include <cstdlib>

#define OJ_H

#define FOR_RANGE(start, end, ind) for(int ind = start; ind < end; ++ind)
#define INF -2147483648
inline int MAX(int i, int j) {
    return i > j ? i : j;
}
inline int MIN(int i, int j) {
    return i > j ? j : i;
}


FILE* g_file_in;
int N;
int* V;
int* B;
int iter_cnt = 1;

void init() {
    fscanf(g_file_in, "%d", &N);
    V = new int[N];
    B = new int[N];

    int temp;
    FOR_RANGE(0, N, i) {
        fscanf(g_file_in, "%d", &temp);
        V[i] = temp;
        B[i] = 0;
    }
}

void destroy() {
    delete[] V;
    delete[] B;
}

void start_1() {
    B[iter_cnt - 1] = 0;
    FOR_RANGE(0, iter_cnt, i) {
        B[iter_cnt - 1] += V[i];
    }

    FOR_RANGE(iter_cnt, N, i) {
        if(B[i - 1] > 0) {
            B[i] = B[i - 1] + V[i];
        }
        else {
            B[i] = V[i];
        }
    }

    ++iter_cnt;
}

void clear() {
    iter_cnt = 1;
    FOR_RANGE(0, N, i) {
        B[i] = 0;
    }
}

void start_2() {
    B[0] = V[0];
    FOR_RANGE(1, N, i) {
        B[i] = B[i - 1] + V[i];
    }

    ++iter_cnt;
}



void iter() {
    int max_temp = B[iter_cnt - 1];
    if(B[iter_cnt - 2] > B[iter_cnt - 1]) {
        max_temp = B[iter_cnt - 2];
    }

    B[iter_cnt - 1] = 0;
    FOR_RANGE(0, iter_cnt, i) {
        B[iter_cnt - 1] += V[i];
    }

    int B_buf = 0;
    FOR_RANGE(iter_cnt, N, i) {
        B_buf = B[i];

        if(B[i - 1] > max_temp) {
            B[i] = B[i - 1] + V[i];
        }
        else {
            B[i] = max_temp + V[i];
        }

        if(B_buf > max_temp) {
            max_temp = B_buf;
        }
    }

    ++iter_cnt;
}


int solve() {
    int max = INF;

    start_1();
    FOR_RANGE(0, 2, i)iter();
    FOR_RANGE(2, N, i) {
        if(B[i] > max)max = B[i];
    }

    clear();
    start_2();
    FOR_RANGE(0, 3, i)iter();
    if(B[N - 1] > max) {
        max = B[N - 1];
    }

    return max;
}

int main() {
#ifdef OJ_H
    g_file_in = stdin;
#else
    g_file_in = fopen("test1.txt", "r");
#endif

    init();
    printf("%d", solve());
    destroy();

#ifndef OJ_H
    fclose(g_file_in);
#endif
    return 0;
}
