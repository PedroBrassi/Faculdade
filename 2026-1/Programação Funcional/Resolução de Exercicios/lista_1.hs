{- 
01 Soma condicional
Faça, em Haskell, uma função que receba um Int n e uma [Int] e retorne a soma apenas dos elementos estritamente maiores que n.
-}
somaM :: Int -> [Int] -> Int
somaM _ [] = 0
somaM n (x : xs)
	| x > n 	= x + somaM n xs
	| otherwise	= somaM n xs

{-
02 Filtragem em lista de listas
Considerando somaM, faça uma função que receba um Int n e uma [[Int]] e retorne uma [[Int]] contendo apenas as sublistas cuja soma dos elementos maiores que n supera n.
-}
filtraS :: Int -> [[Int]] -> [[Int]]
filtraS _ [] = []
filtraS n (l: ls)
	| somaM n l > n = l : filtraS n ls
	| otherwise		= filtraS n ls

{- 
03 Contagem por predicado
Faça uma função que receba uma [(Int, Bool)] e retorne um par (Int, Int) onde o primeiro elemento é a quantidade de pares cujo Bool é True e o segundo é a quantidade cujo Bool é False.
-}
contBool :: [(Int, Bool)] -> (Int, Int)
contBool [] = (0,0)
contBool ((_, teste): ls) 
	| teste 	= (v+1, f)
	| otherwise	= (v, f+1)
	where (v,f) = contBool ls

{- 
04 Intercalação de listas
Faça uma função que receba duas [Int] e retorne uma [Int] com os elementos intercalados (primeiro da lista 1, primeiro da lista 2, segundo da lista 1, etc.). Quando uma lista terminar, concatene o restante da outra.
-}
intercala :: [Int] -> [Int] -> [Int]
intercala [] ys = ys
intercala xs [] = xs
intercala (x : xs) (y : ys) = x : y : intercala xs ys

{-
05 Tupla de extremos
Faça uma função que receba uma [Int] não-vazia e retorne uma tupla (Int, Int) contendo o menor e o maior elemento da lista, respectivamente.
-}
menor :: Int -> Int -> Int
menor a b
	| a < b		= a
	| otherwise	= b

maior :: Int -> Int -> Int
maior a b
	| a > b 	= a
	| otherwise = b

extremos :: [Int] -> (Int, Int)
extremos [x] = (x, x)
extremos (x : xs) = (menor x mn, maior x mx)
	where (mn, mx) = extremos xs	

{- 
06 Rotação de lista
Faça uma função que receba um Int k e uma [Int] e retorne a lista rotacionada k posições à esquerda. Considere que k pode ser maior que o tamanho da lista.

rotaciona :: Int -> [Int] -> [Int]
rotaciona _ [] = []


--Exemplo: rotaciona 2 [1,2,3,4,5] = [3,4,5,1,2]
-}

{- 
07 Classificação em tupla 
Faça uma função que receba uma [Int] e retorne uma ([Int], [Int], [Int]) onde a primeira lista contém os negativos, a segunda os zeros e a terceira os positivos da lista original, mantendo a ordem relativa.
-}

classifica :: [Int] -> ([Int], [Int], [Int])
classifica [] = ([],[],[])
classifica (x : xs)
	| x < 0		= (x:neg, zero, pos)
	| x == 0	= (neg, x:zero, pos)
	| otherwise = (neg, zero, x:pos)
	where (neg, zero, pos) = classifica xs

{- 
08 Lookup em lista de tuplas
Faça uma função que receba uma String (chave) e uma [(String, Int)] (tabela chave-valor) e retorne o Int associado à chave. Se a chave não existir, retorne -1.
-}
busca :: String -> [(String, Int)] -> Int
busca _ [] = -1
busca c ((s, v) : resto)
	| c == s	= v
	| otherwise = busca c resto

{-
09 Agrupamento em sublistas
Faça uma função que receba um Int n e uma [Int] e retorne uma [[Int]] onde cada sublista contém exatamente n elementos consecutivos. Se a lista não for divisível por n, a última sublista conterá os elementos restantes.
-}
-- Exemplo: agrupa 3 [1,2,3,4,5,6,7] = [[1,2,3],[4,5,6],[7]]
agrupa :: Int -> [Int] -> [[Int]]
agrupa _ [] = []
agrupa n lista
    | n <= 0    = []
    | otherwise = pegarN n lista : agrupa n (pularN n lista)

-- Pega os primeiros n elementos (ou menos se a lista acabar)
pegarN :: Int -> [Int] -> [Int]
pegarN _ [] = []
pegarN 0 _  = []
pegarN n (x:xs) = x : pegarN (n-1) xs

-- Pula os primeiros n elementos
pularN :: Int -> [Int] -> [Int]
pularN _ [] = []
pularN 0 lista = lista
pularN n (x:xs) = pularN (n-1) xs

{- Comparação de listas de tuplas
Faça uma função que receba duas listas [(String, Int)] com as mesmas chaves (possivelmente em ordens diferentes) e retorne uma [(String, Int, Int, Bool)] onde cada elemento contém a chave, o valor da primeira lista, o valor da segunda lista e um Bool indicando se os valores são iguais.
-}
 -- compara :: [(String,Int)] -> [(String,Int)] -> [(String,Int,Int,Bool)]

