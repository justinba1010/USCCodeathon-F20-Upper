from queue import PriorityQueue
from math import sqrt
from sys import stderr

# https://en.wikipedia.org/wiki/A*_search_algorithm

def d2(p, q):
    px, py = p
    qx, qy = q
    return sqrt((py - qy)**2 + (px - qx)**2)

def pathlength(path):
    return sum(d2(p,q) for p, q in zip(path, path[1:]))

def astar(adj, start, end):
    dmin = d2(start, end)
    boundary = PriorityQueue(len(adj.keys()))
    boundary.put((dmin, start))
    explored = set()

    parents = {}

    pathlen = {start: 0}
    heuristic = {start: dmin}

    while not boundary.empty():
        # Get best boundary point
        h, current = boundary.get()

        # Check that the heuristic has not changed
        new_h = heuristic.get(current, None)
        if h != new_h:
            boundary.put((new_h, current))
            continue

        # Reconstruct path from neighbors if at end
        if current == end:
            p = end
            path = [p]
            while p != start:
                p = parents[p]
                path.append(p)
            path.reverse()
            return path

        # Add each neighbor
        for neighbor in adj[current]:
            new_pathlen = pathlen[current] + d2(current, neighbor)
            if new_pathlen < pathlen.get(neighbor, new_pathlen+1):
                parents[neighbor] = current
                pathlen[neighbor] = new_pathlen
                heuristic[neighbor] = h = new_pathlen + d2(neighbor, end)

                if neighbor not in explored:
                    boundary.put((h, neighbor))
                explored.add(neighbor)

    print("No path found", file=stderr)
    return []

if __name__ == "__main__":
    # Sample obstacles and path
    from sample import objs, start, end
    from visgraph import visgraph

    adj = visgraph(objs, start, end)

    apath = astar(adj, start, end)

    import visualization
    visualization.show(objs, apath, adj=adj)
