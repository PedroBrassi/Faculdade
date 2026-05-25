-- ============================================================
-- BLOCO 1: FUNÇÕES BÁSICAS SOBRE LISTAS DE INTEIROS
-- ============================================================
     
{- 01 função que soma os elementos de uma lista -}
sumList :: [Int] -> Int
sumList [] = 0
sumList (x : xs) = x + sumList xs

{- 02-localiza elemento em lista -}
searchList :: Int -> [Int] -> Bool
searchList _ [] = False
searchList x (y : ys)
	| x == y 	= True
	| otherwise	= searchList x ys

{-03 remove todas ocorrências de y em uma lista -}
deleteList :: Int -> [Int] -> [Int]
deleteList _ [] 	= []
deleteList x (y : ys)
	| x == y 		= deleteList x ys
	| otherwise 	= y : deleteList x ys

{-04 informa o tamanho de uma lista -}
lenghtList :: [Int] -> Int
lenghtList [] 		= 0
lenghtList (x:xs) 	= 1 + lenghtList xs

{-05 conta a ocorrência de um Int em [Int] -}
contList :: Int -> [Int] -> Int
contList _ [] = 0
contList x (y : ys)
	| x == y 	= 1 + contList x ys
	| otherwise	= contList x ys

{- 06 inverte a lista -}
reverseList :: [Int] -> [Int]
reverseList [] = []
reverseList (x : xs) = reverseList xs ++ [x]

{- 07 inverte elementos das listas internas -}
reverseInList :: [[Int]] -> [[Int]]
reverseInList [] = []
reverseInList (x : xs) = reverseList x : reverseInList xs

{- 08 função que exclui a penúltima ocorrência de um número na lista-}
removeNesima :: Int -> Int -> [Int] -> [Int]
removeNesima _ _ [] = []
removeNesima x 1 (y : ys)
	| x == y 	= ys
	| otherwise	= y : removeNesima x 1 ys
removeNesima x n (y : ys)
	| x == y 	= y : removeNesima x (n-1) ys
	| otherwise	= y : removeNesima x n ys
	
deletePen :: Int -> [Int] -> [Int]
deletePen x lista
	| n < 2 	= lista
	| otherwise	= removeNesima x (n-1) lista
	where n = contList x lista

-- ============================================================
-- BLOCO 2: myHead, myTail, myLast, myInit e gBool
-- ============================================================

{- myHead que recebe uma lista x e retorna a cabeça de x -}
myHead :: [Int] -> Int
myHead (x : _) = x

{- myTail que recebe uma lista x e retorna a lista x sem a cabeça -}
myTail :: [Int] -> [Int]
myTail [] = []
myTail (_ : xs) = xs

{- myLast que recebe uma lista x e retorna o último elemento de x -}
myLast :: [Int] -> Int
myLast [y] = y
myLast (_ : xs) = myLast xs

{- myInit que recebe uma lista x e retorna a lista x sem o último elemento -}
myInit :: [Int] -> [Int]
myInit [_] = []
myInit (x : xs) = x : myInit xs

{- função que gera uma lista de booleanos relativa à comparação entre os elementos e um parâmetro z -}
gBool :: [Int] -> Int -> [Bool]
gBool [] _ = []
gBool (x : xs) z = (x == z) : gBool xs z

{- função que reverte uma lista -}
reverse_2 :: [Int] -> [Int]
reverse_2 [] = []
reverse_2 (x : xs) = reverse xs ++ [x]

-- ============================================================
-- BLOCO 3: LISTAS E TUPLAS (VENDAS)
-- ============================================================

periodo :: Int
periodo = 7
 
maxi :: Int -> Int -> Int
maxi m n
  | m >= n    = m
  | otherwise = n
 
vendas :: Int -> Int
vendas 0 = 0
vendas 1 = 41
vendas 2 = 72
vendas 3 = 48
vendas 4 = 0
vendas 5 = 91
vendas 6 = 55
vendas 7 = 30

{- 01 função que retorna uma lista de vendas até o dia n -}
listaVendas :: Int -> [Int]
listaVendas 0 = [vendas 0]
listaVendas x = listaVendas (x - 1) ++ [vendas x]

{- 02 função que retorna [[Int]] com listas de dia e venda até o dia n -}
funcao_02 :: Int -> [[Int]]
funcao_02 0 = []
funcao_02 x = funcao_02 (x - 1) ++ [[x, vendas x]]

{- 03 função que ordena uma lista de inteiros -}
ordenaLista :: [Int] -> [Int]
ordenaLista [] = []
ordenaLista (x : xs) = insere x (ordenaLista xs)

insere :: Int -> [Int] -> [Int]
insere x [] = [x]
insere x (y : ys)
	| x <= y	= x : y : ys
	| otherwise	= y : insere x ys

{- 03.1 função que ordena uma lista de inteiros e retira os repetidos -}
ordenaLimpa :: [Int] -> [Int]
ordenaLimpa x = removeRepetidos (ordenaLista x)

removeRepetidos :: [Int] -> [Int]
removeRepetidos [] 	= []
removeRepetidos [x] = [x]
removeRepetidos (x : y : ys)
	| x == y	= removeRepetidos (y : ys)
	| otherwise	= x : removeRepetidos (y : ys)
	
{- 04 função que ordena [[Int]] pelo primeiro Int de cada lista  -}
ordenaListaLista :: [[Int]] -> [[Int]]
ordenaListaLista [] = []
ordenaListaLista [x] = [x]
ordenaListaLista (x : xs)
	| ordenaLista 
