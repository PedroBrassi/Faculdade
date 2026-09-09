%{
int car = 0;
int lin = 0; 
int pal = 0;
%}

%%
\n         { car++; lin++;}
[^  \t\n]+ { pal++; car += yyleng;}
.          { car++; }
%%

int yywrap(){
    return 1;
}

int main (){
    yylex();
    printf("Caracteres = %d, linhas = %d, palavras = %d\n", car, lin, pal);
    return 0;
}