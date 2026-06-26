{-Objetivos: introduzir os conceitos de
   a) List Comprehension
   b) Função de alta ordem.
   
   Para tanto, iniciamos com um problema simples e mostramos,
   a cada passo, as possibilidades de melhorias.
   -}
import Data.Char

{-motivação-}
dobra_a :: Int -> Int
dobra_a x = 2 * x

mul_b :: Int -> Int -> Int
mul_b a b = a * b

op_c :: (Int->Int->Int) -> Int -> Int -> Int
op_c f x y = (f) x y

{-pode passar a função (* 9), por exemplo -}
op_d :: (Int->Int) -> Int-> Int
op_d f y = f y

{-escreva função de alta ordem para filtrar digito ou alpha -
  neste caso, o f pode ser isDigit ou isAlpha-}

filtraAO :: (Char->Bool) -> String -> String
filtraAO f [] = []
filtraAO f (a:b)
	|f a       = a:filtraAO f b
	|otherwise =   filtraAO f b
	
{- reescrevendo com uso de list comprehension -}
filtraAO_LC :: (Char->Bool) -> String -> String
filtraAO_LC f s = [a | a<-s, f a]

{-As funções f1_p1 e f2_p2, a seguir, são específicas. A partir
de uma [String], elas geram [(Bool, String)] separando as strings
que só possuem dígitos das demais --}

-- questão 01
f1_p1 :: String -> Bool
f1_p1 []    = True
f1_p1 (a:b) = isDigit (a) && f1_p1 b

-- questão 02
f2_p1 :: [String] -> [(Bool, String)]
f2_p1 []    = []
f2_p1 (a:b) = (f1_p1 a, a) : f2_p1 b   

{-generalizando a função f2_p1 -}
f2_p1_generica :: (String->Bool) -> [String] -> [(Bool,String)]
f2_p1_generica  f l = [(f a,a) | a <- l]


{- Mas, essas funções podem ser generalizadas.
 Pensemos em uma solução recebe uma [String] e retorna uma [(Bool, String)]
 em que o Bool é True se o String atende a uma característica determinada.
 Para tanto, devemos implementar as características desejaveis -}

cr01_Todos_Char :: (Char->Bool) -> String -> Bool
cr01_Todos_Char f [a]     = f a
cr01_Todos_Char f (a:b)   = f (a) && cr01_Todos_Char f b

cr01_Algum_Char :: (Char->Bool) -> String -> Bool
cr01_Algum_Char f [a]     = f a
cr01_Algum_Char f (a:b)   = f (a) || cr01_Algum_Char f b


{-observe que todos ou alguns podem ser generalizados, também. 
  Para isso, basta passar o operador lógico && ou || -}

cr01_A_T_Char :: (Bool->Bool->Bool) -> (Char->Bool) -> String -> Bool  
cr01_A_T_Char _ f [a]     = f a
cr01_A_T_Char o f (a:b)   = (o) (f (a)) (cr01_A_T_Char o f b)


{- as funções abaixo filtram de [(Bool, String)] a [String]
   de acordo com o Booleano -}
    
{-como transformá-las em uma função genérica? -}

filtraT :: [(Bool, String)] -> [String]
filtraT [] = []
filtraT (a:b)
	|fst a     = snd a:filtraT b
	|otherwise =       filtraT b
  
filtraF :: [(Bool, String)] -> [String]  
filtraF [] = []  
filtraF (a:b)
	|not(fst a) = snd a:filtraF b
	|otherwise  =       filtraF b

{--------------------solução---------------------}
filtraTF :: (Bool->Bool) -> [(Bool, String)] -> [String]
filtraTF _ [] = []
filtraTF f (a:b)
	|f (fst a)	= snd a:filtraTF f b
	|otherwise	=       filtraTF f b

seletor  x = x
inversor x = not(x)
{------------------------------------------------}

------------------  revisão e uso de alta ordem ----------------------------
{- Considere f1 capaz de somar uma lista de inteiros se um Char for alfanumérico, 
    ou multiplicar os elementos, caso contrário -}
f1 :: Char -> [Int] -> Int
f1 c x
	|isDigit (c)     && x==[]    = 0
	|not (isDigit c) && x==[]    = 1
	|isDigit c                   = a + f1 c b
	|otherwise                   = a * f1 c b
	where (a:b) = x

    
{- reescreva f1 usando casamento de padrão -}
f2 :: Char -> [Int] -> Int
f2 c []
	|isDigit c	= 0
	|otherwise	= 1
f2 c (a : b)
	|isDigit c	= a + f2 c b
	|otherwise	= a * f2 c b

{- reescreva f2 fazendo chamadas de funções para somar ou multiplicar -}
f3 :: Char -> [Int] -> Int
f3 c x
	|isDigit c	= somaLista x
	|otherwise	= multLista x
	
somaLista :: [Int] -> Int
somaLista [] 		= 0
somaLista (a : b) 	= a + somaLista b

multLista :: [Int] -> Int
multLista [] 		= 1
multLista (a : b) 	= a * multLista b

{- reescreva f3 usando função de alta ordem para definir o operador
   Esta função é didática, pois mostra o uso de função de alta ordem
   Neste caso, considere que a lista tem, pelo menos, um elemento -}
   
{- para lista de pelo menos um elemento -}
f4 :: (Int->Int->Int) -> [Int] -> Int
f4 _ [l] 		= l
f4 f (a : b)	= f a (f4 f b)

{- faça a função myMap aplica uma função a cada elemento de uma lista -}
myMap :: (a->b) -> [a] -> [b]
myMap _ []			= []
myMap f (x : xs) 	= f x : myMap f xs

myMapComprehension :: (u->t) -> [u] -> [t]
myMapComprehension f l = [f a | a <- l]

{- faça a função myFilter aplica uma função a cada elemento de uma lista -}
myFilter :: (a->Bool) -> [a] -> [b]
myFilter f l = [u | u <- l, f u]

{- faça a função myFold aplica uma função a cada elemento de uma lista 
fold :: (u->t->u) -> u -> [t] -> u
fold f s [] = s
fold f s -}

{- função que converte caixa baixa para caixa  alta
   usar a função myMap para aplicar a uma String -}
caixaAlta :: Char -> Char
caixaAlta c
	|c >= 'a' && c <= 'z'	= chr (ord c - 32)
	|otherwise				= c
	
maiuscula :: String -> String
maiuscula = myMap caixaAlta

{- ========================================================================= -}
avalia :: Int -> Bool
avalia x = x `mod` 2 == 0

gerador :: Int -> (Int, Bool)
gerador x = (x, avalia x)

geradorG :: (Int->Bool) -> Int -> (Int->Bool)
geradorG k x = (x, k x)
