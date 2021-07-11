#include "../include/btree.h"
#include <stdio.h>
#include <string.h>

struct BTreeNode {
  // leaf
  char folha;
  // numberOfIndexedKeys
  int nroChavesIndexadas;
  // RRNnode
  int RRNdoNo;
  // Set of keys
  int key[4];
  // Set of references to other file
  ll dataRegReference[4];
  // Subtree pointers
  int pointers[5];
};

void BTreeWriteHeader(BTreeHeader_t *header, s_file_t *indexFile) {
  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }
  fseek(indexFile->fp, 0, SEEK_SET);
  fwrite(&header->status, sizeof(char), 1, indexFile->fp);
  fwrite(&header->noRaiz, sizeof(int), 1, indexFile->fp);
  fwrite(&header->RRNProxNo, sizeof(int), 1, indexFile->fp);
  fwrite(header->lixo, sizeof(char), 68, indexFile->fp);
}

BTreeHeader_t *BTreeReadHeader(s_file_t *indexFile) {
  if (indexFile->fp == NULL || indexFile->consistenciaDoArquivo == '0') {
    printf("Processing file failed.");
    return NULL;
  }
  BTreeHeader_t *header = (BTreeHeader_t *)calloc(1, sizeof(BTreeHeader_t));
  fseek(indexFile->fp, 0, SEEK_SET);
  fread(&header->status, sizeof(char), 1, indexFile->fp);
  fread(&header->noRaiz, sizeof(int), 1, indexFile->fp);
  fread(&header->RRNProxNo, sizeof(int), 1, indexFile->fp);
  fread(header->lixo, sizeof(char), 68, indexFile->fp);
  return header;
}

void BTreeWriteNode(BTreeNode_t *WriteNode, s_file_t *indexFile) {
  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }

  fseek(indexFile->fp, 77 * (WriteNode->RRNdoNo + 1), SEEK_SET);
  fwrite(&WriteNode->folha, sizeof(char), 1, indexFile->fp);
  fwrite(&WriteNode->nroChavesIndexadas, sizeof(int), 1, indexFile->fp);
  fwrite(&WriteNode->RRNdoNo, sizeof(int), 1, indexFile->fp);
  for (int i = 0; i < MAX_INDEXED_KEYS; i++) {
    fwrite(&WriteNode->pointers[i], sizeof(int), 1, indexFile->fp);
    fwrite(&WriteNode->key[i], sizeof(int), 1, indexFile->fp);
    fwrite(&WriteNode->dataRegReference[i], sizeof(ll), 1,
           indexFile->fp);
  }
  fwrite(&WriteNode->pointers[MAX_INDEXED_KEYS], sizeof(int), 1, indexFile->fp);
}

BTreeNode_t *BTreeReadNode(s_file_t *indexFile, int RRNnode) {
  if (indexFile->fp == NULL || indexFile->consistenciaDoArquivo == '0') {
    printf("Processing file failed.");
    return NULL;
  }
  BTreeNode_t *ReadNode = (BTreeNode_t *)calloc(1, sizeof(BTreeNode_t));
  fseek(indexFile->fp, 77 * (RRNnode + 1), SEEK_SET);
  fread(&ReadNode->folha, sizeof(char), 1, indexFile->fp);
  fread(&ReadNode->nroChavesIndexadas, sizeof(int), 1, indexFile->fp);
  fread(&ReadNode->RRNdoNo, sizeof(int), 1, indexFile->fp);
  for (int i = 0; i < MAX_INDEXED_KEYS; i++) {
    fread(&ReadNode->pointers[i], sizeof(int), 1, indexFile->fp);
    fread(&ReadNode->key[i], sizeof(int), 1, indexFile->fp);
    fread(&ReadNode->dataRegReference[i], sizeof(ll), 1, indexFile->fp);
  }
  fread(&ReadNode->pointers[MAX_INDEXED_KEYS], sizeof(int), 1, indexFile->fp);
  return ReadNode;
}

// Creating B-Tree Header
void BTreeHeaderCreate(s_file_t *indexFile) {
  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }
  BTreeHeader_t *header = (BTreeHeader_t *)calloc(1, sizeof(BTreeHeader_t));
  header->RRNProxNo = 0;
  header->noRaiz = -1;
  header->status = '0';
  strcpy(
      header->lixo,
      "@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
  BTreeWriteHeader(header, indexFile);
}

//                    INSERTION
// =========================================================================

void cleanNode(BTreeNode_t *node) {
  for (int i = 0; i < MAX_INDEXED_KEYS; i++) {
    node->key[i] = node->dataRegReference[i] = node->pointers[i] = -1;
  }
  node->pointers[MAX_INDEXED_KEYS] = -1;
  node->nroChavesIndexadas = 0;
}

BTreeNode_t *createNode(int RRNdoNo, char folha) {
  BTreeNode_t *newNode = (BTreeNode_t *)calloc(1, sizeof(BTreeNode_t));
  newNode->RRNdoNo = RRNdoNo;
  newNode->folha = folha;
  newNode->nroChavesIndexadas = 0;
  cleanNode(newNode);

  return newNode;
}

void InsertInNode(BTreeNode_t *root, s_file_t *indexFile, int key, ll dataRegReference, int pointerLeft, int pointerRight) {
  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }

  if(root->nroChavesIndexadas > MAX_INDEXED_KEYS){
    printf("Aqui!");
  }

  /*
  for(int i = 0; i < root->nroChavesIndexadas; i++){
    if(key.key == root->chaves[i].key) return;
  }
  */

  int nroChavesIndexadasdoNo = root->nroChavesIndexadas - 1;
  while (nroChavesIndexadasdoNo >= 0 && key < root->key[nroChavesIndexadasdoNo]) {
    // Shifting keys and dataRegReference
    root->key[nroChavesIndexadasdoNo + 1] = root->key[nroChavesIndexadasdoNo];
    root->dataRegReference[nroChavesIndexadasdoNo + 1] = root->dataRegReference[nroChavesIndexadasdoNo];
    // Shifting pointers
    root->pointers[nroChavesIndexadasdoNo + 2] = root->pointers[nroChavesIndexadasdoNo + 1];
    nroChavesIndexadasdoNo--;
  }
  nroChavesIndexadasdoNo++;
  root->pointers[nroChavesIndexadasdoNo + 1] = root->pointers[nroChavesIndexadasdoNo];
  root->key[nroChavesIndexadasdoNo] = key;
  root->dataRegReference[nroChavesIndexadasdoNo] = dataRegReference;
  root->pointers[nroChavesIndexadasdoNo] = pointerLeft;
  root->pointers[nroChavesIndexadasdoNo + 1] =pointerRight;
  root->nroChavesIndexadas++;
  BTreeWriteNode(root, indexFile);
}

BTreeNode_t *SearchNode(s_file_t *indexFile, int RRNnode, int key) {
  if (indexFile->fp == NULL || indexFile->consistenciaDoArquivo == '0') {
    printf("Processing file failed.");
    return NULL;
  }

  BTreeNode_t *searchNode = BTreeReadNode(indexFile, RRNnode);

  int pos = 0;
  while (key > searchNode->key[pos] && pos < searchNode->nroChavesIndexadas)
    pos++;

  if (key == searchNode->key[pos])
    return searchNode;
  if (searchNode->folha == '1')
    return NULL;

  return SearchNode(indexFile, searchNode->pointers[pos], key);
}

void SplitChild(BTreeHeader_t *header, s_file_t *indexFile, BTreeNode_t *root, BTreeNode_t *child, int key, int dataRegReference, int pointerLeft, int pointerRight) {
  int keys[MAX_INDEXED_KEYS + 1];
  ll dataRegReferences[MAX_INDEXED_KEYS + 1];
  int pointers[MAX_INDEXED_KEYS + 2];


  cleanNode(child);
  BTreeNode_t *newNode = createNode(header->RRNProxNo, child->folha);
  header->RRNProxNo++;


  InsertInNode(child, indexFile, keys[0], dataRegReference[0], pointers[0], pointers[1]);
  InsertInNode(child, indexFile, keys[1], dataRegReference[1], pointers[1], pointers[2]);
  pointers[2] = child->RRNdoNo;
  InsertInNode(root, indexFile, keys[2], dataRegReference[2], pointers[2], newNode->RRNdoNo);
  InsertInNode(newNode, indexFile, keys[3], dataRegReference[3], pointers[3], pointers[4]);
}

void InsertNonFull(BTreeHeader_t *header, BTreeNode_t *root, s_file_t *indexFile, int key, ll dataRegReference, int pointerLeft, int pointerRight) {
  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }
  int nroChavesIndexadasdoNo = root->nroChavesIndexadas - 1;
  if (root->folha == '1') {
    InsertInNode(root, indexFile, key, dataRegReference, pointerLeft, pointerRight);
  } else {
    while (nroChavesIndexadasdoNo >= 0 && key < root->key[nroChavesIndexadasdoNo])
      nroChavesIndexadasdoNo--;
    nroChavesIndexadasdoNo++;
    BTreeNode_t *childNode = NULL;
    childNode = BTreeReadNode(indexFile, root->pointers[nroChavesIndexadasdoNo]);
    if (childNode->nroChavesIndexadas == MAX_INDEXED_KEYS) {
      SplitChild(header, indexFile, root, childNode);
    }
    if (key > root->key[nroChavesIndexadasdoNo])
        nroChavesIndexadasdoNo++;
    InsertNonFull(header, childNode, indexFile, key, dataRegReference, pointerLeft, pointerRight);
  }
}

void BtreeInsertion(s_file_t *indexFile, int codLinha, ll offsetRegistro,
                    char removido) {

  if (indexFile->fp == NULL) {
    printf("Processing file failed.");
    return;
  }
  if(removido == '0') return;
  BTreeHeader_t *header = BTreeReadHeader(indexFile);
  BTreeNode_t *root = BTreeReadNode(indexFile, header->noRaiz);

  if (root->nroChavesIndexadas == MAX_INDEXED_KEYS) {
    BTreeNode_t *newRoot = createNode(header->RRNProxNo + 1, '0');
    SplitChild(header, indexFile, newRoot, root);
    header->RRNProxNo++;
    header->noRaiz = newRoot->RRNdoNo;
    InsertNonFull(header, newRoot, indexFile, codLinha, offsetRegistro, -1, -1);
  }
  else InsertNonFull(header, root, indexFile, codLinha, offsetRegistro, -1, -1);
  BTreeWriteHeader(header, indexFile);
}

void BtreeCreate(s_file_t *indexFile) {
  BTreeHeaderCreate(indexFile);
  BTreeHeader_t *header = BTreeReadHeader(indexFile);
  BTreeNode_t *newRoot = createNode(header->RRNProxNo, '1');
  header->RRNProxNo++;
  header->noRaiz = newRoot->RRNdoNo;
  BTreeWriteHeader(header, indexFile);
  BTreeWriteNode(newRoot, indexFile);
  free(header);
  free(newRoot);
}

// ==========================================================================

//                            B-TREE TRAVERSAL
// ===========================================================================

void InOrderTraversal(int RRNroot, s_file_t *indexFile) {
  if (RRNroot == -1)
    return;
  BTreeNode_t *root = BTreeReadNode(indexFile, RRNroot);
  //printf("node: %d ", root->RRNdoNo);

  for (int i = 0; i < root->nroChavesIndexadas; i++) {
    InOrderTraversal(root->pointers[i], indexFile);
    //printf("(%d, %lld)", root->chaves[i].key, root->chaves[i].dataRegReference);
  }
  InOrderTraversal(root->pointers[root->nroChavesIndexadas], indexFile);
}