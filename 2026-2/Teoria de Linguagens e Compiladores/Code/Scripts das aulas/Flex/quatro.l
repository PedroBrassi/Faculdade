digit [0-9]
letter[A-Za-z]

%{
int count = 0;
%}

%%
{letter}({letter}|{digit})* { ECHO; count++;}
\n
%%

int yywrap(){
    return 1;
}

int main (void) {
    yylex();
    printf("Quantidade de identificadores = %d\n", count);
    return 0;
}