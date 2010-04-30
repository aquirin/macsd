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
#include "shapes.h"

using namespace std;
               
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
    map<pair<string,string>, string> rela;
    for (unsigned int i = 1; i < w.size(); ++i)
      rela.insert(pair<pair<string,string>, string>(pair<string,string>("object",w[i]), "shape"));
    rela.insert(pair<pair<string,string>, string>(pair<string,string>("object","object"), "on"));
    SOLUTION s(string("1"), w, e, rela);
            
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    map<unsigned int,unsigned int> pos;
    string cad;
    unsigned int codigo;
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
                
                        s.agregarEje(atoi(desde.c_str()), atoi(hasta.c_str()), cadena);
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
            }
        }
    }
    if (!s.empty())
        v.push_back(s);
    
    arch.close();
}
               
//----------------------------

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
    leeFicheroDatos (PARA.GLOB_rutaEntrada, baseDatos);
    leeFicheroDatos (argv[1], misub);
    
    //---------------------------------------------------------------------
    map <CANDIDATE,double> aparEje;
//     if (PARA.MOACO_multiheuristics == 1) {
//         // STATIC
//         for (unsigned int i = 0; i < baseDatos.size(); i++) {
//             set< CANDIDATE > tent = baseDatos[i].ejes();
//             set< CANDIDATE >::iterator p = tent.begin();
//             for (; p != tent.end(); p++) {
//                 if (aparEje.find(*p) == aparEje.end())
//                     aparEje[*p] = 1;
//                 else {
//                     aparEje[*p]++;
//                 }
//             }
//         }
//                 
//         for (map<CANDIDATE, double>::iterator pp = aparEje.begin(); pp != aparEje.end(); pp++) {
//             (*pp).second /= (baseDatos.size() * 1.);
// //             cout << "AP1: " << pp->first.first << ' ' << pp->first.second << '=' << pp->second << endl;
//         }
//         
//         params.aparEje = &aparEje;
//     }
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
        misub[i].imprime(arch1);
        Hormiga una(baseDatos, 2, 0, misub[i]);
        cout << i << ' ' << misub[i] << endl;
        cout << soluciones.addDominancia(una, false, kk) << endl;
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

