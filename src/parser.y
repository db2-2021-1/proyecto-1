%{
#include <stdio.h>

int yylex();
void yyerror(const char* s);

%}

%union {
	int intval;
	double floatval;
	char *strval;
	int subtok;
}

%left AND

%token <strval> NAME;

%token BETWEEN
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

select_table: SELECT column_list FROM NAME;

column_list: '*' | columns;

columns: NAME
	| columns ',' NAME
	;
%%

//int main()
//{
//	if(!yyparse())
//	{
//		printf("%s\n", "ok");
//	}
//	else
//	{
//		printf("%s\n", "no");
//	}
//}

void yyerror(const char* s)
{
	fprintf(stderr, "%s\n", s);
}

int yywrap()
{
	return 1;
}
