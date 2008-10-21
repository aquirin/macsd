#include "macs.h"

//---------------------------------------------------------
MACS::MACS (vector<SOLUTION>& b, Parametros &params) : ACO (b, params) {
    unsigned int i,j;
    vector <float> valoresLambda, valoresNoLambda;
    
    // inicializacion de umbrales y heuristicas para cada hormiga en un enfoque multiheuristics
    this->setUmbralesYHeuristicas(PARA.MOACO_multiheuristics);
    
    this->feromonaInicial = PARA.MOACO_tau0;
    
    for (i = 0; i < PARA.MOACO_numHormigas; i++){
        valoresLambda.push_back(((float) i) / PARA.MOACO_numHormigas);
        valoresNoLambda.push_back(1. - valoresLambda[i]);
        this->betaLambda.push_back(PARA.MOACO_beta * valoresLambda[i]);  // en este vector se guarda el valor lambda*beta
        this->betaNoLambda.push_back(PARA.MOACO_beta * valoresNoLambda[i]);  // en este vector se guarda el valor (lambda-1)*beta
    }
    
    // la matriz de feromona seria una matriz NxNXl
#if VERSION == V_SHAPE
    vector< CANDIDATE > lista = b[0].posibilidades_totales();
    for (vector< CANDIDATE >::iterator p = lista.begin(); p != lista.end(); p++) {   
        this->matricesFeromona[*p] = this->feromonaInicial; 
    }
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    for (set< CANDIDATE >::iterator j = b[0].base_ejes()->begin(); j != b[0].base_ejes()->end(); j++) {   
        this->matricesFeromona[*j] = this->feromonaInicial; 
    }
#endif

//     cout << "Feromonas: " << matricesFeromona.size() << ' ' << matricesFeromona.size() << ' ' << matricesFeromona[0][0].size() << endl;
}

//---------------------------------------------------------
double MACS::calculaNumeradoresProbabilidades (Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas){
    	
    double suma, baseA = -1., baseB = -1.;    

    suma = 0.;

    // para los posibles ejes en el conjunto de candidatos calculamos la probabilidad de ser elegidos.
    
    for (map<CANDIDATE,double>::iterator it = this->probabilidades.begin(); it != this->probabilidades.end(); ++it)    
        (*it).second = 0.;
         
    // calculamos la probabilidad de los ejese puedan ser candidatos
    vector< CANDIDATE >::iterator it = candidatas.begin();
    
    while(it != candidatas.end()){
        Hormiga mas = *(this->hormigas[indice]);
    	switch (this->heuristicas[indice]) {
    		case 1:  // STATIC
    			baseA = this->hormigas[indice]->getAparicionesEje(*it);
    			baseB = 1;  			   
    			break;
    		case 2:  // DYNAMIC
#if VERSION == V_SHAPE
mas.avanza((*it).first,(*it).second);
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
mas.avanza((*it).first,(*it).second,(*it).third);
#endif                        

                        if (mas.getCoste(0) > 0)
    			    baseA = 1 - ((this->hormigas[indice]->getCoste(0) * 1.) - mas.getCoste(0));
                        else
                            baseA = 0;
//                         baseA = 0.01; /* epsilon */
//                         if (mas.getCoste(0) <= (this->hormigas[indice]->getCoste(0)))
//                                 baseA += (mas.getCoste(0) - (this->hormigas[indice]->getCoste(0) * 1.));
    			baseB = 1;	
    			break;
    		case 0:  // no se usan heuristicas, solo feromona en el MOACO
    			baseA = baseB = 1;
    			break;
    	}
//     	cout << (*it).first-1 << ' ' << (*it).second-1 << ' ' << (*it).third-1 << '=' << this->matricesFeromona[(*it).first-1][(*it).second-1][(*it).third-1] << endl;
    	this->probabilidades[(*it)] = this->matricesFeromona[(*it)] * pow(baseA, this->betaLambda[indice]) * pow(baseB, this->betaNoLambda[indice]); 
        
    	suma += this->probabilidades[(*it)];
        it++;
    }
            
    return suma;
}

//---------------------------------------------------------
double MACS::calculaProbabilidadesTransicion(Hormiga &unaHormiga, unsigned int indice, 
		vector< CANDIDATE > &candidatas){

    double suma;

    suma = this->calculaNumeradoresProbabilidades(unaHormiga, indice, candidatas);

    if (suma != 0.){

         for (map<CANDIDATE,double>::iterator it = this->probabilidades.begin(); it != this->probabilidades.end(); ++it) {  
        	(*it).second /= suma;         
//                 cout << (*it).second << endl;
         }
      
        return suma;
    }

    return 0.;
}

//---------------------------------------------------------
CANDIDATE MACS::transicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas){
    
#if VERSION == V_SHAPE
    CANDIDATE eleccion(0,"0");
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    CANDIDATE eleccion(0,0,0);
#endif

    double sumaNumeradores, probAleatoria;
    double *sumaProb;
    
//     cout << "Antes" << endl;
    sumaNumeradores = this->calculaProbabilidadesTransicion(unaHormiga, indice, candidatas);
//     cout << "Despues" << endl;
    
    probAleatoria = ((rand() * 1.) / (RAND_MAX * 1.));
        
    // todas las candidatas que quedan tienen probabilidad 0, por lo que elegimos buscando el maximo    
    
    if (sumaNumeradores == 0){
    	
    	// cogemos la primera de todas las candidatas    	
    	vector< CANDIDATE >::iterator it = candidatas.begin();   		
    	eleccion = *it;
//     	cout << "Suma0" << endl;
    	return eleccion;
    }
    
    // en el caso de que tengamos que elegir la de mayor probabilidad entre las tareas candidatas   
    
    if (probAleatoria < PARA.MOACO_q0) {
//         cout << "A" << endl;
        // si el numero aleatorio es menor que q0 elegimos el maximo de los numeradores, no hay aleatoriedad. 
    	vector< CANDIDATE >::iterator it = candidatas.begin();   		
    	eleccion = *it;
        it++;
        
        while(it != candidatas.end()){
//             cout << this->probabilidades[(*it)] << ' ';
            if (this->probabilidades[(*it)] > this->probabilidades[eleccion])
                eleccion = *it;
            it++;
        }
   		
    } else {
//     	cout << "B" << endl;
        // en el caso de que el numero aleatorio sea mayor que q0 elegimos aleatoriamente por ruleta segun las probabilidades de cada eje elegible
    	        
        // calculamos las probabilidades acumuladas para poder aplicar una seleccion de ejes por ruleta
    	sumaProb = new double[this->probabilidades.size()];        		        	
        map<CANDIDATE,double>::iterator it = this->probabilidades.begin();
        sumaProb[0] = (*it).second;
        unsigned int i = 1;        	
    	for (++it; it != this->probabilidades.end(); ++it) {
    		sumaProb[i] = sumaProb[i-1] + (*it).second;
                i++;
        }		        		
//     	    	cout << "@@@@@@@@" << sumaProb[i-1] << endl;
    	do 
            probAleatoria = ((rand() * 1.) / (RAND_MAX * 1.)); 
        while (probAleatoria == 0);
        
        // recorremos las probabilidades acumuladas de todas los ejes para ver el eje elegido aleatoriamente
        int el = 0;
        vector< CANDIDATE >::iterator it1 = candidatas.begin();
        eleccion = *it1;
        while((el < this->probabilidades.size()) && (probAleatoria > sumaProb[el]) && (it1 != candidatas.end())) {
        	el++;
                eleccion = *it1;
                it1++;
        }
//         cout << "@@@@@@@@@@@@" << el-1 << endl;
        delete []sumaProb;

    }

    return eleccion;
    
}

//---------------------------------------------------------
float MACS::nuevaTau0(){
    float *medias, retorno;
    int i, numElementos;
    float newTau0;

    if (this->conjuntoNoDominadas.begin() != this->conjuntoNoDominadas.end()) {
        medias = new float[this->nObj];
        numElementos = 0;
    
        for (i = 0; i < this->nObj; i++)
            medias[i] = 0.;
    
        NDominatedSet::iterator p = this->conjuntoNoDominadas.begin();
        
        while (p != conjuntoNoDominadas.end()){
            for (i = 0; i < this->nObj; i++)
                    medias[i] += p->getCoste(i);
    
            numElementos++;
            p++;
        }
    
        retorno = 1.;
    
        for (i = 0; i < this->nObj; i++){
            medias[i] /= numElementos;
            retorno *= medias[i];
        }
//         for (i = 0; i < this->nObj; i++)
//             cout << "M:" << medias[i] << endl;
//     
        // liberamos el vector creado
        delete []medias;
        newTau0 = retorno;
	cout << "Tau0 initial: " << newTau0 << ", type: computed" << endl;
    }
    else
    {
        newTau0 = (this->feromonaInicial);
	cout << "Tau0 initial: " << newTau0 << ", type: user-provided" << endl;
    }
    return newTau0;
}

//---------------------------------------------------------
void MACS::reinicializaMatriz(){

    for (map<CANDIDATE,float>::iterator i = matricesFeromona.begin(); i != matricesFeromona.end(); i++)
        i->second = this->feromonaInicial;
    
}

//---------------------------------------------------------
#if VERSION == V_SHAPE
void MACS::modificaFeromonaPasoAPaso(unsigned int x, const string y){
this->matricesFeromona[CANDIDATE(x,y)] = (1. - PARA.MOACO_ro) * this->matricesFeromona[CANDIDATE(x,y)] + PARA.MOACO_ro * this->feromonaInicial;
}
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
void MACS::modificaFeromonaPasoAPaso(unsigned int x, unsigned int y, unsigned int z){
    this->matricesFeromona[CANDIDATE(x,y,z)] = (1. - PARA.MOACO_ro) * this->matricesFeromona[CANDIDATE(x,y,z)] + PARA.MOACO_ro * this->feromonaInicial;
}
#endif

    

//---------------------------------------------------------
void MACS::modificaFeromona() {
    float t0;
    
    // pheromone global update
    NDominatedSet::iterator p = this->conjuntoNoDominadas.begin();
    
    while (p != conjuntoNoDominadas.end()){
        t0 = 0.;
        for (int j = 0; j < (unsigned int) this->nObj; j++)
            t0 *= (*p).getCoste(j);
        for (map<CANDIDATE,float>::iterator j = this->matricesFeromona.begin(); j != this->matricesFeromona.end(); j++) {   
            j->second *= (1. - PARA.MOACO_ro);
            j->second += (PARA.MOACO_ro * t0);
        }
                    
        p++;
    }
}

//---------------------------------------------------------
#if VERSION == V_SHAPE
void MACS::accionesTrasDecision (Hormiga *hormiga, unsigned int x, const string y) {

	this->modificaFeromonaPasoAPaso(x, y); 
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
void MACS::accionesTrasDecision (Hormiga *hormiga, unsigned int x, unsigned int y, unsigned int z) {

	this->modificaFeromonaPasoAPaso(x, y, z);       
#endif
      
	
}

//---------------------------------------------------------
void MACS::accionesFinalesHormiga () {
    float nuevoTau;
    
    // calcula el nuevo tau0 para modificar la feromona global segun los 
    // valores de los objetivos de las soluciones del Pareto

    nuevoTau = this->nuevaTau0();

    cout << "NT: " << nuevoTau << endl;
    cout << "FI: " << this->feromonaInicial << endl;
    
    if (nuevoTau > this->feromonaInicial){
        
            this->feromonaInicial = nuevoTau;
            this->reinicializaMatriz();
        
    } else {            
        
        // depositamos feromona en los pares (num_tarea,estacion_pertenece) de la matriz de feromona
            // asociadas a las soluciones del cto. de no dominadas, 
            // ya que seran las que proporcionan las mejores soluciones
            this->modificaFeromona();
    }    
 }
