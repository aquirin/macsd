#if !defined(_VPLAT_PARAM_H)
#define _VPLAT_PARAM_H

#include <string.h>
#include <iostream>
#include <stdio.h>
//#include "macros.h"

#include <vector>
#include <iostream>
#include <stdlib.h>
using std::vector;

#define MAX_PARAMETER_SIZE 2000		// Taille maximale d'un champ de param�re (notamment char[])

enum DataTypeValue
{
	MISSING_BOOL_VALUE = 0,
	INTEGER_VALUE,
	UINTEGER_VALUE,
	DOUBLE_VALUE,
	NOMINAL_VALUE,
	NUMERICAL_VALUE,
	STRING_VALUE
};

typedef struct SParameterRegister {
	void* param;
	char name[50];
	DataTypeValue type;
	char* comment;
} ParameterRegister;

class CParameter
{

// Construction
public:
	CParameter();
    virtual ~CParameter();

// Attributes
private:
	vector<ParameterRegister> param_list;		// Liste des param�res

public:
	bool used;
	
	// Global
	int GLOB_tipoAlgoritmo;
	double GLOB_tCiclo;
	unsigned int GLOB_maxTiempo;
	double GLOB_areaMax;
	char GLOB_rutaEntrada[MAX_PARAMETER_SIZE];
	char GLOB_rutaSalida[MAX_PARAMETER_SIZE];
	int GLOB_areaImplicita;
	int GLOB_BL;
	int GLOB_semilla;
	unsigned int GLOB_numObjs;
	unsigned int GLOB_preferencias;
	
	// MOACO
	double MOACO_tau0;
	double MOACO_beta;
	double MOACO_ro;
	double MOACO_q0;
	unsigned int MOACO_numHormigas;
	unsigned int MOACO_multiheuristics;
	double MOACO_gamma;
	double MOACO_alfaGrasp;
	double MOACO_alfaObj1;
	double MOACO_tau0Prima;
	unsigned int MOACO_numColonias;
	unsigned int MOACO_nEstOptimo;
	unsigned int MOACO_stepSize;
        unsigned int MOACO_maxTama;
        unsigned int MOACO_ranking;
	
	// GO
	char GO_bpn[MAX_PARAMETER_SIZE];
	char GO_bpe[MAX_PARAMETER_SIZE];
	char GO_fmn[MAX_PARAMETER_SIZE];
	char GO_fme[MAX_PARAMETER_SIZE];
	char GO_ccn[MAX_PARAMETER_SIZE];
	char GO_cce[MAX_PARAMETER_SIZE];
	char GO_x[MAX_PARAMETER_SIZE];
	
	// VMAP
	int VMAP_num_nodes;
	int VMAP_num_edges;


// Functions
private:
	void RegisterAll(void);
	void RegisterParameter(void* param, char* name, DataTypeValue type, int value);
	void RegisterParameter(void* param, char* name, DataTypeValue type, double value);
	void RegisterParameter(void* param, char* name, DataTypeValue type, char* value);
	int GetIndexParameter(char* name);
	void TraitementLigne(char *ligne);
	char* TrimSpaces(char *ligne);

public:
	void Import(CParameter *P);
	void GetValueParameter(void* value, char* name);
	void GetValueParameter(void* value, int n);
	void SetValueParameter(void* value, char* name);
	void SetValueParameter(void* value, int n);
	void PrintConfiguration(FILE* out, char* substring);
	int ReadConfiguration(char* path);
	int WriteConfiguration(char* path);
	int GetNumberParameters(void);
	char* GetNameParameter(int n);
	DataTypeValue GetTypeParameter(int n);
	char* GetCommentParameter(int n);
	void* GetPointerParameter(int n);
};


extern CParameter PARA;


/////////////////////////////////////////////////////////////////////////////

#endif // !defined(_VPLAT_PARAM_H)

