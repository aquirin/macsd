#ifndef __UTILS_H__
#define __UTILS_H__


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;

template <typename T>
class tuplax3 {
    public:
        tuplax3();
        tuplax3(T a, T b, T c) {first = a; second = b; third = c;};
        bool operator==(const tuplax3<T>& o) const {return ((first == o.first) && (second == o.second) && (third == o.third));};
        
        T first;
        T second;
        T third;
};

//-----------------------------------------------
template<typename T>
bool operator< (const tuplax3<T>& a, const tuplax3<T>& b) {
    return ((a.first < b.first) || ((a.first == b.first) && (a.second < b.second)) || ((a.first == b.first) && (a.second == b.second) && (a.third < b.third)));
}

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

#define GUARDAR_RESULTADOS_INTERMEDIOS true

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
    int tipoAlgoritmo;   /// identificador del algoritmo
    float tCiclo;   /// tiempo de ciclo permitido
    unsigned long maxTiempo;    /// criterio de parada, el tiempo
    float areaMax;    /// area maxima permitida (no se utiliza)
    string rutaEntrada;     /// ruta del fichero de entrada
    string rutaSalida;     /// ruta del fichero de salida
    bool areaImplicita;     /// booleana que indica si el area se consigue a partir de los tiempos o no
    bool BL;        /// true si se utiliza busqueda local
    long semilla;         /// semilla para el generador de aleatorios
    unsigned int numObjs;        /// numero de objetivos del problema
    unsigned int preferencias;       /// entero que nos indica el tipo de preferencias usadas
    map<tuplax3<unsigned int>,double>* aparEje;
    
    /**
     * parametros MOACO
     */
    float tau0;        /// tau0 para la feromona inicial
    float beta;      /// valor beta del MOACO
    float ro;     /// valor ro del MOACO
    float q0;        /// valor q0 del MOACO
    unsigned int numHormigas;         /// numero de hormigas del algoritmo
    unsigned int multiheuristics;     /// entero que nos indica el tipo de multiheuristics utilizado
    
    float gamma;    /// valor para seleccionar si genero una hormiga nueva o extiendo una existente /// ROCIO
    
    float alfaGrasp;    /// parametro GRASP
    
    
    float alfaObj1;     /// parametro extra ACS
    
    
    float tau0Prima;	/// parametros extra MACSv2 y UnsortBicriterion 
    
    
    unsigned int numColonias;	/// parametro para MOACO multicolony UnsortBicriterion
    
    
    unsigned int nEstOptimo;	/// parametro extra para BicriterionMC
    
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
  
#endif
