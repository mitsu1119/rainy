/* lexer.c */

char yytext[100];

int yylex() {
	int i, c, n;
	int savec;
	char *p;
skip:
	c = getc(stdin);
	savec = c;

	if(isspace(c)) goto skip;	// 空白の読み飛ばし

	// ++ や == などの単行演算子として使えるものが続いているものの処理
	c = getc(stdin);
	if(savec == c) {
		switch(savec) {
			case '=':
				return EQEQ;
		};
	}
	ungetc(c, stdin);
	c = savec;

	switch(c) {
		case '=':
		case '+':
		case '-':
		case '*':
		case '/':
		case '(':
		case ')':
		case '{':
		case '}':
		case '<':
		case '>':
		case '&':
		case '|':
		case '^':
		case ',':
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
		} else if(strcmp(yytext, "ret") == 0) {
			return RETURN;
		} else if(strcmp(yytext, "println") == 0) {
			return PRINTLN;
		} else if(strcmp(yytext, "global") == 0) {
			return GLOBAL;
		} else if(strcmp(yytext, "if") == 0) {
			return IF;
		} else if(strcmp(yytext, "while") == 0) {
			return WHILE;
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
