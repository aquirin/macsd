#include "aco.h"

//---------------------------------------------------------
ACO::ACO (vector<shapes>& b, Parametros &params) : AlgoritmoMO (params.BL, params.numObjs, params.preferencias), base(b) {      	
	unsigned int i,j,k;
        
        this->stepSize = 5;
	
        this->gamma = params.gamma;
        
	// copia de parametros
        this->numHormigas = params.numHormigas;   
        this->numColonias = params.numColonias;
    
        // inicializamos a 0 las n probabilidades para movernos en cada nodo
        // un vector con n probabilidades, una por cada n-esima nodo
        for (i = 0; i < matricesFeromona.size(); i++)
            for (j = 0; j < matricesFeromona.size(); j++)
                for (k = 0; k < matricesFeromona[0][0].size(); k++)
                    this->probabilidades[tuplax3<unsigned int>(i+1,j+1,k+1)] = 0.;
        
    // creamos las hormigas y les asignamos su colonia
    for(i = 0; i < this->numHormigas; i++){
        Hormiga *sec = new Hormiga (0, base, params.numObjs);
        this->hormigas.push_back(sec);
    }
  
}

//---------------------------------------------------------
ACO::~ACO (){
	
	for(unsigned int i = 0; i < this->numHormigas; i++) {
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
	this->umbrales = new float[this->numHormigas];
	this->heuristicas =  new unsigned int[this->numHormigas];
		
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
		
        for (unsigned int i = 0; i < this->numHormigas; i++)
            this->umbrales[i] = (1 / this->numHormigas) * (i+1) - 0.005;
        
        for (unsigned int i = 0; i<this->numHormigas; i++)
            this->heuristicas[i] = multiheuristics;
}

//---------------------------------------------------------

NDominatedSet & ACO::ejecuta (string &filename) {
    // variables de control de tiempos y del algoritmo generico
    unsigned long inicio, fin, tiempoIteracionAnterior, inicioBL;
    string nombreFichero;
    double tiempoTranscurrido, tiempoHastaUltimaIteracion;    
    
    // variables dependientes de los grafos
    vector< tuplax3<unsigned int> > candidatas;
    
    nombreFichero = filename;
    tiempoTranscurrido = tiempoHastaUltimaIteracion = 0.;    
    this->inicioAlgoritmo = inicio = fin = tiempoIteracionAnterior = clock()/CLOCKS_PER_SEC;

    // Iteration 0
    // inicializar cada hormiga reiniciando la asignacion de nodos
    cout << "Iteracion: 0" << endl;
    
    vector<float> proba(matricesFeromona.size(), 0.);
    float suma = 0.;
    for (unsigned int i = 0; i < matricesFeromona.size(); i++) {
        for (unsigned int j = 0; j < matricesFeromona.size(); j++)
            if (i != j)
                for (unsigned int k = 0; k < matricesFeromona[0][0].size(); k++) {
                    suma += matricesFeromona[i][j][k];
//                     suma += matricesFeromona[j][i][k];
                }
        proba[i] = suma;
    }
            
    for (unsigned int i = 0; i < this->numHormigas; i++)
        this->hormigas[i]->posicionaInicialmente(proba);
            
    // para cada hormiga se genera constructivamente una asignacion de nodos
    for (unsigned int nHormiga = 0; nHormiga < this->numHormigas; nHormiga++) {
        cout << "Hormiga: " << nHormiga << endl;
        // las hormigas crean una subestructura, realizando ademas labores de actualizacion y deposito de feromona. La condicion de parada esta basada en una probabilidad de distribucion del tamanio de la subestructura (step_h/step_size)
        float x = ((this->hormigas[nHormiga]->getEjesAsignados() * 1.) / (stepSize * 1.)) + 1;
        bool done = false;

        while (!done && (x > ((this->hormigas[nHormiga]->getEjesAsignados() * 1.) / (stepSize * 1.)))) {
                x = ((rand() * 1.) / (RAND_MAX * 1.));
                cout << this->hormigas[nHormiga]->getEjesAsignados() << ' ' << stepSize << endl;
                cout << "Random: " << x << endl;
                // candidatas posibles a ser elegidas en este paso de la hormiga
                candidatas = this->hormigas[nHormiga]->getCandidatos();
                
//                 cout << this->hormigas[nHormiga]->subEst();
//                 cout << "Candidatas: ";
//                 for (vector< tuplax3<unsigned int> >::iterator p = candidatas.begin(); p != candidatas.end(); p++) {
//                     cout << '(' << (*p).first << ',' << (*p).second << ',' << (*p).third << ')' << endl;
//                 }
    
                // si hay una o mas candidatos seleccionamos la mejor
                if (candidatas.size() > 0) {
                        // elegir el eje mas conveniente segun informacion Greedy y feromona      			
                        tuplax3<unsigned int> arco = this->transicion(*(hormigas[nHormiga]), nHormiga, candidatas);
                                        
//                         cout << arco.first << ' ' << arco.second << ' ' << arco.third << endl;
                        
                        this->hormigas[nHormiga]->avanza(arco.first, arco.second, arco.third);
                }
                else {
                    done = (candidatas.size() == 0);        
                }                   
        }
    }
    
    for (unsigned int nHormiga = 0; nHormiga < this->numHormigas; nHormiga++){
        // obtenemos el fenotipo a partir del genotipo,
        // que es la subestructura obtenida por la hormiga.
        // Antes de hacer esto tenemos calcular los costes o valores de los objetivos 
        this->hormigas[nHormiga]->calculaCostes();             
        this->numEvaluaciones++; 
        
        // Actualiza el Pareto
        int x = 0;
        if (!this->conjuntoNoDominadas.esDominada(*(this->hormigas[nHormiga]), false, x)) {
                            
                if (this->conjuntoNoDominadas.add(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias)) {
    
                    this->tiempoUltimaModificacion = clock()/CLOCKS_PER_SEC;
                    this->evaluacionesUltimaModificacion = this->numEvaluaciones;
                    // this->blsUltimaModificacion = this->numBLs;
                    this->iteracionUltimaModificacion = this->numIteraciones;                       
                } 
                
            }	          
    }
            
    // acciones finales de actualizacion de feromona y demas
    // tras finalizar la iteracion 
    this->accionesFinalesHormiga();       

    // Iteration 1 to (hasta la condicion de parada)
    while (this->maxTiempo > (fin - inicio)) {		// FOR PRODUCTION -- NOT FOR DEBUG!!!
    //while (this->maxTiempo > this->numIteraciones) {		// FOR DEBUG -- NOT FOR PRODUCTION!!!
        cout << "Iteracion: " << numIteraciones + 1 << endl;
        this->numIteraciones++;
        
        for (unsigned int i = 0; i < proba.size(); i++) {
            proba[i] = 0.;
        }
        suma = 0.;
        for (unsigned int i = 0; i < matricesFeromona.size(); i++) {
            for (unsigned int j = 0; j < matricesFeromona.size(); j++)
               if (i != j)
                  for (unsigned int k = 0; k < matricesFeromona[0][0].size(); k++) {
                      suma += matricesFeromona[i][j][k];
//                     suma += matricesFeromona[j][i][k];
                   }
           proba[i] = suma;
        }
        
        // inicializar cada hormiga reiniciando la asignacion de nodos
        set<unsigned int> usados;
        for (unsigned int i = 0; i < this->numHormigas; i++) {
            float x = ((rand() * 1.) / (RAND_MAX));
            if (x > this->gamma)
        	this->hormigas[i]->posicionaInicialmente(proba);
            else {
                if (conjuntoNoDominadas.getNumElementos() > 0) {
                    // Recupero desde el Pareto
                    // Sin reposicion
                    // a menos que no haya mas diferentes
                    if (usados.size() == conjuntoNoDominadas.getNumElementos()) {
                        usados.clear();
                    }
                    int xx = intAzar(0, this->conjuntoNoDominadas.getNumElementos() - 1);
                    while (usados.find(xx) != usados.end()) {
                        int xx = intAzar(0, this->conjuntoNoDominadas.getNumElementos() - 1);
                    }
                    *(this->hormigas[i]) = this->conjuntoNoDominadas.getElemento(xx);
                }
                else {
                    this->hormigas[i]->posicionaInicialmente(proba);
                }
            }
        }
        	
        // para cada hormiga se genera constructivamente una asignacion de nodos
        for (unsigned int nHormiga = 0; nHormiga < this->numHormigas; nHormiga++) {
            cout << "Hormiga: " << nHormiga << endl;
            // las hormigas crean una subestructura, realizando ademas labores de actualizacion y deposito de feromona. La condicion de parada esta basada en una probabilidad de distribucion del tamanio de la subestructura (step_h/step_size)
            float x = ((this->hormigas[nHormiga]->getEjesAsignados() * 1.) / (stepSize * 1.)) + 1;
            bool done = false;

            while (!done && (x > ((this->hormigas[nHormiga]->getEjesAsignados() * 1.) / (stepSize * 1.)))) {
                    x = ((rand() * 1.) / (RAND_MAX * 1.));
                    cout << this->hormigas[nHormiga]->getEjesAsignados() << ' ' << stepSize << endl;
                    cout << "Random: x=" << x << ", nHormiga=" << nHormiga << ", getEjesAsignados=" << this->hormigas[nHormiga]->getEjesAsignados() << ", stepSize=" << stepSize << endl;
                    // candidatas posibles a ser elegidas en este paso de la hormiga
                    candidatas = this->hormigas[nHormiga]->getCandidatos();
                    
//                     cout << this->hormigas[nHormiga]->subEst();
//                     cout << "Candidatas: ";
//                     for (vector< tuplax3<unsigned int> >::iterator p = candidatas.begin(); p != candidatas.end(); p++) {
//                         cout << '(' << (*p).first << ',' << (*p).second << ',' << (*p).third << ')' << endl;
//                     }
//         
                    // si hay una o mas candidatos seleccionamos la mejor
                    if (candidatas.size() > 0) {
                            // elegir el eje mas conveniente segun informacion Greedy y feromona      			
                            tuplax3<unsigned int> arco = this->transicion(*(hormigas[nHormiga]), nHormiga, candidatas);
                                            
//                            cout << arco.first << ' ' << arco.second << ' ' << arco.third << endl;
                            
                            this->hormigas[nHormiga]->avanza(arco.first, arco.second, arco.third);
                                
                            // tenemos que modificar la feromona en el arco nuevo
                            // se realizan modificaciones de feromona o no dependiendo del tipo de algoritmo ACO o MOACO que implemente la clase ACO
                            this->accionesTrasDecision(this->hormigas[nHormiga], arco.first, arco.second, arco.third);
                    }
                    else {
                        done = (candidatas.size() == 0);        
                    }                   
            }
        }    
    
        for (unsigned int nHormiga = 0; nHormiga < this->numHormigas; nHormiga++){
            // obtenemos el fenotipo a partir del genotipo,
            // que es la subestructura obtenida por la hormiga.
            // Antes de hacer esto tenemos calcular los costes o valores de los objetivos 
            this->hormigas[nHormiga]->calculaCostes();             
            this->numEvaluaciones++; 
            
            // Actualiza el Pareto
            int x = 0;
            if (!this->conjuntoNoDominadas.esDominada(*(this->hormigas[nHormiga]), false, x)) {
            			
	            if (this->conjuntoNoDominadas.add(*(this->hormigas[nHormiga]), this->preferencias, this->numDominanciasPorPreferencias)) {
	
	                this->tiempoUltimaModificacion = clock()/CLOCKS_PER_SEC;
	                this->evaluacionesUltimaModificacion = this->numEvaluaciones;
	               // this->blsUltimaModificacion = this->numBLs;
	                this->iteracionUltimaModificacion = this->numIteraciones;                       
	            } 
	            
        	}	          
        }
              
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
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 120. && tiempoHastaUltimaIteracion < 120.){
                    nombreFichero += "2m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
                    nombreFichero.erase(nombreFichero.size()-3,3);
            }

            else if (tiempoTranscurrido >= 300. && tiempoHastaUltimaIteracion < 300.){
                    nombreFichero += "5m";
                    this->conjuntoNoDominadas.writePareto(nombreFichero.c_str());
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
