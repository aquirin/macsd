#ifndef __MACS_H__
#define __MACS_H__

#include "aco.h"
#include "utils.h"

#if VERSION == V_SHAPE
#include "shapes.h"
#elif VERSION == V_GO
#include "ontologia.h"
#elif VERSION == V_SCIENCEMAP
#include "vmap.h"
#elif VERSION == V_WWW
#include "www.h"
#endif


/**
 * 
 *  MACS: (hereda de clase ACO)
 *
 *  Clase que implementa la super clase ACO, implementando las caracteristicas del algoritmo
 *  multiobjetivo MACS, y basandose en las hormigas y matriz de feromona de la clase ACO.
 * 
 *  @see ACO
 *  @author Manuel Chica Serrano  (email manuel.chica@softcomputing.es)
 * 
 */

class MACS : public ACO {
    protected:
    /**
     * vectores con los valores de los productos de lambda, alpha y beta para cada hormiga
     */
        vector <float> betaLambda, betaNoLambda;
    
    
    // funciones miembro privadas -------------------------------

    /**
     * funcion que calcula las probabilidades de los arcos a utilizar en el siguiente paso de la hormiga
     * devuelve la suma de ellas (debiera ser 1)
     * @param unaHormiga el objeto de la clase Hormiga que realizara la transicion
     * @param indice el indice de la hormiga para que esta actue de una forma u otra
     * @param candidatas el conjunto de tareas posibles o candidatas
     * @return devuelve la probabilidad calculada
     */
    long double calculaProbabilidadesTransicion(Hormiga &unaHormiga, unsigned int indice,
		vector< CANDIDATE > &candidatas);

    /**
     * funcion que solo calcula los numeradores de las probabilidades
     * devuelve la suma de ellos
     * @param unaHormiga el objeto de la clase Hormiga que realizara la transicion
     * @param indice el indice de la hormiga para que esta actue de una forma u otra
     * @param candidatas el conjunto de tareas posibles o candidatas
     * @return devuelve la suma de los numeradores de las probabilidades
     */
    long double calculaNumeradoresProbabilidades(Hormiga &unaHormiga, unsigned int indice,
		vector< CANDIDATE > &candidatas);

    
    /**
     * funcion que indica la siguiente tarea a asignar a estacion
     * se implementa esta funcion que era virtual en la clase ACO
     * @param unaHormiga el objeto de la clase Hormiga que realizara la transicion
     * @param indice un entero que representa el numero de hormiga, nos sirve para 
     * 		dar a cada hormiga de un comportamiento distinto
     * @param candidatas el conjunto de tareas posibles o candidatas
     * @return el numero de tarea elegida en la transicion
     */
    CANDIDATE transicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas);

    /**
     * funcion que realiza la actualizacion global de feromona
     */
    void modificaFeromona();

    /**
     * funcion que modifica la feromona tras una eleccion de una hormiga
     * @param numeroTarea el numero de tarea sobre el que se actualizara la feromona
     * @param numeroEstacion el numero de estacion sobre la que se actualizara la feromona
     */

    void modificaFeromonaPasoAPaso(unsigned int x, unsigned int y, unsigned int z);

    /**
     * funcion que calcula la nueva feromona inicial
     * @return devuelve ese valor de tau0
     */
    float nuevaTau0();

    /**
     * funcion que reinicializa la matriz de feromona con el nuevo Tau0
     */
    void reinicializaMatriz();

    /**
     * se implementa esta funcion que era virtual en la clase ACO
     * en ella se llaman a las funciones o se realizan las operaciones que
     * el MACS realiza tras tomar una decision entre un nodo u otro
     * @param hormiga la hormiga que realizara las acciones
     * @param tareaElegida la tarea elegida en el paso dado por la hormiga
     * @param numeroEstacion la estacion a la que se ha asignado la tarea en la decision 
     */

    void accionesTrasDecision (Hormiga *hormiga, unsigned int x, unsigned int y, unsigned int z);

    /**
     * implementacion sobre la funcion virtual original de la clase ACO
     * en ella se especifican las acciones finales que realizaran las hormigas 
     * tras construir sus soluciones al final de cada iteracion en un algoritmo UnsortBicriterion
     */
    void accionesFinalesHormiga ();

public: 
	
	/**
       * Constructor
	 * @param base el conjunto de datos geometricos
	 * @param params nos almacena todos los parametros del BicriterionMC
	 */
	MACS(vector<SOLUTION>& base, Parametros &params);

};

#endif
