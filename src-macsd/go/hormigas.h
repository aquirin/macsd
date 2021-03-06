#ifndef __HORMIGAS_H__

#define __HORMIGAS_H__

#include <iostream>
#include <vector>
#include "ontologia.h"
#include "utils.h"

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
        * subestructura que representa la hormiga
        */
         pair<string,go>  _subestructura;
        
         /**
        *  conjunto auxiliar que nos servira para guardar las nodos candidatos
        */
        vector < tuplax3<unsigned int> > _candidatos;
    
        /**
        *  numero de colonias a las que pertenence la solucion 
        */
        vector<unsigned int> _colonias;
            	
        /**
        *  numero de objetivos, sera el tamanio del vector costes
        */
        unsigned int _numObjetivos;
 
         /**
        * vector de coste en donde se almacenara en la posicion i, el coste respecto al objetivo i-esimo
        */
        vector<float> _costes;

        /**
        * flag para indicar si los costes almacenados estan actualizados o no
        */
        bool _costeValido;
        
        /**
        * Base de datos de instancias
        */
        vector< pair<string,go> > _instancias;
        
        /**
        * Soporte actual
        */
        vector<unsigned int> _support;
        
        float _gamma;
        
        map<tuplax3<unsigned int>, double>* _aparEje;
    public:
	vector<unsigned int> soporte() const {return _support;};
        
	/**
	 * constructor
	 * @param colonia un entero que indica la colonia a la que pertenece la hormiga
       * @param base base de datos (instancias)
       * @param numObjetivos el numero de objetivos 
      */
    
	Hormiga (const unsigned int colonia, const vector< pair<string,go> >& base, const unsigned int numObjetivos, map<tuplax3<unsigned int>, double>* aparEje);
        
        Hormiga (const unsigned int colonia, const vector< pair<string,go> >& base, const unsigned int numObjetivos, map<tuplax3<unsigned int>, double>* aparEje, pair<string,go> sub);

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
        void avanza(const unsigned int nodo1, const unsigned int nodo2, const unsigned int enlace);

        /**
        * devuelve el numero de ejes ya asignados
        * @return el numero de ejes ya asignados
        */
        unsigned int getEjesAsignados() { return _ejesAsignados; }
        
        /**
        * devuelve el conjunto de candidatos posibles
        */
        vector< tuplax3<unsigned int> > getCandidatos();   
        
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
        pair<string,go> subEst() {return _subestructura;};
        
        int dominancia(const Hormiga& v, bool x, int y);
        
        void imprime(ostream &salida) const {salida << "Nombre: " << _subestructura.first << endl; _subestructura.second.imprime(salida);};
        
        int getNumObjetivos() const {return _numObjetivos;};
        
        bool operator== (const Hormiga & unaHormiga) {return (_subestructura.second == unaHormiga._subestructura.second);};
        
        bool operator!= (const Hormiga & unaHormiga) {return (_subestructura.second != unaHormiga._subestructura.second);};
        
        float getAparicionesEje(const tuplax3<unsigned int>& eje);
};

#endif
