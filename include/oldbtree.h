#ifndef BTREE_H_
#define BTREE_H_

#include "funcoes-gerais.h"
#include "linha.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INDEXED_KEYS 4

typedef struct BTreeNode BTreeNode_t;

typedef struct BTreeHeader BTreeHeader_t;

typedef long long int ll;

struct BTreeHeader {
  char status;
  // nodeRoot
  int noRaiz;
  // RRNnextNode
  int RRNProxNo;
  // This guy is not important, ignore this
  char lixo[69];
};

void BTreeHeaderCreate(s_file_t *indexFile);
void BtreeInsertion(s_file_t *indexFile, int codLinha, ll offsetRegistro,
                    char removido);
void BtreeCreate(s_file_t *indexFile);
void InOrderTraversal(int RRNroot, s_file_t *indexFile);
BTreeHeader_t *BTreeReadHeader(s_file_t *indexFile);
void BTreeWriteHeader(BTreeHeader_t *header, s_file_t *indexFile);
#endif
