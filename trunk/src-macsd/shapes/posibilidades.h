#ifndef _POS_
#define _POS_

#include <iostream>
#include <vector>
#include <map>

using namespace std;

template <typename T>
class posibilidades {
    public:
        posibilidades(const vector< vector<T> >& cuales);
        ~posibilidades() {};
        
        class iterator {
            public:
                iterator() {};
                ~iterator() {};
                iterator operator++();
                vector<T> operator*();
                bool operator==(const iterator& x) const {return (_pos == x._pos);};
                bool operator!=(const iterator& x) const {return (!(*this == x));};
                bool valid() const;
                
                friend class posibilidades;
            private:
                iterator(vector< vector<T> >* cuales, const int p);
                iterator(const int p) {_pos = p;};
                vector<unsigned int> _donde;
                int _pos;
                vector< vector<T> >* _cuales;
        };
        
        iterator begin();
        iterator end();
    private:
        vector< vector<T> > _options;
};

#include "posibilidades.template"

#endif
