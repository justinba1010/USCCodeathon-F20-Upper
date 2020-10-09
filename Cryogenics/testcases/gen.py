#!/usr/bin/python3

# Generates a testcase

import sys
import random

if (len(sys.argv) < 3):
    print("Usage: ./gen.py <Max # People> <Max Year>")
    sys.exit(1)

maxPeople = int(sys.argv[1])
maxYear = int(sys.argv[2])

if maxPeople < 0 or maxYear < 0:
    print("ERROR: Max People an Max Year must be positive!",file=sys.stderr)
    sys.exit(1)

year = random.randint(0, maxYear)
people = random.randint(0, maxPeople)

print(year)
print(people)

for i in range(0, people):
    b = random.randint(0, maxYear)
    d = random.randint(b, maxYear)
    print(str(b) + " " + str(d))
