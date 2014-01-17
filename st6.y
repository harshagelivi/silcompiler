%{
#include "decls.h"
%}
%union{
	struct node * nd;
	struct Gsymbol * Gsyl;
        int n;
        char c;
        char * ch;
}
%token NUM PLUS PDT NL MINUS DIV ID STMT DELIM ASGN READ WRITE ENDIF IF THEN ELSE WHILE ENDWHILE DO BOOL VOID LT GT EQ SLIST CTRUE CFALSE DECL ENDDECL INT COMMA EOFF
%nonassoc LT GT EQ
%left PLUS MINUS
%left PDT DIV

%type <nd> E stmt slist start IDE
%type <n> NUMB
%type <ch> ID
%type <Gsyl> declarations dlist indiv idlist d_indiv
%%
start: declarations slist EOFF		{eval($2);return 0;}
		;
declarations: DECL dlist ENDDECL {$$=$2;}
		;
dlist:	d_indiv dlist 			{;}
		| d_indiv				{;}
		;
d_indiv: 	INT idlist DELIM		{$$=put_type($2,INT);add_tree($$);}
		| BOOL idlist DELIM	{$$=put_type($2,BOOL);add_tree($$);}
		;
idlist:	indiv COMMA idlist		{$1->NEXT=$3;$$=$1;}
		|indiv				{$$=$1;tail_id=$$;}
		;
indiv:	ID '[' E ']'				{$$=Ginstall($1, -1, eval($3), NULL);}
		| ID					{$$=Ginstall($1, -1, 1, NULL );}
		;
slist:	stmt				{$$=mnode(VOID, STMT, 0, NULL, NULL, $1, NULL, NULL);}
		|stmt slist			{$$=mnode(VOID, SLIST, 0, NULL, NULL, $1, $2, NULL);}
		;
stmt:	IDE ASGN E DELIM						{$$=mnode(VOID, ASGN, 0, NULL, NULL, $1, $3, NULL);}
		| READ '(' IDE ')' DELIM					{$$=mnode(VOID, READ, 0, NULL, NULL, $3, NULL, NULL);}
		| WRITE '(' E ')' DELIM					{$$=mnode(VOID, WRITE, 0, NULL, NULL, $3, NULL, NULL);}
		| IF E THEN slist ELSE slist ENDIF DELIM	{$$=mnode(VOID, IF, 0, NULL, NULL, $2, $4, $6);}
		| WHILE E DO slist ENDWHILE DELIM		{$$=mnode(VOID, WHILE, 0, NULL, NULL, $2, $4, NULL);}

		;
E: 		NUMB 			{$$=mnode(NUM, NUM, $1, NULL, NULL, NULL, NULL, NULL);}
		| IDE			{$$=$1;}
		| E PLUS E 		{$$=mnode(NUM, PLUS, 0, NULL, NULL, $1, $3, NULL);}
		| E MINUS E		{$$=mnode(NUM, MINUS, 0, NULL, NULL, $1, $3, NULL);}
		| E PDT E			{$$=mnode(NUM, PDT, 0, NULL, NULL, $1, $3, NULL);}
		| E DIV E			{$$=mnode(NUM, DIV, 0, NULL, NULL, $1, $3, NULL);}		
		| '(' E ')'			{$$=$2;}
		| E LT E			{$$=mnode(BOOL, LT, 0, NULL, NULL, $1, $3, NULL);}
		| E GT E			{$$=mnode(BOOL, GT, 0, NULL, NULL, $1, $3, NULL);}
		| E EQ E			{$$=mnode(BOOL, EQ, 0, NULL, NULL, $1, $3, NULL);}
		;
IDE:		ID				{$$=mnode(NUM, ID, 0, $1, NULL, zero_node, NULL, NULL);}
		| ID '[' E ']'        	{$$=mnode(NUM, ID, 0, $1, NULL, $3, NULL, NULL);}
NUMB:        NUM                        {$$=yylval.n;}
                | MINUS NUMB        {$$=-$2;}
                ;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
#include "code_gen.h"
int main(void){
	zero_node=mnode(NUM,NUM,0,NULL, NULL, NULL, NULL, NULL);
        yyparse();
        return 0;
}
#include "make_node.h"
#include "print.h"
#include "evaluate.h"

