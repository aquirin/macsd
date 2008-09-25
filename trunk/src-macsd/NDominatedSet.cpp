#include "NDominatedSet.h"

//-------------------------------------------------------
bool NDominatedSet::addDominancia(Hormiga& solucion, bool preferencias, int &numDominanciasPorPreferencias){
	int dominancia;
        bool res = true;
        bool igual = false;
        solucion.calculaCostes();
        
        vector<Hormiga>::iterator x = Pareto.begin();
	while ((x != Pareto.end()) && res && !igual) {
            // Verificar que no este ya...
#if VERSION == V_SHAPE
            cout << "PPPPPPPP1" << solucion.subEst();
            cout << "PPPPPPPP2" << (*x).subEst();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
//             cout << "PPPPPPPP1" << solucion.subEst().second;
//             cout << "PPPPPPPP2" << (*x).subEst().second;
#endif

            if (*x != solucion)
                if (solucion.dominancia(*x, preferencias, numDominanciasPorPreferencias) == -1)
                    res = false;
                else
                    x++;
             else
                 igual = true;
	}

        if (res && !igual) {
	   // insertarlo
	   Pareto.push_back(solucion);
#if VERSION == V_SHAPE
           cout << "Add: " << solucion.subEst() << endl;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
           cout << "Add: " << solucion.subEst().second << endl;
#endif

           vector<Hormiga>::iterator x = Pareto.begin();
	   while (x != Pareto.end()) {
            // Verificar que no este ya...
#if VERSION == V_SHAPE
            cout << "PPPPPPPP3" << solucion.subEst();
            cout << "PPPPPPPP4" << (*x).subEst();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            cout << "PPPPPPPP1" << solucion.subEst().second;
            cout << "PPPPPPPP2" << (*x).subEst().second;
#endif

               if (solucion.dominancia(*x, preferencias, numDominanciasPorPreferencias) == 1) {
//                     cout << Pareto.size();
                    Pareto.erase(x);
                    cout << "Borrado 2" << Pareto.size() << endl;
                }
                else
                    x++;
           }
        }
        
	return res;
}

//-------------------------------------------------------
bool NDominatedSet::esDominada(Hormiga& solucion, bool preferencias, int &numDominanciasPorPreferencias){

	//Comprobar si hay alguna tarea que lo domina
        vector<Hormiga>::iterator x = Pareto.begin();
        bool fin = false;
	while ((x != Pareto.end()) && !fin) {
            if (solucion.dominancia(*x, preferencias, numDominanciasPorPreferencias) == -1)
			fin = true;
            x++;
	}

	return fin;
}

//-------------------------------------------------------
bool NDominatedSet::add(Hormiga& solucion, bool preferencias, int &numDominanciasPorPreferencias){
	int dominancia;
        bool res = true;
     
        vector<Hormiga>::iterator x = Pareto.begin();
	while ((x != Pareto.end()) && res) {
            // Verificar que no este ya...
#if VERSION == V_SHAPE
            cout << "PPPPPPPP1" << solucion.subEst();
            cout << "PPPPPPPP2" << (*x).subEst();
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            cout << "PPPPPPPP1" << solucion.subEst().second;
            cout << "PPPPPPPP2" << (*x).subEst().second;
#endif

            if (*x != solucion)
                if (dominancia = solucion.dominancia(*x, preferencias, numDominanciasPorPreferencias) == 1) {
//                     cout << Pareto.size();
                    Pareto.erase(x);
//                     cout << "Borrado 2" << Pareto.size() << endl;
                }
                else if (dominancia == -1) {
    // 			// si existia en el pareto una solucion igual pero de distinta colonia guardamos un apuntador de la colonia en la misma solucion 
    // 			if (solucion==(*x)){
    // 				if (unaSec->perteneceAColonia(solucion.getMainColonia())==false )
    // 					unaSec->addColonia(solucion.getMainColonia());				
    // 			}
                    
                            res = false;
                }
                else
                    x++;
             else
                 res = false;
	}

        if (res) {
	   // insertarlo
	   Pareto.push_back(solucion);
#if VERSION == V_SHAPE
           cout << "Add: " << solucion.subEst() << endl;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
           cout << "Add: " << solucion.subEst().second << endl;
#endif

        }
	return res;
}

//-------------------------------------------------------
void NDominatedSet::writePareto(ofstream &salida){
	vector<Hormiga>::iterator x = Pareto.begin();
        int i = 1;
	while (x != Pareto.end()) {
            salida << "** Solucion no dominada " << i << "** " << endl;
            (*x).imprime(salida);
            vector<unsigned int> s = x->soporte();
            for(unsigned int j=0; j<s.size(); j++) {
                salida << s[j] + 1 << " " ;
            }
            salida << endl;
            for(unsigned int j=0; j<x->getNumObjetivos(); j++) {
                salida << x->getCoste(j) << "\t" ;
            }
            salida << endl;
            x++;
            i++;
	}
        
    salida << endl << "El frente de Pareto tiene " << (i-1) << " soluciones en total" << endl;
}

//-------------------------------------------------------
void NDominatedSet::writePareto(const char *ruta){

	ofstream fichero(ruta);

	if (!fichero)
		throw Error("No se pudo abrir el fichero para escritura", 5);
	

	this->writePareto(fichero);        
	fichero.close();
}

//-------------------------------------------------------
void NDominatedSet::writeObjsPareto(ofstream &salida){
        vector<Hormiga>::iterator x = Pareto.begin();
	while (x != Pareto.end()) {
            for(unsigned int j=0; j<x->getNumObjetivos(); j++) {
                salida << x->getCoste(j) << "\t" ;
            }
            salida << endl;
            x++;
	}
}


//-------------------------------------------------------
void NDominatedSet::writeObjsPareto(const char *ruta){

	ofstream fichero(ruta);

	if (!fichero)
		throw Error("No se pudo abrir el fichero para escritura", 5);
	

	this->writeObjsPareto(fichero);        
	fichero.close();
}
