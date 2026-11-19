%{
#include <stdio.h>
%}

identificador [a-zA-Z]([a-zA-Z]|[0-9])*
numero [0-9]+
espaco [ \t]+
novalinha [\n]
retornolinha [\r]

%%

programa            { printf ("%11s: palavra reservada\n", yytext); }
inicio              { printf ("%11s: palavra reservada\n", yytext); }
fimprograma         { printf ("%11s: palavra reservada\n", yytext); }
leia                { printf ("%11s: palavra reservada\n", yytext); }
escreva             { printf ("%11s: palavra reservada\n", yytext); }
se                  { printf ("%11s: palavra reservada\n", yytext); }
entao               { printf ("%11s: palavra reservada\n", yytext); }
senao               { printf ("%11s: palavra reservada\n", yytext); }
fimse               { printf ("%11s: palavra reservada\n", yytext); }
enquanto            { printf ("%11s: palavra reservada\n", yytext); }
faca                { printf ("%11s: palavra reservada\n", yytext); }
fimenquanto         { printf ("%11s: palavra reservada\n", yytext); }

inteiro             { printf ("%11s: tipo de dado\n", yytext); }
logico              { printf ("%11s: tipo de dado\n", yytext); }

V                   { printf ("%11s: valor logico\n", yytext); }
F                   { printf ("%11s: valor logico\n", yytext); }

"+"                 { printf ("%11s: operador de soma\n", yytext); }
"-"                 { printf ("%11s: operador de subtracao\n", yytext); }
"*"                 { printf ("%11s: operador de multiplicacao\n", yytext); }
div                 { printf ("%11s: operador de divisao\n", yytext); }
">"                 { printf ("%11s: operador de maior que\n", yytext); }
"<"                 { printf ("%11s: operador de menor que\n", yytext); }
"="                 { printf ("%11s: operador de igual\n", yytext); }
E                   { printf ("%11s: operador de e-logico\n", yytext); }
OU                  { printf ("%11s: operador de ou-logico\n", yytext); }
NAO                 { printf ("%11s: operador de nao-logico\n", yytext); }
"<-"                { printf ("%11s: operador de atribuicao\n", yytext); }

{identificador}     { printf ("%11s: identificador\n", yytext); }
{numero}            { printf ("%11s: numero\n", yytext); }
{espaco}            { /* ignorar */ }
{novalinha}         { printf ("%11s: nova linha\n", yytext); }
{retornolinha}      { printf ("%11s: retorno de linha\n", yytext); }


.                   { printf ("ERRO = SIMBOLO NAO RECONHECIDO!%s\n", yytext); }

%%

int yywrap() { return 1; }
int main() { yylex(); return 0; }