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
#include "utils.h"
#include "posibilidades.h"
#include <iostream>

using namespace std;

class shapes {
    public:
        shapes() {};
        ~shapes() {};

        shapes(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
        shapes(const shapes& s);
        
        void agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s);
	void agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s);
        void borrarNodo(const unsigned int & ini);
        unsigned int agregarNodo(const string & s);
	unsigned int agregarNodo(const unsigned int & s);
	unsigned int agregarNodoID(const unsigned int & n, const string & s);
        void random(const unsigned int how_many);
        void clear();
        unsigned int size() const;
        float sizeNorm() const;
        bool ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {return (_ejes.find(CANDIDATE(ini,fin,s)) != _ejes.end());};    
	bool nodoUsado(const unsigned int ini) const {return (_nodos.find(ini) != _nodos.end());};  
        set<unsigned int> nodosUtilizados() const;
	string tipoNodo(const unsigned int x) const;
	unsigned int nuevo_codigo_nodo(const unsigned int x) const;
        vector< CANDIDATE > ejesNoUtilizados() const;
	set< CANDIDATE > ejesUtilizados() const {return _ejes;};
        string forma(const unsigned int x) const;
	bool cambiar_forma(const unsigned int x);
        void imprime(ostream &salida) const;
        string graph_g(void) const;
        bool operator== (const shapes& s) const;   
        bool igual(const shapes& s) const;   
        bool operator!= (const shapes& s) const {return !((*this)==s);};
        vector< CANDIDATE > posibilidades_totales();
        unsigned int cantNodos() const {return _nodos.size();};
        bool empty() const;
	unsigned int mapear(const unsigned int& i) const;
	void inicial();
	multimap<unsigned int, pair<unsigned int, unsigned int> > ejes_a_multimap() const;
        vector<unsigned int> hojas() const;
	map<unsigned int, vector<unsigned int> > darPosibilidades(const shapes& s) const;
	vector<unsigned int> darPosibilidades(const shapes& donde, const string& s) const;
        shapes reasignarNodos(const map<unsigned int,unsigned int> & v);
        bool cubiertoPor(const shapes& s) const;

	bool ejeTipoUsado(CANDIDATE ej, const shapes & other) const;

	unsigned int top() const;
	unsigned int bottom() const;
	
	shapes subarbol(const unsigned int sel, bool updown) const;
	void merge(const shapes& other, const unsigned int x, const unsigned int y, const string &edge);

        shapes& operator=(const shapes& other);
    protected:
	// Datos sobre nombre instancia, nodos y ejes
        string _name;
	set<unsigned int> _nodos;
        set< CANDIDATE > _ejes;
        
	map<unsigned int,unsigned int> _relacion_nodos; // Numero de nodo, codigo de nodo

        // Base de datos de ejes y nodos
        static set< CANDIDATE > _base_ejes;
        static map<unsigned int, string> _desc_nodo;
	static map<unsigned int, string> _desc_eje;
	static map<string, unsigned int> _rdesc_nodo;
	static map<string, unsigned int> _rdesc_eje;

	static unsigned int MAX;
};

ostream& operator<<(ostream& os, const shapes& s);

#endif
