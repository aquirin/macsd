#include "vmap.h"

vmap::vmap(const string & name, const vector<string> & shap, const vector<string> & link, const multimap<pair<string,string>, string> & rn) {
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
	MAX = 500;
}

void vmap::inicial() {
  clear();
  agregarNodo("\"\\nMultidisciplinary\\n\"");
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
	  if ((_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, q->first, r->first)) != _base_ejes.end()))
	      res.push_back(CANDIDATE(*p, MAX + q->first, r->first));
	}
    }
    
    return res;
}
