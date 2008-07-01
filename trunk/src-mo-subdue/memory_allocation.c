/***
Functions for allocate/free memory.
***/

#include "memory_allocation.h"

//---------------------------------------------------------------------------
// NAME: Matriz_dinamica_float
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Memory reservation.
//---------------------------------------------------------------------------

float** Matriz_dinamica_float(int nfils, int ncols)
{

   float **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(float **)calloc(nfils,sizeof(float *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(float*)calloc(ncols,sizeof(float)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


//---------------------------------------------------------------------------
// NAME: Libera_matriz_dinamica_float
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Memory liberation.
//---------------------------------------------------------------------------

void Libera_matriz_dinamica_float (float **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}



//---------------------------------------------------------------------------
// NAME: Matriz_dinamica_int
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Memory reservation.
//---------------------------------------------------------------------------

int** Matriz_dinamica_int(int nfils, int ncols)
{

   int **matriz;
   int i,j,error=0;
   /*reservo memoria para un vector de nfil punteros a enteros largos*/
   if((matriz=(int **)calloc(nfils,sizeof(int *)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}

   else
   /*para cada componente del vector (puntero a entero largo) reservo memoria para
   un vector de ncols de enteros largos*/
       for(i=0;(i<nfils) && !error;i++)
          if((matriz[i]=(int*)calloc(ncols,sizeof(int)))==NULL)
          {
             printf("Error en la asignacion de memoria\n");
             error=1;
             for(j=i-1;j>=0;j--)   /* si no se puede reservar, libero la memoria que*/
                free(matriz[j]);   /* habia reservado previamente */
             free(matriz);
             matriz=NULL;          /* igualo el puntero que controla la matriz a NULL */
          }
    return(matriz);                /* devuelvo el puntero que controla la matriz*/
}


//---------------------------------------------------------------------------
// NAME: Libera_matriz_dinamica_int
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Memory liberation.
//---------------------------------------------------------------------------

void Libera_matriz_dinamica_int (int **matriz, int nfils)
{
   int i;

   for(i=0;i<nfils;i++)
	free (matriz[i]);
   free (matriz);
}

//---------------------------------------------------------------------------
// NAME: (...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Memory allocation/liberation for vector of int/float.
//---------------------------------------------------------------------------


float* Vector_dinamica_float(int nfils)
{
  float* vector;
   if((vector=(float *)calloc(nfils,sizeof(float)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}
  return vector;
}

void Libera_vector_dinamica_float (float *vector)
{
  free(vector);
}

int* Vector_dinamica_int(int nfils)
{
  int* vector;
   if((vector=(int *)calloc(nfils,sizeof(int)))==NULL){
       printf("Error en la asignacion de memoria");
       exit(3);}
  return vector;
}

void Libera_vector_dinamica_int (int *vector)
{
  free(vector);
}

//---------------------------------------------------------------------------
// NAME: MatrixInt2Float
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Convert a matrix of int to a matrix of float.
//---------------------------------------------------------------------------

float** MatrixInt2Float(int num_nodos, int **matrix)
{
  int i, j;
  float** matrix_flo = Matriz_dinamica_float(num_nodos, num_nodos);
  for(i=0;i<num_nodos;i++)
    for(j=0;j<num_nodos;j++)
      matrix_flo[i][j] = (float)matrix[i][j];
  return matrix_flo;
}


//---------------------------------------------------------------------------
// NAME: VectorInt2Float
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Convert a vector of int to a vector of float.
//---------------------------------------------------------------------------

float* VectorInt2Float(int num_nodos, int *vector)
{
  int i;
  float* vector_flo = Vector_dinamica_float(num_nodos);
  for(i=0;i<num_nodos;i++)
    vector_flo[i] = (float)vector[i];
  return vector_flo;
}

