/* Questão 1. Qual é a saída (e o valor de L) da consulta abaixo?

d(1). d(2). d(3).
?- findall(X, (d(X), X > 1), L).
*/

% L = [2, 3].

/* Questão 2. Considere: */

teste(X) :- X > 5, !, write(grande).
teste(_) :- write(pequeno).

/* 
Qual a saída de: 
a) ?- teste(3).?
b)?- teste(3), fail.?
*/

% a) pequeno - true.
% b) pequeno - false.

/* Questão 3. Escreva o predicado intercala/3 que intercala os
elementos de duas listas de mesmo tamanho, elemento a elemento.

Exemplo:
?- intercala([1,3,5],[2,4,6],L).
L = [1,2,3,4,5,6].
*/

intercala([],[],[]).
intercala([H1|T1], [H2|T2], [H1, H2|L]) :- intercala(T1, T2, L).

/* Questão 4. Escreva o predicado achatar/2 (flatten) que transforma
uma lista de listas aninhadas em uma lista simples, sem usar o
predicado embutido flatten/2. 

Exemplo:
?- achatar([1,[2,3],[4,[5,6]],7], L).
L = [1,2,3,4,5,6,7].
*/

achatar([],[]) :- !.

achatar([H|T], L) :-
    is_list(H), !,
    achatar(H, L1),
    achatar(T, L2),
    juntar(L1, L2, L).

achatar([H|T], [H|L]) :- achatar(T, L).

juntar([], L, L).
juntar([H|T], L, [H|R]) :- juntar(T, L, R).

