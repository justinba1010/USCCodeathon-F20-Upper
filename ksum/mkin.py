import random
(n, k) = [int(x) for x in input().split()]
x = [str(random.randint(1, 10000)) for _ in range(n)]

w = (random.sample(list(map(int, x)), k))
w.sort()
w = sum(w)


print("{} {} {}".format(n, k, w))
print(" ".join(x))
