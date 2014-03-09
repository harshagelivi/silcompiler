void print(struct node * nd){
	if(nd!=NULL){
		if(nd->Gentry!=NULL)
			printf("%s in %d--%d   %d    %d\n",nd->NAME, nd->Gentry->LOC, nd->TYPE,nd->NODETYPE,nd->VALUE);
		else
			printf("%s--%d   %d    %d\n",nd->NAME, nd->TYPE,nd->NODETYPE,nd->VALUE);
		print(nd->arglist);
		print(nd->ptr1);
		print(nd->ptr2);
		print(nd->ptr3);
	}
}
void print_decl(struct Gsymbol * nd){

	while(nd!=NULL){
		printf("%s of  %d  at %d with size %d\n", nd->NAME, nd->TYPE, nd->LOC, nd->SIZE);
		if(nd->arglist!=NULL){
			printf("function %s: \n", nd->NAME);
			struct arglist * t=nd->arglist;
			while(t!=NULL){
				printf("    name:%s,   type:%d.  \n",t->NAME, t->TYPE);
				t=t->NEXT;
			}
		}
		struct Gsymbol * t=nd->local_decls;
		if(t!=NULL)
			while(t!=NULL){
				printf("           	%s of  %d  at %d\n", t->NAME, t->TYPE, t->LOC);
				t=t->NEXT;
			}
		nd=nd->NEXT;
	}
	
	printf("\n\n");
}


