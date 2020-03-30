---
title: Rod Cutting
---

# Dynamic Programming - Rod Cutting

## Introduction

Dynamic programming is well known algorithm design method. 
It is used to solve problems where problem of size N is solved using
solution of problems of size N - 1 (or smaller). Introductory example is
calculation of Fibonacci numbers where F(N) (problem of size N) is calculated
as sum of F(N - 2) and F(N - 1) (problems of size N - 2 and N - 1).
Usually smaller problems are calculated many times. To avoid repeatable
calculations their result is memorized in an array. This technique
(known as memoization) significantly speeds up calculations. 
In the next sections I will present classic problem known as rod cutting
which is solved using dynamic programming.

## Problem

Rod cutting problem is formulated as maximum profit that
can be obtained by cutting a rod into parts. The question is how to cut 
the rod so that profit is maximized.

## Recursive Algorithm


```never
func max(a : int, b : int) -> int
{
    a > b ? a : b
}
```

As rod cutting problem mentions maximum profit, likely maximum function
is needed. The above code snippet presents such function.

```never
func cutrod(price[_] : int, len : int) -> int
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
be calculated as profit obtained by cutting rod of length ```i``` plus profit earned by
cutting rod of ```length - i``` (in the code above ```1``` is subtracted as
rod of length ```i``` is at index ```i - 1``` in the array ```price```). Please note
that problem of size ```len``` is calculated using solution to problem
of size ```len - i - 1```. This sentence can be formulated by simple
expression ```max_p = max(max_p, price[i] + cutrod(price, len - i - 1))```.

```never
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] : int;
    
    cutrod(price, 8)
}
```

An example of maximizing profit obtained by cutting a rod of length 8 where
sections of lengths 1, 2, 3, ... can be sold for 1, 5, 8, ... is presented above.

### Memoization

```never
func cutrod(price[P] : int, memo[M] : int, len : int) -> int
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

```never
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] : int;
    let memo = [ 0, -1, -1, -1, -1, -1, -1, -1, -1 ] : int; 
    
    cutrod(price, memo, 8)
}
```
As mentioned in the introduction dynamic programming uses memoization to speed up
calculations. Solutions to smaller problems are stored in array ```memo```.
When function ```cutrod``` is invoked for given rod length and profit of
selling such rod is known then it is returned immediately. The above code snippet
introduces this optimization.

## Summary

This article presents short introduction to dynamic programming. There
are many other classic problems which can be solved using this method.
Coin change, matrix multiplication or longest common subsequence are examples
of most well known problems. I encourage you to study them.


