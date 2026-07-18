-- ============================================================
-- LISTA DE EXERCÍCIOS 02 - Programação Funcional
-- Disciplina: DCE673 - UNIFAL - Prof. Eliseu
-- Aluno: Pedro Brassi Luccas
-- ============================================================

import Data.Char

-- ============================================================
-- EXERCÍCIO 1 - List comprehension para 1² + 2² + ... + 100²
-- ============================================================
somaQuadrado :: Int
somaQuadrado = sum [x^2 | x <- [1..10]]

-- ============================================================
-- EXERCÍCIO 2 - Função replicate com list comprehension
-- ============================================================
myReplicate :: Int -> a -> [a]
myReplicate n x = [x | _ <- [1..n]]

-- ============================================================
-- EXERCÍCIO 3 - Tuplas pitagóricas
-- ============================================================
pyths :: Int -> [(Int, Int, Int)]
pyths n = [(x,y,z) | x <- [1..n], y <- [1..n], z <- [1..n], x^2 + y^2 == z^2]

-- ============================================================
-- EXERCÍCIO 4 - Números perfeitos
-- ============================================================
fatores :: Int -> [Int]
fatores n = [x | x <- [1..n-1], n `mod` x == 0]

perfects :: Int -> [Int]
perfects n = [x | x <- [1..n], sum (fatores x) == x]

-- ============================================================
-- EXERCÍCIO 5 - Reescrita com duas list comprehensions de um gerador
-- ============================================================
-- Original (dois geradores):
-- Original = [(x,y) | x <- [1,2,3], y <- [4,5,6]]
-- retorna [(1,4),(2,4),(3,4),(1,5),(2,5),(3,5),(1,6),(2,6),(3,6)]

-- Reescrita com dois geradores únicos, usando concat:
pares :: [(Int,Int)]
pares = concat [[(x,y) | y <- [4,5,6], x <- [1,2,3]]]

-- ============================================================
-- EXERCÍCIO 6 - Função find usada em positions
-- ============================================================
find :: Eq b => b -> [(b, a)] -> [a]
find k t = [v | (x, v) <- t, x == k]
-- k = key / t = target / v = value (valor encontrado) / x = valor passado para teste

positions :: Eq a => a -> [a] -> [Int]
positions x xs = find x (zip xs [0..n])
	where n = (length xs) - 1

-- ============================================================
-- EXERCÍCIO 7 - Produto escalar com list comprehension
-- ============================================================
scalarproduct :: [Int] -> [Int] -> Int
scalarproduct xs ys = sum [xs !! i * ys !! i | i <- [0..length xs - 1]]

scalarproduct :: [Int] -> [Int] -> Int
scalarproduct xs ys = sum [x * y | (x,y) <- zip xs ys]

-- ============================================================
-- EXERCÍCIO 8 - Operador de exponenciação &! por recursão
-- ============================================================
(&!) :: Int -> Int -> Int
m &! 0 = 1
m &! n = m * (m &! (n-1))

{-
2 &! 3
= 2 * (2 &! 2)
= 2 * (2 * (2 &! 1))
= 2 * (2 * (2 * (2 &! 0)))
= 2 * (2 * (2 * 1))
= 8
-}

-- ============================================================
-- EXERCÍCIO 9 - Reescrita de [f x | x <- xs, p x] com map e filter
-- ============================================================
-- [f x | x <- xs, p x]  equivale a: map f (filter p xs)

-- Exemplo concreto: [(+7) x | x <- [1..10], odd x], equivale a:
-- map (+7) (filter odd [1..10])

-- ============================================================
-- EXERCÍCIO 10 - Função dec2int
-- ============================================================
dec2int :: [Int] -> Int
dec2int xs = foldl (\acc x -> acc * 10 + x) 0 xs

-- ============================================================
-- EXERCÍCIO 11 - Lista das potências de 2 com unfold
-- ============================================================
unfold p h t x
	| p x = []
	| otherwise = h x : unfold p h t (t x)

potenciasDois :: [Int]
potenciasDois = unfold (> 2^10) id (*2) 1

-- ============================================================
-- EXERCÍCIO 12 - evenCubes
-- ============================================================
evenCubes :: Int -> [Int]
evenCubes n = map (^3) (filter even [1..n])

-- ============================================================
-- EXERCÍCIO 13 -  insertOrd
-- ============================================================
insertOrd :: Int -> [Int] -> [Int]
insertOrd n xs = [x | x <- xs, x <= n] ++ [n] ++ [x | x <- xs, x > n]

-- ============================================================
-- EXERCÍCIO 14 - howManyMultiples
-- ============================================================
multiplosIntervalo :: Int -> Int -> Int -> Int
multiplosIntervalo m a b = length [x | x <- [a..b], x `mod` m == 0]

-- ============================================================
-- EXERCÍCIO 15 - duplicate
-- ============================================================
duplicate :: String -> Int -> String
duplicare s 0 = []
duplicate s n = concat [s | _ <- [1..n]]
