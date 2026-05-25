-- ============================================================
-- LISTA DE EXERCÍCIOS 01 - Programação Funcional
-- Disciplina: DCE673 - UNIFAL - Prof. Eliseu
-- Aluno: Pedro Brassi Luccas
-- Estilo: sem funções do Prelude, apenas recursão e guardas
-- ============================================================

import Data.Char

-- ============================================================
-- EXERCÍCIO 1 - Funções matemáticas com guardas
-- ============================================================

{- (a) f1 : R -> R
   x >= 0 => (x+4)/(x+2)
   x <  0 => 2/x
   Usamos Float para representar R. -}
f1 :: Float -> Float
f1 x
  | x >= 0    = (x + 4) / (x + 2)
  | otherwise = 2 / x
-- f1 0.0  = 4.0/2.0 = 2.0
-- f1 (-2) = 2/(-2)  = -1.0

{- (b) f2 : R² -> R
   x >= y => x + y
   x <  y => x - y -}
f2 :: Float -> Float -> Float
f2 x y
  | x >= y    = x + y
  | otherwise = x - y
-- f2 3.0 2.0 = 5.0
-- f2 1.0 4.0 = -3.0

{- (c) f3 : R³ -> R
   (x+y) > z => x + y + z
   (x+y) < z => x - y - z
   (x+y) = z => 0 -}
f3 :: Float -> Float -> Float -> Float
f3 x y z
  | (x + y) > z  = x + y + z
  | (x + y) < z  = x - y - z
  | otherwise     = 0
-- f3 3 2 4 => (3+2)=5 > 4 => 3+2+4 = 9
-- f3 1 2 5 => (1+2)=3 < 5 => 1-2-5 = -6
-- f3 2 3 5 => (2+3)=5 = 5 => 0


-- ============================================================
-- EXERCÍCIO 2 - Corrigir fatorial
-- ============================================================

{- ERRO: a função não tem caso base.
   fat x = x * fat(x-1) entra em loop infinito porque
   nunca para — fat 0 chamaria fat(-1), fat(-2), ad infinitum.

   CORREÇÃO: adicionar o caso base fat 0 = 1
   (0! = 1 por definição matemática) -}
fat :: Int -> Int
fat 0 = 1                    -- caso base: 0! = 1
fat x = x * fat (x - 1)     -- caso recursivo: n! = n * (n-1)!
-- fat 5 = 5 * 4 * 3 * 2 * 1 * 1 = 120


-- ============================================================
-- EXERCÍCIO 3 - Multiplicação usando soma
-- ============================================================

{- soma é dada: -}
soma :: Int -> Int -> Int
soma a b = a + b

{- Multiplicar a * b = somar 'a' exatamente 'b' vezes.
   Caso base: qualquer coisa * 0 = 0
   Caso recursivo: a * b = a + (a * (b-1))
   ATENÇÃO: tratamos apenas b >= 0. -}
multiplica :: Int -> Int -> Int
multiplica _ 0 = 0
multiplica a b = soma a (multiplica a (b - 1))
-- multiplica 4 3 = 4 + 4 + 4 + 0 = 12


-- ============================================================
-- EXERCÍCIO 4 - Inverter dígitos de um inteiro
-- ============================================================

{- Estratégia:
   1. Separar os dígitos do número usando div/mod recursivo.
   2. Reconstruir o número invertido.

   Funções auxiliares sem usar funções prontas:
   - resto a b   : resto da divisão (= mod)
   - quociente   : divisão inteira (= div)
   - potencia    : calcula 10^n para reconstruir o número
   - numDigitos  : conta quantos dígitos tem o número
   - invertAux   : reconstrói o número invertido -}

resto :: Int -> Int -> Int
resto a b
  | a < b     = a
  | otherwise = resto (a - b) b

quociente :: Int -> Int -> Int
quociente a b
  | a < b     = 0
  | otherwise = 1 + quociente (a - b) b

potencia :: Int -> Int -> Int
potencia _ 0 = 1
potencia b e = b * potencia b (e - 1)

numDigitos :: Int -> Int
numDigitos 0 = 0
numDigitos n = 1 + numDigitos (quociente n 10)

{- Reconstrói o número com os dígitos na ordem inversa.
   pos: posição atual (começa em numDigitos n - 1, vai até 0)
   Exemplo: invertAux 123 2 => extrai 3 * 10^2 + invertAux 12 1
                                     => 300 + 2*10 + invertAux 1 0
                                     => 300 + 20 + 1*1 = 321 -}
invertAux :: Int -> Int -> Int
invertAux 0 _ = 0
invertAux n pos = (resto n 10) * potencia 10 pos
                + invertAux (quociente n 10) (pos - 1)

invertInt :: Int -> Int
invertInt n = invertAux n (numDigitos n - 1)
-- invertInt 123 = 321
-- invertInt 1000 = 1  (zeros à frente são perdidos, comportamento esperado)


-- ============================================================
-- EXERCÍCIO 5 - fourPower usando square
-- ============================================================

{- square dada em sala: -}
square :: Int -> Int
square x = x * x

{- fourPower x = x^4 = (x^2)^2 = square(square x) -}
fourPower :: Int -> Int
fourPower x = square (square x)
-- fourPower 3 = square(square 3) = square 9 = 81


-- ============================================================
-- EXERCÍCIO 6 - Sequência aninhada de raízes de 6
-- ============================================================

{- Sequência: i0 = sqrt(6)
              i1 = sqrt(6 + sqrt(6))
              i2 = sqrt(6 + sqrt(6 + sqrt(6)))
              ...
              iN = sqrt(6 + i(N-1))

   Usamos Float (sqrt retorna Float). -}
seqRaiz :: Int -> Float
seqRaiz 0 = sqrt 6
seqRaiz i = sqrt (6 + seqRaiz (i - 1))
-- seqRaiz 0 = 2.449
-- seqRaiz 1 = 2.904
-- seqRaiz 5 = 2.9999...  (converge para 3)


-- ============================================================
-- EXERCÍCIO 7 - Combinações C(m,n)
-- ============================================================

{- C(m,n) = m! / (n! * (m-n)!)
   "De quantas formas escolher n objetos de m"
   Reutiliza fat do exercício 2. -}
combinacao :: Int -> Int -> Int
combinacao m n = quociente (fat m) (fat n * fat (m - n))
-- combinacao 5 2 = 120 / (2 * 6) = 10
-- combinacao 4 4 = 1


-- ============================================================
-- EXERCÍCIO 8 - MDC recursivo (algoritmo de Euclides)
-- ============================================================

{- O código C usa while com resto. Tradução recursiva:
   mdc m n: enquanto (m mod n) != 0, faz m=n, n=m mod n
   Caso base: quando resto m n == 0, retorna n
   Caso recursivo: mdc n (resto m n) -}
mdc :: Int -> Int -> Int
mdc m n
  | resto m n == 0 = n
  | otherwise      = mdc n (resto m n)
-- mdc 48 18 = mdc 18 12 = mdc 12 6 = mdc 6 0... base: 6
-- mdc 12 8  = 4


-- ============================================================
-- EXERCÍCIO 9 - Quantos múltiplos de k no intervalo [a,b]
-- ============================================================

{- Percorre o intervalo de a até b contando os múltiplos de k.
   Um número x é múltiplo de k se resto x k == 0. -}
howManyMultiples :: Int -> Int -> Int -> Int
howManyMultiples _ a b
  | a > b      = 0
howManyMultiples k a b
  | resto a k == 0 = 1 + howManyMultiples k (a + 1) b
  | otherwise      = howManyMultiples k (a + 1) b
-- howManyMultiples 4 1 10 = 2  (4 e 8)
-- howManyMultiples 3 1 10 = 3  (3, 6, 9)


-- ============================================================
-- EXERCÍCIO 10 - Último dígito
-- ============================================================

{- O último dígito é simplesmente o resto da divisão por 10. -}
lastDigit :: Int -> Int
lastDigit n = resto n 10
-- lastDigit 1234 = 4
-- lastDigit 7    = 7


-- ============================================================
-- EXERCÍCIO 11 - Dígito na posição p (0 = mais significativo)
-- ============================================================

{- anyDigit p n retorna o dígito na posição p contando da ESQUERDA.
   Estratégia:
   1. Conta o total de dígitos (numDigitos).
   2. Posição da esquerda p => posição da direita = (total - 1 - p).
   3. Remove os dígitos menos significativos com divisões e pega o último.
   Se p >= total de dígitos, retorna -1. -}
anyDigit :: Int -> Int -> Int
anyDigit p n
  | p < 0 || p >= numDigitos n = -1
  | otherwise = resto (quociente n (potencia 10 (numDigitos n - 1 - p))) 10
-- anyDigit 0 7689 = 7
-- anyDigit 2 7689 = 8
-- anyDigit 9 7689 = -1


-- ============================================================
-- EXERCÍCIO 12 - allDifferent corrigido
-- ============================================================

{- (a) ERRO: a condição (m/=n) && (n/=p) não verifica m/=p.
   Exemplo: allDifferent 1 2 1 retornaria True (errado!),
   pois (1/=2)=True e (2/=1)=True, mas m==p.

   (b) CORREÇÃO: verificar todos os três pares. -}
allDifferent :: Int -> Int -> Int -> Bool
allDifferent m n p = (m /= n) && (n /= p) && (m /= p)
-- allDifferent 1 2 3 = True
-- allDifferent 1 2 1 = False  (corrigido!)
-- allDifferent 1 1 2 = False


-- ============================================================
-- EXERCÍCIO 13 - howManyEqual
-- ============================================================

{- Conta quantos dos três valores são iguais entre si.
   Casos: todos iguais (3), dois iguais (2), todos diferentes (0).
   Nota: o enunciado diz que a resposta é 3, 2 ou 0 — nunca 1,
   porque se um par é igual, já são "dois iguais". -}
howManyEqual :: Int -> Int -> Int -> Int
howManyEqual m n p
  | m == n && n == p = 3          -- todos iguais
  | m == n || n == p || m == p = 2 -- algum par igual
  | otherwise                    = 0 -- todos diferentes
-- howManyEqual 3 3 3 = 3
-- howManyEqual 3 3 5 = 2
-- howManyEqual 1 2 3 = 0


-- ============================================================
-- EXERCÍCIO 14 - Funções sobre sales
-- ============================================================

sales :: Int -> Int
sales 0 = 0
sales 1 = 41
sales 2 = 72
sales 3 = 48
sales 4 = 0
sales 5 = 91
sales 6 = 55
sales 7 = 30
sales _ = -1

{- (a) howManyLess: conta dias cujas vendas foram < valor,
   no intervalo [inicio, fim]. -}
howManyLess :: Int -> Int -> Int -> Int
howManyLess _ ini fim
  | ini > fim  = 0
howManyLess valor ini fim
  | sales ini < valor = 1 + howManyLess valor (ini + 1) fim
  | otherwise         = howManyLess valor (ini + 1) fim
-- howManyLess 50 1 7 = dias com vendas < 50: 1(41),3(48),4(0),7(30) = 4

{- (b) noZeroInPeriod: True se não há nenhum dia com vendas == 0
   no período de 1 a n. -}
noZeroInPeriod :: Int -> Bool
noZeroInPeriod 0 = True
noZeroInPeriod n
  | sales n == 0 = False
  | otherwise    = noZeroInPeriod (n - 1)
-- noZeroInPeriod 7 = False (dia 4 tem 0 vendas)
-- noZeroInPeriod 3 = True

{- (c) zerosInPeriod: lista de todos os dias com vendas == 0 -}
zerosInPeriod :: [Int]
zerosInPeriod = zerosAux 1
  where
    zerosAux dia
      | dia > 7       = []
      | sales dia == 0 = dia : zerosAux (dia + 1)
      | otherwise     = zerosAux (dia + 1)
-- zerosInPeriod = [4]

{- (d) diasAbaixoDe: dias com vendas abaixo de um valor -}
diasAbaixoDe :: Int -> [Int]
diasAbaixoDe valor = diasAux 1
  where
    diasAux dia
      | dia > 7          = []
      | sales dia < valor = dia : diasAux (dia + 1)
      | otherwise        = diasAux (dia + 1)
-- diasAbaixoDe 50 = [1,3,4,7]


-- ============================================================
-- EXERCÍCIO 15 - antFib: posição de x na sequência de Fibonacci
-- ============================================================

{- Fibonacci: 0,1,1,2,3,5,8,13,21,...
   antFib x: retorna o índice de x na sequência, ou -1.
   Estratégia: percorre a sequência gerando termos até:
   - encontrar x (retorna posição)
   - ultrapassar x (retorna -1, x não está na sequência)

   Função auxiliar fibAux a b pos:
     a = fib(pos-1), b = fib(pos) -}
antFib :: Int -> Int
antFib x = fibAux 0 1 0 x

fibAux :: Int -> Int -> Int -> Int -> Int
fibAux a _ pos x
  | a == x    = pos
fibAux a b pos x
  | a > x     = -1
  | otherwise = fibAux b (a + b) (pos + 1) x
-- antFib 13 = 7   (0,1,1,2,3,5,8,13 -> posição 7)
-- antFib 4  = -1  (4 não é Fibonacci)
-- antFib 0  = 0


-- ============================================================
-- EXERCÍCIO 16 - funny com uma única cláusula
-- ============================================================

{- Original com guardas:
   funny x y z
     | x > z    = True
     | y >= x   = False
     | otherwise = True

   Uma única cláusula: usar expressão booleana.
   (x > z) ou (not (y >= x)) = (x > z) ou (y < x)
   Simplificando: True quando x>z OU y<x; False apenas quando x<=z E y>=x -}
funny :: Int -> Int -> Int -> Bool
funny x y z = (x > z) || (y < x)
-- funny 5 3 2 = (5>2) = True
-- funny 2 5 4 = (2>4)=F || (5<2)=F = False  -> guarda y>=x=True => False ✓
-- funny 3 1 5 = (3>5)=F || (1<3)=T = True


-- ============================================================
-- EXERCÍCIO 17 - minusculaParaMaiuscula
-- ============================================================

{- isLower do Data.Char verifica se é minúscula.
   A conversão: maiúscula = chr (ord c - 32)
   (distância fixa entre 'a'(97) e 'A'(65) = 32) -}
toMaiuscula :: Char -> Char
toMaiuscula c
  | isLower c = chr (ord c - 32)
  | otherwise = c
-- toMaiuscula 'a' = 'A'
-- toMaiuscula 'Z' = 'Z'  (não é minúscula, retorna igual)
-- toMaiuscula '3' = '3'


-- ============================================================
-- EXERCÍCIO 18 - charToNum
-- ============================================================

{- isDigit verifica se é dígito.
   ord '0' = 48; logo ord c - ord '0' dá o valor numérico.
   Ex: ord '3' - ord '0' = 51 - 48 = 3 -}
charToNum :: Char -> Int
charToNum c
  | isDigit c = ord c - ord '0'
  | otherwise = -1
-- charToNum '3' = 3
-- charToNum 'a' = -1


-- ============================================================
-- EXERCÍCIO 19 - duplicate
-- ============================================================

{- Concatena n cópias de s.
   Caso base: n == 0 => string vazia "".
   Caso recursivo: s ++ duplicate s (n-1). -}
duplicate :: String -> Int -> String
duplicate _ 0 = ""
duplicate s n = s ++ duplicate s (n - 1)
-- duplicate "ab" 3 = "ababab"
-- duplicate "x"  0 = ""


-- ============================================================
-- EXERCÍCIO 20 - pushRight
-- ============================================================

{- Insere '>' no início de s até que length(resultado) == n.
   Se n <= tamanho de s, retorna s.
   Funções auxiliares: tamanhoStr e insereChars. -}
tamanhoStr :: String -> Int
tamanhoStr []     = 0
tamanhoStr (_:xs) = 1 + tamanhoStr xs

insereChars :: Char -> Int -> String
insereChars _ 0 = ""
insereChars c k = c : insereChars c (k - 1)

pushRight :: String -> Int -> String
pushRight s n
  | n <= tamanhoStr s = s
  | otherwise         = insereChars '>' (n - tamanhoStr s) ++ s
-- pushRight "abc" 5 = ">>abc"
-- pushRight "abcde" 3 = "abcde"  (n < tamanho)


-- ============================================================
-- EXERCÍCIO 21 - Operador &-
-- ============================================================

{- Definição: x &- y = x - 2*y -}
infixl 6 &-
(&-) :: Int -> Int -> Int
x &- y = x - 2 * y

{- Avaliação de  10 &- 3 &- 2:

   (a) infixl 6 &-  (associatividade à ESQUERDA):
       (10 &- 3) &- 2 = (10 - 6) &- 2 = 4 &- 2 = 4 - 4 = 0

   (b) infixr 6 &-  (associatividade à DIREITA):
       10 &- (3 &- 2) = 10 &- (3 - 4) = 10 &- (-1) = 10 - 2*(-1) = 12

   (c) infix 6 &-   (sem associatividade — NÃO ASSOCIATIVO):
       ERRO em tempo de compilação: expressão ambígua.
       Haskell não consegue resolver a expressão sem parenteses.

   Avaliação de  10 &- 3 * 2:

   (a) infix 6 &-   (* tem precedência 7 > 6):
       10 &- (3 * 2) = 10 &- 6 = 10 - 12 = -2

   (b) infix 8 &-   (&- tem precedência 8 > precedência 7 de *):
       (10 &- 3) * 2 = (10 - 6) * 2 = 4 * 2 = 8
-}


-- ============================================================
-- EXERCÍCIO 22 - Inverter lista de inteiros
-- ============================================================

{- Igual ao reverseList dos exercícios anteriores.
   Adiciona a cabeça no final usando ++ [x]. -}
inverte :: [Int] -> [Int]
inverte []     = []
inverte (x:xs) = inverte xs ++ [x]
-- inverte [1,2,3,4,5,6,150] = [150,6,5,4,3,2,1]


-- ============================================================
-- EXERCÍCIO 23 - Separar ímpares e pares
-- ============================================================

{- Retorna ([ímpares], [pares]) mantendo a ordem original.
   Usa resto x 2 para verificar paridade (sem mod).
   Desestrutura o resultado recursivo com where. -}
separa :: [Int] -> ([Int], [Int])
separa []     = ([], [])
separa (x:xs)
  | resto x 2 /= 0 = (x : imp, par)   -- ímpar
  | otherwise       = (imp, x : par)   -- par
  where (imp, par) = separa xs
-- separa [1,4,3,4,6,7,9,10] = ([1,3,7,9],[4,4,6,10])


-- ============================================================
-- EXERCÍCIO 24 - Converter lista de posições em letras
-- ============================================================

{- Posição 1 -> 'A', 2 -> 'B', ..., 26 -> 'Z'.
   chr (ord 'A' + (n-1)) converte posição para letra maiúscula.
   Posições inválidas (<=0 ou >26) são ignoradas. -}
convertePos :: [Int] -> String
convertePos []     = ""
convertePos (n:ns)
  | n >= 1 && n <= 26 = chr (ord 'A' + (n - 1)) : convertePos ns
  | otherwise          = convertePos ns
-- convertePos [1,2,6,1,9] = "ABFAI"
-- convertePos []           = ""


-- ============================================================
-- EXERCÍCIO 25 - Intervalos (ranges)
-- ============================================================

{- Respostas:
   (a) ['a'..'g']       = "abcdefg"         => ['a','b','c','d','e','f','g']
   (b) [0.1..0.9]       = [0.1,0.2,...,0.9] => passo padrão 0.1 (mas Float tem imprecisão!)
   (c) [0.1,0.3..0.9]   = [0.1,0.3,0.5,0.7,0.9]
   (d) [0.1,0.3..1.8]   = [0.1,0.3,0.5,0.7,0.9,1.1,1.3,1.5,1.7]
   (e) [0.4,0.2..0.8]   = []  (passo negativo com início < fim => lista vazia)
   (f) [1,4..15]        = [1,4,7,10,13]
-}


-- ============================================================
-- EXERCÍCIO 26 - Contar ocorrências de um Char em String
-- ============================================================

{- conta s c: conta quantas vezes o char c aparece em s.
   NOTA: o enunciado passa o segundo parâmetro como String ("B"),
   então usamos head para extrair o Char, ou aceitamos Char diretamente. -}
conta :: String -> Char -> Int
conta []     _ = 0
conta (x:xs) c
  | x == c    = 1 + conta xs c
  | otherwise = conta xs c
-- conta "ABCAABCDDA" 'B' = 2


-- ============================================================
-- EXERCÍCIO 27 - Remover duplicatas de lista ordenada
-- ============================================================

{- Como a lista já está ordenada, elementos duplicados são adjacentes.
   Basta comparar cada elemento com o próximo:
   se iguais, descarta o atual; se diferentes, mantém. -}
purifica :: [Int] -> [Int]
purifica []        = []
purifica [x]       = [x]
purifica (x:y:xs)
  | x == y    = purifica (y:xs)    -- descarta x (duplicata)
  | otherwise = x : purifica (y:xs) -- mantém x
-- purifica [1,1,4,5,5,5,6,7,8,8] = [1,4,5,6,7,8]


-- ============================================================
-- EXERCÍCIO 28 - Proliferar inteiros pelo seu valor
-- ============================================================

{- Para cada x na lista, repete x exatamente x vezes.
   Função auxiliar repete que usa o padrão de insereChars. -}
repeteInt :: Int -> Int -> [Int]
repeteInt _ 0 = []
repeteInt x n = x : repeteInt x (n - 1)

proliferaInt :: [Int] -> [Int]
proliferaInt []     = []
proliferaInt (x:xs) = repeteInt x x ++ proliferaInt xs
-- proliferaInt [3,0,2,4,0,1] = [3,3,3,2,2,4,4,4,4,1]
-- Nota: 0 repetido 0 vezes = nada; 1 repetido 1 vez = [1]


-- ============================================================
-- EXERCÍCIO 29 - Proliferar Chars pela posição no alfabeto
-- ============================================================

{- Para cada letra maiúscula C, repete C exatamente (ord C - ord 'A' + 1) vezes.
   'A' = posição 1, 'B' = posição 2, ..., 'C' = posição 3, etc. -}
posAlfa :: Char -> Int
posAlfa c = ord c - ord 'A' + 1

repeteChar :: Char -> Int -> String
repeteChar _ 0 = ""
repeteChar c n = c : repeteChar c (n - 1)

proliferaChar :: String -> String
proliferaChar []     = ""
proliferaChar (c:cs) = repeteChar c (posAlfa c) ++ proliferaChar cs
-- proliferaChar "CBD" = 'C'*3 ++ 'B'*2 ++ 'D'*4 = "CCCBBDDDD"


-- ============================================================
-- EXERCÍCIO 30 - Tripla com char, alternância de case e ASCII
-- ============================================================

{- Retorna (char original, char alternado, código ASCII).
   Se minúsculo => alternado é maiúsculo; se maiúsculo => minúsculo;
   caso contrário => próprio char. -}
converteChar :: Char -> (Char, Char, Int)
converteChar c
  | isLower c = (c, chr (ord c - 32), ord c)  -- minúsculo -> maiúsculo
  | isUpper c = (c, chr (ord c + 32), ord c)  -- maiúsculo -> minúsculo
  | otherwise = (c, c, ord c)
-- converteChar 'b' = ('b','B',98)
-- converteChar 'A' = ('A','a',65)


-- ============================================================
-- EXERCÍCIO 31 - Cadastro de pessoas
-- ============================================================

pessoa :: Int -> (String, Int, Char)
pessoa 1   = ("Joao Silva",       12, 'm')
pessoa 2   = ("Jonas Souza",      51, 'm')
pessoa 3   = ("Maria Ferreira",   25, 'f')
pessoa 4   = ("Ana Paula",        30, 'f')
pessoa 5   = ("Carlos Lima",      19, 'm')
pessoa 321 = ("Jocileide Strauss", 21, 'f')
pessoa _   = ("Nao ha ninguem",  9999, 'x')  -- sentinela

-- Acesso às componentes da tripla:
pNome  (n, _, _) = n
pIdade (_, i, _) = i
pSexo  (_, _, s) = s

{- (a) Nome da pessoa de MENOR idade até registro r -}
nomeMenorIdade :: Int -> String
nomeMenorIdade 1 = pNome (pessoa 1)
nomeMenorIdade r
  | pIdade (pessoa r) < pIdade (pessoa (rgMenorIdade (r-1) 1)) = pNome (pessoa r)
  | otherwise = nomeMenorIdade (r - 1)

-- Auxiliar: retorna o rg da pessoa de menor idade no intervalo [1..r]
rgMenorIdade :: Int -> Int -> Int
rgMenorIdade 1 melhor = melhor
rgMenorIdade r melhor
  | pIdade (pessoa r) < pIdade (pessoa melhor) = rgMenorIdade (r-1) r
  | otherwise                                   = rgMenorIdade (r-1) melhor

nomeJovem :: Int -> String
nomeJovem r = pNome (pessoa (rgMenorIdade r 1))
-- nomeJovem 5 = "Joao Silva" (idade 12)

{- (b) Idade média até registro r -}
somaIdades :: Int -> Int
somaIdades 0 = 0
somaIdades r = pIdade (pessoa r) + somaIdades (r - 1)

idadeMedia :: Int -> Int
idadeMedia r = quociente (somaIdades r) r
-- idadeMedia 5 = (12+51+25+30+19)/5 = 27

{- (c) Número de pessoas do sexo masculino até r -}
contaMasculino :: Int -> Int
contaMasculino 0 = 0
contaMasculino r
  | pSexo (pessoa r) == 'm' = 1 + contaMasculino (r - 1)
  | otherwise                = contaMasculino (r - 1)
-- contaMasculino 5 = 3 (registros 1,2,5)

{- (d) Registro da pessoa de MAIOR idade até r -}
rgMaiorIdade :: Int -> Int
rgMaiorIdade r = rgMaiorAux r 1

rgMaiorAux :: Int -> Int -> Int
rgMaiorAux 1 melhor = melhor
rgMaiorAux r melhor
  | pIdade (pessoa r) > pIdade (pessoa melhor) = rgMaiorAux (r-1) r
  | otherwise                                   = rgMaiorAux (r-1) melhor
-- rgMaiorIdade 5 = 2  (Jonas Souza, 51 anos)


-- ============================================================
-- EXERCÍCIO 32 - Ordenar 4 inteiros em tupla-4
-- ============================================================

{- Estratégia: insertion sort manual em 4 valores.
   Reutiliza menor e maior do exercício 5. -}

-- Retorna os dois menores valores de 4 inteiros em ordem
minDe4 :: Int -> Int -> Int -> Int -> Int
minDe4 a b c d = menor a (menor b (menor c d))

-- Remove o valor v de uma lista (primeira ocorrência)
removeUm :: Int -> [Int] -> [Int]
removeUm _ []     = []
removeUm v (x:xs)
  | v == x    = xs
  | otherwise = x : removeUm v xs

-- Ordena lista de 4 elementos por seleção
ordena4aux :: [Int] -> [Int]
ordena4aux [] = []
ordena4aux xs = m : ordena4aux (removeUm m xs)
  where m = minDe4 (xs!!0) (xs!!1) (xs!!2) (xs!!3)

-- Versão sem !! — seleciona mínimo de lista genérica:
minLista :: [Int] -> Int
minLista [x]    = x
minLista (x:xs) = menor x (minLista xs)

removeMin :: [Int] -> [Int]
removeMin xs = removeUm (minLista xs) xs

ordenaLista4 :: [Int] -> [Int]
ordenaLista4 [] = []
ordenaLista4 xs = minLista xs : ordenaLista4 (removeMin xs)

ordena4 :: Int -> Int -> Int -> Int -> (Int, Int, Int, Int)
ordena4 a b c d = (s!!0, s!!1, s!!2, s!!3)
  where s = ordenaLista4 [a, b, c, d]
-- ordena4 3 5 1 (-3) = (-3,1,3,5)


-- ============================================================
-- EXERCÍCIO 33 - Dias entre duas datas
-- ============================================================

{- Estratégia: converter cada data para "número de dias desde 01/01/0001",
   depois subtrair. Simplificação: anos bissextos verificados com resto. -}

eBissexto :: Int -> Bool
eBissexto a
  | resto a 400 == 0 = True
  | resto a 100 == 0 = False
  | resto a 4   == 0 = True
  | otherwise        = False

diasNoMes :: Int -> Int -> Int
diasNoMes 1 _  = 31
diasNoMes 2 a  = if eBissexto a then 29 else 28
diasNoMes 3 _  = 31
diasNoMes 4 _  = 30
diasNoMes 5 _  = 31
diasNoMes 6 _  = 30
diasNoMes 7 _  = 31
diasNoMes 8 _  = 31
diasNoMes 9 _  = 30
diasNoMes 10 _ = 31
diasNoMes 11 _ = 30
diasNoMes 12 _ = 31
diasNoMes _ _  = 0

diasNoAno :: Int -> Int
diasNoAno a = if eBissexto a then 366 else 365

-- Dias acumulados do mês 1 até mês m-1 no ano a
diasAtesMes :: Int -> Int -> Int
diasAtesMes 1 _ = 0
diasAtesMes m a = diasNoMes (m - 1) a + diasAtesMes (m - 1) a

-- Dias acumulados do ano 1 até ano a-1
diasAtesAno :: Int -> Int
diasAtesAno 1 = 0
diasAtesAno a = diasNoAno (a - 1) + diasAtesAno (a - 1)

-- Converte data para número absoluto de dias
dataToDias :: Int -> Int -> Int -> Int
dataToDias d m a = diasAtesAno a + diasAtesMes m a + d

-- Diferença entre duas datas (data1 <= data2)
diasEntre :: (Int,Int,Int) -> (Int,Int,Int) -> Int
diasEntre (d1,m1,a1) (d2,m2,a2) = dataToDias d2 m2 a2 - dataToDias d1 m1 a1
-- diasEntre (1,1,2025) (31,12,2025) = 364
-- diasEntre (1,3,2024) (1,3,2025)   = 366 (2024 é bissexto)


-- ============================================================
-- EXERCÍCIO 34 - Raízes de equação do 2º grau
-- ============================================================

{- ax² + bx + c = 0
   delta = b² - 4ac
   Se delta >= 0: raízes reais (r1, r2)
   Se delta <  0: raízes imaginárias — retorna erro (usamos (0,0) como indicador)
   Nota: retornamos Float para representar raízes não-inteiras. -}
equacao :: (Float, Float, Float) -> (Float, Float)
equacao (a, b, c)
  | delta < 0  = error "Raizes imaginarias: delta negativo"
  | otherwise  = ((-b + sqrt delta) / (2*a),
                  (-b - sqrt delta) / (2*a))
  where delta = b*b - 4*a*c
-- equacao (1,-5,6) = (3.0, 2.0)  pois x²-5x+6=0 -> (x-2)(x-3)=0


-- ============================================================
-- EXERCÍCIO 35 - Triângulo: tipo e perímetro
-- ============================================================

{- Condição de existência: cada lado < soma dos outros dois.
   Tipos (pelo comprimento dos lados):
   - Equilátero: todos iguais
   - Isósceles:  exatamente dois iguais
   - Escaleno:   todos diferentes -}
triangulo :: (Int, Int, Int) -> (String, Int)
triangulo (a, b, c)
  | not valido       = error "Nao forma triangulo"
  | a == b && b == c = ("Equilatero", perim)
  | a == b || b == c || a == c = ("Isosceles", perim)
  | otherwise        = ("Escaleno", perim)
  where
    valido = (a + b > c) && (a + c > b) && (b + c > a)
    perim  = a + b + c
-- triangulo (7,7,11) = ("Isosceles",25)
-- triangulo (3,4,5)  = ("Escaleno",12)
-- triangulo (5,5,5)  = ("Equilatero",15)


-- ============================================================
-- EXERCÍCIO 36 - Base de professores
-- ============================================================

base :: Int -> (Int, String, String, Char)
base 0  = (1793, "Pedro Paulo",             "MESTRE", 'M')
base 1  = (1797, "Joana Silva Alencar",     "MESTRE", 'F')
base 2  = (1534, "Joao de Medeiros",        "DOUTOR", 'M')
base 3  = (1267, "Claudio Cesar de Sa",     "DOUTOR", 'M')
base 4  = (1737, "Paula de Medeiros",       "MESTRE", 'F')
base 5  = (1888, "Rita de Matos",           "MESTRE", 'F')
base 6  = (1501, "Carlos Henrique",         "DOUTOR", 'M')
base 7  = (1645, "Fernanda Oliveira",       "MESTRE", 'F')
base 8  = (1320, "Marcelo Santos",          "DOUTOR", 'M')
base 9  = (1698, "Tereza Cristina Andrade", "MESTRE", 'F')
base 10 = (0, "", "", '0')  -- sentinela fim

bMatricula (m,_,_,_) = m
bNome      (_,n,_,_) = n
bTitulo    (_,_,t,_) = t
bSexo      (_,_,_,s) = s

{- (a) Número de doutores -}
numDoutores :: Int -> Int
numDoutores 10 = 0
numDoutores i
  | bTitulo (base i) == "DOUTOR" = 1 + numDoutores (i + 1)
  | otherwise                    = numDoutores (i + 1)
-- numDoutores 0 = 4 (índices 2,3,6,8)

{- (b) Número de mulheres -}
numMulheres :: Int -> Int
numMulheres 10 = 0
numMulheres i
  | bSexo (base i) == 'F' = 1 + numMulheres (i + 1)
  | otherwise              = numMulheres (i + 1)
-- numMulheres 0 = 5

{- (c) Mestres do sexo masculino -}
mestreMasc :: Int -> Int
mestreMasc 10 = 0
mestreMasc i
  | bTitulo (base i) == "MESTRE" && bSexo (base i) == 'M'
              = 1 + mestreMasc (i + 1)
  | otherwise = mestreMasc (i + 1)
-- mestreMasc 0 = 2 (índices 0 e... verificar)

{- (d) Nome do professor mais antigo (menor matrícula) -}
nomeAntigo :: Int -> String
nomeAntigo i = bNome (base (idxMenorMatricula i 0))

idxMenorMatricula :: Int -> Int -> Int
idxMenorMatricula 10 melhor = melhor
idxMenorMatricula i melhor
  | bMatricula (base i) < bMatricula (base melhor) =
      idxMenorMatricula (i + 1) i
  | otherwise = idxMenorMatricula (i + 1) melhor
-- nomeAntigo 0 = "Claudio Cesar de Sa" (matrícula 1267)


-- ============================================================
-- EXERCÍCIO 37 - Sistema de biblioteca
-- ============================================================

type Isbn      = Int
type Volumes   = Int
type Titulo    = String
type Matricula = String
type Reserva   = Bool
type Acervo    = [(Isbn, Titulo, Reserva, Volumes)]
type Emprestimo = [(Matricula, Isbn)]

-- Acesso aos campos de Acervo:
aIsbn     (i,_,_,_) = i
aTitulo   (_,t,_,_) = t
aReserva  (_,_,r,_) = r
aVolumes  (_,_,_,v) = v

-- Acesso aos campos de Emprestimo:
eMatricula (m,_) = m
eIsbn      (_,i) = i

{- (a) func_1: livro pode ser emprestado? (Reserva == False) -}
func_1 :: Isbn -> Acervo -> Bool
func_1 _ []     = False
func_1 isbn (l:ls)
  | aIsbn l == isbn = not (aReserva l)   -- True se NÃO reservado
  | otherwise       = func_1 isbn ls
-- func_1 42 acervo => False se reservado, True se disponível

{- (b) func_2: quantos volumes de isbn estão emprestados -}
func_2 :: Isbn -> Emprestimo -> Int
func_2 _ []     = 0
func_2 isbn (e:es)
  | eIsbn e == isbn = 1 + func_2 isbn es
  | otherwise       = func_2 isbn es

{- (c) func_3: quantos volumes do isbn há no acervo -}
func_3 :: Isbn -> Acervo -> Int
func_3 _ []     = 0
func_3 isbn (l:ls)
  | aIsbn l == isbn = aVolumes l
  | otherwise       = func_3 isbn ls

{- (d) func_4: volumes disponíveis para empréstimo
   = 0 se reservado
   = volumes no acervo - volumes emprestados, caso contrário
   Precisa do acervo e emprestimo como parâmetros globais (passados). -}
func_4 :: Isbn -> Acervo -> Emprestimo -> Int
func_4 isbn ac emp
  | not (func_1 isbn ac) = 0   -- reservado ou não encontrado
  | otherwise             = func_3 isbn ac - func_2 isbn emp

{- (e) func_5: registra empréstimo se possível, senão retorna lista original -}
func_5 :: Matricula -> Isbn -> Acervo -> Emprestimo -> Emprestimo
func_5 mat isbn ac emp
  | func_4 isbn ac emp > 0 = (mat, isbn) : emp  -- adiciona empréstimo
  | otherwise               = emp                -- sem disponibilidade
HASKELL
