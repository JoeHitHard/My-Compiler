#include "Interpret.h"
void* getMemLocation(MP *mp,PNode *pn){
	void *v;
	switch (pn->type)
	{
	case 'v':
		return ((mp->variableAddresses) + ((pn->loc)*sizeof(char)));
		break;
	case 'c':
		v = mp->constantsAddresses;
		*(char*)v =(char*)pn->loc;
		return v;
		break;
	case 'r':
		return ((mp->registerAddress) + ((pn->loc)*sizeof(char)));
		break;
	default:
		return NULL;
	}
	return NULL;
}
void commandMOV(MP *mp,PArray *pa){
	int i = 0;
	PNode *pn = pa->pnodes[0];
	char *des=(char*)getMemLocation(mp,pn);
	pn = pa->pnodes[1];
	char *sou = (char*)getMemLocation(mp, pn);
	*des = *sou;
	return;
}

void commandREAD(MP *mp, PArray *pa){
	PNode *pn = pa->pnodes[0];
	char *des = (char*)getMemLocation(mp, pn);
	int i = 0;
	if (des != NULL){
		scanf("%d", &i);
		*des = i;
	}
	return;
}
void commandOperator(MP *mp, PArray *pa,int opCode){
	int i = 0;
	PNode *pn = pa->pnodes[0];
	char *des = (char*)getMemLocation(mp, pn);
	pn = pa->pnodes[1];
	char *sou1 = (char*)getMemLocation(mp, pn);
	pn = pa->pnodes[2];
	char *sou2 = (char*)getMemLocation(mp, pn);
	char s1 = *sou1;
	char s2 = *sou2;
	char d = 0;
	if (des != NULL&&sou1 != NULL&&sou2 != NULL){
		switch (opCode)
		{
		case 5:
			d = s1 + s2;
			break;
		case 6:
			d = s1 - s2;
			break;;
		case 12:
			d = (s1) * (s2);
			break;;
		case 13:
			d = s1 / s2;
			return;
		default:
			break;;
		}
	}
	des = &d;
	return;
}

void commandWRITE(MP *mp, PArray *pa){
	PNode *pn = pa->pnodes[0];
	char *des = (char*)getMemLocation(mp, pn);
	if (des != NULL){
		printf("%d\n", *des);
	}
	return;
}
void commandJUMP(MP *mp, PArray *pa, int *i){
	PNode *pn = pa->pnodes[0];
	*i = (pn->loc)-1;
	return;
}



void commandIF(MP *mp, PArray *pa, int *i,SN *sne){
	PNode *pn = pa->pnodes[0];
	char* pv1 = (char*)getMemLocation(mp, pn);
	pn = pa->pnodes[2];
	char* pv2 = (char*)getMemLocation(mp, pn);
	pn = pa->pnodes[1];
	char v1 = *pv1;
	char v2 = *pv2;
	PNode *pne = pa->pnodes[3];
	switch (pn->loc){
	case 1:
		if (v1 == v2){
			push(sne, pne->loc);
		}
		else{
			*i = pne->loc - 1;
		}
		return;
	case 2:
		if (v1 < v2){
			push(sne, pne->loc);
		}
		else{
			*i = pne->loc - 1;
		}
		return;
	case 3:
		if (v1 > v2){
			push(sne, pne->loc);
		}
		else{
			*i = pne->loc - 1;
		}
		return;
	case 4:
		if (v1 <= v2){
			push(sne, pne->loc);
		}
		else{
			*i = pne->loc - 1;
		}
		return;
	case 5:
		if (v1 >= v2){
			push(sne, pne->loc);
		}
		else{
			*i = pne->loc - 1;
		}
		return;
	default:
		break;
	}
	return;
}

void commandELSE(MP *mp, PArray *pa, int *i,SN *sne){
	if (*i == top(sne)){
		pop(sne);
		*i = pa->pnodes[0]->loc - 1;
	}
	return;
}