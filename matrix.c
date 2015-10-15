#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <malloc.h>

#define MATRIX_SIZE 1000
#define THREADS_COUNT 2

int **matrix1, **matrix2, **result;

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

void generateMatrix(int** matrix) {
    int i, j;
    srand(time(0));
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++) {
            matrix[i][j] = rand() % 10 + 1;
        }
    }
}

void printMatrix(int** matrix) {
    int i, j;
    for (i = 0; i < MATRIX_SIZE; i++) {
        for (j = 0; j < MATRIX_SIZE; j++)
            printf("%d ", matrix[i][j]);
        printf("\n");
    }
}

void allocateMemory(int*** matrix_ptr) {
    int i;
    *matrix_ptr = (int**)malloc(MATRIX_SIZE * sizeof(int*));
    for (i = 0; i < MATRIX_SIZE; i++) {
        (*matrix_ptr)[i] = (int*)malloc(MATRIX_SIZE * sizeof(int));
    }
}

void freeMemory(int** matrix) {
    int i;
    for (i = 0; i < MATRIX_SIZE; i++) {
        free(matrix[i]);
    }
    free(matrix);
}

int main() {
    int i, args[THREADS_COUNT];
    int size = MATRIX_SIZE / THREADS_COUNT;
    pthread_t thID[THREADS_COUNT];
    allocateMemory(&matrix1);
    allocateMemory(&matrix2);
    allocateMemory(&result);
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

/*  printMatrix(matrix1);
    printf("\n");
    printMatrix(matrix2);
    printf("\n");
    printMatrix(result); */
    freeMemory(matrix1);
    freeMemory(matrix2);
    freeMemory(result);
    return 0;
}
