%{
#include <stdio.h>
#include <stdlib.h>
int yylex(void);
void yyerror(char *);
%}

%token ENTER
%token MAIS
%token MENOS
%token VEZES
%token DIV
%token NUMERO
%token ABRE
%token FECHA

%start calculo

%left MAIS MENOS 
%left VEZES DIV

%%

calculo: calculo expr ENTER { printf ("resultado = %d\n", $2); }
       | 
       ;

expr : expr MAIS expr { $$ = $1 + $3; }
     | expr MENOS expr { $$ = $1 - $3; }
     | expr VEZES expr { $$ = $1 * $3; }
     | expr DIV expr   { $$ = $1 / $3; }
     | ABRE expr FECHA { $$ = $2; }
     | NUMERO { $$ = $1; }
     ;

%%

void yyerror (char *msg) {
    printf("Erro: %s\n", msg);
} 

int main() {
    yyparse();
    return 0;
}