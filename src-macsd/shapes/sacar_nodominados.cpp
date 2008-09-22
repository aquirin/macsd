#include <iostream>
#include "shapes.h"
#include "hormigas.h"
#include "NDominatedSet.h"

using namespace std;

void leeFicheroDatos(const string& fichero, vector<shapes>& v) {
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
    shapes s(8,w);
    /*
    unsigned int aux;
    unsigned int aux1;
    string desde, hasta;
    set<string> w;
    arch.open("figuras");

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
        exit(1);
    }
    
    while (!arch.eof()) {
        getline(arch, cadena);
        if (!arch.eof()) {
            if (!cadena.empty()) {
                if (w.find(cadena) == w.end())
                    w.insert(cadena);
            }
        }
    }
    arch.close();
    arch.clear();
    
    vector<string> y(w.size());
    int i = 0;
    for (set<string>::iterator p = w.begin(); p != w.end(); p++) {
        y[i] = *p;
        i++;
    }
    shapes s(6,y);*/    
    
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
    if (!s.empty()) {
        v.push_back(s);
        cout << s << endl;
    }
    
    arch.close();
}

int main (int argc, char* argv[]) {
    NDominatedSet pareto;
    vector<shapes> base;    
    // Leo la base de datos
    leeFicheroDatos ("DB.g", base);

    // Leo el fichero de datos
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
    shapes s(8,w);
 
    arch.open(argv[1]);

    if (!arch.good()) { cout << "Problema con el fichero: " << argv[1] << endl;
        exit(1);
    }
    
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
//                 v.push_back(s);
                Hormiga h(base, 2, s);
                int p = 0;
                pareto.addDominancia(h, false, p);
//                 cout << s << endl;
                s.clear();
            }
        }
    }

    arch.close();
    
    string donde = argv[1];
    donde.erase(donde.size() - 2, 2);
    donde += "(soloObj).txt";
    pareto.writeObjsPareto(donde.c_str());
    donde = argv[1];
    donde.erase(donde.size() - 2, 2);
    donde += ".txt";
    pareto.writePareto(donde.c_str());   
}
