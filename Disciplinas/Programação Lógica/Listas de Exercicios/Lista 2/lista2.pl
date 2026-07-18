/*  Segunda lista de exercícios */

/* Exercício 8: */
insOrd(X, [], [X]) :- !.
insOrd(X, [H|T], [X,H|T]) :- X =< H, !.
insOrd(X, [H|T], [H|L]) :- insOrd(X, T, L).

/* Exercício 10: */
merge([], L, L) :- !.
merge(L, [], L) :- !.
merge([H1|T1],[H2|T2],[H1|L]) :- H1 =< H2, !, merge(T1, [H2|T2], L).
merge([H1|T1],[H2|T2],[H2|L]) :- H2 < H1, merge([H1|T1], T2, L).

/* Exercício 13: */
pred1(L, R) :- findall([X], member(X,L), R).
pred2(L, R) :- findall(pred(X,X), member(X,L), R).
pred3(L, R) :- findall([element,X], member(X,L), R).

/* Exercício 14: */
transforma('encher o jarro 1', [X,Y], [3,Y]) :- X < 3.
transforma('enchar o jarro 2', [X,Y], [X,4]) :- Y < 4.

transforma('esvaziar o jarro 1', [X,Y], [0,Y]) :- X > 0.
transforma('esvaziar o jarro 2', [X,Y], [X, 0]) :- Y > 0.

% transferir do jarro 1 (cap. 3) para o jarro 2 (cap. 4)
% caso 1: tudo cabe no jarro 2
transforma('transferir do jarro 1 para o 2', [X,Y], [0,Y1]) :-
    X > 0, Y < 4, Y1 is X + Y, Y1 =< 4.

% caso 2: jarro 2 enche e sobra água no jarro 1
transforma('transferir do jarro 1 para o 2', [X,Y], [R,4]) :-
    X > 0, Y < 4, Y1 is X + Y, Y1 > 4, R is Y1 - 4.  

% transferir do jarro 2 (cap. 4) para o jarro 1 (cap. 3)
% caso 1: tudo cabe no jarro 1
transforma('transferir do jarro 2 para o 1', [X,Y], [X1,0]) :-
    X < 3, Y > 0, X1 is X + Y, X1 =< 3.


% caso 2: jarro 1 enche e sobra água no jarro 2
transforma('transferir do jarro 2 para o 1', [X, Y], [3,R]) :-
    X < 3, Y > 0, X1 is X + Y, X1 > 3, R is X1 - 3.


resolve(Estado, Estado, _, []) :- meta(Estado).
resolve(Estado, Meta, Visitados, [Acao|Resto]) :-
    transforma(Acao, Estado, Proximo),
    \+ member(Proximo, Visitados),
    resolve(Proximo, Meta, [Proximo|Visitados], Resto).

meta([_,2]).