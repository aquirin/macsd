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
	_candidatos = _subestructura.ejesNoUtilizados(); // Todos los posibles ejes
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
                map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
             
             /*  for (int oo = 0; oo < v.size(); oo++) {
                    for (int pp = 0; pp < v[oo].size(); pp++)
                        cout << v[oo][pp] << ",";
                    cout << endl;
                }*/
                
		cout << "T " << v.size() << endl;
                bool done = false;
                posibilidades<unsigned int> op(v);
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
            }
//             cout << "FIN" << endl;
            p++;
        }

        _support = temp;
	
       _candidatos = _subestructura.ejesNoUtilizados();

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
    _subestructura.clear();    
    _candidatos = _subestructura.ejesNoUtilizados();
    _support.resize(_instancias.size(), 0);
    for (unsigned int l = 0; l < _support.size(); l++)
        _support[l] = l;   
}

//-------------------------------------------------------------------------

bool Hormiga::extendible() {
  calculaCostes();
  
  bool res = ((_candidatos.size() > 0) and (_costes[1] < 1));
  
  bool found = res;
  vector<unsigned int>::iterator p = _support.begin();
  while ((p != _support.end()) and found) {
    found = (_subestructura.size() > _instancias[*p].size());
    ++p;
  }

  return res and !found;
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
	  cout << "VER i " << *p + 1 << endl;
	  // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
	  if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
	      map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
	      posibilidades<unsigned int> op(v);
	      
	      bool done = false;
      
	      for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
		  SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
		  if (_instancias[*p].cubiertoPor(nueva_subestructura)) {
		      done = true;
		      temp.push_back(*p);
		  }
	      }
	  }
	  p++;
      }

      _support = temp;
      _costeValido = false;         
      
      // aumentamos el numero de asignados si es que usamos nuevos
      _ejesAsignados++;
      
      _candidatos = _subestructura.ejesNoUtilizados();  
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
    return _subestructura.ejesNoUtilizados();
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

void Hormiga::local_search() {   
	// Cambiar para que recuerde las subestructuras que va encontrando de caminio o se vuelve muy lento...
	
	cout << "LS instancias" << endl;
	
	for (unsigned int i = 0; i < _support.size(); i++) {
	  cout << _support[i] + 1 << ' ';
	}
	cout << endl;
	
	if (_support.size() == 1) {
	  _subestructura = _instancias[_support[0]];
	   _costeValido = false;
	   _candidatos = _subestructura.ejesNoUtilizados();
	}
	else if (_support.size() > 1) {
		// See what all support structures have in common
		
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

		// Where is the substructure in every instance covered
		
		// vector, one for each in _support
		// list, all candidates
		SOLUTION la_subestructura = _subestructura;
		vector< list< map<unsigned int, unsigned int> > > opciones(_support.size());
		vector< list< SOLUTION > > str(_support.size());
		set<unsigned int> primero = _instancias[_support[pos]].nodosUtilizados();
		set<CANDIDATE> segundo = _instancias[_support[pos]].ejesUtilizados();

		for (unsigned int i = 0; i < _support.size(); i++) {
			map<unsigned int, vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[_support[i]]);
			if (i == pos) {
				for (map<unsigned int, vector<unsigned int> >::iterator it = v.begin(); it != v.end(); ++it)
					primero.erase((*it).first);
			}
			posibilidades<unsigned int> op(v);
		
			posibilidades<unsigned int>::iterator q;
			for (q = op.begin(); q != op.end(); ++q) {
				SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
				if (_instancias[_support[i]].cubiertoPor(nueva_subestructura)) {
					// Add to the list
					opciones[i].push_back(*q);
					str[i].push_back(nueva_subestructura);
				}
			}
		}
		
		// For each node used in pos and not in _substructure, try to add it to each instance
		bool algo = false;
		for (set<unsigned int>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
		  vector< list< map<unsigned int, unsigned int> > > nopciones(opciones.size());
		  vector< list< SOLUTION > > nstr(str.size());
		  bool done = false;
		  cout << "ADD:" << *it << endl;
		  for (unsigned int i = 0; (i < _support.size()) and !done; i++) {
		    // Add new node
		    cout << "Instancia " << _support[i] + 1 << endl << _instancias[_support[i]] << endl;
		    done = true;

		    for (list< map<unsigned int, unsigned int> >::iterator itp = opciones[i].begin(); itp != opciones[i].end(); ++itp) {
		      if ((*itp).find(*it) == (*itp).end()) {	
			cout << "NOption " << *it << endl;
				
			// Busco que posibilidades tengo para el nuevo nodo
			vector<unsigned int> que = _subestructura.darPosibilidades(_instancias[_support[i]], _instancias[_support[pos]].tipoNodo(*it));
			for (vector<unsigned int>::iterator itv = que.begin(); itv != que.end(); ++itv) {
			    cout << "elque " << *itv << endl;
			    map<unsigned int, unsigned int> x(*itp);
			    if (x.find(*it) == x.end()) {
				map<unsigned int,unsigned int>::iterator qq = x.begin();
				while ((qq != x.end()) and ((*qq).second != *itv))
				    ++qq;
				  
				if (qq == x.end()) {
				  for (map<unsigned int, unsigned int>::iterator ii = x.begin(); ii != x.end(); ++ii)
					  cout << "POSI " << (*ii).first << " -> " << (*ii).second << endl;
				  
				  // Agrego el nuevo nodo
				  SOLUTION nueva_subestructura = _subestructura;
				  cout << nueva_subestructura << endl;
				  unsigned int nn = nueva_subestructura.agregarNodo(_instancias[_support[pos]].tipoNodo(*it));
				  cout << "Nuevo nodo " << nn << endl;
				   x[nn] = *itv;
				   
				  // Agrego los ejes que tiene
				  bool almenosuno = false;
				  for (set<CANDIDATE>::const_iterator itj = segundo.begin(); itj != segundo.end(); ++itj) {	    
				    if ((*it == (*itj).first) or (*it == (*itj).second)) { // Arreglar, mal numeros
				      // Map al reves!
				      bool pri = true;
				      unsigned int quien = (*itj).second;
				      if (*it == (*itj).second) {
					pri = false;
					quien = (*itj).first;
				      }
				      map<unsigned int,unsigned int>::iterator pp = x.begin();
				      while ((pp != x.end()) and ((*pp).second != quien))
					++pp;
				      
				      if (pp != x.end()) {
					cout << nn << ' ' << (*pp).first << endl;
					if (pri)
					  nueva_subestructura.agregarEje(nn, (*pp).first, (*itj).third);
					else
					  nueva_subestructura.agregarEje((*pp).first, nn, (*itj).third);
					almenosuno = true;
				      }
				    }
				  }
				  
				  if (almenosuno) {
				    cout << nueva_subestructura << endl;
				    SOLUTION super_nueva_subestructura = nueva_subestructura.reasignarNodos(x);
				    if (_instancias[_support[i]].cubiertoPor(super_nueva_subestructura)) {
					    nopciones[i].push_back(x);
					    nstr[i].push_back(nueva_subestructura);
					    done = false;
					    algo = true;
					    cout << "ALGO" << endl;
				    }
				  }
				}
			    }
			}
		      //	cout << "fuera" << endl;
		      }
		    }
		  }
		  if (!done and algo and !opciones[pos].empty()) {
		      opciones = nopciones;
		      str = nstr;
		      _ejesAsignados++;
		      
		      cout << "CAMBIO" << endl;
		      SOLUTION nueva_subestructura = *(str[pos].begin());
		      cout << "OLD " << _subestructura << endl << "NEW " << nueva_subestructura << endl;
		      _subestructura = nueva_subestructura;
		      _costeValido = false;         
		      _candidatos = _subestructura.ejesNoUtilizados();
		  }
// 		  else {
// 		    opciones.clear();
// 		    str.clear();
// 		  }
		}
// 		if (algo and !opciones[pos].empty()) {
// 			cout << "CAMBIO" << endl;
// 			SOLUTION nueva_subestructura = *(str[pos].begin());
// 			cout << "OLD " << _subestructura << endl << "NEW " << nueva_subestructura << endl;
// 			_subestructura = nueva_subestructura;
// 			_costeValido = false;         
// 			_candidatos = _subestructura.ejesNoUtilizados();
// 		}
	}	
}

//-------------------------------------------------------------------------

/*void Hormiga::local_search() {   
	// Cambiar para que recuerde las subestructuras que va encontrando de caminio o se vuelve muy lento...
	
    bool something = false;
    if (_support.size() > 0) {
		// See what all support structures have in common
		
		// Search for the smallest one
		unsigned int min = _instancias[_support[0]].size();
		unsigned int pos = 0;
		for (unsigned int i = 1; i < _support.size(); ++i) {
			unsigned int nmin = _instancias[_support[i]].size();
			if (nmin < min) {
				min = nmin;
				pos = i;
			}
			// 	  cout << "INS " << i << ' ' << _instancias[_support[i]] << endl;
		}
		
		// Check if every edge is in there... 
		// vector, one for each in _support
		// list, all candidates
		// SOLUTION, reasigned node solution
		vector< list< SOLUTION > > found(_support.size());
		
		vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[_support[pos]]);
		posibilidades<unsigned int> op(v);
		
		bool done = false;
		posibilidades<unsigned int>::iterator q;
		for (q = op.begin(); q != op.end(); ++q) {
			SOLUTION nueva_subestructura = _subestructura.reasignarNodos(*q);
			if (_instancias[_support[pos]].cubiertoPor(nueva_subestructura)) {
				// Add to the list
				found[pos].push_back(nueva_subestructura);
			}
		}
		
		cout << "!! " << _subestructura << ',' << _instancias[_support[pos]] << "->" << nueva_subestructura << endl;
		
		set<CANDIDATE> primero = _instancias[_support[pos]].ejesUtilizados();
		
		for (set<CANDIDATE>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
			for (list<SOLUTION>::iterator its = found[pos].begin(); its != found[pos].end(); ++its) {
				if ((*its).nodoUsado(it->first) != (*its).nodoUsado(it->second)) {
					SOLUTION prototype(*its);
					
					cout << "ADD:" << it->first << ',' << it->second << ',' << it->third << endl;
					
					if (! prototype.ejeUsado(it->first, it->second, it->third)) {
						if (! (*its).nodoUsado(it->first)) {
							prototype.agregarNodoID(it->first, _instancias[_support[pos]].tipoNodo(it->first));
						}
						else if (! (*its).nodoUsado(it->second)) {
							prototype.agregarNodoID(it->second, _instancias[_support[pos]].tipoNodo(it->second));
						}
						
#if (VERSION != V_GO) 
						prototype.agregarEje(it->first, it->second, it->third);
#endif
						
						cout << prototype << endl;
						
						bool done = true;
						for (unsigned int i = 0; (i < _support.size()) and done; i++) {
							if (i != pos) {
								// Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
								if (_instancias[_support[i]].cantNodos() >= prototype.cantNodos()) {
									vector< vector<unsigned int> > v = prototype.darPosibilidades(_instancias[_support[i]]);
									posibilidades<unsigned int> op(v);
									
									for (posibilidades<unsigned int>::iterator q = op.begin(); q != op.end(); ++q) {
										SOLUTION la_subestructura = prototype.reasignarNodos(*q);
										if (_instancias[_support[i]].cubiertoPor(la_subestructura)) {
											// Add to the list
											found[i].push_back(la_subestructura);
										}
									}
								}
								if (found[i].empty())
									done = false;
								// 		    cout << found << ' ' << i << ' ' << _instancias[_support[i]] << ' ' << la_subestructura << endl;
							}
						}
						
						if (found) {	
							// Map it!
							nueva_subestructura = prototype;
							// aumentamos el numero de asignados si es que usamos nuevos
							_ejesAsignados++;
							something = true;
						}
					}
				}
			}
		}
		if (something) {
			_subestructura = nueva_subestructura;
			_costeValido = false;         
			_candidatos = _subestructura.ejesNoUtilizados();
		}
	}
}
*/