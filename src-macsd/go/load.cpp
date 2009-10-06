#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>

#include "macs.h"
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

void leerEjes(const string& s, set< tuplax3<unsigned int> >& ejes) {
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
                        ejes.insert(tuplax3<unsigned int>(n,e,0));
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
               
void leeFicheroDatos(const string& fichero, const string& bpn, const string& bpe, const string& fmn, const string& fme, const string& ccn, const string& cce, vector< pair<string,go> >& v) {
    ifstream arch;
    string cadena;
    map<unsigned int,string> *desc = new map<unsigned int,string>;
    set<unsigned int> *nodos = new set<unsigned int>;
    set< tuplax3<unsigned int> > *ejes = new set< tuplax3<unsigned int> >;
    
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
//                 v.push_back(pair<string,go>(name,info));
            }
        }
    }

    arch.close();
    arch.clear();
    
    // Leo el conjunto de anotaciones
    set<unsigned int> *nodos1 = new set<unsigned int>;
    set< tuplax3<unsigned int> > *ejes1 = new set< tuplax3<unsigned int> >;
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
                v.push_back(pair<string,go>(name,data));
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
    unsigned int contadorArgumentos;
    string fichero;
    
    vector< pair<string,go> > baseDatos, misub;
        
    // almacenamiento de parametros        
    // -----------------------------
    
    contadorArgumentos = 1;
    // parametros generales
    params.numObjs = 2;
    params.multiheuristics = 0;params.rutaEntrada,
    params.nEstOptimo = 0;
    params.BL = false;
    params.areaImplicita = false;
    params.preferencias = SIN_PREF;
    
    params.rutaEntrada = argv[contadorArgumentos++];          
    string bpn = argv[contadorArgumentos++];
    string bpe = argv[contadorArgumentos++];
    string fmn = argv[contadorArgumentos++];
    string fme = argv[contadorArgumentos++];
    string ccn = argv[contadorArgumentos++];
    string cce = argv[contadorArgumentos++];
    params.rutaSalida = argv[contadorArgumentos++];
    params.semilla = atoi(argv[contadorArgumentos++]);            
    params.maxTiempo = atoi(argv[contadorArgumentos++]);    
            
    // parametros para algoritmo MACS    
    params.numHormigas = atoi(argv[contadorArgumentos++]);
    params.beta = atof(argv[contadorArgumentos++]);
    params.ro = atof(argv[contadorArgumentos++]);
    params.q0 = atof(argv[contadorArgumentos++]);
    params.tau0 = atof(argv[contadorArgumentos++]);
    params.gamma = atof(argv[contadorArgumentos++]);
    params.multiheuristics = atoi(argv[contadorArgumentos++]);
    params.alfaGrasp = params.alfaObj1 = -1.;
    params.numColonias = 1;

    // leemos los datos del fichero de entrada
    leeFicheroDatos (params.rutaEntrada, bpn, bpe, fmn, fme, ccn, cce, baseDatos);
    string x = argv[contadorArgumentos++];
    leeFicheroDatos (x, bpn, bpe, fmn, fme, ccn, cce, misub);

    
    //---------------------------------------------------------------------
    map <tuplax3<unsigned int>,double> aparEje;
    if (params.multiheuristics == 1) {
        // STATIC
        for (unsigned int i = 0; i < baseDatos.size(); i++) {
            set< tuplax3<unsigned int> > tent = baseDatos[i].second.ejes();
            set< tuplax3<unsigned int> >::iterator p = tent.begin();
            for (; p != tent.end(); p++) {
                if (aparEje.find(*p) == aparEje.end())
                    aparEje[*p] = 1;
                else {
                    aparEje[*p]++;
                }
            }
        }
                
        for (map<tuplax3<unsigned int>, double>::iterator pp = aparEje.begin(); pp != aparEje.end(); pp++) {
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
    
    params.tau0 = 1. / (coste1 * coste2);
    params.tau0Prima = -1.;
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
//     soluciones = colonia.ejecuta(params.rutaSalida);      

    // impresion en fichero HTML de informacion sobre la ejecucion (nº evaluaciones, tiempos ...)
    fichero = params.rutaSalida + ".htm";
    colonia.printInfo(fichero);
    
    //impresion de informacion sobre busqueda local
//     fichero = params.rutaSalida + "-BL.txt";                                
//     if (params.BL == true)
//             BL::imprime(fichero);
        
    // -----------------------------------
    // GENERACIoN DE FICHEROS DE RESULTADOS

    cout << soluciones.getNumElementos() << " elementos" << endl;

    // impresion en fichero de los valores de los objetivos de todas las soluciones del Pareto tras terminar la ejecucion del algoritmo            
    string rutaFinal (params.rutaSalida + "(soloObj).txt");
    soluciones.writeObjsPareto( rutaFinal.c_str() );

    // impresion en fichero de las soluciones del Pareto (configuraciones de estaciones)
    rutaFinal = params.rutaSalida + ".txt";
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

