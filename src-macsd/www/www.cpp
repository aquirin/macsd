#include "www.h"

void www::inicial() {
  clear();
  agregarNodo("page");
}

vector< CANDIDATE > www::ejesNoUtilizados() const {
    vector< CANDIDATE > res;

    if (_nodos.size() < MAX) {
      // Busco ejes entre nodos ya existentes que no haya usado
      for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
	for (set<unsigned int>::const_iterator q = _nodos.begin(); q != _nodos.end(); ++q)
	  for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	    bool found = false;
	    if (r->second == "word") {
	      // Reviso que ho haya otro nodos shape ya asociado
	      for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		found = ((_desc_eje.find(it->third)->second == "word") and (it->first == *p));
	      }
	      
	      // Reviso que el nodo shape al cual quiero asociar no tenga ya un objeto asociado
	      for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		found = ((_desc_eje.find(it->third)->second == "word") and (it->second == *q));
	      }
	    }
// 	    else {
// 	      // Reviso que no tenga ya un enlace
// 	      found = (_ejes.find(CANDIDATE(*q, *p, r->first)) != _ejes.end());
// 	    }
	    
	    if (!found and (_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, _relacion_nodos.find(*q)->second, r->first)) != _base_ejes.end()) and (_ejes.find(CANDIDATE(*p, *q, r->first)) == _ejes.end()))
	      res.push_back(CANDIDATE(*p, *q, r->first));
	  }
    
    // Busco ejes con un nodo nuevo que puede salir de cualquier nodo
    for (set<unsigned int>::const_iterator p = _nodos.begin(); p != _nodos.end(); ++p)
      for (map<unsigned int, string>::const_iterator q = _desc_nodo.begin(); q != _desc_nodo.end(); ++q)
	for (map<unsigned int, string>::const_iterator r = _desc_eje.begin(); r != _desc_eje.end(); ++r) {
	    bool found = false;
	    if (r->second == "word") {
	      for (set<CANDIDATE>::iterator it = _ejes.begin(); (it != _ejes.end()) and !found; ++it) {
		found = ((_desc_eje.find(it->third)->second == "word") and (it->first == *p));
	      }
	    }
	    
	    if (!found and (_base_ejes.find(CANDIDATE(_relacion_nodos.find(*p)->second, q->first, r->first)) != _base_ejes.end()))
	      res.push_back(CANDIDATE(*p, MAX + q->first, r->first));
	}
    }
    
    return res;
}
