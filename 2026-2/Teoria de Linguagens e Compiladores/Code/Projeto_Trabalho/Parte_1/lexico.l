identificador [a-zA-Z]([a-zA-Z]|[0-9])*
numero [0-9]+
espaco [ \t]+
novalinha[\n]

%%
programa         { printf ("%11s: palavra reservada\n", yytext); }
inicio           { printf ("%11s: palavra reservada\n", yytext); }
fimprograma      { printf ("%11s: palavra reservada\n", yytext); }
leia             { printf ("%11s: palavra reservada\n", yytext); }
escreva          { printf ("%11s: palavra reservada\n", yytext); }
se               { printf ("%11s: palavra reservada\n", yytext); }
entao            { printf ("%11s: palavra reservada\n", yytext); }
senao            { printf ("%11s: palavra reservada\n", yytext); }
fimse            { printf ("%11s: palavra reservada\n", yytext); }
enquanto         { printf ("%11s: palavra reservada\n", yytext); }
faca             { printf ("%11s: palavra reservada\n", yytext); }
fimenquanto      { printf ("%11s: palavra reservada\n", yytext); }

inteiro          { printf ("%11s: tipo\n", yytext); }
logico           { printf ("%11s: tipo\n", yytext); }

V                { printf ("%11s: constante verdade\n", yytext); }
F                { printf ("%11s: constante falso\n", yytext); }

"+"              { printf ("%11s: operador de soma\n", yytext); }
"-"              { printf ("%11s: operador de subtracao\n", yytext); }
"*"              { printf ("%11s: operador de multiplicacao\n", yytext); }
div              { printf ("%11s: operador de divisao\n", yytext); }
">"              { printf ("%11s: operador maior\n", yytext); }
"<"              { printf ("%11s: operador menor\n", yytext); }
"="              { printf ("%11s: operador igual\n", yytext); }
E                { printf ("%11s: operador e-logico\n", yytext); }
OU               { printf ("%11s: operador ou-logico\n", yytext); }
NAO              { printf ("%11s: operador de negacao\n", yytext); }
"<-"             { printf ("%11s: operador de atribuicao\n", yytext); }


{identificador}  { printf ("%11s: identificador\n", yytext); }
{numero}         { printf ("%11s: numero\n", yytext); }
{espaco}         {  }
{novalinha}      {  }

.                { printf ("ERRO = SIMBOLO NAO RECONHECIDO!%s\n", yytext); }
%%

int yywrap (void) {
    return 1;
}

int main (void) {
    yylex();
    return 0;
}