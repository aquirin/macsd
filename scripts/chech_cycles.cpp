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
                
			ejes.insert(pair<unsigned int, unsigned int>(atoi(desde.c_str()),atoi(hasta.c_str())));
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
		
		stack< pair<unsigned int, unsigned int> > pila;
		bool res = false;
		for (unsigned int i = 0; (i < nodos.size()) and !res; ++i) {
		  for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); it != ejes.end(); ++it) {
		    if (it->first == nodos[i]) {
		      if (it->first == it->second)
			res = true;
		      else
			pila.push(pair<unsigned int, unsigned int>(it->first, it->second));
		    }
		  }
		  
		  while ((pila.size() > 0) and !res) {
		    pair<unsigned int, unsigned int> op = pila.top();
		    pila.pop();

		    for (multimap<unsigned int, unsigned int>::iterator it = ejes.begin(); (it != ejes.end()) and !res; ++it) {
		      if (it->first == op.second) {
			if (it->second == op.first)
			  res = true;
			else
			  pila.push(pair<unsigned int, unsigned int>(op.first, it->second));
		      }
		    }
		  }
		}
		
		if (res)
		  cout << "Ciclo!" << endl;
		
		nodos.clear();
		ejes.clear();
            }
        }
    }
    
    arch.close();
    
    return 0;
}
