///// DO NOT MODIFY shapes.cpp/h BUT shapes.*.cpp/h
///// AS THESE FILES WILL BE ERASED BY THE MAKEFILE

#include "shapes.h"

set<unsigned int> shapes::nodosUtilizados() const {
    set<unsigned int> candidatas;
    if (_nodo > -1) {
        candidatas.insert(_nodo+1);
        for (unsigned int i = 0; i < _grafo.size(); i++) {
            bool full = false;
            for (unsigned int j = 0; !full && (j < _grafo.size()); j++) {
                if (i != j) {
                    for (unsigned int l = 0; !full && (l < _grafo[0][0].size()); l++) {
                        full = _grafo[i][j][l];
                    }
                }
            }
            if (full) {
                candidatas.insert(i+1);
            }
        }
        
        for (unsigned int i = 0; i < _grafo.size(); i++) {
            bool full = false;
            for (unsigned int j = 0; !full && (j < _grafo.size()); j++) {
                if (i != j) {
                    for (unsigned int l = 0; !full && (l < _grafo[0][0].size()); l++) {
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
Return the list of edges <Node1,Node2,EdgeType> that
are NOT present in the graph.
The values are 1-based.
\**************************************************/
set< CANDIDATE > shapes::ejesNoUtilizados() const {
    set< CANDIDATE > lista;
    
    for (int i = 0; i < _grafo.size(); i++) {
        for (int j = 0; j < _grafo.size(); j++) {
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                if (!_grafo[i][j][k]) {
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
    
    for (int i = 0; i < _grafo.size(); i++) {
        bool iUsed = ((nu.size()==0) || (nu.find(i+1) != nu.end()));
        for (int j = 0; j < _grafo.size(); j++) {
	    bool jUsed = ((nu.size()==0) || (nu.find(j+1) != nu.end()));
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                CANDIDATE p(i, j, k);
		// Keep the edges not found in the substructure, but having at least one node in it
                if ((iUsed || jUsed) && (!_grafo[i][j][k])) {
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

void shapes::clear() {
    for (int i = 0; i < _grafo.size(); i++) {
        for (int j = 0; j < _grafo.size(); j++) {
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                _grafo[i][j][k] = false;
            }
        }
    }
    _nodo = -1;
}

unsigned int shapes::size() const {
    unsigned int x = 0;
    
    for (int i = 0; i < _grafo.size(); i++) {
        for (int j = 0; j < _grafo.size(); j++) {
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                if (_grafo[i][j][k]) {
                    x++;
                }
            }
        }
    }
    
    x = x + nodosUtilizados().size();
    return x;
}

void shapes::imprime(ostream &salida) const {
     salida << "Nodo: " << _nodo + 1 << endl;
     salida << "Ejes: " << endl;
     for (int i = 0; i < _grafo.size(); i++) {
        for (int j = 0; j < _grafo.size(); j++) {
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                if (_grafo[i][j][k]) {
                    salida << i + 1 << ' ' << j + 1 << ' ' << k + 1 <<endl;
                }
            }
        }
    }
}

ostream& operator<<(ostream& os, const shapes& s) {
   s.imprime(os);
   return os;
}

bool shapes::nodoUsado(const unsigned int nod) const {
    assert(nod-1 < cantNodos());
    
    bool res = (_nodo == nod-1);
    if (!res) {
        for (unsigned int i = 0; !res && (i < _grafo.size()); i++)
            if (i != nod)
                for (unsigned int j = 0; !res && (j < _grafo[0][0].size()); j++)
                    res = _grafo[nod-1][i][j] || _grafo[i][nod-1][j];
    }
    return res;
}

bool shapes::operator==(const shapes& s) const {
    bool res = (size() == s.size());
    for (int i = 0; res && (i < _grafo.size()); i++) {
        for (int j = 0; (j < _grafo.size()) && res; j++) {
            for (int k = 0; (k < _grafo[0][0].size()) && res; k++) {
                res = (_grafo[i][j][k] == s._grafo[i][j][k]);
            }
        }
    }
    return res;
}

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
    
    _grafo[ini-1][fin-1][s-1] = true;
};
