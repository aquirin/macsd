#include <stdio.h>
#include <stdlib.h>
#include "memory_allocation.h"
#include <string.h>
#include "global_define.h"

float** open_netfile(char* filename, int* nnodes);
void open_net(FILE* fich, int nnodes, float** mpesos);
int is_symmetrical(int nnodes, float** mpesos);
void save_net(int nnodes, float** pfnet, FILE* fich, int type);
