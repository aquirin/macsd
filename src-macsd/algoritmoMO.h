#ifndef _ALGORITMOMO_H
#define	_ALGORITMOMO_H

#include <sys/timeb.h>
#include "NDominatedSet.h"

using namespace std;


/**
 * 
 *  AlgoritmoMO: 
 *  
 *  Clase general de algoritmo, especifico para algoritmo multiobjetivo, pero tambien podria ser utilizable por algoritmo monoobjetivos. Contiene informacion 
 *  de ejecucion, control de tiempos y archivo de Pareto para incluir las soluciones no dominadas
 *  del algoritmo. En algoritmos monoobjetivo, este archivo no se utilizaria.
 * 
 *  @author Manuel Chica Serrano  (email manuel.chica@softcomputing.es)
 */

class AlgoritmoMO {
    
protected:
    
    /**
     * variable que almacena el tiempo maximo permitido para la ejecucion del algoritmo
     */
    unsigned long maxTiempo;
    
    // Pareto
    NDominatedSet conjuntoNoDominadas;
    
    /**
     * variable que cuenta el numero de evaluaciones realizadas
     */
    unsigned long numEvaluaciones;

    /**
     * variable que cuenta el numero de iteraciones
     */
    unsigned long numIteraciones;

    /** 
     * variable que anota el inicio de la aplicacion del algoritmo
     */
    unsigned long inicioAlgoritmo;

    /**
     * variable que anota el fin de la aplicacion del algoritmo
     */
    unsigned long finAlgoritmo;

    /**
     * variable que anota el tiempo en el que el conjunto de salida fue modificado por ultima vez
     */
    unsigned long tiempoUltimaModificacion;

    /**
     * variable que anota la iteracion en el que el conjunto de salida fue modificado por ultima vez
     */
    unsigned long iteracionUltimaModificacion;

    /**
     * variable que anota el numero de evaluaciones hechas hasta que el conjunto de salida 
     * fuese modificado por ultima vez
     */
    unsigned long evaluacionesUltimaModificacion;

    /**
     * variable que cuenta el numero de aplicaciones de la BL
     */
    unsigned long numBLs;
    
    /**
     * variable que nos almacena el tiempo dedicado a la busqueda local durante la ejecucion
     * del algoritmo
     */
    unsigned long tiempoDedicadoBLs;
    
    /**
     * numero de veces en las que se ha decidido por dominancia basada en preferencias
     */
    int numDominanciasPorPreferencias;
    
    /**
     * variable que indica si se utiliza o no BL
     */
    bool BL;
    
    /**
     * numero de objetivos
     */
    int nObj;
    
	/**
	 * la siguiente variable nos dice si queremos que  haya preferncias en la dominancia 
	 * y que tipo de prefs.
	 */
    unsigned int preferencias;
        
    float gamma;    
        
public:
    
    /**
     * Constructor
     * @param BL nos dice si se aplica BL o no
     * @param nObj1 numero de objetivos a optimizar
     * @param preferencias1 nos dice si queremos que haya preferences en la dominancia
     */ 
    AlgoritmoMO (bool BL, int nObj1, unsigned int preferencias1) {        
    
    	this->maxTiempo = 0;
    	this->tiempoDedicadoBLs = 0;
    	this->numEvaluaciones = 0;
    	this->numIteraciones = 0;
		this->numBLs = 0;
		this->numDominanciasPorPreferencias = 0;
		this->finAlgoritmo = this->tiempoUltimaModificacion = this->inicioAlgoritmo = clock()/CLOCKS_PER_SEC;
		this->iteracionUltimaModificacion = 0;
		this->evaluacionesUltimaModificacion = 0;     
        this->BL = BL;     
        this->nObj = nObj1;
        this->preferencias = preferencias1;
        
    }
    
    /**
     * Destructor virtual
     */
    virtual ~AlgoritmoMO () {}
    
    /**
     * imprime en un stream informacion sobre la ejecucion del algoritmo
     * @param f es el stream sobre el que se escribira la informacion
     */
    void printInfo(ofstream &f) {

        f << "<html><body><table><tr><th>Numero de iteraciones</th><th>Numero de evaluaciones</th><th>Numero de BLs</th>";
        f << "<th>Tiempo consumido</th><th>Iteracion Ultima modificacion</th><th>Evaluaciones Ultima modificacion</th>";
        f << "<th>Tiempo dedicado a BLs</th><th>Tiempo Ultima modificacion</th><th>Numero dominancias por pref.</th></tr><tr>";
        f << "<td>" << this->numIteraciones << "</td><td>" << this->numEvaluaciones;
        f << "</td><td>" << this->numBLs << "</td><td>" << (this->finAlgoritmo - this->inicioAlgoritmo) ;
        f << "</td><td>" << this->iteracionUltimaModificacion << "</td><td>" << this->evaluacionesUltimaModificacion;
        f << "</td><td>" << this->tiempoDedicadoBLs << "</td><td>";
        f << (this->tiempoUltimaModificacion - this->inicioAlgoritmo) << "</td>";
        f << "<td>" << this->numDominanciasPorPreferencias << "</td>";
        
        f << "</tr></table></body></html>";
    }

    /**
     * imprime en un stream informacion sobre la ejecucion del algoritmo
     * @param nombre es el nombre del fichero sobre el que se escribira la informacion
     */
    void printInfo(string &nombre){

        ofstream file(nombre.c_str());

        if (!file){
                throw Error("No se pudo abrir el fichero para escritura", 5);
        }

        this->printInfo(file);
        file.close();
    }
    
    virtual NDominatedSet & ejecuta(string &filename) = 0;
};

#endif	/* _ALGORITMOMO_H */

