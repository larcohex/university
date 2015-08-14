module network where


data Node a = Node {
label :: a,
adjacent :: [(a,Int)]
} deriving Show



data Network a = Graph [Node a] deriving Show
