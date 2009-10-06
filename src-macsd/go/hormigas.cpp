#include "hormigas.h"


//-------------------------------------------------------------------------
Hormiga::Hormiga (const unsigned int colonia, const vector< pair<string,go> >& base, const unsigned int numObjetivos = 2, map<tuplax3<unsigned int>, double>* ap = 0) : _subestructura(pair<string,go>("0",go(base[0].second.base_nodos(),base[0].second.base_ejes(), base[0].second.desc()))), _costes(numObjetivos), _support(base.size(), 0) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
        _aparEje = ap;
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
//         cout << "AAAAAAAAAAAAAA" << base.size() << endl;
}

//-------------------------------------------------------------------------
Hormiga::Hormiga (const unsigned int colonia, const vector< pair<string,go> >& base, const unsigned int numObjetivos = 2, map<tuplax3<unsigned int>, double>* ap = 0, pair<string,go> sub = pair<string,go>()) : _costes(numObjetivos), _support(base.size(), 0) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
        _aparEje = ap;
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
        _subestructura = sub;
        
        set< tuplax3<unsigned int> > ejes = _subestructura.second.ejes();
        for (set< tuplax3<unsigned int> >::iterator q = ejes.begin(); q != ejes.end(); q++) {
//             _subestructura.second.agregarEje(nodo1, nodo2, enlace);
            vector<unsigned int> temp;
            unsigned int nodo1 = q->first;
            unsigned int nodo2 = q->second;
            unsigned int enlace = q->third;
            vector<unsigned int>::iterator p = _support.begin();
            while (p != _support.end()) {
                if (_instancias[*p].second.ejeUsado(nodo1,nodo2,enlace)) {        
                    temp.push_back(*p);
                }
                p++;
            }
            _support = temp;
        }
        calculaCostes();
}

//-------------------------------------------------------------------------
Hormiga::Hormiga(const Hormiga &unaHormiga) {
        _numObjetivos = unaHormiga._numObjetivos;
        _ejesAsignados = unaHormiga._ejesAsignados;
        _colonias = unaHormiga._colonias;
        _candidatos = unaHormiga._candidatos;
        _costeValido = unaHormiga._costeValido;
        _costes = unaHormiga._costes;
        _instancias = unaHormiga._instancias;
        _support = unaHormiga._support;
        _aparEje = unaHormiga._aparEje;
        _subestructura = unaHormiga._subestructura;
}

//-------------------------------------------------------------------------
void Hormiga::posicionaInicialmente() {
    // situar la hormiga en el principio de la asignacion (elijo un nodo)
    _ejesAsignados = 0;

    // actualizamos los costes
    _costeValido = false;

    // el conjunto de candidatos se inicializa
    _candidatos.clear();
    _subestructura.second.clear();
    _candidatos = _subestructura.second.ejesNoUtilizados();
    
/*    _subestructura.first.clear();*/
    
    _support.resize(_instancias.size(), 0);
    for (unsigned int l = 0; l < _support.size(); l++)
        _support[l] = l;   
}

//-------------------------------------------------------------------------
void Hormiga::avanza(const unsigned int nodo1, const unsigned int nodo2, const unsigned int enlace) {
    // avanzar la hormiga
//     cout << nodo1 << '=' << nodo1 << endl;
    _subestructura.second.agregarEje(nodo1, nodo2, enlace);
    
    vector<unsigned int> temp;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        if (_instancias[*p].second.ejeUsado(nodo1,nodo2,enlace)) {        
            temp.push_back(*p);
        }
        p++;
    }

    _support = temp;
    _costeValido = false;         
    
    // aumentamos el numero de asignados si es que usamos nuevos
    _ejesAsignados++;
    
    // eliminamos de la lista de candidatos
    bool done = false;
    for (vector< tuplax3<unsigned int> >::iterator it = _candidatos.begin(); !done && (it != _candidatos.end()); it++)
        if (((*it).first == nodo1) && ((*it).second == nodo2) && ((*it).third == enlace)) {
            _candidatos.erase(it);
            done = true;
        }
}

//-------------------------------------------------------------------------
bool Hormiga::Utilizado(const unsigned int id) const {
    set<unsigned int> lista = _subestructura.second.nodosUtilizados();
    
    return (lista.find(id) != lista.end());
}
 
//-------------------------------------------------------------------------
vector< tuplax3<unsigned int> > Hormiga::getCandidatos() {
    vector< tuplax3<unsigned int> > lista;
//     set< tuplax3<unsigned int> > aux;
//     aux = _subestructura.second.ejesNoUtilizados();
    
/*    // Solo aquellos ejes que aparecen al menos una vez en la base de datos
    set< tuplax3<unsigned int> >::iterator it = aux.begin();
    while (it != aux.end()) {
        bool found = false;
        for (unsigned int i = 0; !found && (i < _instancias.size()); i++)
            if (_instancias[i].second.ejeUsado((*it).first,(*it).second,(*it).third)) {
                 lista.push_back(*it);
                 found = true;
            }
        it++;
    }*/
    
//     set< tuplax3<unsigned int> >::iterator it = aux.begin();
//     while (it != aux.end()) {
// //         cout << (*it).first << ' ' << (*it).second << endl;
//         lista.push_back(*it);
//         it++;
//     }
    
    // Solo me quedo con aquellos que tiene al menos uno de los nodos en la subestructura
    set<unsigned int> nu = _subestructura.second.nodosUtilizados();
//     for (set<unsigned int>::iterator p = nu.begin(); p != nu.end(); p++) cout << *p << endl;
    if (nu.size() > 0) {
        vector< tuplax3<unsigned int> >::iterator it1 = _candidatos.begin();
        
        while (it1 != _candidatos.end()) {
//             cout << (*it1).first << ' ' << (*it1).second << endl;
//             if ((nu.find((*it1).first) == nu.end()) && (nu.find((*it1).second) == nu.end())) {
//             }
//             else {
            if (nu.find((*it1).second) != nu.end())
                lista.push_back(*it1);
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
        _costes[0] = ((_support.size() * 1.) / (_instancias.size() * 1.));
        
        // 1 = complexity
//         _costes[1] = _subestructura.second.sizeNorm();
        
        float total = 0.;
        for (unsigned int i = 0; i < _support.size(); i++) {
            total += (1. - _instancias[_support[i]].second.nivelRelativo(subEst().second));

        }
        
        if (total > 0)
            _costes[1] = (total / (_support.size() * 1.));
        else
            _costes[1] = 0;
            
        cout << _subestructura.second << endl;
        cout << "Costo: " << _costes[0] << ' ' << _costes[1] << endl;
    }
}

//-------------------------------------------------------------------------
int Hormiga::dominancia(const Hormiga& v, bool x, int y) {
    calculaCostes();
//     v.calculaCostes();
    
    int que;
    
    cout << "A: " << _costes[0] << ' ' << _costes[1] << endl;
    for (unsigned int i = 0; i < _support.size(); i++) {
        cout << _support[i] << ' ';
    }
    cout << endl;
    cout << "B: " << v._costes[0] << ' ' << v._costes[1] << endl;
    for (unsigned int i = 0; i < v._support.size(); i++) {
        cout << v._support[i] << ' ';
    }
    cout << endl;
    
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
        unsigned int unio = (_support.size() + v._support.size());
        for (unsigned int i = 0; i < _support.size(); i++) {
            bool found = false;
            for (unsigned int j = 0; (j < v._support.size()) && !found; j++) {
                if (_support[i] == v._support[j]) {
                    found = true;
                    inte++;
                }
            }
        }
                
        unio -= (inte);
        // Jaccard 0.5
        if (((inte * 1.) / (unio * 1.)) < 0.5) {
            que = 0;
            cout << "ND" << endl;
        }
    }
    else {
        if (_costes[0] == 0)
            que = -1;
        else
            que = 1;
    }
    cout << "QUE: " << que << endl;
    return que;
}

//-------------------------------------------------------------------------
Hormiga & Hormiga::operator= (const Hormiga & unaHormiga) {
    if (this != &unaHormiga) {
        _subestructura.second.clear();
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
        _aparEje = unaHormiga._aparEje;
    }
    return *this;
}

//-------------------------------------------------------------------------
float Hormiga::getAparicionesEje(const tuplax3<unsigned int>& eje) {
    assert(_aparEje->find(eje) != _aparEje->end());
//     cout << "AP: " << eje.first << ' ' << eje.second << '=' << (*_aparEje)[eje] << endl;
    return (*_aparEje)[eje];
}
