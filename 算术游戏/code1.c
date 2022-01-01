/* I am trying to use as few lines of code as possible while making the code readeable.
 * If the comments are emmitted, there would be less than 30 lines */

#include <stdio.h>

typedef unsigned int u32;
#define FOR_RANGE(lim, ind) for(int ind = 0;ind < lim; ++ ind)
#define FOR_RANGE_(start, end, ind) for(int ind = start;ind <= end; ++ ind)
#define SET_ZERO(vec, size) FOR_RANGE(size, i){vec[i]=0;}

int main() {
    // get the input number: int is adequate to store the number
    long long int N; scanf("%lld", &N);
    // tackle illegal input (minus)
    if(N < 0) { printf("-1"); return 0; }
    // zero is a special case, and the answer is obviously 10.
    if(N == 0) { printf("10"); return 0; }
    // one is another special case.
    if(N == 1) { printf("1"); return 0; }
    // store the decomposition result in an array
    u32 divider_cnt[11]; SET_ZERO(divider_cnt, 11);
    // decomposite the input number through dividing it through 9 to 2. 
    u32 divider = 9;
    while(1) {
        if(N % divider == 0) {
            ++divider_cnt[divider];
            N /= divider;
        }//residue
        else if(--divider == 1)break;
    }
    // tackle illegal input
    if(N != 1) { printf("-1"); return 0; }
    // print the output result
    FOR_RANGE_(2, 9, i) { FOR_RANGE(divider_cnt[i], j)printf("%d", i); }
    return 0;
}//main