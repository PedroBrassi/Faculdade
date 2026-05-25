{-Assunto: Litas
  Os conceitos introdutórios sobre listas foram apresentados em sala.
  Agora, considerando os casos mais simples, com apenas listas de inteiros,
  implemente as funções abaixo, considerando os operadores ++ e :
    ++ (concatena listas)
     : (insere um elemento na lista)
-}
     
{- 01 função que soma os elementos de uma lista -}
sumList :: [Int] -> Int
sumList [] = 0
sumList (x : c) = x + sumList c

{- 02-localiza elemento em lista -}
searchList :: Int -> [Int] -> Bool
searchList _ _ = False

{-03 remove todas ocorrências de y em uma lista -}
deleteList::Int->[Int]->[Int]
deleteList _ _ = []

{-04 informa o tamanho de uma lista -}
lenghtList :: [Int] -> Int
lenghtList [] = 0
lenghList ( _ : xs) = 1 + lenghtList xs

{-05 conta a ocorrência de um Int em [Int] -}
contList :: [Int] -> Int -> Int
contList [] _ = 0
contList (x : xs) numero 
	| numero == x	= 1 + contList xs numero
	| otherwise 	= 0 + contList xs numero

{- 06 inverte a lista -}
reverseList :: [Int] -> [Int]
reverseList _ = []

{- 07 inverte elementos das listas internas -}

{- 08 função que exclui a penúltima ocorrência de um número na lista-}
excluiPenultima :: [Int] -> Int -> [Int]
excluiPenultima [] _ = []
excluiPenultima (x : xs) numero
	| x == numero && contList xs numero == 1 = xs
	| otherwise 				 = x : excluiPenultima xs numero

{- 09 função que exclui o penúltimo elemento da lista -}
removePenultimo :: [Int] -> [Int]
removePenultimo [] = []
removePenultimo [a] = [a]
removePenultimo (a : b : c)					--comparando os 3 primeiros numeros;
	| c == [] 	= [b]					-- length c == 0
	| otherwise 	= a : removePenultimo (b:c)

-------------------------------------------------------------
{- Exercícios
     Implementar as funções: 
       myHead que recebe uma lista x e retorna a cabeça de x
       myTail que recebe uma lista x e retorna a lista x sem a cabeça
       myLast que recebe uma lista x e retorna o último elemento de x
       myInit que recebe uma lista x e retorna a lista x sem o último elemento
-}       
 
{- função que gera uma lista de booleanos relativa à comparação entre os elementos e um parâmetro z -} 
gBool :: [Int] -> Int-> [Bool]
gBool [] _ = []
gBool (a:x) z = (a>z) : gBool x z

{- função que reverte uma lista -}
reverte :: [a] -> [a]
reverte   []  = []
reverte (b:x) = reverte x ++ [b]

{- função que retira 
