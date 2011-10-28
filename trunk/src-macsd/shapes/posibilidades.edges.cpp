#include "posibilidades.h"

class SOLUTION;

posibilidades::posibilidades(const SOLUTION * mia, const SOLUTION * tuya) {
    _mine = mia;
    _your = tuya;
}

posibilidades::iterator posibilidades::iterator::operator++() {  
   if (_pos >= 0) { // I'm using directed edges ONLY and rooted trees
	do {
		if (_pos == 0) {
			// The stack _trabajo should be empty
			// Put something there
			// Start with the first one: the root!
			// TODO: Choose the one with less possibilities
			info primero;
			primero.nodes_so_far = map<unsigned int,unsigned int>();
			primero.pila_nodos = stack<unsigned int>();
			primero.current_node  = _root;
			map<unsigned int, vector<CANDIDATE> >::iterator it = _p_nmine->find(_root);
			if (it != _p_nmine->end()) {
				vector<CANDIDATE>::const_iterator it_v = (*it).second.begin();
				if (_p_nmine->find((*it_v).second) != _p_nmine->end()) {
					primero.pila_nodos.push((*it_v).second);
				}
	
				// Select first node possibilities
				tuplax3<string> cannonical = _p_mine->cannonical_edge(*it_v);
				primero.current_edge = *it_v;
				map<tuplax3<string>, vector<CANDIDATE> >::iterator ini_fin = _p_cyour->find(cannonical);
				for (vector<CANDIDATE>::iterator x = (*ini_fin).second.begin(); (ini_fin != _p_cyour->end()) and (x != (*ini_fin).second.end()); x++) {
					_initial.insert((*x).first);
				}

				// Determine a
				// Take the first one to start with
				set<unsigned int>::iterator its = _initial.begin();
				if (its != _initial.end()) {
					unsigned int ini = *its;
					_initial.erase(its);
					primero.nodes_so_far.insert(pair<unsigned int,unsigned int>(primero.current_node, ini));
	
					primero.opciones_ejes = stack<CANDIDATE>();

					// Look for all possibilities in your structure
					ini_fin = _p_cyour->find(cannonical);
					for (vector<CANDIDATE>::iterator x = (*ini_fin).second.begin(); x != (*ini_fin).second.end(); x++) {
						if (ini == (*x).first) { // first == current_node
							primero.opciones_ejes.push(*x);
						}
					}
	
					CANDIDATE seje = primero.opciones_ejes.top();
					primero.ya_usados.insert(seje);
					primero.opciones_ejes.pop();
					primero.nodes_so_far.insert(pair<unsigned int,unsigned int>((*it_v).second, seje.second));

					_trabajo.push(primero);
					_pos++;
					_show();
				}
				else _pos = -1;
			}
			else { // Just one node, no edges...
cout<<"1"<<endl;
				// Select first node possibilities
				set<unsigned int> nns =_p_your->nodosUtilizados();
                                for (set<unsigned int>::iterator x = nns.begin(); x != nns.end(); x++) {
                                        if (_p_mine->tipoNodo(primero.current_node) == _p_your->tipoNodo(*x))
						_initial.insert(*x);
                                }
				_pos++;
			}
		}
		else {
		   _show();
		   if (!_trabajo.empty()) {
cout<<"a"<<endl;
			// Take first option
			info useme = _trabajo.top();
			
			// Go a step forward
			// First, check if current node has another edge we did not study
			map<unsigned int, vector<CANDIDATE> >::iterator x = _p_nmine->find(useme.current_node);
			bool found = false;
			bool back = false;
			CANDIDATE edge;
			for (vector<CANDIDATE>::iterator itmm = (*x).second.begin(); (itmm != (*x).second.end()) and (!found); itmm++) {
				found = (useme.nodes_so_far.find((*itmm).second) == useme.nodes_so_far.end());
				edge = *itmm;
			}
			
			if (found) {
				cout << "New edge" << endl;
				// New edge to study
				// We are still working on the same current node
				while (!useme.opciones_ejes.empty())
				     	useme.opciones_ejes.pop();
				useme.current_edge = edge;
				tuplax3<string> cannonical = _p_mine->cannonical_edge(edge);

		                // Look for all possibilities in your structure
                		map<tuplax3<string>, vector<CANDIDATE> >::iterator ini_fin = _p_cyour->find(cannonical);
				unsigned int ini = (*(useme.nodes_so_far.find(useme.current_node))).second;
		                for (vector<CANDIDATE>::iterator x = (*ini_fin).second.begin(); (ini_fin != _p_cyour->end()) and (x != (*ini_fin).second.end()); x++) {
		                        if (ini == (*x).first) { // first == current_node
						if (useme.ya_usados.find(*x) == useme.ya_usados.end()) {
	                		                useme.opciones_ejes.push(*x);
						}
		                        }
                		}

				if (!useme.opciones_ejes.empty()) {
			                CANDIDATE seje = useme.opciones_ejes.top();
					useme.ya_usados.insert(seje);
			                useme.opciones_ejes.pop();
			                useme.nodes_so_far.insert(pair<unsigned int,unsigned int>(edge.second, seje.second));
					if (_p_nmine->find(edge.second) != _p_nmine->end()) {
						useme.pila_nodos.push(edge.second);
					}

					cout << "Selected edge (" << seje.first << "," << seje.second << "," << seje.third << ")" << endl; 

					_trabajo.push(useme);
				}
				else {
					back = true;
				}
			}
			else {
				cout << "New node" << endl;
				// Second, check for the next node
				// Change current_node
				if (!useme.pila_nodos.empty()) {
					useme.current_node = useme.pila_nodos.top();
					useme.pila_nodos.pop();

                	        	// Take the first edge
	                	        map<unsigned int, vector<CANDIDATE> >::iterator x = _p_nmine->find(useme.current_node);

					while (!useme.opciones_ejes.empty())
		                                useme.opciones_ejes.pop();

					CANDIDATE edge = (*((*x).second.begin()));
					useme.current_edge = edge;
					tuplax3<string> cannonical = _p_mine->cannonical_edge(edge);

	                                // Look for all possibilities in your structure
        	                        map<tuplax3<string>, vector<CANDIDATE> >::iterator ini_fin = _p_cyour->find(cannonical);
					unsigned int ini = (*(useme.nodes_so_far.find(useme.current_node))).second;
                        	        for (vector<CANDIDATE>::iterator x = (*ini_fin).second.begin(); (ini_fin != _p_cyour->end()) and (x != (*ini_fin).second.end()); x++) {
                                	        if (ini == (*x).first) { // first == current_node
							if (useme.ya_usados.find(*x) == useme.ya_usados.end()) {
	                                        	        useme.opciones_ejes.push(*x);
							}
	                                        }
        	                        }
	
        	                        if (!useme.opciones_ejes.empty()) {
                	                        CANDIDATE seje = useme.opciones_ejes.top();
						useme.ya_usados.insert(seje);
                        	                useme.opciones_ejes.pop();
                                	        useme.nodes_so_far.insert(pair<unsigned int,unsigned int>(edge.second, seje.second));

	                                        _trabajo.push(useme);
        	                        }
                	                else {
						// Backtracking
						back = true;
	                                }	
				}
				else {
					back = true;
				}
			}
			if (back) {
				// Backtracking
				// Go back and change the option
				cout << "Go back!" << endl;
				while (!_trabajo.empty() and _trabajo.top().opciones_ejes.empty()) {
	                                _trabajo.pop();
				}
				if (!_trabajo.empty()) {
					// Change option
					info useme = _trabajo.top();
					CANDIDATE nc = useme.current_edge;
					nc.first = useme.nodes_so_far[useme.current_edge.first];
					nc.second = useme.nodes_so_far[useme.current_edge.second];
					useme.ya_usados.erase(nc);
					_trabajo.top().opciones_ejes.pop();
					CANDIDATE seje = useme.opciones_ejes.top();
					useme.opciones_ejes.pop();
					useme.ya_usados.insert(seje);
					map<unsigned int, unsigned int>::iterator here = useme.nodes_so_far.find(useme.current_edge.second);
					(*here).second = seje.second;
                                        _trabajo.push(useme);
				}
                       }
		}
		else {
cout<<"b"<<endl;
			if (!_initial.empty()) {
				// Change first node to the next option
				info primero;
		                primero.nodes_so_far = map<unsigned int,unsigned int>();
		                primero.pila_nodos = stack<unsigned int>();
			        primero.current_node = _root;

 				map<unsigned int, vector<CANDIDATE> >::iterator it = _p_nmine->find(_root);
				if (it != _p_nmine->end()) {
			                vector<CANDIDATE>::const_iterator it_v = (*it).second.begin();
 			                primero.pila_nodos = stack<unsigned int>();
                        	        if (_p_nmine->find((*it_v).second) != _p_nmine->end()) {
					        primero.pila_nodos.push((*it_v).second);
					}
	
			                // Determine a
			                set<unsigned int>::iterator its = _initial.begin();
		        	        unsigned int ini = *its;
		                	_initial.erase(its);
			                primero.nodes_so_far.insert(pair<unsigned int,unsigned int>(primero.current_node, ini));

        	        		primero.opciones_ejes = stack<CANDIDATE>();
					primero.current_edge = *it_v;

 		                        tuplax3<string> cannonical = _p_mine->cannonical_edge(*it_v);
                	                map<tuplax3<string>, vector<CANDIDATE> >::iterator ini_fin = _p_cyour->find(cannonical);
 		        	        for (vector<CANDIDATE>::iterator x = (*ini_fin).second.begin(); x != (*ini_fin).second.end(); x++) {
                 			        if (ini == (*x).first) { // first == current_node
		                        	        primero.opciones_ejes.push(*x);
			                        }
			                }
	
        	        		CANDIDATE seje = primero.opciones_ejes.top();
					primero.ya_usados.insert(seje);
			                primero.opciones_ejes.pop();
					primero.nodes_so_far.insert(pair<unsigned int,unsigned int>((*it_v).second, seje.second));

			                _trabajo.push(primero);
				}
				else { // Just one node, no edges...
					 // Determine a
                                        set<unsigned int>::iterator its = _initial.begin();
                                        unsigned int ini = *its;
                                        _initial.erase(its);
                                        primero.nodes_so_far.insert(pair<unsigned int,unsigned int>(primero.current_node, ini));
        	                }
			}
			else _pos = -1;
		    }
		}
	    } while ((!_finished()) and (_pos >= 0));
	    if (_finished()) _pos++;
	//    if (_initial.empty() and _trabajo.empty()) {
//		_pos = -1;
//	    }
    }
cout << _pos << endl;

    return *this;
}

map<unsigned int,unsigned int> posibilidades::iterator::operator*() {
   info actual = _trabajo.top();

	    cout << "OPCION: ";
            map<unsigned int, unsigned int> x = actual.nodes_so_far;
            for (map<unsigned int, unsigned int>::iterator i = x.begin(); i != x.end(); i++) {
                cout << (*i).first << " -> " << (*i).second << endl;
            }
            cout << endl;


   return actual.nodes_so_far;
}

bool posibilidades::iterator::_finished() const {
	bool res = false;
	if (!_trabajo.empty()) {
		info useme = _trabajo.top();
		res = (useme.nodes_so_far.size() == _p_mine->cantNodos());
	}
	return res;
}

posibilidades::iterator::iterator(const SOLUTION * mia, const SOLUTION * tuya, map<tuplax3<string>, vector<CANDIDATE> > * p_cmine, map<tuplax3<string>, vector<CANDIDATE> > * p_cyour, map<unsigned int, vector<CANDIDATE> > * p_nmine,  map<unsigned int, vector<CANDIDATE> > * p_nyour, unsigned int root, const int pos) {
    _pos = pos;
    _p_mine = mia;
    _p_your = tuya;
    _p_cmine = p_cmine;
    _p_cyour = p_cyour;
    _p_nmine = p_nmine;
    _p_nyour = p_nyour;
    _root = root;
}

void posibilidades::iterator::_show() const {
	stack<info> copia(_trabajo);
	cout << "--------------------------" << endl;
	cout << "initial: ";
	for(set<unsigned int>::const_iterator it = _initial.begin(); it != _initial.end(); it++) {
                cout << *it << " ";
        }
	cout << endl;
	while (!copia.empty()) {
		cout << "====================================" << endl;
		info useme = copia.top();
		cout << "pila_nodos: ";
		while (!useme.pila_nodos.empty()) {
			cout << useme.pila_nodos.top() << " ";
			useme.pila_nodos.pop();
		}
		cout << endl;
		cout << "current_node: " << useme.current_node << endl;
		cout << "nodes_so_far: " << endl;
		for (map<unsigned int, unsigned int>::iterator it = useme.nodes_so_far.begin(); it != useme.nodes_so_far.end(); it++) {
			cout << (*it).first << " -> " << (*it).second << endl;
		}
		cout << "opciones_ejes: ";
		while (!useme.opciones_ejes.empty()) {
			CANDIDATE info = useme.opciones_ejes.top();
                        cout << "(" << info.first << "," << info.second << "," << info.third << ")" << endl;
                        useme.opciones_ejes.pop();
                }
                cout << endl;
		cout << "ya_usados: ";
		for(set<CANDIDATE>::const_iterator it = useme.ya_usados.begin(); it != useme.ya_usados.end(); it++) {
			cout << "(" << (*it).first << "," << (*it).second << "," << (*it).third << ")" << endl;
        	}
	        cout << endl;
		copia.pop();
	}
	cout << "--------------------------" << endl;
}
                
posibilidades::iterator posibilidades::begin() {
	posibilidades::iterator res = end();

	// Create maps
	set<CANDIDATE> ejes = _mine->ejesUtilizados();
	set<unsigned int> nodos = _mine->nodosUtilizados();
	for (set<CANDIDATE>::iterator it = ejes.begin(); it != ejes.end(); it++) {
		tuplax3<string> can = _mine->cannonical_edge(*it);
		map<tuplax3<string>, vector<CANDIDATE> >::iterator itc = _cmine.find(can);
		if (itc == _cmine.end()) {
			vector<CANDIDATE> vec;
			vec.push_back(*it);
			_cmine.insert(pair<tuplax3<string>, vector<CANDIDATE> >(can, vec));
		}
		else {
			(*itc).second.push_back(*it);
		}
		map<unsigned int, vector<CANDIDATE> >::iterator itu = _nmine.find((*it).first);
                if (itu == _nmine.end()) {
                        vector<CANDIDATE> vec;
                        vec.push_back(*it);
                        _nmine.insert(pair<int, vector<CANDIDATE> >((*it).first, vec));
                }
                else {
                        (*itu).second.push_back(*it);
                }
		set<unsigned int>::iterator its = nodos.find((*it).second);
		if (its != nodos.end()) {
			nodos.erase(its);
		}
	}

	ejes.clear();
	ejes = _your->ejesUtilizados();
        for (set<CANDIDATE>::iterator it = ejes.begin(); it != ejes.end(); it++) {
                tuplax3<string> can = _your->cannonical_edge(*it);
                map<tuplax3<string>, vector<CANDIDATE> >::iterator itc = _cyour.find(can);
                if (itc == _cyour.end()) {
                        vector<CANDIDATE> vec;
                        vec.push_back(*it);
                        _cyour.insert(pair<tuplax3<string>, vector<CANDIDATE> >(can, vec));
                }
                else {
                        (*itc).second.push_back(*it);
                }
		map<unsigned int, vector<CANDIDATE> >::iterator itu = _nyour.find((*it).first);
                if (itu == _nyour.end()) {
                        vector<CANDIDATE> vec;
                        vec.push_back(*it);
                        _nyour.insert(pair<int, vector<CANDIDATE> >((*it).first, vec));
                }
                else {
                        (*itu).second.push_back(*it);
                }
        }

	// DEBUG

	for (map<tuplax3<string>, vector<CANDIDATE> >::iterator it = _cmine.begin(); it !=_cmine.end(); it++) {
		cout << "(" << (*it).first.first << "," << (*it).first.second << "," << (*it).first.third << ")" << ":" << endl;
		for (vector<CANDIDATE>::iterator itv = (*it).second.begin(); itv != (*it).second.end(); itv++) {
			cout << "(" << (*itv).first << "," << (*itv).second << "," << (*itv).third << ")" << endl;
		}
	}

	cout << endl;

 	for (map<tuplax3<string>, vector<CANDIDATE> >::iterator it = _cyour.begin(); it !=_cyour.end(); it++) {
                cout << "(" << (*it).first.first << "," << (*it).first.second << "," << (*it).first.third << ")" << ":" << endl;
                for (vector<CANDIDATE>::iterator itv = (*it).second.begin(); itv != (*it).second.end(); itv++) {
                        cout << "(" << (*itv).first << "," << (*itv).second << "," << (*itv).third << ")" << endl;
                }
        }

	cout << endl;

	for (map<unsigned int, vector<CANDIDATE> >::iterator it = _nmine.begin(); it !=_nmine.end(); it++) {
                cout << (*it).first << ":" << endl;
                for (vector<CANDIDATE>::iterator itv = (*it).second.begin(); itv != (*it).second.end(); itv++) {
                        cout << "(" << (*itv).first << "," << (*itv).second << "," << (*itv).third << ")" << endl;
                }
        }

	cout << endl;

        for (map<unsigned int, vector<CANDIDATE> >::iterator it = _nyour.begin(); it !=_nyour.end(); it++) {
                cout << (*it).first << ":" << endl;
                for (vector<CANDIDATE>::iterator itv = (*it).second.begin(); itv != (*it).second.end(); itv++) {
                        cout << "(" << (*itv).first << "," << (*itv).second << "," << (*itv).third << ")" << endl;
                }
        }

	for (set<unsigned int>::iterator it = nodos.begin(); it != nodos.end(); it++) {
		cout << "Root:" << *it << " ";
	}
	cout << endl;

	if (nodos.begin() != nodos.end())
		res = ++(posibilidades::iterator(_mine, _your, &_cmine, &_cyour, &_nmine, &_nyour, *(nodos.begin()), 0));
	return res;
}

posibilidades::iterator posibilidades::end() {
    return posibilidades::iterator(-1);
}
