import Data.Char

-- Prova — Lista 2
{-
Questão 1
Implemente a função scalarproduct :: [Int] -> [Int] -> Int que calcula o produto escalar de duas listas de inteiros usando list comprehension. Em seguida, reescreva a mesma função usando apenas funções de alta ordem (sem list comprehension), justificando a equivalência.
-}
--	Main> scalarproduct [1,2,3] [4,5,6] = 32


{-
Questão 2
Considere a função unfold definida abaixo. Usando-a, defina a função dec2bin :: Int -> [Int] que converte um inteiro positivo para sua representação em binário (lista de 0s e 1s, do bit mais significativo ao menos significativo). Mostre o passo a passo da avaliação de dec2bin 6.

unfold p h t x
  | p x       = []
  | otherwise = h x : unfold p h t (t x)
-}

--	Main> dec2bin 6 = [1,1,0]

{-
Questão 3
Defina a função proliferaChar :: String -> String que, dada uma lista de caracteres maiúsculos, retorna uma string onde cada caractere é repetido um número de vezes igual à sua posição no alfabeto (A=1, B=2, ..., Z=26). Em seguida, defina também separa :: [Int] -> ([Int],[Int]) que separa os elementos ímpares dos pares de uma lista. Ambas devem usar obrigatoriamente list comprehension.
-}
--	Main> proliferaChar "CBD" = "CCCBBDDDlengthD"
--	Main> separa [1,4,3,4,6,7,9,10] = ([1,3,7,9],[4,4,6,10])


{-
Questão 4
Analise as duas implementações de busca em lista abaixo e responda:

procuraElemento :: Int -> [Int] -> Bool
procuraElemento n (x:xs) = n == x || procuraElemento n xs

procuraElemento2 :: Int -> [Int] -> Bool
procuraElemento2 n (x:xs) = procuraElemento n xs || n == x
-}

{- (a) Ambas estão corretas? Qual a diferença na computação entre as duas? Considere a avaliação lazy do Haskell. -}

{- (b) Qual é mais eficiente e por quê? Explique usando o conceito de short-circuit evaluation. -}

{- (c) Generalize as funções para trabalhar com qualquer tipo usando a classe Eq. O que muda na assinatura de tipo?-}

{-
Questão 5
-}
{- (a) Defina o operador (&!) :: Int -> Int -> Int de exponenciação para inteiros não negativos, seguindo o mesmo padrão de recursividade do operador de multiplicação. Mostre o passo a passo da avaliação de 2 &! 3. -}

{- (b) Usando foldl e o operador (&!) que você definiu, implemente dec2int :: [Int] -> Int que converte uma lista de dígitos em um inteiro. -}

--	Main> 2 &! 3 = 8
--	Main> dec2int [2,3,4,5] = 2345


-- ====================================================================
-- ============================ RESOLUCAO =============================
-- ====================================================================
-- Questão 1)
-- List comprehension
scalarproduct_1_a :: [Int] -> [Int] -> Int
scalarproduct_1_a x y = sum [x !! i * y !! i | i <- [0..length x -1]]

scalarproduct_1_b :: [Int] -> [Int] -> Int
scalarproduct_1_b x y = sum [x * y | (x,y) <- zip x y]

-- Alta Ordem
scalarproduct_2 :: [Int] -> [Int] -> Int
scalarproduct_2 x y = sum (map (\(x,y) -> x * y) (zip x y))

-- Questão 2)
unfold p h t x
  | p x       = []
  | otherwise = h x : unfold p h t (t x)

dec2bin :: Int -> [Int]
dec2bin n = reverse (unfold (== 0) (`mod` 2) (`div` 2) n)

-- Questão 3)
proliferaChar :: String -> String
proliferaChar cs = [c | c <- cs, _ <- [1..ord c - ord 'A' + 1]]

separa :: [Int] -> ([Int],[Int])
separa xs = ([x | x <- xs, odd x],[x | x <- xs, even x])

-- Questão 4)
{-
procuraElemento :: Int -> [Int] -> Bool
procuraElemento n (x:xs) = n == x || procuraElemento n xs

procuraElemento2 :: Int -> [Int] -> Bool
procuraElemento2 n (x:xs) = procuraElemento n xs || n == x
-}

{- 
a) Sim, ambas produzem o resultado correto. A diferença está na ordem dos operandos do ||. Em Haskell, || é avaliado da esquerda para a direita com short-circuit: se o lado esquerdo for True, o lado direito não é avaliado. Em procuraElemento, a comparação n == x vem primeiro — se o elemento atual for o buscado, a recursão para imediatamente. Em procuraElemento2, a recursão vem primeiro, então Haskell percorre toda a lista antes de testar o elemento atual.
-}

{-
b) procuraElemento é mais eficiente. Graças ao short-circuit de ||, ela para assim que encontra o elemento. procuraElemento2 não pode parar cedo porque a recursão está no lado esquerdo — ela sempre desce até o fim da lista antes de testar qualquer elemento do lado direito.
-}

-- c)
procuraElemento :: (Eq a) => a -> [a] -> Bool
procuraElemento a []		= False
procuraElemento a (b : bs) 	= a == b || procuraElemento a bs 
{-
A restrição Eq a é necessária porque o operador == pertence à classe Eq. Sem ela, o compilador não saberia como comparar os elementos, logo o código não compilaria. Note que adicionamos o caso base [] = False que estava implícito na versão original com Int (onde a lista finita já garantia a terminação).
-}

-- Questão 5)
--a)
(&!) :: Int -> Int -> Int
m &! 0 = 1
m &! n = m * (m &! (n - 1))

{-
2 &! 3
= 2 * (2 &! 2)
= 2 * (2 * (2 &! 1))
= 2 * (2 * (2 * (2 &! 0)))
= 2 * (2 * (2 * 1))
= 2 * (2 * 2)
= 2 * 4
= 8 
-}

--b) 
dec2int :: [Int] -> Int
dec2int xs = sum [d * (10 &! e) | (d, e) <- zip xs exps]
	where exps = reverse [0..length xs - 1]
	
dec2int :: [Int] -> Int
dec2int = foldl (\acc x -> acc * 10 + x) 0
