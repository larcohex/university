import Network
import qualified Data.Map as M
import Data.List
import Data.Maybe

on :: (b -> b -> c) -> (a -> b) -> a -> a -> c
(.*.) `on` f = \x y -> f x .*. f y

instance (Eq a) => Eq (Node a) where
    (==) = (==) `on` label


instance Ord a => Eq (Network a) where
    (==) = (==) `on` f where
        f :: Ord a => Network a -> M.Map a (M.Map a Int)
        f (Graph nodes) = M.fromList $ map g nodes
        g :: Ord a => Node a -> (a, M.Map a Int)
        g node = (label node, M.fromList $ adjacent node)



mergeNodes :: (Eq a) => (Node a) -> (Maybe (Node a)) -> (Node a)
mergeNodes node1 node2
	| isNothing node2 = node1
	| otherwise = Node (label node1) ([x | x <- (adjacent node1)] ++ [x | x <- (adjacent (fromJust node2)), x `notElem` (adjacent node1)])


checkConnection :: (Eq a) => [Node a] -> [Node a] -> Bool -> Bool
checkConnection graph1 graph2 result
	| null graph1 = result
	| isJust (find (== (head (graph1))) graph2) = True
	| otherwise = checkConnection (tail graph1) graph2 result

merging :: (Eq a) => [Node a] -> [Node a] -> [Node a] -> [Node a]
merging graph1 graph2 result
	| null graph1 = result ++ [x | x <- graph2]
	| null graph2 = result ++ [x | x <- graph1]
	| otherwise = merging (tail (graph1)) rest (result ++ newNode)
	where
		rest = delete (head (graph1)) graph2
		newNode = [mergeNodes (head (graph1)) (find (== (head (graph1))) graph2)] 


merge :: (Eq a) => (Network a) -> (Network a) -> (Network a)
merge (Graph graph1) (Graph graph2)
	| checkConnection graph1 graph2 False = Graph (merging graph1 graph2 [])
	| otherwise = Graph graph1
