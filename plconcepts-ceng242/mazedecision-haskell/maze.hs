cellFood :: [Char] -> [(Int, Int)] -> Int -> Int -> [(Int, Int)]
cellFood cell food r c
	| cell == "F" = ((r, c):food)
	| otherwise = food





rowFood :: [[Char]] -> [(Int, Int)] -> Int -> Int -> [(Int, Int)]
rowFood row food r c
	| null row = food
	| otherwise = rowFood (tail row) (cellFood (head row) food r c) r (c + 1)






foodCoordinates :: [[[Char]]] -> [(Int, Int)] -> Int -> [(Int, Int)]
foodCoordinates map food r
	| null map = food
	| otherwise = foodCoordinates (tail map) (rowFood (head map) food r 1) (r + 1)






inScope :: (Int, Int) -> Int -> [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
inScope (r, c) scope food output
	| null food = output
	| ((fst(head food) - r) <= scope) && ((snd(head food) - c) <= scope) = inScope (r, c) scope (tail food) ((head food):output)
	| otherwise = inScope (r, c) scope (tail food) output



manhattanDistance :: (Int, Int) -> (Int, Int) -> Int
manhattanDistance (x1, y1) (x2, y2) = (abs (x1 - x2)) + (abs (y1 - y2))


nearestFood :: (Int, Int) -> [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
nearestFood (r, c) food near
	| null food = near
	| null near = nearestFood (r, c) (tail food) ((head food):near)
	| (manhattanDistance (head food) (r, c)) < (manhattanDistance (head near) (r, c)) = nearestFood (r, c) (tail food) [(head food)]
	| (manhattanDistance (head food) (r, c)) == (manhattanDistance (head near) (r, c)) = nearestFood (r, c) (tail food) ((head food):near)
	| otherwise = nearestFood (r, c) (tail food) near


moreThanOne :: [(Int, Int)] -> Bool
moreThanOne xs
	| null xs = False
	| null (tail xs) = False
	| otherwise = True



minRow :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
minRow input output
	| null input = output
	| null output = minRow (tail input) ((head input):output)
	| (fst (head input)) < (fst (head output)) = minRow (tail input) [(head input)]
	| (fst (head input)) == (fst (head output)) = minRow (tail input) ((head input):output)
	| otherwise = minRow (tail input) output



maxRow :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
maxRow input output
	| null input = output
	| null output = maxRow (tail input) ((head input):output)
	| (fst (head input)) > (fst (head output)) = maxRow (tail input) [(head input)]
	| (fst (head input)) == (fst (head output)) = maxRow (tail input) ((head input):output)
	| otherwise = maxRow (tail input) output



minCol :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
minCol input output
	| null input = output
	| null output = minCol (tail input) ((head input):output)
	| (snd (head input)) < (snd (head output)) = minCol (tail input) [(head input)]
	| (snd (head input)) == (snd (head output)) = minCol (tail input) ((head input):output)
	| otherwise = minCol (tail input) output




maxCol :: [(Int, Int)] -> [(Int, Int)] -> [(Int, Int)]
maxCol input output
	| null input = output
	| null output = maxCol (tail input) ((head input):output)
	| (snd (head input)) > (snd (head output)) = maxCol (tail input) [(head input)]
	| (snd (head input)) == (snd (head output)) = maxCol (tail input) ((head input):output)
	| otherwise = maxCol (tail input) output



upperFood :: [[Char]] -> [(Int, Int)] -> (Int, Int)
upperFood next near
	| null (tail near) = (head near)
	| (moreThanOne (minRow near [])) = targetFood next (minRow near [])
	| otherwise = (head (minRow near []))



lowerFood :: [[Char]] -> [(Int, Int)] -> (Int, Int)
lowerFood next near
	| null (tail near) = (head near)
	| (moreThanOne (maxRow near [])) = targetFood next (maxRow near [])
	| otherwise = (head (maxRow near []))





leftFood :: [[Char]] -> [(Int, Int)] -> (Int, Int)
leftFood next near
	| null (tail near) = (head near)
	| (moreThanOne (minCol near [])) = targetFood next (minCol near [])
	| otherwise = (head (minCol near []))


rightFood :: [[Char]] -> [(Int, Int)] -> (Int, Int)
rightFood next near
	| null (tail near) = (head near)
	| (moreThanOne (maxCol near [])) = targetFood next (maxCol near [])
	| otherwise = head (maxCol near [])




targetFood :: [[Char]] -> [(Int, Int)] -> (Int, Int)
targetFood dir near
	| null near = (-1, -1)
	| null (tail near) = (head near)
	| (head dir) == "Up" = upperFood (tail dir) near
	| (head dir) == "Down" = lowerFood (tail dir) near
	| (head dir) == "Left" = leftFood (tail dir) near
	| (head dir) == "Right" = rightFood (tail dir) near




cellObst :: [Char] -> [(Int, Int)] -> Int -> Int -> [(Int, Int)]
cellObst cell obst r c
	| cell == "X" = ((r, c):obst)
	| otherwise = obst





rowObst :: [[Char]] -> [(Int, Int)] -> Int -> Int -> [(Int, Int)]
rowObst row obst r c
	| null row = obst
	| otherwise = rowObst (tail row) (cellObst (head row) obst r c) r (c + 1)






allObstacles :: [[[Char]]] -> [(Int, Int)] -> Int -> [(Int, Int)]
allObstacles map obst r
	| null map = obst
	| otherwise = allObstacles (tail map) (rowObst (head map) obst r 1) (r + 1)




nearObstacles :: [(Int, Int)] -> (Int, Int) -> [(Int, Int)] -> [(Int, Int)]
nearObstacles obst (r, c) output
	| null obst = output
	| (fst (head obst) == (r + 1)) && (snd (head obst) == c) = nearObstacles (tail obst) (r, c) ((head obst):output)
	| (fst (head obst) == (r - 1)) && (snd (head obst) == c) = nearObstacles (tail obst) (r, c) ((head obst):output)
	| (snd (head obst) == (c + 1)) && (fst (head obst) == r) = nearObstacles (tail obst) (r, c) ((head obst):output)
	| (snd (head obst) == (c - 1)) && (fst (head obst) == r) = nearObstacles (tail obst) (r, c) ((head obst):output)
	| otherwise = nearObstacles (tail obst) (r, c) output







priority :: [[Char]] -> [Char] -> Int
priority prior move
	| move == (head prior) = 1
	| move == (head (tail prior)) = 2
	| move == (last (init prior)) = 3
	| move == (last prior) = 4




assignDistance :: (Int, Int) -> (Int, Int) -> [[Char]] -> [([Char], Int)] -> [([Char], Int)]
assignDistance (r1, c1) (r2, c2) moves output
	| null moves = output
	| (head moves) == "Up" = assignDistance (r1, c1) (r2, c2) (tail moves) (("Up", (manhattanDistance (r1 - 1, c1) (r2, c2))):output)
	| (head moves) == "Down" = assignDistance (r1, c1) (r2, c2) (tail moves) (("Down", (manhattanDistance (r1 + 1, c1) (r2, c2))):output)
	| (head moves) == "Left" = assignDistance (r1, c1) (r2, c2) (tail moves) (("Left", (manhattanDistance (r1, c1 - 1) (r2, c2))):output)
	| (head moves) == "Right" = assignDistance (r1, c1) (r2, c2) (tail moves) (("Right", (manhattanDistance (r1, c1 + 1) (r2, c2))):output)


sortByDistance :: [([Char], Int)] -> [([Char], Int)]
sortByDistance [] = []
sortByDistance moves = sortByDistance small ++ mid ++ sortByDistance large
	where
		small = [x | x <- moves, snd x < snd (head moves)]
		mid = [x | x <- moves, snd x == snd (head moves)]
		large = [x | x <- moves, snd x > snd (head moves)]


sortByPrior :: [([Char], Int)] -> [[Char]] -> [[Char]] -> [[Char]]
sortByPrior moves prior result
	| null (tail moves) = reverse ((fst (head moves)):result)
	| snd (head moves) < snd (head (tail moves)) = sortByPrior (tail moves) prior ((fst (head moves)):result)
	| snd (head moves) == snd (head (tail moves)) = if ((priority prior (fst (head moves))) < (priority prior (fst (head (tail moves)))))
								then sortByPrior (tail moves) prior ((fst (head moves)):result)
								else sortByPrior ((head moves):(tail (tail moves))) prior ((fst (head (tail moves))):result)



upObst :: (Int, Int) -> [(Int, Int)] -> Bool
upObst (r1, c1) obstacles = ((r1 - 1, c1) `elem` obstacles)

downObst :: (Int, Int) -> [(Int, Int)] -> Bool
downObst (r1, c1) obstacles = ((r1 + 1, c1) `elem` obstacles)

leftObst :: (Int, Int) -> [(Int, Int)] -> Bool
leftObst (r1, c1) obstacles = ((r1, c1 - 1) `elem` obstacles)

rightObst :: (Int, Int) -> [(Int, Int)] -> Bool
rightObst (r1, c1) obstacles = ((r1, c1 + 1) `elem` obstacles)



upCheck :: Int -> [(Int, Int)] -> (Int, Int) -> Bool
upCheck size  obstacles (r1, c1)
	| r1 == 1 = False
	| upObst (r1, c1) (nearObstacles obstacles (r1, c1) []) = False
	| otherwise = True

downCheck :: Int -> [(Int, Int)] -> (Int, Int) -> Bool
downCheck size obstacles (r1, c1)
	| r1 == size = False
	| downObst (r1, c1) (nearObstacles obstacles (r1, c1) []) = False
	| otherwise = True


leftCheck :: Int -> [(Int, Int)] -> (Int, Int) -> Bool
leftCheck size obstacles (r1, c1)
	| c1 == 1 = False
	| leftObst (r1, c1) (nearObstacles obstacles (r1, c1) []) = False
	| otherwise = True


rightCheck :: Int -> [(Int, Int)] -> (Int, Int) -> Bool
rightCheck size obstacles (r1, c1)
	| c1 == size = False
	| rightObst (r1, c1) (nearObstacles obstacles (r1, c1) []) = False
	| otherwise = True





makeMove :: Int -> (Int, Int) -> [(Int, Int)] -> [[Char]] -> (Int, Int)
makeMove size (r1, c1) obstacles moves
	| (head moves) == "Up" = if (upCheck size obstacles (r1, c1))
					then (r1 - 1, c1)
					else makeMove size (r1, c1) obstacles (tail moves)
	| (head moves) == "Down" = if (downCheck size obstacles (r1, c1))
					then (r1 + 1, c1)
					else makeMove size (r1, c1) obstacles (tail moves)
	| (head moves) == "Left" = if (leftCheck size obstacles (r1, c1))
					then (r1, c1 - 1)
					else makeMove size (r1, c1) obstacles (tail moves)
	| (head moves) == "Right" = if (rightCheck size obstacles (r1, c1))
					then (r1, c1 + 1)
					else makeMove size (r1, c1) obstacles (tail moves)




move :: Int -> (Int, Int) -> (Int, Int) -> [[Char]] -> [[Char]] -> [(Int, Int)] -> (Int, Int)
move size (r1, c1) (r2, c2) nfmoves fmoves obstacles
	| r2 == -1 = makeMove size (r1, c1) obstacles nfmoves
	| otherwise = makeMove size (r1, c1) obstacles (sortByPrior (sortByDistance (assignDistance (r1, c1) (r2, c2) fmoves [])) fmoves [])




transform :: (Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char]) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)
transform (r, c, nf1, nf2, nf3, nf4, f1, f2, f3, f4) = ((r, c), [nf1, nf2, nf3, nf4], [f1, f2, f3, f4], False, False, False)



changeAgentSet :: [(Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char])] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)]
changeAgentSet agents result
	| null agents = result
	| otherwise = changeAgentSet (tail agents) ((transform (head agents)):result)


backtransform :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> (Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char])
backtransform ((r, c), [nf1, nf2, nf3, nf4], [f1, f2, f3, f4], a, b, d) = (r, c, nf1, nf2, nf3, nf4, f1, f2, f3, f4)





returnAgentSet :: [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [(Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char])] -> [(Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char])]
returnAgentSet agents result
	| null agents = result
	| otherwise = returnAgentSet (tail agents) ((backtransform (head agents)):result)



agentPos :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> (Int, Int)
agentPos (pos, a, b, c, d, e) = pos



agentFoodMove :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> [[Char]]
agentFoodMove (a, b, foodmove, c, d, e) = foodmove


agentNoFoodMove :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> [[Char]]
agentNoFoodMove (a, nofoodmove, b, c, d, e) = nofoodmove



hasmoved :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> Bool
hasmoved (a, b, c, d, e, f) = e


notEqualAgents :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> Bool
notEqualAgents (a1, b1, c1, d1, e1, f1) (a2, b2, c2, d2, e2, f2)
	| a1 /= a2 = True
	| b1 /= b2 = True
	| c1 /= c2 = True
	| d1 /= d2 = True
	| e1 /= e2 = True
	| f1 /= f2 = True
	| otherwise = False



checkit :: (Int, Int) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)
checkit coor (c, a, b, d, e, f)
	| coor == c = (c, a, b, d, e, f)
	| otherwise = ((-1, -1), [], [], False, False, False)




findAgent :: (Int, Int) -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)
findAgent coor agents result
	| null agents = result
	| notEqualAgents result ((-1, -1), [], [], False, False, False) = result
	| otherwise = findAgent coor (tail agents) (checkit coor (head agents))



agentChange :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [[[Char]]] -> (Int, Int) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)
agentChange (old, nfmoves, fmoves, food, moved, toDelete) agents mymap newpos
	| notEqualAgents a ((-1, -1), [], [], False, False, False) = if (hasmoved a)
								then (newpos, nfmoves, fmoves, food, True, True)
								else (newpos, nfmoves, fmoves, food, True, False)
	| (newpos `elem` allfood) = (newpos, nfmoves, fmoves, True, True, False)
	| otherwise = (newpos, nfmoves, fmoves, food, True, False)
	where
		a = findAgent newpos agents ((-1, -1), [], [], False, False, False)
		allfood = foodCoordinates mymap [] 1




assignAgents :: [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> Int -> [[[Char]]] -> Int -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)]
assignAgents agents size mymap scope result
	| null agents = result
	| otherwise = assignAgents (tail agents) size mymap scope ((agentChange (head agents) result mymap a):result)
	where
		a = move size (agentPos (head agents)) newpos (agentNoFoodMove (head agents)) (agentFoodMove (head agents)) obstacles
		newpos = targetFood (agentFoodMove (head agents)) nearest
		nearest = nearestFood (agentPos (head agents)) inscope []
		inscope = inScope (agentPos (head agents)) scope food []
		food = foodCoordinates mymap [] 1
		obstacles = allObstacles mymap [] 1


agentToDelete :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> Bool
agentToDelete (a, b, c, d, e, f) = f


agentOnFood :: ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> Bool
agentOnFood (a, b, c, d, e, f) = d



deleteCopy :: (Int, Int) -> ((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool) -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)]
deleteCopy coor (c, a, b, d, e, f)
	| coor == c = []
	| otherwise = [(c, a, b, d, e, f)]


deleteCopies :: (Int, Int) -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)]
deleteCopies ref agents result
	| null agents = result
	| otherwise = deleteCopies ref (tail agents) ((deleteCopy ref (head agents)) ++ result)



filterAgents :: [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)]
filterAgents agents result
	| null agents = result
	| agentToDelete (head agents) = filterAgents (deleteCopies (agentPos (head agents)) agents []) result
	| otherwise = filterAgents (tail agents) ((head agents):result)



clearInCell :: [Char] -> [Char]
clearInCell cell
	| cell == "A" = "-"
	| otherwise = cell


clearInRow :: [[Char]] -> [[Char]] -> [[Char]]
clearInRow row result
	| null row = result
	| otherwise = clearInRow (tail row) (result ++ [(clearInCell (head row))])



clearAllAgents :: [[[Char]]] -> [[[Char]]] -> [[[Char]]]
clearAllAgents map result
	| null map = result
	| otherwise = clearAllAgents (tail map) (result ++ [(clearInRow (head map) [])])



changeRow :: [[Char]] -> Int -> Int -> [[Char]] -> [[Char]]
changeRow row c counter result
	| null row = result
	| c == counter = changeRow (tail row) c (counter + 1) (result ++ ["A"])
	| otherwise = changeRow (tail row) c (counter + 1) (result ++ [(head row)])



agentToMap :: [[[Char]]] -> (Int, Int) -> Int -> [[[Char]]] -> [[[Char]]]
agentToMap mymap agent counter result
	| null mymap = result
	| fst (agent) == counter = agentToMap (tail mymap) agent (counter + 1) (result ++ [(changeRow (head mymap) (snd agent) 1 [])])
	| otherwise = agentToMap (tail mymap) agent (counter + 1) (result ++ [(head mymap)])


changeMap :: [[[Char]]] -> [((Int, Int), [[Char]], [[Char]], Bool, Bool, Bool)] -> [[[Char]]]
changeMap mymap agents
	| null agents = mymap
	| otherwise = changeMap (agentToMap mymap (agentPos (head agents)) 1 []) (tail agents)


search :: [[[Char]]] -> Int -> Int -> [(Int, Int, [Char], [Char], [Char], [Char], [Char], [Char], [Char], [Char])] -> [[[Char]]]
search mymap scope steps agents
	| steps == 0 = mymap
	| otherwise = search (changeMap (clearAllAgents mymap []) a) scope (steps - 1) newagents
	where
		newagents = returnAgentSet a []
		a = filterAgents b []
		b = assignAgents c (length mymap) mymap scope []
		c = changeAgentSet agents []





exmaze4=[["-","-","-","-","-"],["-","-","-","-","F"],["-","X","A","-","-"],["F","-","X","-","-"],["-","-","-","-","-"]]
exagents4 :: [(Int,Int,[Char],[Char],[Char],[Char],[Char],[Char],[Char],[Char])]
exagents4 = [(3,3,"Left","Up","Down","Right","Left","Right","Down","Up")]



maze = [["-", "F", "F", "F", "X", "A", "-", "-", "X", "X"], ["-", "-", "-", "F", "A", "F", "-", "-", "X", "-"], ["-", "-", "-", "X", "A", "A", "-", "-", "F", "-"], ["X", "-", "A", "A", "A", "X", "F", "-", "X", "A"], ["A", "-", "-", "A", "-", "-", "F", "-", "-", "X"], ["A", "X", "A", "A", "X", "X", "A", "-", "F", "-"], ["-", "-", "-", "-", "-", "-", "-", "-", "A", "-"], ["A", "-", "-", "-", "-", "A", "-", "-", "F", "A"], ["-", "X", "-", "X", "A", "A", "-", "A", "-", "X"], ["-", "-", "X", "-", "F", "-", "A", "A", "X", "-"]]
 
agents :: [(Int,Int,String,String,String,String,String,String,String,String)]
 
agents = [(4, 5, "Down", "Right", "Up", "Left", "Down", "Right", "Up", "Left"), (6, 3, "Right", "Up", "Left", "Down", "Right", "Left", "Down", "Up"), (4, 3, "Down", "Up", "Left", "Right", "Up", "Down", "Right", "Left"), (10, 8, "Down", "Right", "Left", "Up", "Right", "Up", "Left", "Down"), (3, 6, "Right", "Left", "Up", "Down", "Left", "Right", "Down", "Up"), (9, 6, "Left", "Right", "Up", "Down", "Down", "Left", "Up", "Right"), (8, 10, "Right", "Left", "Down", "Up", "Right", "Left", "Down", "Up"), (5, 1, "Up", "Down", "Right", "Left", "Left", "Right", "Up", "Down"), (6, 4, "Up", "Right", "Down", "Left", "Left", "Up", "Right", "Down"), (2, 5, "Down", "Up", "Right", "Left", "Up", "Right", "Left", "Down"), (4, 10, "Right", "Up", "Left", "Down", "Left", "Down", "Up", "Right"), (7, 9, "Up", "Left", "Down", "Right", "Left", "Right", "Down", "Up"), (3, 5, "Down", "Right", "Up", "Left", "Left", "Right", "Down", "Up"), (6, 7, "Down", "Right", "Left", "Up", "Down", "Left", "Right", "Up"), (4, 4, "Left", "Down", "Right", "Up", "Down", "Up", "Left", "Right"), (8, 1, "Up", "Left", "Down", "Right", "Up", "Right", "Down", "Left"), (5, 4, "Right", "Up", "Down", "Left", "Down", "Right", "Left", "Up"), (1, 6, "Right", "Up", "Down", "Left", "Left", "Right", "Up", "Down"), (8, 6, "Up", "Down", "Left", "Right", "Down", "Left", "Right", "Up"), (10, 7, "Left", "Right", "Up", "Down", "Up", "Left", "Down", "Right"), (9, 5, "Down", "Up", "Right", "Left", "Up", "Left", "Down", "Right"), (9, 8, "Up", "Right", "Left", "Down", "Up", "Right", "Down", "Left"), (6, 1, "Left", "Up", "Right", "Down", "Up", "Right", "Left", "Down")]
