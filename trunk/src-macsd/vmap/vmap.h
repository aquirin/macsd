#ifndef __VMAP_H__
#define __VMAP_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include "../utils.h"
// #include "shapes.h"
#include <iostream>


using namespace std;
//class shapes;
//class CANDIDATE;

class vmap {
    public: 
        // Constructores y destructores
        vmap() {};
        vmap(map<string,unsigned int>* bn, set< pair< pair<string,string>, unsigned int> >* be, const bool &azar);
        ~vmap() {};
        vmap(const vmap& s);

        // Recuperacion de la informacion
        map<string,unsigned int>* base_nodos() const {return _base_nodos;};
        set< pair< pair<string,string>, unsigned int> >* base_ejes() const {return _base_ejes;};
        map<unsigned int,string> nodos() const {return _nodos;};
        set< CANDIDATE > ejes() const {return _ejes;};
        
        // Consultas
        bool valido(const unsigned int& ini, const unsigned int& fin, const unsigned int& s) const;
        
        // Heredadas y redefinidas
        void agregarEje(const unsigned int& ini, const unsigned int& fin, const unsigned int& s);
        void agregarNodo(const int& nod, const string& cad);
        void clear();           
        unsigned int size() const;    
        double sizeNorm() const;
        bool ejeUsado(const unsigned int& ini, const unsigned int& fin, const unsigned int& s) const;
        set<unsigned int> nodosUtilizados() const;     
        vector< CANDIDATE > ejesNoUtilizados() const;        
        unsigned int cantNodos() const;
        unsigned int cantEjes() const;
        void imprime(ostream &salida) const;        
        bool operator== (const vmap& s) const;
        bool operator!= (const vmap& s) const;
	bool empty() const {return _nodos.empty();};
        bool igual(const vmap& s) const;   
	unsigned int mapear(const unsigned int& i) const;
	void azar();
	
	vector< CANDIDATE > posibilidades_totales() const;
        vector< vector<unsigned int> > darPosibilidades(const vmap& s) const;
	vmap reasignarNodos(vector<unsigned int> v) const;
	vmap reasignarNodosFijo(const vector<unsigned int>& v) const;
	bool cubiertoPor(const vmap& s) const;
    private:
        map<unsigned int,string> _nodos;
        set< CANDIDATE > _ejes;
        
        map<string,unsigned int>* _base_nodos;
        set< pair< pair<string,string>, unsigned int> >* _base_ejes;
};

ostream& operator<<(ostream& os, const vmap& s);

#endif
