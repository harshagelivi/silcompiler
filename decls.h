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
	struct Gsymbol * local_decls;//---------------------------------------------------------------------------
	char scope_flag;
	
};

struct node {
	int TYPE;		/* token value as of nw */
	int NODETYPE; /* for nw token numbers are used */
	char* NAME; /* for functions/identifiers */
	int VALUE; /* for constants */
	struct node *arglist;
	struct node * next_arg;
	struct node *ptr1, *ptr2, *ptr3;
	struct Gsymbol * Gentry;
	struct arglist * Argentry;
};
int fun_par_type,var_par_type,global_flag=1;
struct arglist{
	struct arglist * NEXT,* TAIL;
	int TYPE;
	char * NAME;
	int VALUE,LOC;
	int reference_flag;
};
void yyerror(char *);
struct node * mnode(int TYPE, int NODETYPE, int VALUE, char* NAME, struct node * arglist, struct node * ptr1, struct node *  ptr2, struct node *  ptr3);
extern int yylineno;
int location=1;
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
struct Gsymbol * head=NULL, * tail=NULL, * tail_id=NULL, * global_head;
struct Gsymbol * Glookup(char * NAME); // Look up for a global identifier
struct Gsymbol * make_Gentry(char * NAME,int TYPE,int SIZE,struct arglist * ARGLIST); // Installation
struct Gsymbol * put_type(struct Gsymbol * t,int type);
struct Gsymbol * Ginstall(struct Gsymbol * t);
void print_decl(struct Gsymbol * nd);
void check_if_exists(char * NAME, struct Gsymbol * head);
struct arglist * make_argentry(char * name,int type,int flag);
struct arglist * fun_put_type(struct arglist * x, int type);
struct arglist * fun_tail;
int i;
char * func_name;
void fun_check_if_exists(char * NAME);
struct arglist * fun_par_head=NULL;
struct arglist * fun_par_tail;
void global_done();
struct Gsymbol * set_head();
void fun_type_check(char *NAME, int RET_TYPE, struct arglist * args);
struct arglist * set_fun_head();
void put_func_decls(char * func_name, struct Gsymbol * func_decls_head);
void insert_main_to_global();
struct node * rargs_head, * rargs_tail;
void append_to_rargs(struct node * t);
struct node * set_rargs_head();
void push_args(struct node *);
int global_var_size;
int get_decls_size(char * func_name);
void callee_entry_gen(char * func_name);
void func_code_gen(char * func_name, struct node * func_body);
int arg_location=1;
void pop_args(struct node * a );
