#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct Parameters{
	int loc;
	char type;
}PNode;
PNode* createPNode(int loc,char type){
	PNode *node = (PNode*)malloc(sizeof(PNode));
	node->loc = loc;
	node->type = type;
	return node;
}
typedef struct ParametersArray{
	PNode **pnodes;
	int ind;
	int size;
}PArray;
PArray** createPArray(int size){
	PArray *node = (PArray*)malloc(sizeof(PArray));
	node->pnodes = (PNode**)malloc(sizeof(PNode*)*size);
	node->ind = 0;
	node->size = size;
	return node;
}
PArray** reAllocPArray(PArray *node,int size){
	node->pnodes = (PNode**)realloc(node->pnodes,sizeof(PNode*)*size);
	node->size = size;
	return node;
}
void insertIntoPArray(PArray *p,PNode *par){
	if (p->ind >= p->size - 2){
		p->size *= 2;
		p = reAllocPArray(p,p->size);
	}	
	p->pnodes[p->ind] = par;
	p->ind = p->ind + 1;
	return;
}
typedef struct InterMediateLanguage{
	int OpCode;
	PArray *Parameter;
}IMLNode;
IMLNode* createIMLNode(int OpCode, PArray *Parameter){
	IMLNode *node = (IMLNode*)malloc(sizeof(IMLNode));
	node->Parameter = Parameter;
	node->OpCode = OpCode;
	return node;
}
IMLNode** createIMLNodeArray(int size){
	IMLNode **nodeArray = (IMLNode**)malloc(sizeof(IMLNode*)*size);
	return nodeArray;
}
IMLNode** reAllocateIMLNodeArray(IMLNode** node, int size){
	IMLNode **nodeArray = (IMLNode**)realloc(node,sizeof(IMLNode*)*size);
	return nodeArray;
}
typedef struct InterMediateLanguageArray{
	IMLNode **Inodes;
	int ind;
	int size;
}IML;
IML* createIMLArray(int size){
	IML *node = (IML*)malloc(sizeof(IML));
	node->Inodes = createIMLNodeArray(size);
	node->size = size;
	node->ind = 0;
	return node;
}
IML* reAllocateIMLArray(int size){
	IML *node = (IML*)malloc(sizeof(IML));
	node->Inodes = reAllocateIMLNodeArray(node->Inodes,size);
	node->size = size;
	return node;
}
void inserIntoIML(IML *node, IMLNode *i){
	if (node->ind >= node->size - 2){
		node->size *= 2;
		node = reAllocateIMLNodeArray(node->size, node);
	}
	node->Inodes[node->ind] = i;
	node->ind = node->ind + 1;
	return;
}
typedef struct SymbolNode{
	char *VName;
	int OffSet;
	int size;
}SNode;
SNode* createSNode(char *VName, int OffSet,int size){
	SNode *node = (SNode*)malloc(sizeof(SNode));
	node->VName = (char*)malloc((sizeof(char)*strlen(VName)) + 10);
	strcpy(node->VName, VName);
	node->OffSet = OffSet;
	node->size = size;
	return node;
}
SNode** createSArray(int Size){
	SNode **node = (SNode**)malloc(sizeof(SNode*)*Size);
	return node;
}
SNode** reAllocSArray(SNode **n,int Size){
	SNode **node = (SNode**)realloc(n,sizeof(SNode*)*Size);
	return node;
}
typedef struct SymbolTable{
	SNode **snodes;
	int ind;
	int memoryOffset;
	int size;
}ST;
ST* createSTNode(int Size){
	ST *node = (ST*)malloc(sizeof(ST)*Size);
	node->snodes = createSArray(Size);
	node->memoryOffset = 0;
	node->ind = 0;
	node->size = Size;
	return node;
}
ST* reAllocSTNode(int Size,ST *node){
	node->snodes = reAllocSArray(node->snodes,Size);
	node->size = Size;
	return node;
}

SNode* isThereInSymbolTable(ST *node, char *VName){
	int i = 0;
	for (i = 0; i < node->ind; i++){
		char *s = node->snodes[i]->VName;
		int j = 0;
		int id = -1;
		while (VName[j] != '\0'&&s[j] != '\0'){
			if (VName[j] != s[j]){
				break;
			}
			j++;
		}
		if (VName[j] == '\0'&&s[j] == '\0'){
			if (node->snodes[i]->size != 1){
				return NULL;
			}
			return node->snodes[i];
		}
		if (s[j] == '\0'){
			if (VName[j]!='\0'&&VName[j] == '['){
				char *ss = &(VName[j + 1]);
				sscanf(ss,"%d", &id);
				if (id >= node->snodes[i]->size){
					return NULL;
				}
				else{
					while (VName[j] != '\0'){
						if (VName[j] == ']'){
							id = id + (node->snodes[i]->OffSet);
							SNode *s = createSNode("", id, 1);
							return s;
						}
						j++;
					}
					continue;
				}
			}
			else{
				continue;
			}
		}
		
	}
	return NULL;
}


int insertIntoSymbolTable(ST *node,char *VName,int size){
	int i = 0;
	if (isThereInSymbolTable(node, VName) !=NULL){
		printf("\nRedefination of Vatiable %s;", VName);
		return -1;
	}
	if (node->ind >= node->size - 2){
		node->size *= 2;
		node = reAllocSTNode(node->size, node);
	}
	node->snodes[node->ind] = createSNode(VName, node->memoryOffset,size);
	node->ind = node->ind + 1;
	node->memoryOffset = node->memoryOffset + size;
	return 1;
}


typedef struct ConstantNode{
	char *VName;
	char ConstVal;
}CNode;
CNode* createCNode(char *VName, int ConstVal){
	CNode *node = (CNode*)malloc(sizeof(CNode));
	node->VName = (char*)malloc((sizeof(char)*strlen(VName)) + 10);
	strcpy(node->VName, VName);
	node->ConstVal = ConstVal;
	return node;
}
CNode** createCArray(int Size){
	CNode **node = (CNode**)malloc(sizeof(CNode*)*Size);
	return node;
}
CNode** reAllocCArray(CNode **n, int Size){
	CNode **node = (CNode**)realloc(n, sizeof(CNode*)*Size);
	return node;
}
typedef struct ConstantTable{
	SNode **cnodes;
	int ind;
	int size;
}CT;
CT* createCTNode(int Size){
	CT *node = (CT*)malloc(sizeof(CT)*Size);
	node->cnodes = createSArray(Size);
	node->ind = 0;
	node->size = Size;
	return node;
}
CT* reAllocCTNode(int Size, CT *node){
	node->cnodes = reAllocSArray(node->cnodes, Size);
	node->size = Size;
	return node;
}

CNode* isThereInConstantTable(CT *node, char *VName){
	int i = 0;
	for (i = 0; i < node->ind; i++){
		if (strcmp(node->cnodes[i]->VName, VName) == 0){
			return node->cnodes[i];
		}
	}
	return NULL;
}

int insertIntoConstantTable(CT *node, char *VName, int constVal){
	int i = 0;
	if (isThereInConstantTable(node, VName) != NULL){
		printf("\nRedefination of Vatiable %s;", VName);
		return -1;
	}
	if (node->ind >= node->size - 2){
		node->size *= 2;
		node = reAllocCTNode(node->size, node);
	}
	node->cnodes[node->ind] = createCNode(VName, constVal);
	node->ind = node->ind + 1;
	return 1;
}

typedef struct BlockAddressTable{
	char *VName;
	int insNo;
}BATNode;
typedef struct BlockAddressTableArray{
	BATNode **bnodes;
	int ind;
	int size;
}BAT;
BATNode* createBATNode(char *VName, int insNo){
	BATNode *node = (BATNode*)malloc(sizeof(BATNode));
	node->VName = (char*)malloc((sizeof(char)*strlen(VName)) + 10);
	strcpy(node->VName, VName);
	node->insNo = insNo;
	return node;
}
BATNode** createBATArray(int Size){
	BATNode **node = (BATNode**)malloc(sizeof(BATNode*)*Size);
	return node;
}
BATNode** reAllocBATArray(BATNode **n, int Size){
	BATNode **node = (BATNode**)realloc(n, sizeof(BATNode*)*Size);
	return node;
}
BAT* createBAT(int Size){
	BAT *node = (BAT*)malloc(sizeof(BAT)*Size);
	node->bnodes = createBATArray(Size);
	node->ind = 0;
	node->size = Size;
	return node;
}
BAT* reAllocBATNode(int Size, BAT *node){
	node->bnodes = reAllocBATArray(node->bnodes, Size);
	node->size = Size;
	return node;
}

BATNode* isThereInBlockAddressTable(BAT *node, char *VName){
	int i = 0;
	for (i = 0; i < node->ind; i++){
		if (strcmp(node->bnodes[i]->VName, VName) == 0){
			return node->bnodes[i];
		}
	}
	return NULL;
}

int insertIntoBlockAddressTable(BAT *node, char *VName, int insNo){
	int i = 0;
	if (isThereInBlockAddressTable(node, VName) != NULL){
		printf("\nRedefination of Block %s;", VName);
		return -1; 
	}
	if (node->ind >= node->size - 2){
		node->size *= 2;
		node = reAllocBATArray(node->size, node);
	}
	node->bnodes[node->ind] = createBATNode(VName, insNo);
	node->ind = node->ind + 1;
	return 1;
}



typedef struct StackNode{
	int *arr;
	int top ;
	int size;
}SN;
SN* createStackNode(int size){
	SN *node = (SN*)malloc(sizeof(SN));
	node->arr = (int*)malloc(sizeof(int)*size);
	node->top = -1;
	node->size = size;
	return node;
}
void reAllocateStackNode(SN *node, int size){
	node->arr = (int*)realloc(node->arr,sizeof(int)*size);
	node->size = size;
	return node;
}
void push(SN *node, int val){
	if (node->top >= node->size - 2){
		node->size = node->size * 2;
		reAllocSArray(node, node->size);
	}
	node->top = node->top + 1;
	node->arr[node->top] = val;
	return;
}
int pop(SN *node){
	if (node->top <0){
		return -1;
	}
	node->top = node->top - 1;
	return node->arr[((node->top)+1)];
}
int top(SN *n){
	if (n->top == -1){
		return -1;
	}
	else{
		return n->arr[n->top];
	}
	return -1;
}

typedef struct MemoryPool{
	long int registerAddress;
	int noc;
	long int variableAddresses;
	long int constantsAddresses;
	void *memory;
}MP;
MP* createMP(){
	MP *node = (MP*)malloc(sizeof(MP));
	node->memory = (void*)malloc(sizeof(char) * 1000);
	void *v = node->memory;
	node->noc = 0;
	node->registerAddress = v;
	node->constantsAddresses =(long int)(v)+(9*sizeof(char));
	node->variableAddresses = (long int)(v)+(50 * sizeof(char));
	return node;
}