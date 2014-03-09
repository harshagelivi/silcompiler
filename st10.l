%{
	#include<stdio.h>
	#include "y.tab.h"
	#include<stdlib.h>	
%}
number 	[0-9]+
ws		[ \n\t]+
plus 	\+
pdt		\*
minus	\-
div		\/
lt		\<
gt		\>
eq		"=="
id		[a-z]([0-9]|[a-z])*
delim	";"
asgn	"="
rd		read
wt		write
cif		"if"
then		"then"
celse	"else"
cendif	"endif"
cwhile	"while"
cdo		"do"
cendw	"endwhile"
creturn 	"return"
%%
"&"				return AMPD;
"!="				return NEQ;
"<="				return LE;
">="				return GE;
"["				return *yytext;
"]"				return *yytext;
"decl"			{return DECL;}
"enddecl"		{return ENDDECL;}
"integer"			{return INT;}
"boolean"		{return BOOL;}
"TRUE"			{yylval.n=CTRUE;return ABOOL;}
"FALSE"			{yylval.n=CFALSE;return ABOOL;}
"begin"			return PBEGIN;
"main()"			return MAIN;
"end"			return PEND;
","				{return COMMA;}
"AND"			return AND;
"OR"			return OR;
"NOT"			return NOT;
"%"				return MOD;
{ws}			;
{cwhile}			{return WHILE;}
{cdo}			{return DO;}
{cendw}			{return ENDWHILE;}
{cif}				{return IF;}
{then}			{return THEN;}
{celse}			{return ELSE;}
{cendif}			{return ENDIF;}
{creturn}			{return RETURN;}
{rd}				{return READ;}
{wt}				{return WRITE;}
{asgn}			{return ASGN;}
{delim}			{return DELIM;}
{id}				{yylval.ch=strdup(yytext);return ID;}
{div}			{return DIV;}
{minus}			{return MINUS;}
{plus}			{return PLUS;}
{pdt}			{return PDT;}

{number}			{yylval.n=atoi(yytext);return NUM;}
[()}{]				return *yytext;
{lt}				return LT;
{gt}				return GT;
{eq}				return EQ;
.				{printf("invalid character: %c\n",*yytext);return 0;}
%%

