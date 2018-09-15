#include <stdio.h>
#include "Execute.h"
int main(){
	if (interpret("ifTest.txt") != -1){
		BAT *b = readBATFile();
		CT *c = readCTFile();
		ST *s = readSTFile();
		IML *i = readIMLFile();
		MP* mp = createMP();
		execute(b, c, s, i,mp);
		system("pause");
	}
	return 1;
}