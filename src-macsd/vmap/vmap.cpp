#include "vmap.h"

//---------------------------------------------

vmap::vmap(set<string>* bn, set< pair< pair<string,string>, unsigned int> >* be, const unsigned int& cant) {
    _base_nodos = bn;
    _base_ejes = be;
    _max_nodos = cant;
    
    // Tomo un nodo al azar
    unsigned int num = intAzar(1,_base_nodos->size());
    unsigned int i = 1;
    cout << "@ " << num << ' ' << _base_nodos->size() << endl;
    set<string>::const_iterator it = _base_nodos->begin();
    for (; (it != _base_nodos->end()) and (i < num); ++it, ++i);
    if (it == _base_nodos->end()) {
	cout << "Horrrorrr!" << endl;
    }
    agregarNodo(1,*it);
}
 
//---------------------------------------------
vmap::vmap(const vmap& s) {
    _nodos = s._nodos;
    _ejes = s._ejes;
    _base_nodos = s._base_nodos;
    _base_ejes = s._base_ejes;
    _max_nodos = s._max_nodos;
}
 
//---------------------------------------------
 
void vmap::agregarEje(const unsigned int& ini, const unsigned int& fin, const unsigned int& s) {
    unsigned int nini = ini;
    
    if ((ini < _max_nodos) and !(_base_nodos->find(_nodos[ini]) != _base_nodos->end())) {
        cout << "ERROR: " << ini << endl;
    }
    if (!(_base_nodos->find(_nodos[fin]) != _base_nodos->end())) {
        cout << "ERROR: " << fin << endl;
    }
    
    cout << "Antes: " << *this << endl;
    
    if (ini > _max_nodos) {
      cout << "AB" << endl;
      unsigned int i = 0;
      nini = nini - _max_nodos - 1;
      set<string>::const_iterator it = _base_nodos->begin();
      for (; i < nini; ++it, ++i);
      nini = _nodos.size() + 1;
      agregarNodo(_nodos.size() + 1,*it);
    }
    
    assert((_base_nodos->find(_nodos[nini]) != _base_nodos->end()) && (_base_nodos->find(_nodos[fin]) != _base_nodos->end()) && (_nodos.find(nini) != _nodos.end()) && (_nodos.find(fin) != _nodos.end()));
     
    cout << '(' << nini << ',' << fin << ',' << s << ')' << endl;
    
    if (valido(nini,fin,s)) {
      _ejes.insert(CANDIDATE(nini,fin,s));
    }
    else {
      cout << "Error!!!!" << endl;
    }
    
    cout << "Despues: " << *this << endl;
}

//---------------------------------------------

void vmap::clear() {
    _nodos.clear();
    _ejes.clear();
    
    // Tomo un nodo al azar
    unsigned int num = intAzar(1,_base_nodos->size());
    unsigned int i = 0;
    set<string>::const_iterator it = _base_nodos->begin();
    for (; (it != _base_nodos->end()) and (i < num); ++it, ++i);
    agregarNodo(1,*it);
}

//---------------------------------------------

void vmap::agregarNodo(const int& nod, const string& cad) {
  _nodos[nod] = cad;
}

//---------------------------------------------

unsigned int vmap::size() const {
    return _nodos.size() + _ejes.size();
}

//---------------------------------------------

double vmap::sizeNorm() const {
    return ((size() * 1.) / ((_base_nodos->size() + _base_ejes->size()) * 1.));
}

//---------------------------------------------

bool vmap::ejeUsado(const unsigned int& ini, const unsigned int& fin, const unsigned int& s) const {
    // Revisar (i,f) y (f,i)
    map<unsigned int, string>::const_iterator i = _nodos.find(ini);
    map<unsigned int, string>::const_iterator f = _nodos.find(fin);
    cout << "Ver: " << ini << ' ' << fin << endl;
    cout << i->second << ' ' << f->second << endl;
    assert((i != _nodos.end()) && (f != _nodos.end()) && (_base_nodos->find(i->second) != _base_nodos->end()) && (_base_nodos->find(f->second) != _base_nodos->end()));
    
    return ((_nodos.find(ini) != _nodos.end()) && (_nodos.find(fin) != _nodos.end()) && ((_ejes.find(CANDIDATE(ini,fin,s)) != _ejes.end()) or (_ejes.find(CANDIDATE(fin,ini,s)) != _ejes.end())));
}

//---------------------------------------------

set<unsigned int> vmap::nodosUtilizados() const {
  set<unsigned int> res;
  for (map<unsigned int,string>::const_iterator it = _nodos.begin(); it != _nodos.end(); ++it)
    res.insert(it->first);
  return res;
}

//---------------------------------------------

vector< CANDIDATE > vmap::ejesNoUtilizados() const {
    vector< CANDIDATE > diff;
    
    cout << "YO: " << *this << endl;

    for (set< pair< pair<string,string>, unsigned int> >::const_iterator p = _base_ejes->begin(); p != _base_ejes->end(); p++) {
      for (map<unsigned int,string>::const_iterator q = _nodos.begin(); q != _nodos.end(); ++q) {
	if (q->second == p->first.first) {
	  // Lo del found es para no repetir nombres de nodos, solo para VMAP, quitar para WWW
	  bool found = false;
	  
	  // Si tengo un nodo compatible
	  for (map<unsigned int,string>::const_iterator r = _nodos.begin(); r != _nodos.end(); ++r) {
	      // Y no existe el eje
	      if (r->second == p->first.second) {
		if (!(ejeUsado(q->first, r->first, p->second)) and !(ejeUsado(r->first, q->first, p->second))) {
		  diff.push_back(CANDIDATE(q->first, r->first, p->second));
		  cout << '(' << q->first << ',' << r->first << ',' << p->second << ')' << endl;
		}
		found = true;
	      }
	  }
	  
	  // Agregar un nodo nuevo
	  if (!found) {
	    unsigned int i = 0;
	    for (set<string>::const_iterator it = _base_nodos->begin(); *it != p->first.second; ++it, ++i);
	    unsigned int value = _max_nodos + i + 1;
	    diff.push_back(CANDIDATE(value, q->first, p->second));
	    cout << '(' << value << ',' << q->first << ',' << p->second << ')' << endl;
	  }
	}
	
	if (q->second == p->first.second) {
	  // Lo del found es para no repetir nombres de nodos, solo para VMAP, quitar para WWW
	  bool found = false;
	  // Si tengo un nodo compatible
	  for (map<unsigned int,string>::const_iterator r = _nodos.begin(); r != _nodos.end(); ++r) {
	      // Y no existe el eje
	      if (r->second == p->first.first)
		found = true;
	  }
	  
	  // Agregar un nodo nuevo
	  if (!found) {
	    unsigned int i = 0;
	    for (set<string>::const_iterator it = _base_nodos->begin(); *it != p->first.first; ++it, ++i);
	    unsigned int value = _max_nodos + i + 1;
	    diff.push_back(CANDIDATE(value, q->first, p->second));
	    cout << '(' << value << ',' << q->first << ',' << p->second << ')' << endl;
	  }
	}  
      }
     }
         
    return diff;
}

//---------------------------------------------
 
void vmap::imprime(ostream &salida) const {
     salida << "Nodos: " << endl;
     for (map<unsigned int,string>::const_iterator p = _nodos.begin(); p != _nodos.end(); p++)
         salida << p->first << ' ' << p->second << endl;
     salida << endl << "Ejes: " << endl;
     for (set< CANDIDATE >::const_iterator p = _ejes.begin(); p != _ejes.end(); p++)
         salida << '(' << p->first << ',' << p->second << ')';
     salida << endl << endl;
}

//---------------------------------------------

bool vmap::operator!=(const vmap& s) const {
    return (!(*this == s));
}

//---------------------------------------------

unsigned int vmap::cantNodos() const {
    return _base_nodos->size();
}

//---------------------------------------------

unsigned int vmap::cantEjes() const {
    return 1;
}

//---------------------------------------------

bool vmap::valido(const unsigned int& ini, const unsigned int& fin, const unsigned int& s) const {
    bool res = false;
    map<unsigned int, string>::const_iterator i = _nodos.find(ini);
    map<unsigned int, string>::const_iterator f = _nodos.find(fin);
    
    if ((i != _nodos.end()) && (f != _nodos.end()))
      res = ((_base_ejes->find(pair< pair<string,string>, unsigned int>(pair<string,string>(i->second,f->second), s)) != _base_ejes->end()) or (_base_ejes->find(pair< pair<string,string>, unsigned int>(pair<string,string>(f->second,i->second), s)) != _base_ejes->end()));
    return res;
}

//---------------------------------------------

vector< CANDIDATE > vmap::posibilidades_totales() const {
    vector< CANDIDATE > diff;
    
    unsigned int n1 = 1;
    for (set<string>::const_iterator p = _base_nodos->begin(); p != _base_nodos->end(); ++p, ++n1) {
      unsigned int n2 = 1;
      for (set<string>::const_iterator q = _base_nodos->begin(); q != _base_nodos->end(); ++q, ++n2) {
	// Un solo tipo de eje
	if (_base_ejes->find(pair< pair<string,string>, unsigned int>(pair<string,string>(*p,*q),0)) != _base_ejes->end()) {
	  diff.push_back(CANDIDATE(n1,n2,0));
	}
      }
    }
    
    return diff;
}

//---------------------------------------------

vector< vector<unsigned int> > vmap::darPosibilidades(const vmap& s) const {
   vector< vector<unsigned int> > res(_nodos.size());
    
    unsigned int i = 0;
    for (map<unsigned int,string>::const_iterator p = _nodos.begin(); p != _nodos.end(); p++) {
        vector<unsigned int> w;
        for (map<unsigned int,string>::const_iterator q = s._nodos.begin(); q != s._nodos.end(); q++) {
            if (p->second == q->second) {
                w.push_back(q->first);
//                 cout << *q;// '+' << *(s._desc.find(*q)) << endl;
            }
        }
        res[i] = w;
        i++;
    }
    return res;
}

//---------------------------------------------

vmap vmap::reasignarNodosFijo(const vector<unsigned int>& v) const {
    vmap nuevo(_base_nodos, _base_ejes, _max_nodos);
    
    map<unsigned int, unsigned int> dicc;
            
    unsigned int i = 0;
    for (map<unsigned int,string>::const_iterator p = _nodos.begin(); p != _nodos.end(); p++) {
//         cout << *p << "-> " << v[i] << endl;
        nuevo.agregarNodo(v[i], p->second);
        dicc[p->first] = v[i];
        i++;
    }
    for (set< CANDIDATE >::const_iterator p = _ejes.begin(); p != _ejes.end(); p++) {
//         cout << "D " << p->first << " " << p->second << endl;
//         cout << "E " << dicc[p->first] << " " << dicc[p->second] << " " << p->third << endl;
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

//---------------------------------------------

vmap vmap::reasignarNodos(vector<unsigned int> v) const {
    vmap nuevo(_base_nodos, _base_ejes, _max_nodos);
    
    map<unsigned int, unsigned int> dicc;
            
    for (unsigned int i = 0; i < v.size(); i++) {
        map<unsigned int,string>::const_iterator p = _nodos.begin();
        for (unsigned int j = 0; j < v[i] - 1; j++) p++;
//         cout << v[i] << endl;
        nuevo.agregarNodo(i + 1, p->second);
        dicc[p->first] = i + 1;
    }
    for (set< CANDIDATE >::const_iterator p = _ejes.begin(); p != _ejes.end(); p++) {
//         cout << "D " << dicc[p->first] << " " << dicc[p->second] << " " << p->third << endl;
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

//---------------------------------------------

bool vmap::cubiertoPor(const vmap& s) const {
    bool res = true;
    
    for (set< CANDIDATE >::const_iterator p = s._ejes.begin(); (p != s._ejes.end()) && res; p++) {
        map<unsigned int, string>::const_iterator v = _nodos.find(p->first);
        map<unsigned int, string>::const_iterator w = s._nodos.find(p->first);
	map<unsigned int, string>::const_iterator x = _nodos.find(p->second);
        map<unsigned int, string>::const_iterator y = s._nodos.find(p->second);
        res = (ejeUsado(p->first, p->second, p->third) && (*v == *w) && (*x == *y));
    }
    
    return res;
}

//---------------------------------------------

bool vmap::operator==(const vmap& s) const {
    bool done = false;
//     cout << "EN==" << endl;
    if ((_nodos.size() == s._nodos.size()) && (_ejes.size() == s._ejes.size())) {
        vmap copia(s);
        vector< vector<unsigned int> > v = darPosibilidades(s);
        posibilidades<unsigned int> op(v);
            
//         cout << "Veamos... " << endl;
        for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//             for (unsigned int j = 0; j < (*q).size(); j++)
//                 cout << (*q)[j] << ' ';
//             cout << endl;
            vmap nueva_subestructura = copia.reasignarNodos(*q);
//             cout << *this << endl << s << endl << nueva_subestructura << endl;
            done = this->igual(nueva_subestructura);
        }
    }
    
    return done;
}

bool vmap::igual(const vmap& s) const {
    return ((_nodos == s._nodos) && (_ejes == s._ejes));
}

//---------------------------------------------
                
ostream& operator<<(ostream& os, const vmap& s) {
     s.imprime(os);
     return os;
}
