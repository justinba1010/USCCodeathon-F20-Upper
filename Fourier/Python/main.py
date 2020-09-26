#!/usr/bin/env python3

import sys
import re

def FoursInRangeNaive(n):
    return sum((len(re.sub("[^4]", "", str(x))) for x in range(n+1)))

def CountFoursInRangeAtDigit(n, d):
    PowerOf10 = int(pow(10, d))
    NextPowerOf10 = PowerOf10 * 10
    right = n % PowerOf10

    roundDown = n - n % NextPowerOf10
    roundUp = roundDown + NextPowerOf10

    digit = int(n / PowerOf10) % 10
    if digit < 4:
        return int(roundDown / 10)
    elif digit == 4:
        return int(roundDown / 10) + right + 1
    else:
        return int(roundUp / 10)


def FoursInRangeOptimal(n):
    return sum( (CountFoursInRangeAtDigit(n, d) for d in range(len(str(n)))) )

def FourierTransform(n, m):
    # we just need to run this on m, the other values smaller than m cannot
    # possibly be any fourier than m, since they will count fours across
    # smaller sets
    return FoursInRangeOptimal(m)

    #  Fours = FoursInRangeOptimal(n)
    #  i = n
    #  while i <= m:
    #      iFours = FoursInRangeOptimal(i)
    #      if iFours > Fours:
    #          Fours = iFours
    #      i += 1
    #  return Fours


for line in sys.stdin:
    n, m = [int(f) for f in line.split()]
    print(FourierTransform(n, m))
