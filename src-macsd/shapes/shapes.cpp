///// DO NOT MODIFY shapes.cpp/h BUT shapes.*.cpp/h
///// AS THESE FILES WILL BE ERASED BY THE MAKEFILE

#include "shapes.h"

void shapes::agregarNodo(const unsigned int n, const string& s) {
    assert(_desc.find(n) == _desc.end());
 
    if (_nodo == 0)
        _nodo = n;
    _nodos.insert(n);
    _desc[n] = s;
//     cout << n << ' ' << _desc[n] << endl;
}

void shapes::agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    assert((_desc.find(ini) != _desc.end()) && (_desc.find(fin) != _desc.end()) && ((s == 1) || (s == 2)));
    
    if (_ejes.find(tuplax3<unsigned int>(ini,fin,s)) == _ejes.end()) {
        _ejes.insert(tuplax3<unsigned int>(ini,fin,s));
//         cout << _desc[ini] << ' ' << _desc[fin] << endl;
    }
    else {
        cout << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
    }
}

set<unsigned int> shapes::nodosUtilizados() const {
    return _nodos;
}

vector< CANDIDATE > shapes::nodosNoUtilizados() const {
    vector< CANDIDATE > lista;
    
    // Cantidad nodos maxima
    if (_nodos.size() < _mobj) {
        set<unsigned int> objetos;
        for (map<unsigned int, string>::const_iterator p = _desc.begin(); p != _desc.end(); p++) {
            if (p->second == "object") {
                objetos.insert(p->first);
                lista.push_back(CANDIDATE(p->first, "object"));
            }
        }
        
//         if (objetos.size() > _mobj) {
//             lista.clear();
//         }
        
        for (set<unsigned int>::iterator p = objetos.begin(); p != objetos.end(); p++) {
    //         Veo si tiene forma
            bool found = false;
            for (set< tuplax3<unsigned int> >::const_iterator q = _ejes.begin(); (q != _ejes.end()) && !found; q++){
                found = ((q->first == *p) && (q->third == 2));
                found = found || ((q->second == *p) && (q->third == 2));
            }
            if (!found) {
    //             Ejes de formas
                unsigned int j = _desc.size();
                for (unsigned int i = 0; i < _shap.size(); i++) {
                    lista.push_back(CANDIDATE(*p, _shap[i]));
                }
            }
        }
    }       
    return lista;
}

void shapes::clear() {
    _nodo = 0 ;
    _nodos.clear();
    _ejes.clear();
    _desc.clear();
}

bool shapes::empty() const {
    return ((_nodo == 0) && _nodos.empty() && _ejes.empty() && _desc.empty());
}

unsigned int shapes::size() const {
    return (_nodos.size() + _ejes.size());
}

void shapes::imprime(ostream &salida) const {
     salida << "Nodos: ";
     for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
         map<unsigned int, string>::const_iterator q = _desc.find(*p);
         salida << '(' << *p << ',' << (*q).second << ')';
     }
     salida << endl;
     salida << "Ejes: ";
     for (set< tuplax3<unsigned int> >::iterator p = _ejes.begin(); p != _ejes.end(); p++)
         salida << '(' << p->first << ',' << p->second << ',' << p->third << ')';
     salida << endl;
}

ostream& operator<<(ostream& os, const shapes& s) {
   s.imprime(os);
   return os;
}

bool shapes::operator==(const shapes& s) const {
    bool done = false;
//     cout << "EN==" << endl;
    if ((_nodos.size() == s._nodos.size()) && (_ejes.size() == s._ejes.size())) {
        shapes copia(s);
        vector< vector<unsigned int> > v = darPosibilidades(s);
        posibilidades<unsigned int> op(v);
            
        cout << "Veamos... " << endl;
        for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
            for (unsigned int j = 0; j < (*q).size(); j++)
                cout << (*q)[j] << ' ';
            cout << endl;
            shapes nueva_subestructura = copia.reasignarNodos(*q);
            cout << *this << endl << s << endl << nueva_subestructura << endl;
            done = this->igual(nueva_subestructura);
        }
    }
    
    return done;
}

bool shapes::igual(const shapes& s) const {
    return ((_nodo == s._nodo) && (_nodos == s._nodos) && (_ejes == s._ejes) && (_desc == s._desc));
}

vector< CANDIDATE > shapes::posibilidades_totales() {
    vector< CANDIDATE > v;
    
    for (unsigned int i = 0; i < _mobj; i++) {
        v.push_back(CANDIDATE(i+1,"object"));
        for (unsigned int j = 0; j < _shap.size(); j++) {
            v.push_back(CANDIDATE(i+1,_shap[j]));
        }
    }
    
    return v;
}

vector< CANDIDATE > shapes::posibilidades_reales() {
    vector< CANDIDATE > v;
    
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
        v.push_back(CANDIDATE(*p,_desc[*p]));
    }
    
    return v;
}


shapes shapes::reasignarNodos(vector<unsigned int> v) {
    shapes nuevo(_mobj, _shap);
    
    map<unsigned int, unsigned int> dicc;
            
    for (unsigned int i = 0; i < v.size(); i++) {
        set<unsigned int>::iterator p = _nodos.begin();
        for (unsigned int j = 0; j < v[i] - 1; j++) p++;
//         cout << v[i] << endl;
        nuevo.agregarNodo(i + 1, _desc[*p]);
        dicc[*p] = i + 1;
    }
    for (set< tuplax3<unsigned int> >::iterator p = _ejes.begin(); p != _ejes.end(); p++) {
        cout << "D " << dicc[p->first] << " " << dicc[p->second] << " " << p->third << endl;
        unsigned int uno, dos;
        if (dicc.find(p->first) != dicc.end())
            uno = dicc[p->first];
        else
            uno = p->first;
        if (dicc.find(p->second) != dicc.end())
            dos = dicc[p->second];
        else
            dos = p->second;
        nuevo.agregarEje(uno, dos, p->third);
    }
    
    return nuevo;
}

shapes shapes::reasignarNodosFijo(vector<unsigned int> v) {
    shapes nuevo(_mobj, _shap);
    
    map<unsigned int, unsigned int> dicc;
            
    unsigned int i = 0;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
        cout << *p << "-> " << v[i] << endl;
        nuevo.agregarNodo(v[i], _desc[*p]);
        dicc[*p] = v[i];
        i++;
    }
    for (set< tuplax3<unsigned int> >::iterator p = _ejes.begin(); p != _ejes.end(); p++) {
        cout << "D " << p->first << " " << p->second << endl;
        cout << "E " << dicc[p->first] << " " << dicc[p->second] << " " << p->third << endl;
        unsigned int uno, dos;
        if (dicc.find(p->first) != dicc.end())
            uno = dicc[p->first];
        else
            uno = p->first;
        if (dicc.find(p->second) != dicc.end())
            dos = dicc[p->second];
        else
            dos = p->second;
        nuevo.agregarEje(uno, dos, p->third);
    }
    
    return nuevo;
}

bool shapes::cubiertoPor(const shapes& s) const {
    bool res = true;
    
    for (set< tuplax3<unsigned int> >::const_iterator p = s._ejes.begin(); (p != s._ejes.end()) && res; p++) {
        map<unsigned int, string>::const_iterator v = _desc.find(p->second);
        map<unsigned int, string>::const_iterator w = s._desc.find(p->second);
        res = (ejeUsado(p->first, p->second, p->third) && (*v == *w));
    }
    
    return res;
}

vector< vector<unsigned int> > shapes::darPosibilidades(const shapes& s) const {
    vector< vector<unsigned int> > res(_nodos.size());
    
    unsigned int i = 0;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
        vector<unsigned int> w;
        for (set<unsigned int>::const_iterator q = s._nodos.begin(); q != s._nodos.end(); q++) {
            if ((*(_desc.find(*p))).second == (*(s._desc.find(*q))).second) {
                w.push_back(*q);
//                 cout << *q;// '+' << *(s._desc.find(*q)) << endl;
            }
        }
        res[i] = w;
        i++;
    }
    return res;
}
