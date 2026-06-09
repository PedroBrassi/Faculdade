import Data.Char

type Dia = Int
type Venda = Int
type Periodo = Int

periodo::Int
periodo = 7

answer :: Int
answer = 42

square :: Int -> Int
square x = x * x

soma :: Int -> Int -> Int
soma z k = z+k

allEqual :: Int -> Int -> Int -> Bool
allEqual m n p = (m==n) && (n==p)

maxi :: Int -> Int -> Int
maxi m n
   |m >= n = m
   |otherwise	= n


f :: Dia -> Venda
f 1 = 9
f 2 = 80
f 3 = 15
f 4 = 14
f 5 = 80
f 6 = 0
f 7 = 30
f x = 0

{- função que retorne a maior venda da semana -}
maiorVenda01 :: Periodo -> Venda -> Venda
maiorVenda01  0 v = v
maiorVenda01  x v 
  | f x > v    = maiorVenda01 (x-1) (f x)
  | otherwise  = maiorVenda01 (x-1) v
  
maiorVenda02 :: Periodo -> Venda
maiorVenda02 0 = f 0
maiorVenda02 x = maxi (f x) (maiorVenda02 (x-1))

maiorVenda :: Venda
maiorVenda = maiorVenda02 periodo

{- Exercícios:
   implemente uma função que retorne o dia em que houve a maior venda (função f)-}
   
diaVenda::Venda -> Dia
diaVenda v = diaVenda01 periodo v

diaVenda01 :: Periodo -> Venda -> Dia
diaVenda01 0 _ = 0
diaVenda01 dia v
  |f dia == v = dia
  |otherwise  = diaVenda01 (dia-1) v  
  
{- implemente uma função que retorne a quantidade de vendas do período -}

{- implemente uma função que retorne a média de vendas-}   

{-Exercício - inclua em suas funções um contador para informar quantos testes
  a função executa. Isso é o mesmo que comprovar a complexidade
  Considere, neste caso, contar os testes nas funções que são chamadas internamente
-}

{- Questão 3 da prova -}
f4 :: [(Bool, String)] -> ([String], [String])
f4 [] = ([] , [])
f4 ((teste, s) : resto)
	| teste		= (s:v, f)
	| otherwise = (v, s:f)
	where (v, f) = f4 resto
