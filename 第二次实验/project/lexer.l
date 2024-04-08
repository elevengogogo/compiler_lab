%{
#include <iostream>
#include <vector>
#include <string>

std::vector<std::string> tokens;

void addToken(const std::string& type, const std::string& lexeme) {
    tokens.push_back(type + ": " + lexeme);
}
%}

%option noyywrap

DIGIT   [0-9]
LETTER  [A-Za-z_]
ID      {LETTER}({LETTER}|{DIGIT})*

%%

"int"           { addToken("KEYWORD_INT", yytext); }
"return"        { addToken("KEYWORD_RETURN", yytext); }
"main"          { addToken("KEYWORD_MAIN", yytext); }
"+"             { addToken("OP_PLUS", yytext); }
"-"             { addToken("OP_MINUS", yytext); }
"*"             { addToken("OP_MULT", yytext); }
"/"             { addToken("OP_DIV", yytext); }
"%"             { addToken("OP_MOD", yytext); }
"<"             { addToken("OP_LT", yytext); }
"<="            { addToken("OP_LTE", yytext); }
">"             { addToken("OP_GT", yytext); }
">="            { addToken("OP_GTE", yytext); }
"=="            { addToken("OP_EQ", yytext); }
"!="            { addToken("OP_NEQ", yytext); }
"&"             { addToken("OP_AND", yytext); }
"|"             { addToken("OP_OR", yytext); }
"^"             { addToken("OP_XOR", yytext); }
";"             { addToken("PUNCT_SEMICOLON", yytext); }
"{"             { addToken("PUNCT_LBRACE", yytext); }
"}"             { addToken("PUNCT_RBRACE", yytext); }
"("             { addToken("PUNCT_LPAREN", yytext); }
")"             { addToken("PUNCT_RPAREN", yytext); }
{ID}            { addToken("IDENTIFIER", yytext); }
{DIGIT}+        { addToken("NUM", yytext); }
"="             { addToken("OP_ASSIGN", yytext); }
[ \t\n]+        { /* Ignore whitespaces */ }

%%