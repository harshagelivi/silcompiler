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
	reg_count=0;
}
int code_gen_aux(struct node * nd){
	printf("START\n");
	code_gen(nd);
	printf("HALT\n");
}
int code_gen(struct node * nd){
	int i,j,label,label2,offset;
	if(nd!=NULL){
		switch(nd->TYPE){
			case(NUM):
				switch(nd->NODETYPE){
					case(NUM):
						i=get_reg();
						printf("MOV R%d, %d\n",i,nd->VALUE);
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
						offset=eval(nd->ptr1);
						i=nd->Gentry->LOC+offset;
						j=get_reg();
						printf("MOV R%d, [%d]\n",j,i);
						return j;
						break;
				}
				break;
			
			case(BOOL):
				switch(nd->NODETYPE){
				
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
						offset=eval(nd->ptr1->ptr1);
						i=nd->ptr1->Gentry->LOC+offset;
						j=code_gen(nd->ptr2);
						printf("MOV [%d], R%d\n",i,j);
						dec_reg();
						break;									
					case(IF):
						i=code_gen(nd->ptr1);
						label=get_label();
						printf("JZ R%d, L%d\n",i,label);
						dec_reg();
						code_gen(nd->ptr2);
						label2=get_label();
						printf("JMP L%d\n",label2);
						printf("L%d:\n",label);
						code_gen(nd->ptr3);
						printf("L%d:\n",label2);
						break;
					case(WHILE):
						label=get_label();
						label2=get_label();
						printf("L%d:\n",label);
						i=code_gen(nd->ptr1);
						printf("JZ R%d, L%d\n",i,label2);
						dec_reg();
						code_gen(nd->ptr2);
						printf("JMP L%d\n",label);
						printf("L%d:\n",label2);
						break;
					case(READ):
						offset=eval(nd->ptr1->ptr1);
						i=nd->ptr1->Gentry->LOC+offset;
						j=get_reg();
						printf("IN R%d\n",j);
						printf("MOV [%d], R%d\n",i,j);
						dec_reg();
						return i;
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
}

