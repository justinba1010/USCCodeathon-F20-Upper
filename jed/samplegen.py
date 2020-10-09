from visgraph import visible_vxs, visgraph, angle
from traversal import astar, d2, pathlength
from visualization import draw

import matplotlib as mpl
import matplotlib.pyplot as plt
import numpy as np
from random import shuffle, seed
from time import time

from argparse import ArgumentParser
import os, os.path

def interactive(N, width, output=None, ask=False):
    height = int(3/4*width)

    plt.clf()
    plt.tight_layout()
    plt.setp(plt.gca(), autoscale_on=False)
    plt.xlim(xmax=width)
    plt.ylim(ymax=height)
    plt.xticks(np.arange(0,width+1,1), labels=[])
    plt.yticks(np.arange(0,height+1,1), labels=[])
    plt.grid(alpha=0.3)
    plt.draw()

    getpoint = lambda: tuple(np.asarray(plt.ginput(1, timeout=-1)).reshape(2).round().astype(np.int32))
    to_px = lambda p: plt.gca().transData.transform_point(p)
    near = lambda p, q: d2(to_px(p), to_px(q)) < 15

    tmp_patches = []
    objs = []
    for i in range(N):
        plt.title("Objects remaining: %d" % (N-i))
        plt.draw()

        obj = []
        while len(obj) < 3 or obj[0] != obj[-1]:
            pt = getpoint()
            if len(obj) >= 3 and near(obj[0], pt):
                pt = obj[0]  # Force match
            if obj:
                tmp_patches.append(plt.gca().add_patch(mpl.patches.PathPatch(mpl.path.Path([obj[-1], pt]), fc='none', ec='r')))
            else:
                tmp_patches.append(plt.scatter(pt[0], pt[1], c='r'))
            plt.draw()
            obj.append(pt)
        from itertools import cycle
        orientation = sum((x1-x0) * (y1+y0) for (x0,y0), (x1,y1) in zip(obj, obj[1:] + [obj[0]]))
        if orientation > 0:
            obj.reverse()  # ensure ccw
        objs.append(obj[:-1])  # Don't include duplicated last vertex

    plt.title("Select starting point")
    start = getpoint()
    plt.scatter(start[0], start[1], c='k')
    plt.draw()

    plt.title("Select end point")
    end = getpoint()
    plt.scatter(end[0], end[1], c='k')
    plt.draw()

    plt.title("Calculating visibility graph...")
    plt.draw()

    now = time()
    adj = visgraph(objs, start, end)
    path = astar(adj, start, end)
    duration = time() - now

    for patch in tmp_patches:
        patch.remove()
    plt.title("Visibility graph (%.3fs)" % duration)
    draw(objs, path, adj)
    plt.show()

    if output is not None:
        os.makedirs("input", exist_ok=True)
        os.makedirs("output", exist_ok=True)
        iname = os.path.join("input", "input%s.txt" % output)
        oname = os.path.join("output", "output%s.txt" % output)
        if not ((os.path.exists(iname) or os.path.exists(oname))
                and ask and not input("Overwrite? [y/N] ").upper().startswith("Y")):
            with open(iname, "w+") as ifile:
                print("%d %d" % start, file=ifile)
                print("%d %d" % end, file=ifile)
                print(len(objs), file=ifile)
                for obj in objs:
                    print(len(obj), file=ifile)
                    for x, y in obj:
                        print(x, y, file=ifile)
            with open(oname, "w+") as ofile:
                print(pathlength(path), file=ofile)
        else:
            print("Skipping")

if __name__ == "__main__":
    seed(42)
    argp = ArgumentParser()
    argp.add_argument("num_objects", type=int)
    argp.add_argument("width", type=int)
    argp.add_argument("output", nargs="?")
    args = argp.parse_args()
    interactive(args.num_objects, args.width, args.output, ask=True)
