/*
Classe permettant de d�inir des param�res arbitraires.
Classe enti�ement libre laiss� en libre acc� pour les mod�es.

A initialiser avant l'utilisation de StartLearning().

Pour ajouter un param�re �cette classe, il faut d�inir une
variable sp�ifique dans Parameter.h et enregistrer dans le
constructeur cette variable, son nom, son type et sa valeur
par d�aut.

Int��:
Parce que chaque mod�e est diff�ent et qu'il peut �re int�essant
d'y injecter des param�res sp�ifique au mod�e consid��ou de
retirer des stats pr�ises sp�ifiques au mod�e, cette classe statique
peut �re utilis� �cet effet.

Gestion de fichiers de configuration avec commentaires (en lecture/�riture).
Il est possible de partir �la d�ouverte de tous les param�res d'une m�hode
(nommage sous forme d'index num�ique, conversion en cha�e de caract�es).
La sauvegarde de la configuration replace les commentaires de ligne (plac�
sur la m�e ligne qu'une variable, apr� '#').
*/

#include "Parameter.h"


/////////////////////////////////////////////////////////////////////////////
// Regle

// Constructeur
CParameter::CParameter()
{
	// Used
	used = true;

	RegisterAll();
}


// Destructeur
CParameter::~CParameter()
{
	// D�ruit les cha�es
	int n;
	for(n=0;n<param_list.size();n++)
	{
		if(param_list[n].comment)
			free(param_list[n].comment);
	}
}

/**************************************************\
Enregistre toutes les variables de classe.
\**************************************************/
void CParameter::RegisterAll(void)
{
	// Predefined types
	// INTEGER_VALUE, UINTEGER_VALUE, DOUBLE_VALUE, STRING_VALUE
	
	// Set here the default values!!

	// Global
	RegisterParameter(&GLOB_tipoAlgoritmo, "GLOB_tipoAlgoritmo", INTEGER_VALUE, 0);
	RegisterParameter(&GLOB_tCiclo, "GLOB_tCiclo", DOUBLE_VALUE, 0.0);
	RegisterParameter(&GLOB_maxTiempo, "GLOB_maxTiempo", UINTEGER_VALUE, 0);
	RegisterParameter(&GLOB_areaMax, "GLOB_areaMax", DOUBLE_VALUE, 0.0);
	RegisterParameter(&GLOB_rutaEntrada, "GLOB_rutaEntrada", STRING_VALUE, "");
	RegisterParameter(&GLOB_rutaSalida, "GLOB_rutaSalida", STRING_VALUE, "");
	RegisterParameter(&GLOB_areaImplicita, "GLOB_areaImplicita", INTEGER_VALUE, 0);
	RegisterParameter(&GLOB_BL, "GLOB_BL", INTEGER_VALUE, 0);
	RegisterParameter(&GLOB_semilla, "GLOB_semilla", INTEGER_VALUE, 0);
	RegisterParameter(&GLOB_numObjs, "GLOB_numObjs", UINTEGER_VALUE, 0);
	RegisterParameter(&GLOB_preferencias, "GLOB_preferencias", UINTEGER_VALUE, 0);
	
	// MOACO
	RegisterParameter(&MOACO_tau0, "MOACO_tau0", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_beta, "MOACO_beta", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_ro, "MOACO_ro", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_q0, "MOACO_q0", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_numHormigas, "MOACO_numHormigas", UINTEGER_VALUE, 0);
	RegisterParameter(&MOACO_multiheuristics, "MOACO_multiheuristics", UINTEGER_VALUE, 0);
	RegisterParameter(&MOACO_gamma, "MOACO_gamma", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_alfaGrasp, "MOACO_alfaGrasp", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_alfaObj1, "MOACO_alfaObj1", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_tau0Prima, "MOACO_tau0Prima", DOUBLE_VALUE, 0.0);
	RegisterParameter(&MOACO_numColonias, "MOACO_numColonias", UINTEGER_VALUE, 0);
	RegisterParameter(&MOACO_nEstOptimo, "MOACO_nEstOptimo", UINTEGER_VALUE, 0);
	RegisterParameter(&MOACO_stepSize, "MOACO_stepSize", UINTEGER_VALUE, 0);
        RegisterParameter(&MOACO_maxTama, "MOACO_maxTama", UINTEGER_VALUE, 0);
      	RegisterParameter(&MOACO_ranking, "MOACO_ranking", UINTEGER_VALUE, 0);
	
	// GO
	RegisterParameter(&GO_bpn, "GO_bpn", STRING_VALUE, "");
	RegisterParameter(&GO_bpe, "GO_bpe", STRING_VALUE, "");
	RegisterParameter(&GO_fmn, "GO_fmn", STRING_VALUE, "");
	RegisterParameter(&GO_fme, "GO_fme", STRING_VALUE, "");
	RegisterParameter(&GO_ccn, "GO_ccn", STRING_VALUE, "");
	RegisterParameter(&GO_cce, "GO_cce", STRING_VALUE, "");
	RegisterParameter(&GO_x, "GO_x", STRING_VALUE, "");
	
	// VMAP
		// VMAP
	RegisterParameter(&VMAP_num_nodes, "VMAP_num_nodes", INTEGER_VALUE, "");
	RegisterParameter(&VMAP_num_edges, "VMAP_num_edges", INTEGER_VALUE, "");

}

/**************************************************\
Enregistre un param�re dans la base.
Affecte au pointeur d'une variable de Parameter::
donn�, un nom, un type et une valeur par d�aut.
\**************************************************/
void CParameter::RegisterParameter(void* param, char* name, DataTypeValue type, int value)
{
	ParameterRegister PR;
	PR.param = param;
	strcpy(PR.name, name);
	PR.type = type;
	*((int*)PR.param) = value;
	PR.comment = NULL;
	int num = GetIndexParameter(name);
	if(num<0) param_list.push_back(PR);
	else param_list[num] = PR;
}

void CParameter::RegisterParameter(void* param, char* name, DataTypeValue type, double value)
{
	ParameterRegister PR;
	PR.param = param;
	strcpy(PR.name, name);
	PR.type = type;
	*((double*)PR.param) = value;
	PR.comment = NULL;
	int num = GetIndexParameter(name);
	if(num<0) param_list.push_back(PR);
	else param_list[num] = PR;
}

void CParameter::RegisterParameter(void* param, char* name, DataTypeValue type, char* value)
{
	ParameterRegister PR;
	PR.param = param;
	strcpy(PR.name, name);
	PR.type = type;
	strcpy((char*)param, value);
	PR.comment = NULL;
	int num = GetIndexParameter(name);
	if(num<0) param_list.push_back(PR);
	else param_list[num] = PR;
}

/**************************************************\
Cherche un param�re dans la liste.
\**************************************************/
int CParameter::GetIndexParameter(char* name)
{
	int n;
	for(n=0;n<param_list.size();n++)
	{
		if(!strcmp(param_list[n].name, name)) return n;
	}

	return -1;
}


/**************************************************\
Renvoit la valeur du param�re nomm�'name' dans
'value'.
\**************************************************/
void CParameter::GetValueParameter(void* value, char* name)
{
	int n = GetIndexParameter(name);
	if(n<0)
	{
		fprintf(stderr, "The parameter '%s' does not exist !\n", name);
		value = NULL;
		return;
	}
	GetValueParameter(value, n);
}

/**************************************************\
Renvoit la valeur du param�re num�o n dans
'value'.
\**************************************************/
void CParameter::GetValueParameter(void* value, int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		value = NULL;
		return;
	}
	if(param_list[n].type == INTEGER_VALUE)
		*((int*)value) = *((int*)(param_list[n].param));
	else if(param_list[n].type == UINTEGER_VALUE)
		*((unsigned int*)value) = *((unsigned int*)(param_list[n].param));
	else if(param_list[n].type == DOUBLE_VALUE)
		*((double*)value) = *((double*)(param_list[n].param));
	else if(param_list[n].type == STRING_VALUE)
		strcpy( (char*)value, (char*)(param_list[n].param) );
	else
	{
		fprintf(stderr, "The parameter number %d has a type not recognized !\n", n);
		value = NULL;
		return;
	}
}

/**************************************************\
D�init la valeur du param�re nomm�'name'.
\**************************************************/
void CParameter::SetValueParameter(void* value, char* name)
{
	int n = GetIndexParameter(name);
	if(n<0)
	{
		fprintf(stderr, "The parameter '%s' does not exist !\n", name);
		value = NULL;
		return;
	}
	SetValueParameter(value, n);
}

/**************************************************\
D�init la valeur du param�re num�o n.
\**************************************************/
void CParameter::SetValueParameter(void* value, int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		value = NULL;
		return;
	}
	if(param_list[n].type == INTEGER_VALUE)
		*((int*)(param_list[n].param)) = *((int*)value);
		//memcpy( param_list[n].param, value, sizeof(int) );
	else if(param_list[n].type == UINTEGER_VALUE)
		*((unsigned int*)(param_list[n].param)) = *((unsigned int*)value);
	else if(param_list[n].type == DOUBLE_VALUE)
		*((double*)(param_list[n].param)) = *((double*)value);
	else if(param_list[n].type == STRING_VALUE)
		strcpy( (char*)(param_list[n].param), (char*)value );
	else
	{
		fprintf(stderr, "The parameter number %d has a type not recognized !\n", n);
		value = NULL;
		return;
	}
}

/**************************************************\
Enregistre la configuration courante.

TODO: les commentaires ne sont pas sauv�!
\**************************************************/
int CParameter::WriteConfiguration(char* path)
{
	printf("Writing Config '%s' ...\n", path);

	FILE *in;
	if((in = fopen(path, "w")) == NULL)
	{
		fprintf(stderr, "Error while writing the config file '%s' !\n", path);
		return -1;
	}

	fprintf(in, "# VPlat 1.0 parameters.\n\n");

	int n;
	int ret = 0;
	for(n=0;n<param_list.size();n++)
	{
		if(param_list[n].type == INTEGER_VALUE) fprintf(in, "%s = %d", param_list[n].name, *((int*)(param_list[n].param)));
		else if(param_list[n].type == UINTEGER_VALUE) fprintf(in, "%s = %u", param_list[n].name, *((unsigned int*)(param_list[n].param)));
		else if(param_list[n].type == DOUBLE_VALUE) fprintf(in, "%s = %f", param_list[n].name, *((double*)(param_list[n].param)));
		else if(param_list[n].type == STRING_VALUE) fprintf(in, "%s = %s", param_list[n].name, (char*)(param_list[n].param));
		else { fprintf(stderr, "The parameter '%s' has a type not recognized !\n", param_list[n].name); ret = -2; }
		if(param_list[n].comment) fprintf(in, "\t\t\t# %s", param_list[n].comment);
		fprintf(in, "\n");
	}

	fclose(in);
	return ret;
}

/**************************************************\
Print a part of the configuration on the provided
FILE pointer. Only the parameters containing the
string "substring" are printed with their comments.
\**************************************************/
void CParameter::PrintConfiguration(FILE* out, char* substring)
{
	//fprintf(out, "# VPlat 1.0 parameters.\n");

	int n;
	int ret = 0;
	for(n=0;n<param_list.size();n++)
	{
		if(strstr(param_list[n].name, substring)==NULL)
			continue;
		
		if(param_list[n].type == INTEGER_VALUE) fprintf(out, "%s = %d", param_list[n].name, *((int*)(param_list[n].param)));
		else if(param_list[n].type == UINTEGER_VALUE) fprintf(out, "%s = %u", param_list[n].name, *((unsigned int*)(param_list[n].param)));
		else if(param_list[n].type == DOUBLE_VALUE) fprintf(out, "%s = %f", param_list[n].name, *((double*)(param_list[n].param)));
		else if(param_list[n].type == STRING_VALUE) fprintf(out, "%s = %s", param_list[n].name, (char*)(param_list[n].param));
		else { fprintf(stderr, "The parameter '%s' has a type not recognized !\n", param_list[n].name); ret = -2; }
		if(param_list[n].comment) fprintf(out, "\t\t\t# %s", param_list[n].comment);
		fprintf(out, "\n");
	}
}


/**************************************************\
Lit/charge la configuration depuis un fichier.
Syntaxe d'une ligne:
Commentaires: '(espaces)# Commentaires'
Ligne vide: '(espaces)'
Param�res:
   '(espaces)NOM_PARAM = entier   # Commentaire'
   '(espaces)NOM_PARAM = r�l     # Commentaire'
\**************************************************/
int CParameter::ReadConfiguration(char* path)
{
	printf("Reading Config '%s' ...\n", path);

    char ligne[500];
	FILE *inC;

	// Lecture du fichier de config (CFG)

	if((inC = fopen(path, "r")) == NULL)
	{
		fprintf(stderr, "Error while reading the config file '%s' !\n", path);
		return -1;
	}

	// Extraction des informations ligne par ligne

	while(1)
	{
		if( fgets( ligne, 500, inC ) == NULL)
		{
			if(feof(inC)) break;
			return -2;
		}
		else
		{
			ligne[strlen(ligne)-strlen("\n")]=0;
			TraitementLigne(ligne);
		}
	}
	fclose( inC );
   
	return 0;
}



/**************************************************\
Traitement d'une ligne de fichier de config du type:
PARAM_NAME = PARAM_VALUE
Prend la ligne �traiter et si l'attribut recherch�est pr�ent dans la cha�e, enregistre sa valeur
�l'aide de 'param_list', sinon ne fait rien. Une
ligne commen�nt par '#' est consid�� comme commentaire.
On peut mettre des commentaires apr� la ligne, s'ils
sont s�ar� par un '#', mais ils ne doivent pas
contenir un nouveau '#'.
\**************************************************/
void CParameter::TraitementLigne(char *ligne)
{
    char *lig = ligne;

    while(lig[0]==' ') *(lig)++;
	if(strlen(lig)==0) return;	// Ligne vide
	if(lig[0]=='#') return;		// Commentaire

	// Passe ' ... = ... # Commentaire'
	char* seps = "=#";
	char* token = strtok(lig, seps);
	if(!token) return;					// Ligne invalide
	char* pname = token;
	token = strtok(NULL, seps);
	if(!token) return;					// Ligne invalide
	char* pvalue = token;
	token = strtok(NULL, "#");
	char* pcomment = token;				// NULL si inexistant

	pname = TrimSpaces(pname);
	pvalue = TrimSpaces(pvalue);
	if(pcomment) pcomment = TrimSpaces(pcomment);

	int n = GetIndexParameter(pname);
	if(n<0)
	{
		fprintf(stderr, "The parameter '%s' does not exist !\n", pname);
		return;					// Pas cet attribut
	}

	if(param_list[n].type == INTEGER_VALUE) *((int*)(param_list[n].param)) = atoi(pvalue);
	else if(param_list[n].type == UINTEGER_VALUE) *((unsigned int*)(param_list[n].param)) = atoi(pvalue);
	else if(param_list[n].type == DOUBLE_VALUE) *((double*)(param_list[n].param)) = atof(pvalue);
	else if(param_list[n].type == STRING_VALUE) strcpy( ((char*)param_list[n].param), pvalue );
	else fprintf(stderr, "The parameter '%s' has a type not recognized !\n", pvalue);
	if(pcomment) param_list[n].comment = strdup(pcomment);
	else param_list[n].comment = NULL;
}



/**************************************************\
Supprime les espaces et les tabulations avant et
apr� la cha�e.
\**************************************************/
char* CParameter::TrimSpaces(char *ligne)
{
	while(strchr(" \t", *ligne)) ligne++;
	int l;
	for(l=strlen(ligne)-1;l>=0;l--)
		if(strchr(" \t", ligne[l])) ligne[l] = 0;
		else break;
	return ligne;
}


/**************************************************\
Renvoit le nombre de param�res.
\**************************************************/
int CParameter::GetNumberParameters(void)
{
	return param_list.size();
}

/**************************************************\
Renvoit le nom du param�re N.
\**************************************************/
char* CParameter::GetNameParameter(int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		return NULL;
	}
	return param_list[n].name;
}

/**************************************************\
Renvoit le type du param�re N.
\**************************************************/
DataTypeValue CParameter::GetTypeParameter(int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		return MISSING_BOOL_VALUE;
	}
	return param_list[n].type;
}

/**************************************************\
Renvoit le commentaire du param�re N.
\**************************************************/
char* CParameter::GetCommentParameter(int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		return NULL;
	}
	return param_list[n].comment;
}

/**************************************************\
Renvoit le pointeur du param�re N.
\**************************************************/
void* CParameter::GetPointerParameter(int n)
{
	if(n>=param_list.size())
	{
		fprintf(stderr, "The parameter number %d does not exist !\n", n);
		return NULL;
	}
	return param_list[n].param;
}

/**************************************************\
Importe un autre objet CParameter (duplication).
Note: ne pas utiliser l'op�ateur '=' car cela copie mal les pointeurs
\**************************************************/
void CParameter::Import(CParameter *P)
{
	RegisterAll();
	used = P->used;

	int n;
	void *value = malloc(MAX_PARAMETER_SIZE);

	for(n=0;n<P->param_list.size();n++)
	{
		P->GetValueParameter(value, n);
		SetValueParameter(value, n);
	}

	free(value);
}

/**************************************************\
Param�res globaux.
\**************************************************/
CParameter PARA;
