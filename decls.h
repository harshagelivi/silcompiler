#include<stdio.h>
#include<stdlib.h>
struct Gsymbol {
	char *NAME; // Name of the Identifier
	int TYPE; // TYPE can be INTEGER or BOOLEAN
	/***The TYPE field must be a TypeStruct if user defined types are allowed***/
	int SIZE; // Size field for arrays
	int * BINDING; // Address of the Identifier in Memory
	int LOC;
	/***Argstruct must store the name and type of each argument ***/
	struct arglist * arglist;
	struct Gsymbol *NEXT; // Pointer to next Symbol Table Entry */
};

struct node {
	int TYPE;		/* token value as of nw */
	int NODETYPE; /* for nw token numbers are used */
	char* NAME; /* for functions/identifiers */
	int VALUE; /* for constants */
	struct node *arglist;
	struct node *ptr1, *ptr2, *ptr3;
	struct Gsymbol * Gentry;
};
int fun_par_type;
struct arglist{
	struct arglist * NEXT,* TAIL;
	int TYPE;
	char * NAME;
	int VALUE;
};
void yyerror(char *);
struct node * mnode(int TYPE, int NODETYPE, int VALUE, char* NAME, struct node * arglist, struct node * ptr1, struct node *  ptr2, struct node *  ptr3);
extern int yylineno;
int location=0;
void print(struct node * nd);
int eval(struct node * nd);
struct node * root;
int reg_count=-1,label_count=0;
int get_reg();
void dec_reg();
int get_label();
int code_gen(struct node * nd);
int code_gen_aux(struct node * nd);
struct node * zero_node;
struct Gsymbol * head=NULL, * tail=NULL, * tail_id=NULL;
struct Gsymbol * Glookup(char * NAME); // Look up for a global identifier
struct Gsymbol * make_Gentry(char * NAME,int TYPE,int SIZE,struct arglist * ARGLIST); // Installation
struct Gsymbol * put_type(struct Gsymbol * t,int type);
struct Gsymbol * Ginstall(struct Gsymbol * t);
void print_decl(struct Gsymbol * nd);
void check_if_exists(char * NAME, struct Gsymbol * head);
struct arglist * make_argentry(char * name,int type);
struct arglist * fun_put_type(struct arglist * x, int type);
struct arglist * fun_tail;
int i;
void fun_check_if_exists(char * NAME);
struct arglist * fun_par_head=NULL;
struct arglist * fun_par_tail;
