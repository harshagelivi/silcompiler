int get_reg(){
	reg_count++;
	return reg_count;
}
void dec_reg(){
	reg_count--;
}
int get_label(){
	label_count++;
	return label_count;
}
void free_reg(){
	reg_count=-1;
}
int code_gen_aux(struct node * nd){
	int i,j,main_size;
	printf("START\n");
	j=get_reg();
	struct Gsymbol * t=global_head;
	global_var_size=-1;
	while(t!=NULL){
		global_var_size+=t->SIZE;
		t=t->NEXT;
	}
	printf("MOV R%d, %d\n",j,global_var_size);
	printf("MOV SP, R%d\n",j);
	printf("MOV BP, SP    //initialising SP and BP\n\n");
	main_size=get_decls_size("main");
	i=get_reg();
	printf("MOV R%d, %d\n",i,main_size);
	printf("MOV R%d, SP\n",j);
	printf("ADD R%d, R%d\n",i,j);
	printf("MOV SP, R%d    //allocating space for main local variables\n\n",i);
	dec_reg();
	dec_reg();
	code_gen(nd);
	printf("HALT\n");
}
int get_decls_size(char * func_name){
	struct Gsymbol * t= Glookup(func_name);
	int count=0;
	t=t->local_decls;
	while(t!=NULL){
		count += t->SIZE;
		t=t->NEXT;
	}
	return count;
}
void callee_entry_gen(char * func_name){
	/*  SAVES OLD BP AND ALLOCATES SPACE FOR LOCAL VARIABLES */
	int size=get_decls_size(func_name),i,j;
	printf("PUSH BP   //callee on entry--strt\n");
	printf("MOV BP, SP\n");
	i=get_reg();
	printf("MOV R%d, %d\n", i,size);
	j=get_reg();
	printf("MOV R%d, SP\n", j);
	printf("ADD R%d, R%d\n",i,j);
	printf("MOV SP, R%d  //callee on entry--stop(setting space for local variables for %s)\n\n", i, func_name);
	dec_reg();
	dec_reg();
}
void callee_exit_gen(struct node * return_node){	
	/* PUTS THE RETURN VALUE IN APPROPRIATE PLACE(FOR BOOL CTRUE/CFALSE IS KEPT)  AND SP IS SET TO POINT TO RETURN ADDRESS*/
	int i;
	int return_val_register=code_gen(return_node);
	i=get_reg();
	printf("MOV R%d, BP   //callee on exit--strt\n",i);
	printf("DCR R%d\n", i);
	printf("DCR R%d\n", i);	//return value always goes to assign
	/*					//so 1 or 0 has to be returned, assign code takes 1 or 0 for bool assignments
	if(return_node->TYPE==BOOL){
		int k, label, label2;
		k=get_reg();
		printf("MOV R%d, 1\n",k);
		printf("EQ R%d, R%d\n", return_val_register, k);
		label=get_label();
		dec_reg();
		printf("JZ R%d, L%d\n", return_val_register, label);
		printf("MOV R%d, %d\n", return_val_register, CTRUE);
		label2=get_label();
		printf("JMP L%d\n",label2);
		printf("L%d:\n",label);
		printf("MOV R%d, %d\n", return_val_register, CFALSE);
		printf("L%d:\n",label2);						
	}
	*/
	printf("MOV [R%d], R%d\n", i, return_val_register);
	dec_reg();
	dec_reg();
	i=get_reg();
	printf("MOV R%d, BP\n",i);
	printf("DCR R%d \n", i);
	printf("MOV SP, R%d\n", i);	//SP is made equal to BP-1 for returning
	dec_reg();
	//printf("MOV BP, [BP]\n");
	free_reg();
	printf("RET   //callee on exit--stop\n\n");
}
void func_code_gen(char * func_name, struct node * func_body){
	printf("%s:\n",func_name);
	callee_entry_gen(func_name);
	code_gen(func_body);
	printf("\n");
	

}
int code_gen(struct node * nd){
	int i,j,label,label2,offset,k,m,prev;
//	printf("            before gen %d\n",reg_count);
	if(nd->NODETYPE==RETURN){
		callee_exit_gen(nd->ptr1);
		return 0;
	}
	if(nd->NODETYPE==FUNC){
		prev=reg_count;
		printf("\nPUSH R7   //caller on entry--strt\n");	
		printf("PUSH R6\n");	
		printf("PUSH R5\n");	
		printf("PUSH R4\n");	
		printf("PUSH R3\n");	
		printf("PUSH R2\n");	
		printf("PUSH R1\n");	
		printf("PUSH R0\n\n");	
		free_reg();
		push_args(nd->arglist);
		printf("PUSH R0\n");//for return
		printf("CALL %s\n",nd->NAME);
		
		pop_args(nd->arglist);
		reg_count=prev;
		printf("\nPOP R0\n");	
		printf("POP R1\n");	
		printf("POP R2\n");	
		printf("POP R3\n");	
		printf("POP R4\n");	
		printf("POP R5\n");	
		printf("POP R6\n");	
		printf("POP R7\n\n");	
		i=get_reg();
		printf("MOV R%d, BP\n",i);
		printf("DCR R%d\n", i);
		printf("DCR R%d\n", i);		
		printf("MOV R%d, [R%d]\n",i,i);
		printf("MOV BP, [BP]  //caller on entry--stop\n\n");
		return i;
	}
	if(nd->NODETYPE==REFR){		//only during function calls.. wen &id is found in grammar
		int offset=code_gen(nd->ptr1->ptr1);
		i=nd->ptr1->Gentry->LOC;
		j=get_reg();
		printf("MOV R%d, %d           //REFR strt\n",j,i);
		printf("ADD R%d, R%d\n", offset, j);
		k=get_reg();
		if(nd->ptr1->Gentry->scope_flag=='g')
			printf("MOV R%d, -1\n",k);
		else
			printf("MOV R%d, BP\n",k);
		printf("ADD R%d, R%d         //REFR stop\n", offset, k);
		dec_reg();
		dec_reg();
		return offset;
	}
	if(nd!=NULL){
		switch(nd->TYPE){
			case(INT):
				switch(nd->NODETYPE){
					case(INT):
						i=get_reg();
						printf("MOV R%d, %d\n",i,nd->VALUE);
						return i;
						break;
					case(MOD):
						i = code_gen(nd->ptr1);
						j = code_gen(nd->ptr2);
						printf("MOD R%d, R%d \n",i,j);
						dec_reg();
						return i;
						break;
					case(DIV):
						i = code_gen(nd->ptr1);
						j = code_gen(nd->ptr2);
						printf("DIV R%d, R%d \n",i,j);
						dec_reg();
						return i;
						break;
					case(PDT):
						i = code_gen(nd->ptr1);
						j = code_gen(nd->ptr2);
						printf("MUL R%d, R%d \n",i,j);
						dec_reg();
						return i;
						break;
					case(PLUS):
						i = code_gen(nd->ptr1);
						j = code_gen(nd->ptr2);
						printf("ADD R%d, R%d \n",i,j);
						dec_reg();
						return i;
						break;
					case(MINUS):
						i = code_gen(nd->ptr1);
						j = code_gen(nd->ptr2);
						printf("SUB R%d, R%d \n",i,j);
						dec_reg();
						return i;
						break;
					case(ID):
						offset=code_gen(nd->ptr1);
						j=get_reg();
						m=get_reg();
						if(nd->Gentry!=NULL){
							i=nd->Gentry->LOC;
							if(nd->Gentry->scope_flag=='g')
								printf("MOV R%d, -1\n", m);
							else
								printf("MOV R%d, BP\n", m);							
						}else{
							i=nd->Argentry->LOC;	//starts from -3 for each argument
							printf("MOV R%d, BP\n", m);
						}
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						printf("ADD R%d, R%d\n", offset, m);
						dec_reg();
						dec_reg();
						printf("MOV R%d, [R%d]   //value from id %s\n\n", offset, offset, nd->NAME);
						if(nd->Argentry!=NULL)
							if(nd->Argentry->reference_flag==1){
								printf("MOV R%d, [R%d]   //value from pointer %s\n\n", offset, offset, nd->NAME);
							}
						return offset;
						break;
				}
				break;
			
			case(BOOL):
				switch(nd->NODETYPE){
					case(ID):
						offset=code_gen(nd->ptr1);
						j=get_reg();
						m=get_reg();
						if(nd->Gentry!=NULL){
							i=nd->Gentry->LOC;
							if(nd->Gentry->scope_flag=='g')
								printf("MOV R%d, -1\n", m);
							else
								printf("MOV R%d, BP\n", m);							
						}else{
							i=nd->Argentry->LOC;	
							printf("MOV R%d, BP\n", m);
						}
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						printf("ADD R%d, R%d\n", offset, m);
						dec_reg();
						dec_reg();
						printf("MOV R%d, [R%d]\n",offset,offset);
						if(nd->Argentry!=NULL)
							if(nd->Argentry->reference_flag==1){
								printf("MOV R%d, [R%d]   //value from pointer %s\n\n", offset, offset, nd->NAME);
							}
						k=get_reg();
						printf("MOV R%d, %d\n",k,CTRUE);
						printf("EQ R%d, R%d   //value from id %s\n\n", offset, k, nd->NAME);
						dec_reg();
						return offset;
						break;
					case(BOOL):
						i=get_reg();
						printf("MOV R%d, %d\n",i,nd->VALUE);
						k=get_reg();
						printf("MOV R%d, %d\n",k,CTRUE);
						printf("EQ R%d, R%d\n",i,k);
						dec_reg();
						return i;
						break;
				
					case(LT):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("LT R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;
					case(GT):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("GT R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;
					case(EQ):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("EQ R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;
					case(LE):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("LE R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;						
					case(AND):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						k=get_reg();
						printf("MOV R%d, 2\n",k);
						printf("ADD R%d, R%d\n",i,j);
						printf("EQ R%d, R%d\n",i,k);
						dec_reg();
						dec_reg();
						return i;
						break;
					case(OR):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						k=get_reg();
						printf("MOV R%d, 0\n",k);
						printf("ADD R%d, R%d\n",i,j);
						printf("GT R%d, R%d\n",i,k);
						dec_reg();
						dec_reg();
						return i;
						break;
					case(NOT):
						i=code_gen(nd->ptr1);
						k=get_reg();
						printf("MOV R%d, 0\n",k);
						printf("EQ R%d, R%d\n",i,k);
						dec_reg();
						return i;
						break;
					case(GE):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("GE R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;
					case(NEQ):
						i=code_gen(nd->ptr1);
						j=code_gen(nd->ptr2);
						printf("NE R%d, R%d\n",i,j);
						dec_reg();
						return i;
						break;
				}		
				break;
			case(VOID):
				switch(nd->NODETYPE){
					case(STMT):
						code_gen(nd->ptr1);
						break;	
					case(SLIST):
						code_gen(nd->ptr1);
						code_gen(nd->ptr2);
						break;	
					case(ASGN):
						offset=code_gen(nd->ptr1->ptr1);
						j=get_reg();
						m=get_reg();
						if(nd->ptr1->Gentry!=NULL){
							i=nd->ptr1->Gentry->LOC;
							if(nd->ptr1->Gentry->scope_flag=='g')
								printf("MOV R%d, -1\n", m);
							else
								printf("MOV R%d, BP\n", m);							
						}else{
							i=nd->ptr1->Argentry->LOC;	
							printf("MOV R%d, BP\n", m);
						}
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						printf("ADD R%d, R%d     // address for assgn\n", offset, m);
						dec_reg();
						dec_reg();
						j=code_gen(nd->ptr2);
						if(nd->ptr1->Argentry!=NULL)
							if(nd->ptr1->Argentry->reference_flag==1)
								printf("MOV R%d, [R%d]\n",offset,offset);
						if(nd->ptr1->TYPE==BOOL){
							k=get_reg();
							printf("MOV R%d, 1\n",k);
							printf("EQ R%d, R%d\n",j,k);
							label=get_label();
							dec_reg();
							printf("JZ R%d, L%d\n",j,label);
							printf("MOV [R%d], %d\n", offset, CTRUE);
							label2=get_label();
							printf("JMP L%d\n",label2);
							printf("L%d:\n",label);
							printf("MOV [R%d], %d\n", offset, CFALSE);
							printf("L%d:\n",label2);						
						}else{
							printf("MOV [R%d], R%d\n",offset,j);
						}	
						dec_reg();
						dec_reg();
						break;									
					case(IF):
						i=code_gen(nd->ptr1);
						label=get_label();
						printf("JZ R%d, L%d        //for if\n",i,label);
						dec_reg();
						code_gen(nd->ptr2);
						label2=get_label();
						printf("JMP L%d\n",label2);
						printf("L%d:\n",label);
						code_gen(nd->ptr3);
						printf("L%d:\n",label2);
						break;
					case(WHILE):
						printf("\n");
						label=get_label();
						label2=get_label();
						printf("L%d:\n",label);
						i=code_gen(nd->ptr1);
						printf("JZ R%d, L%d           //for while\n",i,label2);
						dec_reg();
						code_gen(nd->ptr2);
						printf("JMP L%d\n",label);
						printf("L%d:\n",label2);
						printf("\n");						
						break;
					case(READ):				
						offset=code_gen(nd->ptr1->ptr1);
						j=get_reg();
						m=get_reg();
						if(nd->ptr1->Gentry!=NULL){
							i=nd->ptr1->Gentry->LOC;
							if(nd->ptr1->Gentry->scope_flag=='g')
								printf("MOV R%d, -1\n", m);
							else
								printf("MOV R%d, BP\n", m);							
						}else{
							i=nd->ptr1->Argentry->LOC;	
							printf("MOV R%d, BP\n", m);
						}
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						printf("ADD R%d, R%d     // address for assgn\n", offset, m);
						dec_reg();
						dec_reg();
						j=get_reg();
						printf("IN R%d\n", j);
						if(nd->ptr1->Argentry!=NULL)
							if(nd->ptr1->Argentry->reference_flag==1)
								printf("MOV R%d, [R%d]\n",offset,offset);
						printf("MOV [R%d], R%d\n",offset,j);
						dec_reg();
						dec_reg();
						break;									
					case(WRITE):
						j=code_gen(nd->ptr1);
						printf("OUT R%d\n",j);
						dec_reg();
						break;
				}
				break;		
		}
	}
//	printf("            after gen %d\n",reg_count);
}
void push_args(struct node * t){
	if(t!=NULL){
		push_args(t->next_arg);
	}else
		return;
	//printf("chk %s in %d--%d   %d    %d\n",t->NAME, t->Gentry->LOC, t->TYPE, t->NODETYPE, t->VALUE);
	int i=code_gen(t);
	if(t->TYPE==BOOL){
		if(t->NODETYPE!=REFR){
			int k, label, label2;
			k=get_reg();
			printf("MOV R%d, 1\n",k);
			printf("EQ R%d, R%d\n", i, k);
			label=get_label();
			dec_reg();
			printf("JZ R%d, L%d\n", i, label);
			printf("MOV R%d, %d\n", i, CTRUE);
			label2=get_label();
			printf("JMP L%d\n",label2);
			printf("L%d:\n",label);
			printf("MOV R%d, %d\n", i, CFALSE);
			printf("L%d:\n",label2);
		}
	}
	printf("   PUSH R%d\n\n",i);
	dec_reg();
}
void pop_args(struct node * a ){ 	//this function calculates the number of arguments and sets the SP so that it points to first 								//register to be popped on the stack(R0)
	int count=1;
	while(a!=NULL){
		count++;
		a=a->next_arg;
	}
	int t=get_reg(), u=get_reg();
	printf("\nMOV R%d, SP    //code to set SP\n", t);
	printf("MOV R%d, %d\n", u, count);
	printf("SUB R%d, R%d\n", t, u);
	printf("MOV SP, R%d       //SP now points to R0\n", t);	
	dec_reg();
	dec_reg();
}
