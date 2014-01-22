int eval(struct node * nd){
if(nd!=NULL){
	int offset;
	struct node * t;
	switch(nd->TYPE){
		case(INT):
			switch(nd->NODETYPE){
				case(INT):
					return nd->VALUE;
					break;
				case(DIV):
					return eval(nd->ptr1)/eval(nd->ptr2);
					break;
				case(PDT):
					return eval(nd->ptr1)*eval(nd->ptr2);					
					break;
				case(PLUS):
					
					return eval(nd->ptr1)+eval(nd->ptr2);
					break;
				case(MINUS):
					return eval(nd->ptr1)-eval(nd->ptr2);
					break;
				case(ID):
					offset=eval(nd->ptr1);
					return *(nd->Gentry->BINDING+offset);
					break;
			}
			break;
			
		case(BOOL):
			switch(nd->NODETYPE){
				case(ID):
					offset=eval(nd->ptr1);
					return *(nd->Gentry->BINDING+offset);
					break;
				case(BOOL):
					return nd->VALUE;
					break;				
				case(LT):
					return eval(nd->ptr1)<eval(nd->ptr2)?CTRUE:CFALSE;
					break;
				case(GT):
					return eval(nd->ptr1)>eval(nd->ptr2)?CTRUE:CFALSE;				
					break;
				case(EQ):
					return eval(nd->ptr1)==eval(nd->ptr2)?CTRUE:CFALSE;					
					break;
				case(LE):
					return eval(nd->ptr1)<=eval(nd->ptr2)?CTRUE:CFALSE;
					break;
				case(GE):
					return eval(nd->ptr1)>=eval(nd->ptr2)?CTRUE:CFALSE;				
					break;
				case(NEQ):
					return eval(nd->ptr1)!=eval(nd->ptr2)?CTRUE:CFALSE;					
					break;
			}		
			break;
		case(VOID):
			switch(nd->NODETYPE){
				case(STMT):
					eval(nd->ptr1);
					break;	
				case(SLIST):
					eval(nd->ptr1);
					eval(nd->ptr2);
					break;	
				case(ASGN):
					offset=0;
					t=nd->ptr1->ptr1;
					offset=eval(t);
					*(nd->ptr1->Gentry->BINDING+offset)=eval(nd->ptr2);
					break;
				case(IF):
					if(eval(nd->ptr1)==CTRUE)
						eval(nd->ptr2);
					else
						eval(nd->ptr3);	
					break;
				case(WHILE):
					while(eval(nd->ptr1)==CTRUE)
						eval(nd->ptr2);
					break;
				case(READ):
					t=nd->ptr1->ptr1;
					offset=eval(t);
					scanf("%d", (nd->ptr1->Gentry->BINDING+offset));		
					break;
				case(WRITE):
					printf("value is %d\n",eval(nd->ptr1));
					break;
			}
			break;		
	}
}
}

