#ifndef __HORMIGAS_H__
#define __HORMIGAS_H__

#include <iostream>
#include <vector>
#include "utils.h"
#if VERSION == V_SHAPE
#include "shapes.h"
#elif (VERSION == V_GO)
#include "ontologia.h"
#elif (VERSION == V_SCIENCEMAP)
#include "vmap.h"
#elif (VERSION == V_WWW)
#include "www.h"
#endif

/** 
 *  Hormiga:
 *  
 *  Clase que implementa la clase que representara una
 *      solucion al problema generico de grafos
 * 
 *  @author Rocio Romero Zaliz (basado en el codigo de Manuel Chica Serrano)
 */

class Hormiga {
        private:
        /** 
        * indicara cuantos ejes ha recorrido la hormiga
        */
        unsigned int _ejesAsignados;
        
         /**
        *  conjunto auxiliar que nos servira para guardar las nodos candidatos
        */
        vector < CANDIDATE > _candidatos;
    
        /**
        *  numero de colonias a las que pertenence la solucion 
        */
        vector<unsigned int> _colonias;
            	
        /**
        *  numero de objetivos, sera el tamanio del vector costes
        */
        unsigned int _numObjetivos;
 
        /**
        * flag para indicar si los costes almacenados estan actualizados o no
        */
        bool _costeValido;
        
        /**
        * Base de datos de instancias
        */
        vector< SOLUTION > _instancias;
        
        map<CANDIDATE, double>* _aparEje;
	
	/** 
        * subestructura que representa la hormiga
        */
        SOLUTION _subestructura;
	
	/**
        * Soporte actual
        */
        vector<unsigned int> _support;
	
        /**
        * vector de coste en donde se almacenara en la posicion i, el coste respecto al objetivo i-esimo
        */
        vector<float> _costes;
       
    public:
	vector<unsigned int> soporte() const {return _support;};
        
	Hormiga() {};
	
	/**
	* constructor
	* @param colonia un entero que indica la colonia a la que pertenece la hormiga
	* @param base base de datos (instancias)
	* @param numObjetivos el numero de objetivos 
	*/
    
	Hormiga (const vector< SOLUTION >& base, const unsigned int numObjetivos, map<CANDIDATE, double>* aparEje);

        Hormiga (const vector< SOLUTION >& base, const unsigned int numObjetivos, map<CANDIDATE, double>* aparEje, SOLUTION sub);

	/**
	 * constructor copia
	 * @param unaHormiga la hormiga que sera copiada
	 */
        Hormiga(const Hormiga &unaHormiga);

        /**
       * situa a la hormiga en el inicio de construccion de la solucion, es decir, una subestructura vacia
       */
        void posicionaInicialmente();

        /**
        * avanza la hormiga a la posicion indicada
        */

        void avanza(const CANDIDATE & nuevo);

        /**
        * devuelve el numero de ejes ya asignados
        * @return el numero de ejes ya asignados
        */
        unsigned int getEjesAsignados() { return _ejesAsignados; }
        
        /**
        * devuelve el conjunto de candidatos posibles
        */

        vector< CANDIDATE > getCandidatos();   
        
        /**
        * indica si un nodo ha sido ya utilizado o no
        * @param id es un entero que representa un nodo
        * @return devuelve true si ya ha sido asignado
        */
        bool Utilizado (const unsigned int id) const;

        /**
        operador de asignacion
        */
        Hormiga & operator= (const Hormiga & unaHormiga);
        
        /**
        devuelve el costo de la funcion objetivo pedida
        @param id funcion objetivo
        */
        float getCoste(const unsigned int id);
        
        /**
        Recalcula las funciones objetivo
        */
        void calculaCostes();
        
        /**
        devuelve la subestructura que guarda la hormiga
        */
        SOLUTION subEst() const {return _subestructura;};

	/* Modificado ROCIO x: TRUE = Jaccard, FALSE = Sin Jaccard */
        int dominancia(Hormiga& v, bool x, int y);

	/* Agregado por ROCIO */
	void local_search();
        
        void imprime(ostream &salida) const {salida << _subestructura << endl;};
        
        unsigned int getNumObjetivos() const {return _numObjetivos;};
        
        bool operator== (const Hormiga & unaHormiga);
        
        bool operator!= (const Hormiga & unaHormiga) {return !(*this == unaHormiga);};
        
        float getAparicionesEje(const CANDIDATE& eje);

        bool extendible();
};

ostream& operator<<(ostream& os, const Hormiga& s);

#endif
