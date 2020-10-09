### Challenge Name

Pineapple Piping

### Description

Kenny has taken the pineapple game up a notch.

### Problem Statement

Kenny has some pineapples he needs to send from a source station, $s$, to a
target station, $t$, in a collection of pipes with $n$ intersections. He will
use only one route of pipes to accomplish this, but the pineapples can travel
either way along a given pipe. A pineapple has a width of 1, and so $m$
pineapples can fit in an $m$-wide pipe.

### Input Format

$n$	$s$	$t$

$i_0:w_0$	$i_1:w_1$ $\dots$

$i_0:w_0$	$i_1:w_1$ $\dots$

$\vdots$

$i_0:w_0$	$i_1:w_1$ $\dots$

$n$ is the number of intersections. The number of lines of input will be $n+1$.

$s$ is the source intersection. $t$ is the target intersection. $n$, $s$, and
$t$ are separated by tabs.

Each line after the first is a tab-separated list of colon-separated pairs.
The first of the two numbers is the intersection that is neighbor to the
intersection referred to by the current line. The second number is the width of
the pipe between these intersections. This applies for every pair on the line.

Each line corresponds to an intersection. The line-number intersection-number
relationship is: line = intersection + 2.

### Constraints

$s \ne t$

$0 \le s,t \lt n$

The width of all pipes is less than $1001$

Pipes are bidirectional

All pipes end in 2 distinct intersections.

All intersections have at least two pipes attached.

### Output Format

$p$

Where $p$ is the maximum amount of pineapples Kenny can send at once, using
only one path.

### Tags


