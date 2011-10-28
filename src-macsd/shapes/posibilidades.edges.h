#ifndef _POS_
#define _POS_

#include <iostream>
#include <vector>
#include <map>
#include <stack>
#include <set>

#include "global.h"
#include "shapes.h"

using namespace std;

class SOLUTION;

class posibilidades {
    public:
        posibilidades(const SOLUTION * mia, const SOLUTION * tuya);
        ~posibilidades() {};
        
        class iterator {
            public:
                iterator() {};
                ~iterator() {};
                iterator operator++();
                map<unsigned int,unsigned int> operator*();
                bool operator==(const iterator& x) const {return (_pos == x._pos);};
                bool operator!=(const iterator& x) const {return (!(*this == x));};
                
                friend class posibilidades;
            private:
                iterator(const SOLUTION * mia, const SOLUTION * tuya, map<tuplax3<string>, vector<CANDIDATE> > * p_cmine, map<tuplax3<string>, vector<CANDIDATE> > * p_cyour, map<unsigned int, vector<CANDIDATE> > * p_nmine,  map<unsigned int, vector<CANDIDATE> > * p_nyour, unsigned int root, const int pos);
                iterator(const int p) {_pos = p;};
		bool _finished() const;
		void _show() const;
		
                int _pos; // -1 means end()
		unsigned int _root;

		const shapes * _p_mine;
	        const shapes * _p_your;

		map<tuplax3<string>, vector<CANDIDATE> > * _p_cmine, * _p_cyour;
	        map<unsigned int, vector<CANDIDATE> > * _p_nmine, * _p_nyour;

		set<unsigned int> _initial;

		struct info {
			stack<unsigned int> pila_nodos; 			// de los mios
			map<unsigned int,unsigned int> nodes_so_far;		// mios -> tuyos
			unsigned int current_node;				// mio
			CANDIDATE current_edge;					// mio
			stack<CANDIDATE> opciones_ejes;				// mio -> tuyo
			set<CANDIDATE> ya_usados;				// de los tuyos (already explored edges)
		};
		stack<info> _trabajo;
        };
        
        iterator begin();
        iterator end();
    private:
	const SOLUTION * _mine;
	const SOLUTION * _your;

	map<tuplax3<string>, vector<CANDIDATE> > _cmine, _cyour;
	map<unsigned int, vector<CANDIDATE> > _nmine, _nyour;
};

#endif
