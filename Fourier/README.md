# Fourier Transform

You must implement a Fourier Transform. That is, you must find which of several
numbers in a given range which is Fouriest.

A number $n$ has a Fouriness equal to the total number of fours in all integers
greater equal 0, which is less or equal to $n$.

A Fourier transform finds the largest Fouriness of any number in a range $n
\dots m$ (inclusive).

# Input

Each input contains two numbers, separated by one space. Each line contains the
numbers $n, m$, such that $0 \leq n leq m$, and $0 \leq m \leq 2^{64}-1$.

# Output

For each output, you must output the Fourier transform of the numbers $n, m$, one per line.

# Examples

## Example 1

Input:

```
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
* 2 is 1
  * There is one "4" digit in the sequence "0, 1, 2"
* The numbers 3-10 is 1
  * There is one "4" digit in the sequence "0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10"

Thus, the Fouriness of the numbers 2 to 10 is 1, and the Fourier transform of 0
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
