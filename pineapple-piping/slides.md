# Pineapple Piping

* Maximum Bandwidth Path (MBP)
* Most people used BFS/DFS or some variation

Hereafter:

* Pipes are edges, $E$
* Intersections are vertices, $V$

# Dijkstra's algorithm

* Made for shortest path, but easily modified to find MBP
* This is what the reference solution does, in C
	* Not fully optimized
* Greedy algorithm
* $O(|E|log(|V|))$, optimally

# Further Optimizations

* Use a max-heap for the queue
	* Makes your algorithm more greedy

# Another Approach

* Maximum Spanning Tree (MST)
* Only path between $s$ and $t$
* Kruskal's algorithm
	* Only works for undirected graphs
	* $O(|E|log(|V|))$, optimally

# MST

![](kruskal00.png)

# Reference solution

Written in C, only path width used for output as multiple paths are possible

|Test|V     |E       |Time |#|Test|V     |E       |Time |
|----|------|--------|-----|-|--- |------|--------|-----|
| 0  | 6    | 9      | 0.03|#| 9  | 1000 | 2500   | 0.03|
| 1  | 8    | 12     | 0.04|#| 10 | 1000 | 25000  | 0.04|
| 2  | 10   | 25     | 0.04|#| 11 | 1000 | 25000  | 0.04|
| 3  | 10   | 25     | 0.02|#| 12 | 1000 | 50000  | 0.05|
| 4  | 100  | 250    | 0.03|#| 13 | 1000 | 50000  | 0.04|
| 5  | 100  | 250    | 0.02|#| 14 | 10000| 25000  | 0.41|
| 6  | 100  | 2500   | 0.03|#| 15 | 10000| 500000 | 0.54|
| 7  | 100  | 2500   | 0.03|#| 16 | 5000 | 2500000| 0.98|
| 8  | 1000 | 2500   | 0.03|#| 17 | 10000| 2500000| 1.37|

