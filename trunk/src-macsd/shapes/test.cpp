#include "global.h"
#include "utils.h"
#include "shapes.h"
#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
//     vector<string> figuras(4);
//     figuras[0] = "circle";
//     figuras[1] = "square";
//     figuras[2] = "rectangle";
//     figuras[3] = "triangle";
//     shapes uno(4, figuras), dos(4, figuras);
//     
//     // Genero la primer figura
//     uno.agregarNodo(1, "object");
//     uno.agregarNodo(2, "triangle");
//     uno.agregarNodo(3, "object");
//     uno.agregarNodo(4, "circle");
//     uno.agregarNodo(5, "object");
//     uno.agregarEje(1,2,2);
//     uno.agregarEje(1,3,1);
//     uno.agregarEje(3,4,2);
//     uno.agregarEje(3,5,1);
//             
//     // Genero la segunda figura
//     dos.agregarNodo(1, "object");
//     dos.agregarNodo(2, "object");
//     dos.agregarNodo(3, "object");    
//     dos.agregarNodo(4, "circle");
//     dos.agregarNodo(5, "triangle");
//     dos.agregarEje(1,2,1);
//     dos.agregarEje(2,3,1);
//     dos.agregarEje(1,5,2);
//     dos.agregarEje(2,4,2);
//     
//     if (uno == dos)
//         cout << "Iguales" << endl;
//     else
//         cout << "Distintos" << endl;
// }

//     srand(1);
    map<unsigned int, vector<unsigned int> > v;
        vector<unsigned int> r(3);
        r[0] = 1;
        r[1] = 3;
        r[2] = 5;
        v[0] = r;
        vector<unsigned int> t(3);
        t[0] = 1;
        t[1] = 3;
        t[2] = 5;
        v[1] = t;
        vector<unsigned int> n(3);
        n[0] = 1;
        n[1] = 3;
        n[2] = 5;
        v[2] = n;
//     vector<unsigned int> r(2);
//     r[0] = 1;
//     r[1] = 3;
//     v[0] = r;
//     vector<unsigned int> t(2);
//     t[0] = 1;
//     t[1] = 3;
//     v[1] = t;
//     vector<unsigned int> y(2);
//     y[0] = 2;
//     y[1] = 4;
//     v[2] = y;
    
    multimap<unsigned int, unsigned int> rel_est;
    rel_est.insert(pair<unsigned int,unsigned int>(0,2));
    rel_est.insert(pair<unsigned int,unsigned int>(1,2));
//     rel_est.insert(pair<unsigned int,unsigned int>(2,1));
    
    multimap<unsigned int, unsigned int> rel_ins;
    rel_ins.insert(pair<unsigned int,unsigned int>(1,3));
    rel_ins.insert(pair<unsigned int,unsigned int>(5,3));
//     rel_ins.insert(pair<unsigned int,unsigned int>(3,5));
//     rel_ins.insert(pair<unsigned int,unsigned int>(5,3));
    
    posibilidades<unsigned int> prob(v, rel_est, rel_ins);
    for (posibilidades<unsigned int>::iterator p = prob.begin(); p != prob.end(); ++p) {
            cout << "OPCION: ";
            map<unsigned int, unsigned int> x = *p;
            for (map<unsigned int, unsigned int>::iterator i = x.begin(); i != x.end(); i++) {
                cout << (*i).first + 1 << " -> " << (*i).second << endl;
            }
            cout << endl;
        }
    }
         
//     double at = clock()/CLOCKS_PER_SEC;
//     srand(1);
//       vector< vector<unsigned int> > v(3);
//         vector<unsigned int> r(3);
//         r[0] = 1;
//         r[1] = 3;
//         r[2] = 5;
//         v[0] = r;
//         vector<unsigned int> t(3);
//         t[0] = 1;
//         t[1] = 3;
//         t[2] = 5;
//         v[1] = t;
//         vector<unsigned int> n(3);
//         n[0] = 1;
//         n[1] = 3;
//         n[2] = 5;
//         v[2] = n;
//     for (unsigned int o = 0; o < 1000; o++){
//     //     vector< vector<unsigned int> > v(4);
//     //     for (unsigned int i = 0; i < v.size(); i++) {
//     //         int azar = intAzar(1,5);
//     //         vector<unsigned int> w(azar);
//     //         for (unsigned int j = 0; j < azar; j++) {
//     //             w[j] = j + 1;
//     //         }
//     //         v[i] = w;
//     //     }
//       
//         
//     //     vector<unsigned int> m(2);
//     //     m[0] = 1;
//     //     m[1] = 3;
//     //     v[3] = m;
//         
//         posibilidades<unsigned int> prob(v);
//         
//     //     vector<unsigned int> x = *(prob.begin());
//     //     if (prob.begin() != prob.end()) {
//     //         for (unsigned int i = 0; i < x.size(); i++) {
//     //             cout << x[i] << ' ';
//     //         }
//     //         cout << endl << "===========" << endl;
//     //     }
//     
//    
//         for (posibilidades<unsigned int>::iterator p = prob.begin(); p != prob.end(); ++p) {
//             cout << "OPCION: ";
//             vector<unsigned int> x = *p;
//             for (unsigned int i = 0; i < x.size(); i++) {
//                 cout << x[i] << ' ';
//             }
//             cout << endl;
//         }
//     }
//     double fin = clock()/CLOCKS_PER_SEC;
//     cout << (fin - at)/1000 << endl;
// }
