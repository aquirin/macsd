#ifndef __SHAPES_H__
#define __SHAPES_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include "utils.h"
#include <iostream>

using namespace std;

class shapes {
    public:
        shapes() {_nodo = -1;};
        shapes(const unsigned int x, const unsigned int l) : _grafo(x, vector<vector <bool> >(x, vector<bool>(l, false))) {_nodo = -1;};
        ~shapes() {};
        shapes(const shapes& s) : _grafo(s._grafo) {_nodo = s._nodo;};
        
        void agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s);
        void agregarNodo(const int nod) {assert((nod-1 < cantNodos()) && (_nodo == -1)); _nodo = nod-1;};
        void clear();
                
        unsigned int size() const;
        
        bool ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {return _grafo[ini-1][fin-1][s-1];};
        
        bool nodoUsado(const unsigned int nod) const;
        
        set<unsigned int> nodosUtilizados() const;
        
        set< tuplax3<unsigned int> > ejesNoUtilizados() const;
        vector< tuplax3<unsigned int> > ejesNoUtilizadosButIn(vector<shapes> _inst) const;
        
        unsigned int elNodo() const {return _nodo;};
        
        unsigned int cantNodos() const {return _grafo.size();};
        unsigned int cantEjes() const {return _grafo[0][0].size();};

        void imprime(ostream &salida) const;
        
        bool operator== (const shapes& s) const;
        
        bool operator!= (const shapes& s) const {return !(*this==s);};
    protected:
        int _nodo;
        vector< vector< vector<bool> > > _grafo;
};

ostream& operator<<(ostream& os, const shapes& s);

#endif
