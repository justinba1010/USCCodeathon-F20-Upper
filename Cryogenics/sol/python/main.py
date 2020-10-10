#!/usr/bin/python3

# This is a reference solution to Carl's Cryogenics Corporation Calculator

year = int(input())
n = int(input())

births = []
deaths = []

for i in range(0,n):
    line = input().split(" ")
    a = int(line[0])
    births.append(a)
    b = int(line[1])
    deaths.append(b)

births.sort()
deaths.sort()

bptr = 0
dptr = 0
alive = 0
bdone = False
ddone = False

while dptr < len(deaths) and not(ddone):
    # Process Births first
    if not bdone and bptr < len(births) and births[bptr] <= deaths[dptr]:
        if births[bptr] <= year:
            alive += 1
            # print("Alive", str(births[bptr]), str(alive))
            bptr += 1
        else:
            # print("Births Done")
            bdone = True
    else:
        if deaths[dptr] < year:
            alive -= 1
            # print("Death", str(deaths[dptr]), str(alive))
            dptr += 1
        else:
            # print("Deaths Done")
            dptr = len(deaths) - 1
            ddone = True

print(alive)