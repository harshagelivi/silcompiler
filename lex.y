%{
	#include<stdio.h>
	void yyerror(char *);
	extern int yylineno;
%}
%union{
	int n;
	char c;
	char * ch;
}
%token NUM OP PLUS PDT NL UMINUS
%left PLUS
%left PDT
%nonassoc UMINUS
%type <n> E NUM
%%

start: 	E NL {printf("%d\n",$1);return 0;}
		;
E: 		NUM 		{$$=yylval.n;}
		| E PLUS E 	{$$=$1+$3;}
		| E PDT E		{$$=$1*$3;}
		| '(' E ')'		{$$=$2;}
		| UMINUS E		%prec UMINUS {$$=-$2;}
		;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
int main(void){
	yyparse();
	return 0;
}
