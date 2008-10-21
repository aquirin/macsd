#include "aco.h"
#include <sstream>

//---------------------------------------------------------
ACO::ACO (vector< SOLUTION >& b, Parametros &params) : AlgoritmoMO (PARA.GLOB_BL, PARA.GLOB_numObjs, PARA.GLOB_preferencias), base(b) {      	
    	
    unsigned int i,j,k;
    
    // inicializamos a 0 las n probabilidades para movernos en cada nodo
    // un vector con n probabilidades, una por cada n-esima nodo
#if VERSION == V_SHAPE
    vector< CANDIDATE > lista = b[0].posibilidades_totales();
    for (vector< CANDIDATE >::iterator p = lista.begin(); p != lista.end(); p++)
        this->probabilidades[*p] = 0.;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    set< CANDIDATE >* tent = b[0].base_ejes();
    set< CANDIDATE >::iterator p = tent->begin();
    for (; p != tent->end(); p++)
        this->probabilidades[CANDIDATE((*p).first,(*p).second,(*p).third)] = 0.;
#endif

 
    // creamos las hormigas y les asignamos su colonia
    for(i = 0; i < PARA.MOACO_numHormigas; i++){
        Hormiga *sec = new Hormiga (0, base, PARA.GLOB_numObjs, params.aparEje);
        this->hormigas.push_back(sec);
    }
    _aparEje = params.aparEje;
}

//---------------------------------------------------------
ACO::~ACO (){
	
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
    unsigned long inicio, fin, tiempoIteracionAnterior, inicioBL;
    string nombreFichero;
    double tiempoTranscurrido, tiempoHastaUltimaIteracion;    
    
    // variables dependientes de los grafos
    vector< CANDIDATE > candidatas;
    
    nombreFichero = filename;
    tiempoTranscurrido = tiempoHastaUltimaIteracion = 0.;    
    this->inicioAlgoritmo = inicio = fin = tiempoIteracionAnterior = clock()/CLOCKS_PER_SEC;

    // Iteration 1
    vector <SOLUTION> inicial;
    
    // creamos las hormigas y les asignamos su colonia
    inicial.push_back(this->hormigas[0]->subEst());
    inicial[0].clear();    
    #if VERSION == V_SHAPE
        inicial[0].agregarNodo(1, "object");
    #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
        // Nada
    #endif
    for (unsigned int n = 1; n < PARA.MOACO_numHormigas * 10; n++) {
//         cout << "Inicial: " << n << endl;
        vector < CANDIDATE > dondeir;
        inicial.push_back(this->hormigas[0]->subEst());
        inicial[n].clear();
        #if VERSION == V_SHAPE
            inicial[n].agregarNodo(1, "object");
        #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            // Nada
        #endif
        
        // Elijo cantidad de pasos
        unsigned int x;
        if (PARA.MOACO_ranking)
            x = ranking(PARA.MOACO_maxTama, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
        else
            x = intAzar(1, PARA.MOACO_stepSize);
        for (unsigned int i = 0; i < x; i++) {
//             cout << "It: " << i << " " << x << endl;
            // candidatas posibles a ser elegidas en este paso de la hormiga
            #if VERSION == V_SHAPE
                candidatas = inicial[n].nodosNoUtilizados();
            #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
                candidatas.clear();
                set<unsigned int> nu = inicial[n].nodosUtilizados();
                vector< CANDIDATE > can = inicial[n].ejesNoUtilizados();
                if (nu.size() > 0) {
                    vector< CANDIDATE >::iterator it1 = can.begin();        
                    while (it1 != can.end()) {
                        if (nu.find((*it1).second) != nu.end())
                            candidatas.push_back(*it1);
                        it1++;
                    }
                }
            #endif    
                
            // Elijo un candidato al azar
            unsigned int y = intAzar(0, candidatas.size() - 1);
            if (candidatas.size() > 0) {
                #if VERSION == V_SHAPE
//                     cout << candidatas[y].first << "  " << candidatas[y].second << endl;
                    int cual;
                    if (candidatas[y].second == "object")
                        cual = 1;
                    else
                        cual = 2;
                    inicial[n].agregarNodo(inicial[n].cantNodos() + 1, candidatas[y].second);
                    inicial[n].agregarEje(candidatas[y].first, inicial[n].cantNodos(), cual);
                #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
                    inicial[n].agregarEje(candidatas[y].first, candidatas[y].second, candidatas[y].third);
                #endif                
            }
            else {
                cout << "No hay candidatos!" << endl;
            }
        }
    }
        
    for (unsigned int n = 0; n < PARA.MOACO_numHormigas * 10; n++){
            // Pasamos cada SOLUCION a una hormiga
//             cout << inicial[n] << endl;
            #if VERSION == V_SHAPE
       	        Hormiga una(this->base, this->nObj, this->_aparEje, inicial[n]);
            #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
                Hormiga una(1, this->base, this->nObj, this->_aparEje, inicial[n]);
            #endif 
            una.calculaCostes();             
            
            // Actualiza el Pareto
            this->conjuntoNoDominadas.addDominancia(una, this->preferencias, this->numDominanciasPorPreferencias);         
    }
    
        
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
    while (PARA.GLOB_maxTiempo > (fin - inicio)) {		// FOR PRODUCTION -- NOT FOR DEBUG!!!
#endif

        unsigned long at = clock()/CLOCKS_PER_SEC;
        cout << "Iteracion: " << numIteraciones + 1 << endl;
        this->numIteraciones++;
                
        // inicializar cada hormiga reiniciando la asignacion de nodos
        set<unsigned int> usados;
        unsigned int exito_cero = 0;
        unsigned int exito_pareto = 0;
        vector<bool> de_donde(PARA.MOACO_numHormigas, 0);
        
        for (unsigned int i = 0; i < PARA.MOACO_numHormigas; i++) {
            float x = ((rand() * 1.) / (RAND_MAX));
            if (x >= PARA.MOACO_gamma) {
        	this->hormigas[i]->posicionaInicialmente();
            }
            else {
                if ((conjuntoNoDominadas.getNumElementos() > 0) && (usados.size() < conjuntoNoDominadas.getNumElementos())) {
                    // Recupero desde el Pareto
                    // Sin reposicion
                    // a menos que no haya mas diferentes
//                     if (usados.size() == conjuntoNoDominadas.getNumElementos()) {
//                         usados.clear();
//                     }
                    int xx = intAzar(0, this->conjuntoNoDominadas.getNumElementos() - 1);
                    while ((usados.find(xx) != usados.end()) and (!this->conjuntoNoDominadas.getElemento(xx).extendible())) {
                        int xx = intAzar(0, this->conjuntoNoDominadas.getNumElementos() - 1);
                    }
                    *(this->hormigas[i]) = this->conjuntoNoDominadas.getElemento(xx);
                    de_donde[i] = true;
                }
                else {
                    this->hormigas[i]->posicionaInicialmente();
                }
            }
        }
        	
        unsigned long bt = clock()/CLOCKS_PER_SEC;
        cout << "TIME 1: " << bt - at << endl;
        // para cada hormiga se genera constructivamente una asignacion de nodos
        for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++) {
            cout << "Hormiga: " << nHormiga << endl;
            // las hormigas crean una subestructura, realizando ademas labores de actualizacion y deposito de feromona. La condicion de parada esta basada en una probabilidad de distribucion del tamanio de la subestructura (step_h/step_size)
            
            // Elijo cantidad de pasos
            unsigned int x = 0;
            if (de_donde[nHormiga]) {
                if (PARA.MOACO_ranking)
                    x = ranking(PARA.MOACO_maxTama, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
                else
                    x = intAzar(1, PARA.MOACO_stepSize);
            }
            else {
                 if (PARA.MOACO_ranking)
                    x = ranking(PARA.MOACO_maxTama, /*nu_max*/ 1.99, /*nu_min*/ 0.01);
                else
                    x = intAzar(1, PARA.MOACO_stepSize);
            }
            cout << "Pasos " << x << endl;
            for (unsigned int i = 0; i < x; i++) {
                    // candidatas posibles a ser elegidas en este paso de la hormiga
                    candidatas = this->hormigas[nHormiga]->getCandidatos();
                    
//                     cout << this->hormigas[nHormiga]->subEst();

//                     cout << "Candidatas: ";
//                     for (vector< CANDIDATE >::iterator p = candidatas.begin(); p != candidatas.end(); p++) {
//                         cout << '(' << (*p).first << ',' << (*p).second << ')' << endl;
//                     }              

                    // si hay una o mas candidatos seleccionamos la mejor
                    if (candidatas.size() > 0) {
                            // elegir el eje mas conveniente segun informacion Greedy y feromona 
			    CANDIDATE arco = this->transicion(*(hormigas[nHormiga]), nHormiga, candidatas);
                
// #if VERSION == V_SHAPE
//                             cout << "ARCO: " << arco.first << ' ' << arco.second << endl;
// #elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
//                             cout << "ARCO: " << arco.first << ' ' << arco.second << ' ' << arco.third << endl;
// #endif


#if VERSION == V_SHAPE
this->hormigas[nHormiga]->avanza(arco.first, arco.second);
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
this->hormigas[nHormiga]->avanza(arco.first, arco.second, arco.third);
#endif
                            
                                
                            // tenemos que modificar la feromona en el arco nuevo
                            // se realizan modificaciones de feromona o no dependiendo del tipo de algoritmo ACO o MOACO que implemente la clase ACO
#if VERSION == V_SHAPE
this->accionesTrasDecision(this->hormigas[nHormiga], arco.first, arco.second);
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
this->accionesTrasDecision(this->hormigas[nHormiga], arco.first, arco.second, arco.third);
#endif                            

                    }
                    else {
                        cout << "No hay candidatos!" << endl;
                    }            
            }
        }    
    
        unsigned long ct = clock()/CLOCKS_PER_SEC;
        cout << "TIME 2: " << ct - bt << endl;
        
        for (unsigned int nHormiga = 0; nHormiga < PARA.MOACO_numHormigas; nHormiga++){
            // obtenemos el fenotipo a partir del genotipo,
            // que es la subestructura obtenida por la hormiga.
            // Antes de hacer esto tenemos calcular los costes o valores de los objetivos 
            this->hormigas[nHormiga]->calculaCostes();             
            this->numEvaluaciones++; 
            
            // Actualiza el Pareto
            bool res = this->conjuntoNoDominadas.addDominancia(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias);         
            if (res and de_donde[nHormiga])
                exito_pareto++;
            else if (res and !de_donde[nHormiga])
                exito_cero++;
        }
        
        unsigned long dt = clock()/CLOCKS_PER_SEC;
        cout << "TIME 3: " << dt - ct << endl;

        // acciones finales de actualizacion de feromona y demas
        // tras finalizar la iteracion 
        this->accionesFinalesHormiga();       
                		
        tiempoIteracionAnterior = fin;
        fin = clock()/CLOCKS_PER_SEC;
        tiempoHastaUltimaIteracion = tiempoIteracionAnterior - inicio;
        tiempoTranscurrido = fin - inicio;

        // Muestro datos de exito
        unsigned int suma = 0;
        for (unsigned int conta = 0; conta < de_donde.size(); conta++)
            if (de_donde[conta]) suma++;
        
        cout << "Exito desde cero: " << exito_cero * 1.0 / (de_donde.size() * 1.0 - suma) << ":" << exito_cero << ":" << (de_donde.size() * 1.0 - suma) << endl;
        cout << "Exito desde Pareto: " << exito_pareto * 1.0 / suma << ":" << exito_pareto << ":" << suma << endl;
        
        // operaciones de impresion en archivos de los resultados intermedios conseguidos
        // siempre que el flag este activado (los flags se guardan en el fichero utils.h)
        if (numIteraciones % 100 == 0) {
            // Imprimo el Pareto cada 100 iteraciones
            string temp = nombreFichero + "_ite_";
            stringstream s;
            s << numIteraciones;
            temp += s.str();
            
            this->conjuntoNoDominadas.writePareto(temp.c_str());
        }
        
        if (GUARDAR_RESULTADOS_INTERMEDIOS) {
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
        }          
                   
    }
    
    // almacenamiento del tiempo de finalizacion del algoritmo y del tiempo de la BL
    this->finAlgoritmo = clock()/CLOCKS_PER_SEC;
    this->tiempoDedicadoBLs /= CLOCKS_PER_SEC;
    
    return this->conjuntoNoDominadas;   
}
