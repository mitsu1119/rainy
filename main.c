#include <stdlib.h>
#include "AST.h"
#include <stdio.h>
#include "execute.h"

int main() {
	int r = 0;
	printf("parsing...\n");
	yyparse();
	printf("end parsing !\n");
	printf("\n>>>>>>>>>>>>>>>>>>>>>>> start running!\n\n");
	exeCall(findId("main"), NULL);
	printf("\n<<<<<<<<<<<<<<<<<<<<<<< end running!\n");
	return r;
}
