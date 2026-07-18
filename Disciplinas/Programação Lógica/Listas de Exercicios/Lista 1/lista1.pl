/*  Primeira lista de exercícios */

/* Exercício 1: */

analisa_lista([]) :-
    write('A lista esta vazia'), nl.
analisa_lista([H|T]) :-
    write('A cabeca da lista eh: '), write(H), nl,
    write('A cauda da lista eh: '), write(T), nl.

/* Exercício 2: */

remove_duplicados([],[]).
remove_duplicados([H|T], L) :- pertence(H, T), !, remove_duplicados(T, L).
remove_duplicados([H|T], [H|L]) :- remove_duplicados(T, L).

pertence(X, [X|_]).
pertence(X, [_|T]) :- pertence(X, T).

/* Exercício 3: */

troca([], _, _, []).
troca([H|T], X, Y, [Y|L]) :- H == X, !, troca(T, X, Y, L).
troca([H|T], X, Y, [H|L]) :- troca(T, X, Y, L).

/* Exercício 4: */

subconjunto([],[]).
subconjunto([H|T], [H|L]) :- subconjunto(T, L).
subconjunto([_|T], L) :- subconjunto(T, L).

potencia(L, P) :- findall(X, subconjunto(L,X), P).

/* Exercício 5: */

mais_longa([], [_|_]).
mais_longa([_|T1], [_|T2]) :- mais_longa(T1, T2).

/* Exercício 6: */

distancia((X1,Y1), (X2,Y2), D) :-
    Dx is (X2 - X1) * (X2 - X1),
    Dy is (Y2 - Y1) * (Y2 - Y1),
    D is sqrt(Dx + Dy).

/* Exercício 7: */

quadrado(N, C) :- repeteLinhas(N, N, C).

repeteLinhas(0, _, _) :- !.
repeteLinhas(I, N, C) :-
    I > 0,
    repeteColunas(N, C), nl,
    I1 is I - 1,
    repeteLinhas(I1, N, C).

repeteColunas(0, _):- !.
repeteColunas(J, C) :-
    J > 0,
    write(C), write(' '),
    J1 is J - 1,
    repeteColunas(J1, C).

/* Exercício 8: */

elemento_n([H|_], 1, H) :- !.
elemento_n([_|T], N, X) :- N > 1, N1 is N - 1, elemento_n(T, N1, X).

/* Exercício 9: */

somatorio_contagem([], 0, 0).
somatorio_contagem([H|T], S, C) :- somatorio_contagem(T, S1, C1), S is S1 + H, C is C1 + 1.

media(L, X) :- somatorio_contagem(L, S, C), X is S / C.

/* Exercício 10: */

minimo([X], X) :- !.
minimo([H|T], M) :-
    minimo(T, M1),
    H < M1, !,
    M = H.
minimo([_|T], M) :- minimo(T, M). 

/* Exercício 11: */

intervalo(Inf, Sup, []) :- Inf > Sup, !.
intervalo(Inf, Sup, [Inf|L]) :- Inf =< Sup, Inf1 is Inf + 1, intervalo(Inf1, Sup, L).

/* Exercício 12: */

/* Exercício 13: */

ocorrencias(_, [], 0) :- !.
ocorrencias(E, [H|T], N) :- H == E, !, ocorrencias(E, T, N1), N = N1 + 1.
ocorrencias(E, [_|T], N) :- ocorrencias(E, T, N).

/* Exercício 14: */

divisores(N,L) :- findall(X, (between(1,N,X), N mod X =:= 0), L).