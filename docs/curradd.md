---
title: Tribute to Alonso C.
---

# Tribute to Alonso C.

In mathematics functions are relations between two sets (denoted as f: X -> Y).
Quite often functions take several arguments and give one result (for example
add: R x R -> R). Interestingly it is possible to define only functions
which take only one argument and still be possible to express any function.
The process of transforming a function with arbitrary number of arguments
to a function which takes only one argument is called currying.

## Function Currying

```never
func add(x : int, y : int) -> int
{
    x + y
}
```

The above code snippet presents function ```add``` which returns sum of its
arguments.

```never
func cadd(x : int) -> (int) -> int
{
    let func (y : int) -> int
    {
        x + y
    }
}
```

To curry ```add``` function we define function ```cadd``` which takes first 
addend and returns anonymous function (often called lambda) which takes second
addend (cadd: R -> (R -> R)). Then such function can be used to return sum.
Curried ```add``` is presented in Never programming language in the above listing.

```never
func main() -> int
{
    print(add(5, 7));
    print(add(2, add(3, 7)));

    print(cadd(5)(7));
    print(cadd(2)(cadd(3)(7)));

    0
}
```

Lets see how ```add``` and ```cadd``` functions are used to add two numbers.
As expected ```add``` takes two arguments. Interesting is usage of ```cadd```
function which returns anonymous function which is invoked with second argument.

# Summary

I hope you liked this short introduction to currying. It is often used
in lambda calculus discovered by Alonso Church.


