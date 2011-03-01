#include "hormigas.h"

//-------------------------------------------------------------------------
Hormiga::Hormiga (const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0) :
	_subestructura(base[0]),
	_support(base.size(), 0),
	_costes(numObjetivos, 0.0)
	{
	_subestructura.clear();

        _ejesAsignados = 0;
//         _colonias.push_back(colonia);
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
        _aparEje = ap;
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
// 	_candidatos = _subestructura.ejesNoUtilizados(); // Todos los posibles ejes
	_candidatos = getCandidatos();
	calculaCostes();
}

Hormiga::Hormiga (const vector<SOLUTION>& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _subestructura(sub), _support(base.size(), false), _costes(numObjetivos, 0.0) {
        _numObjetivos = numObjetivos;
        _costeValido = false;
        _instancias = base;
	
        _aparEje = ap;
        
        for (unsigned int l = 0; l < _support.size(); l++)
            _support[l] = l;
        
	vector<unsigned int> temp;
	
        vector<unsigned int>::iterator p = _support.begin();

        int x = 1;
        while (p != _support.end()) {
            // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
            cout << "Instancia: " << x++ << endl;
            if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
                cout << _instancias[*p] << endl;

                #ifndef NO_ISOMORFISM

                bool done = false;

                    map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);

                    cout << "T " << v.size() << endl;
                    multimap<unsigned int, pair<unsigned int, unsigned int> > a = _subestructura.ejes_a_multimap(), b = _instancias[*p].ejes_a_multimap();

                    posibilidades<unsigned int> op(v, a, b);
                    for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
                        cout << "X" << endl;
                        SOLUTION copia(_subestructura);
                        SOLUTION nueva_subestructura = copia.reasignarNodos(*q);
                        cout << "Y" << copia << endl;
                        cout << "Esta: " << nueva_subestructura << endl;
                        cout << "En: " << _instancias[*p] << endl;
                        if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                            done = true;
                            cout << "SI" << endl;
                            temp.push_back(*p);
                        }
                    }
                    
                #else
                    
                     if (_instancias[*p].cubiertoPor(_subestructura)) {
                        cout << "SI" << endl;
                        temp.push_back(*p);
                    }
                    
                #endif
            }
//             cout << "FIN" << endl;
            p++;
        }

        _support = temp;
	
       _candidatos = getCandidatos();

        calculaCostes();
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
    _subestructura.inicial();
    _candidatos = getCandidatos();
    _support.resize(_instancias.size(), 0);
    for (unsigned int l = 0; l < _support.size(); l++)
        _support[l] = l; 
}

//-------------------------------------------------------------------------

bool Hormiga::extendible() {
  bool res = false;
  
  _candidatos = getCandidatos();

  if (_candidatos.size() > 0) {
    calculaCostes(); 
    
    if (_costes[1] < 1) {
      vector<unsigned int>::iterator p = _support.begin();
      res = true;
      while ((p != _support.end()) and res) {
	res = (_subestructura.size() < _instancias[*p].size());
	++p;
      }
    }
  }

  return res;
};

//-------------------------------------------------------------------------

void Hormiga::avanza(const CANDIDATE & nuevo) {
    // avanzar la hormiga
    cout << "Avanza" << endl;
    
    SOLUTION nueva_subestructura = _subestructura;
    nueva_subestructura.agregarEje(nuevo.first, nuevo.second, nuevo.third);
    bool found = false;
    vector<unsigned int>::iterator p = _support.begin();
    while ((p != _support.end()) and !found) {
      found = (nueva_subestructura.size() > _instancias[*p].size());
      ++p;
    }
    
    if (!found) {
      _subestructura.agregarEje(nuevo.first, nuevo.second, nuevo.third);
      
      vector<unsigned int> temp;
      vector<unsigned int>::iterator p = _support.begin();
      while (p != _support.end()) {
// 	  cout << "VER i " << *p + 1 << endl;
	  if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {

              #ifndef NO_ISOMORFISM

                  // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
                  map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
                  multimap<unsigned int, pair<unsigned int, unsigned int> > a = _subestructura.ejes_a_multimap(), b = _instancias[*p].ejes_a_multimap();

                  posibilidades<unsigned int> op(v, a, b);

                  bool done = false;

                  for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
                      SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
                      if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
                          done = true;
                          temp.push_back(*p);
                      }
                  }
                  
              #else
                  
                   if (_instancias[*p].cubiertoPor(_subestructura)) {
                      temp.push_back(*p);
                  }

              #endif
	  }
	  p++;
      }

      _support = temp;
      _costeValido = false;         
      
      // aumentamos el numero de asignados si es que usamos nuevos
      _ejesAsignados++;
      
//       _candidatos = getCandidatos();  
    }
}

//-------------------------------------------------------------------------

bool Hormiga::Utilizado(const unsigned int id) const {
    set<unsigned int> lista = _subestructura.nodosUtilizados();
    
    return (lista.find(id) != lista.end());
}
 
//-------------------------------------------------------------------------

bool Hormiga::operator==(const Hormiga & unaHormiga) {
  cout << "IGUALDAD?" << endl;
  calculaCostes();
  Hormiga copia(unaHormiga);
  copia.calculaCostes();
  
  bool res = true;
  for (unsigned int i = 0; (i < _numObjetivos) and res; ++i)
    res = (_costes[i] == copia._costes[i]);
  
  return (res and (_subestructura == unaHormiga._subestructura));
}

//-------------------------------------------------------------------------

vector< CANDIDATE > Hormiga::getCandidatos() {
  vector< CANDIDATE > res;
  
  cout << _subestructura << endl;
  
  vector< CANDIDATE > primero = _subestructura.ejesNoUtilizados();
  
  // Elimino aquellos candidatos que no existan en ninguna instancias
  
  for (vector< CANDIDATE >::const_iterator p = primero.begin(); p != primero.end(); ++p) {
    bool found = false;
    for (vector<unsigned int>::const_iterator it = _support.begin(); (it != _support.end()) and !found; ++it) {
//       cout << '(' << p->first << ',' << p->second << ',' << p->third << ')' << endl << _instancias[*it] << endl;
      if (_instancias[*it].ejeTipoUsado(*p, _subestructura)) {
	res.push_back(*p);
	cout << "Candidato " << '(' << p->first << ',' << p->second << ',' << p->third << ')' << endl;
	found = true;
      }
    }
  }
  
  cout << "Candd " << primero.size() << ' ' << res.size() << endl;
  
  return res;
}

//-------------------------------------------------------------------------
float Hormiga::getCoste(const unsigned int id) {
    assert(id < _numObjetivos);
    
    calculaCostes();    
    return _costes[id];
}

//-------------------------------------------------------------------------
void Hormiga::calculaCostes() {
	cout << "CC1" << endl;
    if (!_costeValido) {
        _costeValido = true;
        
        // Calculo costes
        // Esto se instancia en cada subclase particular
        
        // 0 = support
        _costes[0] = ((_support.size() * 1.) / (_instancias.size() * 1.));
        
        // 1 = complexity
#if (VERSION == V_GO)
      
        float total = 0.;
        for (unsigned int i = 0; i < _support.size(); i++) {
            total += (1. - _instancias[_support[i]].nivelRelativo(subEst()));

        }
        
        if (total > 0)
            _costes[1] = (total / (_support.size() * 1.));
        else
            _costes[1] = 0;
	
#else
     _costes[1] = _subestructura.sizeNorm();

#endif


//         cout << "Costo: " << _costes[0] << ' ' << _costes[1] << endl;
    }
	cout << "CC2" << endl;
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

float Hormiga::getAparicionesEje(const CANDIDATE& eje) {
    assert(_aparEje->find(eje) != _aparEje->end());
//     cout << "AP: " << eje.first << ' ' << eje.second << '=' << (*_aparEje)[eje] << endl;
    return (*_aparEje)[eje];
}

//-------------------------------------------------------------------------
	
ostream& operator<<(ostream& os, const Hormiga& s) {
  os << s.subEst() << endl;
  
  vector<unsigned int> el = s.soporte();
  
  for (vector<unsigned int>::const_iterator it = el.begin(); it != el.end(); ++it)
    cout << *it + 1 << ' ';
  
  cout << endl;
/*  
  float n = s.getCoste(0);
  float m = s.getCoste(1);
  
  cout << "Cost = " << n << ", Cost = " << m << endl; */
  
  return os;
}

//-------------------------------------------------------------------------

void Hormiga::local_search() {   
	// Cambiar para que lo haga depth first! que sino se queda sin memoria
	bool algo = false;
	
	cout << "LS instancias" << endl;
	
	for (unsigned int i = 0; i < _support.size(); i++) {
	  cout << _support[i] + 1 << ' ';
	}
	cout << endl;
	
	if (_support.size() == 1) {
	  _subestructura = _instancias[_support[0]];
	  cout << "same1" << endl;
	  _costeValido = false;
	  _candidatos.clear();
	  cout << "same2" << endl;
	}
	else if (_support.size() > 1) {
		// See what all support structures have in common
		bool done = false;
		
		// Search for the smallest one
		unsigned int min = _instancias[_support[0]].size();
		unsigned int pos = 0;
		for (unsigned int i = 1; i < _support.size(); ++i) {
			unsigned int nmin = _instancias[_support[i]].size();
			if (nmin < min) {
				min = nmin;
				pos = i;
			}
		}
		cout << "INS " << pos << ' ' << _support[pos] + 1 << endl;

		// Quito los ejes que ya estan en la subestructura
		set<unsigned int> primero = _instancias[_support[pos]].nodosUtilizados();
		
		#ifndef NO_ISOMORFISM
		
                    map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[_support[pos]]);
                    for (map<unsigned int, vector<unsigned int> >::iterator it = v.begin(); it != v.end(); ++it)
                      primero.erase((*it).first);
                    set<CANDIDATE> segundo = _instancias[_support[pos]].ejesUtilizados();
                    multimap<unsigned int, pair<unsigned int, unsigned int> > a = _subestructura.ejes_a_multimap(), b = _instancias[_support[pos]].ejes_a_multimap();

                    for (set<unsigned int>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
                        cout << "ADD:" << *it << endl;

                        posibilidades<unsigned int> op(v, a, b);
                        posibilidades<unsigned int>::iterator q;

			bool almenosuno = false;

                        for (q = op.begin(); (q != op.end()) and !almenosuno; ++q) {
    // 		      SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
    // 		      if (_instancias[_support[pos]].cubiertoPor(nueva_subestructura)) {
                              SOLUTION posible_nueva_subestructura = _subestructura;
                              unsigned int nn = posible_nueva_subestructura.agregarNodo(_instancias[_support[pos]].tipoNodo(*it));
                              map<unsigned int, unsigned int> nq = *q;
                              pair<map<unsigned int, unsigned int>::iterator, bool> par = nq.insert(pair<unsigned int, unsigned int>(nn,*it));
                              cout << posible_nueva_subestructura << ' ' << nn << ' ' << par.second << endl;

                              for(map<unsigned int, unsigned int>::iterator o = nq.begin(); o != nq.end(); ++o)
                                cout << "MAP " << o->first << ' ' << o->second << endl;

                              // Agrego los ejes que tiene
                              for (set<CANDIDATE>::const_iterator itj = segundo.begin(); itj != segundo.end(); ++itj) {
    // 			    cout << itj->first << ' ' << itj->second << ' ' << itj->third << ' ' << *it << endl;
                                if ((*it == (*itj).first) or (*it == (*itj).second)) { // Ejes que involucran al nodo nuevo
                                  // Map al reves!
                                  bool pri = true;
                                  unsigned int quien = (*itj).second;
                                  if (*it == (*itj).second) {
                                    pri = false;
                                    quien = (*itj).first;
                                  }
    // 			      cout << *it << ' ' << quien << endl;
                                  map<unsigned int,unsigned int>::iterator pp = nq.begin();
                                  while ((pp != nq.end()) and ((*pp).second != quien)) {
                                    ++pp;
                                  }

                                  if (pp != nq.end()) {
    // 				cout << nn << ' ' << (*pp).first << ' ' << (*pp).second << endl;
                                    if (pri)
                                      posible_nueva_subestructura.agregarEje(nn, (*pp).first, (*itj).third);
                                    else
                                      posible_nueva_subestructura.agregarEje((*pp).first, nn, (*itj).third);
                                    almenosuno = true;
                                  }
                                  else
                                    cout << "Not found" << endl;
                                }
                              }

                              if (almenosuno) {
                                cout << posible_nueva_subestructura << endl;
                                done = false;
                                for (unsigned int i = 0; (i < _support.size()) and !done; i++) {
                                  cout << "Instancia " << _support[i] + 1 << endl << _instancias[_support[i]] << endl;

                                  map<unsigned int, vector<unsigned int> > w = posible_nueva_subestructura.darPosibilidades(_instancias[_support[i]]);
                                  multimap<unsigned int, pair<unsigned int, unsigned int> > aa = posible_nueva_subestructura.ejes_a_multimap(), bb = _instancias[_support[i]].ejes_a_multimap();

                                  posibilidades<unsigned int> opp(w, aa, bb);

                                  posibilidades<unsigned int>::iterator qq = opp.begin();
                                  done = true;
                                  while ((qq != opp.end()) and done) {
                                    SOLUTION la_subestructura = posible_nueva_subestructura.reasignarNodos(*qq);
                                    if (_instancias[_support[i]].cubiertoPor(la_subestructura)) {
                                        done = false;
                                        cout << "COVERED!" << endl;
                                    }
                                    else ++qq;
                                  }
                                  cout << "fuera" << endl;
                                }
                                if (!done) {
                                  cout << "CAMBIO" << endl;
                                  cout << "OLD " << _subestructura << endl << "NEW " << posible_nueva_subestructura << endl;
                                  _subestructura = posible_nueva_subestructura;
                                  _ejesAsignados++;
                                  algo = true;
                                }
    // 			  }
                            }
                            v = _subestructura.darPosibilidades(_instancias[_support[pos]]);
                            a = _subestructura.ejes_a_multimap();
                            b = _instancias[_support[pos]].ejes_a_multimap();
                          }
                    }
                    
                #else

                    set<unsigned int> alt = _subestructura.nodosUtilizados();
                    for (set<unsigned int>::const_iterator it = alt.begin(); it != alt.end(); ++it)
                      primero.erase(*it);
                    set<CANDIDATE> segundo = _instancias[_support[pos]].ejesUtilizados();

                    for (set<unsigned int>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
			  cout << "ADD:" << *it << endl;
			  
// 			  bool ver = false;
// 			  for (unsigned int i = 0; (i < _support.size()) and !false; i++) {
// 			      set<unsigned int> us = _instancias[_support[i]].nodosUtilizados();
// 			  }

                          SOLUTION posible_nueva_subestructura = _subestructura;
                          unsigned int nn = posible_nueva_subestructura.agregarNodo(_instancias[_support[pos]].tipoNodo(*it));
                          bool almenosuno = false;

                          #if VERSION == V_GO
                            almenosuno = true;
                          #else
                           // Agrego los ejes que tiene
                           for (set<CANDIDATE>::const_iterator itj = segundo.begin(); itj != segundo.end(); ++itj) {
                            if ((*it == (*itj).first) and (posible_nueva_subestructura.nodoUsado((*itj).second))) {
                                posible_nueva_subestructura.agregarEje(nn, (*itj).second, (*itj).third);
                                almenosuno = true;
                            }
                            else if ((*it == (*itj).second) and (posible_nueva_subestructura.nodoUsado((*itj).first))) { // Ejes que involucran al nodo nuevo
                                    posible_nueva_subestructura.agregarEje((*itj).first, nn, (*itj).third);
                                    almenosuno = true;
                                 }
                            }
                          #endif

                          if (almenosuno) {
                            cout << posible_nueva_subestructura << endl;
                            done = false;
                            for (unsigned int i = 0; (i < _support.size()) and !done; i++) {
                              cout << "Instancia " << _support[i] + 1 << endl << _instancias[_support[i]] << endl;

                              if (_instancias[_support[i]].cubiertoPor(posible_nueva_subestructura)) {
                                    done = false;
                                    cout << "COVERED!" << endl;
                              }
                              else done = true;
                            }

                            if (!done) {
                              cout << "CAMBIO" << endl;
                              cout << "OLD " << _subestructura << endl << "NEW " << posible_nueva_subestructura << endl;
                              _subestructura = posible_nueva_subestructura;
                              _ejesAsignados++;
                              algo = true;
                            }
                        }
                }
                #endif

		if (algo) {
		  _costeValido = false;         
// 		  _candidatos = getCandidatos();
		}
	}
}
