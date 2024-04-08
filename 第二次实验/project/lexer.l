%{
#include "y.tab.h"
%}

%%

"int"                   { return INT; }
"return"                { return RETURN; }
"main"                  { return MAIN; }

[a-zA-Z_][a-zA-Z0-9_]*  { yylval.sval = strdup(yytext); return ID; }

[0-9]+                  { yylval.ival = atoi(yytext); return NUM; }

"="                     { return ASSIGN; }
"+"                     { return PLUS; }
"-"                     { return MINUS; }
"*"                     { return MUL; }
"/"                     { return DIV; }
"%"                     { return MOD; }
"<"                     { return LT; }
"<="                    { return LE; }
">"                     { return GT; }
">="                    { return GE; }
"=="                    { return EQ; }
"!="                    { return NE; }
"&"                     { return AND; }
"|"                     { return OR; }
"^"                     { return XOR; }

";"                     { return SEMICOLON; }
"{"                     { return LBRACE; }
"}"                     { return RBRACE; }

[ \t\n]+                { /* ignore whitespace */ }

.                       { /* catch all */ }

%%

int yywrap() { return 1; }
