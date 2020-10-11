numrows,numcols = [int(i) for i in input().split()]
heights = []
lakelevels = []
for line in range(numrows):
    heights.append([int(char) for char in input().strip()])
    lakelevels.append([10 for char in range(numcols)])
queue = set()
for row in range(numrows):
    for col in range(numcols):
        if row in [0, numrows-1] or col in [0, numcols-1]:
            lakelevels[row][col] = heights[row][col]
            queue.add((row,col))
while queue:
    row,col = queue.pop()
    for neighborrow,neighborcol in [(row,col-1),(row,col+1),(row-1,col),(row+1,col)]:
        if neighborrow in range(numrows) and neighborcol in range(numcols):
            templake = max(lakelevels[row][col], heights[neighborrow][neighborcol])
            if templake < lakelevels[neighborrow][neighborcol]:
                lakelevels[neighborrow][neighborcol] = templake
                queue.add((neighborrow,neighborcol))
area = 0
for row in range(numrows):
    for col in range(numcols):
        if lakelevels[row][col] > heights[row][col]:
            area += 1
print(area)
