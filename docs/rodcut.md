---
title: Rod Cutting
---

# Dynamic Programming - Rod Cutting

## Introduction

## Problem

## Solution

### Recursive Algorithm


```swing
func max(a -> int, b -> int) -> int
{
    a > b ? a : b
}
```

```swing
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

```swing
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] -> int;
    
    cutrod(price, 8)
}
```

### Memoization

```swing
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

```swing
func main() -> int
{
    let price = [ 1, 5, 8, 9, 10, 17, 17, 20 ] -> int;
    let memo = [ 0, -1, -1, -1, -1, -1, -1, -1, -1 ] -> int; 
    
    cutrod(price, memo, 8)
}
```

## Summary




