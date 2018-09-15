#include "Command.h"
BAT* readBATFile(){
	FILE *fp = fopen("BAT.txt", "r");
	if (fp == NULL){
		printf("Compile File Before Running\n");
		return NULL;
	}
	char vn[100];
	int ad;
	BAT *bat = createBAT(32);
	while (1){
		char ch = fgetc(fp);
		if (ch == EOF){
			showBATable(bat);
			return bat;
		}
		fscanf(fp, "%d,%s", &ad, vn);
		insertIntoBlockAddressTable(bat, vn, ad);
		ch=fgetc(fp);
		if (ch == EOF){
			showBATable(bat);
			return bat;
		}
	}
	return NULL;
}
CT* readCTFile(){
	FILE *fp = fopen("CT.txt", "r");
	if (fp == NULL){
		printf("Compile File Before Running\n");
		return NULL;
	}
	char vn[100];
	int ad;
	CT *ct = createCTNode(32);
	while (1){
		char ch = fgetc(fp);
		if (ch == EOF){
			showConstantTable(ct);
			return ct;
		}
		fscanf(fp, "%d,%s", &ad, vn);
		insertIntoBlockAddressTable(ct, vn, ad);
		ch = fgetc(fp);
		if (ch == EOF){
			showConstantTable(ct);
			return ct;
		}
	}
	return NULL;
}

ST* readSTFile(){
	FILE *f = fopen("ST.txt", "r");
	if (f == NULL){
		printf("Compile File Before Running\n");
		return NULL;
	}
	ST *st = createSTNode(32);
	int o, s;
	char vn[50];
	while (1){
		char ch = fgetc(f);
		if (ch == EOF){
			showSymbolTable(st);
			return st;
		}
		fscanf(f, "%d,%d,%s", &o, &s, vn);
		insertIntoSymbolTable(st, vn, s);
		ch = fgetc(f);
		if (ch == EOF){
			showSymbolTable(st);
			return st;
		}
	}
	fclose(f);
}

IML* readIMLFile(){
	int i = 0;
	FILE *f = fopen("IT.txt", "r");
	IML *iml = createIMLArray(32);
	int o, l;
	char t;
	char ch;
	while (1){
		ch = fgetc(f);
		if (ch == EOF){
			showInstructionTable(iml);
			return iml;
		}
		fscanf(f, "%d", &o);
		PArray *pa = createPArray(32);
		while (1){
			ch = fgetc(f);
			if (ch == '\n'){
				break;
			}
			fscanf(f, "%d,%c", &l, &t);
			PNode *p = createPNode(l, t);
			insertIntoPArray(pa, p);
		}
		IMLNode *ii = createIMLNode(o, pa);
		inserIntoIML(iml, ii);
	}
	fclose(f);
}


int execute(BAT *bat, CT *ct, ST *st, IML *iml,MP *mp){
	SN *sne = createStackNode(32);
	int i = 0;
	for (i = 0; i < iml->ind; i++){
		int opCode = iml->Inodes[i]->OpCode;
		switch (opCode){
		case 3:
			commandMOV(mp, iml->Inodes[i]->Parameter);
			break;
		case 4:
			commandREAD(mp, iml->Inodes[i]->Parameter);
			break;
		case 5:
			commandOperator(mp, iml->Inodes[i]->Parameter, 5);
			break;
		case 6:
			commandOperator(mp, iml->Inodes[i]->Parameter, 6);
			break;
		case 7:
			commandIF(mp, iml->Inodes[i]->Parameter, &i, sne);
			break;
		case 8:
			commandELSE(mp, iml->Inodes[i]->Parameter, &i, sne);
			break;
		case 10:
			commandJUMP(mp, iml->Inodes[i]->Parameter, &i);
			break;
		case 11:
			commandWRITE(mp, iml->Inodes[i]->Parameter);
			break;
		case 12:
			commandOperator(mp, iml->Inodes[i]->Parameter, 12);
			break;
		case 13:
			commandOperator(mp, iml->Inodes[i]->Parameter, 13);
			break;
		}
	}
	return 1;
}