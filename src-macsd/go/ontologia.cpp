#include "ontologia.h"

go::go(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn) {
  _name = name;
  
  for (unsigned int i = 0; i < shap.size(); ++i) {
    stringstream s(shap[i]);
    unsigned int valor;
    s >> valor;

    _desc_nodo.insert(pair<unsigned int, string>(valor, shap[i]));
    _rdesc_nodo.insert(pair<string, unsigned int>(shap[i], valor));
//    cout << valor << ' ' << shap[i] << endl;
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
  MAX = 1244;
  inicial();
}

void go::inicial() {
  clear();
  agregarNodo("0003674");
  agregarNodo("0005575");
  agregarNodo("0008150");
}

//---------------------------------------------

void go::_reconstruir_arbol(const unsigned int & s) {
  cout << "IC" << endl;
  for (set<CANDIDATE>::const_iterator p = _base_ejes.begin(); p != _base_ejes.end(); p++) {
    if ((p->second == s) and (_nodos.find(p->first) == _nodos.end())) {
      // Los ejes son (padre,hijo)
      cout << "+ " << p->first << endl;
      agregarNodo(p->first);
      agregarEje(p->first,p->second,p->third);
    }
    else if ((_nodos.find(p->first) != _nodos.end()) and (_ejes.find(CANDIDATE(p->first,p->second,p->third)) == _ejes.end())) {
      cout << "l " << p->first << endl;
      agregarEje(p->first,p->second,p->third);
    }
  }
}

unsigned int go::agregarNodo(const string & s) {
  cout << "an string " << s << endl;
  assert(_rdesc_nodo.find(s) != _rdesc_nodo.end());

  stringstream ss(s);
  unsigned int nuevo;
  ss >> nuevo;

  _nodos.insert(nuevo);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>(nuevo, nuevo));
    
  _reconstruir_arbol(nuevo);
  
  return nuevo;
}

unsigned int go::agregarNodo(const unsigned int & s) {
  cout << "an int " << s << endl;

  _nodos.insert(s);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>(s, s));

  _reconstruir_arbol(s);
  
  return s;
}

unsigned int go::agregarNodoID(const unsigned int & n, const string & s) {
  cout << "an id " << s << endl;

  stringstream ss(s);
  unsigned int nuevo;
  ss >> nuevo;
  
  assert(nuevo == n);
  
  _nodos.insert(nuevo);
  _relacion_nodos.insert(pair<unsigned int, unsigned int>(nuevo, nuevo));

  _reconstruir_arbol(nuevo);
  
  return nuevo;
}

//---------------------------------------------

void go::agregarEje(const unsigned int & ini, const unsigned int & fin, const unsigned int & s) {
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    cout << ini << ' ' << fin << ' ' << s << endl;
    assert((_nodos.find(ini) != _nodos.end()) and ((_nodos.find(fin) != _nodos.end()) or (fin > MAX)) and (_desc_eje.find(s) != _desc_eje.end()));
    
     unsigned int segundo = fin;
    if (_nodos.find(fin) == _nodos.end()) {      
      // Agrego un nuevo nodos
      segundo = agregarNodo(fin);
    }
    else {
      if (_ejes.find(CANDIDATE(ini,segundo,s)) == _ejes.end()) {
	  _ejes.insert(CANDIDATE(ini,segundo,s));
      }
      else {
	  cerr << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
      }
    }
}

void go::agregarEje(const unsigned int & ini, const unsigned int & fin, const string & s) {
    // Verifico que alguno de los nodos del eje ya exista en el grafo
    assert((_nodos.find(ini) != _nodos.end()) and ((_nodos.find(fin) != _nodos.end()) or (fin > _desc_nodo.size())) and (_rdesc_eje.find(s) != _rdesc_eje.end()));
    
    unsigned int segundo = fin;
    if (_nodos.find(fin) == _nodos.end()) {
      // Agrego un nuevo nodos
      segundo = agregarNodo(fin);
    }
    else {
      unsigned int eleje = _rdesc_eje.find(s)->second;
      if (_ejes.find(CANDIDATE(ini,segundo,eleje)) == _ejes.end()) {
	  _ejes.insert(CANDIDATE(ini,segundo,eleje));
      }
      else {
	  cerr << "ERRRRRRRRRRRRRROR " << ini << ' ' << fin << ' ' << s << endl;
      }
    }
}

//---------------------------------------------

float go::nivelRelativo(const go& s) const {
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
            for (q = _ejes.begin(); (q != _ejes.end()) and !found; q++) {
                found = (q->first == cual);
            }
            if (!found) {
                cout << "ERROR: " << cual << endl;
		exit(1);
            }
            q--;
            cual = q->second;
            nivel_abs++;
        }
        actual += ((nivel_rel * 1.) / (nivel_abs * 1.));
    }
    return ((actual / (hojas_sub.size() * 1.)) * tam);
}

//---------------------------------------------

vector<CANDIDATE> go::ejesNoUtilizados() const {
    vector<CANDIDATE> diff;
    
     for (set<CANDIDATE>::const_iterator p = _base_ejes.begin(); p != _base_ejes.end(); p++) {
	 if (_ejes.find(*p) == _ejes.end()) {
             diff.push_back(*p);
	 }
     }
         
    return diff;
}
