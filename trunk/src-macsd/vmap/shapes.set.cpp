///// DO NOT MODIFY shapes.cpp/h BUT shapes.*.cpp/h
///// AS THESE FILES WILL BE ERASED BY THE MAKEFILE

#include "shapes.h"

/**************************************************\
Constructor: takes the estimated number of different
nodes and different edges.
\**************************************************/
shapes::shapes(const unsigned int x, const unsigned int l)
{
    _nodo = -1;
    _num_nodes = x;
    _num_edges = l;
}

/**************************************************\
\**************************************************/
shapes::shapes(const shapes& s)
{
    int i, j, k;
    _nodo = s._nodo;
    _num_nodes = s._num_nodes;
    _num_edges = s._num_edges;
    _nodes = s._nodes;
    _edges = s._edges;
}

/**************************************************\
\**************************************************/
shapes::~shapes()
{
}

/**************************************************\
Return the list of nodes for which it exists at least one edge.
The values are 1-based.
\**************************************************/
set<unsigned int> shapes::nodosUtilizados() const {
    set<unsigned int> candidatas;
    if (_nodo > -1) {
        candidatas.insert(_nodo+1);
	
	set< unsigned int >::iterator it = _nodes.begin();
	while (it != _nodes.end())
	{
		candidatas.insert((*it)+1);
		it++;
	}
    }
    return candidatas;
}

/**************************************************\
Return the list of edges <Node1,Node2,EdgeType> that
are NOT present in the graph.
The values are 1-based.
\**************************************************/
set< CANDIDATE > shapes::ejesNoUtilizados() const {
    set< CANDIDATE > lista;
    
    for (int i = 0; i < _num_nodes; i++) {
        for (int j = 0; j < _num_nodes; j++) {
            for (int k = 0; k < _num_edges; k++) {
                CANDIDATE p(i, j, k);
                if (_edges.find(p) == _edges.end()) {
                    lista.insert(CANDIDATE(i+1, j+1, k+1));
                }
            }
        }
    }
    return lista;
}

/**************************************************\
Return the list of edges <Node1,Node2,EdgeType> that
are NOT present in the graph, but for which Node1 and
Node2 are present in the graph, and present at least
one time in '_inst'.
The values are 1-based.
\**************************************************/
vector< CANDIDATE > shapes::ejesNoUtilizadosButIn(vector<shapes> _inst) const {
    vector< CANDIDATE > lista;
    set<unsigned int> nu = nodosUtilizados();
    
    for (int i = 0; i < _num_nodes; i++) {
        bool iUsed = ((nu.size()==0) || (nu.find(i+1) != nu.end()));
        for (int j = 0; j < _num_nodes; j++) {
	    bool jUsed = ((nu.size()==0) || (nu.find(j+1) != nu.end()));
            for (int k = 0; k < _num_edges; k++) {
                CANDIDATE p(i, j, k);
		// Keep the edges not found in the substructure, but having at least one node in it
                if ((iUsed || jUsed) && (_edges.find(p) == _edges.end())) {
		    bool found = false;
                   for (unsigned int m = 0; !found && (m < _inst.size()); m++)
                       if (_inst[m].ejeUsado(i+1,j+1,k+1)) {
		           // Keep the edges found in the DB '_inst'
			   lista.push_back(CANDIDATE(i+1, j+1, k+1));
                           found = true;
                       }
                }
            }
        }
    }
    return lista;
}

/**************************************************\
\**************************************************/
void shapes::clear() {
    _nodes.clear();
    _edges.clear();
    _nodo = -1;
}

/**************************************************\
Number of used edges + number of used nodes.
\**************************************************/
unsigned int shapes::size() const {
    unsigned int x = 0;
    x += _edges.size();
    x += nodosUtilizados().size();
    return x;
}

/**************************************************\
\**************************************************/
void shapes::imprime(ostream &salida) const {
     salida << "Nodo: " << _nodo + 1 << endl;
     salida << "Ejes: " << endl;
     
     set< tuplax3< unsigned int > >::iterator it = _edges.begin();
     while (it != _edges.end())
     {
          salida << (*it).first+1 << ' ' << (*it).second+1 << ' ' << (*it).third+1 <<endl;
	  it++;
     }
}

/**************************************************\
\**************************************************/
ostream& operator<<(ostream& os, const shapes& s) {
   s.imprime(os);
   return os;
}

/**************************************************\
\**************************************************/
bool shapes::ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {
    CANDIDATE p(ini-1, fin-1, s-1);
    bool b = (_edges.find(p) != _edges.end());
    return b;
}

/**************************************************\
Return true if the given node 'nod' is used (or is
equal to _nodo) for any edge.
'nod' is given 1-based.
\**************************************************/
bool shapes::nodoUsado(const unsigned int nod) const {
    assert(nod-1 < cantNodos());
    bool res;
    res = (_nodo == nod-1) || (_nodes.find(nod-1) != _nodes.end());
    return res;
}

/**************************************************\
Equality of two shapes.
\**************************************************/
bool shapes::operator==(const shapes& s) const {
    bool res = (size() == s.size());
    res = res && (_edges == s._edges);
    return res;
}

/**************************************************\
Add the given edge. 'ini': starting node, 'fin': ending
node, 's': edge type. 'ini', 'fin' and 's' are given
1-based.
\**************************************************/
void shapes::agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    set<unsigned int> x = nodosUtilizados();
     /*set<unsigned int>::iterator p = x.begin();
     cout << "nodosUtilizados()=";
     while (p != x.end()) {
         cout << *p << ' ';
         p++;
     }
     cout << endl << "ini=" << ini+1 << " fin=" << fin+1 << endl;*/
    assert((x.find(ini) != x.end()) || (x.find(fin) != x.end()));
    
    if(ini != fin)
    {
        _nodes.insert(ini-1);
        _nodes.insert(fin-1);
    }
    CANDIDATE p(ini-1, fin-1, s-1);
    _edges.insert(p);
};

/**************************************************\
\**************************************************/
shapes & shapes::operator= (const shapes & aShape) {
    if (this != &aShape) {
        // Copy the structure
        _nodo = aShape._nodo;
        _num_nodes = aShape._num_nodes;
        _num_edges = aShape._num_edges;
        _nodes = aShape._nodes;
        _edges = aShape._edges;
    }
    return *this;
}
