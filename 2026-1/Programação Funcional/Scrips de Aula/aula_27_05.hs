import Data.Char
-- LIST COMPREHENSION

{- multiplica todos elementos pares da lista l pela escalar n -}
{-
-- resolvida com conhecimento obtido "até hoje"
f :: Int -> [Int] -> [Int]
f _ [] = []
f n (x : xs)
	| (mod) x 2 == 0	= (x * n) : f n xs
	| otherwise			= f n xs
-}
	
-- resolvida com list comprehension
f1 :: Int -> [Int] -> [Int]
f1 n l = [ n*x | x<-l, x `mod` 2 == 0]

{- ---------------------------------------------------------------- -}
{- dado um booleano, decide por multiplicar todos elementos por 2 ou somar todos elementos com 5 -}
f2 :: Bool -> [Int] -> [Int]
f2 b l
	| b 		= [ n*2 | n<-l ]
	| otherwise	= [ n+5 | n<-l ]

{- ---------------------------------------------------------------- -}
{- função aleatória dada em aula, com o intuito de somar ou diminuir 2 -}
f :: Bool -> [Int] -> [Int]
f b l
	| b 		= [ n-2 | n<-l ]
	| otherwise	= [ n+2 | n<-l ]

{- ---------------------------------------------------------------- -}
f3a :: [Int] -> [Int] -> [(Int, Int)]
f3a _ [] = []
f3a [] _ = []
f3a (x : xs) (y : ys) = (x,y) : f3a xs ys

{- ---------------------------------------------------------------- -}
f3 :: [Int] -> [Int] -> [(Int, Int)]
f3 l1 l2 = [ (x,y) | x<-l1, y<-l2, f3_aux l1 l2 x y ]

f3_aux :: [Int] -> [Int] -> Int -> Int -> Bool
f3_aux (a : as) (b : bs) x y 
    | a == x && b == y = True
    | otherwise        = f3_aux as bs x y
f3_aux _ _ _ _ = False

{-
f3 :: [Int] -> [Int] -> [(Int, Int)]
f3 l1 l2 = [ (x,y) | x<-l1, y<-l2, f3_aux l1 l2 x y ]
f3 [1,3,5] [2,4,6] = [ (x,y) | x<-[1,3,5], y<-[2,4,6], f3_aux [1,3,5] [2,4,6] x y ]

f3 = [
    (1,2), (1,4), (1,6),
    (3,2), (3,4), (3,6),
    (5,2), (5,4), (5,6)
  ]


f3_aux :: [Int] -> [Int] -> Int -> Int -> Bool
-}


{- ############# Para o par (x,y) = (1,2) = TRUE
------------ ENTRA ------------
	= [1,3,5]	[2,4,6]	x	y	= True
	= [1,3,5]	[2,4,6] 1	2	= (a == x && b == y) True (1 == 1 && 2 == 2)
								- a = 1, as = [3,5]
								- b = 2, bs = [4,6]
								- x = 1, y = 2
-}

-- Logo f3 = [(1,2)]

{- ############# Para o par (x,y) = (1,4) = FALSE 
------------ NÃO ENTRA ------------
	= [1,3,5]	[2,4,6]	1	4 	= False
								- (a == x && b != y) False (1 == 1 && 2 == 4)
								- a = 1, as = [3,5]
								- b = 2, bs = [4,6]
								- x = 1, y = 4
								
	= [3,5]	[4,6]	1	4	= False
								- (a != x && b == y) False (3 == 1 && 4 == 4)
								- a = 3, as = [5]
								- b = 4, bs = [6]
								- x = 1, y = 4
								
	= [5]	[6]	1	4	= False
								- (a != x && b != y) False (5 == 1 && 6 == 4)
								- a = 5, as = []
								- b = 6, bs = []
								- x = 1, y = 4
-}

{- ############# Para o par (x,y) = (1,6) = FALSE
------------ NÃO ENTRA ------------
	= [1,3,5]	[2,4,6]	1	6 	= False
								- (a == x && b != y) False (1 == 1 && 2 == 6)
								- a = 1, as = [3,5]
								- b = 2, bs = [4,6]
								- x = 1, y = 6
								
	= [3,5]	[4,6]	1	6	= False
								- (a != x && b != y) False (3 == 1 && 4 == 6)
								- a = 3, as = [5]
								- b = 4, bs = [6]
								- x = 1, y = 6
								
	= [5]	[6]	1	4	= False
								- (a != x && b != y) False (5 == 1 && 6 == 4)
								- a = 5, as = []
								- b = 6, bs = []
								- x = 1, y = 4
-}

{- ############# Para o par (x,y) = (3,2) = FALSE
------------ NÃO ENTRA ------------
	= [1,3,5]	[2,4,6]	3	2 	= False
								- (a != x && b == y)  ( 1 == 3 && 2 == 2)
								- a = 1, as = [3,5]
								- b = 2, bs = [4,6]
								- x = 3, y = 2

	= [3,5]	[4,6]	3	2	= False
								- (a == x && b != y) False (3 == 3 && 4 == 2)
								- a = 3, as = [5]
								- b = 4, bs = [6]
								- x = 3, y = 2

	= [5]	[6]	3	2	= False
								- (a != x && b != y) False (5 == 3 && 6 == 2)
								- a = 5, as = []
								- b = 6, bs = []
								- x = 3, y = 2
-}

{- ############# Para o par (x,y) = (3,4) = True
------------ ENTRA ------------
	= [1,3,5]	[2,4,6]	3	4 	= False
								- (a != x && b != y)  ( 1 == 3 && 2 == 4)
								- a = 1, as = [3,5]
								- b = 2, bs = [4,6]
								- x = 3, y = 4

	= [3,5]	[4,6]	3	4	= True
								- (a == x && b == y) True (3 == 3 && 4 == 4)
								- a = 3, as = [5]
								- b = 4, bs = [6]
								- x = 3, y = 4
-}
