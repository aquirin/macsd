#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>

#include "macs.h"

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
     
    unsigned int aux, aux1, aux2, codigo;
    unsigned int menos_uno = 0 - 1;

    // Me quedo solo con los nodos que aparecen en la base de datos
    
    set<string> esta;
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    unsigned int i = 1;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
//                 200858_s_at|6412/protein biosynthesis/evidence IEA|3735/structural constituent of ribosome/evidence IEA|5840/ribosome/evidence IEA@5622/intracellular/evidence IEA|20
                // Gene product
                aux = cadena.find('|', 0);
                string name = cadena.substr(0, aux);
                // Data
                do {
                    aux1 = cadena.find('/', aux + 1);
                    if (aux1 != menos_uno) {
                        codigo = atoi(cadena.substr(aux + 1, aux1 - aux - 1).c_str());
			esta.insert(cadena.substr(aux + 1, aux1 - aux - 1));
                        aux = aux1;
                        aux1 = cadena.find('@', aux + 1);
                        aux2 = cadena.find('|', aux + 1);
                        if ((aux1 < 0) or (aux2 < aux1))
                            aux1 = aux2;
                        aux = aux1;
                    }
                }
                while (aux1 != menos_uno);
            }
        }
    }

    arch.close();    
    
    // Busco los nodos que se pueden acceder a partir de "esta"
    set<string> todos;
    multimap<pair<string,string>, string> tejes;
    while (!esta.empty()) {
      set<string> new_esta;
      for (set<string>::iterator it = esta.begin(); it != esta.end(); ++it) {
	todos.insert(*it);
	for (multimap<pair<string,string>, string>::iterator itm = rela.begin(); itm != rela.end(); ++itm) {
	  if (itm->first.second == *it) { // Soy el hijo
	    // Agrego al padre
	    if (todos.find(itm->first.first) == todos.end())
	      new_esta.insert(itm->first.first);
	  }
	}
      }
      esta = new_esta;
    }
    
    cout << "<BASE>" << endl;
    for (set<string>::iterator it = todos.begin(); it != todos.end(); ++it) {
      for (multimap<pair<string,string>, string>::iterator itm = rela.begin(); itm != rela.end(); ++itm) {
	if (itm->first.second == *it) { // Soy el hijo
	  tejes.insert(*itm);
	  codigo = atoi(itm->first.first.c_str());
	  cout << '(' << codigo << ',';
	  codigo = atoi(itm->first.second.c_str());
	  cout << codigo << ",enlace)" << endl;
	}
      }
    }
    cout << "</BASE>" << endl;
    
    // Nodos usables
    for(set<string>::iterator it = todos.begin(); it != todos.end(); ++it) {
      w.push_back(*it);
    }
    
    // Ejes usables en tejes
   
    SOLUTION data("1", w, e, tejes);

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
 		cout << "Item: " << i++ << " = " << data << endl;
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

// funcion MAIN del proyecto

int main(int argc, char *argv[]){	
    Parametros params;
    NDominatedSet soluciones;
    string fichero;
    vector<SOLUTION> baseDatos;
  
    // Initialize the random generator    
    my_srand(PARA.GLOB_semilla);
        
    // almacenamiento de parametros        
    // -----------------------------
    PARA.ReadConfiguration(argv[1]);
    
    // leemos los datos del fichero de entrada
#if (VERSION == V_SHAPE) || (VERSION == V_WWW) || (VERSION == V_SCIENCEMAP)
    leeFicheroDatos (PARA.GLOB_rutaEntrada, baseDatos);
        cout << "Leido" << endl;

#elif VERSION == V_GO
    leeFicheroDatos (PARA.GLOB_rutaEntrada, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, baseDatos);
#endif

    
//---------------------------------------------------------------------
    map <CANDIDATE,double> aparEje;
    if (PARA.MOACO_multiheuristics == 1) {
        // STATIC
        for (unsigned int i = 0; i < baseDatos.size(); i++) {
	    map<CANDIDATE,bool> already_in_instance;
            set< CANDIDATE > tent = baseDatos[i].ejesUtilizados();
	    for (set< CANDIDATE >::iterator it = tent.begin(); it != tent.end(); ++it) {
	      already_in_instance[*it] = false;
	    }	      
            set< CANDIDATE >::iterator p = tent.begin();	    
	    
            for (; p != tent.end(); p++) {
		CANDIDATE n;
		
		cout << "!Mapear " << p->first << endl;
		n.first = baseDatos[i].mapear(p->first);
		cout << "!Mapear " << p->second << endl;
		n.second = baseDatos[i].mapear(p->second);
		n.third = p->third;
		cout << "!EJE " << n.first << ' ' << n.second << ' ' << n.third << endl;

                if (aparEje.find(n) == aparEje.end()) {
                    aparEje[n] = 1;
		}
                else {
		    if (! already_in_instance[n])
		      aparEje[n]++;
                }
        	already_in_instance[n] = true;
            }
        }
                
        for (map<CANDIDATE, double>::iterator pp = aparEje.begin(); pp != aparEje.end(); pp++) {
            (*pp).second /= (baseDatos.size() * 1.);
             cout << "AP1: " << pp->first.first << ' ' << pp->first.second << '=' << pp->second << endl;
        }
        
        params.aparEje = &aparEje;
    }
//-------------------------------------------------------------------------

    cout << "El algoritmo MACS se esta ejecutando... " << endl;    
    MACS colonia(baseDatos, params);
    string str_rs(PARA.GLOB_rutaSalida);
    soluciones = colonia.ejecuta(str_rs);      

//  impresion en fichero HTML de informacion sobre la ejecucion (nº evaluaciones, tiempos ...)
    fichero = str_rs + ".htm";
    colonia.printInfo(fichero);
    
//     -----------------------------------
//     GENERACIoN DE FICHEROS DE RESULTADOS

//     cout << soluciones.getNumElementos() << " elementos" << endl;

//     impresion en fichero de los valores de los objetivos de todas las soluciones del Pareto tras terminar la ejecucion del algoritmo            
    string rutaFinal (str_rs + "(soloObj).txt");
    soluciones.writeObjsPareto( rutaFinal.c_str() );

//     impresion en fichero de las soluciones del Pareto (configuraciones de estaciones)
    rutaFinal = str_rs + ".txt";
    soluciones.writePareto( rutaFinal.c_str() );   
//     -----------------------------------                     
    
//     -----------------------------------
/*    DESTRUCCIoN DE OBJETOS Y LIBERAR MEMORIA
    
    eliminamos los objetos manualmente antes de que se destruya
    automaticamente el archivo de no dominados NDominatedSet*/
           
//     -----------------------------------
    
    cout << "Ejecucion finalizada correctamente" << endl;

    return 0;        
}

