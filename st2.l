%{
	#include<stdio.h>
	#include "y.tab.h"
	#include<stdlib.h>	
%}

number 	[0-9]+
plus 	\+
pdt		\*
minus	\-
div		\/

%%
{div}			{return DIV;}
{minus}			{return MINUS;}
{plus}			{return PLUS;}
{pdt}			{return PDT;}
[\n]				return NL;
{number}			{yylval.n=atoi(yytext);return NUM;}
[()]				return *yytext;
.				{printf("invalid character!\n");return 0;}
%%

