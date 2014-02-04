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
	int i,j,label,label2,offset,k;
	if(nd!=NULL){
		switch(nd->TYPE){
			case(INT):
				switch(nd->NODETYPE){
					case(INT):
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
						offset=code_gen(nd->ptr1);
						j=get_reg();
						i=nd->Gentry->LOC;
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						dec_reg();
						printf("MOV R%d, [R%d]\n",offset,offset);
						return offset;
						break;
				}
				break;
			
			case(BOOL):
				switch(nd->NODETYPE){
					case(ID):
						offset=code_gen(nd->ptr1);
						j=get_reg();
						i=nd->Gentry->LOC;
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						dec_reg();
						j=get_reg();
						printf("MOV R%d, [%d]\n",j,offset);
						dec_reg();
						k=get_reg();
						printf("MOV R%d, %d\n",k,CTRUE);
						printf("EQ R%d, R%d\n",j,k);
						dec_reg();
						return j;
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

						i=nd->ptr1->Gentry->LOC;
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						dec_reg();

						j=code_gen(nd->ptr2);
						if(nd->ptr1->TYPE==BOOL){
							k=get_reg();
							printf("MOV R%d, 1\n",k);
							printf("EQ R%d, R%d\n",j,k);
							label=get_label();
							dec_reg();
							printf("JZ R%d, L%d\n",j,label);
							printf("MOV [%d], %d\n",i,CTRUE);
							label2=get_label();
							printf("JMP L%d\n",label2);
							printf("L%d:\n",label);
							printf("MOV [%d], %d\n",i,CFALSE);
							printf("L%d:\n",label2);
						
						}else
							printf("MOV [R%d], R%d\n",offset,j);
						dec_reg();
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
						printf("\n");
						label=get_label();
						label2=get_label();
						printf("L%d:\n",label);
						i=code_gen(nd->ptr1);
						printf("JZ R%d, L%d\n",i,label2);
						dec_reg();
						code_gen(nd->ptr2);
						printf("JMP L%d\n",label);
						printf("L%d:\n",label2);
						printf("\n");						
						break;
					case(READ):
						offset=code_gen(nd->ptr1->ptr1);
						j=get_reg();

						i=nd->ptr1->Gentry->LOC;
						printf("MOV R%d, %d\n",j,i);
						printf("ADD R%d, R%d\n",offset,j);
						printf("IN R%d\n",j);						
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
}

