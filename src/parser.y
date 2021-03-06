%{
#include <stdio.h>
#include <string.h>

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
	float realval;
	char *strval;
	sql_statement_tree *statement;
	sql_expr *expr;
	sql_literal literal;
	sql_columns* columns;
	sql_data_list* data_list;
	sql_insert_values* insert_values;
	sql_type type;
	sql_name_type name_type;
	sql_new_columns* new_columns;
}

%left AND
%left BETWEEN
%nonassoc IS

%token <intval> INTNUM;
%token <realval> REALNUM;
%token <strval> NAME;
%token <strval> STRING

%token COPY
%token CREATE
%token CSV
%token DELETE
%token FROM
%token HEADER
%token INDEX
%token INSERT
%token INT
%token INTO
%token ON
%token REAL
%token SELECT
%token TABLE
%token USING
%token VALUES
%token VARCHAR
%token WHERE

%type <statement> sql_toplevel
%type <statement> sql_statement
%type <statement> create_table
%type <statement> create_index
%type <statement> select_table
%type <statement> insert_into_table
%type <statement> delete_from_table
%type <statement> copy
%type <expr> expr;
%type <literal> literal;
%type <columns> column_list;
%type <columns> columns;
%type <data_list> data_list;
%type <insert_values> insert_values;
%type <type> TYPE;
%type <name_type> name_type;
%type <new_columns> new_columns;

%%
sql_toplevel
	: sql_statement ';' { *tree = $1; $$ = $1; }
	| sql_statement ';' sql_toplevel { *tree = $1; $1->next = $3; }
	;

sql_statement
	: create_table      { $$ = $1; }
	| create_index      { $$ = $1; }
	| select_table      { $$ = $1; }
	| insert_into_table { $$ = $1; }
	| delete_from_table { $$ = $1; }
	| copy              { $$ = $1; }
	;

create_table
	: CREATE TABLE NAME '(' new_columns ')' { $$ = sql_create_table($3, $5); }
	;

new_columns
	: name_type                { $$ = sql_new_columns_alloc($1); }
	| name_type ',' new_columns { $$ = sql_new_columns_alloc($1); $$->next = $3; }
	;

name_type
	: NAME TYPE { $$.name = $1; $$.type = $2; }
	| STRING TYPE { $$.name = $1; $$.type = $2; }
	;

TYPE
	: INT                    { $$ = sql_type_int(); }
	| REAL                   { $$ = sql_type_real(); }
	| VARCHAR '(' INTNUM ')' { $$ = sql_type_varchar($3); }
	;


create_index
	: CREATE INDEX ON NAME '(' column_list ')' { $$ = sql_create_index($4, $6); }
	| CREATE INDEX ON NAME USING NAME '(' column_list ')' {
		$$ = sql_create_index_using($4, $6, $8);
		free($6);
	}
	;

select_table
	: SELECT column_list FROM NAME            { $$ = sql_select($2, $4); }
	| SELECT column_list FROM NAME WHERE expr { $$ = sql_select_where($2, $4, $6); }
	;

column_list
	: '*'     { $$ = sql_columns_alloc(strdup("*")); }
	| columns { $$ = $1; }
	;

columns
	: NAME             { $$ = sql_columns_alloc($1); }
	| NAME ',' columns { $$ = sql_columns_alloc($1); $$->next = $3; }
	;

expr
	: NAME IS literal                  { $$ = sql_expr_is($1, $3); }
	| NAME BETWEEN literal AND literal { $$ = sql_expr_between($1, $3, $5); }
	;

literal
	: INTNUM  { $$ = sql_literal_number($1); }
	| REALNUM { $$ = sql_literal_real_number($1); }
	| STRING  { $$ = sql_literal_string($1); }
	;

insert_into_table
	: INSERT INTO NAME VALUES insert_values { $$ = sql_insert($3, $5); }
	;

insert_values
	: '(' data_list ')'                   { $$ = sql_insert_values_alloc($2); }
	| '(' data_list ')' ',' insert_values { $$ = sql_insert_values_alloc($2); $$->next = $5; }
	;

data_list
	: literal               { $$ = sql_data_list_alloc($1); }
	| literal ',' data_list { $$ = sql_data_list_alloc($1); $$->next = $3; }
	;


delete_from_table
	: DELETE FROM NAME WHERE expr { $$ = sql_delete($3, $5); }
	;


copy
	: COPY NAME FROM STRING CSV HEADER { $$ = sql_copy($2, $4); }
%%

sql_statement_tree* parse_str(const char* str)
{
	parse_str_init(str);

	sql_statement_tree* tree = NULL;
	if(yyparse(&tree) != 0)
		fprintf(stderr, "%s\n", "Bad SQL.");

	parse_str_free();

	return tree;
}

sql_statement_tree* parse_file(const char* file)
{
	sql_statement_tree* tree = NULL;

	if(parse_file_init(file))
	{
		if(yyparse(&tree) != 0)
			fprintf(stderr, "%s\n", "Bad SQL.");

	}
	parse_file_free();

	return tree;
}

void yyerror(sql_statement_tree** tree, const char* s)
{
	if(tree)
	{
		sql_statement_tree_free(*tree);
		*tree = NULL;
	}

	fprintf(stderr, "Parser error: %s\n", s);
}

int yywrap()
{
	return 1;
}
