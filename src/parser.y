%{
#include <stdio.h>

#include "parser_def.h"
#include "tree.h"

int yylex();
void yyerror(sql_statement_tree** tree, const char* s);
%}

%code requires {
#include "tree.h"
}

%parse-param {sql_statement_tree **tree}

%union {
	int intval;
	char *strval;
	sql_statement_tree *statement;
}

%left AND
%left BETWEEN
%nonassoc IS

%token <intval> INTNUM;
%token <strval> NAME;
%token <strval> STRING

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

%type <statement> create_table
%type <statement> select_table
%type <statement> insert_into_table
%type <statement> delete_from_table

%%
sql: create_table       { *tree = $1; }
	| select_table      { *tree = $1; }
	| insert_into_table { *tree = $1; }
	| delete_from_table { *tree = $1; }
	;

create_table
	: CREATE TABLE NAME '(' new_colums ')' { $$ = sql_create(); }
	;

new_colums
	: name_type
	| new_colums ',' name_type
	;

name_type: NAME TYPE;

TYPE
	: INT
	| VARCHAR '(' INTNUM ')'
	;


select_table
	: SELECT column_list FROM NAME            { $$ = sql_select(); }
	| SELECT column_list FROM NAME WHERE expr { $$ = sql_select(); }
	;

column_list: '*' | columns;

columns
	: NAME
	| columns ',' NAME
	;

expr
	: NAME
	| INTNUM
	| STRING
	| expr IS expr
	| expr BETWEEN expr AND expr
	;


insert_into_table
	: INSERT INTO NAME VALUES insert_values { $$ = sql_insert(); }
	;

insert_values
	: '(' data_list ')'
	| insert_values ',' '(' data_list ')'
	;

data_list
	: INTNUM
	| STRING
	| data_list ',' INTNUM
	| data_list ',' STRING
	;


delete_from_table
	: DELETE FROM NAME WHERE expr { $$ = sql_delete(); }
	;
%%

sql_statement_tree* parse(const char* str)
{
	parse_init(str);

	sql_statement_tree* tree = NULL;
	printf("%s\n", yyparse(&tree) == 0 ? "ok" : "no");

	parse_free();

	return tree;
}

void yyerror(sql_statement_tree**, const char* s)
{
	fprintf(stderr, "%s\n", s);
}

int yywrap()
{
	return 1;
}
