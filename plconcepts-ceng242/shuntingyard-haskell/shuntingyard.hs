import Data.Char

data Stack = Empty | Element Char Stack deriving Show
data Stack2 = Empty2 | Element2 Float Stack2 deriving Show
data Stacks = Stack | Stack2

push :: Char -> Stack -> Stack
push elem stack = Element elem stack

pop :: Stack -> Stack
pop (Empty) = (Empty)
pop (Element _ xs) = xs

retrieve :: Stack -> Char
retrieve (Empty) = ' '
retrieve (Element x xs) = x

null' :: Stack -> Bool
null' (Empty) = True
null' (Element x xs) = False

push2 :: Float -> Stack2 -> Stack2
push2 elem stack = Element2 elem stack

pop2 :: Stack2 -> Stack2
pop2 (Empty2) = (Empty2)
pop2 (Element2 _ xs) = xs

retrieve2 :: Stack2 -> Float
retrieve2 (Empty2) = 0
retrieve2 (Element2 x xs) = x

null2 :: Stack2 -> Bool
null2 (Empty2) = True
null2 (Element2 x xs) = False

precedence :: Char -> Int
precedence x
	| elem x ['+', '-'] = 1
	| elem x ['*', '/'] = 2

isoperator :: Char -> Bool
isoperator x = elem x ['+', '-', '*', '/']











operatorhandling :: Char -> ([Char], Stack) -> ([Char],  Stack)
operatorhandling x (queue, stack)
	| null' stack = (queue, (Element x stack))
	| (precedence x) > (precedence (retrieve stack)) = (queue, (push x stack))
	| otherwise = operatorhandling x ((queue ++ [retrieve stack]), (pop stack))


shuntYard :: [Char] -> ([Char], Stack) -> [Char]
shuntYard input (queue, stack)
	| null (input) = if (not (null' stack))
				then shuntYard input ((queue ++ [retrieve stack]), (pop stack))
				else queue
	| isoperator (head input) = shuntYard (tail input) (operatorhandling (head input) (queue, stack))
	| otherwise = shuntYard (tail input) ((queue ++ [head input]), stack)


infixToPostfix :: [Char] -> [Char]
infixToPostfix input = shuntYard input ([], Empty)



















compute :: Float -> Char -> Float -> Float
compute x op y
	| (op == '+') = x + y
	| (op == '-') = x - y
	| (op == '*') = x * y
	| (op == '/') = x / y



chartofloat :: Char -> Float
chartofloat x = toEnum (digitToInt x)




postalgorithm :: [Char] -> Stack2 -> Float
postalgorithm input stack
	|null (input) = retrieve2 stack
	|isoperator (head input) = postalgorithm (tail input) (push2 ( compute (retrieve2 (pop2 stack)) (head input) (retrieve2 stack))       (pop2 (pop2 stack)) )
	|otherwise = postalgorithm (tail input) (push2 (chartofloat (head input)) stack)



evaluatePostfix :: [Char] -> Float
evaluatePostfix input = postalgorithm input Empty2
