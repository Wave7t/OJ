/* An improved version and wish it'll be faster */
#include <cstdlib>
#include <cstdio>

#define OJ_H
// #define DEBUG_H
FILE* g_file_in;
typedef unsigned char BYTE;

int len;
BYTE** U;
BYTE* b;
BYTE* x;

void print_vec(BYTE* vec) {
    for(int i = 0; i < len; ++i) {
        putchar('0' + vec[i]);
    }
    putchar('\n');
}

void substract(int line1, int line2, int start_cnt) {
    for(int c = start_cnt; c < len; ++c) {
        U[line2][c] ^= U[line1][c];
    }
    b[line2] ^= b[line1];
}

void exchange_line(int ind1, int ind2) {
    BYTE* temp = U[ind1];
    U[ind1] = U[ind2];
    U[ind2] = temp;

    BYTE temp1 = b[ind1];
    b[ind1] = b[ind2];
    b[ind2] = temp1;
}

void read_U() {
    int  loc_cnt = 0;
    // x is used as the temp storage
    fscanf(g_file_in, "%s", x);
    for(int i = 0; i < len; ++i) {
        x[i] -= '0';
    }
    for(int i = 0; i < len; ++i) {
        for(int j = 0; j < len; ++j) {
            loc_cnt = j + i;
            if(loc_cnt >= len)loc_cnt -= len;
            U[i][j] = x[loc_cnt];
        }
    }
}

void init() {
    U = new BYTE * [len];
    x = new BYTE[len + 1];
    b = new BYTE[len + 1];
    for(int i = 0; i < len; ++i) {
        U[i] = new BYTE[len + 1];
        x[i] = 0; b[i] = 0;
    }
}

void destroy() {
    for(int i = 0; i < len; ++i) {
        delete[] U[i];
    }
    delete[] U;
    delete[] x;
    delete[] b;
}

bool Gaussian_eliminate() {
    // stage 1
    int row = 0;
    for(int col = 0; col != len; ++col) {
        if(!U[row][col]) {
            // search down
            int j;
            for(j = row + 1; j < len; ++j) {
                if(U[j][col]) {
                    exchange_line(row, j);
                    break;
                }
            }
            if(j == len) continue;
        }
        for(int j = row + 1; j != len; ++j) {
            if(U[j][col])substract(row, j, col);
        }
        ++row;
    }// stage 1

    // see if there is a solution
    for(int i = row; i != len; ++i) {
        if(b[i])return false;
    }

    // stage 2
    BYTE temp = 0;
    for(int i = len - 1; i != -1; --i) {
        temp = b[i];
        for(int j = i + 1; j < len; ++j) {
            temp ^= (x[j] & U[i][j]);
        }
        x[i] = temp;
    }// stage 2

    return true;
}// Gaussian Eliminate

void print_result() {
    for(int i = 0; i < len; ++i) {
        if(x[i])putchar('B');
        putchar('A');
    }
}

void query() {
    fscanf(g_file_in, "%d", &len);
    init();
    read_U();
    fscanf(g_file_in, "%s", b);
    for(int i = 0; i < len; ++i) {
        b[i] -= '0';
    }
    if(Gaussian_eliminate())print_result();
    else putchar('N');
    putchar('\n');
    destroy();
}

int main() {
#ifdef OJ_H
    g_file_in = stdin;
#else// OJ_H
    g_file_in = fopen("test1.txt", "r");
#endif// OJ_H


#ifndef DEBUG_H
    int epoch = 0;
    fscanf(g_file_in, "%d", &epoch);
    for(int cnt = 0; cnt < epoch; ++cnt) query();
#else// DEBUG_H
    // 
#endif// DEBUG_H

    return 0;
}