#include "macs.h"

//---------------------------------------------------------
MACS::MACS (vector<SOLUTION>& b, Parametros &params) : ACO (b, params) {
    vector <float> valoresLambda, valoresNoLambda;
    
    // inicializacion de umbrales y heuristicas para cada hormiga en un enfoque multiheuristics
    this->setUmbralesYHeuristicas(PARA.MOACO_multiheuristics);
    
    this->feromonaInicial = PARA.MOACO_tau0;
    
    for (unsigned int i = 0; i < PARA.MOACO_numHormigas; i++){
        valoresLambda.push_back(((float) i) / PARA.MOACO_numHormigas);
        valoresNoLambda.push_back(1. - valoresLambda[i]);
        this->betaLambda.push_back(PARA.MOACO_beta * valoresLambda[i]);  // en este vector se guarda el valor lambda*beta
        this->betaNoLambda.push_back(PARA.MOACO_beta * valoresNoLambda[i]);  // en este vector se guarda el valor (lambda-1)*beta
    }
    
    // la matriz de feromona seria una matriz NxNXl
    vector< CANDIDATE > lista = b[0].posibilidades_totales();
    for (vector< CANDIDATE >::iterator p = lista.begin(); p != lista.end(); p++) {   
        this->matricesFeromona[*p] = this->feromonaInicial; 
    }
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
			// Search for the correct index for both nodes
			cout << "Mapear " << it->first << endl;
			n.first = this->hormigas[indice]->subEst().mapear(it->first);
			cout << "Mapear " << it->second << endl;
			n.second = this->hormigas[indice]->subEst().mapear(it->second);
			cout << "EJE " << n.first << ' ' << n.second << ' ' << n.third << endl;

			if (this->probabilidades.find(n) == this->probabilidades.end()) {
			  cout << "IN" << endl;
			  n.second = this->hormigas[indice]->subEst().mapear(it->first);
			  n.first = this->hormigas[indice]->subEst().mapear(it->second);
			}
			cout << "EJE " << it->first << ' ' << it->second << ' ' << it->third << endl << this->hormigas[indice]->subEst() << endl;
			cout << "EJE " << n.first << ' ' << n.second << ' ' << n.third << endl;
    			baseA = this->hormigas[indice]->getAparicionesEje(n);
    			baseB = 1;
    			break;
    		case 2:  // DYNAMIC
			mas.avanza(n);

                        if (mas.getCoste(0) > 0)
    			    baseA = 1 - ((this->hormigas[indice]->getCoste(0) * 1.) - mas.getCoste(0));
                        else
                            baseA = 0;
   			baseB = 1;	
    			break;
    		case 0:  // no se usan heuristicas, solo feromona en el MOACO
    			baseA = baseB = 1;
    			break;
    	}
//     	cout << (*it).first-1 << ' ' << (*it).second-1 << ' ' << (*it).third-1 << '=' << this->matricesFeromona[(*it).first-1][(*it).second-1][(*it).third-1] << endl;

	// Search for the correct index for both nodes
	n.first = this->hormigas[indice]->subEst().mapear(it->first);
	n.second = this->hormigas[indice]->subEst().mapear(it->second);
	if (this->probabilidades.find(n) == this->probabilidades.end()) {
	  n.second = this->hormigas[indice]->subEst().mapear(it->first);
	  n.first = this->hormigas[indice]->subEst().mapear(it->second);
	}
    	this->probabilidades[n] = this->matricesFeromona[n] * pow(baseA, this->betaLambda[indice]) * pow(baseB, this->betaNoLambda[indice]); 
    	suma += this->probabilidades[n];
        it++;
    }
            
    return suma;
}

//---------------------------------------------------------
double MACS::calculaProbabilidadesTransicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas) {

    double suma = this->calculaNumeradoresProbabilidades(unaHormiga, indice, candidatas);

    map<CANDIDATE,bool> done;
    
    if (suma > 0){
	 for (unsigned int i = 0; i < candidatas.size(); i++) {
	    CANDIDATE n = candidatas[i];
	    
	    // Search for the correct index for both nodes
	    n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
	    n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
	    if (this->probabilidades.find(n) == this->probabilidades.end()) {
	      n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
	      n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
	    }
	    if (done.find(n) == done.end()) {
	      this->probabilidades[n] /= suma;
	      done[n] = true;
	    }
	 }	
    }

    return suma;
}

//---------------------------------------------------------
CANDIDATE MACS::transicion(Hormiga &unaHormiga, unsigned int indice, vector< CANDIDATE > &candidatas){
   CANDIDATE eleccion(0,0,0);

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
	 
	  // Search for the correct index for both nodes
	  n.first = this->hormigas[indice]->subEst().mapear(candidatas[0].first);
	  n.second = this->hormigas[indice]->subEst().mapear(candidatas[0].second);
	  if (this->probabilidades.find(n) == this->probabilidades.end()) {
	    n.second = this->hormigas[indice]->subEst().mapear(candidatas[0].first);
	    n.first = this->hormigas[indice]->subEst().mapear(candidatas[0].second);
	  }

	  sumaProb[0] = this->probabilidades[n];
	  cout << "@@@@0@@@@" << sumaProb[0] << endl;
	  for (unsigned int i = 1; i < candidatas.size(); i++) {
		  n = candidatas[i];
	      
		  // Search for the correct index for both nodes
		  n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
		  n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		  if (this->probabilidades.find(n) == this->probabilidades.end()) {
		    n.second = this->hormigas[indice]->subEst().mapear(candidatas[i].first);
		    n.first = this->hormigas[indice]->subEst().mapear(candidatas[i].second);
		  }
		  sumaProb[i] = sumaProb[i-1] + this->probabilidades[n];
		  cout << "@@@@" << i << "@@@@" << this->probabilidades[n] << "@@@@" << sumaProb[i] << endl;
	  }	
		  
	  do 
	      probAleatoria = ((rand() * 1.) / (RAND_MAX * 1.)); 
	  while (probAleatoria == 0);
	  cout << "Entonces: " << probAleatoria << endl;
	  
	  // recorremos las probabilidades acumuladas de todas los ejes para ver el eje elegido aleatoriamente
	  unsigned int el = 0;
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

void MACS::modificaFeromonaPasoAPaso(unsigned int x, unsigned int y, unsigned int z){
    this->matricesFeromona[CANDIDATE(x,y,z)] = (1. - PARA.MOACO_ro) * this->matricesFeromona[CANDIDATE(x,y,z)] + PARA.MOACO_ro * this->feromonaInicial;
}

//---------------------------------------------------------
void MACS::modificaFeromona() {
    float t0;
    
    // pheromone global update
    NDominatedSet::iterator p = this->conjuntoNoDominadas.begin();
    
    while (p != conjuntoNoDominadas.end()){
        t0 = 0.;
        for (unsigned int j = 0; j < (unsigned int) this->nObj; j++)
            t0 *= (*p).getCoste(j);
        for (map<CANDIDATE,float>::iterator j = this->matricesFeromona.begin(); j != this->matricesFeromona.end(); j++) {   
            j->second *= (1. - PARA.MOACO_ro);
            j->second += (PARA.MOACO_ro * t0);
        }
                    
        p++;
    }
}

//---------------------------------------------------------

void MACS::accionesTrasDecision (Hormiga *hormiga, unsigned int x, unsigned int y, unsigned int z) {
	this->modificaFeromonaPasoAPaso(x, y, z);
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
