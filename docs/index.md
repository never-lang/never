---
---

# Never - Functional Programming Language
[![Build Status](https://travis-ci.org/never-lang/never.svg?branch=master)](https://travis-ci.org/never-lang/never)

Never is a simple functional programming language. Technically it may be
classified as syntactically scoped, strongly typed, call by value, pure
functional programming language.

In practise Never offers basic data types, arrays, first order functions and some 
mathematical functions to make it useful to calculate expressions. Also it
demonstrates how functions can be compiled, invoked and passed as parameters
or results between other functions.

## Introduction
```
func main() -> float
{
    return 100.0 * 1.8 + 32.0;
}
```
A program written in Never language starts in function ```main```. ```Main```
function takes no parameters and returns ```int``` or ```float``` value. When embedded
in Unix shell or C language ```main``` can take ```int``` or ```float``` parameters.
The function may only return value of one expression. In the above example temperature
of boiling water given in Celsius degrees is converted to Fahrenheit degrees.

```
func cel2fah(c -> float) -> float
{
    return c * 1.8 + 32.0;
}

func main() -> float
{
    return cel2fah(100.0);
}
```

In practice, however, one will define a function which will convert any degree.
The above listing presents such a function.

In particular, Functions may invoke themselves. The Fibonacci function is
a classic example:

```
func fib(n -> int) -> int
{
    return (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2);
}

func main() -> int
{
    return fib(7);
}
```

or greatest common divisor:

```
func gcd(x -> int, y -> int) -> int
{
    return (y == 0) ? x : gcd(y, x % y);
}

func main() -> int
{
    return gcd(56, 12);
}
```

Result of a function is calculated recursively. The above listing also
demonstrates conditional expression. Conditional expression takes the form
of ```condition ? expr true : expr false```. That is when condition is true,
value after ```?``` is returned. When the condition is false, value after
```:``` is returned.

When last function called is recursive function we call it tail recursion. It
lets to substitute function invocations with repetitive calls and improve
program execution. In the above examples ```gcd``` function is recursive.
Fibonacci function ```fib``` may seem tail recursive, however the last function
called is addition, thus it is not considered tail recursive.

## First Class Functions
One of most interesting features of functional programming languages is their
ability to accept and return functions. The following code demonstrates this
feature.

```
func fah2cel(f -> float) -> float
{
    return (f - 32.0) / 1.8;
}

func cel2fah(c -> float) -> float
{
    return c * 1.8 + 32.0;
}

func dir_deg(d -> int) -> (float) -> float
{
    return d == 0 ? fah2cel : cel2fah;
}

func main() -> float
{
    return dir_deg(1)(100.0);
}
```

Very interesting is function ```dir_deg```. The function either returns function
which converts from Celsius degrees to Fahrenheit or from Fahrenheit to Celsius
degrees. As Never is strongly typed the function specifies its return type as
```(float) -> float``` which is the type of degree converting functions.

Functions may also take other functions as arguments.
```
func fah2cel(f -> float) -> float
{
    return (f - 32.0) / 1.8;
}

func cel2fah(c -> float) -> float
{
    return c * 1.8 + 32.0;
}

func degrees(conv(float) -> float, degree -> float) -> float
{
    return conv(degree);
}

func main() -> float
{
    return degrees(cel2fah, 100.0);
}
```

In the above example function ```degrees``` takes conversion function which
then is given passed parameter. In the next step function value is returned.
Also its parameter ```conv``` is strongly typed with function type.

## Syntax Level
Never supports any degree of function nesting. As result it is not needed to
define all functions in programs top level.

```
func dir_deg(d -> int) -> (float) -> float
{
    func fah2cel(f -> float) -> float
    {
        return (f - 32) / 1.8;
    }

    func cel2fah(c -> float) -> float
    {
        return c * 1.8 + 32;
    }

    return d == 0 ? fah2cel : cel2fah;
}

func main() -> float
{
    return dir_deg(0)(100.0);
}
```

Functions ```fah2cel``` and ```cel2fah``` nested inside ```dir_deg``` are
defined within syntactical level of function ```dir_deg```. That means that
they cannot be invoked from function ```main```. Only functions and parameters
which are defined above or at the same level in the structure of a program
can be used.

```
func dir_deg(d -> float, coeff -> float) -> (float) -> float
{
    func fah2cel(f -> float) -> float
    {
        return coeff * ((f - 32.0) / 1.8);
    }

    func cel2fah(c -> float) -> float
    {
        return coeff * (c * 1.8 + 32.0);
    }

    return d == 0 ? cel2fah : fah2cel;
}

func main() -> float
{
    return dir_deg(0, 100.0)(100.0);
}
```

The above listing demonstrates how parameter ```coeff``` is accessed from
within functions ```fah2cel``` or ```cel2fah```. After ```dir_def```
is called in ```main``` parameter ```coeff``` is bound to ```dir_deg```
environment. This way ```coeff``` can be used in functions which convert
temperature after ```dir_deg``` returns.

## Functions as Expressions
Functions in functional programming languages are also expressions.
This leads to very interesting syntax which is supported by Never.

```
func degrees(conv(float) -> float, degree -> float) -> float
{
    return conv(degree);
}

func main() -> float
{
    return degrees(func rea2cel(d -> float) -> float
                   {
                        return d * 4.0 / 5.0;
                   }, 100.0);
}
```

The above listing outlines how a function ```rea2cel``` may be defined as
a parameter being passed to function ```degrees```. The function converts from
Réaumur degrees to Celsius degrees.

The idea of in-lining functions may be taken into extreme...
```
func calc() -> (float) -> float
{
    return func fah2cel(float f) -> float { return (f - 32.0) / 1.8; };
}

func main() -> float
{
    return calc()(212.0);
}
```
... and a little step further.

```
func dir_deg(d -> int) -> (float) -> float
{
    return d == 0 ? func fah2cel(f -> float) -> float { return (f - 32.0) / 1.8; }
                  : func cel2fah(c -> float) -> float { return c * 1.8 + 32.0; };
}

func main() -> float
{
    return dir_deg(0)(100.0);
}

```

## Arrays
Never supports arrays of any dimension. Array are also expressions and may be
passed between functions. The following example declares an array and returns
value of its element.

```
func f1(a -> int) -> [D, D] -> int
{
    return { { a, 0, 0, 0 },
             { 0, a, 0, 0 },
             { 0, 0, a, 0 },
             { 0, 0, 0, a } } -> int;
}

func main() -> int
{
    return f1(11)[0, 0];
}
```

Arrays may contain elements of any type. In particular these may be other
arrays...

```
func call(tab[row] -> [D] -> int) -> int
{
    return tab[row - 1][1];
}

func f1() -> int
{
    return call({ { 9, 8, 7, 6, 5 } -> int,
                  { 9, 7, 5 } -> int        } -> [_] -> int);
}

func main() -> int
{
    return f1();
}
```
...or even functions.
```
func f1(a -> int, b -> int, c -> int) -> [D] -> () -> int
{
    return {
             func f1() -> int { return a + b + c; },
             func f2() -> int { return a + b - c; }  
           } -> () -> int;
}

func main() -> int
{
    return f1(80, 90, 100)[1]();
}

```

When arrays are passed to functions their dimensions are also passed as function
arguments. This type of array passing type is called conformant arrays.
```
func f1(tab[row, col] -> int) -> int
{
    return row * col;
}

func main() -> int
{
    return f1( { {10, 20, 30}, {30, 40, 50} } -> int );
}
```

Conformat arrays let to iterate over array elements. The following listing
demonstrates how conformant arrays and tail recursion are used to determine
lowest element in an array.
```
func tmin( t[elems] -> int ) -> int
{
	func __tmin( min -> int, i -> int, t[elems] -> int ) -> int
	{
		return i < elems ? __tmin( t[i] < min ? t[i] : min, i + 1, t ) : min;
	}
	return __tmin(t[0], 0, t);
}

func main() -> int
{
	return tmin( { 20, 10, 30, 50, 40 } -> int );
}
```

The following example presents how to pass any function which is executed over
all elements of an array. This program uses arrays, first class functions
and tail recursion.
```
func add_five(e -> int) -> int
{
	return print(e + 5);
}

func tforeach( t[elems] -> int, each(e -> int) -> int) -> int
{
	func __tforeach( val -> int, i -> int, t[elems] -> int ) -> int
	{
		return i < elems ? __tforeach( each(t[i]), i + 1, t ) : 0;
	}
	return __tforeach(t[0], 0, t);
}

func main() -> int
{
	return tforeach( { 10, 20, 50, 30, 40 } -> int, add_five );
}
```

Arrays may contain other arrays. This feature lets us to define vectors
of arrays.
```
func printTab( tab[dim] -> int ) -> int
{
    func __printTab( val -> int, i -> int, tab[dim] -> int ) -> int
    {
        return i < dim ? __printTab( print(2 * tab[i]), i + 1, tab) : i;
    }
    return __printTab(0, 0, tab);
}

func print2Tab( tab[dim] -> [D] -> int ) -> int
{
    func __print2Tab( val -> int, i -> int, tab[dim] -> [D] -> int ) -> int
    {
        return i < dim ? __print2Tab( printTab(tab[i]), i + 1, tab ) : i;
    }
    return __print2Tab(0, 0, tab);
}

func main() -> int
{
    return print2Tab( { { 1, 2, 3, 4, 5, 6 } -> int,
                        { 16, 17, 18 } -> int } -> [D] -> int );
}
```

The above code can be rewritten using ```foreach``` functions.
```
func twice(e -> int) -> int
{
    return print(2 * e); 
}

func foreachTab( tab[dim] -> int, each(e -> int) -> int ) -> int
{
    func __foreachTab( val -> int, i -> int, tab[dim] -> int ) -> int
    {
        return i < dim ? __foreachTab( each(tab[i]), i + 1, tab) : i;
    }
    return __foreachTab(0, 0, tab);
}

func foreach2Tab( tab[dim] -> [D] -> int, eachTab(t[D] -> int, (int) -> int) -> int, each(e -> int) -> int ) -> int
{
    func __foreach2Tab( val -> int, i -> int, tab[dim] -> [D] -> int ) -> int
    {
        return i < dim ? __foreach2Tab( eachTab(tab[i], each), i + 1, tab ) : i;
    }
    return __foreach2Tab(0, 0, tab);
}

func main() -> int
{
    return foreach2Tab( { { 1, 2, 3, 4, 5, 6 } -> int,
                          { 16, 17, 18 } -> int } -> [D] -> int,
                        foreachTab, twice );
}
```

## Mathematical Functions
Never supports a few built-in mathematical functions - ```sin(x)```,
```cos(x)```, ```tan(x)```, ```exp(x)```, ```log(x)```, ```sqrt(x)```
and ```pow(x,y)```. These functions are also first class so they may be passed
in between functions as any other function.

```
func deg2rad(deg -> float) -> float
{
    return deg * 3.14159265359 / 180;
}

func get_func() -> (float) -> float
{
    return cos;
}

func main() -> float
{
    return get_func()(deg2rad(60.0));
}
```

## Write to stdout functions - print and printf
Never implements a simple `print(int x) -> int` and `printf(float x) -> float` function.
The function writes an integer or float parameter `x` (with a new line character)
to standard output and returns passed value.  By default `printf` uses `"%.2f\n"` formatting.

```
func main() -> float
{
	return printf(123.456);
}
```

## Embedded Never
Never language can be embedded in Unix shell and C code.

### Shell
```
#!/usr/bin/nev

func add(a -> int, b -> int, c -> int) -> int
{
    return a + b + c;
}

func main(a -> int, b -> int) -> int
{
    return add(a, b, 1);
}

```

After adding ```#!/usr/bin/nev``` to the script first line and setting script
as executable it is possible to run a program without specifying interpreter name.
Then a script is executed from command line with additional parameters.
```
$ ./sample81.nevs 10 20
result is 31
```


Also nev can be executed with ```-e``` parameter followed by program.

### C language
```C
#include <stdio.h>
#include <assert.h>
#include "nev.h"

void test_one()
{
    int ret = 0;
    object result = { 0 };
    program * prog = program_new();
    const char * prog_str = "func main(a -> int, b -> int) -> int { return 10 * (a + b); }";

    ret = nev_compile_str(prog_str, prog);
    if (ret == 0)
    {
        prog->params[0].int_value = 2;
        prog->params[1].int_value = 3;

        ret = nev_execute(prog, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT && result.int_value == 50);
        }
        
        prog->params[0].int_value = 9;
        prog->params[1].int_value = 1;

        ret = nev_execute(prog, &result);
        if (ret == 0)
        {
            assert(result.type == OBJECT_INT && result.int_value == 100);
        }
    }

    program_delete(prog);
}
```
The above code present how Never can be embedded into C code. First ```nev.h```
header is included. Then a new program ```prog``` is created and parsed with
```parse_str``` function. In the next step, parameters are set to values. Please
note that the program can be executed with different input parameters many times.
Return value is set in ```result``` object which then can be used. In this
example ```assert``` function assures that calculations are as expected.

## Contact
If you find it useful for any purpose I will be glad to hear from you.

If you need help to develop a programming or domain specific language please contact me.

