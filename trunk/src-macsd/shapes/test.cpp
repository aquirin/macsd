#include "global.h"
#include "utils.h"
#include "shapes.h"
#include <iostream>
#include <vector>
#include <map>

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

int main() {
    vector<shapes> baseDatos;
    leeFicheroDatos("prueba1.g", baseDatos);

    shapes uno = baseDatos[0];
    shapes dos = baseDatos[1];
    
    posibilidades prob(&uno, &dos);
    for (posibilidades::iterator p = prob.begin(); p != prob.end(); ++p) {
            cout << "OPCION: ";
            map<unsigned int, unsigned int> x = *p;
            for (map<unsigned int, unsigned int>::iterator i = x.begin(); i != x.end(); i++) {
                cout << (*i).first << " -> " << (*i).second << endl;
            }
            cout << endl;
    }
}
