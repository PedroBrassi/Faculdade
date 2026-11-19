%{
#include "sintatico.h"
int nlinha = 1;
char atomo[30];
void yyerror(const char *);

%}

identificador [a-zA-Z]([a-zA-Z]|[0-9])*
numero [0-9]+
espaco [ \t]+
novalinha [\n]

%%
programa          { return T_PROG; }
inicio            { return T_INICIO; }
fimprograma       { return T_FIM; }
leia              { return T_LEIA; }
escreva           { return T_ESCREVA; }
se                { return T_SE; }
entao             { return T_ENTAO; }
senao             { return T_SENAO; }
fimse             { return T_FIMSE; }
enquanto          { return T_ENQTO; }
faca              { return T_FACA; }
fimenquanto       { return T_FIMENQTO; }

inteiro           { return T_INTEIRO; }
logico            { return T_LOGICO; }

V                 { return T_V; }
F                 { return T_F; }

"+"               { return T_MAIS; }
"-"               { return T_MENOS; }
"*"               { return T_VEZES; }
div               { return T_DIV; }
">"               { return T_MAIOR; }
"<"               { return T_MENOR; }
"="               { return T_IGUAL; }
E                 { return T_E; }
OU                { return T_OU; }
NAO               { return T_NAO; }
"<-"              { return T_ATRIB; }
"("               { return T_ABRE;}
")"               { return T_FECHA; }

{identificador}   { 
                     strcpy(atomo, yytext);
                     return T_IDENTIF; 
                  }
{numero}          { 
                     strcpy(atomo, yytext);
                     return T_NUMERO; 
                  }
{espaco}          {  }
{novalinha}       {  nlinha++; }

.                 { 
                      char msg[100];
                      sprintf(msg, "  símbolo %s desconhecido!", yytext);
                      yyerror(msg); 
                  }
%%


int yywrap() {
  return 1;
}