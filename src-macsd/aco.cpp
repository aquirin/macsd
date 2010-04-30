#include "aco.h"
#include <time.h>
#include <sstream>

//---------------------------------------------------------
ACO::ACO (vector< SOLUTION >& b, Parametros &params) : AlgoritmoMO (PARA.GLOB_BL, PARA.GLOB_numObjs, PARA.GLOB_preferencias), base(b) {      	
    	
    unsigned int i,j,k;
    
    // inicializamos a 0 las n probabilidades para movernos en cada nodo
    // un vector con n probabilidades, una por cada n-esima nodo
    vector< CANDIDATE > lista = b[0].posibilidades_totales();
    for (vector< CANDIDATE >::iterator p = lista.begin(); p != lista.end(); p++)
        this->probabilidades[*p] = 0.;

    cout << "CC " << this->probabilidades.size() << endl;
 
    // creamos las hormigas y les asignamos su colonia
    for(i = 0; i < PARA.MOACO_numHormigas; i++){
        Hormiga *sec = new Hormiga (base, PARA.GLOB_numObjs, params.aparEje);
        this->hormigas.push_back(sec);
    }
    _aparEje = params.aparEje;
}

//---------------------------------------------------------
ACO::~ACO () {
	for(unsigned int i = 0; i < PARA.MOACO_numHormigas; i++) {
		delete (this->hormigas[i]);
		this->hormigas[i] = NULL;
	}
	
        delete [] (this->umbrales);
	this->umbrales = NULL;
        
	delete [] (this->heuristicas);
	this->heuristicas = NULL;        
}

//---------------------------------------------------------
void ACO::setUmbralesYHeuristicas (unsigned int multiheuristics) {
	this->umbrales = new float[PARA.MOACO_numHormigas];
	this->heuristicas =  new unsigned int[PARA.MOACO_numHormigas];
		
	switch (multiheuristics) {
           case 0: //No hay multiheuristicas...
               break;
	   case 1: 		
		//STATIC
		cout << "Se utilizan distintos umbrales para cada hormiga pero heuristica fija (tipica/inicial).\n";	
		break;
           case 2:
                //DYNAMIC:
		cout << "Se utilizan distintos umbrales para cada hormiga pero heuristica dinamica.\n";	
		break;
	}
		
        for (unsigned int i = 0; i < PARA.MOACO_numHormigas; i++)
            this->umbrales[i] = (1 / PARA.MOACO_numHormigas) * (i+1) - 0.005;
        
        for (unsigned int i = 0; i<PARA.MOACO_numHormigas; i++)
            this->heuristicas[i] = multiheuristics;
}

//---------------------------------------------------------

NDominatedSet & ACO::ejecuta (string &filename) {
    // variables de control de tiempos y del algoritmo generico
    time_t inicio, fin;
    unsigned long tiempoIteracionAnterior, inicioBL;
    string nombreFichero;
    unsigned long tiempoTranscurrido, tiempoHastaUltimaIteracion;
     
    // variables dependientes de los grafos
    vector< CANDIDATE > candidatas;
    
    nombreFichero = filename;
    tiempoTranscurrido = tiempoHastaUltimaIteracion = 0.; 
    inicio = time( NULL );
    fin = time( NULL );
    this->inicioAlgoritmo = tiempoIteracionAnterior = clock()/CLOCKS_PER_SEC;

    // Iteration 1
    vector <SOLUTION> inicial;
    
    // creamos las hormigas y les asignamos su colonia
    inicial.push_back(this->hormigas[0]->subEst());
    inicial[0].inicial();
   
    for (unsigned int n = 1; n < PARA.MOACO_numHormigas * 10; n++) {
//         cout << "Inicial: " << n << endl;
        vector < CANDIDATE > dondeir;
        inicial.push_back(this->hormigas[0]->subEst());
	inicial[n].inicial();
              
        // Elijo cantidad de pasos
        unsigned int x;
        if (PARA.MOACO_ranking)
	    x = ranking(PARA.MOACO_stepSize, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
        else
            x = intAzar(1, PARA.MOACO_stepSize);
        for (unsigned int i = 0; i < x; i++) {
            // candidatas posibles a ser elegidas en este paso de la hormiga
            candidatas = inicial[n].ejesNoUtilizados();
                         
	    cout << inicial[n] << endl;
	    for (vector<CANDIDATE>::iterator it = candidatas.begin(); it != candidatas.end(); ++it)
	      cout << "C " << it->first << ' ' << it->second << ' ' << it->third << endl;
	    cout << endl;
	    
            // Elijo un candidato al azar
            unsigned int y = intAzar(0, candidatas.size() - 1);
            if (candidatas.size() > 0) {
	      inicial[n].agregarEje(candidatas[y].first, candidatas[y].second, candidatas[y].third);              
            }
            else {
                cout << "No hay candidatos!" << endl;
            }
        }
    }
    unsigned int cuantos = 0;
        
    Hormiga best(this->base, this->nObj, this->_aparEje, inicial[0]);
      
    for (unsigned int n = 0; n < PARA.MOACO_numHormigas * 10; n++) {
            // Pasamos cada SOLUCION a una hormiga
            Hormiga una(this->base, this->nObj, this->_aparEje, inicial[n]);
            	    
	    cout << "Antes Local Search: " << una.subEst();
// 	    una.calculaCostes();
// 
// 	    vector <float> cost1(2);
// 	    cost1[0] = una.getCoste(0);
// 	    cost1[1] = una.getCoste(1);
// 	    cout << "Costo: " << cost1[0] << ' ' << cost1[1] << endl;
// 
// // 	    if (cost1[0] > 0) {
// 	    // Aqui agrego el local search
// 	    candidatas = una.getCandidatos();
// 	    set<CANDIDATE> pasos = una.local_search();
// 	    // pasos son todos los ejes que tengo que agregar
// 	    while (pasos.size() > 0) {
// 		for (set<CANDIDATE>::iterator pp = pasos.begin(); pp != pasos.end(); pp++) {
// 		    bool found = false;
// 		    unsigned int q = 0;
// 	    // 	cout << "Las candidatas:" << endl;
// 		    while ((q < candidatas.size()) and !found) {
// 	    //		cout << "Cand: " << candidatas[q].first << "," << candidatas[q].second << "," << candidatas[q].third << endl;
// 			    found = ((candidatas[q] == *pp) or (candidatas[q] == CANDIDATE(q.second,q.first,q.third));
// 			    q++;
// 		    }
// 		    if (found) {
// 			    CANDIDATE ar = *pp;
// 			    #if VERSION == V_SHAPE
// 			    una.avanza(ar.first, ar.second);
// 			    #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
// 			    una.avanza(ar.first, ar.second, ar.third);
// 			    #endif
// 
// 			    candidatas = una.getCandidatos();
// 			    
//     // 			cout << "Listo: " << pp->first << "," << pp->second << "," << pp->third << endl;
// 			    
// 			    pasos.erase(pp);
// 			    pp = pasos.begin();
// 		    }
// 		}
// 	    }
// // 	    }
// // 	    else cuantos++;
// 	
// 	    cout << "Local Search: " << una.subEst();
// 	    una.calculaCostes();
// 
// 	    vector <float> cost(2);
// 	    cost[0] = una.getCoste(0);
// 	    cost[1] = una.getCoste(1);
// 	    cout << "Costo: " << cost[0] << ' ' << cost[1] << endl;

           // Actualiza el Pareto
           this->conjuntoNoDominadas.addDominancia(una, this->preferencias, this->numDominanciasPorPreferencias);
	   if ((una.getCoste(0) == 1) and (una.getCoste(1) < best.getCoste(1))) {
	     best = una;
	   }
// 	    
// // 	    // Actualiza el conjunto intermedio
// //             bool res = this->conjuntoIntermedio.addDominancia(una, true, this->numDominanciasPorPreferencias);         
    }
    
    cout << "Inicial: " << (PARA.MOACO_numHormigas * 10) - cuantos << endl;
        
    // Actualizo feromona
    vector<float> prom(this->nObj);
    
    for (int j = 0; j < (unsigned int) this->nObj; j++)
        prom[j] = 0.;
        
    // pheromone global update
    NDominatedSet::iterator p = this->conjuntoNoDominadas.begin();
    
    while (p != conjuntoNoDominadas.end()){
        for (int j = 0; j < (unsigned int) this->nObj; j++) {
            prom[j] += (*p).getCoste(j);
        }
                    
        p++;
    }
    
    float ntau = 1.;
    for (int j = 0; j < (unsigned int) this->nObj; j++) {
        ntau *= (prom[j] / this->conjuntoNoDominadas.getNumElementos());
    }
    
    this->feromonaInicial = ntau;
    cout << "Feromona incial: " << this->feromonaInicial << endl;
    
    // Iteraciones siguientes
#ifdef ENABLE_ITERATION_COUNT
    while (PARA.GLOB_maxTiempo > this->numIteraciones) {		// FOR DEBUG -- NOT FOR PRODUCTION!!!
#else
    while (PARA.GLOB_maxTiempo > difftime(fin, inicio)) {		// FOR PRODUCTION -- NOT FOR DEBUG!!!
#endif

        unsigned long at = clock()/CLOCKS_PER_SEC;
        cout << "Iteracion: " << this->numIteraciones + 1 << endl;
        this->numIteraciones++;
                
        // inicializar cada hormiga reiniciando la asignacion de nodos
        set<unsigned int> usados;
        unsigned int exito_cero = 0;
        unsigned int exito_pareto = 0;
        vector<bool> de_donde(PARA.MOACO_numHormigas, false);
        
        for (unsigned int i = 0; i < PARA.MOACO_numHormigas; i++) {
            float x = ((rand() * 1.) / (RAND_MAX));
	    cout << "Rgamma: " << x << endl;
            if (x >= PARA.MOACO_gamma) {
// 	      if (best.getCoste(0) != 1)
        	this->hormigas[i]->posicionaInicialmente();
// 	      else
// 		*(this->hormigas[i]) = best;
            }
            else {
		// Reviso posiblidades
		vector<Hormiga> cuales;
                unsigned int pp = 0;
                while (pp < conjuntoNoDominadas.getNumElementos()) {
                        if (this->conjuntoNoDominadas.getElemento(pp).extendible())
			  cuales.push_back(this->conjuntoNoDominadas.getElemento(pp));
			pp++;
                }
		
		if (cuales.size() > 0) {
                    int xx = intAzar(0, cuales.size() - 1);
                    *(this->hormigas[i]) = cuales[xx];
                    de_donde[i] = true;
                }
                else {
// 		    if (best.getCoste(0) != 1)
		      this->hormigas[i]->posicionaInicialmente();
// 		    else
// 		      *(this->hormigas[i]) = best;
                }
            }
        }
        	
        unsigned long bt = clock()/CLOCKS_PER_SEC;
	
        cout << "TIME 1: " << bt - at << endl;
        // para cada hormiga se genera constructivamente una asignacion de nodos
	
	vector<unsigned int> pasos(PARA.MOACO_numHormigas);
	unsigned int max = 0;
	// Elijo los pasos que va a dar cada una
	vector<Hormiga> caminos;
        for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++) {
//             cout << "Hormiga: " << nHormiga << endl;
            // las hormigas crean una subestructura, realizando ademas labores de actualizacion y deposito de feromona. La condicion de parada esta basada en una probabilidad de distribucion del tamanio de la subestructura (step_h/step_size)
            
            // Elijo cantidad de pasos
            unsigned int x = 0;
            if (de_donde[nHormiga]) {
                cout << "Del Pareto!" << endl;
                if (PARA.MOACO_ranking)
                    x = ranking(PARA.MOACO_stepSize, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
                else
                    x = intAzar(1, PARA.MOACO_stepSize);
            }
            else {
                cout << "Nuevo!" << endl;
                if (PARA.MOACO_ranking)
                    x = ranking(PARA.MOACO_stepSize, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
                else
                    x = intAzar(1, PARA.MOACO_stepSize);
            }
	    cout << "Pasos: " << x << endl;
	    pasos[nHormiga] = x;
	    if (x > max) {
		max = x;
	    }
	}
	
	// Avanzo paso a paso
	vector<bool> puedo(PARA.MOACO_numHormigas, true);
	unsigned int i = 0;
	while (i < max) {
	    vector< pair<bool,CANDIDATE> > hice(PARA.MOACO_numHormigas, pair<bool,CANDIDATE>(false, CANDIDATE()));
	    for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++) {
		float complejidad = this->hormigas[nHormiga]->getCoste(1);
		if (puedo[nHormiga] and (i < pasos[nHormiga]) and (complejidad < 1)) {
		    cout << "Hormiga: " << nHormiga << " paso " << i << endl;
		    float soporte = 1;
		    // candidatas posibles a ser elegidas en este paso de la hormiga
		    candidatas = this->hormigas[nHormiga]->getCandidatos();

		    cout << "Antes: " << this->hormigas[nHormiga]->subEst();
		    vector <float> coste(2);
		    coste[0] = this->hormigas[nHormiga]->getCoste(0);
		    coste[1] = this->hormigas[nHormiga]->getCoste(1);
		    cout << "Costo: " << coste[0] << ' ' << coste[1] << endl;

// 		    // Muestro los candidatos
// 		    for (vector< CANDIDATE >::iterator it1 = candidatas.begin(); it1 != candidatas.end(); it1++) {
// 	                 cout << "CAN: " << (*it1).first << ' ' << (*it1).second << endl;
// 		    }

		    // si hay una o mas candidatos seleccionamos la mejor
		    if (candidatas.size() > 0) {
			// elegir el eje mas conveniente segun informacion Greedy y feromona 
			CANDIDATE arco = this->transicion(*(hormigas[nHormiga]), nHormiga, candidatas);

			Hormiga copiar(*(hormigas[nHormiga]));
		
			copiar.avanza(arco);

			soporte = copiar.getCoste(0);
			if (soporte > 0) {
			    this->hormigas[nHormiga]->avanza(arco);

			    hice[nHormiga] = pair<bool,CANDIDATE>(true, arco);
			    Hormiga qw(*(this->hormigas[nHormiga]));
			    caminos.push_back(qw);
 			    cout << "Despues: " << this->hormigas[nHormiga]->subEst();
			}
			else {
			    cout << "No hay candidatos!" << endl;
			    puedo[nHormiga] = false;
			}
		    }
		}
	    }
		    
	    for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++) {
		if (hice[nHormiga].first) {
		    cout << "Hormiga: " << nHormiga << " avanzo" << endl;
		    // tenemos que modificar la feromona en el arco nuevo
		    // se realizan modificaciones de feromona o no dependiendo del tipo de algoritmo ACO o MOACO que implemente la clase ACO   
		    this->accionesTrasDecision(this->hormigas[nHormiga], hice[nHormiga].second.first, hice[nHormiga].second.second, hice[nHormiga].second.third);

// // 		    // Aqui agrego el local search
// 		    candidatas = this->hormigas[nHormiga]->getCandidatos();
// 		    set<CANDIDATE> pasos = this->hormigas[nHormiga]->local_search();
// 		    // pasos son todos los ejes que tengo que agregar
// 		    while (pasos.size() > 0) {
// 			for (set<CANDIDATE>::iterator pp = pasos.begin(); pp != pasos.end(); pp++) {
// 			    bool found = false;
// 			    unsigned int q = 0;
// 	// 		    cout << "Las candidatas:" << endl;
// 			    while ((q < candidatas.size()) and !found) {
// 	// 			cout << "Cand: " << candidatas[q].first << "," << candidatas[q].second << "," << candidatas[q].third << endl;
// 				found = (candidatas[q] == *pp);
// 				q++;
// 			    }
// 			    if (found) {
// 				CANDIDATE ar = *pp;
// 				#if VERSION == V_SHAPE
// 				this->hormigas[nHormiga]->avanza(ar.first, ar.second);
// 				#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
// 				this->hormigas[nHormiga]->avanza(ar.first, ar.second, ar.third);
// 				#endif
// 
// 				// tenemos que modificar la feromona en el arco nuevo
// 				// se realizan modificaciones de feromona o no dependiendo del tipo de algoritmo ACO o MOACO que implemente la clase ACO
// 				#if VERSION == V_SHAPE
// 				this->accionesTrasDecision(this->hormigas[nHormiga], ar.first, ar.second);
// 				#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
// 				this->accionesTrasDecision(this->hormigas[nHormiga], ar.first, ar.second, ar.third);
// 				#endif
// 				
// 				candidatas = this->hormigas[nHormiga]->getCandidatos();
// 				
// 	// 			cout << "Listo: " << pp->first << "," << pp->second << "," << pp->third << endl;
// 				
// 				pasos.erase(pp);
// 			    }
// 			}
// 			cout << "Inter: " << this->hormigas[nHormiga]->subEst();
// 			cout << "Faltan: " << pasos.size() << endl;
// // 			for (set<CANDIDATE>::iterator pp = pasos.begin(); pp != pasos.end(); pp++) {
// // 			    cout << pp->first << "," << pp->second << "," << pp->third << endl;
// // 			}
// 		    }
// 		    
// 		    cout << "Local Search: " << this->hormigas[nHormiga]->subEst();
// 		    this->hormigas[nHormiga]->calculaCostes();
// 
// 		    vector <float> cost(2);
// 		    cost[0] = this->hormigas[nHormiga]->getCoste(0);
// 		    cost[1] = this->hormigas[nHormiga]->getCoste(1);
// 		    cout << "Costo: " << cost[0] << ' ' << cost[1] << endl;
		    
// 		    // Actualiza el Pareto
// 		    // Intermedio...
		    bool res = this->conjuntoNoDominadas.addDominancia(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias);
		    if ((this->hormigas[nHormiga]->getCoste(0) == 1) and (this->hormigas[nHormiga]->getCoste(1) < best.getCoste(1))) {
		      best = *(this->hormigas[nHormiga]);
		    }
// 		    cout << "Intermedio: " << res << endl;
		}
	    }
	    i++;
	}
    
// 	// Rocio: semi path relinking
// 	for (vector<Hormiga>::iterator qq = caminos.begin(); qq != caminos.end(); qq++) {
// 	    candidatas = (*qq).getCandidatos();
// 	    set<CANDIDATE> pasos = (*qq).local_search();
// 	    // pasos son todos los ejes que tengo que agregar
// 	    while (pasos.size() > 0) {
// 		for (set<CANDIDATE>::iterator pp = pasos.begin(); pp != pasos.end(); pp++) {
// 		    bool found = false;
// 		    unsigned int q = 0;
// 	// 	    cout << "Las candidatas:" << endl;
// 		    while ((q < candidatas.size()) and !found) {
// 	// 		cout << "Cand: " << candidatas[q].first << "," << candidatas[q].second << "," << candidatas[q].third << endl;
// 			found = (candidatas[q] == *pp);
// 			q++;
// 		    }
//  		    if (found) {
// 			CANDIDATE ar = *pp;
// 			#if VERSION == V_SHAPE
// 			(*qq).avanza(ar.first, ar.second);
// 			#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
// 			(*qq).avanza(ar.first, ar.second, ar.third);
// 			#endif
// 
// 			candidatas = (*qq).getCandidatos();
// 				
// 	// 		cout << "Listo: " << pp->first << "," << pp->second << "," << pp->third << endl;
// 				
// 			pasos.erase(pp);
// 		    }
// 		}
// 		// Actualiza el Pareto
// 		bool res = this->conjuntoNoDominadas.addDominancia(*qq, this->preferencias, this->numDominanciasPorPreferencias);
// 		if (res)
// 		    cout << "Intermedio!" << endl;
// 	// 		cout << "Inter: " << this->hormigas[nHormiga]->subEst();
// 	// 		cout << "Faltan: " << pasos.size() << endl;
// 	// 		for (set<CANDIDATE>::iterator pp = pasos.begin(); pp != pasos.end(); pp++) {
// 	// 		    cout << pp->first << "," << pp->second << "," << pp->third << endl;
// 	// 		}
// 	    }
// 	}
    
        unsigned long ct = clock()/CLOCKS_PER_SEC;
        cout << "TIME 2: " << ct - bt << endl;
        
        for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++){
            // obtenemos el fenotipo a partir del genotipo,
            // que es la subestructura obtenida por la hormiga.
            // Antes de hacer esto tenemos calcular los costes o valores de los objetivos 
// 	    cout << this->hormigas[nHormiga]->subEst();
            this->hormigas[nHormiga]->calculaCostes();             
// 	    vector <float> cost(2);
// 	    cost[0] = this->hormigas[nHormiga]->getCoste(0);
// 	    cost[1] = this->hormigas[nHormiga]->getCoste(1);
// 	    cout << "Costo: " << cost[0] << ' ' << cost[1] << endl;
            this->numEvaluaciones++; 
            
// 	    // Actualiza el conjunto intermedio
//             bool res = this->conjuntoIntermedio.addDominancia(*(this->hormigas[nHormiga]), true, this->numDominanciasPorPreferencias);
// 	    
            // Actualiza el Pareto
            bool res = this->conjuntoNoDominadas.addDominancia(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias);         
	    if ((this->hormigas[nHormiga]->getCoste(0) == 1) and (this->hormigas[nHormiga]->getCoste(1) < best.getCoste(1))) {
	      best = *(this->hormigas[nHormiga]);
	    }
	    
            if (res and de_donde[nHormiga])
                exito_pareto++;
            else if (res and !de_donde[nHormiga])
                exito_cero++;
        }
        
        unsigned long dt = clock()/CLOCKS_PER_SEC;
        cout << "TIME 3: " << dt - ct << endl;
	cout << "CI: " << this->conjuntoNoDominadas.getNumElementos() << endl;
        // acciones finales de actualizacion de feromona y demas
        // tras finalizar la iteracion 
        this->accionesFinalesHormiga();       
                		
        tiempoIteracionAnterior = fin;
        fin = time( NULL );
        tiempoHastaUltimaIteracion = tiempoIteracionAnterior - inicio;
        tiempoTranscurrido = difftime(fin, inicio);
        cout << "TT: " << difftime(fin, inicio) << endl;

        // Muestro datos de exito
        unsigned int suma = 0;
        for (unsigned int conta = 0; conta < de_donde.size(); conta++)
            if (de_donde[conta]) suma++;
        
        cout << "Exito desde cero: " << exito_cero * 1.0 / (de_donde.size() * 1.0 - suma) << ":" << exito_cero << ":" << (de_donde.size() * 1.0 - suma) << endl;
        cout << "Exito desde Pareto: " << exito_pareto * 1.0 / suma << ":" << exito_pareto << ":" << suma << endl;
        
        // operaciones de impresion en archivos de los resultados intermedios conseguidos
        // siempre que el flag este activado (los flags se guardan en el fichero utils.h)
        if ((this->numIteraciones % 2) == 0) {
            // Imprimo el Pareto cada 100 iteraciones
            string temp = nombreFichero + "_ite_";
            stringstream s;
	    int num = this->numIteraciones;
            s << num;
            temp += s.str();
            
            this->conjuntoNoDominadas.writePareto(temp.c_str());
        }
        
       /* if (GUARDAR_RESULTADOS_INTERMEDIOS) {
            if (tiempoTranscurrido >= 10. && tiempoHastaUltimaIteracion < 10.){
                    nombreFichero += "10s";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 20. && tiempoHastaUltimaIteracion < 20.){
                    nombreFichero += "20s";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 30. && tiempoHastaUltimaIteracion < 30.){
                    nombreFichero += "30s";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 60. && tiempoHastaUltimaIteracion < 60.){
                    nombreFichero += "1m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-2,3);
            }

            else if (tiempoTranscurrido >= 120. && tiempoHastaUltimaIteracion < 120.){
                    nombreFichero += "2m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-2,3);
            }

            else if (tiempoTranscurrido >= 300. && tiempoHastaUltimaIteracion < 300.){
                    nombreFichero += "5m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-2,3);
            }

            else if (tiempoTranscurrido >= 600. && tiempoHastaUltimaIteracion < 600.){
                    nombreFichero += "10m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-2,3);
            }
        }      */                  
    }
    
    // almacenamiento del tiempo de finalizacion del algoritmo y del tiempo de la BL
    this->finAlgoritmo = clock()/CLOCKS_PER_SEC;
    this->tiempoDedicadoBLs /= CLOCKS_PER_SEC;
    
    return this->conjuntoNoDominadas;   
}
