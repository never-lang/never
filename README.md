# Never - Functional Programming Language

Never is a simple functional programming language. Technically it may be 
classified as syntactically scoped, strongly typed, call by value, pure 
functional programming language.

In practise Never offers first order functions and some mathematical functions
to make it useful to calculate expressions. Also it demonstrates how
functions can be compiled, invoked and passed as parameters or results between
other functions.

## Introduction
```
func main() -> float
{
    return 100 * 1.8 + 32;
}
```
A program written in Never language starts in function ```main```. ```Main```
function always takes no parameters and returns float value. The function may
only return value of one expression. In the above example temperature
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

In practice, however, one will define a function which will convert any degree.
The above listing presents such a function.

In particular, Functions may invoke themselves. The Fibonacci function is
a classic example:

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

Result of a function is calculated recursively. The above listing also
demonstrates conditional expression. Conditional expression takes the form 
of ```condition ? expr true : expr false```. That is when condition is true,
value after ```?``` is returned. When the condition is false, value after
```:``` is returned.

## First Order Functions
One of most interesting features of functional programming languages is their
ability to accept and return functions. The following code demonstrates this
feature.

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

Very interesting is function ```dir_deg```. The function either returns function
which converts from Celsius degrees to Fahrenheit or from Fahrenheit to Celsius
degrees. As Never is strongly typed the function specifies its return type as
```(float) -> float``` which is the type of degree converting functions.

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

In the above example function ```degrees``` takes conversion function which 
then is given passed parameter. Next function value is returned. Also its
parameter ```conv``` is strongly typed with function type.

## Syntax level
Never supports any degree of function nesting. As result it is not needed to
define all functions in programs top level.

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

Functions ```fah2cel``` and ```cel2fah``` nested inside ```dir_deg``` are
defined within syntactical level of function ```dir_deg```. That means that
they cannot be invoked from function ```main```. Only functions and parameters
which are defined above or at the same level in the structure of a program
can be used.

```
func dir_deg(float d, float coeff) -> (float) -> float
{
    func fah2cel(float f) -> float
    {
        return coeff * ((f - 32) / 1.8);
    }
    
    func cel2fah(float c) -> float
    {
        return coeff * (c * 1.8 + 32);
    }

    return d == 0 ? cel2fah : fah2cel;
}

func main() -> float
{
    return dir_deg(0, 100)(100);
}
```

The above listing demonstrates how parameter ```coeff``` is accessed from
within functions ```fah2cel``` or ```cel2fah```. After ```dir_def```
is called in ```main``` parameter ```coeff`` is bound to ```dir_deg```
environment. This way ```coeff``` can be used in functions which convert
temperature after ```dir_deg``` returns.

## Functions as expressions
Functions in functional programming languages are also expressions.
This leads to very interesting syntax which is supported by Never.

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

The above listing outlines how a function ```rea2cel``` may be defined as
a parameter being passed to function ```degrees```. The function converts from
Réaumur degrees to Celsius degrees.

The idea of in-lining functions may be taken into extreme...
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
... and a little step further.

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
Never supports a few built in mathematical functions - ```sin(x)```,
```cos(x)```, ```tan(x)```, ```exp(x)```, ```log(x)```, ```sqrt(x)```
and ```pow(x,y)```. These functions are also first class so they may be passed
in between functions as any other function.

```
func deg2rad(float deg) -> float
{
    return deg * 3.14159265359 / 180;
}

func get_func() -> (float) -> float
{
    return cos;
}

func main() -> float
{
    return get_func()(deg2rad(60));
}
```

## Contact
If you find it useful for any purpose I will be glad to hear from you.


