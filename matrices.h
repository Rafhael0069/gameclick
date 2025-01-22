#ifndef MATRICES_H
#define MATRICES_H

#include "matrix_control.h"

// Matriz representando uma seta para a esquerda
int left_arrow[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 1, 0, 0},
    {0, 1, 0, 0, 0},
    {1, 1, 1, 1, 1},
    {0, 1, 0, 0, 0},
    {0, 0, 1, 0, 0}
};

// Matriz representando uma seta para a direita
int right_arrow[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 0, 1, 0, 0},
    {0, 0, 0, 1, 0},
    {1, 1, 1, 1, 1},
    {0, 0, 0, 1, 0},
    {0, 0, 1, 0, 0}
};

// Matriz representando um "X"
int x_pattern[MATRIX_SIZE][MATRIX_SIZE] = {
    {1, 0, 0, 0, 1},
    {0, 1, 0, 1, 0},
    {0, 0, 1, 0, 0},
    {0, 1, 0, 1, 0},
    {1, 0, 0, 0, 1}
};

// Matrizes representando os números 3, 2 e 1 para a contagem regressiva
int number_3[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 1, 0}
};

int number_2[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 1, 1, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 1, 1, 1, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 1, 1, 0}
};

int number_1[MATRIX_SIZE][MATRIX_SIZE] = {
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0},
    {0, 0, 0, 1, 0},
    {0, 1, 0, 0, 0}
};

#endif // MATRICES_H