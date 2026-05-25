%------------------------------------------
% Busca em largura/profundidade
% Por Luiz Eduardo
%------------------------------------------
ap([],X,X).
ap([A|B],C,[A|D]) :- ap(B,C,D).

in(A,[A|_]) :- !.
in(A,[_|B]) :- in(A,B).

% Implementacao do algoritmo de busca em largura ou profundidade
% 1 = largura
% 2 = profundidade
estrategia(2).

atingemeta([_-E|_]) :- meta(E).

busca([Caminho|_], Solucao) :- atingemeta(Caminho), !, Solucao = Caminho.
busca([Caminho|Lista], Solucao) :- 
   findall(UMAEXT, estende(Caminho,UMAEXT), EXT),
   estrategia(Tipo),
   estrategia(Tipo),
   (Tipo = 1 -> ap(Lista, EXT,  Lista1);
    Tipo = 2 -> ap(EXT, Lista, Lista1)),
   busca(Lista1, Solucao).

naorepete(_-E,C) :- not(in(_-E,C)).

estende([OperacaoX-EstadoA|Caminho], [OperacaoY-EstadoB,OperacaoX-EstadoA|Caminho]) :-
   oper(OperacaoY,EstadoA,EstadoB),
   naorepete(OperacaoY-EstadoB,Caminho).

margem([F,L,C,R], M) :-
   (F = M -> write('F'); write(' ')),
   (L = M -> write('L'); write(' ')),
   (C = M -> write('C'); write(' ')),
   (R = M -> write('R'); write(' ')).

desenha(Estado) :-
     write('    '), margem(Estado, a), write('|    |'), margem(Estado,b).

escreve([_-E]) :- write('Estado Inicial: '), write(E), nl, !.
escreve([O-E|R]) :- 
    escreve(R), 
    write('Executando: '), 
    traduz(O,T),
    write(T), write(' obtemos '), /*desenha(E),*/ write(E), nl.

resolva :-
    inicial(X), 
    busca([[raiz-X]],S), 
    write(S), nl,
    escreve(S),
    write('que é a solução do problema').

%-----------------------------------
% Especificacao do problema
%-----------------------------------

traduz(c1, 'encher o jarro 1  ').
traduz(c2, 'encher o jarro 2  ').
traduz(v1, 'esvaziar o jarro 1').
traduz(v2, 'esvaziar o jarro 2').
traduz(12, 'despejar 1 em 2   ').
traduz(21, 'despejar 2 em 1   ').
inicial([0,0]).
meta([_,2]).
oper(c1, [X,Y], [3,Y]) :- X < 3.
oper(c2, [X,Y], [X,4]) :- Y < 4.
oper(v1, [X,Y], [0,Y]) :- X > 0.
oper(v2, [X,Y], [X,0]) :- Y > 0.
oper(12, [X,Y], [0,Y1]) :- X > 0, Y < 4, Y1 is X + Y, Y1 =< 4.
oper(12, [X,Y], [X1,4]) :- X > 0, Y < 4, X1 is X + Y - 4, X + Y > 4.
oper(21, [X,Y], [X1,0]) :- Y > 0, X < 3, X1 is X + Y, X1 =< 3.
oper(21, [X,Y], [3,Y1]) :- Y > 0, X < 3, Y1 is X + Y - 3, X + Y > 3.

% traduz(fa, 'fazendeiro vai sozinho   ').
% traduz(fb, 'fazendeiro volta sozinho ').
% traduz(la, 'fazendeiro leva o lobo   ').
% traduz(lb, 'fazendeiro traz o lobo   ').
% traduz(ca, 'fazendeiro leva a cabra  ').
% traduz(cb, 'fazendeiro traz a cabra  ').
% traduz(ra, 'fazendeiro leva o repolho').
% traduz(rb, 'fazendeiro traz o repolho').

% oper(fa, [a,L,C,R], [b,L,C,R]) :- L \= C, C \= R.
% oper(fb, [b,L,C,R], [a,L,C,R]) :- L \= C, C \= R.
% oper(la, [a,a,C,R], [b,b,C,R]) :- (C \= R; C = b).
% oper(lb, [b,b,C,R], [a,a,C,R]) :- (C \= R; C = a).
% oper(ca, [a,L,a,R], [b,L,b,R]).
% oper(cb, [b,L,b,R], [a,L,a,R]).
% oper(ra, [a,L,C,a], [b,L,C,b]) :- (L \= C; C = b).
% oper(rb, [b,L,C,b], [a,L,C,a]) :- (L \= C; C = a).

% % [F,L,C,R], margem em que está cada elemento
% inicial([a,a,a,a]).
% meta([b,b,b,b]).