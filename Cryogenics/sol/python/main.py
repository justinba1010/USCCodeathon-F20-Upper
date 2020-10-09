#!/usr/bin/python3

# This is a reference solution to Carl's Cryogenics Corporation Calculator

year = int(input())
n = int(input())

births = []
deaths = []

for i in range(0,n):
    a = int(input())
    births.append(a)
    b = int(input())
    deaths.append(b)

births.sort()
deaths.sort()

bptr = 0
dptr = 0
alive = 0

while bptr < len(births) and dptr < len(deaths):
    # Process Births first
    if births[bptr] <= deaths[dptr]:
        if births[bptr] <= year:
            alive += 1
            bptr += 1
        else:
            bptr = len(births) - 1
    else:
        if deaths[dptr] < year:
            alive -= 1
            dptr += 1
        else:
            dptr = len(deaths) - 1

print(alive)