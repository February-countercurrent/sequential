#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define num_row 2048
#define num_col 2048

struct Matrix
{
    int row;
    int column;
    double** m; 
};

void Matrix_init0(struct Matrix *matrix);
void Matrix_init(struct Matrix *matrix);
struct Matrix Matrix_mul(struct Matrix *m1, struct Matrix *m2);
struct Matrix Matrix_transpose(struct Matrix *matrix);
void Matrix_print(struct Matrix *matrix);
void Matrix_free(struct Matrix *matrix);

void Matrix_init0(struct Matrix *matrix){
    matrix->row = num_row;
    matrix->column = num_col;
    matrix->m = (double**)malloc(matrix->row * sizeof(double*));
    int i, j;
    for(i = 0;i < matrix->row;i++){
        matrix->m[i] = (double*)malloc(matrix->column * sizeof(double*));
    }
    for(i = 0;i < matrix->row;i++){
        for(j = 0;j < matrix->column;j++){
            matrix->m[i][j] = 0;
        }
    }
}

void Matrix_init(struct Matrix *matrix){
    matrix->row = num_row;
    matrix->column = num_col;
    matrix->m = (double**)malloc(matrix->row * sizeof(double*));
    int i, j;
    for(i = 0;i < matrix->row;i++){
        matrix->m[i] = (double*)malloc(matrix->column * sizeof(double*));
    }
    srand(1);
    for(i = 0;i < matrix->row;i++){
        for(j = 0;j < matrix->column;j++){
            matrix->m[i][j] = rand();
        }
    }
}

struct Matrix Matrix_mul(struct Matrix *m1, struct Matrix *m2){
    int i, j, k;
    struct Matrix result;
    Matrix_init0(&result);
    for(i = 0;i < result.row;i++){
        for(j = 0;j < result.column;j++){
            for(k = 0;k < m1->column;k++){
                result.m[i][j] = result.m[i][j] + m1->m[i][k] * m2->m[k][j];
            }
        }
    }
    return result;
}

struct Matrix Matrix_transpose(struct Matrix *matrix){
    struct Matrix trans;
    Matrix_init0(&trans);
    int i, j;
    for(i = 0; i < matrix->row; i++){
        for(j = 0; j < matrix->column; j++){
            trans.m[i][j] = matrix->m[j][i];
        }
    }
    return trans;
}

void Matrix_print(struct Matrix *matrix){
    int i, j;
    for(i = 0;i < matrix->row;i++){
        for(j = 0;j < matrix->column;j++){
            printf(" %lf ", matrix->m[i][j]);
        }
        printf("\n");
    }
}

void Matrix_free(struct Matrix *matrix){
    int i;
    for(i = 0;i < matrix->row;i++){
        free(matrix->m[i]);
        matrix->m[i] = NULL;
    }
    free(matrix->m);
    matrix->m = NULL;
}

int main(int argc, char* argv[]){
    struct Matrix A, B, result;
    clock_t start, end;
    double time;

    Matrix_init(&A);
    Matrix_init(&B);

    start = clock();
    result = Matrix_mul(&A, &B);
    end = clock();
    time = (double)(end - start) / CLOCKS_PER_SEC;
    
    printf("The time is %lf\n", time);

    //Matrix_print(&result);

    double flops, achieved_gflops, theoretical_gflops;
    flops = 2 * (double)result.row * (double)result.column * (double)A.column;
    achieved_gflops = flops/(time * 1e9);
    printf("The actual GFLOP/s is %lf\n", achieved_gflops);
    theoretical_gflops = pow((double)result.row, 4) / (time * 1e9);
    printf("The theoretical peak GFLOP/s is %lf\n", theoretical_gflops);
    printf("The gap equals to %lf\n", fabs(theoretical_gflops - achieved_gflops));

    // Optimization using transpose
    struct Matrix trans;
    clock_t start1, end1;
    double time1;
    trans = Matrix_transpose(&B);
    start1 = clock();
    result = Matrix_mul(&A,&trans);
    end1 = clock();
    time1 = (double)(end1 - start1) / CLOCKS_PER_SEC;
    printf("The time is %lf\n", time1);
    double achieved_gflops1 = flops/(time1 * 1e9);
    printf("The actual GFLOP/S of optimization1 is %lf\n", achieved_gflops1);
    printf("The gap between equals to %lf\n", achieved_gflops1 - achieved_gflops);

    Matrix_free(&A);
    Matrix_free(&B);
    Matrix_free(&result);
    return 0;
}