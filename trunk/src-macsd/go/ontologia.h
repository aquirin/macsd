#ifndef __ONTO_H__
#define __ONTO_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include "../utils.h"
#include "shapes.h"
#include <iostream>


using namespace std;
//class shapes;
//class CANDIDATE;

class go : public shapes {
    public: 
        // Constructores y destructores
        go() {};
        go(string name, set<unsigned int>* bn, set< CANDIDATE >* be, map<unsigned int,string>* d);
        ~go() {};
        go(const go& s);

        // Recuperacion de la informacion
        set<unsigned int>* base_nodos() const {return _base_nodos;};
        set< CANDIDATE >* base_ejes() const {return _base_ejes;};
        map<unsigned int, string>* desc() const {return _desc;};
        set<unsigned int> nodos() const {return _nodos;};
        set< CANDIDATE > ejes() const {return _ejes;};
        
        // Consultas
        bool valido(const unsigned int ini, const unsigned int fin, const unsigned int s) const;
        
        // Calculos
        float nivelRelativo(const go& s) const;
        
        // Heredadas y redefinidas
        void agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s);
        void agregarNodo(const int nod);
        void clear();           
        unsigned int size() const;    
        double sizeNorm() const;
        bool ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const;
        bool nodoUsado(const unsigned int nod) const;  
        set<unsigned int> nodosUtilizados() const;      
        vector< CANDIDATE > ejesNoUtilizados() const;        
        unsigned int cantNodos() const;
        unsigned int cantEjes() const;
        void imprime(ostream &salida) const;        
        bool operator== (const go& s) const;
        bool operator!= (const go& s) const;
    private:
        set<unsigned int> _nodos;
        set< CANDIDATE > _ejes;
        
        set<unsigned int>* _base_nodos;
        set< CANDIDATE >* _base_ejes;
        map<unsigned int, string>* _desc;
        string _name;
};

ostream& operator<<(ostream& os, const go& s);

#endif
