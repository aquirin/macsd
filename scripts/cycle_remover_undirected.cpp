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
#include <sstream>
#include <iostream>
#include <list>
#include <stdio.h>
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
    vector<string> texto;
    multimap<unsigned int, unsigned int>::iterator it;
    pair<multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> pit;
	
    while (!arch.eof()) {
	getline(arch, cadena);
	if (!arch.eof()) {
	    texto.push_back(cadena);

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
		    case 'e':
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
	      bool finiquito = false;
	      
	      while (!finiquito) {
// 		cout << "Nodos:" << endl;
// 		for (unsigned int i = 0; i < nodos.size(); ++i)
// 		  cout << nodos[i] << ',';
// 		cout << endl;
// 	
// 		cout << "Ejes:" << endl;
// 		for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it)
// 		  cout << '(' << it->first << ',' << it->second << ')';
// 		cout << endl;
		
		set<unsigned int> nodes_in_cycle;
		stack< list<unsigned int> > pila;
		bool res = false;
		list< pair<unsigned int, unsigned int> > eliminar;
		
		for (unsigned int i = 0; i < nodos.size(); ++i) {
		  cout << "Nodo: " << nodos[i] << endl;
		  if ((nodes_in_cycle.find(nodos[i]) == nodes_in_cycle.end()) and (ejes.count(nodos[i]) > 1)) {
		    for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
		      if (it->first == nodos[i]) {
			if (it->first == it->second) {
			  res = true;
			}
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
  		    cout << "OP: ";
  		    for (list<unsigned int>::iterator lit = op.begin(); lit != op.end(); ++lit)
  		      cout << *lit << ' ';
  		    cout << endl;
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
		      
		      list<unsigned int>::iterator lit1 = op.begin();
		      list<unsigned int>::iterator lit2 = ++op.begin();
		      // Rompo el ciclo
		      eliminar.push_back(pair<unsigned int,unsigned int>(*lit1,*lit2));
		      eliminar.push_back(pair<unsigned int,unsigned int>(*lit2,*lit1));
		      
		      for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it)
			if ((it->first == *lit1) and (it->second == *lit2))
			  ejes.erase(it);
			else if ((it->first == *lit2) and (it->second == *lit1))
			  ejes.erase(it);
		    }
		    
		    while (pila.size() > 0) {
		      pila.pop();
		    }
		    res = false;
		  }
		}
			
		string fich("erase.me");
		fich += fichero;
		ofstream out(fich.c_str());
		if (!eliminar.empty()) {
		  for (vector<string>::iterator it = texto.begin(); it != texto.end(); ++it) {
		    if (((*it)[0] == 'd') or ((*it)[0] == 'e')) {
		      aux = it->find(' ', 0);
		      aux1 = it->find(' ', aux + 1);
		      desde = it->substr(aux + 1, aux1 - aux - 1);
		      aux = it->rfind(' ', it->size());
		      hasta = it->substr(aux1 + 1, aux - aux1 - 1);
		      cadena = it->substr(aux + 1, cadena.size() - aux);
		      
		      bool found = false;
		      for (list< pair<unsigned int, unsigned int> >::iterator ll = eliminar.begin(); (ll != eliminar.end()) and !found; ++ll) {
			found = ((ll->first == atoi(desde.c_str())) and (ll->second == atoi(hasta.c_str())));
			if (found) {
			  out << "% Borrado " << ll->first << ' ' << ll->second << endl;
			  stringstream ss1;
			  ss1 << ll->first;
			  stringstream ss2;
			  ss2 << ll->second;
			  *it = "% Borrado " + ss1.str() + " " + ss2.str();
			}
		      }
		      if (!found)
			out << *it << endl;
		    }
		    else
		      out << *it << endl;
		  }
		  res = false;
		}
		else {
		  finiquito = true;
		  for (vector<string>::iterator it = texto.begin(); it != texto.end(); ++it)
		    out << *it << endl;
		}
		out.close();
// 		string a;
// 		cin >> a;
		eliminar.clear();
	    }
	    nodos.clear();
	    ejes.clear();
	    texto.clear();
	    string ej("mv erase.me");
	    ej += fichero;
	    ej += " ";
	    ej += fichero;
	    ej += "_nc";
// 	    cout << ej << endl;
	    system(ej.c_str());
	  }
	}
    }
    
    arch.close();
    
    return 0;
}
