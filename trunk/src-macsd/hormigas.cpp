#include "hormigas.h"

//-------------------------------------------------------------------------
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0) :
	_costes(numObjetivos, 0.0),
	_support(base.size(), 0),
	
#if VERSION == V_SHAPE
	_subestructura(base[0])
	{
        _subestructura.clear();	/* TODO: HARMONIZE (no 'clear' in the GO version) */
#elif VERSION == V_GO
	_subestructura(SOLUTION("0", base[0].base_nodos(),base[0].base_ejes(),base[0].desc()))
	{
#elif (VERSION == V_SCIENCEMAP) || (VERSION == V_WWW)
	_subestructura(base[0])
	{
	_subestructura.clear();
#endif

        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
        _aparEje = ap;
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
	#if (VERSION == V_GO) || (VERSION == V_SCIENCEMAP) || (VERSION == V_WWW)
	    _candidatos = _subestructura.ejesNoUtilizados(); // Todos los posibles ejes
	    calculaCostes();
	#endif
}

//------------------------------------------------------------------------

#if VERSION == V_GO
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _costes(numObjetivos), _support(base.size(), false) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
	
// 	cout << "La base" << endl;
// 	for (vector<SOLUTION>::const_iterator it = _instancias.begin(); it != _instancias.end(); ++it)
// 	  cout << *it << endl;
	
        _aparEje = ap;
        
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
	vector<unsigned int> temp;
	
        _subestructura = sub;
        
	for (vector<unsigned int>::iterator p = _support.begin(); p != _support.end(); ++p) {
	  bool found = true;
	  set< CANDIDATE > ejes = _subestructura.ejes();
	  for (set< CANDIDATE >::iterator q = ejes.begin(); (q != ejes.end()) and found; q++) {
            unsigned int nodo1 = q->first;
            unsigned int nodo2 = q->second;
            unsigned int enlace = q->third;
	   
            found = _instancias[*p].ejeUsado(nodo1,nodo2,enlace);
	  }
	  if (found)
	    temp.push_back(*p);
        }
	
	_support = temp;
	_candidatos = _subestructura.ejesNoUtilizados(); // Todos los posibles ejes
 	_candidatos = getCandidatos();
	cout << "H: " << _candidatos.size() << endl;
        calculaCostes();
}
#endif

//------------------------------------------------------------------------

#if VERSION == V_SCIENCEMAP
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _costes(numObjetivos), _support(base.size(), false) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
	
// 	cout << "La base" << endl;
// 	for (vector<SOLUTION>::const_iterator it = _instancias.begin(); it != _instancias.end(); ++it)
// 	  cout << *it << endl;
	
        _aparEje = ap;
        
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
	vector<unsigned int> temp;
	
        _subestructura = sub;
        
	for (vector<unsigned int>::iterator p = _support.begin(); p != _support.end(); ++p) {
	  bool found = true;
	  set< CANDIDATE > ejes = _subestructura.ejes();
	  for (set< CANDIDATE >::iterator q = ejes.begin(); (q != ejes.end()) and found; q++) {
            unsigned int nodo1 = q->first;
            unsigned int nodo2 = q->second;
            unsigned int enlace = q->third;
	   
	    if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
//                 cout << _instancias[*p] << endl;
                vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
                posibilidades<unsigned int> op(v);
                
//                 for (int oo = 0; oo < v.size(); oo++) {
//                     for (int pp = 0; pp < v[oo].size(); pp++)
//                         cout << v[oo][pp] << ",";
//                     cout << endl;
//                 }
                
                bool done = false;
        
                for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//                     cout << "X" << endl;
                    SOLUTION copia(_subestructura);
                    SOLUTION nueva_subestructura = copia.reasignarNodosFijo(*q);
//                     cout << "Y" << endl;
//                     cout << "Esta: " << nueva_subestructura << endl;
//                     cout << "En: " << _instancias[*p] << endl;
                    if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                        done = true;
//                         cout << "SI" << endl;
//                         temp.push_back(*p);
                    }
                }
		found = done;
            }	  
	  }
	  if (found)
	    temp.push_back(*p);
        }
	
        _support = temp;
	
	_candidatos = _subestructura.ejesNoUtilizados();
	cout << "H: " << _candidatos.size() << endl;
        calculaCostes();
}
#endif

//------------------------------------------------------------------------

#if VERSION == V_WWW
Hormiga::Hormiga (const unsigned int colonia, const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _costes(numObjetivos), _support(base.size(), false) {
        _ejesAsignados = 0;
        _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
	
// 	cout << "La base" << endl;
// 	for (vector<SOLUTION>::const_iterator it = _instancias.begin(); it != _instancias.end(); ++it)
// 	  cout << *it << endl;
	
        _aparEje = ap;
        
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
	vector<unsigned int> temp;
	
        _subestructura = sub;
        
	for (vector<unsigned int>::iterator p = _support.begin(); p != _support.end(); ++p) {
// 	  bool found = true;
// 	  set< CANDIDATE > ejes = _subestructura.ejes();
// 	  for (set< CANDIDATE >::iterator q = ejes.begin(); (q != ejes.end()) and found; q++) {
//             unsigned int nodo1 = q->first;
//             unsigned int nodo2 = q->second;
//             string enlace = q->third;
// 	   
// 	    if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
// //                 cout << _instancias[*p] << endl;
//                 vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
//                 posibilidades<unsigned int> op(v);
//                 
// //                 for (int oo = 0; oo < v.size(); oo++) {
// //                     for (int pp = 0; pp < v[oo].size(); pp++)
// //                         cout << v[oo][pp] << ",";
// //                     cout << endl;
// //                 }
//                 
//                 bool done = false;
//         
//                 for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
// //                     cout << "X" << endl;
//                     SOLUTION copia(_subestructura);
//                     SOLUTION nueva_subestructura = copia.reasignarNodosFijo(*q);
// //                     cout << "Y" << endl;
// //                     cout << "Esta: " << nueva_subestructura << endl;
// //                     cout << "En: " << _instancias[*p] << endl;
//                     if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
cout << "Verificar: " << *p << endl;
cout << _subestructura << endl;
                    if (_instancias[*p].cubiertoPor(_subestructura)) {
//                         done = true;
                        cout << "SI" << endl;
                        temp.push_back(*p);
                    }
//                 }
// 		found = done;
//             }	  
// 	  }
// 	  if (found)
// 	    temp.push_back(*p);
        }
	
        _support = temp;
	
	_candidatos = _subestructura.ejesNoUtilizados();
	cout << "H: " << _candidatos.size() << endl;
        calculaCostes();
}
#endif

//-------------------------------------------------------------------------

#if VERSION == V_SHAPE
Hormiga::Hormiga (const vector<SOLUTION>& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _subestructura(sub), _costes(numObjetivos, 0.0), _support(base.size(), false) {
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
	
// 	cout << "La base" << endl;
// 	for (vector<SOLUTION>::const_iterator it = _instancias.begin(); it != _instancias.end(); ++it)
// 	  cout << *it << endl;
	
        _aparEje = ap;
        
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
	vector<unsigned int> temp;
	
        vector<unsigned int>::iterator p = _support.begin();

        int x = 1;
        while (p != _support.end()) {
            // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
//             cout << "Instancia: " << x++ << endl;
            if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
//                 cout << _instancias[*p] << endl;
                vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
                posibilidades<unsigned int> op(v);
                
//                 for (int oo = 0; oo < v.size(); oo++) {
//                     for (int pp = 0; pp < v[oo].size(); pp++)
//                         cout << v[oo][pp] << ",";
//                     cout << endl;
//                 }
                
                bool done = false;
        
                for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//                     cout << "X" << endl;
                    SOLUTION copia(_subestructura);
                    SOLUTION nueva_subestructura = copia.reasignarNodosFijo(*q);
//                     cout << "Y" << endl;
//                     cout << "Esta: " << nueva_subestructura << endl;
//                     cout << "En: " << _instancias[*p] << endl;
                    if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                        done = true;
//                         cout << "SI" << endl;
                        temp.push_back(*p);
                    }
                }
            }
//             cout << "FIN" << endl;
            p++;
        }

        _support = temp;
	
    _candidatos = _subestructura.nodosNoUtilizados();

        calculaCostes();
}
#endif

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
    _subestructura.clear();
    
#if VERSION == V_SHAPE
    _subestructura.agregarNodo(1, "object");
    _candidatos = _subestructura.nodosNoUtilizados();
#elif VERSION == V_GO
    _candidatos = _subestructura.ejesNoUtilizados();
#elif VERSION == V_SCIENCEMAP
    _subestructura.azar();
    _candidatos = _subestructura.ejesNoUtilizados();
#elif VERSION == V_WWW
    _subestructura.agregarNodo(1, "page");
    _candidatos = _subestructura.ejesNoUtilizados();   
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
//     cout << "Antes: " << _subestructura << endl;
    _subestructura.agregarNodo(_subestructura.cantNodos() + 1, nodo2);
    unsigned int enlace;
    if (nodo2 == "object")
        enlace = 1;
    else
        enlace = 2;
    _subestructura.agregarEje(nodo1, _subestructura.cantNodos(), enlace);
//     cout << _subestructura << endl;
    
    vector<unsigned int> temp;
    int pp = 1;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
//         cout << "Instancia " << pp++ << endl;
        if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
            vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
            posibilidades<unsigned int> op(v);
            
            bool done = false;
    
            for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//                 cout << "X" << endl;
                SOLUTION nueva_subestructura = _subestructura.reasignarNodosFijo(*q);
//                 cout << "Y" << endl;
//                 cout << "Esta: " << nueva_subestructura << endl;
//                 cout << "En: " << _instancias[*p] << endl;
                if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                    done = true;
//                     cout << "SI" << endl;
                    temp.push_back(*p);
                }
            }
//             cout << "FIN" << endl;
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
//     cout << "Despues: " << _subestructura << endl;
}


#elif VERSION == V_GO

void Hormiga::avanza(const unsigned int nodo1, const unsigned int nodo2, const unsigned int enlace) {
    // avanzar la hormiga
    cout << nodo1 << '=' << nodo2 << endl;
    _subestructura.agregarEje(nodo1, nodo2, enlace);
    
    vector<unsigned int> temp;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        if (_instancias[*p].ejeUsado(nodo1,nodo2,enlace)) {        
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
    _candidatos = getCandidatos();
}

#elif VERSION == V_SCIENCEMAP
void Hormiga::avanza(const unsigned int nodo1, const unsigned int nodo2, const unsigned int enlace) {

   // avanzar la hormiga
//     cout << nodo1 << '=' << nodo1 << endl;
//     cout << "Antes: " << _subestructura << endl;
    _subestructura.agregarEje(nodo1, nodo2, enlace);
    
//     cout << _subestructura << endl;
    
    vector<unsigned int> temp;
    int pp = 1;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
//         cout << "Instancia " << pp++ << endl;
        if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
            vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
            posibilidades<unsigned int> op(v);
            
            bool done = false;
    
            for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//                 cout << "X" << endl;
                SOLUTION nueva_subestructura = _subestructura.reasignarNodosFijo(*q);
//                 cout << "Y" << endl;
//                 cout << "Esta: " << nueva_subestructura << endl;
//                 cout << "En: " << _instancias[*p] << endl;
                if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                    done = true;
//                     cout << "SI" << endl;
                    temp.push_back(*p);
                }
            }
//             cout << "FIN" << endl;
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
    _candidatos = _subestructura.ejesNoUtilizados();
//     cout << "Despues: " << _subestructura << endl;
}

#elif VERSION == V_WWW
void Hormiga::avanza(const unsigned int nodo1, const unsigned int nodo2, const string enlace) {

   // avanzar la hormiga
//     cout << nodo1 << '=' << nodo1 << endl;
//     cout << "Antes: " << _subestructura << endl;
    _subestructura.agregarEje(nodo1, nodo2, enlace);
    
//     cout << _subestructura << endl;
    
    vector<unsigned int> temp;
    int pp = 1;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
//         // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
// //         cout << "Instancia " << pp++ << endl;
//         if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
//             vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
//             posibilidades<unsigned int> op(v);
//             
//             bool done = false;
//     
//             for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
// //                 cout << "X" << endl;
//                 SOLUTION nueva_subestructura = _subestructura.reasignarNodosFijo(*q);
// //                 cout << "Y" << endl;
// //                 cout << "Esta: " << nueva_subestructura << endl;
// //                 cout << "En: " << _instancias[*p] << endl;
//                 if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
//                     done = true;
// //                     cout << "SI" << endl;
//                     temp.push_back(*p);
//                 }
//             }
// //             cout << "FIN" << endl;
	   if (_instancias[*p].cubiertoPor(_subestructura)) {
	       temp.push_back(*p);
	   }
//         }
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
    _candidatos = _subestructura.ejesNoUtilizados();
//     cout << "Despues: " << _subestructura << endl;
}

#endif

//-------------------------------------------------------------------------
bool Hormiga::Utilizado(const unsigned int id) const {
    set<unsigned int> lista = _subestructura.nodosUtilizados();
    
    return (lista.find(id) != lista.end());
}
 
//-------------------------------------------------------------------------
#if VERSION == V_SHAPE

vector< CANDIDATE > Hormiga::getCandidatos() {
    return _subestructura.nodosNoUtilizados();
}

#elif VERSION == V_GO

vector< CANDIDATE > Hormiga::getCandidatos() {
    vector< CANDIDATE > lista;
    // Solo me quedo con aquellos que tiene al menos uno de los nodos en la subestructura
    // Y ademas pertenezcan a alguna de las instancias que cubre hasta el momento
    set<unsigned int> nu = _subestructura.nodosUtilizados();
    if (nu.size() > 0) {
        vector< CANDIDATE >::iterator it1 = _candidatos.begin();
        
        while (it1 != _candidatos.end()) {
	  if ((nu.find(it1->second) != nu.end()) or (nu.find(it1->first) != nu.end())) {
//             if (nu.find(it1->second) != nu.end()) {
		bool found = false;
		unsigned int i = 0;
		while (!found and (i < _support.size())) {
		    found = _instancias[_support[i]].ejeUsado(it1->first, it1->second, it1->third);
// 		    		  cout << "L: " << it1->first << ' ' << it1->second << ' ' << it1->third << ' ' << found << endl;

		    i++;
		}
		if (found) {
// 		  cout << "LL" <<endl;
		    lista.push_back(*it1);
		}
	    }
            it1++;
        }
    }
    cout << "S: " << _subestructura << lista.size() << endl;
        
    return lista;
// return _candidatos;
}

#elif (VERSION == V_SCIENCEMAP) || (VERSION == V_WWW)

vector< CANDIDATE > Hormiga::getCandidatos() {
   return _subestructura.ejesNoUtilizados();
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
#if (VERSION == V_SHAPE) ||  (VERSION == V_SCIENCEMAP) || (VERSION == V_WWW)

        _costes[1] = _subestructura.sizeNorm();
//         cout << _subestructura << endl;

#elif VERSION == V_GO

//         _costes[1] = _subestructura.second.sizeNorm();
        
        float total = 0.;
        for (unsigned int i = 0; i < _support.size(); i++) {
            total += (1. - _instancias[_support[i]].nivelRelativo(subEst()));

        }
        
        if (total > 0)
            _costes[1] = (total / (_support.size() * 1.));
        else
            _costes[1] = 0;
            
//         cout << _subestructura << endl;

#endif


//         cout << "Costo: " << _costes[0] << ' ' << _costes[1] << endl;
    }
}

//-------------------------------------------------------------------------
int Hormiga::dominancia(Hormiga& v, bool x, int y) {
    calculaCostes();
    v.calculaCostes();

    int que;
    
//     cout << "A: " << _costes[0] << ' ' << _costes[1] << endl;
//     for (unsigned int i = 0; i < _support.size(); i++) {
//         cout << _support[i] << ' ';
//     }
//     cout << endl;
//     cout << "B: " << v._costes[0] << ' ' << v._costes[1] << endl;
//     for (unsigned int i = 0; i < v._support.size(); i++) {
//         cout << v._support[i] << ' ';
//     }
//     cout << endl;
//     
    if ((_costes[0] >= v._costes[0]) && (_costes[1] >= v._costes[1]) && ((_costes[0] > v._costes[0]) || (_costes[1] > v._costes[1]))) {
        que = 1;
    }
    else if ((v._costes[0] >= _costes[0]) && (v._costes[1] >= _costes[1]) && ((v._costes[0] > _costes[0]) || (v._costes[1] > _costes[1]))) {
        que = -1;
    }
    else que = 0;
    
    if ((_costes[0] != 0) && (v._costes[0] != 0)) {
	if (x) {
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
    //             cout << "ND" << endl;
	    }
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

#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP) || (VERSION == V_WWW)

float Hormiga::getAparicionesEje(const CANDIDATE& eje) {
    assert(_aparEje->find(eje) != _aparEje->end());
//     cout << "AP: " << eje.first << ' ' << eje.second << '=' << (*_aparEje)[eje] << endl;
    return (*_aparEje)[eje];
}

#endif

//-------------------------------------------------------------------------

set<CANDIDATE> Hormiga::local_search() const {
    set<CANDIDATE> tiene;
    
    if (_support.size() > 0) {
      #if VERSION == V_SCIENCEMAP
      
      // Interseccion de todos
      set<CANDIDATE> primero = _instancias[_support[0]].ejes();
      for (set<CANDIDATE>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
	bool found = true;
	for (unsigned int i = 1; (i < _support.size()) and found; i++)
	  found = _instancias[_support[i]].ejeUsado(it->first,it->second,it->third);
	if (found)
	  tiene.insert(*it);
      }
      cout << "FIN " << tiene.size() << endl;
      #endif
    }
    return tiene;
}
