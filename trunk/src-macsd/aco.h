#ifndef ACO_H_
#define ACO_H_

#include <algorithm>

#include "hormigas.h"
#include "utils.h"
#include "algoritmoMO.h"

using namespace std;

/**
 * 
 *  ACO: (hereda de AlgoritmoMO)
 * 
 *  Clase general de algoritmo ACO, independientemente de si es un
 *  algoritmo mono o multi-objetivo. Tambien contiene las distintas hormigas
 *  que participan en el algoritmo y la matriz de feromona. 
 * 
 *  @see AlgoritmoMO
 * 
 *  @author Rocio Romero Zaliz (basado en el codigo de Manuel Chica Serrano)
 */

class ACO : public AlgoritmoMO {
	
protected:
    vector< SOLUTION > base;
    /** 
     * el vector con las hormigas (es decir, las subestructuras)
     */
    vector <Hormiga *> hormigas;
       
    /**
     * feromona inicial para la matriz de rastros de feromona
     */
    float feromonaInicial;
    
    /** 
     * tenemos un vector de matrices de feromona que contendran la feromona por nodo y enlace
     * tamanio n x n x l (n nodos, l enlaces)
     */
    // Es demasiado grande, solo voy a poner los validos
    map<CANDIDATE,float> matricesFeromona;
    

    /** 
     * vector donde se almacenara temporalmente las probabilidades de seleccion
     * de los arcos para la transicion de una hormiga
     */
    map<CANDIDATE,long double> probabilidades;
    
  
    /** 
     * vector para guardar las probabilidades umbrales de cada hormiga
	 * se han puesto alternos para que no se sesgue la informacion ya que las hormigas
	 * tambien daban mas o menos importancia a una u otra heuristica segun su posicion
	 */
        float *umbrales;
	
	/** 
	 * vector para almacenar la distinta heuristica de cada hormiga en un enfoque multicolony
	 */
	unsigned int *heuristicas;
		
	/** 
	 * funcion miembro para establecer umbrales y que cada hormiga funcione de forma distinta
	 * de igual forma asocia una heuristica distinta a cada hormiga para un enfoque multicolony
	 * @param multiheuristics entero que nos indica el tipo de heuristica utilizada
	 */
	void setUmbralesYHeuristicas(unsigned int multiheuristics);
        
        map<CANDIDATE, double>* _aparEje;
public:
	
	/** 
	 * constructor
	 * @param prec1 el conjunto de relaciones de precedencia
	 * @param infoTareas1 objetos que nos definen los datos de cada tarea
	 * @param params nos almacena todos los parametros del ACO
	 */
	ACO (vector< SOLUTION >& b, Parametros &params);
	
		
	/** 
	 * destructor
	 */
	~ACO ();
        
    /** 
     * funcion que indica la siguiente tarea a asignar a estacion
     * como es dependiente del tipo de ACO que tengamos, es virtual y debera
     * ser implementado por el algoritmo ACO que herede de esta clase
     * @param unaHormiga el objeto de la clase Hormiga que realizara la transicion
     * @param indice un entero que representa el numero de hormiga, nos sirve para 
     * 		dar a cada hormiga de un comportamiento distinto
     * @param candidatas el conjunto de tareas posibles o candidatas
     * @return el numero de tarea elegida en la transicion
     */
    virtual CANDIDATE transicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas) = 0;
    

    /** 
     * funcion que realiza las acciones necesarias dependiendo del tipo de ACO o
     * MOACO despues de que la hormiga tome una decision u otra
     * sera implementada por el algoritmo que herede de la clase general ACO
     * @param hormiga la hormiga que realizara las acciones
     * @param nodo1 el primer nodo elegido en el paso dado por la hormiga
     * @param nodo2 el segundo nodo elegido en el paso dado por la hormiga
     * @param eje el eje que se aplicara en la decision 
     */

    virtual void accionesTrasDecision (Hormiga *hormiga, const unsigned int nodo1, const unsigned int nodo2, const unsigned int eje) = 0;    

    /** 
     * funcion que realiza las acciones necesarias dependiendo del tipo de ACO o
     * MOACO despues de que todas las hormigas de cada iteracion terminen de 
     * construir sus soluciones
     * sera implementada por el algoritmo que herede de la clase general ACO
     */
    virtual void accionesFinalesHormiga () = 0;       

    /** 
     * funcion que simula el algoritmo ACO
     * @param filename nombre del fichero sobre el que escribir los datos finales
     * @return el conjunto de soluciones de Pareto obtenidas tras la ejecucion
     */
    
    NDominatedSet & ejecuta(string &filename);
};


#endif /*ACO_H_*/
