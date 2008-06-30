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

set< tuplax3<unsigned int> > shapes::ejesNoUtilizados() const {
    set< tuplax3<unsigned int> > lista;
    
    for (int i = 0; i < _grafo.size(); i++) {
        for (int j = 0; j < _grafo.size(); j++) {
            for (int k = 0; k < _grafo[0][0].size(); k++) {
                if (!_grafo[i][j][k]) {
                    lista.insert(tuplax3<unsigned int>(i+1, j+1, k+1));
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
