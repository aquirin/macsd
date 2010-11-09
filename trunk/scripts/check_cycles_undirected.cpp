#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <fstream>
#include <utility>
#include <iostream>
#include <stdio.h>
#include <list>
#include <stdlib.h>
#include <math.h>
#include <malloc.h>

using namespace std;
                          
int main(int argc, char *argv[]) {  
    string fichero(argv[1]);
    ifstream arch;
    string cadena;
    
    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;
            
    vector<unsigned int> nodos;
    multimap<unsigned int, unsigned int> ejes;
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    string cad;
    unsigned int nn;
    multimap<unsigned int, unsigned int>::iterator it;
    pair<multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> pit;
    
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
//                     v 1 object
                        aux = cadena.rfind(' ', cadena.size());
                        cad = cadena.substr(aux + 1, cadena.size() - aux);
                        aux1 = cadena.find(' ', 0);
                        desde = cadena.substr(aux1 + 1, aux - aux1 - 1);
			
			nodos.push_back(atoi(desde.c_str()));
// 			cout << "Nodo " << desde << endl;
				
			break;
//                     case 'e':
                    case 'd':
    //                     d 1 3 on
                        aux = cadena.find(' ', 0);
                        aux1 = cadena.find(' ', aux + 1);
                        desde = cadena.substr(aux + 1, aux1 - aux - 1);
                        aux = cadena.rfind(' ', cadena.size());
                        hasta = cadena.substr(aux1 + 1, aux - aux1 - 1);
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);
                
			pit = ejes.equal_range(atoi(desde.c_str()));
			
			it = pit.first;
			
			while ((it != ejes.end()) and (it != pit.second) and (it->second != atoi(hasta.c_str()))) {
			  ++it;
			}
			
			if ((it == ejes.end()) or ((it != ejes.end()) and (it->second != atoi(hasta.c_str()))))
			  ejes.insert(pair<unsigned int, unsigned int>(atoi(desde.c_str()),atoi(hasta.c_str())));
			
			pit = ejes.equal_range(atoi(hasta.c_str()));
			
			it = pit.first;
			
			while ((it != ejes.end()) and (it != pit.second) and (it->second != atoi(desde.c_str()))) {
			  ++it;
			}
			
			if ((it == ejes.end()) or ((it != ejes.end()) and (it->second != atoi(desde.c_str()))))
			  ejes.insert(pair<unsigned int, unsigned int>(atoi(hasta.c_str()),atoi(desde.c_str())));
// 			cout << "Eje " << desde <<  ' ' << hasta << endl;
			
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {
		cout << "Nodos:" << endl;
		for (unsigned int i = 0; i < nodos.size(); ++i)
		  cout << nodos[i] << ',';
		cout << endl;
	
		cout << "Ejes:" << endl;
		for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it)
		  cout << '(' << it->first << ',' << it->second << ')';
		cout << endl;
		
		set<unsigned int> nodes_in_cycle;
		stack< list<unsigned int> > pila;
		bool res = false;
		for (unsigned int i = 0; i < nodos.size(); ++i) {
// 		  cout << "Nodo: " << nodos[i] << endl;
// 		  if (nodes_in_cycle.find(nodos[i]) == nodes_in_cycle.end()) {
		  if ((nodes_in_cycle.find(nodos[i]) == nodes_in_cycle.end()) and (ejes.count(nodos[i]) > 1)) {
		    for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
		      if (it->first == nodos[i]) {
			if (it->first == it->second)
			  res = true;
			else {
			  list<unsigned int> l;
			  l.push_back(it->first);
			  l.push_back(it->second);
			  pila.push(l);
  // 			cout << "Inicio: " << it->first << ' ' << it->second << endl;
			}
		      }
		    }

		    list<unsigned int> op;
		    while ((pila.size() > 0) and !res) {
		      op = pila.top();
		      pila.pop();
		      
//   		    cout << "OP: ";
//   		    for (list<unsigned int>::iterator lit = op.begin(); lit != op.end(); ++lit)
//   		      cout << *lit << ' ';
//   		    cout << endl;
		    
		      list<unsigned int>::iterator last = --(op.end());

		      for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); (it != ejes.end()) and !res; ++it) {
  // 			cout << it->first << ' ' << it->second << endl;
			  if (it->first == *last) {
			    // Search in the list op
			    bool found = false;
			    for (list<unsigned int>::iterator lit = op.begin(); (lit != last) and !found; ++lit) {
			      list<unsigned int>::iterator litm1 = lit;
			      ++litm1;
			      found = ((*lit == it->second) and (*litm1 == it->first));
			    }
			    for (list<unsigned int>::iterator lit = ++op.begin(); (lit != op.end()) and !found; ++lit) {
			      found = (it->second == *lit);
			    }
			    
			    if ((it->second == nodos[i]) and !found) {
			      res = true;
			      op.push_back(it->second);
			    }
			    else if (!found) {
			      list<unsigned int> cop = op;
			      cop.push_back(it->second);
			      pila.push(cop);
  // 			    cout << "Sub: " << it->first << ' ' << it->second << endl;
			    }
			  }
		      }
		    }
		    
		    if (res) {
		      cout << "Ciclo! ";
		      for (list<unsigned int>::iterator lit = op.begin(); lit != op.end(); ++lit) {
			cout << *lit << ' ';
			nodes_in_cycle.insert(*lit);
		      }
		      cout << endl;
		    }
		    
		    while (pila.size() > 0) {
		      pila.pop();
		    }
		    res = false;
		  }
		}
		nodos.clear();
		ejes.clear();
            }
        }
    }
    
    arch.close();
    
    return 0;
}