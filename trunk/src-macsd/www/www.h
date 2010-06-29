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
        www() : shapes() {MAX = 255;};
        www(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn);
	
       ~www() {};
        www(const www& s) : shapes(s) {MAX = 255;};
	www(const shapes& s) : shapes(s) {MAX = 255;};	
	
	void inicial();
        vector< CANDIDATE > ejesNoUtilizados() const;
};

#endif
