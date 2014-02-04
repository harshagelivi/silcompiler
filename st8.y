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
%token INT PLUS PDT NL MINUS DIV ID STMT DELIM ASGN READ WRITE ENDIF IF THEN ELSE WHILE ENDWHILE DO BOOL VOID LT LE GT GE EQ NEQ SLIST CTRUE CFALSE DECL ENDDECL COMMA PBEGIN PEND NUM ABOOL
%nonassoc LT GT EQ LE GE NEQ
%left PLUS MINUS
%left PDT DIV

%type <nd> E stmt slist start IDE
%type <n> NUMB ABOOL
%type <ch> ID
%type <Gsyl> declarations dlist indiv idlist d_indiv
%%
start: declarations PBEGIN slist PEND		{code_gen_aux($3);return 0;}
		;
declarations: DECL dlist ENDDECL {$$=$2;}
		;
dlist:	d_indiv dlist 			{;}
		| d_indiv				{;}
		;
d_indiv: 	INT idlist DELIM		{$$=put_type($2,INT);Ginstall($$);}
		| BOOL idlist DELIM	{$$=put_type($2,BOOL);Ginstall($$);}
		;
idlist:	indiv COMMA idlist		{$1->NEXT=$3;$$=$1;}
		|indiv				{$$=$1;tail_id=$$;}
		;
indiv:	ID '[' E ']'				{$$=make_Gentry($1, -1, eval($3), NULL);}
		| ID					{$$=make_Gentry($1, -1, 1, NULL );}
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
E: 		NUMB 			{$$=mnode(INT, INT, $1, NULL, NULL, NULL, NULL, NULL);}
		|ABOOL			{$$=mnode(BOOL,BOOL,$1,NULL,NULL,NULL,NULL,NULL);}
		| IDE			{$$=$1;}
		| E PLUS E 		{$$=mnode(INT, PLUS, 0, NULL, NULL, $1, $3, NULL);}
		| E MINUS E		{$$=mnode(INT, MINUS, 0, NULL, NULL, $1, $3, NULL);}
		| E PDT E			{$$=mnode(INT, PDT, 0, NULL, NULL, $1, $3, NULL);}
		| E DIV E			{$$=mnode(INT, DIV, 0, NULL, NULL, $1, $3, NULL);}		
		| '(' E ')'			{$$=$2;}
		| E LT E			{$$=mnode(BOOL, LT, 0, NULL, NULL, $1, $3, NULL);}
		| E GT E			{$$=mnode(BOOL, GT, 0, NULL, NULL, $1, $3, NULL);}
		| E EQ E			{$$=mnode(BOOL, EQ, 0, NULL, NULL, $1, $3, NULL);}
		| E LE E			{$$=mnode(BOOL, LE, 0, NULL, NULL, $1, $3, NULL);}
		| E GE E			{$$=mnode(BOOL, GE, 0, NULL, NULL, $1, $3, NULL);}
		| E NEQ E		{$$=mnode(BOOL, NEQ, 0, NULL, NULL, $1, $3, NULL);}
		;
IDE:		ID				{$$=mnode(VOID, ID, 0, $1, NULL, zero_node, NULL, NULL);}
		| ID '[' E ']'        	{$$=mnode(VOID, ID, 0, $1, NULL, $3, NULL, NULL);}
		;
NUMB:        NUM                        {$$=yylval.n;}
                | MINUS NUMB        {$$=-$2;}
                ;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
#include "code_gen.h"
int main(void){
	zero_node=mnode(INT,INT,0,NULL, NULL, NULL, NULL, NULL);
        yyparse();
        return 0;
}
#include "make_node.h"
#include "print.h"
#include "evaluate.h"

