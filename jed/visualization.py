import numpy as np
import matplotlib as mpl
import matplotlib.pyplot as plt
from matplotlib.ticker import MaxNLocator

def draw(objs, path=[], adj={}):
    objs = [np.array(obj).reshape((-1,2)).T for obj in objs]

    for obj in objs:
        plt.fill(obj[0], obj[1], color='gray')
    for p, qs in adj.items():
        for q in qs:
            plt.gca().add_patch(mpl.patches.PathPatch(mpl.path.Path([p, q]), fc='none', ec='r'))
    if len(path) >= 2:
        plt.gca().add_patch(mpl.patches.PathPatch(mpl.path.Path(path), fc='none', ec='k'))
    if len(path) >= 1:
        path_np = np.array(path).reshape((-1,2)).T
        plt.scatter(path_np[0,[0,-1]], path_np[1,[0,-1]], color='k')
    plt.scatter([0], [0], color='k', alpha=0)  # Force (0,0) to be included
    plt.grid(alpha=0.3)
    plt.gca().xaxis.set_major_locator(MaxNLocator(integer=True))
    plt.gca().yaxis.set_major_locator(MaxNLocator(integer=True))

def show(objs, path=[], adj={}):
    draw(objs, path, adj)
    plt.show()

if __name__ == "__main__":
    # Sample obstacles and path
    from sample import objs, path
    show(objs, path)
