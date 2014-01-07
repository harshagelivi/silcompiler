%{
        #include<stdio.h>
        #include<stdlib.h>
        void yyerror(char *);
        extern int yylineno;
        int * var[26];
%}
%union{
        int n;
        char c;
        char * ch;
}
%token NUM PLUS PDT NL MINUS DIV ID DELIM ASGN READ WRITE
%left PLUS MINUS
%left PDT DIV
%right ASGN

%type <n> NUMB E stmt slist ID
%%
start: slist NL				{return 0;}
	;
slist:	stmt
	|	slist stmt
	;
stmt:	ID ASGN E DELIM		{var[$1-'a']=malloc(sizeof(int));*var[$1-'a']=$3;}
		| READ '(' ID ')' DELIM	{var[$3-'a']=malloc(sizeof(int));scanf("%d",var[$3-'a']);}
		| WRITE '(' E ')' DELIM	{printf("%d\n",$3);}
		;
E:		NUMB                         {$$=($1);}
                | E PLUS E                 {$$=$1+$3;}
                | E MINUS E                {$$=$1-$3;}                
                | E PDT E                        {$$=$1*$3;}
                | E DIV E                        {$$=$1/$3;}                
                | '(' E ')'                        {$$=$2;}
                | ID				{if(var[yylval.c-'a']==NULL){ printf("undeclared variable\n"); return 0; } else $$=*var[yylval.c-'a'];}	
                ;
                
NUMB:        NUM                        {$$=yylval.n;}
                | MINUS NUMB        {$$=-$2;}
                ;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
int main(void){
	int i=0;
	for(;i<26;i++)
		var[i]=NULL;
        yyparse();
        return 0;
}

