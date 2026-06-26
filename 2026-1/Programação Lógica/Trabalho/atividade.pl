% ================================================
%        UNIFAL - Universidade Federal de Alfenas.
%        BACHARELADO EM CIENCIA DA COMPUTACAO.
% Trabalho..: Atividade 1 – Ordenação de listas de listas
% Professor.: Luiz Eduardo da Silva
% Aluno.....: Pedro Brassi Luccas
% Data......: 26/06/2026
% ================================================


% ================================================
% Parte A - Ordenar pelo tamanho das sublistas

% Estratégia: Insere -> Ordena -> Remove
%   1. Calcula o tamanho de cada sublista
%   2. Cria pares (Tamanho, Sublista)
%   3. Ordena os pares por Tamanho (insertion sort)
%   4. Remove os tamanhos, retornando só as sublistas

% ================================================

% 1 - Calcula o tamanho de uma lista:
% N é o número de elementos de Lista
tamanho([], 0).
tamanho([_|T], N) :-
    tamanho(T, N1),
    N is N1 + 1.

% 2 - Transforma uma lista de sublistas em uma lista de pares (Tamanho, Sublista).
tamanhoLista([], []).
tamanhoLista([L|Ls], [(N,L)|R]) :-
    tamanho(L, N),
    tamanhoLista(Ls, R).

% 3 - Inserção ordenada por comprimento:
% Insere o par (N,L) na posição correta de uma lista já ordenada por N.
inserirOrdenado((N,L), [], [(N,L)]).
inserirOrdenado((N,L), [(N1,L1)|T], [(N,L),(N1,L1)|T]) :- N =< N1.
inserirOrdenado((N,L), [(N1,L1)|T], [(N1,L1)|R]) :-
    N > N1,
    inserirOrdenado((N,L), T, R).

% 4 - Ordena uma lista de pares (Tamanho, Sublista) pelo tamanho,
insertionSort([], []).
insertionSort([H|T], Ordenado) :-
    insertionSort(T, OrdenadoT),
    inserirOrdenado(H, OrdenadoT, Ordenado).

% 4 - Remove os comprimentos
removeTamanho([], []).
removeTamanho([(_,L)|T], [L|R]) :-removeTamanho(T, R).

% 5 - lsort/2
% Sublistas mais curtas aparecem primeiro. Duplicatas são preservadas.
lsort(InList, OutList) :-
    tamanhoLista(InList, Pares),
    insertionSort(Pares, ParesOrdenados),
    removeTamanho(ParesOrdenados, OutList).

% ================================================
% Parte B - Ordenar pela frequência dos comprimentos

% Estratégia: Insere → Ordena → Remove
%   1. Calcula o tamanho de cada sublista
%   2. Conta quantas sublistas têm cada tamanho (frequência)
%   3. Cria triplas (Frequência, Tamanho, Sublista)
%   4. Ordena por Frequência; desempata pelo Tamanho
%   5. Remove frequência e tamanho, retornando só as sublistas
% ================================================

% 1 - Conta a ocorrencia de um comprimento:
% N é o número de vezes que X aparece em Lista.
frequencia(_, [], 0).
frequencia(X, [X|T], N) :-
    frequencia(X, T, N1),
    N is N1 + 1.
frequencia(X, [Y|T], N) :-
    X \= Y,
    frequencia(X, T, N).

% 2 - Obtém lista de comprimentos:
% Ns é a lista dos comprimentos de cada sublista em Listas.
comprimentos([], []).
comprimentos([L|Ls], [N|Ns]) :-
    tamanho(L, N),
    comprimentos(Ls, Ns).

% 3 - Associa frequência a cada lista, no formato : (Freq, Comprimento, Lista):
frequenciaLista([], _, []).
frequenciaLista([L|Ls], TodosComprimentos, [(Freq,Comp,L)|R]) :-
    tamanho(L, Comp),
    frequencia(Comp, TodosComprimentos, Freq),
    frequenciaLista(Ls, TodosComprimentos, R).

% 4 - Inserção ordenada da tripla (F,Ln,L) por frequência desempate usa o comprimento (menor primeiro):
insert_freq((F,Ln,L), [], [(F,Ln,L)]).
insert_freq((F,Ln,L), [(F1,Ln1,L1)|T], [(F,Ln,L),(F1,Ln1,L1)|T]) :-
    (F < F1 ; (F =:= F1, Ln =< Ln1)).
insert_freq((F,Ln,L), [(F1,Ln1,L1)|T], [(F1,Ln1,L1)|R]) :-
    (F > F1 ; (F =:= F1, Ln >= Ln1)),
    insert_freq((F,Ln,L), T, R).

% 5 - Ordenação por frequência crescente:
ordenaFreq([], []).
ordenaFreq([H|T], Ordenado) :-
    ordenaFreq(T, OrdenadoT),
    insert_freq(H, OrdenadoT, Ordenado).

% 6 - Remove frequência e comprimento:
% Extrai apenas as sublistas de uma lista de triplas (Freq, Comp, Sublista).
limpaLista([], []).
limpaLista([(_,_,L)|T], [L|R]) :- limpaLista(T, R).

% 7 - lfsort/2
lfsort(InList, OutList) :-
    comprimentos(InList, TodosComprimentos),
    frequenciaLista(InList, TodosComprimentos, Triplas),
    ordenaFreq(Triplas, TripasOrdenadas),
    limpaLista(TripasOrdenadas, OutList).