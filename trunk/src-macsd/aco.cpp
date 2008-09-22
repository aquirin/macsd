#include "aco.h"

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
    set< CANDIDATE >* tent = b[0].second.base_ejes();
    set< CANDIDATE >::iterator p = tent->begin();
    for (; p != tent->end(); p++)
        this->probabilidades[CANDIDATE((*p).first,(*p).second,(*p).third)] = 0.;
#endif

 
    // creamos las hormigas y les asignamos su colonia
    for(i = 0; i < PARA.MOACO_numHormigas; i++){
        Hormiga *sec = new Hormiga (0, base, PARA.GLOB_numObjs, params.aparEje);
        this->hormigas.push_back(sec);
    }
  
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
#if VERSION == V_SHAPE
    bool first = true;		/* TODO: HARMONIZE */
#endif

    // variables de control de tiempos y del algoritmo generico
    unsigned long inicio, fin, tiempoIteracionAnterior, inicioBL;
    string nombreFichero;
    double tiempoTranscurrido, tiempoHastaUltimaIteracion;    
    
    // variables dependientes de los grafos
    vector< CANDIDATE > candidatas;

    
    nombreFichero = filename;
    tiempoTranscurrido = tiempoHastaUltimaIteracion = 0.;    
    this->inicioAlgoritmo = inicio = fin = tiempoIteracionAnterior = clock()/CLOCKS_PER_SEC;

     // Iteration 1 to (hasta la condicion de parada)

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
        for (unsigned int i = 0; i < PARA.MOACO_numHormigas; i++) {
            float x = ((rand() * 1.) / (RAND_MAX));
            if (x > PARA.MOACO_gamma) {
        	this->hormigas[i]->posicionaInicialmente();
#if VERSION == V_SHAPE
                if (first) {		/* TODO: HARMONIZE */
                    // Actualiza el Pareto
                    this->conjuntoNoDominadas.addDominancia(*(this->hormigas[i]), this->preferencias, this->numDominanciasPorPreferencias);      
                    first = false;
                }
#endif

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
                    while (usados.find(xx) != usados.end()) {
                        int xx = intAzar(0, this->conjuntoNoDominadas.getNumElementos() - 1);
                    }
                    *(this->hormigas[i]) = this->conjuntoNoDominadas.getElemento(xx);
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
            unsigned int x = intAzar(1, PARA.MOACO_stepSize);
            cout << "Pasos " << x << endl;
            bool done = false;

#if VERSION == V_SHAPE
            for (unsigned int i = 0; (i < x) && !done; i++) {	/* TODO: HARMONIZE */
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
            for (unsigned int i = 0; i < x; i++) {
#endif

                    // candidatas posibles a ser elegidas en este paso de la hormiga
                    candidatas = this->hormigas[nHormiga]->getCandidatos();
                    
#if VERSION == V_SHAPE
                    cout << this->hormigas[nHormiga]->subEst();

                    cout << "Candidatas: ";
                    for (vector< CANDIDATE >::iterator p = candidatas.begin(); p != candidatas.end(); p++) {
                        cout << '(' << (*p).first << ',' << (*p).second << ')' << endl;
                    }
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
                    cout << this->hormigas[nHormiga]->subEst().second;
#endif

                   

                    

                    
                    // si hay una o mas candidatos seleccionamos la mejor
                    if (candidatas.size() > 0) {
                            // elegir el eje mas conveniente segun informacion Greedy y feromona 
							CANDIDATE arco = this->transicion(*(hormigas[nHormiga]), nHormiga, candidatas);
                
#if VERSION == V_SHAPE
                            cout << "ARCO: " << arco.first << ' ' << arco.second << endl;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
                            cout << "ARCO: " << arco.first << ' ' << arco.second << ' ' << arco.third << endl;
#endif


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
                        done = (candidatas.size() == 0);        
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
            this->conjuntoNoDominadas.addDominancia(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias);         
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

        // operaciones de impresion en archivos de los resultados intermedios conseguidos
        // siempre que el flag este activado (los flags se guardan en el fichero utils.h)
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
/* TODO: HARMONIZE (next line: you have -2, in my old code I have -3) */                    
//nombreFichero.erase(nombreFichero.size()-2,3);

nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 120. && tiempoHastaUltimaIteracion < 120.){
                    nombreFichero += "2m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
/* TODO: HARMONIZE (next line: you have -2, in my old code I have -3) */  
//nombreFichero.erase(nombreFichero.size()-2,3);
nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 300. && tiempoHastaUltimaIteracion < 300.){
                    nombreFichero += "5m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    /* TODO: HARMONIZE (next line: you have -2, in my old code I have -3) */
//nombreFichero.erase(nombreFichero.size()-2,3);
nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 600. && tiempoHastaUltimaIteracion < 600.){
                    nombreFichero += "10m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }
        }          
                   
    }
    
    // almacenamiento del tiempo de finalizacion del algoritmo y del tiempo de la BL
    this->finAlgoritmo = clock()/CLOCKS_PER_SEC;
    this->tiempoDedicadoBLs /= CLOCKS_PER_SEC;
    
    return this->conjuntoNoDominadas;   
}
