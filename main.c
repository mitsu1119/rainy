#include <stdlib.h>
#include "AST.h"
#include <stdio.h>
#include "execute.h"

int main() {
	int r = 0;
	printf("parsing...\n");
	yyparse();
	printf("end parsing !\n");
	exeCall(findId("main"));
	printf("end running!\n");
	return r;
}
