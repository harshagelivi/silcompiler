%{
	#include<stdio.h>
	#include<stdlib.h>
	void yyerror(char *);
	extern int yylineno;
	struct node{
		char * op;
		int * val;
		struct node * left, * rite;
	};
	struct node * root;
	struct node * opnode(char c,struct node * l,struct node * r);
	struct node * mnode(int val);
	void print(struct node * t);
%}
%union{
	struct node * nd;
	int n;
	char c;
	char * ch;
}
%token NUM OP PLUS PDT NL UMINUS
%left PLUS
%left PDT
%nonassoc UMINUS
%type <n> NUMB
%type <nd> E
%%

start: 	E NL {root=$1;print(root);printf("\n");return 0;}
		;
E: 		NUMB 			{$$=mnode($1);}
		| E PLUS E 		{$$=opnode('+',$1,$3);}
		| E PDT E			{$$=opnode('*',$1,$3);}
		| '(' E ')'			{$$=$2;}
		;
		
NUMB:	NUM			{$$=yylval.n;}
		| UMINUS NUMB	{$$=-$2;}
		;
%%
void yyerror(char *s) {
    fprintf(stderr, "line %d: %s\n", yylineno, s);
}
int main(void){
	yyparse();
	return 0;
}
struct node * mnode(int val){
	struct node * t;
	t=(struct node *)malloc(sizeof(struct node));
	t->left=NULL;
	t->rite=NULL;
	t->op=NULL;
	t->val=malloc(sizeof(int));
	*(t->val)=val;
	return t;
}
struct node * opnode(char c,struct node * l,struct node * r){
	struct node * t;
	t=(struct node *)malloc(sizeof(struct node));
	t->left=l;
	t->rite=r;
	t->op=malloc(sizeof(char));
	*(t->op)=c;
	t->val=NULL;
	return t;
}
void print(struct node * t){
	if(t->op==NULL){
		printf(" %d ",*t->val);
	}else{
		printf(" %c ",*t->op);
		print(t->left);
		print(t->rite);
	}
}
