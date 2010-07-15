#ifndef __VMAP_H__
#define __VMAP_H__

#include <string>
#include <vector>
#include <list>
#include <set>
#include <map>
#include <cassert>
#include <fstream>
#include "utils.h"
#include "shapes.h"
#include <iostream>

using namespace std;

class vmap : public shapes {
    public: 
        // Constructores y destructores
        vmap() : shapes() {MAX = 999;};
	vmap(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
        ~vmap() {};
        vmap(const vmap& s) : shapes(s) {MAX = 999;};
	vmap(const shapes& s) : shapes(s) {MAX = 999;};

	void inicial();
        vector< CANDIDATE > ejesNoUtilizados() const;

        unsigned int agregarNodo(const string & s);
	unsigned int agregarNodo(const unsigned int & s);
	unsigned int agregarNodoID(const unsigned int & n, const string & s);

        void agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s);
	void agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s);
};

#endif
