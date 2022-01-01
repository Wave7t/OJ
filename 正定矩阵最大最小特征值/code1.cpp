/* parameter tuning is a lot of fun */

#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <iomanip>

#define OJ_H
#define SET_ACC_H
// #define DEBUG_H
// #define CNT_H
FILE* g_file_in;
#define L1_DISTANCE

int dim;
double** A;
double** B;
#define FOR_DIM(i) for(int i = 0; i < dim; ++i)
double ABS(double x) {
    return x > 0 ? x : -x;
}

void init() {
    A = new double* [dim];
    B = new double* [dim];
    FOR_DIM(i) {
        A[i] = new double[dim];
        B[i] = new double[dim];
        FOR_DIM(j) {
            fscanf(g_file_in, "%lf", &A[i][j]);
            B[i][j] = 0;
        }
        B[i][i] = 1.0;
    }
}

void print_mat(double** mat) {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << mat[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void print_AB() {
    for(int i = 0; i < dim; ++i) {
        for(int j = 0; j < dim; ++j) {
            std::cout << A[i][j] << "\t";
        }
        std::cout << "\t";
        for(int j = 0; j < dim; ++j) {
            std::cout << B[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

void destroy() {
    for(int i = 0; i < dim; ++i) {
        delete[] A[i];
        delete[] B[i];
    }
    delete A;
    delete B;
}

void vec_scale(double* vec, double scale) {
    FOR_DIM(i) {
        vec[i] *= scale;
    }
}

void multiply(double* y, double* x, double** mat) {
    // y = Ax
    FOR_DIM(i) {
        double temp = 0;
        FOR_DIM(j) {
            temp += mat[i][j] * x[j];
        }
        y[i] = temp;
    }
}

double find_max(double* vec) {
    double ret;
    FOR_DIM(i) {
        if(vec[i] > ret)ret = vec[i];
    }
    return ret;
}

void exchange_line(int i, int j) {
    double* temp = A[i];
    A[i] = A[j];
    A[j] = temp;
    temp = B[i];
    B[i] = B[j];
    B[j] = temp;
}

double find_ratio(double* vec1, double* vec2) {
    double ret = 0;

#ifdef L1_DISTANCE
    FOR_DIM(i) {
        if(vec2[i] != 0)ret += ABS(vec1[i] / vec2[i]);
        else {
            ret += 1e8;
        }
    }
#else

#endif

    return ret / dim;
}

void normalize(double* vec) {
    double max = 0;
    double temp = 0;
    FOR_DIM(i) {
        temp = ABS(vec[i]);
        if(temp > max)max = temp;
    }
    if(max == 0)return;
    max = 1.0 / max;
    FOR_DIM(i) {
        vec[i] *= max;
    }
}

void reverse() {

    int row = 0, col = 0;
    for(; col < dim; ++col) {
        // print_AB();

        if(A[row][col] == 0.0) {
            int cnt_down = row + 1;
            for(; cnt_down < dim; ++cnt_down) {
                if(A[cnt_down][col] != 0.0) {
                    exchange_line(row, cnt_down);
                    break;
                }
            }
            if(cnt_down == dim)continue;
        }

        // scale
        double div = 1 / A[row][col];
        A[row][col] = 1;
        for(int i = col + 1; i < dim; ++i) {
            A[row][i] *= div;
        }
        for(int i = 0; i < dim; ++i) {
            B[row][i] *= div;
        }

        // 
        for(int i = row + 1; i < dim; ++i) {
            double scale = A[i][col];
            for(int j = col + 1; j < dim; ++j) {
                A[i][j] -= scale * A[row][j];
            }
            for(int j = 0; j < dim; ++j) {
                B[i][j] -= scale * B[row][j];
            }
        }
        ++row;
    }
    // print_AB();

    for(int i = dim - 1; i != -1; --i) {
        for(int j = i - 1; j != -1; --j) {
            for(int k = 0; k != dim; ++k) {
                B[j][k] -= A[j][i] * B[i][k];
            }
        }
    }

}

double get_max_eigenvalue(double** mat) {
    double* x = new double[dim];
    double* y = new double[dim];
    double* current = x;
    double* next = y;
    FOR_DIM(i)current[i] = 0.005;
    double m1 = 1, m2 = 0;

#ifdef CNT_H
    int cnt = 0;
#endif
    while(ABS(m1 - m2) / m2 > 1e-8) {
#ifdef CNT_H
        ++cnt;
#endif
        multiply(next, current, mat);
        m1 = m2;
        // m2 = find_max(next);
        m2 = find_ratio(next, current);
        // vec_scale(next, 1.0 / m2);
        normalize(next);
        double* temp = current;
        current = next;
        next = temp;
    }
    delete[] x;
    delete[] y;

#ifdef CNT_H
    std::cout << "cnt: " << cnt << std::endl;
#endif
    return m2;
}// get_max_eigenvalue


int main() {
#ifdef OJ_H
    g_file_in = stdin;
#else// OJ_H
    g_file_in = fopen("test1.txt", "r");
#endif// OJ_H

#ifndef DEBUG_H
    fscanf(g_file_in, "%d", &dim);
    init();
    double max = get_max_eigenvalue(A);
    std::cout
#ifdef SET_ACC_H
        << std::setprecision(4)
#endif
        << max << " ";
    reverse();
    // print_mat(B);
    double min = 1.0 / get_max_eigenvalue(B);
    std::cout
#ifdef SET_ACC_H
        << std::setprecision(4)
#endif
        << min << std::endl;

    destroy();
#else// DEBUG_H
    double a = 3.32353;
    double b = 3.32353 / 17.1;
    a *= 2.133;
    a -= 17.1 * b * 2.133;
    std::cout << a << std::endl;
    std::cout << (a == 0) << std::endl;

#endif//DEBUG_H

    return 0;
}// main
