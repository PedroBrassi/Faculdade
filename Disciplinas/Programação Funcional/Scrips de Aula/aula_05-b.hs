{- objetivo desta aula: 
    - Trabalhar os tipos lista e tuplas 
    - introduzir o uso de where
    - atentar para a base da função 02
-}

--01- Operador que defina o menor entre dois inteiros
infix 7 &<&
(&<&) :: Int->Int->Int
x &<& y
  |x<y = x
  |otherwise = y
  
--02- função que retorna o menor de uma [Int]
menorL::[Int]->Int
menorL [a] = a
menorL (a:b) = a &<& menorL b

--03- função que gera uma dupla com o menor e a lista de entrada
menorD::[Int]->(Int, [Int])
menorD x = (menorL x, x)

--04- função que recebe uma dupla com o menor de uma lista e a lista e retorna a lista sem o menor

filtraL :: (Int, [Int])->[Int]
filtraL (_,[]) = []
filtraL (a, (b:x))
  |a==b      = x
  |otherwise = b:filtraL (a,x)

--05 função que recebe uma dupla com o menor e a lista original e retorna uma lista ordenada  
ordenaL :: (Int, [Int])->[Int]
ordenaL (_,[]) = []
ordenaL (x, l) = x:ordenaL (menorD (filtraL (x,l))) {-oficial-}
{-ordenaL (x, l) = x:ordenaL (menorL y, y )
  where y = filtraL (x,l) -} {- opção alternativa e didática -}


--06 função interface para ordenar a lista de entrada
ordena::[Int]->[Int]
ordena a = ordenaL (menorD a)

--Quais funções podem ser simplificadas para essa ordenação?

