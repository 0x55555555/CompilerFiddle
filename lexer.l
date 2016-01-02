%{
#include <stdio.h>
#include <stdlib.h>

#define YY_DECL int yylex()

#include "parser.h"

%}

%option noyywrap
%option yylineno

%%

[ \t\r\n]	; // ignore all whitespace
\/\/.*      { printf("got a line comment\n"); return T_LINE_COMMENT; }
node		    { printf("got an node keyword\n"); return T_NODE_KEYWORD; } 
\{          { printf("got an open keyword\n"); return T_OPEN_STATEMENT; }
\}          { printf("got an close keyword\n"); return T_CLOSE_STATEMENT; }
\<          { printf("got an open t keyword\n"); return T_OPEN_TEMPLATE; }
\>          { printf("got an close t keyword\n"); return T_CLOSE_TEMPLATE; }
;           { return T_END_DEFINITION; }

[a-z]+   { strcpy_s(yylval.sval, 64, yytext); printf("got a name for a node '%s'\n", yytext); return T_IDENTIFIER; }

%%