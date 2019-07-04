---
title: Rod Cutting
---

# Dynamic Programming - Rod Cutting

## Introduction

Dynamic programming is an established algorithm design method. 
Often it is used to solve problems where problem of size P(N) is solved using
solution to problems of size P(N - 1) (or smaller). Introductory example is
calculation of Fibonacci numbers where F(N) (problem of size N) is calculated
as sum of smaller problems F(N - 2) + F(N - 1). Usually smaller problems
are calculated many times. To avoid repeatable calculations their result
is memorized in an array. This technique (known as memorization) significantly
speeds up calculations. In the next sections I will present classic
problem known as rod cutting which is solved using dynamic programming.

## Problem

Rod cutting problem is formulated as maximum profit which
can be earned by a company which cuts rods. Each section of a rod can
be sold earning some money. The question is how to cut the rod so that
income is maximized.

## Recursive Algorithm


```swift
func max(a -> int, b -> int) -> int
{
    a > b ? a : b
}
```

As rod cutting problem mentions maximum profit, likely maximum function
is needed. The above code snippet presents such function.

```swift
func cutrod(price[_] -> int, len -> int) -> int
{
    var i = 0;
    var max_p = -1;
    
    if (len <= 0)
    {
        max_p = 0
    }
    else
    {
         while (i < len)
         {
             max_p = max(max_p, price[i] + cutrod(price, len - i - 1));
             i = i + 1
         }
    };
    
    max_p
}
```

Now a little more difficult part. Given price list (in array ```price```)
of different rod lengths and length of the rod, maximum profit ```max_p``` can
be calculated as profit gained by cutting rod of length ```i``` plus profit earned by
cutting rod of ```length - i``` (in the code above ```1``` is subtracted as
rod of length ```i``` is at index ```i - 1``` in array ```price```). Please note
that problem of size cutrod(len) is calculated using solution to problem
of size cutrod(len - 1). This sentence can be formulated by simple
expression ```max_p = max(max_p, price[i] + cutrod(price, len - i - 1))```.

```swift
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] -> int;
    
    cutrod(price, 8)
}
```

An example of maximizing profit gained by cutting a rod of size 8 where
sections of length 1, 2, 3, ... can be sold for 1, 5, 8, ... is presented above.

### Memoization

```swift
func cutrod(price[P] -> int, memo[M] -> int, len -> int) -> int
{
    var i = 0;
    var max_p = -1;
    
    if (memo[len] != -1)
    {
        max_p = memo[len]
    }
    else
    {
         while (i < len)
         {
             max_p = max(max_p, price[i] + cutrod(price, memo, len - i - 1));
             i = i + 1
         }
    };
    
    memo[len] = max_p
}
```

```swift
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] -> int;
    let memo = [ 0, -1, -1, -1, -1, -1, -1, -1, -1 ] -> int; 
    
    cutrod(price, memo, 8)
}
```
As mentioned in introduction dynamic programming uses memorization to speed up
calculations. Solutions to smaller problems are stored in array ```memo```
when function ```cutrod``` is invoked for given rod length and profit for
selling such rod is known then it is returned immediately. The above code snippet
introduces this optimization.

## Summary

This article presents short introduction to dynamic programming. There
are many other classic problems which are solved using this technique such
as coin change or frog jump.


