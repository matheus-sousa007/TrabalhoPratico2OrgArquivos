#ifndef VEICULO_H_
#define VEICULO_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include "../include/funcoes-gerais.h"

typedef struct header_veic headv_t;
typedef struct reg_veic regv_t;
int writeBin(char* filenamecsv,char* filenamebin);
int insert_num_reg(int num_ins,char*filenamebin);
void printBin(char*filename);
int searchPrint(char* filename, char* campo);

#endif