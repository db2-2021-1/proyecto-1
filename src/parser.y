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

%token CREATE
%token DELETE
%token FROM
%token INSERT
%token INT
%token INTO
%token SELECT
%token TABLE
%token VALUES
%token VARCHAR
%token WHERE

%%
sql: create_table
	| select_table
	| insert_into_table
	| delete_from_table
;

create_table: CREATE TABLE NAME '(' new_colums ')';

new_colums: name_type | new_colums ',' name_type;

name_type: NAME TYPE;

TYPE: INT
	| VARCHAR '(' INTNUM ')'
	;


select_table: SELECT column_list FROM NAME
	| SELECT column_list FROM NAME WHERE expr
	;

column_list: '*' | columns;

columns: NAME
	| columns ',' NAME
	;

expr: NAME
	| INTNUM
	| expr IS expr
	| expr BETWEEN expr AND expr
	;


insert_into_table: INSERT INTO NAME VALUES insert_values;

insert_values: '(' data_list ')'
	| insert_values ',' '(' data_list ')'
	;

data_list: INTNUM
	| data_list ',' INTNUM
	;


delete_from_table: DELETE FROM NAME WHERE expr;
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
