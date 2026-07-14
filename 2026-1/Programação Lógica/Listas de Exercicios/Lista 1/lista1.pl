% Primeira lista de exercícios
/*
Exercício 1:
Escreva o predicado Prolog analisa_lista/1 que toma uma lista como argumento e escreve
sua cabeça e cauda na tela .Se a lista está vazia, o predicado deve escrever uma
mensagem.

Exemplos de uso:
?- analisa_lista([dog, cat, horse, cow]).
A cabeca da lista eh: dog
A cauda da lista eh: [cat, horse, cow]
?- analisa_lisa([]).
A lista esta vazia
*/

analisa_lista([]) :-
    write('A lista esta vazia'), nl.
analisa_lista([H|T]) :-
    write('A cabeca da lista eh: '), write(H), nl,
    write('A cauda da lista eh: '), write(T), nl.

/*
Exercício 2:
Implemente o predicado Prolog remove_duplicados/2 que remove todos os elementos
duplicados de uma lista dada como primeiro argumento e retorna o resultado no segundo
argumento.

Exemplo de uso:
?- remove_duplicados([a, b, a, c, d, d], List).
List = [b, a, c, d]
*/

pertence(X,[X|_]).
pertence(X,[_|T]) :- pertence(X,T).

remove_duplicados([],[]).
remove_duplicados([H|T], L) :- pertence(H, T), !, remove_duplicados(T,L).
remove_duplicados([H|T], [H|L]) :- remove_duplicados(T,L).

/*
Exercício 3:
Escreva o predicado Prolog troca/4 que troca todas as ocorrências de um dado elemento
(segundo argumento), por outro elemento (terceiro argumento) de uma lista (primeiro
argumento). 

Exemplo de uso:
?- troca([1, 2, 3, 4, 3, 5, 6, 3], 3, x, List).
List = [1, 2, x, 4, x, 5, 6, x]

*/

troca([], _, _, []).
troca([H|T], X, Y, [Y|L]) :- H == X, !, troca(T, X, Y, L).
troca([H|T], X, Y, [H|L]) :- troca(T, X, Y, L).

/*
Exercício 4:
Uma lista Prolog sem valores duplicados pode ser usada para representar um conjunto.
Escreva um predicado que dado uma lista que representa um conjunto C, produza uma
lista de lista que é o conjunto potência de C. 

Exemplo de uso:
?- potencia([a, b, c], P).
P = [[a, b, c], [a, b], [a, c], [a], [b, c], [b], [c], []]
*/

concat([], L, L).
concat([H|T], L, [H|R]) :- concat(T, L, R).

adiciona(_, [], []).
adiciona(X, [H|T], [[X|H]|R]) :- adiciona(X, T, R).

potencia([], [[]]).
potencia([H|T], P) :-
    potencia(T, PT),
    adiciona(H, PT, ComH),
    concat(ComH, PT, P).

/*
Exercício 5:
Escreva o predicado Prolog mais_longa/2 que recebe duas listas como argumentos e
verifica se a segunda lista é mais longa (tem mais elementos) que a primeira lista.
Implementar sem usar qualquer operação aritmética (contar o tamanho das listas, por
exemplo). 

Exemplo de uso:
?- mais_longa([dog,cat,snake], [giraffe,elephant,lion,tiger]).
True

?- mais_longa([1,2,3,4,5], []).
False
*/


mais_longa([],[_|_]).
mais_longa([_|T1],[_|T2]) :- mais_longa(T1, T2).