#include <stdio.h>
#include <stdlib.h>

float** Matriz_dinamica_float(int nfils, int ncols);
void Libera_matriz_dinamica_float (float **matriz, int nfils);
int** Matriz_dinamica_int(int nfils, int ncols);
void Libera_matriz_dinamica_int (int **matriz, int nfils);
float* Vector_dinamica_float(int nfils);
void Libera_vector_dinamica_float (float *vector);
int* Vector_dinamica_int(int nfils);
void Libera_vector_dinamica_int (int *vector);
float** MatrixInt2Float(int num_nodos, int **matrix);
float* VectorInt2Float(int num_nodos, int *vector);
