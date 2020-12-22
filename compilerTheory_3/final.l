%option noyywrap
%{
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

FILE *yyin;

typedef enum tokenType {
SQLend, SQLmore, SQLname, SQLscope, SQLescope, SQLdigit, SQLstring,
SQLoperator, SQLequals, SQLcondition,
SQLshowtables, SQLshowdatabases, SQLshowcolumnsfrom,
SQLselect, SQLfrom, SQLselectdistinct,
SQLlimit, SQLoffset,
SQLorderby,
SQLwhere, SQLbetweenand, SQLandor, SQLin, SQLnotin, SQLlike,
SQLfunc,
SQLupdate, SQLset, SQLdelete, SQLinsertinto, SQLvalues,
SQLcreatetable, SQLnewcolumn
}tokenType;

int size = 0;
tokenType *tokenArr;

void addToken(tokenType token);

%}

%%
"("																																																										{addToken(SQLscope);}
")"																																																										{addToken(SQLescope);}
("+"|"-"|"*"|"/")																																																						{addToken(SQLoperator);}
"="																																																										{addToken(SQLequals);}
(">"|"<"|">="|"<="|"!=")																																																				{addToken(SQLcondition);}
";"[[:space:]]*																																																							{addToken(SQLend);}
","[[:space:]]*																																																							{addToken(SQLmore);}
"SHOW"[[:space:]]+"DATABASES"[[:space:]]* 																																																{addToken(SQLshowdatabases);}
"SHOW"[[:space:]]+"TABLES"[[:space:]]*																																																	{addToken(SQLshowtables);}
"SHOW"[[:space:]]+"COLUMNS"[[:space:]]+"FROM"[[:space:]]+[[:alnum:]]+[[:space:]]*																																						{addToken(SQLshowcolumnsfrom);}
"SELECT"[[:space:]]+																																																					{addToken(SQLselect);}
"FROM"[[:space:]]+																																																						{addToken(SQLfrom);}							
"SELECT"[[:space:]]+"DISTINCT"[[:space:]]+																																																{addToken(SQLselectdistinct);}
"LIMIT"[[:space:]]+[[:digit:]]+[[:space:]]*																																																{addToken(SQLlimit);}
"OFFSET"[[:space:]]+[[:digit:]]+[[:space:]]*																																															{addToken(SQLoffset);}
"ORDER"[[:space:]]+"BY"[[:space:]]+([[:alnum:]]+("."[[:alnum:]]+)?","?[[:space:]]*)+[[:space:]]*																																		{addToken(SQLorderby);}
"WHERE"[[:space:]]*																																																						{addToken(SQLwhere);}
"BETWEEN"[[:space:]]+[[:digit:]]+[[:space:]]+"AND"[[:space:]]+[[:digit:]]+[[:space:]]*																																					{addToken(SQLbetweenand);}
("AND"|"OR")[[:space:]]*																																																				{addToken(SQLandor);}
[[:alnum:]]+[[:space:]]+"IN"[[:space:]]*("("?"'"[[:alpha:]]+"'"","?")"?)+[[:space:]]*																																					{addToken(SQLin);}
[[:alnum:]]+[[:space:]]+"NOT IN"[[:space:]]*("("?"'"[[:alpha:]]+"'"","?")"?)+[[:space:]]*																																				{addToken(SQLnotin);}
"LIKE"[[:space:]]+"'""%"[[:alnum:]]+"'"[[:space:]]*																																														{addToken(SQLlike);}
("SQRT"|"AVG"|"SUM"|"MIN")																																																				{addToken(SQLfunc);}
"UPDATE"[[:space:]]+[[:alnum:]]+[[:space:]]*																																															{addToken(SQLupdate);}
"SET"[[:space:]]*																																																						{addToken(SQLset);}
"DELETE"[[:space:]]*																																																					{addToken(SQLdelete);}
"INSERT"[[:space:]]+"INTO"[[:space:]]*																																																	{addToken(SQLinsertinto);}
"VALUES"[[:space:]]+([[:digit:]]+|"'"[[:alnum:]]+"',"?)+[[:space:]]*																																									{addToken(SQLvalues);}
"CREATE"[[:space:]]+"TABLE"[[:space:]]+[[:alnum:]]+[[:space:]]*																																											{addToken(SQLcreatetable);}
([[:space:]]+[[:alnum:]]+[[:space:]]+(("int"|"date"|"datetime"|"time"|"text")|("varchar"|"char")"("[ ]*[[:digit:]]+[ ]*")"|("float"|"double")"("[ ]*[[:digit:]]+","[ ]*[[:digit:]]+[ ]*")")","?)+[[:space:]]?[[:space:]]* 				{addToken(SQLnewcolumn);}
[[:digit:]]+																																																							{addToken(SQLdigit);}
[[:alnum:]]+("."[[:alnum:]]+)?																																																			{addToken(SQLname);}
"'"[ ]*[[:alnum:]]+[ ]*"'"																																																				{addToken(SQLstring);}
	
%%

int main(int argc, char **argv) {

	tokenArr = (tokenType*)malloc(1 * sizeof(tokenType));
	yyin = fopen("Text.txt", "r");
	yylex();
	fclose(yyin);
	printf("%d\n", size);
	for (int i = 0; i < size; i++) {
		printf("%d ", tokenArr[i]);
	}
}

void addToken(tokenType token) {
	size++;
	tokenArr = (tokenType*)realloc(tokenArr, size * sizeof(tokenType));
	tokenArr[size - 1] = token;
}


