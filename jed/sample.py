# Each object is given in clockwise order
# Objects do not overlap
# No lines are colinear (maybe this will change)
objs = [[(0,1), (3,1), (2,2)],
        [(4,6), (6,7), (6,10), (2,7)]]

path = [(0,0), (0,1), (2,7), (5,10)]
start, end = path[0], path[-1]
