%{

#include <stdio.h>
#include <stdlib.h>

extern int yylex();
extern int yyparse();
extern FILE* yyin;
extern char *yytext;
extern int yylineno;

void yyerror(const char* s);
%}

%union {
  char sval[64];
	int ival;
	float fval;
}

%token T_LINE_COMMENT T_END_DEFINITION
%token T_NODE_KEYWORD
%token<sval> T_IDENTIFIER
%token T_OPEN_STATEMENT T_CLOSE_STATEMENT
%token T_OPEN_TEMPLATE T_CLOSE_TEMPLATE

%type<sval> type

%start file

%%

file:
	   | file line
;

line: T_LINE_COMMENT
    | node_definition
;

node_definition: T_NODE_KEYWORD T_IDENTIFIER T_OPEN_STATEMENT node_members T_CLOSE_STATEMENT T_END_DEFINITION    { printf("node definition '%s'!\n", $2); }
;

node_members: node_member
            | node_members node_member

node_member: node_input
           | node_output
;

node_input: type T_IDENTIFIER T_END_DEFINITION    { printf("member definition '%s' called '%s'!\n", $1, $2); }
;

node_output: type T_IDENTIFIER T_OPEN_STATEMENT T_CLOSE_STATEMENT { printf("output definition '%s' called '%s'!\n", $1, $2); }
;

type: T_IDENTIFIER                                                { strcpy_s($$, 64, $1); }
    | T_IDENTIFIER T_OPEN_TEMPLATE T_IDENTIFIER T_CLOSE_TEMPLATE  { strcpy_s($$, 64, $1); }
;

%%
#include <stdio.h>

int main(int argv, char **argc) {

  FILE *fp;
  errno_t err = fopen_s(&fp, argc[1], "r");
  if (err) {
    exit(EXIT_FAILURE);
  }

	yyin = fp;

	do {
		yyparse();
	} while(!feof(yyin));

  return EXIT_SUCCESS;
}

void yyerror(const char* s) {
	printf("error on line %d: %s at '%s'\n", yylineno, s, yytext);
	exit(1);
}
