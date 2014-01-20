struct node * mnode(int TYPE, int NODETYPE, int VALUE, char* NAME, struct node * arglist, struct node * ptr1, struct node *  ptr2, struct node *  ptr3){
	/* to check whether E in if statemnt has a bool return type */
	if(NODETYPE==IF && ptr1->TYPE!=BOOL){
		yyerror("bool expected for if");
		return NULL;
	}
	/* to check whether E in while statemnt has a bool return type */	
	if(NODETYPE==WHILE && ptr1->TYPE!=BOOL){
		yyerror("bool expected for while");
		return NULL;
	}
	/* to check whether both the operands are present */	
	if(NODETYPE==PLUS || NODETYPE==MINUS || NODETYPE==PDT || NODETYPE==DIV || NODETYPE==GT || NODETYPE==LT || NODETYPE==EQ )
		if(ptr1==NULL || ptr2==NULL)
			return NULL;
	/* checks if assignment operands are of same type */		
	if(NODETYPE==ASGN && (ptr1->TYPE != ptr2->TYPE) )
		return NULL;
	
	struct node * t;
	t=(struct node *)malloc(sizeof(struct node));
	t->TYPE=TYPE;
	/* to check whether ideantifier is declared.... */	
	if(NODETYPE==ID){
		struct Gsymbol * gsy;
		gsy=Glookup(NAME);
		if(gsy==NULL){
			printf("undefined variable: %s\n",NAME);
			return NULL;
		}
		if(ptr1==NULL){
			return NULL;
		}
		t->TYPE=gsy->TYPE;
		t->Gentry=gsy;
	}	
	/*  to ckeck for valid tree formation */
	if(TYPE==VOID)
		if(ptr1==NULL)
			return NULL;	
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

struct Gsymbol * Ginstall(char * NAME,int TYPE,int SIZE,struct Gsymbol * ARGLIST){
	struct Gsymbol * t;
	t=(struct Gsymbol *)malloc(sizeof(struct Gsymbol));
	t->BINDING=(int *)malloc(sizeof(int)*SIZE);
	t->NAME=(char *)malloc(sizeof(char)*50);
	t->NAME=NAME;
	t->SIZE=SIZE;
	t->NEXT=NULL;
	t->LOC=location;
	location+=SIZE;
	return t;
}
struct Gsymbol * put_type(struct Gsymbol * t,int type){
	struct Gsymbol * x;
	x=t;
	while(t!=NULL){
		t->TYPE=type;
		t=t->NEXT;
	}
	return x;
}
struct Gsymbol * add_tree(struct Gsymbol * t){
	if(tail==NULL){
		head=t;
	}else{
		tail->NEXT=t;
	}
	tail=tail_id;
}
struct Gsymbol * Glookup(char * name){
	struct Gsymbol * t;
	t=head;
	while( t!=NULL && strcmp(t->NAME,name) )
		t=t->NEXT;
	return t;	
}
