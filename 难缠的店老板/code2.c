#include <stdio.h>
#include <stdlib.h>

#define MAX_INPUT_LENGTH 200000
#define FOR_RANGE(_ind, _lim) for(int _ind = 0; _ind < _lim; ++_ind)

/* each of these stored numbers hold a index that points to its PREV or NEXT */
/* an IMAGINARY linked list */
#define head_  0
#define rear_  MAX_INPUT_LENGTH
int prev_[MAX_INPUT_LENGTH + 1];
int next_[MAX_INPUT_LENGTH + 1];

int* prev = prev_;
int* next = next_;
int head = head_;
int rear = rear_;

void setNull(int* vec, int len) {
    for(int i = 0; i < len; ++i) {
        // "-1" is not a legal index, thus used to represent "NULL"
        vec[i] = -1;
    }
}// setNull

int defaultMain() {
    // initialize
    setNull(prev, MAX_INPUT_LENGTH + 1);
    setNull(next, MAX_INPUT_LENGTH + 1);
    next[head] = rear;
    prev[rear] = head;
    // read input geometry info
    int N, M, K, L;
    scanf("%d%d%d%d", &N, &M, &K, &L);
    if(N == 0)exit(EXIT_FAILURE);

    int flag = 0, index = 0, target = 0, parity = -1;
    FOR_RANGE(i, N + M + K + L) {
        // identify the type of this input
        scanf("%d", &flag);
        if(flag == 1) {
            scanf("%d%d%d", &index, &target, &parity);
            // identify the first insert
            if(next[head] == rear) {
                next[head] = index;
                prev[rear] = index;
                next[index] = rear;
                prev[index] = head;
            }// first insertion
            else {
                if(parity == 0) {
                    // left side
                    int temp = prev[target];
                    prev[target] = index;
                    next[temp] = index;
                    prev[index] = temp;
                    next[index] = target;
                }
                else if(parity == 1) {
                    // right side
                    int temp = next[target];
                    next[target] = index;
                    prev[temp] = index;
                    prev[index] = target;
                    next[index] = temp;
                }
            }// later insertions
        }// 1
        else if(flag == 0) {
            scanf("%d", &index);
            next[prev[index]] = next[index];
            prev[next[index]] = prev[index];
            prev[index] = -1;
            next[index] = -1;
        }// 0
        else if(flag == -1) {
            scanf("%d%d", &index, &target);
            int prevA = prev[index];
            int nextA = next[index];
            int prevB = prev[target];
            int nextB = next[target];

            // the two numbers to exchange can be neighbours, so be careful
            // I admmit this is not an elegant way of dealing with it...
            if(next[index] == target) {
                next[prevA] = target;
                prev[target] = prevA;
                next[target] = index;
                prev[index] = target;
                next[index] = nextB;
                prev[nextB] = index;
            }
            else if(prev[index] == target) {
                next[prevB] = index;
                prev[index] = prevB;
                next[index] = target;
                prev[target] = index;
                next[target] = nextA;
                prev[nextA] = target;
            }
            else {
                next[index] = nextB;
                next[target] = nextA;
                prev[index] = prevB;
                prev[target] = prevA;

                next[prev[index]] = index;
                prev[next[index]] = index;
                prev[next[target]] = target;
                next[prev[target]] = target;
            }
        }// -1
        else if(flag == -2) {
            // reverse: a simple trick, exchange the pointers instead of actual data. 
            // Make the fullest use of the built-in symmetry
            int* temp = prev;
            prev = next;
            next = temp;
            int temp_ = head;
            head = rear;
            rear = temp_;
        }// -2
    }// line cnt

    // time for output
    int cnt = next[head];
    while(cnt != rear) {
        printf("%d ", cnt);
        cnt = next[cnt];
    }
    printf("-1");
    return 0;
}// defautlMain

int main() {
    return defaultMain();
}
