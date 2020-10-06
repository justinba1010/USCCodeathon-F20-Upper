# Carl's Cryogenics Corporation Calculator

## Author Notes

This is a problem heavily inspired by one from Cracking the Coding Interview, `16.10 - Living People`. I liked this problem because there is a trick to colving it in linear time

## Problem Statement

Carl works at Cryo Corp, where people are voluntarily frozen for a length of time so as to be "transported" into the future. However, they have a problem; Carl needs to keep track of how many people are frozen at one time, and this is hard with people being frozen and unfrozen all the time. Carl only really needs to keep a yearly count on this, so we will only use the year in our calculations. You can also assume that a person will only be unfrozen and released after being frozen. Given a year, how many people are currently frozen?

## Input Format

Given a year (*Y*), and the length of the list of people (*N*), the input will be a list of people (*P*), with the years that they were frozen (*P1*) and unfrozen (*P2*).

```
Y
N
P1 P2
Q1 Q2
R1 R2
...
```

## Constraints

Since Cryo Corp only has 1 million cryogenic chambers, that's the maximum number of people that we can possibly have.

1 <= *N* <= 1,000,000

A person may be frozen for under 1 year, but since there was in fact a person that occupied a cryogenic chamber, we need to count that as 1 person.

P1 <= P2

However, sometimes these people are frozen for a very long time.

0 <= P1 <= 2,147,483,647

Also, since Carl needs this information as soon as possible, there is a timeout implemented for the test cases.

## Output format

Print out the number of people.

```
5
```