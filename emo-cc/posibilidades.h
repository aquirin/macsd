#ifndef _POS_
#define _POS_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <typename T>
class posibilidades {
    public:
        posibilidades(const map<T, vector<T> >& cuales, const multimap<T,pair<T,T> >& est, const multimap<T,pair<T,T> >& ins);
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
                iterator(vector< vector<T> >* cuales, map<unsigned int,T>* que, map<T,T>* quien, multimap<T,pair<T,T> >* est, multimap<T,pair<T,T> >* ins, const int p);
                iterator(const int p) {_pos = p;};
		
                vector<unsigned int> _donde;
                int _pos;
		int _sitio;
                vector< vector<T> >* _cuales;
		map<unsigned int,T>* _index;
		map<T,T>* _lafija;
		multimap<T,pair<T,T> >* _lins;
		multimap<T,pair<T,T> >* _lest;		
        };
        
        iterator begin();
        iterator end();
    private:
        vector< vector<T> > _options;
	map<unsigned int,T> _lapos;
	map<T,T> _fijos;
	unsigned int _total;
	
	multimap<T,pair<T,T> > _ins;
	multimap<T,pair<T,T> > _est;
};

#include "posibilidades.template"

#endif
