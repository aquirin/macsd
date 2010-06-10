#ifndef __ONTO_H__
#define __ONTO_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <stack>
#include <fstream>
#include "shapes.h"
#include <iostream>
#include <sstream>

using namespace std;

class go : public shapes {
  private:
    void _reconstruir_arbol(const unsigned int & s);
    
  public: 
        go(): shapes() {MAX = 51275;};
        go(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
        ~go() {};
        go(const go& s) : shapes(s) {MAX = 51275;};
	go(const shapes& s) : shapes(s) {MAX = 51275;};

        vector< CANDIDATE > ejesNoUtilizados() const;
	void inicial();
	float nivelRelativo(const go& s) const;
	
	unsigned int agregarNodo(const string & s);
	unsigned int agregarNodo(const unsigned int & s);
	unsigned int agregarNodoID(const unsigned int & n, const string & s);
	
	void agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s);
	void agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s);
};
#endif
