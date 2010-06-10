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
    _relacion_nodos.insert(pair<unsigned int, unsigned int>(n, _rdesc_nodo.find(s)->second));
    
    return n;
}

unsigned int shapes::mapear(const unsigned int& i) const {
  unsigned int res = 0;

  if (i > _desc_nodo.size()) {
    res = i - MAX;
  }
  else {
    res = _relacion_nodos.find(i)->second;
  }
    
  return res;
}

void shapes::agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
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
        cerr << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
    }
}

void shapes::agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s){
    // Verifico que alguno de los nodos del eje ya exista en el grafo
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
        cerr << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
    }
}

set<unsigned int> shapes::nodosUtilizados() const {
    return _nodos;
}

vector< CANDIDATE > shapes::ejesNoUtilizados() const {
    vector< CANDIDATE > res;

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
     for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++)
         salida << '(' << *p << ',' << _desc_nodo.find(_relacion_nodos.find(*p)->second)->second << ')';
     
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

bool shapes::operator==(const shapes& s) const {
    bool done = false;
    if ((_nodos.size() == s._nodos.size()) and (_ejes.size() == s._ejes.size())) {
        shapes copia(s);
        vector< vector<unsigned int> > v = darPosibilidades(s);
        posibilidades<unsigned int> op(v);
            
        for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) and !done; ++q) {
            shapes nueva_subestructura = copia.reasignarNodos(*q);
            
            done = this->igual(nueva_subestructura);
	    
	    if (!igual(s) and done)
	      cout << "==" << *this << endl << s << endl << nueva_subestructura << endl;
        }
    }
    
    return done;
}

bool shapes::igual(const shapes& s) const {
    return ((_nodos == s._nodos) and (_ejes == s._ejes) and (_relacion_nodos == s._relacion_nodos) and (_base_ejes == s._base_ejes) and (_desc_nodo == s._desc_nodo) and (_desc_eje == s._desc_eje) and (_rdesc_nodo == s._rdesc_nodo) and (_rdesc_eje == s._rdesc_eje));
}

vector< CANDIDATE > shapes::posibilidades_totales() { // Candidatos unicos...
    vector< CANDIDATE > v;
    
    for (set< CANDIDATE >::const_iterator it = _base_ejes.begin(); it != _base_ejes.end(); ++it)
      v.push_back(*it);
    
    return v;
}

shapes shapes::reasignarNodos(const vector<unsigned int> & v) {
    shapes nuevo(*this);
    nuevo.clear();
    
    map<unsigned int, unsigned int> dicc;
            
    unsigned int i = 0;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); ++p) {
        dicc[*p] = nuevo.agregarNodoID(v[i], _desc_nodo.find(_relacion_nodos.find(*p)->second)->second);
	++i;
    }
    for (set< CANDIDATE >::iterator p = _ejes.begin(); p != _ejes.end(); p++) {
        nuevo.agregarEje(dicc[p->first], dicc[p->second], p->third);
    }
    
    return nuevo;
}

bool shapes::cubiertoPor(const shapes& s) const {
    bool res = true;
    
    // Reviso ejes validos
    for (set< CANDIDATE >::const_iterator p = s._ejes.begin(); (p != s._ejes.end()) and res; p++)
        res = ((_ejes.find(CANDIDATE(p->first, p->second, p->third)) != _ejes.end()) and (_relacion_nodos.find(p->first)->second == s._relacion_nodos.find(p->first)->second) and (_relacion_nodos.find(p->second)->second == s._relacion_nodos.find(p->second)->second));
    
    return res;
}

vector< vector<unsigned int> > shapes::darPosibilidades(const shapes& s) const {
    vector< vector<unsigned int> > res(_nodos.size());
    
    unsigned int i = 0;
    for (set<unsigned int>::iterator p = _nodos.begin(); p != _nodos.end(); p++) {
        vector<unsigned int> w;
        for (set<unsigned int>::const_iterator q = s._nodos.begin(); q != s._nodos.end(); q++) {
            if (_relacion_nodos.find(*p)->second == s._relacion_nodos.find(*q)->second) {
                w.push_back(*q);
            }
        }
        res[i] = w;
        ++i;
    }
    return res;
}
