#include "utils.h"


// //-----------------------------------------------
// template<typename T>
// bool operator< (const tuplax3<T>& a, const tuplax3<T>& b) {
//     return ((a.first < b.first) || ((a.first == b.first) && (a.second < b.second)) || ((a.first == b.first) && (a.second == b.second) && (a.third < b.third)));
// }

//-----------------------------------------------
bool compDecreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b) { 
	return (a.dato > b.dato); 
}

//-----------------------------------------------
bool compCreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b) { 
	return (a.dato < b.dato); 
}

// //-----------------------------------------------
// double resta_tiempos (timeb t1, timeb t0)
// {
// 	/* Devuelve t1 - t0 */
// 	double temp1, temp2;
// 
// 	temp1  = (double) t0.time + (t0.millitm/1000.0);
// 	temp2 = (double) t1.time + (t1.millitm/1000.0);
// 
// 	return temp2-temp1;
// }

//-----------------------------------------------
void **matriz(long nfilas, long ncolumnas, long bytesPorDato)
{
	void **salida;
	long i;

	if (NULL == (salida = (void **)malloc (sizeof(void *) * nfilas)))
	{
		cerr << "No se pudo reservar el vector de filas de la matriz" << endl << "(" << nfilas << "," << ncolumnas << ") de " << bytesPorDato << "bytes por dato" << endl;
		exit(1);
	}

	if (NULL == (salida[0] = malloc(bytesPorDato * nfilas * ncolumnas)))
	{
		cerr << "No se pudo reservar el vector de filas de la matriz" << endl << "(" << nfilas << "," << ncolumnas << ") de " << bytesPorDato << "bytes por dato" << endl;
		free(salida);
		exit(1);
	}

	for(i = 1; i < nfilas; i++)
		salida[i] = ((char*)salida[i - 1]) + ncolumnas * bytesPorDato;

	return salida;
}

//-----------------------------------------------------------------------
unsigned int buscaElemento(unsigned int *vector, unsigned int tamanio, unsigned int elemento)
{
	unsigned int i;

	for (i = 0; i < tamanio; i++)
	{
		if (vector[i] == elemento)
			return i;
	}

	return i;
}

//-----------------------------------------------------------------------
bool esPermutacion(unsigned int *vector, unsigned int tamanio)
{
	unsigned int i;

	for (i = 1; i < tamanio; i++)
	{
		//Si alg�n elemento es mayor al tamanio, no puede ser una permutaci�n de elementos
		//consecutivos que empieza en 0
		if (vector[i - 1] >= tamanio)
		{
			fprintf(stderr, "Valor en posicion %d mayor o igual a %u\n", i - 1, tamanio);
			return false;
		}

		//Si un elemento est� repetido en otra posici�n posterior, no es una permutaci�n
		if (buscaElemento(vector + i, tamanio - i, vector[i - 1]) != tamanio - i)
		{
			fprintf(stderr, "Posiciones repetidas %d y %d\n", i - 1, buscaElemento(vector + i, tamanio - i, vector[i - 1]));
			return false;
		}
	}

	//Si no sale por otro lado es que es una permutaci�n
	return true;
}

//-----------------------------------------------------------------------
void swapUI(unsigned int &n1, unsigned int &n2)
{
	unsigned int aux;

	aux = n1;
	n1 = n2;
	n2 = aux;
}

//-----------------------------------------------------------------------
float calculaDistancia(float posX1, float posY1, float posX2, float posY2)
{
	float posX, posY;

	posX = (posX2 - posX1);
	posY = (posY2 - posY1);

	return (sqrt(posX * posX + posY * posY));
}

//-----------------------------------------------------------------------
unsigned int posMinimo(float *vector, short *aUtilizar, unsigned int tamanio)
{
	unsigned int i;
	unsigned int minVal = 0;

	//Asignar como menor valor el primer no marcado
	for (i = 0; i < tamanio; i++)
	{
		if (aUtilizar[i] == 1)
		{
			minVal = i;
			break;
		}
	}

	//buscar alguno menor que el menor
	for (; i < tamanio; i++)
	{
		if (vector[minVal] > vector[i] && aUtilizar[i] == 1)
		{
			minVal = i;
		}
	}

	return minVal;
}

//-----------------------------------------------------------------------
unsigned int posMaximo(float *vector, short *aUtilizar, unsigned int tamanio)
{
	unsigned int i;
	unsigned int maxVal = 0;

	//Asignar como mayor valor el primer no marcado
	for (i = 0; i < tamanio; i++)
	{
		if (aUtilizar[i] == 1)
		{
			maxVal = i;
			break;
		}
	}

	//buscar alguno mayor que el mayor
	for (; i < tamanio; i++)
	{
		if (vector[maxVal] < vector[i] && aUtilizar[i] == 1)
		{
			maxVal = i;
		}
	}

	return maxVal;
}

//-----------------------------------------------------------------------
void copiaVector(unsigned long *destino, unsigned long *origen, unsigned int tamanio)
{
	unsigned int i;

	for (i = 0; i < tamanio; i++)
		destino[i] = origen[i];
}

//-----------------------------------------------------------------------
void escribeMatriz(FILE *salida, unsigned long **matriz, unsigned int tamanio)
{
	unsigned int i, j;

	for (i = 0; i < tamanio; i++)
	{
		fprintf(salida, "\n");

		for(j = 0; j < tamanio; j++)
			fprintf(salida, "%d ", (int)matriz[i][j]);
	}
}

//-----------------------------------------------------------------------
void pesosAleatorios(float *vector, unsigned int tamanio){
	
	unsigned int i;
	float suma = 0;
	
	for (i = 0; i < tamanio; i++){
		
// 		vector[i] = Rand();
		suma += vector[i];
	}
	
	for (i = 0; i < tamanio; i++)
		vector[i] /= suma;
}

//-----------------------------------------------------------------------
int intAzar(const int inicio, const int fin) {
    if (inicio == fin) {
        return inicio;
    }
    else
        return (rand() % fin + inicio);
}
