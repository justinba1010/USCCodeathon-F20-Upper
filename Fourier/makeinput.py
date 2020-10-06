#!/usr/bin/env python3

import random
import sys

# medium
if random.randint(0, 100) > 60:
    sys.stderr.write("medium\n")
    min_outputs = 1
    max_outputs = 4096
    min_n = 1
    max_m = 2**16

# hard
elif random.randint(0, 100) > 75:
    sys.stderr.write("hard\n")
    min_outputs = 1
    max_outputs = 2**16
    min_n = 1
    max_m = 2**28-1

# easy
else:
    sys.stderr.write("easy\n")
    min_outputs = 1
    max_outputs = 20
    min_n = 1
    max_m = 200


k = random.choice(range(min_outputs, max_outputs))

print(k)

for i in range(k):
    n = random.randint(min_n, max_m-3)
    m = random.randint(n+1, max_m)
    print("{} {}".format(n, m))
