/* LambdaOJ 5
 * An implementaion using dynamic programming
 * The code is written in pure C */

#include <stdio.h>
#include <malloc.h>

#define FOR_RANGE(ind, lim) for(int ind = 0; ind < lim; ++ind)

int max_orz(int a, int b) {
    if(a > b)return a;
    return b;
}

int main() {
    // *****************************************************************
    int N; scanf("%d", &N);/* Number of stones */
    int* W = (int*)malloc(sizeof(int) * N);/* stone weights */
    FOR_RANGE(i, N)scanf("%d", &W[i]);
    int* this_layer = NULL, * next_layer = NULL;
    // *****************************************************************
    // layer 0: length n; layer k: length n - k
    this_layer = (int*)malloc(sizeof(int) * N);
    FOR_RANGE(i, N)this_layer[i] = 0;
    for(int i = 0; i < N - 1; ++i) {
        // length of an individual subsequence in next_layer
        int nxtsubsqclen = i + 2;
        // move one step up
        next_layer = (int*)malloc(sizeof(int) * (N - 1 - i));
        // the action depends on the parity of the index of this layer
        if(i % 2 == 0) {
            for(int j = 0; j < N - 1 - i; ++j) {
                next_layer[j] = max_orz(this_layer[j + 1] + W[j], this_layer[j] + W[j + nxtsubsqclen - 1]);
            }// loop over next_layer
        }
        else {
            for(int j = 0; j < N - 1 - i; ++j) {
                next_layer[j] = max_orz(this_layer[j], this_layer[j + 1]);
            }// loop over next_layer
        }
        free(this_layer);
        this_layer = next_layer;
    }// layer counter
    // *****************************************************************
    // next_layer has just one element, which is the anwser to the problem
    printf("%d\n", next_layer[0]);
    // *****************************************************************
    // garbage collection
    free(next_layer);
    free(W);
    return 0;
    // *****************************************************************
}// main