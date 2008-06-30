#include "hormigas.h"

//-------------------------------------------------------------------------
Hormiga::Hormiga (const unsigned int colonia, const vector<shapes>& base, const unsigned int numObjetivos = 2) : _subestructura(base[0].cantNodos(),base[0].cantEjes()), _costes(numObjetivos), _support(base.size(),true) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
        
        unsigned int suma = 0;
        for (unsigned int i = 0; i < _instancias[0].cantNodos(); i++)
            for (unsigned int j = 0; j < _instancias[0].cantNodos(); j++)
                for (unsigned int k = 0; k < _instancias[0].cantEjes(); k++)
                    for (unsigned int l = 0; l < _instancias.size(); l++)
                        if (_instancias[l].ejeUsado(i+1,j+1,k+1)) {
                            suma++;
                            tuplax3<unsigned int> p(i+1,j+1,k+1);
                            if (_aparEje.find(p) == _aparEje.end())
                                _aparEje[p] = 0;
                            else
                                _aparEje[p]++;
                        }
                        
        for (map<tuplax3<unsigned int>, double>::iterator p = _aparEje.begin(); p != _aparEje.end(); p++)
            (*p).second /= (suma * 1.);
}

//-------------------------------------------------------------------------
Hormiga::Hormiga(const Hormiga &unaHormiga) {
        _subestructura = unaHormiga._subestructura;
        _numObjetivos = unaHormiga._numObjetivos;
        _ejesAsignados = unaHormiga._ejesAsignados;
        _colonias = unaHormiga._colonias;
        _candidatos = unaHormiga._candidatos;
        _costeValido = unaHormiga._costeValido;
        _costes = unaHormiga._costes;
        _instancias = unaHormiga._instancias;
        _support = unaHormiga._support;
}

//-------------------------------------------------------------------------
void Hormiga::posicionaInicialmente(const vector<float>& proba) {
    // situar la hormiga en el principio de la asignacion (elijo un nodo)
    _ejesAsignados = 0;

    // actualizamos los costes
    _costeValido = false;

    // el conjunto de candidatos se inicializa     
    _candidatos.clear();
    
    _subestructura.clear();
    
    for (unsigned int l = 0; l < _support.size(); l++)
        _support[l] = true;
    
    // Elijo el primer nodo
    // con probabilidad proporcional a la suma de las feromonas que llegan a el

    float y = ((rand() * 1.) / (RAND_MAX * 1.)) * (proba[proba.size()-1]);
    unsigned int i;
    for (i = 0; (i < proba.size()) && (y > proba[i]); i++);
    i++;
    _subestructura.agregarNodo(i);
    for (unsigned int l = 0; l < _support.size(); l++)
        if (_support[l]) {
            set<unsigned int> x = _instancias[l].nodosUtilizados();
            if  (x.find(i) == x.end())
                _support[l] = false;
        }
}

//-------------------------------------------------------------------------
void Hormiga::avanza(const unsigned int nodo1, const unsigned int nodo2, const unsigned int enlace) {
    assert((nodo1 <= _subestructura.cantNodos()) && (nodo2 <= _subestructura.cantNodos()) && (enlace <= _subestructura.cantEjes()));
    
    // avanzar la hormiga
    _subestructura.agregarEje(nodo1, nodo2, enlace);
    for (unsigned int l = 0; l < _support.size(); l++)
        if (_support[l])
            if (!(_instancias[l].ejeUsado(nodo1,nodo2,enlace)))
                _support[l] = false;

    _costeValido = false;         
    
    // aumentamos el numero de asignados si es que usamos nuevos
    _ejesAsignados++;
    
    // eliminamos de la lista de candatos
    bool done = false;
    for (vector< tuplax3<unsigned int> >::iterator it = _candidatos.begin(); !done && (it != _candidatos.end()); it++)
        if (((*it).first == nodo1) && ((*it).second == nodo2) && ((*it).third == enlace)) {
            _candidatos.erase(it);
            done = true;
        }
}

//-------------------------------------------------------------------------
bool Hormiga::Utilizado(const unsigned int id) const {
    set<unsigned int> lista = _subestructura.nodosUtilizados();
    
    return (lista.find(id) != lista.end());
}
 
//-------------------------------------------------------------------------
vector< tuplax3<unsigned int> > Hormiga::getCandidatos() {
    vector< tuplax3<unsigned int> > lista;
    set< tuplax3<unsigned int> > aux = _subestructura.ejesNoUtilizados();
    
    // Solo aquellos ejes que aparecen al menos una vez en la base de datos
    set< tuplax3<unsigned int> >::iterator it = aux.begin();
    while (it != aux.end()) {
        bool found = false;
        for (unsigned int i = 0; !found && (i < _instancias.size()); i++)
            if (_instancias[i].ejeUsado((*it).first,(*it).second,(*it).third)) {
                 lista.push_back(*it);
                 found = true;
            }
        it++;
    }
    
    // Solo me quedo con aquellos que tiene al menos uno de los nodos en la subestructura
    set<unsigned int> nu = _subestructura.nodosUtilizados();
    for (set<unsigned int>::iterator p = nu.begin(); p != nu.end(); p++) cout << *p << ' ';
    if (nu.size() > 0) {
        vector< tuplax3<unsigned int> >::iterator it1 = lista.begin();
        
        while (it1 != lista.end()) {
            if ((nu.find((*it1).first) == nu.end()) && (nu.find((*it1).second) == nu.end())) {
                lista.erase(it1);
            }
            else
                it1++;
        }
    }
        
    return lista;
}

//-------------------------------------------------------------------------
float Hormiga::getCoste(const unsigned int id) {
    assert(id < _numObjetivos);
    
    calculaCostes();    
    return _costes[id];
}


//-------------------------------------------------------------------------
void Hormiga::calculaCostes() {
    if (!_costeValido) {
        _costeValido = true;
        
        // Calculo costes
        // Esto se instancia en cada subclase particular
        
        // 0 = support
        _costes[0] = 0;
        for (unsigned int i = 0; i < _support.size(); i++)
            if (_support[i]) {
                _costes[0]++;
                cout << i+1 << ' ';
            }
        _costes[0] /= _instancias.size();
        // 1 = complexity
        _costes[1] = (_subestructura.size() * 1.) / ((_subestructura.cantNodos()*_subestructura.cantNodos()*_subestructura.cantEjes()) * 1.);
        
        cout << _subestructura;
        cout << "Costo: " << _costes[0] << ' ' << _costes[1] << endl;
    }
}

//-------------------------------------------------------------------------
int Hormiga::dominancia(const Hormiga& v, bool x, int y) {
    calculaCostes();
    
    int que;
    
//     cout << "A: " << _costes[0] << ' ' << _costes[1] << endl;
//     cout << "B: " << v._costes[0] << ' ' << v._costes[1] << endl;
    if ((_costes[0] >= v._costes[0]) && (_costes[1] >= v._costes[1]) && ((_costes[0] > v._costes[0]) || (_costes[1] > v._costes[1]))) {
        que = 1;
    }
    else if ((v._costes[0] >= _costes[0]) && (v._costes[1] >= _costes[1]) && ((v._costes[0] > _costes[0]) || (v._costes[1] > _costes[1]))) {
        que = -1;
    }
    else que = 0;
    
    if ((_costes[0] != 0) && (v._costes[0] != 0)) {
        // Nichos genotipicos
        unsigned int inte = 0;
        unsigned int unio = 0;
        for (unsigned int i = 0; i < _support.size(); i++)
            for (unsigned int j = 0; j < v._support.size(); j++) {
                if ((_support[i] == _support[j]) && (_support[i])) {
                    inte++;
                }
                if (_support[i] || _support[j])
                    unio++;
            }
                
        // Jaccard 0.5
        if (((inte * 1.) / (unio * 1.)) < 0.5) {
            que = 0;
        }
    }
    else {
        if (_costes[0] == 0)
            que = -1;
        else
            que = 1;
    }
//     cout << "QUE: " << que << endl;
    return que;
}

//-------------------------------------------------------------------------
Hormiga & Hormiga::operator= (const Hormiga & unaHormiga) {
    if (this != &unaHormiga) {
        _subestructura.clear();
        _candidatos.clear();
        _instancias.clear();
        _support.clear();
        _colonias.clear();
        
        _subestructura = unaHormiga._subestructura;
        _numObjetivos = unaHormiga._numObjetivos;
        _ejesAsignados = unaHormiga._ejesAsignados;
        _colonias = unaHormiga._colonias;
        _candidatos = unaHormiga._candidatos;
        _costeValido = unaHormiga._costeValido;
        _costes = unaHormiga._costes;
        _instancias = unaHormiga._instancias;
        _support = unaHormiga._support;
    }
    return *this;
}

//-------------------------------------------------------------------------
float Hormiga::getAparicionesEje(const tuplax3<unsigned int>& eje) {
    assert(_aparEje.find(eje) != _aparEje.end());
    
    return _aparEje[eje];
}
