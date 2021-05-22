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
	sql_expr *expr;
	sql_literal literal;
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
%type <expr> expr;
%type <literal> literal;

%%
sql: create_table       { *tree = $1; }
	| select_table      { *tree = $1; }
	| insert_into_table { *tree = $1; }
	| delete_from_table { *tree = $1; }
	;

create_table
	: CREATE TABLE NAME '(' new_colums ')' { $$ = sql_create($3); }
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
	: SELECT column_list FROM NAME            { $$ = sql_select($4); }
	| SELECT column_list FROM NAME WHERE expr { $$ = sql_select($4); }
	;

column_list: '*' | columns;

columns
	: NAME
	| columns ',' NAME
	;

expr
	: NAME IS literal                  { $$ = sql_expr_is($1, $3); }
	| NAME BETWEEN literal AND literal { $$ = sql_expr_between($1, $3, $5); }
	;

literal
	: INTNUM { $$ = sql_literal_number($1); }
	| STRING { $$ = sql_literal_string($1); }
	;

insert_into_table
	: INSERT INTO NAME VALUES insert_values { $$ = sql_insert($3); }
	;

insert_values
	: '(' data_list ')'
	| insert_values ',' '(' data_list ')'
	;

data_list
	: literal
	| data_list ',' literal
	;


delete_from_table
	: DELETE FROM NAME WHERE expr { $$ = sql_delete($3, $5); }
	;
%%

sql_statement_tree* parse(const char* str)
{
	parse_init(str);

	sql_statement_tree* tree = NULL;
	if(yyparse(&tree) != 0)
		fprintf(stderr, "%s\n", "Bad SQL.");

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
