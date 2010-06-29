#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>
#include "global.h"
#include "macs.h"
#include "ontologia.h"

using namespace std;
               
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
               
//----------------------------
// ./load cluster20.csv process.nodes process.edges function.nodes function.edges cellular.nodes cellular.edges emo-cc 1 2 10 0.1 0.2 0.5 0.4 0.5 0 experimentos/subdue.csv

int main(int argc, char *argv[]){	
    Parametros params;
    NDominatedSet soluciones;
    unsigned int contadorArgumentos;
    string fichero;
    
    vector< SOLUTION > baseDatos, misub;
        
   // Initialize the random generator    
    srand(PARA.GLOB_semilla);
	
    // almacenamiento de parametros        
    // -----------------------------
    PARA.ReadConfiguration("./config.txt");
      
    // leemos los datos del fichero de entrada
    leeFicheroDatos (PARA.GLOB_rutaEntrada, PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, baseDatos);
    leeFicheroDatos (argv[1], PARA.GO_bpn, PARA.GO_bpe, PARA.GO_fmn, PARA.GO_fme, PARA.GO_ccn, PARA.GO_cce, misub);

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
        misub[i].imprime(arch1);
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
    string str_rs(argv[1]);
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

