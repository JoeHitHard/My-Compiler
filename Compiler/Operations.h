#include "Structures.h" 
char getLower(char c);
int isRegister(char *c){
	int i = 0;
	while (c[i] != '\0'){
		c[i] = getLower(c[i]);
		i++;
	}
	if (i != 2||c[1]!='x'){
		return -1;
	}
	return c[i]-'a';
}
int readFileForData(ST *symbolTable , FILE **pf){
	char ch = fgetc(*pf);
	int iVname = 0;
	int maxVname = 32;
	char *Vname = (char*)malloc(sizeof(char) * maxVname);
	int size=1;
	while (ch != EOF && ch != '\n'){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		if (ch == ']'){
			Vname[iVname] = '\0';
			printf("\nInvalid Format of Data %s", Vname);
			return -1;
		}
		if (ch == '['){
			fscanf(*pf, "%d", &size);
			ch = fgetc(*pf);
			if (ch != ']'){
				Vname[iVname] = '\0';
				printf("\nInvalid Format of Data %s", Vname);
				return -1;
			}
			else{
				ch = fgetc(*pf);
				break;
			}
		}
		Vname[iVname] = ch;
		iVname = iVname + 1;
		if (iVname >= maxVname - 2){
			maxVname *= 2;
			Vname = (char*)realloc(Vname, (sizeof(char) * maxVname));
		}
		ch = fgetc(*pf);
	}
	Vname[iVname] = '\0';
	return insertIntoSymbolTable(symbolTable, Vname, size);
}
int readFileForConst(CT *constTable, FILE **pf){
	char ch = fgetc(*pf);
	int iVname = 0;
	int maxVname = 32;
	char *Vname = (char*)malloc(sizeof(char) * maxVname);
	int ConstVal = 0;
	int gotConst = 0;
	while (ch != EOF && ch != '\n'){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		if (ch == '='){
			gotConst = 1;
			fscanf(*pf, "%d", &ConstVal);
			ch = fgetc(*pf);
			break;
		}
		Vname[iVname] = ch;
		iVname = iVname + 1;
		if (iVname >= maxVname - 2){
			maxVname *= 2;
			Vname = (char*)realloc(Vname, (sizeof(char) * maxVname));
		}
		ch = fgetc(*pf);
	}
	Vname[iVname] = '\0';
	if (gotConst != 0){
		return insertIntoConstantTable(constTable, Vname, ConstVal);
	}
	else{
		printf("\nEnter Constant values for %s;", Vname);
		return -1;
	}
	return -1;

}
PNode* getParameterNode(char *d,ST *st,CT *ct,int isDes){
	int i = isRegister(d);
	if (i == -1){
		SNode *s = isThereInSymbolTable(st, d);
		if (s == NULL){
			CNode *c = isThereInConstantTable(ct, d);
			if (c == NULL){
				printf("\nInvalid Variable %s", d);
				return NULL;
			}
			if (isDes==1){
				printf("\nTrying to change Constant: %s", d);
				return NULL;
			}
			PNode *p = createPNode(c->ConstVal, 'c');
			return p;
		}
		else{
			PNode *p = createPNode(s->OffSet, 'v');
			return p;
		}
	}
	else{
		PNode *p = createPNode(i, 'r');
		return p;
	}
	return NULL;
}
int readFileForMove(CT *ct, ST *st, IML *iml,FILE **pf){
	char ch = fgetc(*pf);
	int iS = 0;
	int iD = 0;
	int maxS = 32;
	char *s = (char*)malloc(sizeof(char) * maxS);
	int maxD = 32;
	char *d = (char*)malloc(sizeof(char) * maxD);
	int ConstVal = 0;
	int gotConst = 0;
	while (ch != ','){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD*= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid Mov Format");
		return -1;
	}
	ch = fgetc(*pf);
	while (ch != EOF&&ch!='\n'){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		s[iS] = ch;
		iS = iS + 1;
		if (iS >= maxS - 2){
			maxS *= 2;
			s = (char*)realloc(s, (sizeof(char) * maxS));
		}
		ch = fgetc(*pf);
	}
	s[iS] = '\0';
	if (s[0] == '\0'){
		printf("\nInvalid Mov Format at %s",d);
		return -1;
	}
	PNode *p = getParameterNode(d, st, ct,1);
	if (p != NULL){
		PArray *pa = createPArray(iml->size);
		insertIntoPArray(pa, p);
		p = getParameterNode(s, st, ct, 0);
		if (p != NULL){
			insertIntoPArray(pa, p);
			IMLNode *newIMLNode = createIMLNode(3, pa);
			inserIntoIML(iml, newIMLNode);
			return 1;
		}
	}
	return -1;
}
int readFileForRead(CT *ct, ST *st, IML *iml, FILE **pf){
	char ch = fgetc(*pf);
	int iS = 0;
	int iD = 0;
	int maxD = 32;
	char *d = (char*)malloc(sizeof(char) * maxD);
	while (ch != EOF&&ch != '\n'&&ch != ' '){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid Mov Format");
		return -1;
	}
	PNode *p = getParameterNode(d, st, ct, 1);
	if (p != NULL){
		PArray *pa = createPArray(iml->size);
		insertIntoPArray(pa, p);
		IMLNode *newIMLNode = createIMLNode(4, pa);
		inserIntoIML(iml, newIMLNode);
		return 1;
	}
	return -1; 
}


int readFileForOperator(CT *ct, ST *st, IML *iml, FILE **pf,int op){
	char ch = fgetc(*pf);
	int iS = 0;
	int iD = 0;
	int maxS = 32;
	PArray *pa=NULL;
	pa = createPArray(iml->size);
	PNode *p=NULL;
	int maxD = 32;
	char *d = (char*)malloc(sizeof(char) * maxD);
	iD = 0;
	while (ch != ','){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		if (ch == '\n'||ch==EOF){
			printf("\n Invalid Add command");
			return -1;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid add Format");
		return -1;
	}
	p = getParameterNode(d, st, ct, 1);
	if (p != NULL){
		insertIntoPArray(pa, p);
	}
	iD = 0;
	ch = fgetc(*pf);
	while (ch != ','){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		if (ch == '\n' || ch == EOF){
			printf("\n Invalid Add command");
			return -1;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid Mov Format");
		return -1;
	}
	p = getParameterNode(d, st, ct, 0);
	if (p != NULL){
		insertIntoPArray(pa, p);
	}
	iD = 0;
	ch = fgetc(*pf);
	while (ch != ','&&ch!=EOF&&ch!='\n'){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		if (ch == '\n'){
			printf("\n Invalid Add command");
			return -1;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid Add Format");
		return -1;
	}
	p = getParameterNode(d, st, ct, 0);
	if (p != NULL){
		insertIntoPArray(pa, p);
	}
	IMLNode *newIMLNode = createIMLNode(op, pa);
	inserIntoIML(iml, newIMLNode);
	return 1;
}
int readFileForLabel(char *tok,BAT *bat,int insNo){
	int iD = 0;
	int maxD = 32;
	int i = 0;
	char *d = (char*)malloc(sizeof(char) * maxD);
	iD = 0;
	char ch = tok[i];
	while (tok[i] != ':'){
		if (ch == ' '||ch=='\0'||ch==EOF){
			d[iD] = '\0';
			return;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		i++;
	}
	d[iD] = '\0';
	insertIntoBlockAddressTable(bat, d, insNo);
	return 1;
}
int getIfCommandCode(char *Operation){
	int i = 0;
	while (Operation[i] != '\0'){
		Operation[i] = getLower(Operation[i]);
		i++;
	}
	if (strcmp(Operation, "eq") == 0){
		return 1;
	}
	if (strcmp(Operation, "lt") == 0){
		return 2;
	}
	if (strcmp(Operation, "gt") == 0){
		return 3;
	}
	if (strcmp(Operation, "lteq") == 0){
		return 4;
	}
	if (strcmp(Operation, "gteq") == 0){
		return 5;
	}
	return -1;
}
int readFileForIf(SN *sn, BAT *bat, CT *ct, ST *st, IML *iml, FILE **pf){
	char ex[200];
	PArray *pa = createPArray(32);
	PNode *p = NULL;
	fscanf(*pf, "%s", &ex);
	p = getParameterNode(ex, st, ct, 0);
	if (p != NULL){
		insertIntoPArray(pa, p);
	}
	else{
		printf("Unknown %s", ex);
		return -1;
	}
	fscanf(*pf, "%s", &ex);
	int pp = getIfCommandCode(ex);
	if (pp != -1){
		p = createPNode(pp, 'o');
		insertIntoPArray(pa, p);
	}
	else{
		printf("Unknown %s", ex);
		return -1;
	}
	fscanf(*pf, "%s", &ex);
	p = getParameterNode(ex, st, ct, 0);
	if (p != NULL){
		insertIntoPArray(pa, p);
	}
	else{
		printf("Unknown %s", ex);
		return -1;
	}
	fscanf(*pf, "%s", &ex);
	int i = 0;
	while (ex[i] != '\0'){
		ex[i] = getLower(ex[i]);
		i++;
	}
	if (strcmp("then", ex) == 0){
		push(sn, iml->ind);
		p = createPNode(-1,'i');
		insertIntoPArray(pa, p);
		IMLNode *i = createIMLNode(7, pa);
		inserIntoIML(iml, i);
		return 1;
	}
	else{
		printf("Unknown %s", ex);
		return -1;
	}
}
int readFileForElse(SN *sn, BAT *bat, CT *ct, ST *st, IML *iml, FILE **pf){
	int ind = pop(sn);
	if (ind == -1){
		printf("Else Without IF");
		return -1;
	}
	IMLNode *i = iml->Inodes[ind];
	PArray *p = i->Parameter;
	PNode *pn=p->pnodes[((p->ind)-1)];
	if (pn->loc == -1){
		pn->loc = iml->ind;
		PArray *pa = createPArray(3);
		insertIntoPArray(pa, createPNode(-1, 'e'));
		push(sn, iml->ind);
		IMLNode *ii = createIMLNode(8, pa);
		inserIntoIML(iml, ii);
		return 1;
	}
	else{
		printf("Else Without IF");
		return -1;
	}
}
int readFileForEndIf(SN *sn, BAT *bat, CT *ct, ST *st, IML *iml, FILE **pf){
	int ind = pop(sn);
	if (ind == -1){
		printf("Else Without IF");
		return -1;
	}
	IMLNode *i = iml->Inodes[ind];
	PArray *p = i->Parameter;
	PNode *pn = p->pnodes[((p->ind) - 1)];
	if (pn->loc == -1){
		pn->loc = iml->ind;
		return 1;
	}
	else{
		printf("EndIF Without IF");
		return -1;
	}
}

int readFileForJump(BAT *bat, CT *ct, ST *st, IML *iml, FILE **pf){
	char ex[200];
	PArray *pa = createPArray(32);
	PNode *p = NULL;
	fscanf(*pf, "%s", &ex);
	BATNode *bt= isThereInBlockAddressTable(bat, ex);
	if (bt == NULL){
		printf("\n Unknown Label : %s",ex);
		return -1;
	}
	p = createPNode(bt->insNo, 'l');
	insertIntoPArray(pa, p);
	IMLNode *ii = createIMLNode(10, pa);
	inserIntoIML(iml, ii);
	return 1;
}


int readFileForWrite(CT *ct, ST *st, IML *iml, FILE **pf){
	char ch = fgetc(*pf);
	int iS = 0;
	int iD = 0;
	int maxD = 32;
	char *d = (char*)malloc(sizeof(char) * maxD);
	while (ch != EOF&&ch != '\n'&&ch != ' '){
		if (ch == ' '){
			ch = fgetc(*pf);
			continue;
		}
		d[iD] = ch;
		iD = iD + 1;
		if (iD >= maxD - 2){
			maxD *= 2;
			d = (char*)realloc(d, (sizeof(char) * maxD));
		}
		ch = fgetc(*pf);
	}
	d[iD] = '\0';
	if (d[0] == '\0'){
		printf("\nInvalid Mov Format");
		return -1;
	}
	PNode *p = getParameterNode(d, st, ct,0);
	if (p != NULL){
		PArray *pa = createPArray(iml->size);
		insertIntoPArray(pa, p);
		IMLNode *newIMLNode = createIMLNode(11, pa);
		inserIntoIML(iml, newIMLNode);
		return 1;
	}
	return -1;
}