#include <sys/timeb.h>
#include <time.h>
#include <string>
#include <vector>
#include <iostream>

#include "macs.h"
#include "shapes.h"

using namespace std;
               
// void obtenerTau0MACS(float &coste1, float &coste2) {
//     float sumaCostesGreedies, medias[2];
//     
//     // creamos una secuencia de tareas con 2 objetivos sobre la informacion creada
//     SecTareas tareas(precedencias, infoTareas, areaPromedia, tCiclo, 2);
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
               
void leeFicheroDatos(const string& fichero, vector<shapes>& v, const unsigned int n, const unsigned int e) {
    ifstream arch;
    string cadena;
    map<string,int> nodos, ejes;
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
    //                     v 1 object
                        aux = cadena.rfind(' ', cadena.size());
                        cadena = cadena.substr(aux + 1, cadena.size() - aux);
//                         cout << "Nodo:" << cadena << ' ';
                        if (nodos.find(cadena) == nodos.end()) {
                            nodos[cadena] = nodos.size();
                        }
//                         cout << nodos[cadena] << endl;
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
//                         cout << "Eje:!" << desde << '!' << hasta << '!' << cadena << endl;
                        if (ejes.find(cadena) == ejes.end()) {
                            ejes[cadena] = ejes.size();
                        }
                        break;
                    case '%':
                    default:
                        break;
                }
            }
        }
    }

    arch.close();
    arch.clear();
    
//     for (map<string,int>::iterator x = ejes.begin(); x != ejes.end(); x++)
//         cout << (*x).first << '=';
//     cout << endl;
    
    shapes s(n, e);
    
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    bool first = true;
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                switch (cadena[0]) {
                    case 'v':
//                     v 1 object
//                         aux = cadena.rfind(' ', cadena.size());
//                         cadena = cadena.substr(aux + 1, cadena.size() - aux);
//                         temp_nodos.push_back(cadena);
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
//                         cout << atoi(desde.c_str()) << ' ' << atoi(hasta.c_str()) << ' ' << ejes[cadena] << endl;
                        if (first) {
                            s.agregarNodo(atoi(desde.c_str()));
                            first = false;
                        }
                        s.agregarEje(atoi(desde.c_str()), atoi(hasta.c_str()), ejes[cadena]);
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {
//                 temp_nodos.clear();
                first = true;
                v.push_back(s);
                s.clear();
            }
        }
    }

    arch.close();
}
               
// funcion MAIN del proyecto
//-------------------------------------------------
// ./exe rocio1 salida 1 10 10 0.1 0.2 0.5 0.4 0.5 1
int main(int argc, char *argv[]){	
    Parametros params;
    NDominatedSet soluciones;
    unsigned int contadorArgumentos;
    string fichero;
    vector<shapes> baseDatos;
        
    // almacenamiento de parametros        
    // -----------------------------
    
    contadorArgumentos = 1;
    // parametros generales
    params.numObjs = 2;
    params.multiheuristics = 0;
    params.nEstOptimo = 0;
    params.BL = false;
    params.areaImplicita = false;
    params.preferencias = SIN_PREF;
    
    params.rutaEntrada = argv[contadorArgumentos++];          
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
    leeFicheroDatos (params.rutaEntrada, baseDatos, 5, 3);
    
//     ofstream arch1("debug");
//     for (int i = 0; i < baseDatos.size(); i++) {
//         cout << i << endl;
//         baseDatos[i].imprime(arch1);
//     }
//     arch1.close();
    
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
                    
    cout << "Base de datos:" << endl;
    for (int i = 0; i < baseDatos.size(); i++)
        cout << baseDatos[i];
    
    soluciones = colonia.ejecuta(params.rutaSalida);      

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

