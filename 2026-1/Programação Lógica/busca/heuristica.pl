/*-------------------------------------------
 * BUSCA HEURISTICA.
 * Por Luiz Eduardo da Silva
 *------------------------------------------*/
ap([], X, X).
ap([X|Y], Z, [X|W]) :- 
         ap(Y, Z, W). 

membro(X, [X|_]) :- !.
membro(X, [_|Y]) :- 
         membro(X,Y).

imprima(t(_,_,T)) :-
         imprimatrajeto(T).

imprimatrajeto([r(_, RAIZ)]) :- !,
         nl,
         write('Estado Inicial : '), 
         write(RAIZ),
         write('.'), nl, nl.

imprimatrajeto([r(R, N)| RE]) :-
         imprimatrajeto(RE),
         write(R),
         write(' e portanto temos: '),
         write(N),
         write('.'), nl.

resolva :- estadoinicial(E),
           calcule_hn(E, HN), 
           busca([t(HN, 0, [ r(raiz, E) ])], S),
           imprima(S),nl,
           write('que é a solução do problema '), 
           nl, nl.

busca([T|_], S) :- 
         atingemeta(T), !, S = T.      		 
busca([T|L], S) :- 
         findall(UMAEXT, estende(T, UMAEXT), EXT),
         ap(L, EXT, L1),
         bubblesort(L1, LORD),
         busca(LORD, S).

atingemeta(t(_,_,[r(_, M)|_])) :- 
         meta(M).

estende(t(_,G, [r(R, N)| T]), t(F1,G1, [r(O, F), r(R, N) |T])) :-
        transforma(O, N, F),
        naoproduzcirculo(F, T),
        calcule_hn(F, HNF),
        calcule_custo(N, F, C),
        G1 is G + C, 
        F1 is G1 + HNF.

naoproduzcirculo(E, T) :-
         not(membro(r(_, E), T)).
        
% Predicados especificos do problema
transforma('a para b', a, b).
transforma('b para m', b, m).
transforma('m para f', m, f).
transforma('f para q', f, q).
transforma('q para p', q, p).
transforma('p para n', p, n).
transforma('p para s', p, s).
transforma('b para c', b, c).
transforma('c para d', c, d).
transforma('d para q', d, q).
transforma('d para n', d, n).
transforma('d para g', d, g).
transforma('n para h', n, h).
%transforma('n para s', n, s).
transforma('h para g', h, g).

coord(a, 2, 4) :- !.
coord(b, 5, 6) :- !.
coord(c, 4, 2) :- !.
coord(d, 7, 4) :- !.
coord(f, 7, 8) :- !.
coord(g, 8, 2) :- !.
coord(h, 10, 1) :- !.
coord(m, 9, 6) :- !.
coord(n, 11, 3) :- !.
coord(p, 12, 6) :- !.
coord(q, 11, 7) :- !.
coord(s, 13, 2).

calcule_custo(N, NF, CUSTO) :-
          coord(N, XN, YN),
          coord(NF, XF, YF),
          CUSTO is sqrt((XN-XF)*(XN-XF)+(YN-YF)*(YN-YF)).

calcule_hn(N, HN) :-  
          meta(S), 
          coord(S, XS, YS),
          coord(N, XN, YN),
          HN is sqrt((XN-XS)*(XN-XS)+(YN-YS)*(YN-YS)).

estadoinicial(a).
meta(s).

bubblesort(L,Lord) :-
         troca(L, L1), !,
         bubblesort(L1, Lord).
bubblesort(Lord, Lord).

troca([t(X,G,L),t(Y,G1,L1)|Resto], [t(Y,G1,L1),t(X,G,L)|Resto]) :-
         X > Y.
troca([Z|Resto], [Z|Resto1]) :-
         troca(Resto, Resto1).