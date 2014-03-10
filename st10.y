%{
#include "decls.h"
%}
%union{
	struct node * nd;
	struct Gsymbol * Gsyl;
	struct arglist * arg;
        int n;
        char c;
        char * ch;
}
%token INT PLUS PDT NL MINUS DIV ID STMT DELIM ASGN READ WRITE ENDIF IF THEN ELSE WHILE ENDWHILE DO BOOL VOID LT LE GT GE EQ NEQ SLIST CTRUE CFALSE DECL ENDDECL COMMA PBEGIN PEND NUM ABOOL AMPD MAIN FUNC RETURN AND OR NOT MOD REFR
%nonassoc LT GT EQ LE GE NEQ AND OR 
%left PLUS MINUS
%left PDT DIV MOD
%nonassoc NOT

%type <nd> E stmt slist start IDE farg_list body arg_exp
%type <n> NUMB ABOOL var_par_type fun_par_type
%type <ch> ID
%type <Gsyl> declarations dlist indiv idlist d_indiv
%type <arg> fun_dlist fun_indiv fun_idlist fid aux_fun_dlist
%%
start: gdeclarations fdeflist mainbody		{return 0;}
		;
gdeclarations: declarations				{insert_main_to_global();}
		;
mainbody: INT main '{' fdeclarations body '}'   {code_gen_aux($5);}
		;
main:	MAIN						{func_name="main";}
		;
fdeflist:  								{;}
		| fdeflist fdef_ind 				{;}
		;
fdef_ind: var_par_type function_name '(' aux_fun_dlist ')' '{' fdeclarations body '}'	{fun_type_check(func_name, $1, $4);func_code_gen(func_name, $8);}
		;
fdeclarations: declarations	{put_func_decls(func_name,$1);}
		;
function_name: ID			{func_name=$1;}	
		;	
body:	PBEGIN slist PEND			{$$=$2;}
		;		
declarations: DECL dlist ENDDECL 	{$$=set_head();}
			| DECL ENDDECL		{$$=set_head();global_done();} 
			;
dlist:	d_indiv dlist 				{;}
		| d_indiv					{;}
		;
d_indiv: 	var_par_type idlist DELIM	{;}
		;
var_par_type:	INT					{var_par_type=INT;$$=INT;}
			| BOOL				{var_par_type=BOOL;$$=BOOL;}
			;				
idlist:	indiv COMMA idlist			{;}
		|indiv					{;}
		;
indiv:	ID '[' E ']'				{$$=make_Gentry($1, var_par_type, eval($3), NULL);}
		| ID					{$$=make_Gentry($1, var_par_type, 1, NULL );}
		| ID '(' aux_fun_dlist ')'	{$$=make_Gentry($1, var_par_type, 0, $3);}
		;
aux_fun_dlist: fun_dlist			{$$=set_fun_head();}
			;
fun_dlist:	fun_indiv DELIM fun_dlist		
			| fun_indiv					
			;
fun_indiv: 	fun_par_type fun_idlist			{;}
			;
fun_par_type:	INT							{fun_par_type=INT;}
			| BOOL						{fun_par_type=BOOL;}
			;			
fun_idlist:	fid COMMA fun_idlist 			{;}
			|fid							{;}
			;
fid:			ID							{$$=make_argentry($1, fun_par_type, 0);}
			| AMPD ID					{$$=make_argentry($2, fun_par_type, 1);}
			;
slist:	stmt								{$$=mnode(VOID, STMT, 0, NULL, NULL, $1, NULL, NULL);}
		|stmt slist							{$$=mnode(VOID, SLIST, 0, NULL, NULL, $1, $2, NULL);}
		;
stmt:	IDE ASGN E DELIM						{$$=mnode(VOID, ASGN, 0, NULL, NULL, $1, $3, NULL);}
		| READ '(' IDE ')' DELIM					{$$=mnode(VOID, READ, 0, NULL, NULL, $3, NULL, NULL);}
		| WRITE '(' E ')' DELIM					{$$=mnode(VOID, WRITE, 0, NULL, NULL, $3, NULL, NULL);}
		| IF E THEN slist ELSE slist ENDIF DELIM	{$$=mnode(VOID, IF, 0, NULL, NULL, $2, $4, $6);}
		| WHILE E DO slist ENDWHILE DELIM		{$$=mnode(VOID, WHILE, 0, NULL, NULL, $2, $4, NULL);}
		| RETURN E DELIM						{$$=mnode(VOID, RETURN, 0, func_name, NULL, $2, NULL, NULL );}
		;
E: 		NUMB 			{$$=mnode(INT, INT, $1, NULL, NULL, NULL, NULL, NULL);}
		|ABOOL			{$$=mnode(BOOL,BOOL,$1,NULL,NULL,NULL,NULL,NULL);}
		| IDE			{$$=$1;}
		| E PLUS E 		{$$=mnode(INT, PLUS, 0, NULL, NULL, $1, $3, NULL);}
		| E MINUS E		{$$=mnode(INT, MINUS, 0, NULL, NULL, $1, $3, NULL);}
		| E PDT E			{$$=mnode(INT, PDT, 0, NULL, NULL, $1, $3, NULL);}
		| E DIV E			{$$=mnode(INT, DIV, 0, NULL, NULL, $1, $3, NULL);}		
		| '(' E ')'			{$$=$2;}		
		| E AND E		{$$=mnode(BOOL, AND, 0, NULL, NULL, $1, $3, NULL);}
		| E OR E			{$$=mnode(BOOL, OR, 0, NULL, NULL, $1, $3, NULL);}
		| E MOD E		{$$=mnode(INT, MOD, 0, NULL, NULL, $1, $3, NULL);}
		| NOT E			{$$=mnode(BOOL, NOT, 0, NULL, NULL, $2, NULL, NULL);}
		| E LT E			{$$=mnode(BOOL, LT, 0, NULL, NULL, $1, $3, NULL);}
		| E GT E			{$$=mnode(BOOL, GT, 0, NULL, NULL, $1, $3, NULL);}
		| E EQ E			{$$=mnode(BOOL, EQ, 0, NULL, NULL, $1, $3, NULL);}
		| E LE E			{$$=mnode(BOOL, LE, 0, NULL, NULL, $1, $3, NULL);}
		| E GE E			{$$=mnode(BOOL, GE, 0, NULL, NULL, $1, $3, NULL);}
		| E NEQ E		{$$=mnode(BOOL, NEQ, 0, NULL, NULL, $1, $3, NULL);}
		| ID '(' farg_list ')' 	{$$=mnode(VOID, FUNC, 0, $1, $3, NULL, NULL, NULL);}
		| ID '('  ')'	 		{$$=mnode(VOID, FUNC, 0, $1, NULL , NULL, NULL, NULL);}
		;
farg_list:	arg_exp					{$$=$1;}		
		| farg_list COMMA arg_exp	{$1->next_arg=$3;$$=$1;}		 				
		;
arg_exp:	E				{$$=$1;}
		| AMPD IDE		{$$=mnode(VOID, REFR, 0, $2->NAME, NULL, $2, NULL, NULL);}
		;
IDE:		ID				{$$=mnode(VOID, ID, 0, $1, NULL, zero_node, NULL, NULL);}
		| ID '[' E ']'        	{$$=mnode(VOID, ID, 0, $1, NULL, $3, NULL, NULL);}
		;
NUMB:        NUM                     {$$=yylval.n;}
                | MINUS NUMB        {$$=-$2;}
                ;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
#include "code_gen.h"
int main(void){
	i=0;
	zero_node=mnode(INT,INT,0,NULL, NULL, NULL, NULL, NULL);
        yyparse();
        return 0;
}
#include "make_node.h"
#include "print.h"
#include "evaluate.h"

