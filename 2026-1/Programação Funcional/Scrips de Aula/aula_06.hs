{-Objetivo da aula 
  exercícios e introdução à List Comprehension-}

import Data.Char

-- List comprehension---------------------------------------

{-multiplica todos elementos pares da lista pela escalar -}
f1 :: Int -> [Int] -> [Int]
f1    z l = [a*z | a<-l, a `mod` 2==0]

{-dado um booleano, decide por multiplicar todos elementos por 2 ou somar todos elementos com 5 -}
f2 :: Bool -> [Int] -> [Int]
f2     b  l 
  | b           = [ a*2 |a <-l]
  | otherwise   = [ a+5 |a <-l]

{-fazer a função f0 que agrupa em duplas os cabeças de duas listas-}
f0 ::[t]->[u]->[(t,u)]
f0    _ []     = []
f0   [] _      = []
f0 (a:b) (c:d) = (a,c):f0 b d  

{-objetivo da computação dado em sala de aula:
  Filtrar o resultado de produto cartesiano 
  para gerar o resultado de f0-}  
{- gera o produto cartesiano entre duas listas -}  

f3::[t]->[u]->[(t,u)]
f3 l m = [(a,b)| a<-l, b<-m]


{- desfaz a lista de duplas gerada por f3 -}
f3_aux01 ::[(t,u)]-> ([t],[u])
f3_aux01 l = ([a|(a,b)<-l],[b|(a,b)<-l] )   

{-conta as ocorrências repetidas de fst(dupla) 
  da lista gerada por f3 -}
f3_count ::(Eq t) => [(t,u)]-> Int
f3_count []    =  0
f3_count [a]   =  1
f3_count (a:b:x)
  |fst a == fst b       = 1 + f3_count (b:x)
  |otherwise  = 1

{-solução que gera lista de listas de duplas separando as listas com duplas com mesmo fst-}
f3_newList :: (Eq u) => u-> [(u,t)]->[(u,t)]
f3_newList a l = [b | b<-l, a==fst b]

f3_newListDel :: (Eq u) => u-> [(u,t)]->[(u,t)]
f3_newListDel a l = [b|b<-l, a/=fst b]

f3_newListofList :: (Eq u) => [(u,t)] -> [[(u,t)]]
f3_newListofList   []      = []
f3_newListofList   (a:x)   = (f3_newList (fst a) (a:x)):f3_newListofList (f3_newListDel (fst a) (a:x))

{- função que gera o filtro no produto cartesiano 
   -------aqui, tem algo a ser ajustado---------}
f3_filtra :: Int->[[(u,t)]] -> [(u,t)]
f3_filtra   _   [] = []
f3_filtra     i (a:b)= busca i a:f3_filtra (i+1) b

busca :: Int->[(u,t)]->(u,t)
busca i (a:b)
  | i/=0      = busca (i-1) b
  | otherwise = a
  
--como usar a saída de f3 em uma função que retorne a computação de f0?
{-solução do Pedro -----}

f3P::(Eq t, Eq u)=>[t]->[u]->[(t,u)]
f3P l m = [(a,b)| a<-l, b<-m, f3_auxP l m a b]

f3_auxP ::(Eq t, Eq u)=> [t]->[u]->t->u->Bool
f3_auxP (a:as) (b:bs) x y = (a==x && b==y) || (f3_auxP as bs x y)
f3_auxP _ _ _ _ = False

------- Revisão -----------------------------------------------------

{-01 função que retorna lista de duplas com char e posição na ASCII -}
listaDuplaCharInt :: Int -> [(Char,Int)]
listaDuplaCharInt n
	| n < 0 	= []
	| n == 0	= [(chr 0, 0)]
	| otherwise = listaDuplaCharInt (n-1) ++ [(chr n, n)]

{-01-B função que retorna lista de duplas com char e posição na ASCII -}
{- começando em 'A' (65) -}
listaDuplaCharIntB :: Int -> [(Char,Int)]
listaDuplaCharIntB n
	| n <= 0 	= []
	| otherwise = listaDuplaCharIntB (n-1) ++ [(chr (64 + n), 64 + n)]
	
{-02 função meuChr que pesquisa um char pelo int na lista gerada -}
meuChr :: Int -> Char
meuChr n = pesquisaMeuChr n (listaDuplaCharInt n)

pesquisaMeuChr :: Int -> [(Char,Int)] -> Char
pesquisaMeuChr _ [] = '-'	-- O que colocar caso não achar???
pesquisaMeuChr n ((char,alvo) : resto)
	| n == alvo		= char
	| otherwise		= pesquisaMeuChr n resto

{-03 função meuOrd que pesquisa o int pelo char na lista gerada -}
meuOrd :: Char -> Int
meuOrd c = pesquisaMeuOrd c (listaDuplaCharInt (ord c))

pesquisaMeuOrd :: Char -> [(Char, Int)] -> Int
pesquisaMeuOrd _ []	= -1		-- O que colocar caso não achar???
pesquisaMeuOrd c ((alvo, posicao) : resto)
	| c == alvo		= posicao
	| otherwise		= pesquisaMeuOrd c resto

{-04 função que ordena uma lista de inteiros -}
ordenaLista :: [Int] -> [Int]
ordenaLista [] = []
ordenaLista (x : xs) = insertion x (ordenaLista xs)

insertion :: Int -> [Int] -> [Int]
insertion x [] = [x]
insertion x (y : ys)
	| x <= y 	= x : y : ys
	| otherwise = y : insertion x ys

{-05 seja o tipo [(Bool, [Int])]. 
Faça uma função que ordena [Int] quando o booleano é True. 
Também, passe o Bool para False, quando ordenar [Int]
exemplo: ordenaListaDupla [(True,[3,4,1,0,9]),(False,[]),(True,[4,3,2,1,0])]
retorna:                  [(False,[0,1,3,4,9]),(False,[]),(False,[0,1,2,3,4])]
-}
ordenaListaDupla :: [(Bool, [Int])] -> [(Bool, [Int])]
ordenaListaDupla [] = []
ordenaListaDupla ((teste, lista) : resto)
    | teste == True = (False, ordenaLista lista) : ordenaListaDupla resto
    | otherwise 	= (False, lista) : ordenaListaDupla resto
