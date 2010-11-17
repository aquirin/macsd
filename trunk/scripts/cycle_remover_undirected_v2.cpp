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
// 		cout << "Nodos:" << endl;
// 		for (unsigned int i = 0; i < nodos.size(); ++i)
// 		  cout << nodos[i] << ',';
// 		cout << endl;
// 	
// 		cout << "Ejes:" << endl;
// 		for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it)
// 		  cout << '(' << it->first << ',' << it->second << ')';
// 		cout << endl;
		
		bool nuevo = true;
		list< pair<unsigned int, unsigned int> > eliminar;
		while (nuevo) {
		  // Creo todos los de a 2
		  multimap< pair<unsigned int, unsigned int>, list<unsigned int> > pd;
		  set<string> done;
		  
		  for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
		    pd.insert(pair< pair<unsigned int, unsigned int>, list<unsigned int> >(pair<unsigned int, unsigned int>(it->first, it->second), list<unsigned int>()));
		    stringstream ss;
		    ss << it->first << ',' << it->second;
  // 		  cout << "Done:" << ss.str() << endl;
		    done.insert(ss.str());
		  }
		  
		  // Creo todos los de a 3, 4, 5, etc...
		  multimap< pair<unsigned int, unsigned int>, list<unsigned int> > final = pd;
		  bool salir = false;
		  while (nuevo and !salir) {
		    nuevo = false;
		    multimap< pair<unsigned int, unsigned int>, list<unsigned int> > npd;
		    for (multimap< pair<unsigned int, unsigned int>, list<unsigned int> >::iterator it = pd.begin(); (it != pd.end()) and !salir; ++it) {
  // 		    cout << "Nodo " << it->first.second << endl;
		      pair<multimap<unsigned int, unsigned int>::iterator, multimap<unsigned int, unsigned int>::iterator> par = ejes.equal_range(it->first.second);
		      stringstream ss;
		      ss << it->first.first << ',';
		      list<unsigned int> intermedios = it->second;
		      bool found = false;
		      for (list<unsigned int>::iterator q = intermedios.begin(); (q != intermedios.end()) and !found; ++q) {
			ss << *q << ',';
			found = (*q == it->first.second);
		      }
		      intermedios.push_back(it->first.second);
		      ss << it->first.second << ',';

		      if ((par.first != ejes.end()) and !found) {
			multimap<unsigned int, unsigned int>::iterator fin = par.second;
			for (multimap<unsigned int, unsigned int>::iterator p = par.first; (p != fin) and !salir; ++p) {
//     			cout << it->first.second << ' ' << p->first << ' ' << p->second << endl;
			  stringstream nss;
			  nss << p->second;
			  string tog = ss.str() + nss.str();
    /*		      cout << "Done:" << tog << endl;*/
			  if (done.find(tog) == done.end()) {
			    if (it->first.first != p->second) {
			      npd.insert(pair< pair<unsigned int, unsigned int>, list<unsigned int> >(pair<unsigned int, unsigned int>(it->first.first, p->second), intermedios));
			      done.insert(tog);
			      nuevo = true;
			    }
			    else {		      
			      if (intermedios.size() != 1) {
				cout << "Ciclo! " << tog <<  endl;

				// Erase it and start over
				eliminar.push_back(pair<unsigned int,unsigned int>(p->first,p->second));
				eliminar.push_back(pair<unsigned int,unsigned int>(p->second,p->first));
				
				for (multimap<unsigned int, unsigned int>::iterator itt = ejes.begin(); itt != ejes.end(); ++itt) {
				  if ((itt->first == p->first) and (itt->second == p->second)) {
				    ejes.erase(itt);
// 				    cout << "Borrado: " << itt->first << ' ' << itt->second << endl;
				  }
				}
				for (multimap<unsigned int, unsigned int>::iterator itt = ejes.begin(); itt != ejes.end(); ++itt) {
				  if ((itt->first == p->second) and (itt->second == p->first))
				    ejes.erase(itt);
				}
				salir = true;
				nuevo = true;
			      }
			    }
			  }
			}
		      }
		    }
		    
// 		    for (multimap< pair<unsigned int, unsigned int>, list<unsigned int> >::iterator it = npd.begin(); it != npd.end(); ++it) {
// 		      final.insert(*it);
// 		      cout << it->first.first << ',';
// 		    for (list<unsigned int>::iterator p = it->second.begin(); p != it->second.end(); ++p) {
// 		      cout << *p << ',';
// 		    }
// 		    cout << it->first.second << endl;
// 		    }
		    
		    pd = npd;
		    
		    cout << "New! " << pd.size() << ' ' << ejes.size() << endl;
		  }
		  
// 		  for (multimap< pair<unsigned int, unsigned int>, list<unsigned int> >::iterator it = final.begin(); it != final.end(); ++it) {
// 		    cout << it->first.first << ',';
// 		    for (list<unsigned int>::iterator p = it->second.begin(); p != it->second.end(); ++p) {
// 		      cout << *p << ',';
// 		    }
// 		    cout << it->first.second << endl;
// 		  }
		}
		
		string fich("erase.me");
		fich += fichero;
		ofstream out(fich.c_str());
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
		out.close();
	    }
	}
    }
    
    arch.close();
    
    return 0;
}
