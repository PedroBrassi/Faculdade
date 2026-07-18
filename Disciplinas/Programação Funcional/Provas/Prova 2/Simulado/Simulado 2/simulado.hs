import Data.Char

{- 1. (20 pt) Faça, em Haskell, utilizando list comprehension, uma solução
que receba um Int n e retorne uma [Int] com todos os divisores próprios de
n (excluindo o próprio n).
-}
divisores :: Int -> [Int]
divisores n = [x | x <- [1..n-1], n `mod` x == 0]

{- 2. (20 pt) Considerando divisores, faça, em Haskell, uma solução que receba
uma [Int] ns e retorne uma [(Int,Bool)] que informa, para cada elemento
x de ns, se x é um número perfeito (soma dos divisores próprios é igual a ele
mesmo).
-}
ehPerfeito :: [Int] -> [(Int,Bool)]
ehPerfeito ns = [(x, sum(divisores x) == x) | x <- ns]

ehPerfeito_2 :: [Int] -> [(Int,Bool)]
ehPerfeito_2 = map (\x -> (x, sum(divisores x) == x))

{- 3. (20 pt) Considerando ehPerfeito, faça, em Haskell, uma solução que
receba uma [Int] ns e retorne uma ([Int],[Int]), isto é, uma dupla em que
o primeiro elemento é a lista dos números perfeitos contidos em ns, e o segundo
é a lista dos que não são perfeitos.
-}
separaPerfeitos :: [Int] -> ([Int],[Int])
separaPerfeitos ns = ([x | (x, True) <- pares], [x | (x, False) <- pares])
	where pares = ehPerfeito ns

{- 4. (20 pt) Faça, em Haskell, utilizando funções de alta ordem
(map/filter) e/ou lambda, uma solução que receba uma [String]
palavras e um Int k, retornando apenas os tamanhos das palavras de
tamanho maior ou igual a k (ou seja, retorna a [Int] com os tamanhos, não
as strings).
-}
tamanhosGrandes :: [String] -> Int -> [Int]
tamanhosGrandes s k = [length w | w <- s, length w >= k]

tamanhosGrandes_2 :: [String] -> Int -> [Int]
tamanhosGrandes_2 s k = map length (filter (\w -> length w >= k) s)

{- 5. (15 pt) Faça, em Haskell, usando foldl ou foldr, uma solução que receba
uma [Int] xs e retorne um único Int, que é o produto de todos os elementos
pares de xs (se não houver elemento par, retorne 1). 
-}
produtoPares :: [Int] -> Int
produtoPares xs = foldr (*) 1 (filter even xs)


-- ======================================================
-- ===================== Simulado 2 =====================
-- ======================================================

{- 1. (20 pt)
Faça, em Haskell, utilizando list comprehension, uma solução que receba um 
Int n e retorne uma [Int] contendo todos os múltiplos de 3 menores ou iguais a n.
-}
multiplosDe3 :: Int -> [Int]
multiplosDe3 n = [x | x <- [1..n], x `mod` 3 == 0]

{- 2. (20 pt)
Considerando a função multiplosDe3, faça uma solução que receba uma [Int] ns e r
etorne uma [(Int,Bool)] informando, para cada elemento x de ns, se x é múltiplo de 3.
-}
ehMultiploDe3 :: [Int] -> [(Int,Bool)]
ehMultiploDe3 ns = [(x, x `mod` 3 == 0) | x <- ns]

{- 3. (20 pt)
Considerando ehMultiploDe3, faça uma solução que receba uma [Int] ns e retorne 
uma dupla ([Int],[Int]), onde:
- o primeiro elemento contém os números múltiplos de 3;
- o segundo elemento contém os números que não são múltiplos de 3.
-}
separaMultiplos :: [Int] -> ([Int],[Int])
separaMultiplos ns = ([x | (x, True) <- elemento],[x | (x, False) <- elemento])
	where elemento = ehMultiploDe3 ns
	
{- 4. (20 pt)
Faça, em Haskell, utilizando funções de alta ordem (map/filter) e/ou lambda, uma solução que receba uma [String] palavras e um Int k, retornando apenas as palavras cujo tamanho é menor que k convertidas para maiúsculas.
-}
maiusculasPequenas :: [String] -> Int -> [String]
maiusculasPequenas s k = map (map toUpper) (filter (\w -> length w >= k) s)

