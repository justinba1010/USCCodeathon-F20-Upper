from visgraph import visgraph
from traversal import astar, pathlength

from argparse import ArgumentParser

def parse_input():
    start = tuple(map(int, input().split()))
    end = tuple(map(int, input().split()))
    objs = []
    num_objs = int(input())
    for i in range(num_objs):
        obj = []
        obj_size = int(input())
        for j in range(obj_size):
            obj.append(tuple(map(int, input().split())))
        objs.append(obj)
    return objs, start, end

def main():
    argp = ArgumentParser()
    argp.add_argument("-v", "--view", action="store_true")
    argp.add_argument("-n", "--no-visgraph", action="store_true")
    argv = argp.parse_args()

    objs, start, end = parse_input()
    G = visgraph(objs, start, end)
    path = astar(G, start, end)

    if argv.view:
        import visualization
        visualization.show(objs, path, adj={} if argv.no_visgraph else G)

    print(pathlength(path))

if __name__ == "__main__":
    main()
