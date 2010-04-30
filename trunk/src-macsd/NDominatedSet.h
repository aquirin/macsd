#ifndef __NDominatedSet_H__
#define __NDominatedSet_H__

#include <fstream>

#include "utils.h"
#include "shapes.h"
#include "hormigas.h"

using namespace std;

/**
 *  NDominatedSet
 * 
 *  Realmente es el fichero de Pareto que utilizaremos. 
 *  
 *  @author Rocio Romero Zaliz (basado en el codigo de Manuel Chica Serrano)
 */

class NDominatedSet {
    private:
        vector<Hormiga> Pareto;

    public:
	unsigned int size() const {return Pareto.size();};
	
        typedef vector<Hormiga>::iterator iterator;
        
        iterator begin() {return Pareto.begin();};
        iterator end() {return Pareto.end();};
                        
	/**
	 * constructor
	 */
	NDominatedSet() {};
        
        unsigned int getNumElementos() const {return Pareto.size();};
        
        Hormiga& getElemento(const unsigned int id) {assert(id < getNumElementos()); return Pareto[id];};
        
        bool vacia() const {return (Pareto.size() == 0);};
	
	/**
	 * funcion que indica si una solucion seria dominada por alguna del conjunto
	 * @param solucion la solucion (secuencia de tareas) que sera chequeado frente a las del conjunto de Pareto
	 * @param preferencias nos indica si se utilizan preferencias en la dominancia o no
	 * @param numDominanciasPorPreferencias se incrementara el contador si se produce una dominancia por preferencias
	 * @return 1, 0  o -1 segun la dominancia de la solucion respecto al Pareto
	 */
	bool esDominada(Hormiga &solucion, bool preferencias, int &numDominanciasPorPreferencias);
        
	/**
	 * funcion que inserta una nueva solucion y elimina las que domina, siempre
	 * que no sea dominada por alguna solucion. Devuelve true si ha sido insertada
	 * false en caso contrario
	 * @param solucion el objeto SecTareas que se insertara en el Pareto
	 * @param preferencias indica con true si se utilizan preferencias en la no-dominancia
	 * @param numDominanciasPorPreferencias se incrementara el contador si se produce una dominancia por preferencias 
	 * @return devuelve true si se ha insertado correctamente en la lista
	 */
	bool add(Hormiga &solucion, bool preferencias, int &numDominanciasPorPreferencias);
	        
        bool addDominancia(Hormiga &solucion, bool preferencias, int &numDominanciasPorPreferencias);
        
	/**
	 * escribe en fichero informacion sobre el pareto tal como estaciones y asignacion de tareas,
     * numero de soluciones del Pareto...	
	 * @param salida el stream en donde se escribe la informacion
     */
	void writePareto(ofstream &salida);

	/**
	 * escribe en fichero informacion sobre el pareto tal como estaciones y asignacion de tareas,
     * numero de soluciones del Pareto...
	 * @param ruta la ruta del fichero en donde se escribe la informacion
     */
	void writePareto(const char *ruta);
        
    /**
     * funcion que escribe en un fichero por columnas los valores de las distintas funciones
	 * objetivo de las soluciones que contiene
	 * @param ruta la ruta del fichero en donde se escribe la informacion
	 */
    void writeObjsPareto(const char *ruta);
        
    /**
     * funcion que escribe en un fichero por columnas los valores de las distintas funciones
	 * objetivo de las soluciones que contiene
	 * @param salida el stream en donde se escribe la informacion
	 */
    void writeObjsPareto(ofstream &salida);
};

#endif
