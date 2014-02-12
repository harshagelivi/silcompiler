void print(struct node * nd){
	if(nd!=NULL){
		printf("%d   %d    %d\n",nd->TYPE,nd->NODETYPE,nd->VALUE);
		print(nd->ptr1);
		print(nd->ptr2);
		print(nd->ptr3);
	}
}
void print_decl(struct Gsymbol * nd){
	if(nd->arglist!=NULL){
		printf("function %s: \n", nd->NAME);
		struct arglist * t=nd->arglist;
		while(t!=NULL){
			printf("name:%s,   type:%d.  \n",t->NAME, t->TYPE);
			t=t->NEXT;
		}
	}else{
		while(nd!=NULL){
			printf("%s  type %d     size %d\n",nd->NAME, nd->TYPE, nd->SIZE);
			nd=nd->NEXT;
		}
	}	
}


