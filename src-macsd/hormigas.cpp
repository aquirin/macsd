#include "hormigas.h"

//-------------------------------------------------------------------------
Hormiga::Hormiga (const vector< SOLUTION >& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0) :
	_costes(numObjetivos, 0.0),
	_support(base.size(), 0),
	_subestructura(base[0])
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

Hormiga::Hormiga (const vector<SOLUTION>& base, const unsigned int numObjetivos = 2, map<CANDIDATE, double>* ap = 0, SOLUTION sub = SOLUTION()) : _subestructura(sub), _costes(numObjetivos, 0.0), _support(base.size(), false) {
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
                vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
                
                for (int oo = 0; oo < v.size(); oo++) {
                    for (int pp = 0; pp < v[oo].size(); pp++)
                        cout << v[oo][pp] << ",";
                    cout << endl;
                }
                
                bool done = false;
                posibilidades<unsigned int> op(v);
                for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !done; ++q) {
//                     cout << "X" << endl;
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

void Hormiga::avanza(const CANDIDATE & nuevo) {
    // avanzar la hormiga
    _subestructura.agregarEje(nuevo.first, nuevo.second, nuevo.third);
    
    vector<unsigned int> temp;
    int pp = 1;
    vector<unsigned int>::iterator p = _support.begin();
    while (p != _support.end()) {
        // Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
        if (_instancias[*p].cantNodos() >= _subestructura.cantNodos()) {
            vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[*p]);
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

//-------------------------------------------------------------------------

bool Hormiga::Utilizado(const unsigned int id) const {
    set<unsigned int> lista = _subestructura.nodosUtilizados();
    
    return (lista.find(id) != lista.end());
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
    unsigned int a;
    bool something = false;
//     cin >> a;
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
      vector< vector<unsigned int> > v = _subestructura.darPosibilidades(_instancias[_support[pos]]);
      posibilidades<unsigned int> op(v);
      
      bool done = false;
      posibilidades<unsigned int>::iterator q;
      SOLUTION nueva_subestructura;
      for (q = op.begin(); (q != op.end()) && !done; ++q) {
	  nueva_subestructura = _subestructura.reasignarNodos(*q);
	  done = _instancias[_support[pos]].cubiertoPor(nueva_subestructura);
      }
      
      cout << "!! " << _subestructura << ',' << _instancias[_support[pos]] << "->" << nueva_subestructura << endl;
//       cin >> a;
      
      set<CANDIDATE> primero = _instancias[_support[pos]].ejesUtilizados();
      done = false;
//       while (!done) {
	for (set<CANDIDATE>::const_iterator it = primero.begin(); it != primero.end(); ++it) {
	  if (nueva_subestructura.nodoUsado(it->first) != nueva_subestructura.nodoUsado(it->second)) {
	    SOLUTION prototype(nueva_subestructura);
	    
	    cout << "ADD:" << it->first << ',' << it->second << ',' << it->third << endl;
	    if (! prototype.ejeUsado(it->first, it->second, it->third)) {
	      if (! nueva_subestructura.nodoUsado(it->first)) {
		prototype.agregarNodoID(it->first, _instancias[_support[pos]].tipoNodo(it->first));
	      }
	      else if (! nueva_subestructura.nodoUsado(it->second)) {
		prototype.agregarNodoID(it->second, _instancias[_support[pos]].tipoNodo(it->second));
	      }
	      #if (VERSION != V_GO) 
              prototype.agregarEje(it->first, it->second, it->third);
              #endif
                 
	      cout << prototype << endl;
// 	      cin >> a;
	      
	      bool found = true;
	      for (unsigned int i = 0; (i < _support.size()) and found; i++) {
		// Tengo que probar las combinaciones de nodos para ver si cubre la subestructura (isomorfismo)
		if (_instancias[_support[i]].cantNodos() >= prototype.cantNodos()) {
		    vector< vector<unsigned int> > v = prototype.darPosibilidades(_instancias[_support[i]]);
		    posibilidades<unsigned int> op(v);
		    
		    found = false;
		    SOLUTION la_subestructura;
		    for (posibilidades<unsigned int>::iterator q = op.begin(); (q != op.end()) && !found; ++q) {
			la_subestructura = prototype.reasignarNodos(*q);
			found = _instancias[_support[i]].cubiertoPor(la_subestructura);
		    }
		    
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
// 	}
      }
      if (something) {
	_subestructura = nueva_subestructura;
	_costeValido = false;         
	_candidatos = _subestructura.ejesNoUtilizados();
      }
    }
}
