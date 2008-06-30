#include "shapes.h"

/**************************************************\
\**************************************************/
shapes::shapes(const unsigned int x, const unsigned int l)
{
    _grafo = MALLOC3(x, x, l, char);
    _nodo = -1;
    num_nodes = x;
    num_edges = l;
}

/**************************************************\
\**************************************************/
shapes::shapes(const shapes& s)
{
    int i, j, k;
    _nodo = s._nodo;
    num_nodes = s.num_nodes;
    num_edges = s.num_edges;
    _grafo = MALLOC3(num_nodes, num_nodes, num_edges, char);
    for(i=0;i<num_nodes;i++)
      for(j=0;j<num_nodes;j++)
        for(k=0;k<num_edges;k++)
          _grafo[i][j][k] = s._grafo[i][j][k];
}

/**************************************************\
\**************************************************/
shapes::~shapes()
{
	FREE3(_grafo, num_nodes, num_nodes, num_edges);
}

/**************************************************\
Return the list of nodes for which it exists at least one edge.
The values are 1-based.
\**************************************************/
set<unsigned int> shapes::nodosUtilizados() const {
    set<unsigned int> candidatas;
    if (_nodo > -1) {
        candidatas.insert(_nodo+1);
        for (unsigned int i = 0; i < num_nodes; i++) {
            char full = 0;
            for (unsigned int j = 0; !full && (j < num_nodes); j++) {
                if (i != j) {
                    for (unsigned int l = 0; !full && (l < num_edges); l++) {
                        full = _grafo[i][j][l];
                    }
                }
            }
            if (full) {
                candidatas.insert(i+1);
            }
        }
        
        for (unsigned int i = 0; i < num_nodes; i++) {
            char full = 0;
            for (unsigned int j = 0; !full && (j < num_nodes); j++) {
                if (i != j) {
                    for (unsigned int l = 0; !full && (l < num_edges); l++) {
                        full = _grafo[j][i][l];
                    }
                }
            }
            if (full) {
                candidatas.insert(i+1);
            }
        }
    }
    return candidatas;
}

/**************************************************\
Return the list of edges <Node1,Node2,EdgeType> that are NOT present in the graphs.
The values are 1-based.
\**************************************************/
set< tuplax3<unsigned int> > shapes::ejesNoUtilizados() const {
    set< tuplax3<unsigned int> > lista;
    
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            for (int k = 0; k < num_edges; k++) {
                if (!_grafo[i][j][k]) {
                    lista.insert(tuplax3<unsigned int>(i+1, j+1, k+1));
                }
            }
        }
    }
    return lista;
}

/**************************************************\
\**************************************************/
void shapes::clear() {
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            for (int k = 0; k < num_edges; k++) {
                _grafo[i][j][k] = 0;
            }
        }
    }
    _nodo = -1;
}

/**************************************************\
Number of used edges + number of used nodes.
\**************************************************/
unsigned int shapes::size() const {
    unsigned int x = 0;
    
    for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            for (int k = 0; k < num_edges; k++) {
                if (_grafo[i][j][k]) {
                    x++;
                }
            }
        }
    }
    
    x = x + nodosUtilizados().size();
    return x;
}

/**************************************************\
\**************************************************/
void shapes::imprime(ostream &salida) const {
     salida << "Nodo: " << _nodo + 1 << endl;
     salida << "Ejes: " << endl;
     for (int i = 0; i < num_nodes; i++) {
        for (int j = 0; j < num_nodes; j++) {
            for (int k = 0; k < num_edges; k++) {
                if (_grafo[i][j][k]) {
                    salida << i + 1 << ' ' << j + 1 << ' ' << k + 1 <<endl;
                }
            }
        }
    }
}

/**************************************************\
\**************************************************/
ostream& operator<<(ostream& os, const shapes& s) {
   s.imprime(os);
   return os;
}

/**************************************************\
Return true if the given node 'nod' is used (or is
equal to _nodo) for any edge.
\**************************************************/
bool shapes::nodoUsado(const unsigned int nod) const {
    assert(nod-1 < cantNodos());
    
    bool res = (_nodo == nod-1);
    if (!res) {
        for (unsigned int i = 0; !res && (i < num_nodes); i++)
            if (i != nod)
                for (unsigned int j = 0; !res && (j < num_edges); j++)
                    res = _grafo[nod-1][i][j] || _grafo[i][nod-1][j];
    }
    return res;
}

/**************************************************\
Equality of two shapes.
\**************************************************/
bool shapes::operator==(const shapes& s) const {
    bool res = (size() == s.size());
    for (int i = 0; res && (i < num_nodes); i++) {
        for (int j = 0; (j < num_nodes) && res; j++) {
            for (int k = 0; (k < num_edges) && res; k++) {
                res = (_grafo[i][j][k] == s._grafo[i][j][k]);
            }
        }
    }
    return res;
}

/**************************************************\
Add the given edge. 'ini': starting node, 'fin': ending
node, 's': edge type.
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
    
    _grafo[ini-1][fin-1][s-1] = 1;
};

/**************************************************\
\**************************************************/
shapes & shapes::operator= (const shapes & aShape) {
    if (this != &aShape) {
        int i, j, k;
        // Free the memory if needed
        if(_grafo != NULL)
          FREE3(_grafo, num_nodes, num_nodes, num_edges);
        // Copy the structure
        _nodo = aShape._nodo;
        num_nodes = aShape.num_nodes;
        num_edges = aShape.num_edges;
        _grafo = MALLOC3(num_nodes, num_nodes, num_edges, char);
        for(i=0;i<num_nodes;i++)
          for(j=0;j<num_nodes;j++)
            for(k=0;k<num_edges;k++)
              _grafo[i][j][k] = aShape._grafo[i][j][k];
    }
    return *this;
}
