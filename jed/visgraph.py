from itertools import islice, chain

# Visibility graph from Springer's Computational Geometry

def visgraph(objs, p0, p1, O=0):
    # Optimization levels
    # O0: No optimization -> O(n^3)
    # O1: Remove edges which go into an obstacle
    # O2: Compute visible_vxs efficiently by sorting edges cyclically

    adj = {}
    allpts = [p for obj in objs for p in obj] + [p0, p1]
    edges = [p for obj in objs for p in list(window(obj, 2))]

    for p in allpts:
        adj[p] = set()

    for obj in objs + [None]:
        for i, p in enumerate(obj or {p0, p1}):
            for q in visible_vxs(p, obj, i, allpts, edges, O):
                adj[p].add(q)
                adj[q].add(p)

    if O >= 1:
        # Remove edges which go into an obstacle (rotation 180 degrees would be interior)
        for obj in objs:
            for p_prev, p, p_next in window(obj):
                useless_nbhs = set()
                for q in adj[p]:
                    r = tuple(2*p[i] - q[i] for i in range(2))
                    if interior(p, p_prev, p_next, r):
                        useless_nbhs.add(q)

                adj[p].difference_update(useless_nbhs)
                for q in useless_nbhs:
                    adj[q].discard(p)

    return adj

def visible_vxs(p, obj, i, allpts, edges, O=0):
    visible = set()

    # Add neighbors
    if obj is not None:
        p_prev = obj[(i-1) % len(obj)]
        p_next = obj[(i+1) % len(obj)]
        visible.update({p_prev, p_next})

    # TODO implement O2

    for q in allpts:
        # Never include loop
        if p == q:
            continue

        # Drop if edge goes in polygon interior
        if obj is not None:
            if interior(p, p_prev, p_next, q):
                continue

        # Check for overlapping edges
        for edge in edges:
            if p not in edge and q not in edge and intersect((p, q), edge):
                break
        else:
            visible.add(q)

    return visible

def window(it, n=3):
    it = iter(it)
    res = res0 = tuple(islice(it, n))  # First iteration
    yield res

    # Cycle in each subsequent iteration
    for x in chain(it, res0[:-1]):
        res = res[1:] + (x,)
        yield res

def interior(p, p_prev, p_next, q):
    concave = angle(p_prev, p, p_next) > 0

    ang_prev = angle(p_prev, p, q)
    ang_next = angle(q, p, p_next)
    return not (ang_prev >= 0 and ang_next >= 0) if concave else (ang_prev <= 0 and ang_next <= 0)

def angle(p, q, r):
    # https://www.geeksforgeeks.org/orientation-3-ordered-points/
    px, py = p
    qx, qy = q
    rx, ry = r
    return (qy - py) * (rx - qx) - (ry - qy) * (qx - px)

def intersect(e, f):
    # https://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/
    e0, e1 = e
    f0, f1 = f
    sign_angle = lambda p, q, r: angle(p, q, r) > 0
    return sign_angle(e0, e1, f0) != sign_angle(e0, e1, f1) and \
           sign_angle(f0, f1, e0) != sign_angle(f0, f1, e1)

if __name__ == "__main__":
    # Sample obstacles and path
    from sample import objs, start, end
    adj = visgraph(objs, start, end)

    import visualization
    visualization.show(objs, adj=adj)
