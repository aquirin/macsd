#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>

#include "macs.h"
#if VERSION == V_SHAPE
#include "shapes/shapes.h"
#elif (VERSION == V_GO)
#include "go/ontologia.h"
#elif (VERSION == V_SCIENCEMAP)
#include "vmap/vmap.h"
#elif (VERSION == V_WWW)
#include "www/www.h"
#endif


using namespace std;

// void obtenerTau0MACS(float &coste1, float &coste2) {
//     float sumaCostesGreedies, medias[2];
//     
//     // creamos una secuencia de tareas con 2 objetivos sobre la informacion creada
//     SecTareas tareas(precedencias, infoTareas, areaPromedia, PARA.GLOB_tCiclo, 2);
//     
//     sumaCostesGreedies = 1.;
// 	
//     for (int i = 0; i < 2; i++)            
//         medias[i] = 0.;
// 
//     for (int i = 0; i < 2; i++){
//         tareas.circuitoGreedy(i);                
//         cout << "El algoritmo Greedy (info. heur. " << i+1 << ") dio una solucion con costes " 
//                 << tareas.getCoste(0) << " y " << tareas.getCoste(1) << endl;
// 
//         for (int j = 0; j < 2; j++)
//         	medias[j] += tareas.getCoste(j); 
//     }
// 
//     coste1 = (medias[0] / 2.);
//     coste2 = (medias[1] / 2.);
//                         
// }

//-------------------------------------------------------------------------

#if VERSION == V_GO
void leerNodos(const string& s, set<unsigned int>& nodos, map<unsigned int,string>& desc) {
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
                  unsigned int n = atoi(cadena.substr(0, 7).c_str());
                  if (desc.find(n) == desc.end()) {
                      desc[n] = cadena.substr(8, cadena.size() - 8);
                      nodos.insert(n);
                  }
            }
        }
    }

    arch.close();
}
#endif

//-------------------------------------------------------------------------

#if VERSION == V_GO
void leerEjes(const string& s, set< CANDIDATE >& ejes) {
//     0009701:0009716;0009717;0046289;0009689
// Los ejes son (hijo,padre)
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
                  unsigned int n = atoi(cadena.substr(0, 7).c_str());
                  unsigned int ini = 9;
                  do {
//                       cout << cadena << endl;
                        unsigned int e = atoi(cadena.substr(ini,ini+7).c_str());
                        ejes.insert(CANDIDATE(n,e,0));
//                          cout << e << " -> " << n << endl;
                        ini += 8;
                  }
                  while (ini < cadena.size());
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
    vector<string> w(5);
    w[0] = "square";
    w[1] = "triangle";
    w[2] = "ellipse";
    w[3] = "rectangle";
    w[4] = "circle";
    SOLUTION s(8,w);
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
//     bool first = true;
    string cad;
    unsigned int codigo;
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
//                         cout << desde << ' ' << cad << endl;
                        s.agregarNodo(atoi(desde.c_str()), cad);
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
                        
//                         if (first) {
//                             s.agregarNodo(atoi(desde.c_str()));
//                             first = false;
//                         }
                        codigo = 1;
                        if (cadena == "shape")
                            codigo++;
//                         cout << atoi(desde.c_str()) << ' ' << atoi(hasta.c_str()) << ' ' << codigo << endl;
                        s.agregarEje(atoi(desde.c_str()), atoi(hasta.c_str()), codigo);
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {
//                 temp_nodos.clear();
//                 first = true;
                v.push_back(s);
                cout << s << endl;
                s.clear();
            }
        }
    }
    if (!s.empty())
        v.push_back(s);
    
    arch.close();
}
#endif

#if VERSION == V_GO
void leeFicheroDatos(const string& fichero, const string& bpn, const string& bpe, const string& fmn, const string& fme, const string& ccn, const string& cce, vector< SOLUTION >& v) {
    ifstream arch;
    string cadena;
    map<unsigned int,string> *desc = new map<unsigned int,string>;
    set<unsigned int> *nodos = new set<unsigned int>;
    set< CANDIDATE > *ejes = new set< CANDIDATE >;
    
    // Leo la base de datos de GO
    leerNodos(bpn, *nodos, *desc);
    leerNodos(fmn, *nodos, *desc);
    leerNodos(ccn, *nodos, *desc);
    leerEjes(bpe, *ejes);
    leerEjes(fme, *ejes);
    leerEjes(cce, *ejes);
    
//     go info("0", nodos, ejes, desc);
//     
//     // Leo todas las anotaciones a la vez y genero un go
//     // Leo el conjunto de anotaciones
//     arch.open(fichero.c_str());
// 
//     if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
//         exit(1);
//     }
//     
    unsigned int pri = 0;
    int aux, aux1, aux2, codigo;
    unsigned int menos_uno = 0 - 1;
//     while (!arch.eof()) {
//         getline(arch, cadena);
//         if (!arch.eof()) {
//             if (!cadena.empty()) {
//                 // 
// //                 200858_s_at|6412/protein biosynthesis/evidence IEA|3735/structural constituent of ribosome/evidence IEA|5840/ribosome/evidence IEA@5622/intracellular/evidence IEA|20
// //                 info.clear();
//                 // Gene product
//                 aux = cadena.find('|', 0);
//                 string name = cadena.substr(0, aux);
//                 // Data
//                 do {
//                     aux1 = cadena.find('/', aux + 1);
//                     aux2 = aux;
//                     unsigned int ant;
//                     do {
//                         ant = aux2;
//                         aux2 = cadena.find('|', ant + 1);
//                     }
//                     while ((aux2 != menos_uno) && (aux2 < aux1));
//                     aux2 = ant;
//                     if (aux2 < aux1)
//                         aux = aux2;
//                     if (aux1 != menos_uno) {
//                         codigo = atoi(cadena.substr(aux + 1, aux1 - aux - 1).c_str());
// //                         cout << cadena.substr(aux + 1, aux1 - aux - 1) << ' ' << codigo << endl;
//                         info.agregarEje(codigo,pri,0);
//                         aux = aux1;
//                         aux1 = cadena.find('@', aux + 1);
//                         aux2 = cadena.find('|', aux + 1);
//                         if (aux2 < aux1)
//                             aux1 = aux2;
//                         aux = aux1;
//                     }
//                 }
//                 while (aux1 != menos_uno);
// //                 v.push_back(SOLUTION(name,info));
//             }
//         }
//     }
// 
//     arch.close();
//     arch.clear();
    
    // Leo el conjunto de anotaciones
//     set<unsigned int> *nodos1 = new set<unsigned int>;
//     set< CANDIDATE > *ejes1 = new set< CANDIDATE >;
//     *nodos1 = info.nodos();
//     *ejes1 = info.ejes();
    SOLUTION data("0", nodos, ejes, desc);
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                // 
//                 200858_s_at|6412/protein biosynthesis/evidence IEA|3735/structural constituent of ribosome/evidence IEA|5840/ribosome/evidence IEA@5622/intracellular/evidence IEA|20
                data.clear();
                // Gene product
                aux = cadena.find('|', 0);
                string name = cadena.substr(0, aux);
                // Data
                do {
                    aux1 = cadena.find('/', aux + 1);
                    aux2 = aux;
                    unsigned int ant;
                    do {
                        ant = aux2;
                        aux2 = cadena.find('|', ant + 1);
                    }
                    while ((aux2 != menos_uno) && (aux2 < aux1));
                    aux2 = ant;
                    if (aux2 < aux1)
                        aux = aux2;
                    if (aux1 != menos_uno) {
                        codigo = atoi(cadena.substr(aux + 1, aux1 - aux - 1).c_str());
//                        cout << cadena.substr(aux + 1, aux1 - aux - 1) << ' ' << codigo << endl;
                        data.agregarEje(codigo,pri,0);
                        aux = aux1;
                        aux1 = cadena.find('@', aux + 1);
                        aux2 = cadena.find('|', aux + 1);
                        if (aux2 < aux1)
                            aux1 = aux2;
                        aux = aux1;
                    }
                }
                while (aux1 != menos_uno);
                v.push_back(data);
// 		cout << "Item: " << data << endl;
		data.clear();
            }
        }
    }

    arch.close();
}
#endif

#if VERSION == V_SCIENCEMAP           
void leeFicheroDatos(const string& fichero, vector<SOLUTION>& v, map<string,unsigned int>* nodos, set< pair< pair<string,string>, unsigned int> >* ejes) {
    ifstream arch;
    string cadena;
    
    unsigned int i = 1;
    
    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }

    map<unsigned int, string> quenodo;
    string cad;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
			//  v 1 "\nMultidisciplinary\n"
                        aux = cadena.rfind(' ', cadena.size());
                        cad = cadena.substr(aux + 1, cadena.size() - aux);
                        aux1 = cadena.find(' ', 0);
                        desde = cadena.substr(aux1 + 1, aux - aux1 - 1);
			if (nodos->find(cad) == nodos->end())
			  (*nodos)[cad] = i++;
			quenodo[atoi(desde.c_str())] = cad;
                        break;
		    case 'e':
		    case 'd':
		    case 'u':
			//  u 11 3 ""
                        aux = cadena.find(' ', 0);
                        aux1 = cadena.find(' ', aux + 1);
                        desde = cadena.substr(aux + 1, aux1 - aux - 1);
                        aux = cadena.rfind(' ', cadena.size());
                        hasta = cadena.substr(aux1 + 1, aux - aux1 - 1);
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);
                        
			ejes->insert(pair< pair<string,string>, unsigned int>(pair<string,string>(quenodo[atoi(desde.c_str())],quenodo[atoi(hasta.c_str())]), 0));
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
    
    SOLUTION s(nodos,ejes,false);
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
// 	  	        v 1 "\nMultidisciplinary\n"
                        aux = cadena.rfind(' ', cadena.size());
                        cad = cadena.substr(aux + 1, cadena.size() - aux);
                        aux1 = cadena.find(' ', 0);
                        desde = cadena.substr(aux1 + 1, aux - aux1 - 1);
			
                        s.agregarNodo(atoi(desde.c_str()), cad);
                        break;
                    case 'e':
                    case 'd':
		    case 'u':
// 		        u 11 3 ""
                        aux = cadena.find(' ', 0);
                        aux1 = cadena.find(' ', aux + 1);
                        desde = cadena.substr(aux + 1, aux1 - aux - 1);
                        aux = cadena.rfind(' ', cadena.size());
                        hasta = cadena.substr(aux1 + 1, aux - aux1 - 1);
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);
                        
                        s.agregarEje(atoi(desde.c_str()), atoi(hasta.c_str()), 0);
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
//-------------------------------------------------
// SHAPE-version:
// ./exe house3.g shapes 1 2 10 0.1 0.2 0.5 0.4 0.5 0

// GO-version:
// ./exe cluster20.csv process.nodes process.edges function.nodes function.edges cellular.nodes cellular.edges emo-cc 1 2 10 0.1 0.2 0.5 0.4 0.5 0

// SCIENCEMAP-version:
// ./exe rocio1 salida 1 10 10 0.1 0.2 0.5 0.4 0.5 1 5 3

// 

int main(int argc, char *argv[]){	
    Parametros params;
    NDominatedSet soluciones;
    string fichero;
    vector<SOLUTION> baseDatos;
    int num_nodes, num_edges;
  
	/* TODO: HARMONIZE (OLD code) */
#if VERSION == V_SCIENCEMAP
	/*if(argc!=13)
    {
    	fprintf(stderr, "Usage: %s <input file> <output file> <random seed> <max time> <num ants> <beta> <ro> <q0> <tau0> <gamma> <num nodes> <num edges>\n",
		argv[0]);
	exit(-1);
    }*/
#endif

	// Initialize the random generator    
	srand(PARA.GLOB_semilla);
        
    // almacenamiento de parametros        
    // -----------------------------
    PARA.ReadConfiguration("./config.txt");
    
    // leemos los datos del fichero de entrada
#if VERSION == V_SHAPE
    leeFicheroDatos (PARA.GLOB_rutaEntrada, baseDatos);
#elif VERSION == V_GO
    leeFicheroDatos (PARA.GLOB_rutaEntrada, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, baseDatos);
#elif VERSION == V_SCIENCEMAP
    map<string,unsigned int> nodos;
    set< pair< pair<string,string>, unsigned int> > ejes;
    leeFicheroDatos (PARA.GLOB_rutaEntrada, baseDatos, &nodos, &ejes);
#endif

    
//---------------------------------------------------------------------
    map <CANDIDATE,double> aparEje;
    if (PARA.MOACO_multiheuristics == 1) {
        // STATIC
	map<CANDIDATE,bool> already_in_instance;
        for (unsigned int i = 0; i < baseDatos.size(); i++) {
#if VERSION == V_SHAPE
            vector< CANDIDATE > tent = baseDatos[i].posibilidades_totales();
	    for (vector< CANDIDATE >::iterator it = tent.begin(); it != tent.end(); ++it) {
// 	      it->first = 1;
	      already_in_instance[*it] = false;
	    }	      
            vector< CANDIDATE >::iterator p = tent.begin();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            set< CANDIDATE > tent = baseDatos[i].ejes();
	    for (set< CANDIDATE >::iterator it = tent.begin(); it != tent.end(); ++it) {
	      already_in_instance[*it] = false;
	    }
            set< CANDIDATE >::iterator p = tent.begin();
#endif
	    
	    
            for (; p != tent.end(); p++) {
#if VERSION == V_SHAPE
		// Must do only in shapes when using isomorphism
		p->first = 1;
#endif

                if (aparEje.find(*p) == aparEje.end()) {
                    aparEje[*p] = 1;
		}
                else {
		    if (! already_in_instance[*p])
		      aparEje[*p]++;
                }
        	already_in_instance[*p] = true;
            }
        }
                
        for (map<CANDIDATE, double>::iterator pp = aparEje.begin(); pp != aparEje.end(); pp++) {
            (*pp).second /= (baseDatos.size() * 1.);
             cout << "AP1: " << pp->first.first << ' ' << pp->first.second << '=' << pp->second << endl;
        }
        
        params.aparEje = &aparEje;
    }
//-------------------------------------------------------------------------

//     ofstream arch1("debug");
//     for (int i = 0; i < baseDatos.size(); i++) {
//         baseDatos[i].imprime(arch1);
//     }
//     arch1.close();
//     cout << "FIN" << endl;
    // algoritmo de hormigas multi-objetivo MACS
/*
    // Para obtener el tau0 de un ACO necesitamos conocer las medias de los costes de las soluciones greedies respecto a cada objetivo
    float coste1, coste2;
    
    obtenerTau0MACS(coste1, coste2);
    
    PARA.GLOB_tau0 = (coste1 * coste2);
    */
    cout << "El algoritmo MACS se esta ejecutando... " << endl;    
    MACS colonia(baseDatos, params);
                    
 /*   cout << "Base de datos:" << endl;
    for (int i = 0; i < baseDatos.size(); i++)
        cout << baseDatos[i];
 */   
    string str_rs(PARA.GLOB_rutaSalida);
    soluciones = colonia.ejecuta(str_rs);      

//     impresion en fichero HTML de informacion sobre la ejecucion (nº evaluaciones, tiempos ...)
    fichero = str_rs + ".htm";
    colonia.printInfo(fichero);
    
//     impresion de informacion sobre busqueda local
//     fichero = str_rs + "-BL.txt";                                
//     if (PARA.GLOB_BL == true)
//             BL::imprime(fichero);
        
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
#if VERSION == V_GO
    soluciones.vacia();		/* TODO: HARMONIZE (not in the other version) */
#endif
           
//     -----------------------------------
    
    cout << "Ejecucion finalizada correctamente" << endl;

    return 0;        
}

