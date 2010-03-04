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
#if (VERSION == V_SHAPE) || (VERSION == V_SCIENCEMAP)
    vector< CANDIDATE > lista = b[0].posibilidades_totales();
    for (vector< CANDIDATE >::iterator p = lista.begin(); p != lista.end(); p++) {   
        this->matricesFeromona[*p] = this->feromonaInicial; 
    }
#elif VERSION == V_GO
    for (set< CANDIDATE >::iterator j = b[0].base_ejes()->begin(); j != b[0].base_ejes()->end(); j++) {   
        this->matricesFeromona[*j] = this->feromonaInicial; 
    }
#endif

//     cout << "Feromonas: " << matricesFeromona.size() << ' ' << matricesFeromona.size() << ' ' << matricesFeromona[0][0].size() << endl;
}

//---------------------------------------------------------
double MACS::calculaNumeradoresProbabilidades (Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas) {
    	
    double suma, baseA = -1., baseB = -1.;    

    suma = 0.;

    // para los posibles ejes en el conjunto de candidatos calculamos la probabilidad de ser elegidos.
    
    for (map<CANDIDATE,double>::iterator it = this->probabilidades.begin(); it != this->probabilidades.end(); ++it)    
        (*it).second = 0.;
         
    // calculamos la probabilidad de los ejese puedan ser candidatos
    vector< CANDIDATE >::iterator it = candidatas.begin();
    
    while(it != candidatas.end()) {
	CANDIDATE n = *it;
        Hormiga mas = *(this->hormigas[indice]);
    	switch (this->heuristicas[indice]) {
    		case 1:  // STATIC
			#if VERSION == V_SHAPE
			  n.first = 1;
			#elif VERSION == V_SCIENCEMAP
			    // Search for the correct index for both nodes
			    n.first = this->hormigas[indice]->subEst().mapear(it->first);
			    n.second = this->hormigas[indice]->subEst().mapear(it->second);
			    if (this->probabilidades.find(n) == this->probabilidades.end()) {
			      n.second = this->hormigas[indice]->subEst().mapear(it->first);
			      n.first = this->hormigas[indice]->subEst().mapear(it->second);
			    }
			#endif
    			baseA = this->hormigas[indice]->getAparicionesEje(n);
    			baseB = 1;  			   
    			break;
    		case 2:  // DYNAMIC
			#if VERSION == V_SHAPE
			mas.avanza(n.first,n.second);
			#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
			mas.avanza(n.first,n.second,n.third);
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
#if VERSION == V_SHAPE
  n.first = 1;
#elif VERSION == V_SCIENCEMAP
  // Search for the correct index for both nodes
  n.first = this->hormigas[indice]->subEst().mapear(it->first);
  n.second = this->hormigas[indice]->subEst().mapear(it->second);
  if (this->probabilidades.find(n) == this->probabilidades.end()) {
    n.second = this->hormigas[indice]->subEst().mapear(it->first);
    n.first = this->hormigas[indice]->subEst().mapear(it->second);
  }
#endif
    	this->probabilidades[n] = this->matricesFeromona[n] * pow(baseA, this->betaLambda[indice]) * pow(baseB, this->betaNoLambda[indice]); 
    	suma += this->probabilidades[n];
        it++;
    }
            
    return suma;
}

//---------------------------------------------------------
double MACS::calculaProbabilidadesTransicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas) {

    double suma = this->calculaNumeradoresProbabilidades(unaHormiga, indice, candidatas);

    #if (VERSION == V_SHAPE) || (VERSION == V_SCIENCEMAP)
      map<CANDIDATE,bool> done;
    #endif
    
    if (suma > 0){
	 for (unsigned int i = 0; i < candidatas.size(); i++) {
		CANDIDATE n = candidatas[i];
		#if VERSION == V_SHAPE
		  n.first = 1;
		  if (done.find(n) == done.end()) {
		#elif VERSION == V_SCIENCEMAP
		   // Search for the correct index for both nodes
		  n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
		  n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		  if (this->probabilidades.find(n) == this->probabilidades.end()) {
		    n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
		    n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		  }
		  if (done.find(n) == done.end()) {
		#endif
        	this->probabilidades[n] /= suma;
		#if (VERSION == V_SHAPE) || (VERSION == V_SCIENCEMAP)
		    done[n] = true;
		  }
		#endif
	 }	
    }

    return suma;
}

//---------------------------------------------------------
CANDIDATE MACS::transicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas){
    
#if VERSION == V_SHAPE
    CANDIDATE eleccion(0,"0");
    cout << "Candidatos: " << endl;
    for (vector<CANDIDATE>::iterator it = candidatas.begin(); it != candidatas.end(); ++it)
      cout << it->first << ' ' << it->second << endl;
#elif (VERSION == V_GO) || (VERSION == V_SCIENCEMAP)
    CANDIDATE eleccion(0,0,0);
//     cout << "Candidatos: " << endl;
//     for (vector<CANDIDATE>::iterator it = candidatas.begin(); it != candidatas.end(); ++it)
//       cout << it->first << ' ' << it->second << endl;
#endif

    double sumaNumeradores, probAleatoria;
    double *sumaProb;
    
//     cout << "Antes" << endl;
    sumaNumeradores = this->calculaProbabilidadesTransicion(unaHormiga, indice, candidatas);
//     cout << "Despues" << endl;
    
    probAleatoria = ((rand() * 1.) / (RAND_MAX * 1.));
        
    // todas las candidatas que quedan tienen probabilidad 0, por lo que elegimos buscando el maximo    
    
    if (sumaNumeradores == 0) {
    	// cogemos la primera de todas las candidatas    	
	// Modificado ROCIO, elijo una al azar
	unsigned int x = intAzar(0, candidatas.size() - 1);
    	vector< CANDIDATE >::iterator it = candidatas.begin();
    	eleccion = *(it + x);
    	cout << "Suma0" << endl;
    }
    else {
      // en el caso de que tengamos que elegir la de mayor probabilidad entre las tareas candidatas   
      
      if (probAleatoria < PARA.MOACO_q0) {
	  cout << "A" << endl;
	  // Si hay varios con el mismo valor elijo uno al azar
	  vector<CANDIDATE> mejores;
	  // si el numero aleatorio es menor que q0 elegimos el maximo de los numeradores, no hay aleatoriedad. 
	  vector< CANDIDATE >::iterator it = candidatas.begin();  
	  mejores.push_back(*it);
	  eleccion = *it;
	  it++;
	  
	  while (it != candidatas.end()) {
	      CANDIDATE n = *it;
	      CANDIDATE m = eleccion;
	      #if VERSION == V_SHAPE
		n.first = 1;
		m.first = 1;
	      #elif VERSION == V_SCIENCEMAP
		  // Search for the correct index for both nodes
		  n.first = this->hormigas[indice]->subEst().mapear(it->first);
		  n.second = this->hormigas[indice]->subEst().mapear(it->second);
		  if (this->probabilidades.find(n) == this->probabilidades.end()) {
		    n.second = this->hormigas[indice]->subEst().mapear(it->first);
		    n.first = this->hormigas[indice]->subEst().mapear(it->second);
		  }
		  // Search for the correct index for both nodes
		  m.first = this->hormigas[indice]->subEst().mapear(eleccion.first);
		  m.second = this->hormigas[indice]->subEst().mapear(eleccion.second);
		  if (this->probabilidades.find(m) == this->probabilidades.end()) {
		    m.second = this->hormigas[indice]->subEst().mapear(eleccion.first);
		    m.first = this->hormigas[indice]->subEst().mapear(eleccion.second);
		  }
	      #endif
	      if (this->probabilidades[n] > this->probabilidades[m])
		mejores.push_back(*it);
	      it++;
	  }
	  unsigned int cual = intAzar(0, mejores.size()-1);
	  eleccion = mejores[cual];
		  
      } else {
	  cout << "B" << endl;
	  // en el caso de que el numero aleatorio sea mayor que q0 elegimos aleatoriamente por ruleta segun las probabilidades de cada eje elegible
		  
	  // calculamos las probabilidades acumuladas para poder aplicar una seleccion de ejes por ruleta
	  cout << "CC " <<  this->probabilidades.size() << endl;
	  sumaProb = new double[candidatas.size()];
	  CANDIDATE n = candidatas[0];
	  #if VERSION == V_SHAPE
	    n.first = 1;
	  #elif VERSION == V_SCIENCEMAP
	      // Search for the correct index for both nodes
	      n.first = this->hormigas[indice]->subEst().mapear(candidatas[0].first);
	      n.second = this->hormigas[indice]->subEst().mapear(candidatas[0].second);
	      if (this->probabilidades.find(n) == this->probabilidades.end()) {
		n.second = this->hormigas[indice]->subEst().mapear(candidatas[0].first);
		n.first = this->hormigas[indice]->subEst().mapear(candidatas[0].second);
	      }
	  #endif

	  sumaProb[0] = this->probabilidades[n];
	  cout << "@@@@0@@@@" << sumaProb[0] << endl;
	  for (unsigned int i = 1; i < candidatas.size(); i++) {
		  n = candidatas[i];
		  #if VERSION == V_SHAPE
		    n.first = 1;
		  #elif VERSION == V_SCIENCEMAP
		     // Search for the correct index for both nodes
		      n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
		      n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		      if (this->probabilidades.find(n) == this->probabilidades.end()) {
			n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
			n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		      }
		  #endif
		  sumaProb[i] = sumaProb[i-1] + this->probabilidades[n];
		  cout << "@@@@" << i << "@@@@" << this->probabilidades[n] << "@@@@" << sumaProb[i] << endl;
	  }	
		  
	  do 
	      probAleatoria = ((rand() * 1.) / (RAND_MAX * 1.)); 
	  while (probAleatoria == 0);
	  cout << "Entonces: " << probAleatoria << endl;
	  
	  // recorremos las probabilidades acumuladas de todas los ejes para ver el eje elegido aleatoriamente
	  int el = 0;
	  while((el < candidatas.size()) && (probAleatoria >= sumaProb[el])) {
  // 		cout << "A: " << el << endl;
		  el++;
	  }
	  cout << "@@@@@@@@@@@@" << el << endl;
	  eleccion = candidatas[el];
	  delete []sumaProb;

      }
    }
    
    return eleccion;
}

//---------------------------------------------------------
float MACS::nuevaTau0(){
    float *medias, retorno;
    int i, numElementos;
    float newTau0;

    if (this->conjuntoNoDominadas.size() > 0) {
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
