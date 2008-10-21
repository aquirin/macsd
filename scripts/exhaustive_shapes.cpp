#include <iostream>
#include "shapes.h"
#include "global.h"
#include "hormigas.h"
#include "NDominatedSet.h"
#include <stack>
#define MAX 8

using namespace std;

bool find(const vector<shapes>& aca, const shapes& que) {
    bool found = false;
    for (vector<shapes>::const_iterator p = aca.begin(); (p != aca.end()) and !found; p++){
        found = ((*p) == que);
    }
    return found;
}

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
 
    arch.open(fichero.c_str());

    if (!arch.good()) { cout << "Problema con el fichero: " << fichero << endl;
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
                        codigo = 1;
                        if (cadena == "shape")
                            codigo++;
                        s.agregarEje(atoi(desde.c_str()), atoi(hasta.c_str()), codigo);
                        break;
                    case '%':
                    default:
                        break;
                }
            }
            else {
                v.push_back(s);
//                 cout << s << endl;
                s.clear();
            }
        }
    }
    if (!s.empty()) {
        v.push_back(s);
//         cout << s << endl;
    }
    
    arch.close();
}

int main (int argc, char* argv[]) {
    NDominatedSet pareto;
    vector<shapes> base;    
    // Leo la base de datos
    leeFicheroDatos ("DB.g", base);

    vector<string> w(5);
    w[0] = "square";
    w[1] = "triangle";
    w[2] = "ellipse";
    w[3] = "rectangle";
    w[4] = "circle";
    
    unsigned int tamMax = 4;
    stack<shapes> pila;
    stack< pair<int,bool> > don;
    shapes s(8,w);
    s.agregarNodo(1, "object");
    pila.push(s);
    pair<int,bool> q(1, true);
    don.push(q);
    int p = 0;
    vector<shapes> done;
    vector<Hormiga> todas;
    ofstream file("debug");
    while (pila.size() > 0) {
        shapes n = pila.top();
//         cout << n << endl;
        pila.pop();
        pair<int,bool> q = don.top();
        don.pop();
        if (find(done, n) == false) {
            Hormiga h(base, 2, 0, n);
            h.imprime(file);
            todas.push_back(h);
//             pareto.addDominancia(h, false, p);
            done.push_back(n);
            
            shapes nn = n;
            nn = n;
            unsigned int eje = q.first;
            nn.agregarNodo(++eje, "object");
            if (q.second) {
                nn.agregarEje(eje - 1, eje, 1);
            }
            else {
                nn.agregarEje(eje - 2, eje, 1);
            }
            
            if ((nn.cantNodos() <= MAX) and (find(done, nn) == false)) {
                Hormiga hh(base, 2, 0, nn);
//                 pareto.addDominancia(hh, false, p);
                pila.push(nn);
                pair<int,bool> qqq(eje, true);
                don.push(qqq);
            }
            
            if (q.second) {
                // Sin forma
                for (unsigned int j = 0; j < w.size(); j++) {
                    shapes nn = n;
                    // Sin forma
                    unsigned int eje = q.first;
                    nn.agregarNodo(++eje, w[j]);          
                    nn.agregarEje(eje - 1, eje, 2);
                    if ((nn.cantNodos() <= MAX) and (find(done, nn) == false)) {
                        Hormiga h(base, 2, 0, nn);
//                         pareto.addDominancia(h, false, p);
                        pila.push(nn);
                        pair<int,bool> qq(eje, false);
                        don.push(qq);                
                    }
                }
            }
        }
    }
    
    // Reviso dominancia
    for (unsigned int i = 0; i < todas.size(); i++) {
        bool dominada = false;
        for (unsigned int j = 0; (j < todas.size()) and !dominada; j++) {
            dominada = (todas[j].dominancia(todas[i], false, 0) == 1);
        }
        if (!dominada) {
            pareto.addDominancia(todas[i], false, p);
        }
    }
    
    string donde = "shapes_exhaustivo1";
    donde += "(soloObj).txt";
    pareto.writeObjsPareto(donde.c_str());
    donde = "shapes_exhaustivo1";
    donde += ".txt";
    pareto.writePareto(donde.c_str());   
    file.close();
}
