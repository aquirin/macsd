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
        vmap() : shapes() {MAX = 500;};
	vmap(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
        ~vmap() {};
        vmap(const vmap& s) : shapes(s) {MAX = 500;};
	vmap(const shapes& s) : shapes(s) {MAX = 500;};	

	void inicial();
        vector< CANDIDATE > ejesNoUtilizados() const;
};

#endif
