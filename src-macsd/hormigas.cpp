#include "hormigas.h"

//-------------------------------------------------------------------------
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0) :
	_costes(numObjetivos),
	_support(base.size(), 0),
	
#if VERSION == V_SHAPE
	_subestructura(base[0])
	{
        _subestructura.clear();	/* TODO: HARMONIZE (no 'clear' in the GO version) */
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
	_subestructura(SOLUTION("0",go(base[0].second.base_nodos(),base[0].second.base_ejes(),base[0].second.desc())))
	{
#endif

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

//------------------------------------------------------------------------
#if VERSION == V_SHAPE
Hormiga::Hormiga (const vector<SOLUTION>& base, const unsigned int numObjetivos, SOLUTION quien) : _subestructura(quien), _costes(numObjetivos), _support(base.size(), 0) {
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _costes(numObjetivos), _support(base.size(), 0) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
#endif

        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
#if (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
        _aparEje = ap;	/* TODO: HARMONIZE */
#endif
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
#if (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
        _subestructura = sub;
        
        set< CANDIDATE > ejes = _subestructura.second.ejes();
        for (set< CANDIDATE >::iterator q = ejes.begin(); q != ejes.end(); q++) {
//             _subestructura.second.agregarEje(nodo1, nodo2, enlace);
            unsigned int nodo1 = q->first;
            unsigned int nodo2 = q->second;
            unsigned int enlace = q->third;
#endif
        vector<unsigned int> temp;
        vector<unsigned int>::iterator p = _support.begin();

#if VERSION == V_SHAPE
        int x = 0;
        while (p != _support.end()) {
            // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
            cout << "Instancia: " << x++ << endl;
            if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
                vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
                posibilidades<unsigned int> op(v);
                
                bool done = false;
        
                for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
                    cout << "X" << endl;
                    SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
                    cout << "Y" << endl;
                    cout << "Esta: " << nueva_subestructura << endl;
                    cout << "En: " << _instancias[*p] << endl;
                    if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                        done = true;
                        cout << "SI" << endl;
                        temp.push_back(*p);
                    }
                }
            }
            cout << "FIN" << endl;
            p++;
        }
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            while (p != _support.end()) {
                if (_instancias[*p].second.ejeUsado(nodo1,nodo2,enlace)) {        
                    temp.push_back(*p);
                }
                p++;
            }
#endif

    
        _support = temp;
#if (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
        }
        calculaCostes();	/* TODO: HARMONIZE (not present in the shape version) */
#endif
}

//-------------------------------------------------------------------------
Hormiga::Hormiga(const Hormiga &unaHormiga): _subestructura(unaHormiga._subestructura) {
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

//-------------------------------------------------------------------------
void Hormiga::posicionaInicialmente() {
    // situar la hormiga en el principio de la asignacion (elijo un nodo)
    _ejesAsignados = 0;

    // actualizamos los costes
    _costeValido = false;

    // el conjunto de candidatos se inicializa
    _candidatos.clear();
#if VERSION == V_SHAPE
    _subestructura.clear();
    _subestructura.agregarNodo(1, "object");
    _candidatos = _subestructura.nodosNoUtilizados();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    _subestructura.second.clear();
    _candidatos = _subestructura.second.ejesNoUtilizados();
    
/*    _subestructura.first.clear();*/
#endif

        
    _support.resize(_instancias.size(), 0);
    for (unsigned int l = 0; l < _support.size(); l++)
        _support[l] = l;   
}

//-------------------------------------------------------------------------
#if VERSION == V_SHAPE
void Hormiga::avanza(const unsigned int nodo1, const string nodo2) {
    // avanzar la hormiga
//     cout << nodo1 << '=' << nodo1 << endl;
    _subestructura.agregarNodo(_subestructura.cantNodos() + 1, nodo2);
    unsigned int enlace;
    if (nodo2 == "object")
        enlace = 1;
    else
        enlace = 2;
    _subestructura.agregarEje(nodo1, _subestructura.cantNodos(), enlace);
    
    vector<unsigned int> temp;
    int pp = 1;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
        cout << "Instancia " << pp++ << endl;
        if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
            vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
            posibilidades<unsigned int> op(v);
            
            bool done = false;
    
            for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
                cout << "X" << endl;
                SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
                cout << "Y" << endl;
                cout << "Esta: " << nueva_subestructura << endl;
                cout << "En: " << _instancias[*p] << endl;
                if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                    done = true;
                    cout << "SI" << endl;
                    temp.push_back(*p);
                }
            }
            cout << "FIN" << endl;
        }
        p++;
    }

    _support = temp;
    _costeValido = false;         
    
    // aumentamos el numero de asignados si es que usamos nuevos
    _ejesAsignados++;
    
    // eliminamos de la listtuplax3a de candidatos
//     bool done = false;
//     if (enlace == 2) {
//         // Hay que eliminar todos los tipos de shapes asociados
//         do {
//             done = false;
//             for (vector< CANDIDATE >::iterator it = _candidatos.begin(); !done && (it != _candidatos.end()); it++)
//             if (((*it).first == nodo1) && ((*it).third == enlace)) {
//                 _candidatos.erase(it);
//                 done = true;
//             }
//         }
//         while (done);
//     }
//     else {
//         for (vector< CANDIDATE >::iterator it = _candidatos.begin(); !done && (it != _candidatos.end()); it++)
//             if (((*it).first == nodo1) && ((*it).second == nodo2) && ((*it).third == enlace)) {
//                 _candidatos.erase(it);
//                 done = true;
//             }
//     }
    _candidatos = _subestructura.nodosNoUtilizados();
}


#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)

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
    for (vector< CANDIDATE >::iterator it = _candidatos.begin(); !done && (it != _candidatos.end()); it++)
        if (((*it).first == nodo1) && ((*it).second == nodo2) && ((*it).third == enlace)) {
            _candidatos.erase(it);
            done = true;
        }
}

#endif



//-------------------------------------------------------------------------
bool Hormiga::Utilizado(const unsigned int id) const {
#if VERSION == V_SHAPE
    set<unsigned int> lista = _subestructura.nodosUtilizados();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    set<unsigned int> lista = _subestructura.second.nodosUtilizados();
#endif

    
    return (lista.find(id) != lista.end());
}
 
//-------------------------------------------------------------------------
#if VERSION == V_SHAPE

vector< CANDIDATE > Hormiga::getCandidatos() {
    return _subestructura.nodosNoUtilizados();
}

#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)

vector< CANDIDATE > Hormiga::getCandidatos() {
    vector< CANDIDATE > lista;
//     set< CANDIDATE > aux;
//     aux = _subestructura.second.ejesNoUtilizados();
    
/*    // Solo aquellos ejes que aparecen al menos una vez en la base de datos
    set< CANDIDATE >::iterator it = aux.begin();
    while (it != aux.end()) {
        bool found = false;
        for (unsigned int i = 0; !found && (i < _instancias.size()); i++)
            if (_instancias[i].second.ejeUsado((*it).first,(*it).second,(*it).third)) {
                 lista.push_back(*it);
                 found = true;
            }
        it++;
    }*/
    
//     set< CANDIDATE >::iterator it = aux.begin();
//     while (it != aux.end()) {
// //         cout << (*it).first << ' ' << (*it).second << endl;
//         lista.push_back(*it);
//         it++;
//     }
    
    // Solo me quedo con aquellos que tiene al menos uno de los nodos en la subestructura
    set<unsigned int> nu = _subestructura.second.nodosUtilizados();
//     for (set<unsigned int>::iterator p = nu.begin(); p != nu.end(); p++) cout << *p << endl;
    if (nu.size() > 0) {
        vector< CANDIDATE >::iterator it1 = _candidatos.begin();
        
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

#endif

//-------------------------------------------------------------------------
// This is my old version, non-optimized (Arnaud). TODO: Delete it or keep it?
#if VERSION == V_SCIENCEMAP
vector< CANDIDATE > Hormiga::getCandidatos_original() {
    vector< CANDIDATE > lista;
    set< CANDIDATE > aux = ((shapes)(_subestructura.second)).ejesNoUtilizados();	/* TODO: The return type of ejesNoUtilizados() has changed between go and shapes */
    //cout << "DEBUG getCandidatos " << aux.size();
    
    // Solo aquellos ejes que aparecen al menos una vez en la base de datos
    set< CANDIDATE >::iterator it = aux.begin();
    while (it != aux.end()) {
        bool found = false;
        for (unsigned int i = 0; !found && (i < _instancias.size()); i++)
            if (_instancias[i].second.ejeUsado((*it).first,(*it).second,(*it).third)) {
                 lista.push_back(*it);
                 found = true;
            }
        it++;
    }
    //cout << " " << lista.size();
    
    // Solo me quedo con aquellos que tiene al menos uno de los nodos en la subestructura
    set<unsigned int> nu = _subestructura.second.nodosUtilizados();
    //for (set<unsigned int>::iterator p = nu.begin(); p != nu.end(); p++) cout << *p << ' ';
    //cout << endl;
    if (nu.size() > 0) {
        vector< CANDIDATE >::iterator it1 = lista.begin();
        
        while (it1 != lista.end()) {
            if ((nu.find((*it1).first) == nu.end()) && (nu.find((*it1).second) == nu.end())) {
                lista.erase(it1);
            }
            else
                it1++;
        }
    }
    //cout << " " << lista.size() << endl;
        
    return lista;
}
#endif

//-------------------------------------------------------------------------
// This version is the optimized one. TODO: Check how it fusion with the new stuff of Rocio, delete it or keep it?
#if VERSION == V_SCIENCEMAP
vector< CANDIDATE > Hormiga::getCandidatos_optimized() {
    //return ((shapes)_subestructura.second).ejesNoUtilizadosButIn(_instancias);	/* TODO: The parameter has changed between shape and SOLUTION */
    vector< CANDIDATE > TODO_dummy;
    return TODO_dummy;
}
#endif

//-------------------------------------------------------------------------
// This contain debug instruction to test if the two versions (non-optimized + optimized) produce the same results: we just have to print what happen in the both case on the screen, and compare with a diff to validate the optimized version. TODO: Check the optimized version, and mergo or delete it.
#if VERSION == V_SCIENCEMAP
vector< CANDIDATE > Hormiga::getCandidatos_debug() {
    vector< CANDIDATE > v;
    //v = getCandidatos_original();
    v = getCandidatos_optimized();
    //cout << "DEBUG getCandidatos " << v.size() << endl;
    //printf("DEBUG list-getCandidatos");
    //for(int i=0;i<v.size();i++) printf(" (%d,%d,%d)", v[i].first, v[i].second, v[i].third);
    //printf("\n");
    return v;
}
#endif





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
#if VERSION == V_SHAPE

        _costes[1] = _subestructura.sizeNorm();
        cout << _subestructura << endl;

#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)

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

#endif


        cout << "Costo: " << _costes[0] << ' ' << _costes[1] << endl;
    }
}

//-------------------------------------------------------------------------
int Hormiga::dominancia(Hormiga& v, bool x, int y) {
    calculaCostes();
#if VERSION == V_SHAPE
    v.calculaCostes();		/* TODO: HARMONIZE (before 'v' was declared as const; and why x/y are not used??) */
#endif
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
#if VERSION == V_SHAPE
        _subestructura.clear();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
        _subestructura.second.clear();
#endif

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
#if VERSION == V_SHAPE

float Hormiga::getAparicionesEje(const CANDIDATE& eje) {
    CANDIDATE n = eje;
    n.first = 1;	/* TODO: HARMONIZE (not done in the GO version) */
//     cout << "AP: " << eje.first << ' ' << eje.second << '=' << (*_aparEje)[eje] << endl;
    assert(_aparEje->find(n) != _aparEje->end());
    
        
    return ((*_aparEje)[n]);
}

#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)

float Hormiga::getAparicionesEje(const CANDIDATE& eje) {
    assert(_aparEje->find(eje) != _aparEje->end());
//     cout << "AP: " << eje.first << ' ' << eje.second << '=' << (*_aparEje)[eje] << endl;
    return (*_aparEje)[eje];
}


#endif

