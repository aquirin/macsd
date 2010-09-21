/***************************************************************************
 *   Copyright (C) 2008 by Rocio Celeste Romero Zaliz   *
 *   rromero@gemini   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <iostream>
#include <fstream>
#include <cstdlib>

#include "NSGA-II.h"
#include "emocc_solution.h"
#include "global.h"

#if VERSION == V_SHAPE
#include "shapes.h"
#elif (VERSION == V_GO)
#include "ontologia.h"
#elif (VERSION == V_SCIENCEMAP)
#include "vmap.h"
#elif (VERSION == V_WWW)
#include "www.h"
#endif

using namespace std;

#if VERSION == V_GO
void leerNodos(const string& s, set<string> & nodos) {
//     0008150,biological_process
    ifstream arch;
    string cadena;

    arch.open(s.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << s << endl;
        exit(1);
    }

    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
	      nodos.insert(cadena.substr(0, 7));
            }
        }
    }

    arch.close();
}

void leerEjes(const string& s, multimap<pair<string,string>, string> & mm) {
//     0009701:0009716;0009717;0046289;0009689
// Los ejes son (hijo,padre) pero los pondre padre hijo como en los demas casos
    ifstream arch;
    string cadena;

    arch.open(s.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << s << endl;
        exit(1);
    }

    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                  unsigned int ini = 8;
                  do {
		       // Pasar de (hijo, padre) a (padre,hijo)
                       mm.insert(pair< pair<string,string>, string>(pair<string,string>(cadena.substr(ini,7),cadena.substr(0, 7)),"enlace"));
                       ini += 8;
                  }
                  while (ini < cadena.size());
            }
        }
    }

    arch.close();
}

void leeFicheroDatos(const string& fichero, const string& bpn, const string& bpe, const string& fmn, const string& fme, const string& ccn, const string& cce, vector< SOLUTION >& v) {
    ifstream arch;
    string cadena;

    vector<string> w;
    set<string> ww;
    vector<string> e;
    multimap<pair<string,string>, string> rela;

    // Leo la base de datos de GO
    leerNodos(bpn, ww);
    leerNodos(fmn, ww);
    leerNodos(ccn, ww);
    leerEjes(bpe, rela);
    leerEjes(fme, rela);
    leerEjes(cce, rela);

    e.push_back("enlace");

    for(set<string>::iterator it = ww.begin(); it != ww.end(); ++it)
      w.push_back(*it);

    unsigned int aux, aux1, aux2, codigo;
    unsigned int menos_uno = 0 - 1;

    SOLUTION data("1", w, e, rela);

    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }

    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
//                 200858_s_at|6412/protein biosynthesis/evidence IEA|3735/structural constituent of ribosome/evidence IEA|5840/ribosome/evidence IEA@5622/intracellular/evidence IEA|20
                data.clear();
                // Gene product
                aux = cadena.find('|', 0);
                string name = cadena.substr(0, aux);
                // Data
                do {
                    aux1 = cadena.find('/', aux + 1);
                    if (aux1 != menos_uno) {
                        codigo = atoi(cadena.substr(aux + 1, aux1 - aux - 1).c_str());
                        data.agregarNodo(cadena.substr(aux + 1, aux1 - aux - 1));
                        aux = aux1;
                        aux1 = cadena.find('@', aux + 1);
                        aux2 = cadena.find('|', aux + 1);
                        if ((aux1 < 0) or (aux2 < aux1))
                            aux1 = aux2;
                        aux = aux1;
                    }
                }
                while (aux1 != menos_uno);
                v.push_back(data);
 		cout << "Item: " << data << endl;
		data.clear();
            }
        }
    }

    arch.close();
}

#endif

//-------------------------------------------------------------------------

#if VERSION == V_SHAPE
void leeFicheroDatos(const string& fichero, vector<SOLUTION>& v) {
    ifstream arch;
    string cadena;

    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;
    vector<string> w(6);
    w[0] = "object";
    w[1] = "square";
    w[2] = "triangle";
    w[3] = "ellipse";
    w[4] = "rectangle";
    w[5] = "circle";
    vector<string> e(2);
    e[0] = "on";
    e[1] = "shape";
    multimap<pair<string,string>, string> rela;
    for (unsigned int i = 1; i < w.size(); ++i)
      rela.insert(pair<pair<string,string>, string>(pair<string,string>("object",w[i]), "shape"));
    rela.insert(pair<pair<string,string>, string>(pair<string,string>(w[0],w[0]), "on"));
    SOLUTION s("1", w, e, rela);
    s.clear();

    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }

    map<unsigned int,unsigned int> pos;
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

			nn = s.agregarNodo(cad);

			pos.insert(pair<unsigned int,unsigned int>(atoi(desde.c_str()),nn));
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

			s.agregarEje(pos.find(atoi(desde.c_str()))->second, pos.find(atoi(hasta.c_str()))->second, cadena);
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {;
                v.push_back(s);
                cout << s << endl;
                s.clear();
		pos.clear();
            }
        }
    }
    if (!s.empty())
        v.push_back(s);

    arch.close();
}
#endif

#if (VERSION == V_WWW) || (VERSION == V_SCIENCEMAP)
void leeFicheroDatos(const string& fichero, vector<SOLUTION>& v) {
    ifstream arch;
    string cadena;

    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;

    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }

    // Primera pasada para poner los nodos
    set<string> bw;
    set<string> be;
    multimap<pair<string,string>, string> rela;
    bool found;
    pair<multimap<pair<string,string>, string>::iterator, multimap<pair<string,string>, string>::iterator> pit;

    map<unsigned int,string> quenodo;
    string cad;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
			//  v 1 page
                        aux = cadena.rfind(' ', cadena.size());
                        cad = cadena.substr(aux + 1, cadena.size() - aux);
                        aux1 = cadena.find(' ', 0);
                        desde = cadena.substr(aux1 + 1, aux - aux1 - 1);

			bw.insert(cad);
			if (quenodo.find(atoi(desde.c_str())) == quenodo.end())
			  quenodo.insert(pair<unsigned int,string>(atoi(desde.c_str()),cad));
                        break;
		    case 'd':
			// d 1 25 word
// 			cout << cadena << endl;
                        aux = cadena.find(' ', 0);
                        aux1 = cadena.find(' ', aux + 1);
                        desde = cadena.substr(aux + 1, aux1 - aux - 1);
                        aux = cadena.rfind(' ', cadena.size());
                        hasta = cadena.substr(aux1 + 1, aux - aux1 - 1);
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);

			be.insert(cadena);

			pit = rela.equal_range(pair<string,string>(quenodo.find(atoi(desde.c_str()))->second,quenodo.find(atoi(hasta.c_str()))->second));

			found = (pit.first == rela.end());

			if (!found) {
			  for (multimap<pair<string,string>, string>::iterator it = pit.first; (it != pit.second) and !found; ++it)
			    found = (it->second == cadena);
			  found = !found;
			}

			if (found) {
			  rela.insert(pair< pair<string,string>, string>(pair<string,string>(quenodo.find(atoi(desde.c_str()))->second,quenodo.find(atoi(hasta.c_str()))->second),cadena));
// 			cout << quenodo.find(atoi(desde.c_str()))->second << ' ' << quenodo.find(atoi(hasta.c_str()))->second << ' ' << cadena << endl;
			}
			break;
		    default:
		        break;
                }
            }
	    else {
	      quenodo.clear();
            }
        }
    }

    arch.close();

    vector<string> w;
    for(set<string>::iterator it = bw.begin(); it != bw.end(); ++it) w.push_back(*it);

    vector<string> e;
    for(set<string>::iterator it = be.begin(); it != be.end(); ++it) e.push_back(*it);

    SOLUTION s("1", w, e, rela);

    s.clear();

    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }

    map<unsigned int,unsigned int> pos;
    unsigned int nn;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
			//  v 1 page
			cout << cadena << endl;

                        aux = cadena.rfind(' ', cadena.size());
                        cad = cadena.substr(aux + 1, cadena.size() - aux);
                        aux1 = cadena.find(' ', 0);
                        desde = cadena.substr(aux1 + 1, aux - aux1 - 1);

			nn = s.agregarNodo(cad);
			cout << cad << ' ' << nn << endl;
                        pos.insert(pair<unsigned int,unsigned int>(atoi(desde.c_str()),nn));
                        break;
                    case 'd':
			// d 1 25 word
			cout << cadena << endl;
                        aux = cadena.find(' ', 0);
                        aux1 = cadena.find(' ', aux + 1);
                        desde = cadena.substr(aux + 1, aux1 - aux - 1);
                        aux = cadena.rfind(' ', cadena.size());
                        hasta = cadena.substr(aux1 + 1, aux - aux1 - 1);
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);

                        s.agregarEje(pos.find(atoi(desde.c_str()))->second, pos.find(atoi(hasta.c_str()))->second, cadena);
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {
		if (!s.empty()) {
		  v.push_back(s);
		  cout << s << endl;
		  s.clear();
		  pos.clear();
		}
            }
        }
    }
    if (!s.empty())
        v.push_back(s);

    arch.close();
}
#endif

//-------------------------------------------------------------------------

int main(int argc, char *argv[]) {
//    Parametros params;
    string fichero;
    vector<SOLUTION> baseDatos;

    // Initialize the random generator
    srand(PARA.GLOB_semilla);

    // almacenamiento de parametros
    // -----------------------------
    PARA.ReadConfiguration((char *)"config.txt");

    // leemos los datos del fichero de entrada
#if (VERSION == V_SHAPE) || (VERSION == V_WWW) || (VERSION == V_SCIENCEMAP)
    leeFicheroDatos (PARA.GLOB_rutaEntrada, baseDatos);
        cout << "Leido" << endl;

#elif VERSION == V_GO
    leeFicheroDatos (PARA.GLOB_rutaEntrada, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, baseDatos);
#endif

    termination_criteria cri(PARA.GLOB_maxTiempo);
    int nic = 5;
    nsgaii<emocc_solution<SOLUTION>,SOLUTION> v(baseDatos, PARA.MOACO_numHormigas * 10, nic, cri);
    list< emocc_solution<SOLUTION> > pop;

//-------------------------------------------------------------------------

// ./emocc cluster19PFC.csv 50 1 salida 20 1

// --			1) nombre del archivo
// --			2) tama~o de la poblacion
// --			3) cantidad de iteraciones (se ignora)
// --			4) directorio destino
// --			5) tiempo en segundos
// --			6) semilla

	pop = v.execute(9999999, string(PARA.GLOB_rutaSalida) + "_EMOCC");

	// Escribo la salida
	cout << "------------------------" << endl;
        unsigned int i = 0;
        ofstream out((string(PARA.GLOB_rutaSalida) + "_EMOCC").c_str());
	for (list< emocc_solution<SOLUTION> >::iterator p = pop.begin(); p != pop.end(); p++) {
            stringstream ss;
            ss << ++i;
            cout << "** Solucion no dominada " << ss.str() << "**\n" << *p << endl;
            out << "** Solucion no dominada " << ss.str() << "**\n" <<*p << endl;
        }

	out.close();

	return EXIT_SUCCESS;
}
