#!/bin/bash
yacc -d -v $1.y
v=`echo $?`
if [ "$v" -ne "0" ]; then
	echo "YACC error"
else	
	lex $1.l
	v=`echo $?`
	if [ "$v" -ne "0" ]; then
		echo "LEX error"
	else	
		gcc lex.yy.c y.tab.c -ly -ll -o $1
		v=`echo $?`
		if [ "$v" -ne "0" ]; then
			echo "error in compiling"
		else
			echo "//success!"
			./$1
		fi;
	fi;

fi;

