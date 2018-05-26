# Never - functional programming language

Never is a simple functional progamming laguage. Technically it may be classified as syntactically scoped, 
strongly typed, call by value, pure functional programming language.

In practise it offers first order functions and some mathematical functions to make it useful for some small purposes.

## Introduction
```
func main() -> float
{
    return 100 * 1.8 + 32;
}
```

A program written in Never language stars in function ```main```. Main function always takes no parameters
and returns float value. The function may only return value of one expression. In the above example temperature
of boiling water given in Celsius degrees is converted to Fahrenheit degrees.

```
func cel2fah(float c) -> float
{
    return c * 1.8 + 32;
}

func main() -> float
{
    return cel2fah(100);
}
```

In practse, however, one will define a function which will convert any degree value. The above listing 
presents such a function.

Functions may invoke themselves. The Fibonacci function is a classic example:

```
func fib(float n) -> float
{
    return (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2);
}

func main() -> float
{
    return fib(7);
}
```

Result of a function is calculated recursively. Also the above listing demonstates conditional expression.
Conditional expression takes the form of ```condition ? value if condition is true : value is condition is false```

## First Order Functions
One of the most fundamental features of functional programming langauges is their ability to accept
and return functions. Lets demonstrate this feature by the following code.

```
func fah2cel(float f) -> float
{
    return (f - 32) / 1.8;
}

func cel2fah(float c) -> float
{
    return c * 1.8 + 32;
}

func dir_deg(float d) -> (float) -> float
{
    return d == 0 ? fah2cel : cel2fah;
}

func main() -> float
{
    return dir_deg(1)(100);
}
```

Very interesting is function dirDeg. The function either returns function which converts from Celsius degrees to Fahrenheit or from Fahrenheit to Celsius degrees. As Never is strongly typed the function specifies as its return type ```(float) -> float``` which is the type of degree converting functions.

Never supports any degree of function nesting. So it is not needed to define all functions in top level. The above code may be rewritten into the following:

```
func dir_deg(float d) -> (float) -> float
{
    func fah2cel(float f) -> float
    {
        return (f - 32) / 1.8;
    }

    func cel2fah(float c) -> float
    {
        return c * 1.8 + 32;
    }

    return d == 0 ? fah2cel : cel2fah;
}

func main() -> float
{
    return dir_deg(0)(100);
}
```

Now, however, functions ```fah2cel``` and ```cel2fah``` cannot be invoked from function ```main``` as their syntax 
level has been changed.

Functions may also take other functions as arguments.

```
func fah2cel(float f) -> float
{
    return (f - 32) / 1.8;
}

func cel2fah(float c) -> float
{
    return c * 1.8 + 32;
}

func degrees(conv(float) -> float, float degree) -> float
{
    return conv(degree);
}

func main() -> float
{
    return degrees(cel2fah, 100);
}
```

In the above example function ```degrees``` takes conversion function which then is given passed paramater. Next function
value is returned. Also its parameter ```conv``` is strongly typed with function type.

## Functions as expressions
In fact functions in functional programming languages are also expressions. This leads to very interesting syntax.

```
func degrees(conv(float) -> float, float degree) -> float
{
    return conv(degree);
}

func main() -> float
{
    return degrees(func rea2cel(float d) -> float
                   {
                        return d * 4 / 5;
                   }, 100);
}

```

The above listing demonstates how a function ```rea2cel``` may be defined as a parameter being passed to function ```degrees```. The function converts from Réaumur degrees to Celsius degrees but this is not that very interesting.

The idea of inlining functions may be take into extreme.

```
func dir_deg(float d) -> (float) -> float
{
    return d == 0 ? func fah2cel(float f) -> float { return (f - 32) / 1.8; }
                  : func cel2fah(float c) -> float { return c * 1.8 + 32; };
}

func main() -> float
{
    return dir_deg(0)(100);
}
```

and a little step further

```
func dir_deg(float d, float degree) -> float
{
    return (d == 0 ? func fah2cel(float f) -> float { return (f - 32) / 1.8; }
                   : func cel2fah(float c) -> float { return c * 1.8 + 32; })(degree);
}

func main() -> float
{
    return dir_deg(0, 100);
}

```

## Mathematical functions
To make Never useful for any purpose it supports a few built in matematical functions -
sin, cos, tan, exp, log, sqrt and pow. These functions are also first class, so you may pass them in between functions
as any other.




