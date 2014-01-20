%{
        #include<stdio.h>
        #include<stdlib.h>
	struct node {
		int TYPE;		/* token value as of nw */
		int NODETYPE; /* for nw token numbers are used */
		char* NAME; /* for functions/identifiers */
		int VALUE; /* for constants */
		struct node *arglist;
		struct node *ptr1, *ptr2, *ptr3;
	};
        void yyerror(char *);
        struct node * mnode(int TYPE, int NODETYPE, int VALUE, char* NAME, struct node * arglist, struct node * ptr1, struct node *  ptr2, struct node *  ptr3);
        extern int yylineno;
        int var[26];
        void print(struct node * nd);
        int eval(struct node * nd);
        struct node * root;
        int ger;
        int reg_count=-1,label_count=0;

%}
%union{
	struct node * nd;
        int n;
        char c;
        char * ch;
}
%token NUM PLUS PDT NL MINUS DIV ID STMT DELIM ASGN READ WRITE ENDIF IF THEN ELSE WHILE ENDWHILE DO BOOL VOID LT GT EQ SLIST CTRUE CFALSE
%nonassoc LT GT EQ
%left PLUS MINUS
%left PDT DIV

%type <nd> E stmt slist start
%type <n> NUMB
%type <ch> ID
%%
start: slist NL					{printf("START\n");code_gen($$);printf("HALT\n");return 0;}
	;
slist:	stmt				{$$=mnode(VOID, STMT, 0, NULL, NULL, $1, NULL, NULL);}
	|	stmt slist				{$$=mnode(VOID, SLIST, 0, NULL, NULL, $1, $2, NULL);}
	;
stmt:	ID ASGN E DELIM		{yylineno++;
							root=mnode(VOID, ID, 0, $1, NULL, NULL, NULL, NULL);
							$$=mnode(VOID, ASGN, 0, NULL, NULL, root, $3, NULL);
							}
		| READ '(' ID ')' DELIM	{yylineno++;
							root=mnode(VOID, ID, 0, $3, NULL, NULL, NULL, NULL);
							$$=mnode(VOID, READ, 0, NULL, NULL, root, NULL, NULL);
							}
		| WRITE '(' E ')' DELIM	{yylineno++;$$=mnode(VOID, WRITE, 0, NULL, NULL, $3, NULL, NULL);}
		| IF E THEN slist ELSE slist ENDIF DELIM	{yylineno++;$$=mnode(VOID, IF, 0, NULL, NULL, $2, $4, $6);}
		| WHILE E DO slist ENDWHILE DELIM		{yylineno++;$$=mnode(VOID, WHILE, 0, NULL, NULL, $2, $4, NULL);}

		;
E: 		NUMB 			{$$=mnode(NUM, NUM, $1, NULL, NULL, NULL, NULL, NULL);}
		| ID				{$$=mnode(NUM, ID, 0, yylval.ch, NULL, NULL, NULL, NULL);}
		| E PLUS E 		{$$=mnode(NUM, PLUS, 0, NULL, NULL, $1, $3, NULL);}
		| E MINUS E		{$$=mnode(NUM, MINUS, 0, NULL, NULL, $1, $3, NULL);}
		| E PDT E			{$$=mnode(NUM, PDT, 0, NULL, NULL, $1, $3, NULL);}
		| E DIV E			{$$=mnode(NUM, DIV, 0, NULL, NULL, $1, $3, NULL);}		
		| '(' E ')'			{$$=$2;}
		| E LT E			{$$=mnode(BOOL, LT, 0, NULL, NULL, $1, $3, NULL);}
		| E GT E			{$$=mnode(BOOL, GT, 0, NULL, NULL, $1, $3, NULL);}
		| E EQ E			{$$=mnode(BOOL, EQ, 0, NULL, NULL, $1, $3, NULL);}
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
        yyparse();
        return 0;
}
struct node * mnode(int TYPE, int NODETYPE, int VALUE, char* NAME, struct node * arglist, struct node * ptr1, struct node *  ptr2, struct node *  ptr3){
	if(NODETYPE==IF && ptr1->TYPE!=BOOL){
		yyerror("bool expected for if");
		return NULL;
	}
	if(NODETYPE==WHILE && ptr1->TYPE!=BOOL){
		yyerror("bool expected for while");
		return NULL;
	}
	struct node * t;
	t=(struct node *)malloc(sizeof(struct node));
	t->TYPE=TYPE;
	t->NODETYPE=NODETYPE;
	t->VALUE=VALUE;
	if(NAME!=NULL){
		t->NAME=(char *)malloc(sizeof(char)*50);
		t->NAME=NAME;
	}
	if(arglist!=NULL){
		t->arglist=(struct node *)malloc(sizeof(struct node));
		t->arglist=arglist;
	}
	if(ptr1!=NULL){
		t->ptr1=(struct node *)malloc(sizeof(struct node));
		t->ptr1=ptr1;
	}
	if(ptr2!=NULL){
		t->ptr2=(struct node *)malloc(sizeof(struct node));
		t->ptr2=ptr2;
	}
	if(ptr3!=NULL){
		t->ptr3=(struct node *)malloc(sizeof(struct node));
		t->ptr3=ptr3;
	}
	return t;
}
void print(struct node * nd){
	if(nd!=NULL){
		printf("%d   %d    %d\n",nd->TYPE,nd->NODETYPE,nd->VALUE);
		print(nd->ptr1);
		print(nd->ptr2);
		print(nd->ptr3);
	}
}

