//---------------------------------------------------------------------------
// main.c
//
// SUBDUE 5
//---------------------------------------------------------------------------

#include <stdlib.h>
#include "subdue.h"
#include "time.h"
#include <unistd.h>
#include <sys/times.h>
#include "io_net.h"
#include "memory_allocation.h"
#include "global_define.h"

// If 1, print the statistic about each node of each instance.
int DEBUG=0;

#define ALLOC_INCR	100		// Number of blocks allocated in once


// Centrality matrices: used to encode the main matrices used by other algorithms, corresponding to a
// given graph.
typedef struct 
{
  int nnodes;				// Size of the tables
  float **pesos;			// table of the shortest weights
  float **predecessors;			// table of the predecessors
  int **dist_nodes;			// table of the size of the shortest path between two nodes
  int *sum_dist;			// sum_dist[i] = sum(j=1;j=n;dist_nodes(i,j)) [central <=> small]
  float *normalized_inv_sum_dist;	// same than sum_dist[], but normalized between 0 and 1 [central <=> 1.0] <= GOOD CENTRALITY MEASURE
  float graph_sum_dist;			// S(G) = somme(i=1;i=n;S(x_i))
  int *max_dist;			// max_dist[i] = max(j=1;j=n;dist_nodes(i,j)) [central <=> small]
  float *normalized_max_dist;		// same than max_dist[], but normalized between 0 and 1 [central <=> 0.0]
  int *num_connected;			// Number of connected links [central <=> large]
  float *normalized_num_connected;	// [central <=> 1.0]
  float *sum_connected;			// Sum of connected links [central <=> large]
  float *normalized_sum_connected;	// [central <=> 1.0]
} CentralityMatrices;


// GraphFile
// Related to 1 'big' file (domain graph)
typedef struct 
{
   char sub_name[1024];		// The filename of this graph (Subdue format)
   char net_name[1024];		// The filename of this graph (NET format)
   Graph *graph;		// The graph corresponding to the file (Subdue format)
   float** pesos;		// The original weights for this graph (from the NET file)
   int nnodes;			// Number of nodes
   CentralityMatrices* cm;	// Some useful matrices
   char type;			// Type of the instance (P=positive, N=negative)
} GraphFile;


/*
// SubgraphObject
Used to encode a subgraph, and all the properties which could be used in the MO
algorithm.
Related to 1 substructure, in relation with (included in) N domain graphs.
*/
typedef struct 
{
  // General
  Graph *graph;

  // coverage-based properties
  int cov_num_pos;		// Number of positive graphs containing this substructure
  int cov_num_neg;		// Number of negative graphs containing this substructure
  char* cov_filenames_pos[100];	// List of filenames of the positive graphs containing this substructure
  char* cov_filenames_neg[100];	// List of filenames of the negative graphs containing this substructure
  int pos_inst;		// Number of positive instances
  int neg_inst;		// Number of negative instances
  
  // complexity-based properties
  int n_nodes;		// Number of nodes
  int n_links;		// Number of links
  double v_mdl;		// MDL value returned by Subdue
  ULONG v_size;		// Size value returned by Subdue
  float v_value;	// Value used by subdue to order the substructures. This value is graph-dependant
  			// and parameter dependant (3 different value-measures can be used). So this value
			// is not very useful, expect to compare structures in a relative way.
			
  // position-based properties (graph-dependent, and the average for each node in the sub-structure)
  float sum_weights;			// Sum of all the weights of the links
  float sum_dist;			// Avg. of the sum of the dist. from one node to all other
  float normalized_inv_sum_dist;		// Same, but using the normalized 'sum_dist'
  float max_dist;			// Max. of the maximal dist. from one node to all other
  float normalized_max_dist;		// Max. of the normalized 'max_dist'
  float num_connected;			// Agv. of number of connected links
  float normalized_num_connected;	// Using the normalized 'num_connected'
  float sum_connected;			// Avg. of the sum of the weights of connected links
  float normalized_sum_connected;	// Using the normalized 'sum_connected'
  
  // only for the MOO algorithm
  int objv_size;				// Size of the objective vector
  float* objv;				// Objective vector (objectives have to be maximized)
  char** objn;				// Objective vector names
  
  // memory managment
  int num_inst;				// Number of instance (for SOList): when 0, the structure is physically deleted
} SubgraphObject;



// SOList
// List of 'SubgraphObject'
typedef struct 
{
  SubgraphObject** list;		// List
  int size;			// Number of filled blocks
  int alloc;			// Number of allocated blocks
} SOList;




// Function prototypes

int main(int, char **);
void ISubdue(Parameters *);
Parameters *GetParameters(int, char **);
void PrintParameters(Parameters *);
void FreeParameters(Parameters *);
Parameters *PostProcessParameters(Parameters *parameters);
SOList* BuildSOList(SubList *subList, Parameters *parameters, GraphFile* graphsDatabase);
void myPrintSubList(SubList *subList, Parameters *parameters, GraphFile* graphsDatabase);
void myPrintSub(Substructure *sub, Parameters *parameters, GraphFile* graphsDatabase);
GraphFile* LoadDatabase(char *filename, Parameters *parameters);
void FreeDatabase(GraphFile* database);
GraphFile LoadGraph(char typefile, char *sub_filename, char *net_filename, Parameters *parameters);
void FreeGraphFile(GraphFile gf);
Graph** GetCoverageSet(Graph* graph, GraphFile* database, Parameters *parameters);
void GraphToMatrixRepresentation(Graph* in_graph, float ***out_matrix, int *out_size);
CentralityMatrices* BuildCentralityMatrices(int num_nodos, float **pesos_originales);
void FreeCentralityMatrices(CentralityMatrices* cm);
void PrintInDOT_float(FILE* out, int num_nodos, float **adjacency, float *node_labels, float **link_labels);
SubgraphObject* ComputeSubgraphObject(Substructure* sub, Parameters *parameters, GraphFile* database);
void FreeSO(SubgraphObject* so);
void PrintSubgraphObject(SubgraphObject* so, Parameters *parameters);
void PrintSOL(SOList* L, Parameters *parameters);
void PrintSubgraphObjectInFile(FILE* _fin, SubgraphObject* so, Parameters *parameters);
void PrintSOLInFile(FILE* _fin, SOList* L, Parameters *parameters);
SOList* NewSOL(void);
void FreeSOL(SOList* L);
void AddSO(SOList* L, SubgraphObject* so);
void RemoveSO(SOList* L, int n);

int KungSortFirst(const void* _o1, const void* _o2);
int KungSort(SubgraphObject* o1, SubgraphObject* o2);
int KungCheckDomination(SubgraphObject* o1, SubgraphObject* o2, int n);
SOList* KungNonDominatedSet(SOList* P);
SOList* KungFront(SOList* P, Parameters *parameters);
double Jaccard(SubgraphObject* o1, SubgraphObject* o2);
double NegJaccard(SubgraphObject* o1, SubgraphObject* o2);

//---------------------------------------------------------------------------
// NAME:    main
//
// INPUTS:  (int argc) - number of arguments to program
//          (char **argv) - array of strings of arguments to program
//
// RETURN:  (int) - 0 if all is well
//
// PURPOSE: Main SUBDUE function that processes command-line arguments
//          and initiates discovery.
//---------------------------------------------------------------------------

int main(int argc, char **argv)
{
   struct tms tmsstart, tmsend;
   clock_t startTime, endTime;
   static long clktck = 0;
   time_t iterationStartTime;
   time_t iterationEndTime;
   SubList *subList;
   Parameters *parameters;
   FILE *outputFile;
   ULONG iteration;
   BOOLEAN done;
   GraphFile* graphsDatabase;

   clktck = sysconf(_SC_CLK_TCK);
   startTime = times(&tmsstart);
   printf("SUBDUE %s\n\n", SUBDUE_VERSION);
   parameters = GetParameters(argc, argv);

   // read the graphs database
   graphsDatabase = LoadDatabase(argv[argc - 1], parameters);
   
   if (parameters->incremental)
      ISubdue(parameters);
   else
   {
      // compress pos and neg graphs with predefined subs, if given
      if (parameters->numPreSubs > 0)
         CompressWithPredefinedSubs(parameters);

      PostProcessParameters(parameters);
      PrintParameters(parameters);

      if (parameters->iterations > 1)
         printf("----- Iteration 1 -----\n\n");

      iteration = 1;
      done = FALSE;
      while ((iteration <= parameters->iterations) && (!done))
      {
         iterationStartTime = time(NULL);
         if (iteration > 1)
            printf("----- Iteration %lu -----\n\n", iteration);

         printf("%lu positive graphs: %lu vertices, %lu edges",
                parameters->numPosEgs, parameters->posGraph->numVertices,
                parameters->posGraph->numEdges);

         if (parameters->evalMethod == EVAL_MDL)
            printf(", %.0f bits\n", parameters->posGraphDL);
         else
            printf("\n");
         if (parameters->negGraph != NULL) 
         {
            printf("%lu negative graphs: %lu vertices, %lu edges",
                   parameters->numNegEgs, parameters->negGraph->numVertices,
                   parameters->negGraph->numEdges);
            if (parameters->evalMethod == EVAL_MDL)
               printf(", %.0f bits\n", parameters->negGraphDL);
            else 
               printf("\n");
         }
         printf("%lu unique labels\n", parameters->labelList->numLabels);
         printf("\n");

         subList = DiscoverSubs(parameters);

         if (subList->head == NULL) 
         {
            done = TRUE;
            printf("No substructures found.\n\n");
         }
         else 
         {
            // write output to stdout
            if (parameters->outputLevel > 1) 
            {
               printf("\nBest %lu substructures:\n\n", CountSubs (subList));
               // *** Personal modification: hook of the print function
	       //myPrintSubList(subList, parameters, graphsDatabase);
	       
	       // *** Personal modification: convert 'subList' to 'SOList', full of useful properties
	       SOList* SOL = BuildSOList(subList, parameters, graphsDatabase);
	       //PrintSO(SOL, parameters);
	       
	       // *** Personal modification: launch the MO algorithm
	       SOList* SOL_front = KungFront(SOL, parameters);
	       
	       // *** Personal modification: print the results in a file
	       if (parameters->outputToFile) 
	       {
	          char filename[1000];
		  FILE *fin;
		  
	          // Print the original DB
		  sprintf(filename, "%s.dat", parameters->outFileName);
	          fin = fopen(filename, "w");
		  PrintSOLInFile(fin, SOL, parameters);
		  fclose(fin);
		  
		  // Print the pareto front
		  sprintf(filename, "%s.front", parameters->outFileName);
	          fin = fopen(filename, "w");
		  PrintSOLInFile(fin, SOL_front, parameters);
		  fclose(fin);

		 
	       }
	       
	       FreeSOL(SOL);
	       FreeSOL(SOL_front);
            } 
            else 
            {
               printf("\nBest substructure:\n\n");
               // *** Personal modification: hook of the print function
               myPrintSub(subList->head->sub, parameters, graphsDatabase);
            }

            // write machine-readable output to file, if given
            if (parameters->outputToFile) 
            {
               outputFile = fopen(parameters->outFileName, "a");
               if (outputFile == NULL) 
               {
                  printf("WARNING: unable to write to output file %s,",
                         parameters->outFileName);
                  printf("disabling\n");
                  parameters->outputToFile = FALSE;
               }
               WriteGraphToFile(outputFile, subList->head->sub->definition,
                                parameters->labelList, 0, 0, 
                                subList->head->sub->definition->numVertices, 
                                TRUE);
               fclose(outputFile);
            }

            if (iteration < parameters->iterations) 
            {                                    // Another iteration?
               if (parameters->evalMethod == EVAL_SETCOVER) 
               {
                  printf("Removing positive examples covered by");
                  printf(" best substructure.\n\n");
                  RemovePosEgsCovered(subList->head->sub, parameters);
               } 
               else 
                  CompressFinalGraphs(subList->head->sub, parameters, iteration,
                                      FALSE);

               // check for stopping condition
               // if set-covering, then no more positive examples
               // if MDL or size, then positive graph contains no edges
               if (parameters->evalMethod == EVAL_SETCOVER) 
               {
                  if (parameters->numPosEgs == 0) 
                  {
                     done = TRUE;
                     printf("Ending iterations - ");
                     printf("all positive examples covered.\n\n");
                  }
               } 
               else 
               {
                  if (parameters->posGraph->numEdges == 0) 
                  {
                     done = TRUE;
                     printf("Ending iterations - graph fully compressed.\n\n");
                  }
               }
            }
            if ((iteration == parameters->iterations) && (parameters->compress))
            {
               if (parameters->evalMethod == EVAL_SETCOVER)
                  WriteUpdatedGraphToFile(subList->head->sub, parameters);
               else 
                  WriteCompressedGraphToFile(subList->head->sub, parameters,
                                             iteration);
            }
         }
         FreeSubList(subList);
         if (parameters->iterations > 1) 
         {
            iterationEndTime = time(NULL);
            printf("Elapsed time for iteration %lu = %lu seconds.\n\n",
            iteration, (iterationEndTime - iterationStartTime));
         }
         iteration++;
      }
   }
 
   FreeDatabase(graphsDatabase);
   FreeParameters(parameters);
   endTime = times(&tmsend);
   printf("\nSUBDUE done (elapsed CPU time = %7.2f seconds).\n",
          (endTime - startTime) / (double) clktck);
   return 0;
}


//---------------------------------------------------------------------------
// NAME:   ISubdue 
//
// INPUTS: (Parameters *parameters)
//
// RETURN: void
//
// PURPOSE: Perform incremental discovery.
//---------------------------------------------------------------------------

void ISubdue(Parameters *parameters)
{
   FILE *outputFile;
   BOOLEAN done;
   BOOLEAN ignoreBoundary = FALSE;
   BOOLEAN newData = FALSE;
   ULONG incrementCount = 0;
   ULONG sizeOfPosIncrement;
   ULONG sizeOfNegIncrement;
   Increment *increment;
   InstanceList *boundaryInstances = NULL;
   SubList *localSubList = NULL;
   SubList *globalSubList = NULL;

   if (parameters->evalMethod == EVAL_SETCOVER)
      ignoreBoundary = TRUE;

   while(TRUE)
   {
      // Get next batch of data, either dependent or independent
      newData = GetNextIncrement(parameters);
      if (!newData)
         break;
 
      PostProcessParameters(parameters);
      PrintParameters(parameters);
 
      increment = GetCurrentIncrement(parameters);
      parameters->posGraphSize +=
         (increment->numPosVertices + increment->numPosEdges);
   
      if (parameters->evalMethod == EVAL_SETCOVER)
         SetIncrementNumExamples(parameters);
      else
      {
         // We have to set the size here instead of in evaluate, otherwise
         // it gets reset at each iteration to the compressed graph size
         sizeOfPosIncrement =
            IncrementSize(parameters, GetCurrentIncrementNum(parameters), POS);
         sizeOfNegIncrement =
            IncrementSize(parameters, GetCurrentIncrementNum(parameters), NEG);
      }

      printf("Increment #%lu: %lu positive vertices, %lu positive edges\n",
         incrementCount+1, increment->numPosVertices, increment->numPosEdges);
      printf("Accumulated Positive Graph Size: %lu vertices, %lu edges\n",
         parameters->posGraph->numVertices, parameters->posGraph->numEdges);
      if (parameters->negGraph != NULL)
      {
         printf("Increment #%lu: %lu negative vertices, %lu negative edges\n",
           incrementCount+1, increment->numNegVertices, increment->numNegEdges);
         printf("Accumulated Negative Graph Size: %lu vertices, %lu edges\n",
            parameters->negGraph->numVertices, parameters->negGraph->numEdges);
      }
   
      done = FALSE;
      printf("%lu unique labels\n", parameters->labelList->numLabels);
      printf("\n");

      // Return the n best subs for this increment
      localSubList = DiscoverSubs(parameters);
      if (localSubList->head == NULL)
      {
         printf("No local substructures found.\n\n");
         done = TRUE;
      }
      else
      {
         if (parameters->outputLevel > 1)
         {
            // print local subs
            printf("\n");
            printf("Best %lu local substructures ", CountSubs(localSubList));
            printf("before boundary processing:\n");
            PrintSubList(localSubList, parameters);
         }
                                        // Compress only supports EVAL_SETCOVER 
         if (parameters->compress)      // Compress using best and write to file
         {
            printf("Removing positive examples covered by");
            printf(" best substructure.\n\n");
            WriteUpdatedIncToFile(localSubList->head->sub, parameters);
         }

         // Store a copy of the local subs in the current increment data
         // we only keep the instance list for the current and
         // immediate predecessor increments
         StoreSubs(localSubList, parameters);

         // Compute the best overall subs for all increments,
         // before boundary evaluation
         // We do this so we have a more accurate list of best subs for the
         // boundary evaluation, although it is not technically necessary
         globalSubList = ComputeBestSubstructures(parameters, 0);
         if (!ignoreBoundary)
         {
            if (globalSubList != NULL)
            {
               boundaryInstances =
                  EvaluateBoundaryInstances(globalSubList, parameters);
               FreeSubList(globalSubList);
            } 

            if (parameters->outputLevel > 1)
            {
               if ((boundaryInstances != NULL) &&
                   (boundaryInstances->head != NULL))
               {
                  printf("Boundary instances found:\n");
                  PrintInstanceList(boundaryInstances, parameters->posGraph,
                                    parameters->labelList);
               }
               else 
                  printf("No Boundary Instances Were Found.\n");

               printf("\n");
               printf("Best %lu local substructures ",
                      CountSubs(increment->subList));
               printf("after boundary evaluation:\n");
               PrintStoredSubList(increment->subList, parameters);
            }

            // Recompute the globally best subs after the boundary instances
            // have been recovered
            globalSubList = ComputeBestSubstructures(parameters, 0);
         }
         printf("\nGlobally Best Substructures - Final:\n");
         PrintStoredSubList(globalSubList, parameters);
         FreeSubList(globalSubList);
      }

      // write machine-readable output to file, if given
      if (parameters->outputToFile)
      {
         outputFile = fopen(parameters->outFileName, "a");
         if (outputFile == NULL)
         {
            printf("WARNING: unable to write to output file %s, disabling\n",
                   parameters->outFileName);
            parameters->outputToFile = FALSE;
         }
         if (parameters->evalMethod == EVAL_SETCOVER)
	 {
            printf("Removing positive examples covered by");
            printf(" best substructure.\n\n");
            RemovePosEgsCovered(localSubList->head->sub, parameters);
            WriteGraphToFile(outputFile, localSubList->head->sub->definition,
                             parameters->labelList, 0, 0, 
                             localSubList->head->sub->definition->numVertices,
                             TRUE);
         }
	 else 
            WriteGraphToFile(outputFile, localSubList->head->sub->definition,
                             parameters->labelList, 0, 0, 
                             localSubList->head->sub->definition->numVertices,
                             TRUE);
         fclose(outputFile);
      }
      incrementCount++;
   }
}


//---------------------------------------------------------------------------
// NAME: GetParameters
//
// INPUTS: (int argc) - number of command-line arguments
//         (char *argv[]) - array of command-line argument strings
//
// RETURN: (Parameters *)
//
// PURPOSE: Initialize parameters structure and process command-line
//          options.
//---------------------------------------------------------------------------

Parameters *GetParameters(int argc, char *argv[])
{
   Parameters *parameters;
   int i;
   double doubleArg;
   ULONG ulongArg;
   BOOLEAN limitSet = FALSE;
   FILE *outputFile;

   parameters = (Parameters *) malloc(sizeof(Parameters));
   if (parameters == NULL)
      OutOfMemoryError("parameters");

   // initialize default parameter settings
   parameters->directed = TRUE;
   parameters->limit = 0;
   parameters->numBestSubs = 3;
   parameters->beamWidth = 4;
   parameters->valueBased = FALSE;
   parameters->prune = FALSE;
   strcpy(parameters->outFileName, "none");
   parameters->outputToFile = FALSE;
   parameters->outputLevel = 2;
   parameters->allowInstanceOverlap = FALSE;
   parameters->threshold = 0.0;
   parameters->evalMethod = EVAL_MDL;
   parameters->iterations = 1;
   strcpy(parameters->psInputFileName, "none");
   parameters->predefinedSubs = FALSE;
   parameters->minVertices = 1;
   parameters->maxVertices = 0; // i.e., infinity
   parameters->recursion = FALSE;
   parameters->variables = FALSE;
   parameters->relations = FALSE;
   parameters->incremental = FALSE;
   parameters->compress = FALSE;

   if (argc < 2)
   {
      fprintf(stderr, "input graph file name must be supplied\n");
      exit(1);
   }

   // process command-line options
   i = 1;
   while (i < (argc - 2))
   {
      if (strcmp(argv[i], "-beam") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: beam must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->beamWidth = ulongArg;
      }
      else if (strcmp(argv[i], "-compress") == 0)
      {
         parameters->compress = TRUE;
      }
      else if (strcmp(argv[i], "-eval") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if ((ulongArg < 1) || (ulongArg > 3)) 
         {
            fprintf(stderr, "%s: eval must be 1-3\n", argv[0]);
            exit(1);
         }
         parameters->evalMethod = ulongArg;
      } 
      else if (strcmp(argv[i], "-inc") == 0)
      {
         parameters->incremental = TRUE;
      }
      else if (strcmp(argv[i], "-iterations") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         parameters->iterations = ulongArg;
      } 
      else if (strcmp(argv[i], "-limit") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: limit must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->limit = ulongArg;
         limitSet = TRUE;
      }
      else if (strcmp(argv[i], "-maxsize") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: maxsize must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->maxVertices = ulongArg;
      }
      else if (strcmp(argv[i], "-minsize") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: minsize must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->minVertices = ulongArg;
      }
      else if (strcmp(argv[i], "-nsubs") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if (ulongArg == 0) 
         {
            fprintf(stderr, "%s: nsubs must be greater than zero\n", argv[0]);
            exit(1);
         }
         parameters->numBestSubs = ulongArg;
      }
      else if (strcmp(argv[i], "-out") == 0) 
      {
         i++;
         strcpy(parameters->outFileName, argv[i]);
         parameters->outputToFile = TRUE;
      }
      else if (strcmp(argv[i], "-output") == 0) 
      {
         i++;
         sscanf(argv[i], "%lu", &ulongArg);
         if ((ulongArg < 1) || (ulongArg > 5)) 
         {
            fprintf(stderr, "%s: output must be 1-5\n", argv[0]);
            exit(1);
         }
         parameters->outputLevel = ulongArg;
      }
      else if (strcmp(argv[i], "-overlap") == 0) 
      {
         parameters->allowInstanceOverlap = TRUE;
      }
      else if (strcmp(argv[i], "-prune") == 0) 
      {
         parameters->prune = TRUE;
      }
      else if (strcmp(argv[i], "-ps") == 0) 
      {
         i++;
         strcpy(parameters->psInputFileName, argv[i]);
         parameters->predefinedSubs = TRUE;
      }
      else if (strcmp(argv[i], "-recursion") == 0) 
      {
         parameters->recursion = TRUE;
      }
      else if (strcmp(argv[i], "-relations") == 0) 
      {
         parameters->relations = TRUE;
         parameters->variables = TRUE; // relations must involve variables
      }
      else if (strcmp(argv[i], "-threshold") == 0) 
      {
         i++;
         sscanf(argv[i], "%lf", &doubleArg);
         if ((doubleArg < (double) 0.0) || (doubleArg > (double) 1.0))
         {
            fprintf(stderr, "%s: threshold must be 0.0-1.0\n", argv[0]);
            exit(1);
         }
         parameters->threshold = doubleArg;
      }
      else if (strcmp(argv[i], "-undirected") == 0) 
      {
         parameters->directed = FALSE;
      }
      else if (strcmp(argv[i], "-valuebased") == 0) 
      {
         parameters->valueBased = TRUE;
      }
      else if (strcmp(argv[i], "-variables") == 0) 
      {
         parameters->variables = TRUE;
      }
      else 
      {
         fprintf(stderr, "%s: unknown option %s\n", argv[0], argv[i]);
         exit(1);
      }
      i++;
   }

   if (parameters->iterations == 0)
      parameters->iterations = MAX_UNSIGNED_LONG; // infinity

   // initialize log2Factorial[0..1]
   parameters->log2Factorial = (double *) malloc(2 * sizeof(double));
   if (parameters->log2Factorial == NULL)
      OutOfMemoryError("GetParameters:parameters->log2Factorial");
   parameters->log2FactorialSize = 2;
   parameters->log2Factorial[0] = 0; // lg(0!)
   parameters->log2Factorial[1] = 0; // lg(1!)

   // initialise the other parameters
   parameters->labelList = AllocateLabelList();
   parameters->posGraph = NULL;
   parameters->negGraph = NULL;
   parameters->numPosEgs = 0;
   parameters->numNegEgs = 0;
   parameters->posEgsVertexIndices = NULL;
   parameters->negEgsVertexIndices = NULL;
   
   // read graphs from input file
   strcpy(parameters->inputFileName, argv[argc - 2]);
   
   if (parameters->incremental)
   {
      if (parameters->predefinedSubs)
      {
         fprintf(stderr, "Cannot process predefined examples incrementally");
         exit(1);
      }
  
      if (parameters->evalMethod == EVAL_MDL)
      {
         fprintf(stderr, "Incremental SUBDUE does not support EVAL_MDL, ");
         fprintf(stderr, "switching to EVAL_SIZE\n");
         parameters->evalMethod = EVAL_SIZE;
      }

      if ((parameters->evalMethod == EVAL_SIZE) && (parameters->compress))
      {
         fprintf(stderr, "Incremental SUBDUE does not support compression, ");
         fprintf(stderr, "with EVAL_SIZE, turning compression off\n");
         parameters->compress = FALSE;
      }

      if (parameters->iterations > 1)
      {
         fprintf(stderr,
            "Incremental SUBDUE only one iteration, setting to 1\n");
         parameters->iterations = 1;
      }
   }
   else 
   {
      ReadInputFile(parameters);
      if (parameters->evalMethod == EVAL_MDL)
      {
         parameters->posGraphDL = MDL(parameters->posGraph,
                                     parameters->labelList->numLabels, parameters);
         if (parameters->negGraph != NULL)
         {
            parameters->negGraphDL =
               MDL(parameters->negGraph, parameters->labelList->numLabels,
                   parameters);
         }
      }
   }

   // read predefined substructures
   parameters->numPreSubs = 0;
   if (parameters->predefinedSubs)
      ReadPredefinedSubsFile(parameters);

   parameters->incrementList = malloc(sizeof(IncrementList));
   parameters->incrementList->head = NULL;

   if (parameters->incremental)
   {
      parameters->vertexList = malloc(sizeof(InstanceVertexList));
      parameters->vertexList->avlTreeList = malloc(sizeof(AvlTreeList));
      parameters->vertexList->avlTreeList->head = NULL;
   }

   // create output file, if given
   if (parameters->outputToFile) 
   {
      outputFile = fopen(parameters->outFileName, "w");
      if (outputFile == NULL) 
      {
         printf("ERROR: unable to write to output file %s\n",
                parameters->outFileName);
         exit(1);
      }
      fclose(outputFile);
   }  

   return parameters;
}


//---------------------------------------------------------------------------
// NAME: PostProcessParameters
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Initialize parts of the parameters structure.  This must be called
//          after graph data is obtained.
//
// NOTE:    This code was separated from the other parametric processing
//          because of the need to handle some parameters differentely when
//          the incremental version of SUBDUE is executed.
//---------------------------------------------------------------------------
    
Parameters *PostProcessParameters(Parameters *parameters)
{
   Increment *increment = NULL;

   if (parameters->incremental)
      increment = GetCurrentIncrement(parameters);

   // Code from this point until end of function was moved from GetParameters
   if (parameters->numPosEgs == 0)
   {
      fprintf(stderr, "ERROR: no positive graphs defined\n");
      exit(1);
   }

   // Check bounds on discovered substructures' number of vertices
   if (parameters->maxVertices == 0)
      parameters->maxVertices = parameters->posGraph->numVertices;
   if (parameters->maxVertices < parameters->minVertices)
   {
      fprintf(stderr, "ERROR: minsize exceeds maxsize\n");
      exit(1);
   }

   // Set limit accordingly
   if (parameters->limit == 0)
   {
      if (parameters->incremental)
         parameters->limit = increment->numPosEdges / 2;
      else 
         parameters->limit = parameters->posGraph->numEdges / 2;
   }

   return parameters;
}


//---------------------------------------------------------------------------
// NAME: PrintParameters
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Print selected parameters.
//---------------------------------------------------------------------------

void PrintParameters(Parameters *parameters)
{
   printf("Parameters:\n");
   printf("  Input file..................... %s\n",parameters->inputFileName);
   printf("  Predefined substructure file... %s\n",parameters->psInputFileName);
   printf("  Output file.................... %s\n",parameters->outFileName);
   printf("  Beam width..................... %lu\n",parameters->beamWidth);
   printf("  Compress....................... ");
   PrintBoolean(parameters->compress);
   printf("  Evaluation method.............. ");
   switch(parameters->evalMethod) 
   {
      case 1: printf("MDL\n"); break;
      case 2: printf("size\n"); break;
      case 3: printf("setcover\n"); break;
   }
   printf("  'e' edges directed............. ");
   PrintBoolean(parameters->directed);
   printf("  Incremental.................... ");
   PrintBoolean(parameters->incremental);
   printf("  Iterations..................... ");
   if (parameters->iterations == 0)
      printf("infinite\n");
   else 
      printf("%lu\n", parameters->iterations);
   printf("  Limit.......................... %lu\n", parameters->limit);
   printf("  Minimum size of substructures.. %lu\n", parameters->minVertices);
   printf("  Maximum size of substructures.. %lu\n", parameters->maxVertices);
   printf("  Number of best substructures... %lu\n", parameters->numBestSubs);
   printf("  Output level................... %lu\n", parameters->outputLevel);
   printf("  Allow overlapping instances.... ");
   PrintBoolean(parameters->allowInstanceOverlap);
   printf("  Prune.......................... ");
   PrintBoolean(parameters->prune);
   printf("  Threshold...................... %lf\n", parameters->threshold);
   printf("  Value-based queue.............. ");
   PrintBoolean(parameters->valueBased);
   printf("  Recursion...................... ");
   PrintBoolean(parameters->recursion);
   printf("\n");

   printf("Read %lu total positive graphs\n", parameters->numPosEgs);
   if (parameters->numNegEgs > 0) 
      printf("Read %lu total negative graphs\n", parameters->numNegEgs);
   if (parameters->numPreSubs > 0)
      printf("Read %lu predefined substructures\n", parameters->numPreSubs);
   printf("\n");
}


//---------------------------------------------------------------------------
// NAME: ameters
//
// INPUTS: (Parameters *parameters)
//
// RETURN: (void)
//
// PURPOSE: Free memory allocated for parameters.  Note that the
//          predefined substructures are de-allocated as soon as they are
//          processed, and not here.
//---------------------------------------------------------------------------

void FreeParameters(Parameters *parameters)
{
   FreeGraph(parameters->posGraph);
   FreeGraph(parameters->negGraph);
   FreeLabelList(parameters->labelList);
   free(parameters->posEgsVertexIndices);
   free(parameters->negEgsVertexIndices);
   free(parameters->log2Factorial);
   free(parameters->incrementList);
   free(parameters);
}


//---------------------------------------------------------------------------
// NAME: BuildSOList : convert a Subdue's original 'SubList' object into a
//       'SOList' object.
//
// INPUTS: 
//
// RETURN:  
//
// PURPOSE: 
//---------------------------------------------------------------------------

SOList* BuildSOList(SubList *subList, Parameters *parameters, GraphFile* graphsDatabase)
{
   // parameters used
   ULONG counter = 1;
   SubListNode *subListNode = NULL;
   SOList *SOL = NewSOL();

   if (subList != NULL) 
   {
      subListNode = subList->head;
      while (subListNode != NULL) 
      {
         counter++;
         Substructure *sub = subListNode->sub;
         if (sub != NULL) 
         {
           // *** Personal modification: compute/update all the properties for this substructure
	   // *** and add it to the list
           SubgraphObject* so = ComputeSubgraphObject(sub, parameters, graphsDatabase);
           AddSO(SOL, so);		// Add the substructure+properties to a global list
         }
         subListNode = subListNode->next;
      }
   }
   
   return SOL;
}




//---------------------------------------------------------------------------
// NAME: PrintSubList
//
// INPUTS: (SubList *subList) - list of substructures to print
//         (Parameters *parameters)
//
// RETURN:  void
//
// PURPOSE: Print given list of substructures.
//---------------------------------------------------------------------------

void myPrintSubList(SubList *subList, Parameters *parameters, GraphFile* graphsDatabase)
{
   ULONG counter = 1;
   SubListNode *subListNode = NULL;

   if (subList != NULL) 
   {
      subListNode = subList->head;
      while (subListNode != NULL) 
      {
         printf("(%lu) ", counter);
         counter++;
         myPrintSub(subListNode->sub, parameters, graphsDatabase);
         printf("\n");
         subListNode = subListNode->next;
      }
   }
}

//---------------------------------------------------------------------------
// NAME: PrintSub
//
// INPUTS: (Substructure *sub) - substructure to print
//         (Parameters *parameters) - parameters
//
// RETURN: void
//
// PURPOSE: Print given substructure's value, number of instances,
// definition, and possibly the instances.
//---------------------------------------------------------------------------

void myPrintSub(Substructure *sub, Parameters *parameters, GraphFile* graphsDatabase)
{
   // parameters used
   LabelList *labelList = parameters->labelList;
   ULONG outputLevel = parameters->outputLevel;
   //Graph *graph = sub->definition;

   if (sub != NULL) 
   {
      printf("Substructure: value = %.*g", NUMERIC_OUTPUT_PRECISION,
             sub->value);
      // print instance/example statistics if output level high enough
      if (outputLevel > 2) 
      {
         printf("\n                  pos instances = %lu",sub->numInstances);
         if (parameters->incremental)
            printf(", pos examples = %lu\n",(ULONG) sub->posIncrementValue);
         else
            printf(", pos examples = %lu\n",sub->numExamples);

         printf("\n                  neg instances = %lu",sub->numNegInstances);
         if (parameters->incremental)
            printf(", neg examples = %lu\n",(ULONG) sub->negIncrementValue);
         else
            printf(", neg examples = %lu\n",sub->numNegExamples);
      } 
      else 
      {
         if ((parameters->incremental) && (parameters->evalMethod == EVAL_SETCOVER))
            printf(", pos examples = %lu, neg examples = %lu\n",
               (ULONG) sub->posIncrementValue, (ULONG) sub->negIncrementValue);
         else
            printf(", pos instances = %lu, neg instances = %lu\n",
                   sub->numInstances, sub->numNegInstances);
      }
      
      // *** Personal modification: compute/update all the properties for this substructure
      SubgraphObject* so = ComputeSubgraphObject(sub, parameters, graphsDatabase);
      PrintSubgraphObject(so, parameters);
      
      // *** Personal modification: print other kind of statistics.
      // print MDL value
      //printf("size = %lu, mdl = %.*g\n", so->v_size, NUMERIC_OUTPUT_PRECISION, so->v_mdl);
      
      // print coverage statistics
      //GetCoverageSet(graph, graphsDatabase, parameters);
      
      // print subgraph
      //if (sub->definition != NULL) 
      //{
      //   PrintGraph(sub->definition, labelList);
      //}
      if (sub->recursive) 
      {
         printf("    re ");
         PrintLabel(sub->recursiveEdgeLabel, labelList);
         printf("\n");
      }
      // print instances if output level high enough
      if (outputLevel > 2) 
      {
         printf("\n  Positive instances:\n");
         PrintPosInstanceList(sub, parameters);
         if (sub->numNegInstances > 0) 
         {
            printf("\n  Negative instances:\n");
            PrintNegInstanceList(sub, parameters);
         }
      }
   }
}

//---------------------------------------------------------------------------
// NAME: LoadDatabase
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Load all the graphs listed in the file 'filename'. The format of
//    this file for each line is: <P/N> <file in Subdue's format> <file in NET's format>
//    and return the table of graphs.
//    'P' or 'N' is used to indicate a positive or a negative example.
//---------------------------------------------------------------------------

GraphFile* LoadDatabase(char *filename, Parameters *parameters)
{
  FILE* fin;
  char twofilenames[1024], subfilename[1024], netfilename[1024], typefile;
  int i=0, num=0;
  GraphFile* database;
  
  // Open the list of filenames
  if((fin=fopen(filename, "r")) == NULL)
  {
    fprintf(stderr, "Unable to open '%s' !\n", filename);
    exit(-1);
  }
  
  // Get the number of filenames
  while(fgets(twofilenames, 1024, fin) != NULL)
    num++;

  // Allocate the memory for 'Num' graphs
  if((database=(GraphFile*)calloc(num+1,sizeof(GraphFile)))==NULL)
  {
    fprintf(stderr, "Unable to allocate the memory in LoadGraphs !\n");
    exit(-1);
  }
  rewind(fin);
  
  // Read the list of graph filenames
  while(fgets(twofilenames, 1024, fin) != NULL)
  {
    char* token, *seps=" \r\n";
    token = strtok(twofilenames, seps);
    typefile = token[0];
    token = strtok(NULL, seps);
    strcpy(subfilename, token);
    token = strtok(NULL, seps);
    strcpy(netfilename, token);
    
    printf("Loading files %s and %s (%s instance)...\n", subfilename, netfilename, (typefile=='P')?"positive":"negative");
    database[i] = LoadGraph(typefile, subfilename, netfilename, parameters);
    i++;
   }
  database[i].graph = NULL;
  fclose(fin);
  return database;
}

//---------------------------------------------------------------------------
// NAME: FreeDatabase
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE:
//---------------------------------------------------------------------------

void FreeDatabase(GraphFile* database)
{
	int i=0;
	while(database[i].graph!=NULL)
	{
		FreeGraphFile(database[i]);
		i++;
	}
	free(database);
}

//---------------------------------------------------------------------------
// NAME: LoadGraph
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Load one the graphs listed in the file 'filename'. The format of
//    this file for each line is: <file in Subdue's format> <file in NET's format>
//    and return the table of graphs.
//---------------------------------------------------------------------------

GraphFile LoadGraph(char typefile, char *sub_filename, char *net_filename, Parameters *parameters)
{
  GraphFile gf;
  int nnodes=0;
  
  strcpy(gf.sub_name, sub_filename);
  strcpy(gf.net_name, net_filename);
  gf.graph = ReadGraph(sub_filename, parameters->labelList, parameters->directed);
  gf.pesos = open_netfile(net_filename, &nnodes);
  gf.nnodes = nnodes;
  gf.cm = BuildCentralityMatrices(nnodes, gf.pesos);
  gf.type = typefile;

  // DEBUG
  /*if(strstr(sub_filename, "ancia")!=NULL)
  {
     printf("==============BEGIN================\n");
     char buffer[1024];
     sprintf(buffer, "%s.dot", sub_filename);
     FILE* fcurrent;		// Current file to be written (DEBUG only)
     fcurrent = fopen(buffer, "wt");
     float** matrix;
     int size;
     //GraphToMatrixRepresentation(database[i].graph, &matrix, &size);
     matrix = gf.pesos;
     size = gf.nnodes;
     // Print one of the centrality matrices in a file
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, VectorInt2Float(gf.nnodes, gf.cm->sum_dist), NULL);
     PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, gf.cm->normalized_inv_sum_dist, NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, VectorInt2Float(gf.nnodes, gf.cm->max_dist), NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, gf.cm->normalized_max_dist, NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, VectorInt2Float(gf.nnodes, gf.cm->num_connected), NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, gf.cm->normalized_num_connected, NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, gf.cm->sum_connected, NULL);
     //PrintInDOT_float(fcurrent, gf.nnodes, gf.pesos, gf.cm->normalized_sum_connected, NULL);

     fclose(fcurrent);
     printf("==============END================\n");
  }*/

  return gf;
}

//---------------------------------------------------------------------------
// NAME: FreeGraphFile
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Load one the graphs listed in the file 'filename'. The format of
//    this file for each line is: <file in Subdue's format> <file in NET's format>
//    and return the table of graphs.
//---------------------------------------------------------------------------

void FreeGraphFile(GraphFile gf)
{
	Libera_matriz_dinamica_float(gf.pesos, gf.nnodes);
	FreeCentralityMatrices(gf.cm);
	FreeGraph(gf.graph);
}


//---------------------------------------------------------------------------
// NAME: GetCoverageSet
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: For a DB of graphs, and a graph, compute the coverage set: the
// list of files containing the graph, and the number of files found.
//---------------------------------------------------------------------------

Graph** GetCoverageSet(Graph* graph, GraphFile* database, Parameters *parameters)
{
  int i=0, n=0;
  while(database[i].graph != NULL)
  {
    InstanceList *instanceList;
    ULONG numInstances;
    instanceList = FindInstances(graph, database[i].graph, parameters);
    numInstances = CountInstances(instanceList);
    FreeInstanceList(instanceList);
    if(numInstances>0)
    {
      printf("The graph has been found in the graph %s.\n", database[i].sub_name);
      n++;
    }
    i++;
  }
  printf("Coverage size: %d\n\n", n);
  
  return NULL;
}


//---------------------------------------------------------------------------
// NAME: GetCentralityMeasures
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: For a DB of graphs, and a graph, compute the coverage set: the
// list of files containing the graph, and the number of files found.
//---------------------------------------------------------------------------

/*Graph** GetCentralityMeasures(Graph* graph, int type)
{*/



//---------------------------------------------------------------------------
// NAME: GraphToMatrixRepresentation
//
// INPUTS: Graph* in_graph
//
// RETURN: float **out_matrix, int *out_size
//
// PURPOSE: Convert the internal structure of a Subdue's graph to an adjacency
// matrix.
//---------------------------------------------------------------------------

void GraphToMatrixRepresentation(Graph* in_graph, float ***out_matrix, int *out_size)
{
  *out_size = in_graph->numVertices;
  (*out_matrix) = Matriz_dinamica_float(*out_size, *out_size);
  ULONG e;
  Edge *edge;

  // scan edges
  for (e = 0; e < in_graph->numEdges; e++) 
  {
    edge = & in_graph->edges[e];
    (*out_matrix)[edge->vertex1][edge->vertex2] = 1;
    if(!edge->directed)
      (*out_matrix)[edge->vertex2][edge->vertex1] = 1;
  }
  
  //Libera_matriz_dinamica_float(matrix, size);
}


//---------------------------------------------------------------------------
// NAME: BuildCentralityMatrices
//
// INPUTS: 
//    int num_nodos : number of nodes in the graph
//    float **pesos_originales : the matrix of original weights
//
// RETURN: 
//
// PURPOSE: Build all the mandatory matrices to compute the centrality measures.
// This function use the Floyd-Warshall algorithm to compute the shortest path
// between each node, so this can be used further in the function.
//---------------------------------------------------------------------------

CentralityMatrices* BuildCentralityMatrices(int num_nodos, float **pesos_originales)
{
  int i, j, k, s, n;
  float min, max;
  CentralityMatrices *cm;
  
  cm = (CentralityMatrices *) malloc(sizeof(CentralityMatrices));
  if(cm == NULL)
  {
    fprintf(stderr, "[ERROR] Cannot allocate memory for CentralityMatrices!\n");
    exit(3);
  }
  cm->nnodes = num_nodos;
  
  // Allocation of the main matrices
  cm->pesos = Matriz_dinamica_float(num_nodos, num_nodos);
  cm->predecessors = Matriz_dinamica_float(num_nodos, num_nodos);
  cm->dist_nodes = Matriz_dinamica_int(num_nodos, num_nodos);
  cm->sum_dist = Vector_dinamica_int(num_nodos);
  cm->normalized_inv_sum_dist = Vector_dinamica_float(num_nodos); 
  cm->max_dist = Vector_dinamica_int(num_nodos);
  cm->normalized_max_dist = Vector_dinamica_float(num_nodos); 
  cm->num_connected = Vector_dinamica_int(num_nodos);
  cm->normalized_num_connected = Vector_dinamica_float(num_nodos); 
  cm->sum_connected = Vector_dinamica_float(num_nodos);
  cm->normalized_sum_connected = Vector_dinamica_float(num_nodos); 
  
  // Initialisation of pesos[][]
  for(i=0; i<num_nodos; i++)
    for(j=0; j<num_nodos; j++){
      if(pesos_originales[i][j] == 0)
        cm->pesos[i][j] = INFINITY;
      else
        cm->pesos[i][j] = pesos_originales[i][j];
    }
 
  // Initialisation of predecessors[][]
  for(i=0; i<num_nodos; i++)
    for(j=0; j<num_nodos; j++){
      if(i==j || cm->pesos[i][j]==INFINITY)
        cm->predecessors[i][j] = INFINITY;
      else
        cm->predecessors[i][j] = i;
    }

  // Original Floyd-Warshall algorithm
  // Compute the pesos[][] and the predecessors[][] matrices
  for(k=0;k<num_nodos;k++)
    for(i=0;i<num_nodos;i++)
    {
      for(j=0;j<num_nodos;j++)
      {
	
	//pesos[i][j] = MIN( pesos[i][j] , (pesos[i][k]+pesos[k][j]) );
	if(cm->pesos[i][j] > (cm->pesos[i][k] + cm->pesos[k][j]))
	{
		cm->pesos[i][j] = (cm->pesos[i][k] + cm->pesos[k][j]);
		cm->predecessors[i][j] = cm->predecessors[k][j];
	}
      }
      cm->pesos[i][i] = 0;
      cm->predecessors[i][i] = 0;
    }
    
  // Compute the table of the size of the shortest path between two nodes.
  // Computation of dist_nodes[][]
  // dist_nodes[i][j] is the minimal number of links to join j from i
  for(i=0;i<num_nodos;i++)
  for(j=0;j<num_nodos;j++)
  {
    s=j;
    n=0;
    while(s!=i)
    {
      s=cm->predecessors[i][s];
      if(s==INFINITY) { n=INFINITY; break; }
      if(n>num_nodos) { n=INFINITY; break; }
      n++;
    }
    cm->dist_nodes[i][j]=n;
  }
  
  // Compute sum_dist[i] that is the sum of the # of links to reach all the nodes from i
  // sum_dist[i] = sum(j=1;j=n;dist_nodes(i,j))
  for(i=0;i<num_nodos;i++)
  {
    cm->sum_dist[i] = 0;
    for(j=0;j<num_nodos;j++)
        if(cm->dist_nodes[i][j] != INFINITY)
            cm->sum_dist[i] += cm->dist_nodes[i][j];
  }
  
  // Compute the sum of the distance for the graph
  cm->graph_sum_dist = 0;
  for(i=0;i<num_nodos;i++)
    cm->graph_sum_dist = cm->graph_sum_dist + cm->sum_dist[i];
  
  // Compute normalized_inv_sum_dist[]
  min=INFINITY; max=0;
  for(i=0;i<num_nodos;i++)
  {
    if(cm->sum_dist[i]!=0)
    {
      min = MIN(min, cm->graph_sum_dist/cm->sum_dist[i]);
      max = MAX(max, cm->graph_sum_dist/cm->sum_dist[i]);
    }
  }
  for(i=0;i<num_nodos;i++)
    if(cm->sum_dist[i]!=0)
      cm->normalized_inv_sum_dist[i] = fabs((cm->graph_sum_dist/cm->sum_dist[i]-min)/(max-min));
    else
      cm->normalized_inv_sum_dist[i] = -1;
    
  // Compute max_dist[i] that is the maximal length of the distance between i and all the other nodes
  // max_dist[i] = max(j=1;j=n;dist_nodes(i,j))
  for(i=0;i<num_nodos;i++)
  {
    cm->max_dist[i] = 0;
    for(j=0;j<num_nodos;j++)
        if(cm->dist_nodes[i][j] != INFINITY)
            cm->max_dist[i] = MAX(cm->max_dist[i], cm->dist_nodes[i][j]);
  }
  
  // Compute normalized_max_dist[]
  min=INFINITY; max=0;
  for(i=0;i<num_nodos;i++)
  {
    min = MIN(min, cm->max_dist[i]);
    max = MAX(max, cm->max_dist[i]);
  }
  for(i=0;i<num_nodos;i++)
    cm->normalized_max_dist[i] = fabs(((float)(cm->max_dist[i])-min)/(max-min));
    
  // Compute num_connected[]
  for(i=0;i<num_nodos;i++)
  {
    cm->num_connected[i] = 0;
    for(j=0;j<num_nodos;j++)
      if((pesos_originales[i][j]!=INFINITY) && (pesos_originales[i][j]>0))
        cm->num_connected[i]++;
  }
  
  // Compute normalized_num_connected[]
  min=INFINITY; max=0;
  for(i=0;i<num_nodos;i++)
  {
    min = MIN(min, cm->num_connected[i]);
    max = MAX(max, cm->num_connected[i]);
  }
  for(i=0;i<num_nodos;i++)
    cm->normalized_num_connected[i] = fabs(((float)(cm->num_connected[i])-min)/(max-min));
    
  // Compute sum_connected[]
  for(i=0;i<num_nodos;i++)
  {
    cm->sum_connected[i] = 0;
    for(j=0;j<num_nodos;j++)
      if((pesos_originales[i][j]!=INFINITY) && (pesos_originales[i][j]>0))
        cm->sum_connected[i] += pesos_originales[i][j];
  }
  
  // Compute normalized_sum_connected[]
  min=INFINITY; max=0;
  for(i=0;i<num_nodos;i++)
  {
    min = MIN(min, cm->sum_connected[i]);
    max = MAX(max, cm->sum_connected[i]);
  }
  for(i=0;i<num_nodos;i++)
    cm->normalized_sum_connected[i] = fabs((cm->sum_connected[i]-min)/(max-min));
 
  // Cancel all the value in predecessors not on the normal path
  // Only for DEBUG
  /*for(i=0;i<num_nodos;i++)
  for(j=0;j<num_nodos;j++)
  {
    if((pesos_originales[i][j] == 0) && (i!=num_nodos-17))
      cm->dist_nodes[i][j] = 0;
  }*/
  
  return cm;
}

/*

// Find the central node (many definitions)
// Type should be a number between 1 and 4
// (1) the node minimizing the sum of the distance between a node and all the nodes
// (2) the node minimizing the maximal distance
// (3) the node maximizing the number of inner links
// (4) the node maximizing the sum of links  <== the most stable for normal and pfnet maps (node 30 or 7)
// The returned node is 1-based !
int find_central_node(int type)
{
  int i, j, k, n;
  float wsum, bestsum, wmax, bestmax;
  float wnum, bestnum, wtot, besttot;
  int bestn1, bestn2, bestn3, bestn4;
  
  // Compute bestn1 and bestn2  
  bestn1 = bestn2 = 0;
  bestsum = bestmax = 10E7;
  k=num_nodos;
  for(i=0;i<num_nodos;i++)
  {
    wsum = wmax = 0;
    n = 0;
    for(j=0;j<num_nodos;j++)
    {
      if((pesos[k][i][j]!=INFINITY) && (pesos[k][i][j]!=0.0))
      {
        wsum += pesos[k][i][j];
	wmax = MAX(wmax, pesos[k][i][j]);
	n++;
      }
    }
    if(n>0)
    {
	    if(bestsum>wsum) { bestsum=wsum; bestn1=i+1; }
	    if(bestmax>wmax) { bestmax=wmax; bestn2=i+1; }
    }
  }
  
  // Compute bestn3 and bestn4
  bestn3 = bestn4 = 0;
  bestnum = besttot = 0;
  for(i=0;i<num_nodos;i++)
  {
    wnum = wtot = 0;
    n = 0;
    for(j=0;j<num_nodos;j++)
    {
      if((pesos_originales[i][j]!=INFINITY) && (pesos_originales[i][j]!=0.0))
      {
	wnum++;
	wtot += pesos_originales[i][j];
	n++;
      }
    }
    if(n>0)
    {
	    if(bestnum<wnum) { bestnum=wnum; bestn3=i+1; }
	    if(besttot<wtot) { besttot=wtot; bestn4=i+1; }
    }
  }
 
  //printf("(1) bestn1=%d bestsum=%f\n", bestn1, bestsum);
  //printf("(2) bestn2=%d bestmax=%f\n", bestn2, bestmax);
  //printf("(3) bestn3=%d bestnum=%f\n", bestn3, bestnum);
  //printf("(4) bestn4=%d besttot=%f\n", bestn4, besttot);
  
  switch(type)
  {
    case 1: return bestn1;
    case 2: return bestn2;
    case 3: return bestn3;
    case 4: return bestn4;
    default: return -1;		// Should never arise
  }
}
*/


//---------------------------------------------------------------------------
// NAME: FreeCentralityMatrices
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Free the memory allocated for the CentralityMatrices structure.
//---------------------------------------------------------------------------

void FreeCentralityMatrices(CentralityMatrices* cm)
{
  int nnodes = cm->nnodes;
  Libera_matriz_dinamica_float(cm->pesos, nnodes);
  Libera_matriz_dinamica_float(cm->predecessors, nnodes);
  Libera_matriz_dinamica_int(cm->dist_nodes, nnodes);
  Libera_vector_dinamica_int(cm->sum_dist);
  Libera_vector_dinamica_float(cm->normalized_inv_sum_dist);
  Libera_vector_dinamica_int(cm->max_dist);
  Libera_vector_dinamica_float(cm->normalized_max_dist);
  Libera_vector_dinamica_int(cm->num_connected);
  Libera_vector_dinamica_float(cm->normalized_num_connected);
  Libera_vector_dinamica_float(cm->sum_connected);
  Libera_vector_dinamica_float(cm->normalized_sum_connected);
  free(cm);
}

//---------------------------------------------------------------------------
// NAME: PrintInDOT
//
// INPUTS: 
//    FILE* out : the file where to print the data (could be stdout)
//    int num_nodos : number of nodes in the graph
//    float **adjacency : used to enable or disable a link. If adjacency[i][j] = 0,
//        the link (i,j) is not printed.
//    float *node_labels : node_labels[i] indicates the label used for the
//        node i. If node_labels==null, no label is printed for nodes.
//    float **link_labels : link_labels[i][j] indicates the label used for the
//        link (i,j). If link_labels==null, no label is printed for links.
//
// RETURN: 
//
// PURPOSE: Print the given graph using the DOT format.
//---------------------------------------------------------------------------

void PrintInDOT_float(FILE* out, int num_nodos, float **adjacency, float *node_labels, float **link_labels)
{
  int i, j;
  // Print in the Graphviz input DOT format
  // To export in PNG, use:
  // dot -Gratio=fill -Gsize=15,15 -Kneato -Tpng -o map.png map.dot && eog map.png
  // fontsize=20 for Francia/Japan, 30 for Espana/Alemania/China
  fprintf(out, "graph G {\n");
  // Normal graph
  //fprintf(out, "\tnode [shape=none, margin=0, fixedsize=false, width=0.2, height=0.2];\n");
  // Normal graph
  //fprintf(out, "\tx, margin=0, fixedsize=false, /*width=1.2, height=1.2*/];\n");
  // Yellow circles + maximizing the proximity between nodes
  fprintf(out, "\tratio=auto;\n");
  fprintf(out, "\tsize=\"10,10\";\n");
  fprintf(out, "\toverlap=\"scale\"; \n");
  fprintf(out, "\tsep=-0.3;\n");
  fprintf(out, "\tcenter=true;\n");
  fprintf(out, "\t\n");
  fprintf(out, "\tedge [size=1];\n");
  fprintf(out, "\tnode [shape=circle, margin=0, fixedsize=true, width=1.0, height=1.0,\n");
  fprintf(out, "\t\tfontsize=20, fillcolor=gold2, style=filled, color=white];\n");
  // Yellow circles + minimizing the crossing between edges
  /*fprintf(out, "\tratio=auto;\n");
  fprintf(out, "\tsize=\"10,10\";\n");
  fprintf(out, "\toverlap=\"scale\"; \n");
  fprintf(out, "\tsep=-0.3;\n");
  fprintf(out, "\tcenter=true;\n");
  fprintf(out, "\tmodel=subset;\n");
  fprintf(out, "\t\n");
  fprintf(out, "\tedge [size=1];\n");
  fprintf(out, "\tnode [shape=circle, margin=0, fixedsize=true, width=3.0, height=3.0,\n");
  fprintf(out, "\t\tfontsize=30, fillcolor=gold2, style=filled, color=white];\n");*/
  // Print all the labels
  for(i=0; i<num_nodos; i++)
  {
    if(node_labels == NULL)
    {
      // Use the ID for the label of each node
      fprintf(out, "\t%d [label=\"%d\"];\n", i+1, i+1);
    }
    else
    {
      // Use the ID + the provided label for the label of each node
      fprintf(out, "\t%d [label=\"%d\\n(%f)\"];\n", i+1, i+1, node_labels[i]);
    }
  }
  // Print all the edges
  for(i=0; i<num_nodos; i++)
    for(j=0; j<i; j++)
    if ((adjacency[i][j]!=FALSE) || (adjacency[j][i]!=FALSE))
    {
      if(link_labels == NULL)
      {
        // Without the weights
        fprintf(out, "\t%d -- %d;\n",i+1,j+1);
      }
      else
      {
        // Using the weights for labels
        fprintf(out, "\t%d -- %d [label=\"%f\"];\n",i+1,j+1, link_labels[i][j]);
      }
    }
  fprintf(out, "}\n");
}


//---------------------------------------------------------------------------
// NAME: ComputeSubgraphObject(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Used to update all the properties of a SubgraphObject.
//---------------------------------------------------------------------------

SubgraphObject* ComputeSubgraphObject(Substructure* sub, Parameters *parameters, GraphFile* database)
{
  SubgraphObject* so;
  ULONG numLabels;
  int n;
  Graph* subgraph = sub->definition;
  Graph* origgraph;

  so = (SubgraphObject *) malloc(sizeof(SubgraphObject));
  if(so == NULL)
  {
    fprintf(stderr, "[ERROR] Cannot allocate memory for SubgraphObject!\n");
    exit(3);
  }
  so->graph = subgraph;
  
  // Position-based statistics
  int num_instance_nodes = 0;
  so->sum_weights = 0;
  so->sum_dist = 0;
  so->normalized_inv_sum_dist = 0;
  so->max_dist = 0;
  so->normalized_max_dist = 0;
  so->num_connected = 0;
  so->normalized_num_connected = 0;
  so->sum_connected = 0;
  so->normalized_sum_connected = 0;
  
  // Coverage-based properties
  so->cov_num_pos = 0;
  so->cov_num_neg = 0;
  n=0;
  while((origgraph=database[n].graph) != NULL)		// For each graph (n) given by the user
  {
    InstanceList *instanceList;
    ULONG numInstances;
    instanceList = FindInstances(subgraph, origgraph, parameters);
    numInstances = CountInstances(instanceList);
    
    // === Try to explore the corresponding nodes in the original graph
    ULONG i = 0, j;
    InstanceListNode *instanceListNode;
    if (instanceList != NULL) 
    {
       instanceListNode = instanceList->head;
       while (instanceListNode != NULL)		// For each instance (i)....
       {
          if(DEBUG==1)
	    printf("Graph %d:%s, nodes for instance %lu:", n+1, database[n].sub_name, i+1);
          if (instanceListNode->instance != NULL) 
          {
             for (j = 0; j < instanceListNode->instance->numVertices; j++)	// For each node (num_node)...
             {
                ULONG num_node = instanceListNode->instance->vertices[j];
		GraphFile gf = database[n];
		ULONG idx = origgraph->vertices[num_node].label;
		if(DEBUG==1)
		{
		  printf("\n\t%lu:", num_node + 1);
		  PrintLabel(idx, parameters->labelList);
		  printf("(sum=%d,Nsum=%f,max=%d,Nmax=%f,numC=%d,sumC=%f)",
		  	gf.cm->sum_dist[num_node], gf.cm->normalized_inv_sum_dist[num_node], gf.cm->max_dist[num_node], 
		  	gf.cm->normalized_max_dist[num_node], gf.cm->num_connected[num_node], gf.cm->sum_connected[num_node]);
		}
		// Update the statistics
		so->sum_weights += gf.cm->sum_connected[num_node];
		so->sum_dist += gf.cm->sum_dist[num_node];
		so->normalized_inv_sum_dist += gf.cm->normalized_inv_sum_dist[num_node];
		so->max_dist += gf.cm->max_dist[num_node];
		so->normalized_max_dist += gf.cm->normalized_max_dist[num_node];
		so->num_connected += gf.cm->num_connected[num_node];
		so->normalized_num_connected += gf.cm->normalized_num_connected[num_node];
		so->sum_connected += gf.cm->sum_connected[num_node];
		so->normalized_sum_connected += gf.cm->normalized_sum_connected[num_node];
		num_instance_nodes++;
             }
          }
	  if(DEBUG==1) printf("\n");
          instanceListNode = instanceListNode->next;
          i++;
       }
    }
    // === END Try...

    FreeInstanceList(instanceList);
    if(numInstances>0)
    {
      // Found a graph containing the substructure
      if(database[n].type == 'P')
      {
      	// Positive instance
        so->cov_filenames_pos[so->cov_num_pos] = strdup(database[n].sub_name);
        (so->cov_num_pos)++;
      }
      else
      {
        // Negative instance
        so->cov_filenames_neg[so->cov_num_neg] = strdup(database[n].sub_name);
        (so->cov_num_neg)++;

      }
    }
    n++;
  }
  so->pos_inst = sub->numInstances;
  so->neg_inst = sub->numNegInstances;

  // Complexity-based properties
  so->n_nodes = subgraph->numVertices;
  so->n_links = subgraph->numEdges;
  numLabels = parameters->labelList->numLabels;
  so->v_mdl = MDL(subgraph, numLabels, parameters);
  so->v_size = GraphSize(subgraph);
  so->v_value = sub->value;
  
  // Position-based properties (graph-dependent, and the average for each node in the sub-structure)
  so->sum_dist /= num_instance_nodes;
  so->normalized_inv_sum_dist /= num_instance_nodes;
  so->max_dist /= num_instance_nodes;
  so->normalized_max_dist /= num_instance_nodes;
  so->num_connected /= num_instance_nodes;
  so->normalized_num_connected /= num_instance_nodes;
  so->sum_connected /= num_instance_nodes;
  so->normalized_sum_connected /= num_instance_nodes;
  
  // MOO algorithm: create the objective vector
  so->objv_size = 2;
  so->objv = (float*)malloc((so->objv_size)*sizeof(float));
  so->objn = (char**)malloc((so->objv_size)*sizeof(char*));
  n = 0;
  // ** TODO: This part have to be customized
  /*// Default objective functions
  so->objv[n] = so->cov_num_pos;
  so->objn[n++] = strdup("cov_num_pos");
  so->objv[n] = so->normalized_max_dist;
  so->objn[n++] = strdup("normalized_max_dist");*/
  /*// For testing
  so->objv[n] = so->pos_inst;
  so->objn[n++] = strdup("pos_inst");
  so->objv[n] = so->neg_inst;
  so->objn[n++] = strdup("neg_inst");*/
  /*// For testing
  so->objv[n] = so->n_nodes;
  so->objn[n++] = strdup("n_nodes");
  so->objv[n] = so->n_links;
  so->objn[n++] = strdup("n_links");*/
  /*// For testing
  so->objv[n] = so->normalized_num_connected;
  so->objn[n++] = strdup("normalized_num_connected");
  so->objv[n] = -so->normalized_sum_connected;
  so->objn[n++] = strdup("-normalized_sum_connected");*/
  /*// Support/Complexity based (conservative approach)
  so->objv[n] = so->cov_num_pos;
  so->objn[n++] = strdup("cov_num_pos");
  so->objv[n] = so->cov_num_neg;
  so->objn[n++] = strdup("cov_num_neg");
  so->objv[n] = so->n_nodes;
  so->objn[n++] = strdup("n_nodes");*/
  // Support/Complexity based (subdue approach)
  so->objv[n] = so->cov_num_pos-so->cov_num_neg;
  so->objn[n++] = strdup("cov_num_pos-neg");
  so->objv[n] = so->n_nodes;
  so->objn[n++] = strdup("n_nodes");
  // **********
  so->num_inst = 0;
  return so;
}

//---------------------------------------------------------------------------
// NAME: FreeSO(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE:
//---------------------------------------------------------------------------


void FreeSO(SubgraphObject* so)
{
  so->num_inst--;
  if(so->num_inst == 0)
  {
    int i;
    for(i=0;i<so->objv_size;i++)
      free(so->objn[i]);
    for(i=0;i<so->cov_num_pos;i++)
      free(so->cov_filenames_pos[i]);
    for(i=0;i<so->cov_num_neg;i++)
      free(so->cov_filenames_neg[i]);
    free(so->objv);
    free(so->objn);
    free(so);
  }
}


//---------------------------------------------------------------------------
// NAME: PrintSubgraphObject(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Print a SubgraphObject.
//---------------------------------------------------------------------------

void PrintSubgraphObject(SubgraphObject* so, Parameters *parameters)
{
  int i;
  printf("=== SubgraphObject:\n");
  /*PrintGraph(so->graph, parameters->labelList);
  printf("** COVERAGE: cov_num=%d [", so->cov_num);
  for(i=0;i<so->cov_num;i++)
    printf("%s,", so->cov_filenames[i]);
  printf("], pos_inst=%d, neg_inst=%d\n", so->pos_inst, so->neg_inst);
  printf("** COMPLEXITY: n_nodes=%d, n_links=%d, v_mdl=%f, v_size=%lu, v_value=%f\n",
  	so->n_nodes, so->n_links, so->v_mdl, so->v_size, so->v_value);
  printf("** POSITION: sum_weights=%f, sum_dist=%f, normalized_inv_sum_dist=%f, max_dist=%f, normalized_max_dist=%f, num_connected=%f, normalized_num_connected=%f, sum_connected=%f, normalized_sum_connected=%f\n",
  	 so->sum_weights, so->sum_dist, so->normalized_inv_sum_dist, so->max_dist, so->normalized_max_dist, so->num_connected, so->normalized_num_connected, so->sum_connected, so->normalized_sum_connected);*/
  printf("** OBJECTIVE VECTOR: [");
  for(i=0;i<so->objv_size;i++)
    printf(" %f", so->objv[i]);
  printf("]\n");

  printf("==========\n");
}


//---------------------------------------------------------------------------
// NAME: PrintSOL(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

void PrintSOL(SOList* L, Parameters *parameters)
{
  int i;
  printf("SOList %p (size %d):\n", L, L->size);
  for(i=0;i<L->size;i++)
  {
    PrintSubgraphObject(L->list[i], parameters);
  }
  printf("\n");
}


//---------------------------------------------------------------------------
// NAME: PrintSubgraphObjectInFile(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Print a 'SubgraphObject' object into a file, using a
//          easy-to-decrypt format.
//---------------------------------------------------------------------------

void PrintSubgraphObjectInFile(FILE* _fin, SubgraphObject* so, Parameters *parameters)
{
  int i;
  fprintf(_fin, "BEGIN_OBJECT\n");
  
  // Write the coverage values
  fprintf(_fin, "coverage_pos_size=%d\n", so->cov_num_pos);
  fprintf(_fin, "coverage_neg_size=%d\n", so->cov_num_neg);
  for(i=0;i<so->cov_num_pos;i++)
    fprintf(_fin, "coverage_pos_%d=%s\n", i+1, so->cov_filenames_pos[i]);
  for(i=0;i<so->cov_num_neg;i++)
    fprintf(_fin, "coverage_neg_%d=%s\n", i+1, so->cov_filenames_neg[i]);
  fprintf(_fin, "pos_instance=%d\n", so->pos_inst);
  fprintf(_fin, "neg_instance=%d\n", so->neg_inst);
  
  // Write the complexity values
  fprintf(_fin, "num_nodes=%d\n", so->n_nodes);
  fprintf(_fin, "num_links=%d\n", so->n_links);
  fprintf(_fin, "value_mdl=%f\n", so->v_mdl);
  fprintf(_fin, "value_size=%lu\n", so->v_size);
  fprintf(_fin, "v_value=%f\n", so->v_value);
  
  // Write the location values
  fprintf(_fin, "sum_weights=%f\n", so->sum_weights);
  fprintf(_fin, "sum_dist=%f\n", so->sum_dist);
  fprintf(_fin, "normalized_inv_sum_dist=%f\n", so->normalized_inv_sum_dist);
  fprintf(_fin, "max_dist=%f\n", so->max_dist);
  fprintf(_fin, "normalized_max_dist=%f\n", so->normalized_max_dist);
  fprintf(_fin, "num_connected=%f\n", so->num_connected);
  fprintf(_fin, "normalized_num_connected=%f\n", so->normalized_num_connected);
  fprintf(_fin, "sum_connected=%f\n", so->sum_connected);
  fprintf(_fin, "normalized_sum_connected=%f\n", so->normalized_sum_connected);
  
  // Write the objective vector values
  fprintf(_fin, "objective_vect_size=%d\n", so->objv_size);
  for(i=0;i<so->objv_size;i++)
    fprintf(_fin, "objective_vect_name_%d=%s\n", i+1, so->objn[i]);
  for(i=0;i<so->objv_size;i++)
    fprintf(_fin, "objective_vect_value_%d=%f\n", i+1, so->objv[i]);
    
  // Write the structure
  fprintf(_fin, "BEGIN_STRUCT\n");
  WriteGraphToFile(_fin, so->graph,
    parameters->labelList, 0, 0, 
    so->graph->numVertices, FALSE);
  fprintf(_fin, "END_STRUCT\n");
 
  fprintf(_fin, "END_OBJECT\n");
}

//---------------------------------------------------------------------------
// NAME: PrintSOLInFile(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

void PrintSOLInFile(FILE* _fin, SOList* L, Parameters *parameters)
{
  int i;
  fprintf(_fin, "size=%d\n", L->size);
  fprintf(_fin, "BEGIN_LIST\n");
  for(i=0;i<L->size;i++)
  {
    PrintSubgraphObjectInFile(_fin, L->list[i], parameters);
  }
  fprintf(_fin, "END_LIST\n");
}



//---------------------------------------------------------------------------
// NAME: NewSOL(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

SOList* NewSOL(void)
{
	SOList* L = (SOList*) malloc(ALLOC_INCR*sizeof(SOList));
	L->list = (SubgraphObject**) calloc(ALLOC_INCR, sizeof(SubgraphObject*));
	L->alloc = ALLOC_INCR;
	L->size = 0;
	return L;
}

//---------------------------------------------------------------------------
// NAME: FreeSOL(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

void FreeSOL(SOList* L)
{
  int i;
  for(i=0;i<L->size;i++)
    FreeSO(L->list[i]);
  free(L->list);
  free(L);
}

//---------------------------------------------------------------------------
// NAME: AddSO(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

void AddSO(SOList* L, SubgraphObject* so)
{
	//int i;
	if(L->alloc == ALLOC_INCR)
	{
		// Reallocation
		L->alloc += ALLOC_INCR;
		L->list = (SubgraphObject**) realloc(L->list, (L->alloc)*sizeof(SubgraphObject*));
		if(L->list == NULL)
		{
			fprintf(stderr, "Problem when allocating memory for L->list\n");
			exit(1);
		}
	}
	// Add the data
	L->list[L->size] = so;
	L->size++;
	so->num_inst++;
}

//---------------------------------------------------------------------------
// NAME: RemoveSO(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: 
//---------------------------------------------------------------------------

void RemoveSO(SOList* L, int n)
{
  int i;
  FreeSO(L->list[n]);
  for(i=n+1;i<L->size;i++)
    L->list[i-1] = L->list[i];
  L->size--;
}


//---------------------------------------------------------------------------
// NAME: KungSort(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Kung et al.'s Efficient MO-Method. Sort function
//    For more information, see the book "Multi-objective optimization using
//    evolutionary algorithms" page 38.
//    This function implements a lexicographic sort order.
//---------------------------------------------------------------------------

// Used in the first step of the MO algorithm; sort according to the descending order.
int KungSortFirst(const void* _o1, const void* _o2)
{
	//int k;
	SubgraphObject* o1 = *((SubgraphObject**) _o1);
	SubgraphObject* o2 = *((SubgraphObject**) _o2);
	/*if(o1==NULL || o2==NULL || o1->objv_size==0 || o2->objv_size==0)
		return 0;
	else*/
	return o1->objv[0]<o2->objv[0];
}

int KungSort(SubgraphObject* o1, SubgraphObject* o2)
{
	//int k;
	return o1->objv[0]<o2->objv[0];
	/*for(k=0;k<o1->SIZE...;k++)
	{
		if(o1->objv[k]<o2->objv[k])
	}*/
}

//---------------------------------------------------------------------------
// NAME: KungCheckDomination(...)
//
// INPUTS: o1, o2, the objects to compare; n, the objective we start from
//
// RETURN: -1 if o2 dominates o1
//          0 if o1 and o2 are equals (same values for all the objectives)
//          1 if o1 dominates o2
//          2 if no one dominates
//         Check only these following objectives: from n (included) to MAX.
//
// PURPOSE: Kung et al.'s Efficient MO-Method.
//    For more information, see the book "Multi-objective optimization using
//    evolutionary algorithms" page 38.
//
// Notes on the Jaccard index.
// The Jaccard index is used to mark as non dominating the objects having a
// too much similar covering set of the instances. It is used as following:
// IF Jaccard(o1,o2) < 0.5,
// THEN non-dominance,
// ELSE look the objective values to decide.
//
// For negatives instances, we consider two different measures:
// - the conservative one, in which any dissimilarity inside the positive or
//   the negative set will be considered as non-dominative (f.i. if the objects
//   cover the same positive instances, but different negative instances, we
//   consider these two objects as non-dominating)/
//   ==> Jaccard(o1,o2) = MIN( PositiveJaccard(o1,o2) , NegativeJaccard(o1,o2) )
//
// - the positive one, in which negative instances as simply ignored. From the
//   point of view of the user, objects found in the same positive instances
//   could be viewed as similar, even if they cover different negative instances.
//   ==> Jaccard(o1,o2) = PositiveJaccard(o1,o2)
//
//---------------------------------------------------------------------------
#define CONSERVATIVE_JACCARD
//#define POSITIVE_JACCARD
int KungCheckDomination(SubgraphObject* o1, SubgraphObject* o2, int n)
{
	int k, flagO1=0, flagO2=0, flagEQ=0;
	double v1, v2;
	
#ifdef POSITIVE_JACCARD
	if(Jaccard(o1,o2)<0.5)
		return 2;	// Save time: in this case, we consider o1/o2 as different, and non-dominating
#endif
	
#ifdef CONSERVATIVE_JACCARD
	if(Jaccard(o1,o2)<0.5)
		return 2;
	if(NegJaccard(o1,o2)<0.5)
		return 2;
#endif

	if(NegJaccard(o1,o2)<0.5)
		return 2;	// Save time: in this case, we consider o1/o2 as different, and non-dominators

	for(k=n;k<o1->objv_size;k++)
	{
		v1 = o1->objv[k];
		v2 = o2->objv[k];
		if(v1>v2) flagO1=1;		// O1 dominates for at least one objective
		else if(v1<v2) flagO2=1;	// O2 dominates for at least one objective
		else flagEQ=1;
	}
	if((flagO1==1) && (flagO2==1)) return 2;
	else if(flagO1==1) return 1;
	else if(flagO2==1) return -1;
	else return 0;	// flagO1==0 && flagO2==0
}


//---------------------------------------------------------------------------
// NAME: KungNonDominatedSet(...)
//
// INPUTS: 
//
// RETURN: the non-dominated set of solutions of P.
//
// PURPOSE: Kung et al.'s Efficient MO-Method.
//    For more information, see the book "Multi-objective optimization using
//    evolutionary algorithms" page 38.
//---------------------------------------------------------------------------
SOList* KungNonDominatedSet(SOList* P)
{
	int i, j, size=P->size, flag, ret;
	SOList* N = NewSOL();
	if(size==1)
	{
		AddSO(N, P->list[0]);
		return N;
	}
	for(i=0;i<size;i++)
	{
		flag=1;
		for(j=0;j<size;j++)
		{
			ret = KungCheckDomination(P->list[i], P->list[j], 0);
			if((i!=j) && (ret==-1))
				flag=0;
			if((ret==0) && (i>j))
				flag=0;		// If solutions are equals, take only the first one
		}
		if(flag)
			AddSO(N, P->list[i]);
	}
	return N;
}


//---------------------------------------------------------------------------
// NAME: KungFront(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Kung et al.'s Efficient MO-Method. Front function
//    For more information, see the book "Multi-objective optimization using
//    evolutionary algorithms" page 38.
//---------------------------------------------------------------------------

SOList* KungFront(SOList* P, Parameters *parameters)
{
  int i, it, ib, size = P->size, sizeB, sizeT, flag;
  SOList *T, *B, *Tnew, *Bnew, *nonT, *nonB;
  // Step 1
  if(size == 1)
  {
#if MODEBUG
    printf("size==1 => quit!\n");
#endif
    T = NewSOL();
    AddSO(T, P->list[0]);
    return T;
  }
  // Step 2
  qsort(P->list, P->size, sizeof(SubgraphObject*), (int (*)(const void*, const void*)) KungSortFirst);
#if MODEBUG
  printf("######### P ########\n");
  PrintSOL(P, parameters);
#endif
  T = NewSOL();
  B = NewSOL();
  for(i=0;i<size/2;i++) AddSO(T, P->list[i]);
  for(;i<size;i++) AddSO(B, P->list[i]);
#if MODEBUG
  printf("######### T before ########\n");
  PrintSOL(T, parameters);
  printf("######### B before ########\n");
  PrintSOL(B, parameters);
#endif
  
  Tnew = KungFront(T, parameters);
  Bnew = KungFront(B, parameters);

  //if(T->size!=1) { free(T->list); free(T); } //FreeSOL(T);
  //if(B->size!=1) { free(B->list); free(B); } //FreeSOL(B);
  FreeSOL(T);
  FreeSOL(B);
  T=Tnew;
  B=Bnew;
  
#if MODEBUG
  printf("######### T after ########\n");
  PrintSOL(T, parameters);
  printf("######### B after ########\n");
  PrintSOL(B, parameters);
#endif
  
  nonT = KungNonDominatedSet(T);
  nonB = KungNonDominatedSet(B);
  FreeSOL(T);
  FreeSOL(B);
  
#if MODEBUG
  printf("######### nonT after ########\n");
  PrintSOL(nonT, parameters);
  printf("######### nonB after ########\n");
  PrintSOL(nonB, parameters);
#endif
  
  sizeB = nonB->size;
  sizeT = nonT->size;
  for(ib=0;ib<sizeB;ib++)
  {
    SubgraphObject *objB = nonB->list[ib];
    flag=1;
    for(it=0;it<sizeT;it++)
    {
      SubgraphObject *objT = nonT->list[it];
      if(KungCheckDomination(objB, objT, 0)<=0)
      {
        flag=0;
#if MODEBUG
	printf("nonT[%d] dominate nonB[%d] => objB will not be included\n", it, ib);
#endif
        break;
      }
    }
    if(flag)
    {
      // objB is not dominated by any objT
      AddSO(nonT, objB);
    }
  }
  
#if MODEBUG
  printf("######### T final ########\n");
  PrintSOL(nonT, parameters);
#endif
  
  //FreeSOL(nonT);
  FreeSOL(nonB);

  return nonT;
}


//---------------------------------------------------------------------------
// NAME: Jaccard(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Return the Jaccard index of the two given objects.
//
//---------------------------------------------------------------------------

double Jaccard(SubgraphObject* o1, SubgraphObject* o2)
{
  int i, j, _inclusion=0, _union=0;
#if MODEBUG
  printf("======================\n");
  for(i=0;i<o1->cov_num_pos;i++)
    printf("O1 POS %s\n", o1->cov_filenames_pos[i]);
  for(i=0;i<o2->cov_num_pos;i++)
    printf("O2 POS %s\n", o2->cov_filenames_pos[i]);
#endif
  // Count in one loop the number of maps in o2 included or not in the maps of o1
  // That is, count inclusion(o1,o2) and union(o1,o2)
  _union = o1->cov_num_pos;
  for(j=0;j<o2->cov_num_pos;j++)
  {
    int flag = 0;
    for(i=0;i<o1->cov_num_pos;i++)
    {
      if(!strcmp(o1->cov_filenames_pos[i],o2->cov_filenames_pos[j]))
      {
      	flag = 1;
	break;
      }
    }
    if(flag) _inclusion++;
    else _union++;
  }
#if MODEBUG
  printf("Jaccard(o1,o2) = %f\n", (double)_inclusion/_union);
#endif
  return (double)_inclusion/_union;
}


//---------------------------------------------------------------------------
// NAME: NegJaccard(...)
//
// INPUTS: 
//
// RETURN: 
//
// PURPOSE: Return the Jaccard index of the two given objects (considering
//          only the negative instances).
//
//---------------------------------------------------------------------------

double NegJaccard(SubgraphObject* o1, SubgraphObject* o2)
{
  int i, j, _inclusion=0, _union=0;
  // Count in one loop the number of maps in o2 included or not in the maps of o1
  // That is, count inclusion(o1,o2) and union(o1,o2)
  _union = o1->cov_num_neg;
  for(j=0;j<o2->cov_num_neg;j++)
  {
    int flag = 0;
    for(i=0;i<o1->cov_num_neg;i++)
    {
      if(!strcmp(o1->cov_filenames_neg[i],o2->cov_filenames_neg[j]))
      {
      	flag = 1;
	break;
      }
    }
    if(flag) _inclusion++;
    else _union++;
  }
#if MODEBUG
  printf("NegJaccard(o1,o2) = %f\n", (double)_inclusion/_union);
#endif
  return (double)_inclusion/_union;
}
