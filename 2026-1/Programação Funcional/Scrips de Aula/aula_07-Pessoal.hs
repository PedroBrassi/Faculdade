{- Função de alta ordem -}
filtraAO :: ( u -> Bool) -> [u] -> [u]
filtraAo f l = [ a | a <- l, f a]

f1_p1 :: String -> Bool

