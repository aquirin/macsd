#include "vmap.h"

vmap::vmap(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn) {
	_name = name;
	
	for (unsigned int i = 0; i < shap.size(); ++i) {
		_desc_nodo.insert(pair<unsigned int, string>(i+1, shap[i]));
		_rdesc_nodo.insert(pair<string, unsigned int>(shap[i], i+1));
	//	    cout << "##" << i+1 << ' ' << shap[i] << endl;
	}
	
	for (unsigned int i = 0; i < link.size(); ++i) {
		_desc_eje.insert(pair<unsigned int, string>(i+1, link[i]));
		_rdesc_eje.insert(pair<string, unsigned int>(link[i], i+1));
		//     cout << i+1 << ' ' << link[i] << endl;
	}
	
	for (map<pair<string,string>, string>::const_iterator it = rn.begin(); it != rn.end(); ++it) {
		//       cout << it->first.first << ' ' << it->first.second << ' ' << it->second << endl;
		_base_ejes.insert(CANDIDATE(_rdesc_nodo.find(it->first.first)->second, _rdesc_nodo.find(it->first.second)->second, _rdesc_eje.find(it->second)->second));
		//      cout << _rdesc_nodo.find(it->first.first)->second << ' ' << _rdesc_nodo.find(it->first.second)->second << ' ' << _rdesc_eje.find(it->second)->second << endl;
	}
	
	  cout << "Maximo " << _desc_nodo.size() + _desc_eje.size() << endl;
	inicial();
	MAX = 999;
}

void vmap::inicial() {
  clear();
  agregarNodo("\"Multidisciplinary\"");
}

vector< CANDIDATE > vmap::ejesNoUtilizados() const {
    vector< CANDIDATE > res;

    if (_nodos.size() < MAX) {
      // Busco ejes entre nodos ya existentes que no haya usado
      for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
	for (set<unsigned int>::const_iterator q = _nodos.begin(); q != _nodos.end(); ++q)
	  for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	    if ((_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, _relacion_nodos.find(*q)->second, r->first)) != _base_ejes.end()) and (_ejes.find(CANDIDATE(*p, *q, r->first)) == _ejes.end()))
	      res.push_back(CANDIDATE(*p, *q, r->first));
	  }
    
    // Busco ejes con un nodo nuevo que puede salir de cualquier nodo
    for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
      for (map<unsigned int, string>::const_iterator q = _desc_nodo.begin(); q != _desc_nodo.end(); ++q)
	for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	  if ((_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, q->first, r->first)) != _base_ejes.end()) and (_ejes.find(CANDIDATE(*p, q->first, r->first)) == _ejes.end()))
	      res.push_back(CANDIDATE(*p, MAX + q->first, r->first));
	}
    }
    
    return res;
}

unsigned int vmap::agregarNodo(const string & s) {
  cout << "an string " << s << endl;
  assert(_rdesc_nodo.find(s) != _rdesc_nodo.end());

  map<string, unsigned int>::const_iterator it = _rdesc_nodo.find(s);

  _nodos.insert((*it).second);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>((*it).second, (*it).second));

  return (*it).second;
}

unsigned int vmap::agregarNodo(const unsigned int & s) {
  cout << "an int " << s << endl;

  unsigned int nuevo = s;
  if (nuevo > MAX) nuevo = s - MAX;

  _nodos.insert(nuevo);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>(nuevo, nuevo));

  return nuevo;
}

unsigned int vmap::agregarNodoID(const unsigned int & n, const string & s) {
  assert(_rdesc_nodo.find(s) != _rdesc_nodo.end());

  map<string, unsigned int>::const_iterator it = _rdesc_nodo.find(s);

  assert(n == (*it).second);

  _nodos.insert((*it).second);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>((*it).second, (*it).second));

  return (*it).second;
}

void vmap::agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    cout << ini << ' ' << fin << ' ' << s << endl;
    assert((_nodos.find(ini) != _nodos.end()) and ((_nodos.find(fin) != _nodos.end()) or (fin > _desc_nodo.size())) and (_rdesc_eje.find(s) != _rdesc_eje.end()));

    unsigned int segundo = fin;
    if (_nodos.find(fin) == _nodos.end()) {
      // Agrego un nuevo nodos
      segundo = agregarNodo(fin);
    }
    unsigned int eleje = _rdesc_eje.find(s)->second;
    if (_ejes.find(CANDIDATE(ini,segundo,eleje)) == _ejes.end()) {
        _ejes.insert(CANDIDATE(ini,segundo,eleje));
    }
    else {
        cout << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
    }
}

void vmap::agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    cout << ini << ' ' << fin << ' ' << s << endl;
    assert((_nodos.find(ini) != _nodos.end()) and ((_nodos.find(fin) != _nodos.end()) or (fin > MAX)) and (_desc_eje.find(s) != _desc_eje.end()));

    unsigned int segundo = fin;
    if (_nodos.find(fin) == _nodos.end()) {
      // Agrego un nuevo nodos
      segundo = agregarNodo(fin);
    }
    if (_ejes.find(CANDIDATE(ini,segundo,s)) == _ejes.end()) {
        _ejes.insert(CANDIDATE(ini,segundo,s));
    }
    else {
        cout << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
    }
}

vmap vmap::crossover(const vmap & other) const {
  // Selecciono el punto de corte en ambos
  vmap res;
  
  CANDIDATE primero, segundo;
  vector<CANDIDATE> uno;
  unsigned int pos_uno = 0, pos_dos = 0;
  
  vmap A, B;
  
  cout << "Cruzar" << endl << *this << endl << other << endl;

  if (_ejes.size() >= 1) {
    int sel = intAzar(1, _ejes.size());
    set<CANDIDATE>::iterator it = _ejes.begin();
    for (int i = 1; i < sel; ++i) ++it;
//     cout << "antes" << endl;
    A = _subgraph(*it, it->first, pos_uno);
//     cout << "despues" << endl;
  }
  else {
    A = *this;
    set<unsigned int>::iterator it = _nodos.begin();
    pos_uno = *it;
  }
   
  vector<CANDIDATE> dos;
  if (other._ejes.size() >= 1) {
    int sel = intAzar(1, other._ejes.size());
    set<CANDIDATE>::iterator it = other._ejes.begin();
    for (int i = 1; i < sel; ++i) ++it;
//     cout << "antes" << endl;
    B = other._subgraph(*it, it->second, pos_dos);
//     cout << "despues" << endl;
  }
  else {
    B = other;
    set<unsigned int>::iterator it = other._nodos.begin();
    pos_dos = *it;
  }
  
  cout << "A es:" << A << endl << "B es:" << B << endl;
  
  // Hago el cruce
  res = A;
  
  unsigned int x;
  map<unsigned int, unsigned int> nn;
  
  cout << res << endl;
  
  for (set<unsigned int>::const_iterator p = B._nodos.begin(); p != B._nodos.end(); ++p) {
    x = res.agregarNodo(B._desc_nodo.find(B._relacion_nodos.find(*p)->second)->second);
    nn[*p] = x;
    cout << res << endl;
  }
   
  for (set<CANDIDATE>::iterator it = other._ejes.begin(); it != other._ejes.end(); ++it) {
    if ((nn.find(it->first) != nn.end()) and (nn.find(it->second) != nn.end()))
      res.agregarEje(nn[it->first], nn[it->second], _desc_eje.find(it->third)->second);
  }
  
  cout << pos_uno << ' ' << nn[pos_dos] << endl;
  
  res.agregarEje(pos_uno, nn[pos_dos], 1);
 
  cout << "Solucion:" << endl << res << endl;
  
  return res;
}

vmap vmap::mutation() const {
  // Agrego, elimino o cambio?
  vmap nueva(*this);
  
  int sel = intAzar(0, 2);
  cout << "Azar " << sel << endl;
  cout << *this << ' ' << size() << endl;

  if (sel == 0) {
      // Agrego una hoja
      vector<CANDIDATE> cand = nueva.ejesNoUtilizados();
      // Elimino los auto-ciclos
      for (vector<CANDIDATE>::iterator x = cand.begin(); x != cand.end(); ++x) {
	if (x->first == x->second)
	  cand.erase(x);
	else
	  cout << "CD " << x->first << ' ' << x->second << ' ' << x->third << endl;
      }
      if (!cand.empty()) {
	int sel1 = intAzar(1, cand.size());
	nueva.agregarEje(cand[sel1-1].first, cand[sel1-1].second, cand[sel1-1].third);
	cout << "MutA " << *this << ' ' << nueva << endl;
      }
  }
  else if (sel == 1) {
      // Elimino una hoja
      if (nueva.size() > 1) {
	vector<unsigned int> primero = nueva.hojas();
	int sel1 = intAzar(1, primero.size());
	nueva.borrarNodo(primero[sel1 - 1]);
	cout << "MutE " << *this << ' ' << nueva << endl;
      }
  }
  else {
    // Cambio una hoja
    if (nueva.size() > 1) {
      vector<unsigned int> primero = nueva.hojas();
      int sel1 = intAzar(1, primero.size());
      set<unsigned int> origen;
      vector<unsigned int> usables;
      
      for (map<unsigned int, string>::const_iterator q = _desc_nodo.begin(); q != _desc_nodo.end(); ++q) {
	if (q->first != _relacion_nodos.find(primero[sel1 - 1])->second)
	  usables.push_back(q->first);
      }
      
  //     // Reviso que nodos pueden ser intercambiados por este
  //     for (map<unsigned int, string>::const_iterator q = _desc_nodo.begin(); q != _desc_nodo.end(); ++q) {
  //       if (q->first != _relacion_nodos.find(primero[sel1 - 1])->second) {
  // 	for (set<CANDIDATE>::iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
  // 	  if (it->first == _relacion_nodos.find(primero[sel1 - 1])->second) {
  // 	    origen.insert(_relacion_nodos.find(it->second)->second);
  // 	  }
  // 	  if (it->second == _relacion_nodos.find(primero[sel1 - 1])->second) {
  // 	    origen.insert(_relacion_nodos.find(it->first)->second);
  // 	  }
  // 	}
  //       }
  //     }
  // 
  //     for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
  //       if (origen.find(r->first) != origen.end())
  // 	usables.push_back(r->second);
  //       if (origen.find(r->second) != origen.end())
  // 	usables.push_back(r->first);
  //     }
    
      int sel2 = intAzar(1, usables.size());
      nueva._relacion_nodos[primero[sel1 - 1]] = usables[sel2 - 1];
      
      cout << "MutC " << *this << ' ' << nueva << endl;
    }
  }
  
  return nueva;
}

vmap vmap::_subgraph(CANDIDATE eje, unsigned int cual, unsigned int & donde) const {
  vmap res;
  stack<unsigned int> pila;
  set<unsigned int> used;
  map<unsigned int, unsigned int> nn;
  
  // Agrego nodos
  pila.push(cual);
  
  while (!pila.empty()) {
    unsigned int actual = pila.top();
    pila.pop();
    used.insert(actual);
    unsigned int x = res.agregarNodo(_desc_nodo.find(_relacion_nodos.find(actual)->second)->second);
    nn[actual] = x;
    for (set<CANDIDATE>::iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
      if ((it->first == actual) and (*it != eje) and (used.find(it->second) == used.end()))
	pila.push(it->second);
    }
  }
  
  // Agrego ejes
  for (set<CANDIDATE>::iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
    if ((*it != eje) and (nn.find(it->first) != nn.end()) and (nn.find(it->second) != nn.end()))
      res.agregarEje(nn[it->first], nn[it->second], _desc_eje.find(it->third)->second);
  }
  
  donde = nn[cual];
  
  return res;
}

void vmap::random(const unsigned int how_many) {
    inicial();
    for (unsigned int i = 0; i < how_many; ++i) {
        vector< CANDIDATE > nuevo = ejesNoUtilizados();

        int sel = intAzar(1, nuevo.size());
        cout << "RANDOM: " << nuevo[sel-1].first << ' ' << nuevo[sel-1].second << ' ' << nuevo[sel-1].third << endl;
        agregarEje(nuevo[sel-1].first, nuevo[sel-1].second, nuevo[sel-1].third);
    }
}

vector<unsigned int> vmap::hojas() const {
    // En el caso particular de www son hojas de verdad...
    vector<unsigned int> sol;
    map<unsigned int, unsigned int> nn;

    for (set<CANDIDATE>::const_iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
      if (nn.find(it->first) == nn.end())
	nn[it->first] = 1;
      else
	nn[it->first]++;
      if (nn.find(it->second) == nn.end())
	nn[it->second] = 1;
      else
	nn[it->second]++;
    }
    
    for (map<unsigned int, unsigned int>::iterator it = nn.begin(); it != nn.end(); ++it)
      if (it->second == 1)
	sol.push_back(it->first);

    return sol;
}
