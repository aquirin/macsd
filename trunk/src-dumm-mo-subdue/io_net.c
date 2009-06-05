/***
Functions for open/save .NET files.
***/


#include "io_net.h"

/***
Open .NET files (matrix, edges or arcs)
Used to open physical files.
IN: filename
OUT: num_nodos, mpesos[][]
***/
float** open_netfile(char* filename, int* nnodes)
{
  FILE *fich;
  float **mpesos;
  int i, j;
  char cadena[1024];
  
  fich = fopen(filename,"r");
  if(fich==NULL){
    fprintf(stderr, "\n[ERROR] - Cannot open the file %s !\n", filename);
    exit(2);
  }
  
  fscanf(fich, ENTRADA_1, nnodes);
  
  // Allocation of pesos_originales[][]
  mpesos = Matriz_dinamica_float(*nnodes, *nnodes);
  
  for(i=0; i<(*nnodes); i++)		//Inicializamos la matriz de pesos
  	for(j=0; j<(*nnodes); j++){
		mpesos[i][j] = 0.0;
		}
		
  // Drop all the labels
  for (i=0; i<(*nnodes)+1;i++){		//obviamos la informacion de cada nodo
  	fgets(cadena, 1024, fich);
	}
	
  // Read the file
  open_net(fich, *nnodes, mpesos);
  fclose(fich);
  
  return mpesos;
}

/***
Open .NET files (matrix, edges or arcs)
Used to open stdin also.
IN: fich, nnodes, mpesos[][]
OUT: mpesos[][]
***/
void open_net(FILE* fich, int nnodes, float** mpesos)
{
  int i, j;
  float aux;
  char cadena[256];
  
  fgets(cadena, 1024, fich);

    
  //Dependiendo del formato de entrada leemos de una forma u otra.
  if(strncmp(cadena,"*matrix",7)==0 || strncmp(cadena,"*Matrix",7)==0 || strncmp(cadena,"*MATRIX",7)==0)
  {
	for(i=0; i<nnodes; i++)
		for(j=0; j<nnodes; j++)
		{
			fscanf(fich,"%f",&aux);
			mpesos[i][j] = aux;
		}

  }
  else if(strncmp(cadena,"*edges",6)==0 || strncmp(cadena,"*Edges",6)==0 || strncmp(cadena,"*EDGES",6)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		mpesos[i-1][j-1] = aux;
		/* si el fichero de entrada solo tiene los ejes de la media matriz diagonal */
		mpesos[j-1][i-1] = aux;
  	}
  }
  else if(strncmp(cadena,"*arcs",5)==0 || strncmp(cadena,"*Arcs",5)==0 || strncmp(cadena,"*ARCS",5)==0)
  {
  	while( fscanf(fich, "%i %i %f", &i, &j, &aux) != EOF )
	{
		mpesos[i-1][j-1] = aux;
		
  	}
  }
  else
  {
	printf("[ERROR] - Formato del fichero de entrada no valido.\n");
	exit(3);
  }
  
  /*// Print pesos[][]
  printf("***** Matrix pesos_originales[][] :\n");
  for(i=0;i<num_nodos;i++)
  {
    for(j=0;j<num_nodos;j++)
    {
      printf(" %f", pesos_originales[i][j]);
    }
    printf("\n");
  }
  printf("*****\n");*/
	
}


/***
Return 1 if the matrix encoded in pesos_originales is symmetrical, 0 if not.
***/
int is_symmetrical(int nnodes, float** mpesos)
{
  int i, j;
  for(i=0; i<nnodes; i++)
    for(j=i+1; j<nnodes; j++)
    {
      if(mpesos[i][j] != mpesos[j][i])
        return 0;
    }
  return 1;
}




/***
Write .NET files (matrix, edges or arcs)
***/
void save_net(int nnodes, float** pfnet, FILE* fich, int type)
{
  int i, j;
 
  switch(type)
  {
  case OUTPUT_MATRIX:
    // Print the "*matrix" matrix
    fprintf(fich, "*matrix\n");
    for(i=0; i<nnodes; i++)
    {
    	for(j=0; j<nnodes; j++)
  	  fprintf(fich, "%" NDIGITS "f ",pfnet[i][j]);
        fprintf(fich, "\n");
    }
  break;
  case OUTPUT_EDGES:
    // Print the "*edges" matrix
    fprintf(fich, "*edges\n");
    for(i=0; i<nnodes; i++)
    	for(j=i+1; j<nnodes; j++)
    		if (pfnet[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);
  break;
  case OUTPUT_ARCS:
    // Print the "*arcs" matrix
    fprintf(fich, "*arcs\n");
    for(i=0; i<nnodes; i++)
    	for(j=0; j<nnodes; j++)
    		if (pfnet[i][j]!=FALSE)
  		    fprintf(fich, "%d %d %" NDIGITS "f\n",i+1,j+1,pfnet[i][j]);
  break;
  default:
	fprintf(stderr, "[ERROR] - Type for the output file not recognized.\n");
	exit(3);
  }


  }
