/* lexer.c */

char yytext[100];

int yylex() {
	int c, n;
	char *p;
skip:
	c = getc(stdin);
	if(isspace(c)) goto skip;	// 空白の読み飛ばし
	switch(c) {
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '{':
		case '}':
		case ':':
		case ';':
		case EOF:
			return c;
	};
	// 数値
	if(isdigit(c)) {
		n = 0;
		do {
			n = n*10+c-'0';
			c = getc(stdin);
		} while(isdigit(c));
		ungetc(c, stdin);
		yylval.ast = makeNumAST(n);
		return NUM;
	}
	// ID とか (if とかも入りそう)
	if(isalpha(c)) {			// [A-Za-z][A-Za-z0-9]*
		p = yytext;
		do {
			*p++ = c;
			c = getc(stdin);
		} while(isalpha(c) || isdigit(c));
		*p = '\0';
		ungetc(c, stdin);

		// それ以外
		if(strcmp(yytext, "i32") == 0) {
			return I32;
		} else if(strcmp(yytext, "println") == 0) {
			return PRINTLN;
		} else {
			yylval.ast = makeId(yytext);
			return ID;
		}
	}
	fprintf(stderr, "invalid id '%c'\n", c);
	exit(1);
}

void yyerror() {
	printf("syntax error!!!!:(\n");
	exit(1);
}
