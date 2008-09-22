#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <fstream>
#include "../global.h"
#include "../macs.h"
#include "ontologia.h"

using namespace std;
               
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

//-------------------------------------------------------------------------

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
//                         cout << e << " -> " << n << endl;
                        ini += 8;
                  }
                  while (ini < cadena.size());
            }
        }
    }

    arch.close();
}

//-------------------------------------------------------------------------
               
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
    
    go info(nodos, ejes, desc);
    
    // Leo todas las anotaciones a la vez y genero un go
    // Leo el conjunto de anotaciones
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    unsigned int pri = 3673;
    int aux, aux1, aux2, codigo;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                // 
//                 200858_s_at|6412/protein biosynthesis/evidence IEA|3735/structural constituent of ribosome/evidence IEA|5840/ribosome/evidence IEA@5622/intracellular/evidence IEA|20
//                 info.clear();
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
                    while ((aux2 != string::npos) && (aux2 < aux1));
                    aux2 = ant;
                    if (aux2 < aux1)
                        aux = aux2;
                    if (aux1 != string::npos) {
                        codigo = atoi(cadena.substr(aux + 1, aux1 - aux - 1).c_str());
//                         cout << cadena.substr(aux + 1, aux1 - aux - 1) << ' ' << codigo << endl;
                        info.agregarEje(codigo,pri,0);
                        aux = aux1;
                        aux1 = cadena.find('@', aux + 1);
                        aux2 = cadena.find('|', aux + 1);
                        if (aux2 < aux1)
                            aux1 = aux2;
                        aux = aux1;
                    }
                }
                while (aux1 != string::npos);
//                 v.push_back(SOLUTION(name,info));
            }
        }
    }

    arch.close();
    arch.clear();
    
    // Leo el conjunto de anotaciones
    set<unsigned int> *nodos1 = new set<unsigned int>;
    set< CANDIDATE > *ejes1 = new set< CANDIDATE >;
    *nodos1 = info.nodos();
    *ejes1 = info.ejes();
    go data(nodos1, ejes1, desc);
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
                    while ((aux2 != string::npos) && (aux2 < aux1));
                    aux2 = ant;
                    if (aux2 < aux1)
                        aux = aux2;
                    if (aux1 != string::npos) {
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
                while (aux1 != string::npos);
                v.push_back(SOLUTION(name,data));
            }
        }
    }

    arch.close();
}
               
//----------------------------
// ./load cluster20.csv process.nodes process.edges function.nodes function.edges cellular.nodes cellular.edges emo-cc 1 2 10 0.1 0.2 0.5 0.4 0.5 0 experimentos/subdue.csv

int main(int argc, char *argv[]){	
    Parametros params;
    NDominatedSet soluciones;
    string fichero;
    
    vector< SOLUTION > baseDatos, misub;
        
	// Initialize the random generator    
	srand(PARA.GLOB_semilla);
	
    // almacenamiento de parametros        
    // -----------------------------
    PARA.ReadConfiguration("./config.txt");
    
     // leemos los datos del fichero de entrada
    leeFicheroDatos (PARA.GLOB_rutaEntrada, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, baseDatos);
    leeFicheroDatos (PARA.GO_x, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, misub);

    
    //---------------------------------------------------------------------
    map <CANDIDATE,double> aparEje;
    if (PARA.MOACO_multiheuristics == 1) {
        // STATIC
        for (unsigned int i = 0; i < baseDatos.size(); i++) {
            set< CANDIDATE > tent = baseDatos[i].second.ejes();
            set< CANDIDATE >::iterator p = tent.begin();
            for (; p != tent.end(); p++) {
                if (aparEje.find(*p) == aparEje.end())
                    aparEje[*p] = 1;
                else {
                    aparEje[*p]++;
                }
            }
        }
                
        for (map<CANDIDATE, double>::iterator pp = aparEje.begin(); pp != aparEje.end(); pp++) {
            (*pp).second /= (baseDatos.size() * 1.);
//             cout << "AP1: " << pp->first.first << ' ' << pp->first.second << '=' << pp->second << endl;
        }
        
        params.aparEje = &aparEje;
    }
    //-------------------------------------------------------------------------

    // algoritmo de hormigas multi-objetivo MACS
/*
    // Para obtener el tau0 de un ACO necesitamos conocer las medias de los costes de las soluciones greedies respecto a cada objetivo
    float coste1, coste2;
    
    obtenerTau0MACS(coste1, coste2);
    
    PARA.GLOB_tau0 = 1. / (coste1 * coste2);
    */
    cout << "El algoritmo MACS se esta ejecutando... " << endl;    
    MACS colonia(baseDatos, params);
    
    ofstream arch1("debug");
    int kk;
    for (int i = 0; i < misub.size(); i++) {
//         cout << i << ' ' << baseDatos[i].first << endl;
        misub[i].second.imprime(arch1);
        Hormiga una(1, baseDatos, 2, 0, misub[i]);
        soluciones.addDominancia(una, false, kk);
    }
    arch1.close();
    cout << "FIN" << endl;
    
    
//                     
//     cout << "Base de datos:" << endl;
//     for (int i = 0; i < baseDatos.size(); i++)
//         cout << baseDatos[i];
//     
//     soluciones = colonia.ejecuta(PARA.GLOB_rutaSalida);      

    // impresion en fichero HTML de informacion sobre la ejecucion (nº evaluaciones, tiempos ...)
    string str_rs(PARA.GLOB_rutaSalida);
    fichero = str_rs + ".htm";
    colonia.printInfo(fichero);
    
    //impresion de informacion sobre busqueda local
//     fichero = str_rs + "-BL.txt";                                
//     if (PARA.GLOB_BL == true)
//             BL::imprime(fichero);
        
    // -----------------------------------
    // GENERACIoN DE FICHEROS DE RESULTADOS

    cout << soluciones.getNumElementos() << " elementos" << endl;

    // impresion en fichero de los valores de los objetivos de todas las soluciones del Pareto tras terminar la ejecucion del algoritmo            
    string rutaFinal = str_rs + "(soloObj).txt";
    soluciones.writeObjsPareto( rutaFinal.c_str() );

    // impresion en fichero de las soluciones del Pareto (configuraciones de estaciones)
    rutaFinal = str_rs + ".txt";
    soluciones.writePareto( rutaFinal.c_str() );   
    // -----------------------------------                     
    
    // -----------------------------------
    // DESTRUCCIoN DE OBJETOS Y LIBERAR MEMORIA
    
    // eliminamos los objetos manualmente antes de que se destruya
    // automaticamente el archivo de no dominados NDominatedSet
    soluciones.vacia();
                 
    // -----------------------------------
    
    cout << "Ejecucion finalizada correctamente" << endl;

    return 0;        
}

