#include <iostream>
#include "shapes.h"
#include "global.h"
#include "hormigas.h"
#include <stack>
#include <sstream>
#define MAX 15
#define CANT 500

using namespace std;

bool find(const vector<shapes>& aca, const shapes& que) {
    bool found = false;
    for (vector<shapes>::const_iterator p = aca.begin(); (p != aca.end()) and !found; p++){
        found = ((*p) == que);
    }
    return found;
}

int main (int argc, char* argv[]) {
    vector<string> w(5);
    w[0] = "square";
    w[1] = "triangle";
    w[2] = "ellipse";
    w[3] = "rectangle";
    w[4] = "circle";
        
    vector<shapes> done;
    ofstream file("NewDB.g");
    for (unsigned int i = 0; i < CANT; i++) {
        shapes s(MAX, w);
        s.agregarNodo(1, "object");

        int x = intAzar(1, MAX - 1);
        bool last = false;
        string salida = "XP\n";
        for (unsigned int j = 0; j < x; j++) {
            unsigned int fig = 1;
            if (last) {
                s.agregarNodo(j + 2, " object");
                stringstream ssCadena;
                ssCadena << j + 2;
                string sub;
                ssCadena >> sub;
                salida += "v " + sub + " object\n"; 
            }
            else {
                int y = intAzar(1, 2);
                if (y == 1) {
                    int y = intAzar(1, 5);
                    s.agregarNodo(j + 2, w[y-1]);
                    stringstream ssCadena;
                    ssCadena << j + 2;
                    string sub;
                    ssCadena >> sub;
                    salida += "v " + sub + " " + w[y-1] + "\n"; 
                    fig = 2;
                }
                else {
                    s.agregarNodo(j + 2, " object");
                    stringstream ssCadena;
                    ssCadena << j + 2;
                    string sub;
                    ssCadena >> sub;
                    salida += "v " + sub + " object\n"; 
                }
            }
            s.agregarEje(j + 1, j + 2, fig);
            stringstream ssCadena;
            ssCadena << j + 1;
            string sub;
            ssCadena >> sub;
            stringstream ssCadena1;
            ssCadena1 << j + 2;
            string sub1;
            ssCadena1 >> sub1;
            string que = "on";
            if (fig == 2) que = "shape";
            salida += "e " + sub + " " + sub1+ " " + que + "\n"; 
        }
        
        salida += "\n";
        
        if (find(done, s) == false) {
            done.push_back(s);
            file << salida << endl;
        }
        else {
            i--;
        }
    }
    
    file.close();
}
