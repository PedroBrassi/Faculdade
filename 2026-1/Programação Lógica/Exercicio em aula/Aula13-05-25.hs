{- objetivo desta aula: Trabalhar os tipos lista e tuplas -}

--01- Operador que defina o menor entre dois inteiros
--infix 7 &<& 						"&<&" vai agir como um operador. Ou seja, caso eu quiser o menor entre A e B (onde B é menor), teremos: A &<& B = B (menor).

--02- Função que retorna o menor de uma [Int]
--menorL :: [Int] -> Int
--CHAMAMENTO: menorL [2,3,1,6,0,8] == 0

--03- Função que gera uma dupla com o menor e a lista de entrada
--menorD :: [Int] -> (Int, [Int])
--CHAMAMENTO: menorD [2,3,1,6,0,8] == (0,[2,3,1,6,0,8])

--04- Função que recebe uma dupla com o menor de uma lista e a lista e retorna a lista sem a primeira ocorrencia do menor 
--filtraL :: (Int, [Int]) -> [Int] 			sem o menor da dupla (lista sem o menor adicionado) --
--CHAMAMENTO: filtraL (menorD [2,3,1,6,0,8]) == [2,3,1,6,8]
--CHAMAMENTO: filtraL (0, [2,3,6,0,8]) == [2,3,6,8]

--05 Função que recebe uma dupla com o menor e a lista original e retorna uma lista ordenada  
--ordenaL :: (Int, [Int]) -> [Int]
--CHAMAMENTO: ordenaL(1,[6,7,3,1,5,9]) == [1,3,5,6,7,9]
--CHAMAMENTO: ordenaL (menorD [6,7,3,1,5,9]) == [1,3,5,6,7,9]

--06 Função interface para ordenar a lista de entrada
--ordenaL :: [Int] -> [Int]

--Quais funções podem ser simplificadas para essa ordenação?

-- Resolução:
-- Exercicio 01:
infix 7 &<&
(&<&) :: Int -> Int -> Int
x &<& y
	| x < y		= x
	| otherwise 	= y

--Exercicio 02:
menorL :: [Int] -> Int
menorL [a] = a -- VAI CAIR NA PROVA (tem que levar em consideração que tenha PELO MENOS um elemento!!!)
menorL (a : b) = a &<& menorL b

--Exercicio 03:
menorD :: [Int] -> (Int, [Int])
menorD x = (menorL x, x)

--Exercicio 04:
filtraL :: (Int, [Int]) -> [Int]
filtraL (_ , []) = []
filtraL (a, (b:x))
	| a == b 	= x
	| otherwise 	= b : filtraL (a,x)

--Exercicio 05:
ordenaL :: (Int, [Int]) -> [Int]
ordenaL (_,[]) = []
-- ordenaL (x, l) = x : ordenaL ( menorD (filtraL (x,l))) {-oficial-}
ordenaL (x,l) = x : ordenaL (menorL y, y)
	where y = filtraL (x,l)

--Ecercicio 06:
ordena :: [Int] -> [Int]
ordena a = ordenaL (menorD a)

--Quais funções podem ser simplificadas para essa ordenação?
-- Unir as funções medorD e o filtraL. Fazendo uma tripla (elemento, [lista], posicao)
