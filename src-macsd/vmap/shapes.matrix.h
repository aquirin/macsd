///// DO NOT MODIFY shapes.cpp/h BUT shapes.*.cpp/h
///// AS THESE FILES WILL BE ERASED BY THE MAKEFILE

#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include "../utils.h"
#include <iostream>
#include "../routines.h"

using namespace std;

class shapes {
    public:
        shapes() {_nodo = -1; num_nodes = -1; num_edges = -1; _grafo=NULL; };
        shapes(const unsigned int x, const unsigned int l);
        ~shapes();
        shapes(const shapes& s);
        
        void agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s);
        void agregarNodo(const int nod) {assert((nod-1 < cantNodos()) && (_nodo == -1)); _nodo = nod-1;};
        void clear();
                
        unsigned int size() const;
        
        bool ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {return _grafo[ini-1][fin-1][s-1];};
        
        bool nodoUsado(const unsigned int nod) const;
        
        set<unsigned int> nodosUtilizados() const;
        
        set< CANDIDATE > ejesNoUtilizados() const;
        vector< CANDIDATE > ejesNoUtilizadosButIn(vector<shapes> _inst) const;
        
        unsigned int elNodo() const {return _nodo;};
        
        unsigned int cantNodos() const {return num_nodes;};
        unsigned int cantEjes() const {return num_edges;};

        void imprime(ostream &salida) const;
        
        bool operator== (const shapes& s) const;
        
        bool operator!= (const shapes& s) const {return !(*this==s);};
	
        /**
        operador de asignacion
        */
        shapes & operator= (const shapes & aShape);
	
    protected:
        int _nodo;
        char*** _grafo;
	int num_nodes, num_edges;
};

ostream& operator<<(ostream& os, const shapes& s);

#endif
