#ifndef __UTILS_H__
#define __UTILS_H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string>
#include <map>
#include "global.h"
#include "Parameter.h"


using namespace std;

/**
 *  Utils.h
 * 
 *  Fichero con definiciones globales para todas las demas clases y funciones
 *  utiles. Estas funciones utiles son heredadas y la mayoria de ellas no se utilizan
 *  en la practica
 * 
 *  @author Manuel Chica Serrano  (email manuel.chica@softcomputing.es)
 */


/**
 * PARaMETROS CONSTANTES DEL ALGORITMO
 */
 
#define VALOR_CORRECCION_NULO 0.00001

#define GUARDAR_RESULTADOS_INTERMEDIOS false

/**
 * macros sobre los parametros de los algoritmos
 */
#define NUM_PARAMS_ALEATORIO 10
#define NUM_PARAMS_GRASP 11
#define NUM_PARAMS_MACS 15
#define NUM_PARAMS_ACS 15
#define NUM_PARAMS_MACSv2 14
#define NUM_PARAMS_UNSORT 16
#define NUM_PARAMS_BICRITERIONMC 16

#define ALGORITMO_ALEATORIO 0 
#define ALGORITMO_GRASP 1
#define ALGORITMO_MACS 2
#define ALGORITMO_ACS 3
#define ALGORITMO_MACSv2 4
#define ALGORITMO_UNSORT 5
#define ALGORITMO_BICRITERIONMC 6

/** 
 * 0--> SUCESORES EN AREA Y TC,  1--> NO SUCESORES,  2--> SUCESORES CON TC,  
 * 3--> SUCESORES CON AREA, 4--> SIN H1 NI H2
 */
#define CSEG_ASEG 0
#define C_A 1
#define CSEG_A 2 
#define C_ASEG 3
#define SOLO_FER 4

#define TIPO_HEURISTICA SOLO_FER

/**
 * valores para las preferencias, 0--> NO SE USAN PREFERENCIAS, 1--> EQUILIBRIO RESPECTO TC; 
 * 2--> EQUILIBRIO RESPECTO A; 3--> EQUILIBRIO RESPECTO TC Y A
 */
#define SIN_PREF 0
#define PREF_EQ_TC 1
#define PREF_EQ_A 2
#define PREF_EQ_TC_A 3

/**
 * valor para definir el tipo de regla de generacion de lambdas en algoritmos multicolony
 * en nuestro caso, para los algoritmos Unsort y Bicriterion
 */
#define RULE1 1
#define RULE2 2
#define RULE3 3

#define REGLA_LAMBDAS_MULTICOLONY RULE1

/**
 * numero de iteraciones de la busqueda local. Servira de condicion de parada
 */
#define N_ITER_BL 20

/**
 * Maximum number of line in a data file
 */
#define N_MAX_LINES 1000

// *** The while loop in aco.cpp: use the time (production) or the number of iterations (debug)?
//#define ENABLE_ITERATION_COUNT


/**
 * -------------------------------------------------------------------
 * ESTRUCTURA PARA PODER ORDENAR SIN PERDER EL ID O iNDICE
 *
 * definimos un struct para guardar un valor definido por un template y su indice,
 * nos servira para que cuando ordenemos las estaciones o tareas por areas y tiempos
 * con el QuickSort de STL, no perdamos el identificador de la estacion o de la tarea
 */

typedef struct X {
	
	/// dato que se va a ordenar
	float dato;     
	
	/// indice original de ese dato para no perder la posicion
	unsigned int id;	
	
} TipoDatoOrdenacion;

//--------------------------------------------------------------------


/**
 * Estructura para manejar excepciones y parametros 
 */
struct Error {
	
	/// el string que guarda el mensaje del error
    string message; 
    
    /// numero de error
    int number;     
    
    /**
     * constructor de la estructura
     * @param q el mensaje de error
     * @param j el numero de error
     */
    Error(string q, int j ) { message = q; number = j; }
};

/**
 * Estructura para guardar los parametros necesarios por todos los algoritmos
 * que estan incluidos en el sistema
 */
struct Parametros {
    
    /**
     *  parametros no dependientes del algoritmo
     */

    map<CANDIDATE,double>* aparEje;
    
};

// -----------------------------------

/**
 * Funciones de comparacion en orden creciente y decreciente.
 */
bool compDecreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b);
bool compCreciente (TipoDatoOrdenacion a, TipoDatoOrdenacion b);

/**
 * Copia de vectores
 */
void copiaVector(unsigned long *destino, unsigned long *origen, unsigned int tamanio);

/**
 * Funcion que da la diferencia entre dos struct timeb
 */
// double resta_tiempos (time t1, time t0);

/**
 * Reserva una matriz de nfilas * ncolumnas y bytesPorDato bytes por elemento de la matriz
 */
void **matriz(long nfilas, long ncolumnas, long bytesPorDato);

/**
 * funcion que devuelve la posicion de "elemento" en el vector, o tamanio si no lo encuentra
 */
unsigned int buscaElemento(unsigned int *vector, unsigned int tamanio, unsigned int elemento);

/**
 * funcion que devuelve si un vector contiene o no una permutacion
 */
bool esPermutacion(unsigned int *vector, unsigned int tamanio);

/**
 * funcion que calcula la distancia euclidea entre dos puntos
 */
float calculaDistancia(float posX1, float posY1, float posX2, float posY2);

/**
 * funcion que intercambia dos elementos
 */
void swapUI(unsigned int &n1, unsigned int &n2);

/**
 * funcion que devuelve la posicion del minimo elemento de un vector entre los que aUtilizar[i] == 1
 */
unsigned int posMinimo(float *vector, short *aUtilizar, unsigned int tamanio);

/**
 * funcion que devuelve la posicion del maximo elemento de un vector entre los que aUtilizar[i] == 1
 */
unsigned int posMaximo(float *vector, short *aUtilizar, unsigned int tamanio);

/**
 * funcion que almacena en el vector un conjunto de pesos aleatorios, cuya suma es 1
 */
void pesosAleatorios(float *vector, unsigned int tamanio);

int intAzar(const int inicio, const int fin);

int ranking(const unsigned int cant, const float nu_max, const float nu_min);

template <class G>
class better {
        public:
                better(int d) : _indice(d) {};

                bool operator ()(const G &a, const G &b) const {
                        return (a.first.better(b.first, _indice));
                }

        private:
                int _indice;
};

int my_rand();
void my_srand(unsigned int seed);

#endif
