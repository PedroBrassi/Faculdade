%{
#include "calc.h"
void yyerror(char *);
%}

%%
[0-9]+     {  
              yylval = atoi(yytext);
              return NUMERO;
           }
"+"        return MAIS;
"-"        return MENOS;
"*"        return VEZES;
"/"        return DIV;
"("        return ABRE;
")"        return FECHA;
[ \t]+     ;
\n         return ENTER;
.          {
             char msg[100];
             sprintf(msg, "[%s] desconhecido!", yytext);
             yyerror(msg);
           }
%%

int yywrap () {
    return 1;
}