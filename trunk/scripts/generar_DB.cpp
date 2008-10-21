#include <iostream>
#include "shapes.h"
#include "global.h"
#include "hormigas.h"
#include "NDominatedSet.h"
#include <stack>
#define MAX 15

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
    
    stack<shapes> pila;
//     stack<Hormiga> todas;
    stack< pair<int,bool> > don;
    shapes s(8,w);
    s.agregarNodo(1, "object");
    pila.push(s);
    pair<int,bool> q(1, true);
    don.push(q);
    int p = 0;
    vector<shapes> done;
    while (pila.size() > 0) {
        shapes n = pila.top();
//         cout << n << endl;
        pila.pop();
        pair<int,bool> q = don.top();
        don.pop();
        if (find(done, n) == false) {
//             Hormiga h(base, 2, 0, n);
//             h.imprime(file);
//             todas.push_back(h);
            done.push_back(n);
            string xx = n.graph_g();
            
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
//                 Hormiga hh(base, 2, 0, nn);
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
//                         Hormiga h(base, 2, 0, nn);
                        pila.push(nn);
                        pair<int,bool> qq(eje, false);
                        don.push(qq);                
                    }
                }
            }
        }
    }
    
//     for (vector<shapes>::iterator p = done.begin(); p != done.end(); p++) {
//         (*p).graph_g();
//     }
    
//     file.close();
}
