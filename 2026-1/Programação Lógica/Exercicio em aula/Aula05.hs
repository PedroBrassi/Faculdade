{ - 06 função transforma string de alfa em Int -}
alfaToInt :: String -> [Int]
alfaToInt [] = []
alfaToInt (a,b) = (ord a - ord '0') : alfaToInt b
