#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define MATRIX_SIZE 1000
#define THREADS_COUNT 1

/*
 * FIXIT:
 * Такие большие матрицы надо выделять в куче malloc`ом, а не на стеке.
   Также надо проверить ускорение от многопоточности. Вероятно, вы это проделали с помощью утилиты time.
   Если нет, то как вариант можно воспользоваться системным вызовом clock().
 */

int matrix1[MATRIX_SIZE][MATRIX_SIZE], matrix2[MATRIX_SIZE][MATRIX_SIZE], result[MATRIX_SIZE][MATRIX_SIZE];

void* multMatrix(void* arg) {
    int size = MATRIX_SIZE / THREADS_COUNT;
    int i, j, k;
    int row, end, begin;
    row = *((int*)arg);
    begin = size * row;
    end = size * row + size;
    for (i = begin; i < end; i++)
        for (j = 0; j < MATRIX_SIZE; j++)
            for (k = 0; k < MATRIX_SIZE; k++)
                result[i][j]+= matrix1[i][k] * matrix2[k][j];
}

void generateMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j;
    srand(time(0));
    for (i = 0; i < MATRIX_SIZE; i++)
        for (j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
}

void printMatrix(int matrix[MATRIX_SIZE][MATRIX_SIZE]) {
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

int main() {
    int i, args[THREADS_COUNT];
    int size = MATRIX_SIZE / THREADS_COUNT;
    pthread_t thID[THREADS_COUNT];
    generateMatrix(matrix1);
    sleep(1);
    generateMatrix(matrix2);
    for (i = 0; i < THREADS_COUNT; i++) {
        args[i] = i;
        pthread_create(&(thID[i]), NULL, multMatrix, (void*)(&(args[i])));
    }
    for (i = 0; i < THREADS_COUNT; i++) {
        pthread_join(thID[i], NULL);
    }
    
    /* printMatrix(matrix1);
    printf("\n");
    printMatrix(matrix2);
    printf("\n");
    printMatrix(result); */
    return 0;
}
