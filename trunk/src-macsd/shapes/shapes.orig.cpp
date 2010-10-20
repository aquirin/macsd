///// DO NOT MODIFY shapes.cpp/h BUT shapes.*.cpp/h
///// AS THESE FILES WILL BE ERASED BY THE MAKEFILE

#include "shapes.h"

// Definición e inicialización de miembros estáticos
set< CANDIDATE > shapes::_base_ejes = set< CANDIDATE >();
map<unsigned int, string> shapes::_desc_nodo = map<unsigned int, string>();
map<unsigned int, string> shapes::_desc_eje = map<unsigned int, string>();
map<string, unsigned int> shapes::_rdesc_nodo = map<string, unsigned int>();
map<string, unsigned int> shapes::_rdesc_eje = map<string, unsigned int>();
unsigned int shapes::MAX = 10;

shapes::shapes(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn) {
  _name = name;
  
  for (unsigned int i = 0; i < shap.size(); ++i) {
    _desc_nodo.insert(pair<unsigned int, string>(i+1, shap[i]));
    _rdesc_nodo.insert(pair<string, unsigned int>(shap[i], i+1));
/*    cout << i+1 << ' ' << shap[i] << endl;*/
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
  
/*  cout << "Maximo " << _desc_nodo.size() + _desc_eje.size() << endl;*/
  inicial();
//   MAX = _desc_nodo.size();
}

shapes::shapes(const shapes & s) {
  _name = s._name;
  _nodos = s._nodos;
  _ejes = s._ejes;
  _relacion_nodos = s._relacion_nodos;
  _base_ejes = s._base_ejes;
  _desc_nodo = s._desc_nodo;
  _desc_eje = s._desc_eje;
  _rdesc_nodo = s._rdesc_nodo;
  _rdesc_eje = s._rdesc_eje;  
//   MAX = s._desc_nodo.size();
}

void shapes::inicial() {
  clear();
  agregarNodo("object");
}

float shapes::sizeNorm() const {
  return ((size() * 1.) / (MAX *  _desc_nodo.size() * 1.));
}

unsigned int shapes::agregarNodo(const string & s) {
    assert(_rdesc_nodo.find(s) != _rdesc_nodo.end());

    unsigned int real = _rdesc_nodo.find(s)->second;
    unsigned int nuevo = _nodos.size() + 1;
    _nodos.insert(nuevo);
    _relacion_nodos.insert(pair<unsigned int, unsigned int>(nuevo, real));
    
    return nuevo;
}

string shapes::tipoNodo(const unsigned int x) const {
  assert(_nodos.find(x) != _nodos.end());
  
  unsigned int cual = (*(_relacion_nodos.find(x))).second;
  return _desc_nodo[cual];
}

unsigned int shapes::nuevo_codigo_nodo(const unsigned int x) const {
    assert(_nodos.find(x) != _nodos.end());

    unsigned int cual = (*(_relacion_nodos.find(x))).second;
    return cual + MAX;
}

unsigned int shapes::agregarNodo(const unsigned int & s) {
    assert(s > MAX);

    unsigned int real = s - MAX;
    unsigned int nuevo = _nodos.size() + 1;
    _nodos.insert(nuevo);
    _relacion_nodos.insert(pair<unsigned int, unsigned int>(nuevo, real));
    
    return nuevo;
}

unsigned int shapes::agregarNodoID(const unsigned int & n, const string & s) {
    _nodos.insert(n);
    cout << "s " << s << ' ' << n << endl;
    _relacion_nodos.insert(pair<unsigned int, unsigned int>(n, _rdesc_nodo.find(s)->second));
    
    return n;
}

unsigned int shapes::mapear(const unsigned int& i) const {
  unsigned int res = 0;

  if (i > MAX) {
    res = i - MAX;
//     cout << "MAX " << MAX << ' ' << res << endl;
  }
  else {
//     for (map<unsigned int,unsigned int>::const_iterator it = _relacion_nodos.begin(); it != _relacion_nodos.end(); ++it) {
// 	  cout << "RN " << it->first << ' ' << it->second << endl;
// 	}
// 	
    res = _relacion_nodos.find(i)->second;
//     cout << i << ' ' <<  res << endl;
  }
    
  return res;
}

void shapes::agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s){
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

void shapes::agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s){
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

set<unsigned int> shapes::nodosUtilizados() const {
    return _nodos;
}

vector< CANDIDATE > shapes::ejesNoUtilizados() const {
    vector< CANDIDATE > res;
    
    cout << "Ej_no_ut" << endl;

    if (_nodos.size() < MAX) {
      // Busco ejes entre nodos ya existentes que no haya usado
      for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
	for (set<unsigned int>::const_iterator q = _nodos.begin(); q != _nodos.end(); ++q)
	  if (*p != *q)
	    for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	      bool found = false;
	      if (r->second == "shape") {
		// Reviso que ho haya otro nodos shape ya asociado
		for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		  found = ((_desc_eje.find(it->third)->second == "shape") and (it->first == *p));
		}
		
		// Reviso que el nodo shape al cual quiero asociar no tenga ya un objeto asociado
		for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		  found = ((_desc_eje.find(it->third)->second == "shape") and (it->second == *q));
		}
	      }
	      else {
		// Reviso que no tenga ya un enlace
		found = (_ejes.find(CANDIDATE(*q, *p, r->first)) != _ejes.end());
		
		// Reviso que ho haya otro nodos object ya asociado
		for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		  found = ((_desc_eje.find(it->third)->second == "on") and (it->first == *p));
		}
	      }
	      
	      if (!found and (_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, _relacion_nodos.find(*q)->second, r->first)) != _base_ejes.end()) and (_ejes.find(CANDIDATE(*p, *q, r->first)) == _ejes.end()))
		res.push_back(CANDIDATE(*p, *q, r->first));
	    }
      
    // Busco ejes con un nodo nuevo que puede salir de cualquier nodo
    for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
      for (map<unsigned int, string>::const_iterator q = _desc_nodo.begin(); q != _desc_nodo.end(); ++q)
	for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	    bool found = false;
	    if (r->second == "shape") {
	      for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		found = ((_desc_eje.find(it->third)->second == "shape") and (it->first == *p));
	      }
	    }
	    else {
	       for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		found = ((_desc_eje.find(it->third)->second == "on") and (it->first == *p));
	      }
	    }
	    
	    if (!found and (_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, q->first, r->first)) != _base_ejes.end()))
	      res.push_back(CANDIDATE(*p, MAX + q->first, r->first));
	}
    }
    
    return res;
}

void shapes::clear() {
  _name = "";
  _nodos.clear();
  _ejes.clear();
  _relacion_nodos.clear();
}

bool shapes::empty() const {
    return (_nodos.empty());
}

unsigned int shapes::size() const {
    return (_nodos.size() + _ejes.size());
}

string shapes::graph_g(void) const {
    cout << "XP" << endl;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
      cout << "v " << *p << " " << _desc_nodo.find(_relacion_nodos.find(*p)->second)->second << endl;
    }
    for (set< CANDIDATE >::iterator p = _ejes.begin(); p != _ejes.end(); p++)
      cout << "e " << p->first << " " << p->second << " " << _desc_eje.find(p->third)->second << endl;
    cout << endl;
    return "";
}

void shapes::imprime(ostream &salida) const {
     salida << "Nodos: ";
     for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
	 if (_relacion_nodos.find(*p) == _relacion_nodos.end()) {
	   cout << "ERROR1! no encontrado " << *p << endl;
	 }
	 else if (_desc_nodo.find(_relacion_nodos.find(*p)->second) == _desc_nodo.end()) {
		cout << "ERROR2! no encontrado " << *p << ' ' << _relacion_nodos.find(*p)->second << endl;
	 }
         salida << '(' << *p << ',' << _desc_nodo.find(_relacion_nodos.find(*p)->second)->second << ')';
     }
     
     salida << endl;
     salida << "Ejes: ";
     for (set< CANDIDATE >::iterator p = _ejes.begin(); p != _ejes.end(); p++)
         salida << '(' << p->first << ',' << p->second << ',' << _desc_eje.find(p->third)->second << ')';
     salida << endl;
}

ostream& operator<<(ostream& os, const shapes& s) {
   s.imprime(os);
   return os;
}

multimap<unsigned int, pair<unsigned int, unsigned int> > shapes::ejes_a_multimap() const {
  multimap<unsigned int, pair<unsigned int, unsigned int> > res;
  
  for (set< CANDIDATE >::const_iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
    res.insert(pair<unsigned int, pair<unsigned int, unsigned int> >(it->first, pair<unsigned int, unsigned int>(it->second,it->third)));
  }
  
  return res;
}

bool shapes::operator==(const shapes& s) const {
    cout << "OP==" << endl << *this << endl << s << endl;

    #ifndef NO_ISOMORFISM

        bool done = false;
        if ((_nodos.size() == s._nodos.size()) and (_ejes.size() == s._ejes.size())) {
	  if (igual(s)) {
	    done = true;
	    cout << "==I" << *this << endl << s << endl;
	  }
	  else {
            shapes copia(*this);
            map<unsigned int, vector<unsigned int> > v = darPosibilidades(s);

            multimap<unsigned int, pair<unsigned int, unsigned int> > a = ejes_a_multimap(), b = s.ejes_a_multimap();

            posibilidades<unsigned int> op(v, a, b);

            for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) and !done; ++q) {
                cout << "!" << endl;

                shapes nueva_subestructura = copia.reasignarNodos(*q);

                cout << s << endl << nueva_subestructura << endl;

                done = s.igual(nueva_subestructura);

		if (done)
                  cout << "==" << *this << endl << s << endl << nueva_subestructura << endl;
            }
        }
      }

     #else

        bool done = false;
        if ((_nodos.size() == s._nodos.size()) and (_ejes.size() == s._ejes.size())) {
            done = this->igual(s);
        }

     #endif
    
     cout << "/OP==" << done << endl;

    return done;
}

bool shapes::igual(const shapes& s) const {
// 	for (set<unsigned int>::const_iterator it = _nodos.begin(); it != _nodos.end(); ++it) {
// 	  cout << "N " << *it << endl;
// 	}
// 	
//         for (set< CANDIDATE >::const_iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
// 	  cout << "E " << it->first << ' ' << it-> second << ' ' << it->third << endl;
// 	}
// 	
// 	for (map<unsigned int,unsigned int>::const_iterator it = _relacion_nodos.begin(); it != _relacion_nodos.end(); ++it) {
// 	  cout << "RN " << it->first << ' ' << it->second << endl;
// 	}
// 	
// 	cout << "¿¿¿" << endl;
// 	
// 	for (set<unsigned int>::const_iterator it = s._nodos.begin(); it != s._nodos.end(); ++it) {
// 	  cout << "N " << *it << endl;
// 	}
// 	
//         for (set< CANDIDATE >::const_iterator it = s._ejes.begin(); it != s._ejes.end(); ++it) {
// 	  cout << "E " << it->first << ' ' << it-> second << ' ' << it->third << endl;
// 	}
// 	
// 	for (map<unsigned int,unsigned int>::const_iterator it = s._relacion_nodos.begin(); it != s._relacion_nodos.end(); ++it) {
// 	  cout << "RN " << it->first << ' ' << it->second << endl;
// 	}
 
    return ((_nodos == s._nodos) and (_ejes == s._ejes) and (_relacion_nodos == s._relacion_nodos));
}

vector< CANDIDATE > shapes::posibilidades_totales() { // Candidatos unicos...
    vector< CANDIDATE > v;
    
    for (set< CANDIDATE >::const_iterator it = _base_ejes.begin(); it != _base_ejes.end(); ++it)
      v.push_back(*it);
    
    return v;
}

shapes shapes::reasignarNodos(const map<unsigned int, unsigned int> & v) {
    shapes nuevo(*this);
    nuevo.clear();
    
    cout << "Reasignar" << endl;
    
    map<unsigned int, unsigned int> dicc;
            
    for (map<unsigned int, unsigned int>::const_iterator p = v.begin(); p != v.end(); ++p) {
	cout << "L " << (*p).first << ' ' << (*p).second << endl;
	unsigned int o = _relacion_nodos.find((*p).first)->second;
	cout << o << endl;
	
// 	cout << *this << endl;
// 	
// 	for (map<unsigned int, string>::const_iterator it = _desc_nodo.begin(); it != _desc_nodo.end(); ++it) {
// 	  cout << it->first << ' ' << it->second << endl;
// 	}
	
        dicc[(*p).first] = nuevo.agregarNodoID((*p).second, _desc_nodo.find(o)->second);
    }
    for (set< CANDIDATE >::iterator p = _ejes.begin(); p != _ejes.end(); p++) {
// 	cout << "E " << dicc[p->first] << ' ' << dicc[p->second] << ' ' << p->third << endl;
        nuevo.agregarEje(dicc[p->first], dicc[p->second], p->third);
    }
    
// 	cout << "done" << endl;
    return nuevo;
}

bool shapes::cubiertoPor(const shapes& s) const {
    bool res = true;
    
    // Reviso ejes validos
    for (set< CANDIDATE >::const_iterator p = s._ejes.begin(); (p != s._ejes.end()) and res; p++)
        res = ((_ejes.find(CANDIDATE(p->first, p->second, p->third)) != _ejes.end()) and (_relacion_nodos.find(p->first)->second == s._relacion_nodos.find(p->first)->second) and (_relacion_nodos.find(p->second)->second == s._relacion_nodos.find(p->second)->second));
    
    return res;
}

map<unsigned int, vector<unsigned int> > shapes::darPosibilidades(const shapes& s) const {
    map<unsigned int, vector<unsigned int> > res;
    
    cout << *this << endl;
    bool out = false;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
      if (!out) {
	cout << "Nodo est " << *p << endl;
        bool algo = false;
        for (set<unsigned int>::const_iterator q = s._nodos.begin(); q != s._nodos.end(); q++) {
	  if (_relacion_nodos.find(*p)->second == s._relacion_nodos.find(*q)->second) {
	    cout << "Nodo ins " << *q << endl;

	    // Reviso si tambien comparten los ejes correctos
	    set< CANDIDATE > usados;
	    bool found = true;
	    for (set< CANDIDATE >::iterator it =  _ejes.begin(); (it != _ejes.end()) and found; ++it) {
	      if ((it->first == *p) and (it->second != *p)) {
		found = false;
// 		cout << "FoundP1a " << it->first << ' ' << it->second << ' ' << it->third << endl;
		for (set< CANDIDATE >::iterator its =  s._ejes.begin(); (its != s._ejes.end()) and !found; ++its) {
// 		  cout << "FoundP1b " << its->first << ' ' << its->second << ' ' << its->third << endl;
		  if ((its->first == *q) and (it->third == its->third) and (_relacion_nodos.find(it->second)->second == s._relacion_nodos.find(its->second)->second) and (usados.find(*its) == usados.end())) {
		    usados.insert(*its);
		    cout << "Found1 " << it->first << ' ' << it->second << ' ' << it->third << endl;
		    cout << "Found1 " << its->first << ' ' << its->second << ' ' << its->third << endl;
		    found = true;
		  }
		}
	      }
	      else if ((it->second == *p) and (it->first != *p)) {
		found = false;
// 		cout << "FoundP2a " << it->first << ' ' << it->second << ' ' << it->third << endl;
		for (set< CANDIDATE >::iterator its =  s._ejes.begin(); (its != s._ejes.end()) and !found; ++its) {
// 		  cout << "FoundP2b " << its->first << ' ' << its->second << ' ' << its->third << endl;
		  if ((its->second == *q) and (it->third == its->third) and (_relacion_nodos.find(it->first)->second == s._relacion_nodos.find(its->first)->second) and (usados.find(*its) == usados.end())) {
		    usados.insert(*its);
		    cout << "Found2 " << it->first << ' ' << it->second << ' ' << it->third << endl;
		    cout << "Found2 " << its->first << ' ' << its->second << ' ' << its->third << endl;
		    found = true;
		  }
		}
	      }
	      else if ((it->first == *p) and (it->second == *p)) {
		found = false;
// 		cout << "FoundP3a " << it->first << ' ' << it->second << ' ' << it->third << endl;
		for (set< CANDIDATE >::iterator its =  s._ejes.begin(); (its != s._ejes.end()) and !found; ++its) {
// 		  cout << "FoundP3b " << its->first << ' ' << its->second << ' ' << its->third << endl;
		  if ((its->second == *q) and (its->first == *q) and (it->third == its->third) and (_relacion_nodos.find(it->first)->second == s._relacion_nodos.find(its->first)->second) and (usados.find(*its) == usados.end())) {
		    usados.insert(*its);
		    cout << "Found3 " << it->first << ' ' << it->second << ' ' << it->third << endl;
		    cout << "Found3 " << its->first << ' ' << its->second << ' ' << its->third << endl;
		    found = true;
		  }
		}
	      }
	    }

	    if (found) {
	      cout << "dp " << *p << ' ' << *q << endl;
	      res[*p].push_back(*q);
	      algo = true;
	    }
	  }
        }
	if (!algo) {
	  res[*p] = vector<unsigned int>();
	  out = true;
	}
      }
      else {
	res[*p] = vector<unsigned int>();
      }
    }
	
// 	cout << "M " << res.size() << endl;
    return res;
}

vector<unsigned int> shapes::darPosibilidades(const shapes& donde, const string& s) const {
    vector<unsigned int> res;

    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
      bool algo = false;
      if (_relacion_nodos.find(*p)->second == _rdesc_nodo.find(s)->second) {
	for (set<unsigned int>::const_iterator q = donde._nodos.begin(); q != donde._nodos.end(); q++) {
	    if (_relacion_nodos.find(*p)->second == donde._relacion_nodos.find(*q)->second) {
// 		cout << "dp " << *p << ' ' << *q << endl;
		res.push_back(*q);
		algo = true;
	    }
	}
      }
    }
  
    return res;	
}

void shapes::random(const unsigned int how_many) {
    inicial();
    for (unsigned int i = 0; i < how_many; ++i) {
        vector< CANDIDATE > nuevo = ejesNoUtilizados();

        int sel = intAzar(1, nuevo.size());
        cout << "RANDOM: " << nuevo[sel-1].first << ' ' << nuevo[sel-1].second << ' ' << nuevo[sel-1].third << endl;
        agregarEje(nuevo[sel-1].first, nuevo[sel-1].second, nuevo[sel-1].third);
    }
}

shapes& shapes::operator=(const shapes& other) {
    _name = other._name;
    _nodos = other._nodos;
    _ejes = other._ejes;
    _relacion_nodos = other._relacion_nodos;

    return *this;
}

vector<unsigned int> shapes::hojas() const {
    // En el caso particular de shapes, las hojas son los nodos "object"
    vector<unsigned int> sol;

    for (set<unsigned int>::const_iterator it = _nodos.begin(); it != _nodos.end(); ++it) {
        map<unsigned int, unsigned int>::const_iterator aca = _relacion_nodos.find(*it);
        if (aca != _relacion_nodos.end()) {
            map<unsigned int, string>::const_iterator itd = _desc_nodo.find(aca->second);
            if ((itd != _desc_nodo.end()) and (itd->second == "object"))
                sol.push_back(*it);
        }
    }

    return sol;
}

string shapes::forma(const unsigned int x) const {
    string res = "";

    bool found = false;
    for (set< CANDIDATE >::const_iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
        if ((it->third == 2) and (it->first == x)) {
            map<unsigned int, unsigned int>::const_iterator aca = _relacion_nodos.find(it->second);
            map<unsigned int, string>::const_iterator itd = _desc_nodo.find(aca->second);
            res = itd->second;
            found = true;
        }
    }

    return res;
}

void shapes::borrarNodo(const unsigned int & ini) {
    _nodos.erase(ini);

    set< CANDIDATE >::const_iterator it = _ejes.begin();
    while (it != _ejes.end()) {
        if ((it->first == ini) or (it->second == ini)) {
            _ejes.erase(it);
	    it = _ejes.begin();
        }
        else{
	  ++it;
	}
    }
    
    map<unsigned int,unsigned int>::iterator itrn = _relacion_nodos.begin();
    bool found = false;
    while ((itrn != _relacion_nodos.end()) and !found) {
      if (itrn->first == ini) {
	_relacion_nodos.erase(itrn);
	found = true;
      }
      else
	++itrn;
    }
}

unsigned int shapes::top() const {
  // Returns the top object id of the stack of shapes
  set<unsigned int> sol(_nodos);

  for (set< CANDIDATE >::const_iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
    sol.erase(it->second);
  }  
  
  set<unsigned int>::iterator ip = sol.begin();
  return *ip;
}

unsigned int shapes::bottom() const {
  // Returns the bottom object id of the stack of shapes
  set<unsigned int> sol(_nodos);

  for (set< CANDIDATE >::const_iterator it = _ejes.begin(); it != _ejes.end(); ++it) {
    if (it->third == 1) {
      sol.erase(it->first);
    }
    else
      if (it->third == 2)
	sol.erase(it->second);
  }  
  
  set<unsigned int>::iterator ip = sol.begin();
  return *ip;
}

shapes shapes::subarbol(const unsigned int sel, bool updown) const {
    bool found = true;
    shapes res(*this);
    res.clear();
    
    if (updown) {
      unsigned int act = sel;
      unsigned int pos = res.agregarNodo("object");
      unsigned int old_pos = pos;        
      while (found) {
	// Search for the shape of pos
	string form = forma(act);
	if (form != "") {
	  unsigned int npos = res.agregarNodo(form);
	  res.agregarEje(pos, npos, "shape");
	}

	// Search for the on node of pos (x -> pos)
	found = false;
	for (set< CANDIDATE >::const_iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
	  if (it->second == act) {
	    act = it->first;
	    old_pos = pos;
	    pos = res.agregarNodo("object");
	    res.agregarEje(pos, old_pos, "on");
	    found = true;
	  }
	}
      }
    }
    else {
      // Search for the on node of pos (x -> pos)
      unsigned int act = sel;
      unsigned int pos = res.agregarNodo("object");  
      unsigned int old_pos = pos;        
      
      while (found) {
	// Search for the shape of pos
	string form = forma(act);
	if (form != "") {
	  cout << "#" << endl;
	  unsigned int npos = res.agregarNodo(form);
	  res.agregarEje(pos, npos, "shape");
	}

	// Search for the on node of pos (pos -> x)
	found = false;
	for (set< CANDIDATE >::const_iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
	  if ((it->first == act) and (it->third == 1)) {
	    act = it->second;
	    old_pos = pos;
	    pos = res.agregarNodo("object");
	    res.agregarEje(old_pos, pos, "on");
	    found = true;
	  }
	}
      }
    }
    
    return res;
}

void shapes::merge(const shapes& other, const unsigned int x, const unsigned int y, const string &edge) {
  bool found = true;
    
  unsigned int pos = agregarNodo("object");
  agregarEje(x, pos, "on");
  unsigned int act = y;
  unsigned int old_pos = pos;        
  
  while (found) {
    // Search for the shape of pos
    string form = other.forma(act);
    if (form != "") {
      unsigned int npos = agregarNodo(form);
      agregarEje(pos, npos, "shape");
    }

    // Search for the on node of pos (pos -> x)
    found = false;
    for (set< CANDIDATE >::const_iterator it = other._ejes.begin(); (it != other._ejes.end()) and !found; ++it) {
      if ((it->first == act) and (it->third == 1)) {
	act = it->second;
	old_pos = pos;
	pos = agregarNodo("object");
	agregarEje(old_pos, pos, "on");
	found = true;
      }
    }
  }
}

bool shapes::cambiar_forma(const unsigned int x) {
  bool done = false;
  
  vector< pair<string, unsigned int> > opciones;
  string miforma = _desc_nodo.find(_relacion_nodos.find(x)->second)->second;
  for (map<string, unsigned int>::const_iterator it =  _rdesc_nodo.begin(); it != _rdesc_nodo.end(); ++it) {
    if ((it->first != "object") and (it->first != miforma)) {
      opciones.push_back(pair<string, unsigned int>(it->first,it->second));
    }
  }
  
  if (! opciones.empty()) {
    int sel = intAzar(1, opciones.size());
    _relacion_nodos[x] = opciones[sel-1].second;
    done = true;
  }
  
  return done;
}

bool shapes::ejeTipoUsado(CANDIDATE ej, const shapes & other) const {
  bool found = false;
  unsigned int p;
  if (ej.first >= MAX)
    p = ej.first - MAX;
  else
    p = other._relacion_nodos.find(ej.first)->second;
  
  unsigned int q;
  if (ej.second >= MAX)
    q = ej.second - MAX;
  else
    q = other._relacion_nodos.find(ej.second)->second;
  
//   cout << p << ' ' << q << ' ' << MAX << endl;
  
  for (set< CANDIDATE >::const_iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
    if ((_relacion_nodos.find(it->first)->second == p) and (_relacion_nodos.find(it->second)->second == q) and (it->third == ej.third))
      found = true;
  }
  
  return found;
}
