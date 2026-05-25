import Data.Char

{- faça a função f1 que receba uma String S
e Retorne uma String R.
R deve ser igual a S, com exceção de que
todo caracter alfanumérico x encontrado em S
que esteja seguido de um caracter y não alfanumérico
determinará que y será repetido x vezes em R

exemplo  f1 "ab42c570sd3f" retorna "ab42cc570d3fff" 
-}

{- Observação. No Data.Char existe 
            isDigit::Char->Bool -}

f1::[Char]->[Char]
f1      [] = []

{- faça f11, outra versão de f1, que retorne R do tipo [(Char, Bool, Int)] 
de modo que, para cada caractere de S, informe se ele será repetido ou não
e a quantidade de vezes. 

Por exemplo, f11 "ab42c570sd3f" retorna [('a',False,1),('b',False,1),
('4',False,1),('2',False,1),('c',True,2),('5',False,1),('7',False,1),
('0',False,1),('s',True,0),('d',False,1),('3',False,1),('f',True,3)] -}

f11::String->[(Char, Bool, Int)]
f11   []     = []

{-Agora, implemente a função f111 que receba [(Char, Bool, Int)] e gere uma String
com os caracteres repetidos ou não (como R em f1). Use o Bool da dupla-}

f111:: [(Char, Bool, Int)] -> String
f111 [] = []

{-faça a função f2 que receba uma lista de
Strings e aplique a todas as strings a 
computação da função f1-}

f2::[String]->[String]
f2 [] = []

{-faça a função f3 que receba uma String S 
e retorne uma dupla de Bool e String.
A string de saída deve ter o caracter
da ordem alfabética das letras minúsculas
por substituição a cada caracter numérico
(1..9) que aparece em S. O bool deve informar se 
a entrada foi ou não alterada.
Ex:  f3 "a2c4x" retorna ("abcdx", True)-}

f3::String -> (String, Bool)
f3 [] = ([],False)

{-faça uma função que transforme uma string de dígitos
para o número correspondente-}

--entrada- reverso da string 
strToInt::String->Int
strToInt [] = 0

