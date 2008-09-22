#include "posibilidades.h"
#include "utils.h"
#include <iostream>
#include <vector>

using namespace std;

int main() {
    srand(1);
    vector< vector<unsigned int> > v(3);
    vector<unsigned int> r(2);
    r[0] = 1;
    r[1] = 3;
    v[0] = r;
    vector<unsigned int> t(2);
    t[0] = 1;
    t[1] = 3;
    v[1] = t;
    vector<unsigned int> y(2);
    y[0] = 2;
    y[1] = 4;
    v[2] = y;
    posibilidades<unsigned int> prob(v);
    for (posibilidades<unsigned int>::iterator p = prob.begin(); p != prob.end(); ++p) {
            cout << "OPCION: ";
            vector<unsigned int> x = *p;
            for (unsigned int i = 0; i < x.size(); i++) {
                cout << x[i] << ' ';
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
