{- 1) Faça, em Haskell, uma solução que receba um Int a e uma [Int] b. Como saída, teremos um Bool que informa se a ∈ b. -}
f_in :: Int -> [Int] -> Bool
f_in _ [] = False
f_in a (b : bs)
	| a == b  	= True
	| otherwise	= f_in a bs
	
{- 2) Considerando f_in, faça, em Haskell, uma solução que receba um Int x e uma [[Int]] l. Como saída, teremos uma [(Bool, [Int])] s que informa, para cada sublista li de l, se o elemento x pertence ou não à li. -}
f2 :: Int -> [[Int]] -> [(Bool,[Int])]
f2 _ [] = []
f2 x (l : ls) = (f_in x l, l) : f2 x ls

{- 3) Considerando f2, faça, em Haskell, uma solução que receba um Int x e uma [[Int]] l. Como saída, teremos uma (Int, [(Bool, [Int])]) s que informa o Int x e, para cada sublista li de l, se o elemento x pertence ou não à li. -}
f3 :: Int -> [[Int]] -> (Int,[(Bool,[Int])])
f3 x l = (x, f2 x l)

{- 4) Faça, em Haskell, uma solução que receba uma (Int,[(Bool,[Int])]) e gere uma [[Int]] com apenas as listas [Int] cujo Bool da dupla é False. -}
f4 :: (Int,[(Bool,[Int])]) -> [[Int]]
f4 (_, []) = []
f4 (n, (teste, l) : resto)
	| teste		= f4 (n, resto)
	| otherwise = l : f4 (n, resto)

{- 5) Faça, em Haskell, uma solução que receba duas strings S e R, cuja menor tem tamanho n. Sua solução deverá retornar uma lista de Bool B de tamanho n, de modo que cada Bi será a informação se Si é igual ou diferente de Ri , para 0 ≤ i < n. -}
f5 :: String -> String -> [Bool]
f5 [] ys = []
f5 xs [] = []
f5 (s : ss) (r : rs) = (s == r) : f5 ss rs
