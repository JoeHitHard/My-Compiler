#include "Operations.h"
#define OPERATIONSSIZE 14
char OPERATIONS[20][100] = { "", "data", "const", "mov", "read", "add", "sub", "if", "else", "endif", "jump", "write","mul","div" };
char getLower(char c){
	if (c >= 'A'&&c <= 'Z'){
		c = c + 32;
	}
	return c;
}
int getOpCode(char *Operation){
	int i = 0;
	while (Operation[i] != '\0'){
		Operation[i] = getLower(Operation[i]);
		i++;
	}
	for (i = 0; i < OPERATIONSSIZE; i++){
		if (strcmp(OPERATIONS[i], Operation) == 0){
			return i;
		}
	}
	return -1;
}
void showSymbolTable(ST *sm){
	int i = 0;
	printf("\nSymbol Table:\n");
	for (i = 0; i < sm->ind; i++){
		SNode *s = sm->snodes[i];
		printf("%s,%d,%d\n", s->VName, s->OffSet,s->size);
	}
}

void showConstantTable(CT *ct){
	int i = 0;
	printf("\nConstant Table:\n");
	for (i = 0; i < ct->ind; i++){
		CNode *s = ct->cnodes[i];
		printf("%s,%d\n", s->VName, s->ConstVal);
	}
}

void showBATable(BAT *ct){
	int i = 0;
	printf("\nBlock Addresses Table:\n");
	for (i = 0; i < ct->ind; i++){
		BATNode *s = ct->bnodes[i];
		printf("%s,%d\n", s->VName, s->insNo);
	}
}

void showInstructionTable(IML *im){
	int i = 0;
	printf("\nInstruction Table:\n");
	for (i = 0; i < im->ind; i++){
		IMLNode *s = im->Inodes[i];
		printf("%d. %d->", i,s->OpCode);
		PArray *pa = s->Parameter;
		int j = 0;
		for (j = 0; j < pa->ind; j++){
			PNode *p = pa->pnodes[j];
			printf("%d,%c->", p->loc, p->type);
		}
		printf("\n");
	}
}
int executeCommand(CT *ConstantTable ,ST *SymbolTable,IML *InstructionTable,BAT *BlockAddressTable,SN *StackTable ,FILE *programFile , char *tok){
	int opcode = getOpCode(tok);
	int er = 0;
	switch (opcode){
	case 1:
		er=readFileForData(SymbolTable, &programFile);
		break;
	case 2:
		er = readFileForConst(ConstantTable, &programFile);
		break;
	case 3:
		er = readFileForMove(ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 4:
		er = readFileForRead(ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 5:
		er = readFileForOperator(ConstantTable, SymbolTable, InstructionTable, &programFile, 5);
		break;
	case 6:
		er = readFileForOperator(ConstantTable, SymbolTable, InstructionTable, &programFile, 6);
		break;
	case 7:
		er = readFileForIf(StackTable, BlockAddressTable, ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 8:
		er = readFileForElse(StackTable, BlockAddressTable, ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 9:
		er = readFileForEndIf(StackTable, BlockAddressTable, ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 10:
		er = readFileForJump(BlockAddressTable, ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 11:
		er = readFileForWrite(ConstantTable, SymbolTable, InstructionTable, &programFile);
		break;
	case 12:
		er = readFileForOperator(ConstantTable, SymbolTable, InstructionTable, &programFile, 12);
		break;
	case 13:
		er = readFileForOperator(ConstantTable, SymbolTable, InstructionTable, &programFile, 13);
		break;
	case -1:
		er = readFileForLabel(tok, BlockAddressTable, InstructionTable->ind);
		break;
	}
	return er;
}

void saveSymbolTable(ST *sm){
	FILE *f = fopen("ST.txt","w");
	int i = 0;
	printf("\nSymbol Table Saved;\n");
	for (i = 0; i < sm->ind; i++){
		SNode *s = sm->snodes[i];
		fprintf(f, "$%d,%d,%s\n", s->OffSet, s->size, s->VName);
	}
	fclose(f);
}

void saveConstantTable(CT *ct){
	FILE *f = fopen("CT.txt", "w");
	int i = 0;
	printf("\nConstant Table Saved;\n");
	for (i = 0; i < ct->ind; i++){
		CNode *s = ct->cnodes[i];
		fprintf(f, "$%d,%s\n", s->ConstVal, s->VName);
	}
	fclose(f);
}

void saveBATable(BAT *ct){
	FILE *f = fopen("BAT.txt", "w");
	int i = 0;
	printf("\nBlock Addresses Table Saved;\n");
	for (i = 0; i < ct->ind; i++){
		BATNode *s =ct->bnodes[i];
		fprintf(f, "$%d,%s\n", s->insNo, s->VName);
	}
	fclose(f);
}

void saveInstructionTable(IML *im){
	int i = 0;
	FILE *f = fopen("IT.txt", "w");
	printf("\nInstruction Table Saved;\n");
	for (i = 0; i < im->ind; i++){
		IMLNode *s = im->Inodes[i];
		fprintf(f,"$%d",s->OpCode);
		PArray *pa = s->Parameter;
		int j = 0;
		for (j = 0; j < pa->ind; j++){
			PNode *p = pa->pnodes[j];
			fprintf(f,"-%d,%c", p->loc, p->type);
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int interpret(char *FileName){
	CT *ConstantTable = createCTNode(32);
	ST *SymbolTable = createSTNode(32);
	IML *InstructionTable = createIMLArray(32);
	BAT *BlockAddressTable = createBAT(32);
	SN *StackTable = createStackNode(32);
	FILE *programFile=NULL;
	programFile=fopen(FileName, "r+");
	if (programFile == NULL){
		printf("Enter Valid File Name.");
		return;
	}
	int opcode;
	int iTok = 0;
	int er = 0, err = 1;;
	int tokSize=32;
	char *tok = (char*)malloc(sizeof(char) * tokSize);
	char ch = fgetc(programFile);
	while (ch != EOF){
		if (ch == ' '||ch == '\n'){
			tok[iTok] = '\0';
			er=executeCommand(ConstantTable,SymbolTable,InstructionTable,BlockAddressTable,StackTable,programFile, tok);
			iTok = 0;
			ch = fgetc(programFile);
			continue;
		}
		tok[iTok] = ch;
		iTok = iTok + 1;
		if (iTok >= tokSize - 2){
			tokSize *= 2;
			tok = (char*)realloc(tok, (sizeof(char) * tokSize));
		}
		ch = fgetc(programFile);
		if (er == -1){
			err = -1;
		}
	}
	tok[iTok] = '\0';
	er=executeCommand(ConstantTable, SymbolTable, InstructionTable, BlockAddressTable, StackTable, programFile, tok);
	if (er == -1 || err == -1){
		return -1;
	}
	saveBATable(BlockAddressTable);
	saveConstantTable(ConstantTable);
	saveSymbolTable(SymbolTable);
	saveInstructionTable(InstructionTable);
	int k = 0;
	return 1;
}