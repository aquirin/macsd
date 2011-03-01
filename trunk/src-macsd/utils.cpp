#include "utils.h"

//-----------------------------------------------
bool compDecreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b) { 
	return (a.dato > b.dato); 
}

//-----------------------------------------------
bool compCreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b) { 
	return (a.dato < b.dato); 
}

//-----------------------------------------------
void **matriz(long nfilas, long ncolumnas, long bytesPorDato)
{
	void **salida;
	long i;

	if (NULL == (salida = (void **)malloc (sizeof(void *) * nfilas)))
	{
		fprintf(stderr, "No se pudo reservar el vector de filas de la matriz\n"
			"(%ld,%ld) de %ld bytes por dato\n", nfilas, ncolumnas, bytesPorDato);
		exit(1);
	}

	if (NULL == (salida[0] = malloc(bytesPorDato * nfilas * ncolumnas)))
	{
		fprintf(stderr, "No se pudo reservar la matriz (%ld,%ld) de %ld bytes"
			" por dato\n", nfilas, ncolumnas, bytesPorDato);
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
			fprintf(salida, "%ld ", matriz[i][j]);
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
    if (inicio >= fin) {
        return inicio;
    }
    else
        return ((my_rand() % (fin - inicio + 1)) + inicio);
}

//-----------------------------------------------------------------------
int ranking(const unsigned int cant, const float nu_max, const float nu_min) {
    float random_entre_0_y_1 = my_rand()/(float(RAND_MAX)+1); 
    vector<float> proba(cant+1);
    
    proba[0] = 0.0;
    for (unsigned int i = 1; i <= cant; i++) {
        proba[i] = proba[i-1] + (1./cant)*(nu_min+(((nu_max-nu_min)*(i-1.))/(cant-1.)));
//          cout << proba[i] << " ";
    }
//     cout << endl;
    
//      cout << "RANDOM: " << random_entre_0_y_1 << endl;
//     float valor = 1+((((random_entre_0_y_1*cant)-nu_min)*(cant-1))/(nu_max-nu_min));
    unsigned int x = 0;
    while ((x <= cant) and (proba[x] < random_entre_0_y_1)) {
//          cout << x << " " << proba[x] << endl;
        x++;
    }
    if (x > cant) cerr << "ERRRRRRRRRRRRRRRRRR!" << endl;
    cout << "Salir: " << (cant - x + 1) << endl;
    return (cant - x + 1);
}


//-----------------------------------------------------------------------

// rand and srand in the same compilation unit (so the seed is controlled in
// some systems in which otherwise would have a different behavior).
int my_rand() {
    return rand();
}

//-----------------------------------------------------------------------

// rand and srand in the same compilation unit (so the seed is controlled in
// some systems in which otherwise would have a different behavior).
void my_srand(unsigned int seed) {
    srand(seed);
}
