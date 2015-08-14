result1 = merge (Graph [(Node 'a' [('b', 3), ('c', 2)]), (Node 'b' [('c', 3)]), (Node 'c' [])]) (Graph [(Node 'a' [('d', 2)]), (Node 'b' [('d', 3), ('a', 2)]), (Node 'd' [('e', 3)]), (Node 'e' [])])

result2 = Graph [(Node 'a' [('b', 3), ('c', 2)]), (Node 'b' [('c', 3)]), (Node 'c' [])] == Graph [(Node 'a' [('d', 2)]), (Node 'b' [('d', 3), ('a', 2)]), (Node 'd' [('e', 3)]), (Node 'e' [])]

result3 = Graph [(Node 'a' [('b', 3),('c', 2)]), (Node 'b' [('c', 3)]), (Node 'c' [])] == Graph [(Node 'a' [('b',3), ('c',2)]), (Node 'b' [('c', 3)]), (Node 'c' [])]


result4 = Node 'a' [('b', 3), ('c', 2)] == Node 'a' [('c', 2),('b', 3)]


b1 = Graph [Node 'a' [('b', 2)], Node 'b' [('c', 1)], Node 'c' []]
b2 = Graph [Node 'a' [('d', 2)], Node 'b' [('d', 3), ('a', 2)], Node 'd' [('e', 3)], Node 'e' []]
b3 = Graph [Node 'e' [('f', 3)], Node 'f' []]
b4 = Graph [Node 'a' [('b', 1), ('c', 3)], Node 'b' [('c', 2)], Node 'c' [], Node 'd' [('e', 4)], Node 'e' []]
b5 = Graph [Node 'c' [('e', 5)], Node 'e' []]
b41 = Graph [Node 'a' [('c',3),('b',1)], Node 'b' [('c', 2)], Node 'c' [], Node 'd' [('e', 4)], Node 'e' []]
b42 = Graph [Node 'e' [], Node 'a' [('c', 3),('b', 1)], Node 'c' [], Node 'b' [('c', 2)], Node 'd' [('e', 4)]]
b6 = Graph [Node 'a' [('t', 2)], Node 't' []]
b7 = Graph [Node 's' [('e', 3)],Node 'e' []]




test1=mergeb1b2==Graph[Node{label='a',adjacent=[('b',2),('d',2)]},Node{label='b',adjacent=[('c',1),('d',3),('a',2)]},Node{label='c',adjacent=[]},Node{label='d',adjacent=[('e',3)]},Node{label='e',adjacent=[]}]
test2=mergeb1b3==Graph[Node{label='a',adjacent=[('b',2)]},Node{label='b',adjacent=[('c',1)]},Node{label='c',adjacent=[]}]
test3=mergeb2b3==Graph[Node{label='a',adjacent=[('d',2)]},Node{label='b',adjacent=[('d',3),('a',2)]},Node{label='d',adjacent=[('e',3)]},Node{label='e',adjacent=[('f',3)]},Node{label='f',adjacent=[]}]
test4=mergeb6b1==Graph[Node{label='a',adjacent=[('t',2),('b',2)]},Node{label='t',adjacent=[]},Node{label='b',adjacent=[('c',1)]},Node{label='c',adjacent=[]}]
test5=mergeb6b7==Graph[Node{label='a',adjacent=[('t',2)]},Node{label='t',adjacent=[]}]
test6=mergeb4b5==Graph[Node{label='a',adjacent=[('b',1),('c',3)]},Node{label='b',adjacent=[('c',2)]},Node{label='c',adjacent=[('e',5)]},Node{label='d',adjacent=[('e',4)]},Node{label='e',adjacent=[]}]
test7=b41==b42
test8=b41==b4
test9=b42==b4
test10=b1==b7
