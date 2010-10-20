#ifndef __VWWW_H__
#define __VWWW_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <cassert>
#include <fstream>
#include "shapes.h"
#include <iostream>

using namespace std;

class www : public shapes {
    public: 
        // Constructores y destructores
        www() : shapes() {MAX = 520;};
        www(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
	
       ~www() {};
        www(const www& s) : shapes(s) {MAX = 520;};
	www(const shapes& s) : shapes(s) {MAX = 520;};	
	
	void inicial();
        vector< CANDIDATE > ejesNoUtilizados() const;
	www crossover(const www & other) const; // No usar si hay ciclos!
	www mutation() const;
        void random(const unsigned int how_many);
	vector<unsigned int> hojas() const;
	
	unsigned int agregarNodo(const string & s);
	unsigned int agregarNodo(const unsigned int & s);
	unsigned int agregarNodoID(const unsigned int & n, const string & s);

        void agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s);
	void agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s);
  private:
	www _subgraph(CANDIDATE eje, unsigned int cual, unsigned int & donde) const;
};

#endif

