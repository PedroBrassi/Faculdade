{-Assunto: Litas
  Os conceitos introdutórios sobre listas foram apresentados em sala.
  Agora, considerando os casos mais simples, com apenas listas de inteiros,
  implemente as funções abaixo, considerando os operadores ++ e :
    ++ (concatena listas)
     : (insere um elemento na lista)
-}
     
{- 01 função que soma os elementos de uma lista -}
sumList::[Int]->Int
sumList [] = 0

{- 02-localiza elemento em lista -}
searchList::Int->[Int]->Bool
searchList _ _ = False

{-03 remove todas ocorrências de y em uma lista -}
deleteList::Int->[Int]->[Int]
deleteList _ _ = []

{-04 informa o tamanho de uma lista -}
lenghtList::[Int]->Int
lenghtList _ = 0

{-05 conta a ocorrência de um Int em [Int] -}
contList::Int->[Int]->Int
contList _ _ = 0

{- 06 inverte a lista -}
reverseList:: [Int]->[Int]
reverseList _ = []

{- 07 inverte elementos das listas internas -}

{- 08 função que exclui a penúltima ocorrência de um número na lista-}


-------------------------------------------------------------
{- Exercícios
     Implementar as funções: 
       myHead que recebe uma lista x e retorna a cabeça de x
       myTail que recebe uma lista x e retorna a lista x sem a cabeça
       myLast que recebe uma lista x e retorna o último elemento de x
       myInit que recebe uma lista x e retorna a lista x sem o último elemento
-}       
 
{- função que gera uma lista de booleanos relativa à comparação entre os elementos e um parâmetro z -} 
gBool :: [Int]->Int-> [Bool]
gBool [] _ = []
gBool (a:x) z = (a>z):gBool x z

{- função que reverte uma lista -}
reverte :: [a] -> [a]
reverte   []  = []
reverte (b:x) = reverte x ++ [b]
