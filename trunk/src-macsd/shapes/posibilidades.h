#ifndef _POS_
#define _POS_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <typename T>
class posibilidades {
    public:
        posibilidades(const map<T, vector<T> >& cuales);
        ~posibilidades() {};
        
        class iterator {
            public:
                iterator() {};
                ~iterator() {};
                iterator operator++();
                map<T,T> operator*();
                bool operator==(const iterator& x) const {return (_pos == x._pos);};
                bool operator!=(const iterator& x) const {return (!(*this == x));};
                
                friend class posibilidades;
            private:
                iterator(vector< vector<T> >* cuales, map<unsigned int,T>* que, map<T,T>* quien, const int p);
                iterator(const int p) {_pos = p;};
                vector<unsigned int> _donde;
                int _pos;
		int _sitio;
                vector< vector<T> >* _cuales;
		map<unsigned int,T>* _index;
		map<T,T>* _lafija;
        };
        
        iterator begin();
        iterator end();
    private:
        vector< vector<T> > _options;
	map<unsigned int,T> _lapos;
	map<T,T> _fijos;
	unsigned int _total;
};

#include "posibilidades.template"

#endif
