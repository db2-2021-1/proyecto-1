%{
#include <stdio.h>
#include "parser_def.h"

int yylex();
void yyerror(const char* s);
%}

%union {
	int intval;
	char *strval;
}

%left AND
%left BETWEEN
%nonassoc IS

%token <strval> NAME;
%token <intval> INTNUM;

%token DELETE
%token FROM
%token INSERT
%token INTO
%token SELECT
%token TABLE
%token VALUES
%token WHERE

%%
sql: select_table;

select_table:
	| SELECT column_list FROM NAME
	| SELECT column_list FROM NAME WHERE expr
	;

column_list: '*' | columns;

columns: NAME
	| columns ',' NAME
	;

expr:
	| NAME
	| INTNUM
	| expr IS expr
	| expr BETWEEN expr AND expr
	;
%%

void parse(const char* str)
{
	parse_init(str);

	printf("%s\n", yyparse() == 0 ? "ok" : "no");

	parse_free();
}

void yyerror(const char* s)
{
	fprintf(stderr, "%s\n", s);
}

int yywrap()
{
	return 1;
}
