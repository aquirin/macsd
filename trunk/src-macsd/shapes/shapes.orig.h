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

using namespace std;

class shapes {
    public:
        shapes() {};
        shapes(const unsigned int maxobj, const vector<string> shap) {_nodo = 0; _mobj = maxobj; _shap = shap;};
        ~shapes() {};
        shapes(const shapes& s) : _nodos(s._nodos), _ejes(s._ejes), _desc(s._desc) {_nodo = s._nodo; _mobj = s._mobj; _shap = s._shap;};
        
        void agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s);
        void agregarNodo(const unsigned int nod, const string& s);
        void clear();
        unsigned int size() const;
        float sizeNorm() const {return ((size() * 1.) / (_mobj * _shap.size() * 1.));};
        bool ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {return (_ejes.find(tuplax3<unsigned int>(ini,fin,s)) != _ejes.end());};    
        set<unsigned int> nodosUtilizados() const;
        vector< CANDIDATE > nodosNoUtilizados() const;
        void imprime(ostream &salida) const;
        string graph_g(void) const;
        bool operator== (const shapes& s) const;   
        bool igual(const shapes& s) const;   
        bool operator!= (const shapes& s) const {return !((*this)==s);};
        vector< CANDIDATE > posibilidades_totales();
//         vector< CANDIDATE > posibilidades_reales();
        unsigned int cantNodos() const {return _nodos.size();};
        vector< vector<unsigned int> > darPosibilidades(const shapes& s) const;
        bool empty() const;
        
        shapes reasignarNodos(vector<unsigned int> v);
        shapes reasignarNodosFijo(vector<unsigned int> v);
        bool cubiertoPor(const shapes& s) const;
        
    protected:
        unsigned int _nodo;
        set<unsigned int> _nodos;
        set< tuplax3<unsigned int> > _ejes;
        
        map<unsigned int, string> _desc;
        
        unsigned int _mobj;
        vector<string> _shap;
};

ostream& operator<<(ostream& os, const shapes& s);

#endif
