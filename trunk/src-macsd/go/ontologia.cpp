#include "ontologia.h"

//---------------------------------------------

go::go(string name, set<unsigned int>* bn, set< tuplax3<unsigned int> >* be, map<unsigned int,string>* d) {
    _base_nodos = bn;
    _base_ejes = be;
    _desc = d;
 
    _nodos.insert(3673);
    agregarEje(3674,3673,0);
    agregarEje(5575,3673,0);
    agregarEje(8150,3673,0);
    _nodo = 3673;
}
 
//---------------------------------------------
go::go(const go& s) {
    _nodos = s._nodos;
    _ejes = s._ejes;
    _base_nodos = s._base_nodos;
    _base_ejes = s._base_ejes;
    _desc = s._desc;
    _nodo = s._nodo;
}
 
//---------------------------------------------
 
void go::agregarEje(const unsigned int ini, const unsigned int fin, const unsigned int s) {
    if (_desc->find(ini) == _desc->end()) {
        cout << "ERROR: " << ini << endl;
        exit(1);
    }
    if ((fin != 0) and (_desc->find(fin) == _desc->end())) {
        cout << "ERROR: " << fin << endl;
        exit(1);
    }
    
//     cout << ini << ' ' << fin << endl;
    
    assert((_desc->find(ini) != _desc->end()) and ((fin == 0) or (_desc->find(fin) != _desc->end())));
     
    if (_nodos.find(ini) == _nodos.end()) {
        _nodos.insert(ini);
    }
    if ((fin != 0) and (_nodos.find(fin) == _nodos.end())) {
        _nodos.insert(fin);
    }
    
    if (fin != 0)
      _ejes.insert(tuplax3<unsigned int>(ini,fin,s));
        
//     // Agrego padres...
//     for (set< tuplax3<unsigned int> >::const_iterator p = _base_ejes->begin(); p != _base_ejes->end(); p++) {
//       if ((p->first == ini) and (!ejeUsado(p->first,p->second,p->third))) {
// 	// Los ejes son (hijo,padre)
// // 	_nodos.insert(p->second);
// // 	_ejes.insert(tuplax3<unsigned int>(p->first,p->second,p->third));
// 	agregarEje(p->first,p->second,p->third);
// 	cout << "+" << p->first << ' ' << p->second << endl;
//       }
//    }
//    
  int current;

  stack<int> still_new;

  still_new.push(ini);
  
  while (! still_new.empty()) {
    current = still_new.top();
//     cout << "+" << current << endl;
    still_new.pop();
    for (set< tuplax3<unsigned int> >::const_iterator p = _base_ejes->begin(); p != _base_ejes->end(); p++) {
      if ((p->first == current) and (!ejeUsado(p->first,p->second,p->third))) {
	// Los ejes son (hijo,padre)
	agregarEje(p->first,p->second,p->third);
	still_new.push(p->second);
      }
    }
  }
}

//---------------------------------------------

void go::clear() {
    _nodos.clear();
    _ejes.clear();
    
    _nodos.insert(3673);
    agregarEje(3674,3673,0);
    agregarEje(5575,3673,0);
    agregarEje(8150,3673,0);
}

//---------------------------------------------

float go::nivelRelativo(const go& s) const {
//     return (((_ejes.size() - s._ejes.size()) * 1.) / (_ejes.size() * 1.));
    float tam = (((_ejes.size() - s._ejes.size()) * 1.) / (_ejes.size() * 1.));
    float actual = 0.;
    
    // Me fijo cuales son las hojas de la subestructura
    set<unsigned int> hojas_sub;
    for (set<unsigned int>::iterator p = s._nodos.begin(); p != s._nodos.end(); p++) {
        bool found = false;
        for (set< tuplax3<unsigned int> >::iterator q = s._ejes.begin(); (q != s._ejes.end()) && !found; q++) {
            found = (q->second == *p);
        }
        if (!found)
            hojas_sub.insert(*p);
    }
//     cout << hojas_sub.size() << endl;
    
    for (set<unsigned int>::iterator p = hojas_sub.begin(); p != hojas_sub.end(); p++) {
        // Me fijo cual es el nivel relativo de cada hoja
        unsigned int cual = *p;        
        unsigned int nivel_rel = 1;
        bool found = true;
        while (found) {
            found = false;
            set< tuplax3<unsigned int> >::iterator q;
            for (q = _ejes.begin(); (q != _ejes.end()) and !found; q++) {
                found = (q->second == cual);
            }
            if (found) {
                q--;
                cual = q->first;
                nivel_rel++;
            }
        }
        
        // Me fijo cuales es el nivel absoluto de cada hoja

        unsigned int nivel_abs = 1;
        while (cual != 3673) {
            bool found = false;
            set< tuplax3<unsigned int> >::iterator q;
//             cout << "CUAL: " << cual;
            for (q = _ejes.begin(); (q != _ejes.end()) and !found; q++) {
                found = (q->first == cual);
            }
            if (!found) {
                cout << "ERROR: " << cual << endl;
		exit(1);
            }
            q--;
            cual = q->second;
//             cout << " " << cual << endl;
            nivel_abs++;
        }
//         cout << "NR " << nivel_rel << "NA " << nivel_abs << endl;        
        actual += ((nivel_rel * 1.) / (nivel_abs * 1.));
    }
//     cout << actual / (hojas_sub.size() * 1.) << endl;
    return ((actual / (hojas_sub.size() * 1.)) * tam);
}

//---------------------------------------------

void go::agregarNodo(const int nod) {
    assert(nod == 3673);
            
    _nodo = 3673;
}

//---------------------------------------------

unsigned int go::size() const {
    return _nodos.size() + _ejes.size();
}

//---------------------------------------------

double go::sizeNorm() const {
    return ((size() * 1.) / ((_base_nodos->size() + _base_ejes->size()) * 1.));
}

//---------------------------------------------

bool go::ejeUsado(const unsigned int ini, const unsigned int fin, const unsigned int s) const {
    assert((_desc->find(ini) != _desc->end()) && (_desc->find(fin) != _desc->end()) && (s == 0));
    
    return ((_nodos.find(ini) != _nodos.end()) && (_nodos.find(fin) != _nodos.end()) && (_ejes.find(tuplax3<unsigned int>(ini,fin,s)) != _ejes.end()));
}

//---------------------------------------------

bool go::nodoUsado(const unsigned int n) const {
    assert(_desc->find(n) != _desc->end());
    
    return (_nodos.find(n) != _nodos.end());
}

//---------------------------------------------

set<unsigned int> go::nodosUtilizados() const {
    return _nodos;
}

//---------------------------------------------

vector< tuplax3<unsigned int> > go::ejesNoUtilizados() const {
    vector< tuplax3<unsigned int> > diff;
    
     for (set< tuplax3<unsigned int> >::const_iterator p = _base_ejes->begin(); p != _base_ejes->end(); p++) {
//          if ((_ejes.find(*p) == _ejes.end()) && (_nodos.find(p->second) != _nodos.end()))
// 	              cout << "JJ: " << (*p).first << ' ' << (*p).second << endl;

	 if (_ejes.find(*p) == _ejes.end()) {
             diff.push_back(*p);
	 }
     }
         
    return diff;
}

//---------------------------------------------
 
void go::imprime(ostream &salida) const {
     salida << "Nodos: " << endl;
     for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); p++)
         salida << *p << ' ';
     salida << endl << "Ejes: " << endl;
     for (set< tuplax3<unsigned int> >::const_iterator p = _ejes.begin(); p != _ejes.end(); p++)
         salida << '(' << p->first << ',' << p->second << ')';
     salida << endl << endl;
     for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); p++)
         salida << (*_desc)[*p] << endl;
     salida << endl;
}

//---------------------------------------------

bool go::operator==(const go& s) const {
    return ((_nodos == s._nodos) && (_ejes == s._ejes) && (_nodo == s._nodo));
}

//---------------------------------------------

bool go::operator!=(const go& s) const {
    return (!((_nodos == s._nodos) && (_ejes == s._ejes) && (_nodo == s._nodo)));
}

//---------------------------------------------

unsigned int go::cantNodos() const {
    return _base_nodos->size();
}

//---------------------------------------------

unsigned int go::cantEjes() const {
    return 1;
}

//---------------------------------------------

bool go::valido(const unsigned int ini, const unsigned int fin, const unsigned int s) const {
    return (_base_ejes->find(tuplax3<unsigned int>(ini,fin,s)) != _base_ejes->end());
}

//---------------------------------------------
                
ostream& operator<<(ostream& os, const go& s) {
     s.imprime(os);
     return os;
}
