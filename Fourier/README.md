# Fourier Transform

Fourier transform or something? IDK, I never did math before. Pls help, this is
due tomorrow!

A number $n$ has a Fouriness equal to the total number of four-digits in all
integers greater equal 0, which is less or equal to $n$. For example, the
number 14 has a fouriness of 2 because there are two numbers satisfying this
constraint: 4 and 14.

You must implement a Fourier Transform. That is, you must find which of several
numbers in a given range which is Fouriest.

A Fourier transform finds the largest Fouriness of any number in a range $n
\dots m$ (inclusive).

# Input

Each input begins with a number $k$, with $1\leq k \leq 2^{20}$.

The following $k$ many lines contain two numbers $n, m$ in that order,
separated by one space. $0\leq n \leq m \leq 2^{64}-1$.

# Output

For each output, you must output the Fourier transform of the numbers $n, m$,
one per line.

# Examples

## Example 1

Input:

```
3
0 10
5 20
10 24
```

Output:

```
1
2
3
```

Explanation:

The Fouriness of ...
* 0 is 0
* 1 is 0
* …
* 4 is 1
  * There is one "4" digit in the sequence "0, 1, 2, 3, 4"
* The numbers 3-10 is 1
  * There is one "4" digit in the sequence "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10"

Thus, the Fouriness of the numbers 0 to 10 is 1, and the Fourier transform of 0
to 10 is 1, as this is the maximum Fouriness in the given range.

Lets consider the Fouriness of the number 20; we observe that the range of
numbers less or equal to 20 is: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13,
14, 15, 16, 17, 18, 19, 20. This contains two "4" digits, in the numbers 4,
14. Thus, the Fouriness of 20 is 2.

Finally, lets consider the Fouriness of the number 24; we observe that the
range of numbers less or equal to 24 is: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11,
12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24. There are three "4" digits
in this sequence: 4, 14, and 24.

## Example 2

Input:

```
3
10 75
50 200
30 60
```

Output:

```
18
40
16
```

## Example 3

Input:

```
5
10 200
50 100
70 275
1000 2000
1 876543
```

Output:

```
40
20
58
600
541008
```
