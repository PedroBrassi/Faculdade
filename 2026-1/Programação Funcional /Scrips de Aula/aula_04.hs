{- objetivo: trabalhar tipos distintos entre listas e tuplas -}

import Data.Char

------------------------------------------------------
{- 01 função que separa [(Int,Char)] em ([Int],[Char]) -}
--myUnzip :: [(Int,Char)]->([Int],[Char])

{- 02 versão em uma única função -}
--myUnzipU :: [(Int,Char)]->([Int],[Char])

------------------------------------------------------------
{- 03 função que junta duas listas em lista de duplas -}
--myZip::[Bool]->[Char] ->[(Bool,Char)]  

{- 04 função que recebe [Char] e retorna [(Bool,Char)] 
   True se Char for alfanumérico e False, caso contrário -}
--setAlfa::String -> [(Bool,Char)] 

   
{- 05 função que recebe [(Bool, Char)] e filtra alfanuméricos -}
--filtraAlfa:: [(Bool,Char)] -> String

{- 06 função transforma String de alfa em Int -}
--alfaToInt::String -> [Int]


{-- 07 função que gera tabela ascii -}
--geraASCII::Int->[(Int,Char)]


