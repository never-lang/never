# Never - Functional Programming Language
[![Build Status](https://travis-ci.org/never-lang/never.svg?branch=master)](https://travis-ci.org/never-lang/never)
[![Version](https://img.shields.io/github/release/never-lang/never.svg)](https://github.com/never-lang/never/releases)
[![Codecov](https://codecov.io/gh/never-lang/never/branch/master/graph/badge.svg)](https://codecov.io/gh/never-lang/never)

Never is a simple functional programming language. Technically it may be
classified as syntactically scoped, strongly typed, call by value, functional programming language.

In practise Never offers basic data types, assignment, control flow, arrays,
first order functions and some mathematical functions to make it useful
to calculate expressions. Also it demonstrates how functions can be compiled,
invoked and passed as parameters or results between other functions.

## Introduction
```swift
func main() -> float
{
    100.0 * 1.8 + 32.0
}
```
A program written in Never language starts in function ```main```. ```Main```
function takes no parameters and returns ```int``` or ```float``` value. When embedded
in Unix shell or C language ```main``` can take ```int``` or ```float``` parameters.
The function may only return value of one expression. In the above example temperature
of boiling water given in Celsius degrees is converted to Fahrenheit degrees.

```swift
func cel2fah(c -> float) -> float
{
    c * 1.8 + 32.0
}

func main() -> float
{
    cel2fah(100.0)
}
```

In practice, however, one will define a function which will convert any degree.
The above listing presents such a function.

In particular, functions may invoke themselves. The Fibonacci function is
a classic example:

```swift
func fib(n -> int) -> int
{
    (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2)
}

func main() -> int
{
    fib(7)
}
```

or greatest common divisor:

```swift
func gcd(x -> int, y -> int) -> int
{
    (y == 0) ? x : gcd(y, x % y)
}

func main() -> int
{
    gcd(56, 12)
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

```swift
func fah2cel(f -> float) -> float
{
    (f - 32.0) / 1.8
}

func cel2fah(c -> float) -> float
{
    c * 1.8 + 32.0
}

func dir_deg(d -> int) -> (float) -> float
{
    d == 0 ? fah2cel : cel2fah
}

func main() -> float
{
    dir_deg(1)(100.0)
}
```

Very interesting is function ```dir_deg```. The function either returns function
which converts from Celsius degrees to Fahrenheit or from Fahrenheit to Celsius
degrees. As Never is strongly typed the function specifies its return type as
```(float) -> float``` which is the type of degree converting functions.

Functions may also take other functions as arguments.
```swift
func fah2cel(f -> float) -> float
{
    (f - 32.0) / 1.8
}

func cel2fah(c -> float) -> float
{
    c * 1.8 + 32.0
}

func degrees(conv(float) -> float, degree -> float) -> float
{
    conv(degree)
}

func main() -> float
{
    degrees(cel2fah, 100.0)
}
```

In the above example function ```degrees``` takes conversion function which
then is given passed parameter. In the next step function value is returned.
Also its parameter ```conv``` is strongly typed with function type.

## Syntax Level
Never supports any degree of function nesting. As result it is not needed to
define all functions in programs top level.

```swift
func dir_deg(d -> int) -> (float) -> float
{
    func fah2cel(f -> float) -> float
    {
        (f - 32) / 1.8
    }
    func cel2fah(c -> float) -> float
    {
        c * 1.8 + 32
    }

    d == 0 ? fah2cel : cel2fah
}

func main() -> float
{
    dir_deg(0)(100.0)
}
```

Functions ```fah2cel``` and ```cel2fah``` nested inside ```dir_deg``` are
defined within syntactical level of function ```dir_deg```. That means that
they cannot be invoked from function ```main```. Only functions and parameters
which are defined above or at the same level in the structure of a program
can be used.

```swift
func dir_deg(d -> float, coeff -> float) -> (float) -> float
{
    func fah2cel(f -> float) -> float
    {
        coeff * ((f - 32.0) / 1.8)
    }
    func cel2fah(c -> float) -> float
    {
        coeff * (c * 1.8 + 32.0)
    }

    d == 0 ? cel2fah : fah2cel
}

func main() -> float
{
    dir_deg(0, 100.0)(100.0)
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

```swift
func degrees(conv(float) -> float, degree -> float) -> float
{
    conv(degree)
}

func main() -> float
{
    degrees(let func rea2cel(d -> float) -> float
            {
                 d * 4.0 / 5.0
            }, 100.0)
}
```

The above listing outlines how a function ```rea2cel``` may be defined as
a parameter being passed to function ```degrees```. The function converts from
Réaumur degrees to Celsius degrees.

The idea of in-lining functions may be taken into extreme...
```swift
func calc() -> (float) -> float
{
    func fah2cel(float f) -> float { (f - 32.0) / 1.8 }
}

func main() -> float
{
    calc()(212.0)
}
```
... and a little step further.

```swift
func dir_deg(d -> int) -> (float) -> float
{
    d == 0 ? let func fah2cel(f -> float) -> float { (f - 32.0) / 1.8 }
           : let func cel2fah(c -> float) -> float { c * 1.8 + 32.0 }
}

func main() -> float
{
    dir_deg(0)(100.0)
}

```

## Bindings
Functions let to define bindings with local values.

```swift
func area(a -> float, b -> float, c -> float) -> float
{
    let p = (a + b + c) / 2.0;
    sqrt(p * (p - a) * (p - b) * (p - c))
}
```

In comparison to function, though, they cannot be mutually recursive. Thus
their values can be declared and used in their order. In the following example
variables ```q``` and ```p``` are declared in correct order. When reversed
compilation error will be displayed.

```swift
func outer(a -> float, b -> float) -> float
{
    let q = 10.0;
    let p = a + q;

    p + q
}
```

Bindings can hold any expressions. Thus the following code is also possible...

```swift
func outer(to -> int) -> () -> int
{
    let p = 2 * to;
    let f = let func rec() -> int
    {
        p
    };

    f
}
```

... or even

```swift
func outer(to -> int) -> (int) -> int
{
    let f = let func rec(start -> int) -> int
    {
        start < to ? rec(print(start) + 1) : 0
    };

    f
}

func main() -> int
{
    outer(10)(0)
}
```

## Assignments and Flow Control

Writing code using just recursion if very difficult. Never lets to use control
flow expressions known from other languages. These are ```if```, ```if else```,
```while```, ```do while``` and ```for``` expressions. As these structures
are expressions they also return a value. All of them, except for ```if else```
return ```0 -> int``` value. Also expression following ```if``` must return
```int``` value.

Assignment expression ```=``` lets to assign value of an expression on the
right hand side to a value on the left hand side. Please note, that if
the value on the left hand side is a temporary, assignment will be discarded.

The following examples present assignments and flow control.

```swift
func main() -> int
{
    let n = 18;

    do
    {
        print(n % 2);
        n = n / 2
    } while (n != 0)
}
```

The above example converts value ```18``` into binary format.

The following code calculates divisors of a number and outlines ```for``` and ```if```
expressions. The following factorizes a number using ```for```
and ```while``` expressions.

```swift
func divisors(n -> int) -> int
{
    var i = 1;

    for (i = 1; i * i <= n; i = i + 1) 
    {
        if (n % i == 0)
        {
            if (n / i != i)
            {
                print(n / i);
                print(i)
            }
            else
            {
                print(i)
            }
        }
    }

}

func main() -> int
{
    divisors(60)
}
```

```swift
func factorize(n -> int) -> int
{
    var i = 1;

    for (i = 2; i <= n; i = i + 1) 
    {
        while (n % i == 0)
        {
            print(i);
            n = n / i
        }
    }

}

func main() -> int
{
    factorize(2020)
}
```

## Records

```swift
record Tree
{
    value -> int;
    left -> Tree;
    right -> Tree;
    print(t -> Tree) -> int;
}

func print_tree(t -> Tree) -> int
{
    print(t.value);
    0
}

func main() -> int
{
    var t0 = new Tree;

    t0.value = 100;
    t0.print = print_tree;
    t0.print(t0);

    0
}
```

## Arrays
Never supports arrays of any dimension. Array are also expressions and may be
passed between functions. The following example declares an array and returns
value of its element.

```swift
func f1(a -> int) -> [D, D] -> int
{
     [ [ a, 0, 0, 0 ],
       [ 0, a, 0, 0 ],
       [ 0, 0, a, 0 ],
       [ 0, 0, 0, a ] ] -> int
}

func main() -> int
{
    f1(11)[0, 0]
}
```

Arrays may contain elements of any type. In particular these may be other
arrays...

```swift
func call(tab[row] -> [D] -> int) -> int
{
    tab[row - 1][1]
}

func f1() -> int
{
    call([ [ 9, 8, 7, 6, 5 ] -> int,
           [ 9, 7, 5 ] -> int        ] -> [_] -> int)
}

func main() -> int
{
    f1()
}
```
...or even functions.
```swift
func f1(a -> int, b -> int, c -> int) -> [D] -> () -> int
{
     [
        let func f1() -> int { a + b + c },
        let func f2() -> int { a + b - c }  
     ] -> () -> int
}

func main() -> int
{
    f1(80, 90, 100)[1]()
}

```

When arrays are passed to functions their dimensions are also passed as function
arguments. This type of array passing type is called conformant arrays.
```swift
func f1(tab[row, col] -> int) -> int
{
    row * col
}

func main() -> int
{
    f1( [ [10, 20, 30], [30, 40, 50] ] -> int )
}
```

Conformat arrays let to iterate over array elements. The following listing
demonstrates how conformant arrays and tail recursion are used to determine
lowest element in an array.
```swift
func tmin( t[elems] -> int ) -> int
{
    func __tmin( min -> int, i -> int, t[elems] -> int ) -> int
    {
        i < elems ? __tmin( t[i] < min ? t[i] : min, i + 1, t ) : min
    }
    __tmin(t[0], 0, t)
}

func main() -> int
{
    tmin( [ 20, 10, 30, 50, 40 ] -> int )
}
```

The following example presents how to pass any function which is executed over
all elements of an array. This program uses arrays, first class functions
and tail recursion.
```swift
func add_five(e -> int) -> int
{
    print(e + 5)
}

func tforeach( t[elems] -> int, each(e -> int) -> int) -> int
{
    func __tforeach( val -> int, i -> int, t[elems] -> int ) -> int
    {
        i < elems ? __tforeach( each(t[i]), i + 1, t ) : 0
    }
    __tforeach(t[0], 0, t)
}

func main() -> int
{
    tforeach( [ 10, 20, 50, 30, 40 ] -> int, add_five )
}
```

Arrays may contain other arrays. This feature lets us to define vectors
of arrays.
```swift
func printTab( tab[dim] -> int ) -> int
{
    func __printTab( val -> int, i -> int, tab[dim] -> int ) -> int
    {
        i < dim ? __printTab( print(2 * tab[i]), i + 1, tab) : i
    }
    __printTab(0, 0, tab)
}

func print2Tab( tab[dim] -> [D] -> int ) -> int
{
    func __print2Tab( val -> int, i -> int, tab[dim] -> [D] -> int ) -> int
    {
        i < dim ? __print2Tab( printTab(tab[i]), i + 1, tab ) : i
    }
    __print2Tab(0, 0, tab)
}

func main() -> int
{
    print2Tab( [ [ 1, 2, 3, 4, 5, 6 ] -> int,
                 [ 16, 17, 18 ] -> int ] -> [D] -> int )
}
```

The above code can be rewritten using ```foreach``` functions.
```swift
func twice(e -> int) -> int
{
    print(2 * e)
}

func foreachTab( tab[dim] -> int, each(e -> int) -> int ) -> int
{
    func __foreachTab( val -> int, i -> int, tab[dim] -> int ) -> int
    {
        i < dim ? __foreachTab( each(tab[i]), i + 1, tab) : i
    }
    __foreachTab(0, 0, tab)
}

func foreach2Tab( tab[dim] -> [D] -> int, eachTab(t[D] -> int, (int) -> int) -> int, each(e -> int) -> int ) -> int
{
    func __foreach2Tab( val -> int, i -> int, tab[dim] -> [D] -> int ) -> int
    {
        i < dim ? __foreach2Tab( eachTab(tab[i], each), i + 1, tab ) : i
    }
    __foreach2Tab(0, 0, tab)
}

func main() -> int
{
    foreach2Tab( [ [ 1, 2, 3, 4, 5, 6 ] -> int,
                   [ 16, 17, 18 ] -> int ] -> [D] -> int,
                   foreachTab, twice )
}
```

Arrays can be used to memorize sub-problem results in dynamic programming. 
The following example solves rod cutting dynamic problem.

```swift
func max(a -> int, b -> int) -> int { a > b ? a : b }

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

func main() -> int
{
    let price = [ 2, 7, 9, 10, 10, 14, 17, 21 ] -> int;
    let memo = [ 0, -1, -1, -1, -1, -1, -1, -1, -1 ] -> int; 
    
    cutrod(price, memo, 8)
}
```

## Array Operators
Never lets to add, subtract and multiply int and float arrays.

```swift
func main() -> int
{
    printtab( 2 * [ 3, 5, 7, 9 ] -> int )
}
```

```swift
func main() -> int
{
    printtab( - [ 1, -2, 3, -4, 5, -6 ] -> int )
}
```

```swift
func main() -> int
{
    printtab( [ 3.5, 5.5, 7.5 ] -> float - [ 3.0, 4.0, 7.0 ] -> float )
}
```

```swift
func main() -> int
{
    printtab( [ 1.5, 2.5, 3.5 ] -> float + [ 3.0, 4.0, 7.0 ] -> float )
}
```

```swift
func main() -> int
{
    printtab( [ [ 1.0, 2.0, 3.0 ],
                [ 3.0, 4.0, 5.0 ] ] -> float
                        *
              [ [ 3.0, 4.0, 1.0, 1.0 ],
                [ 6.0, 7.0, 1.0, 1.0 ],
                [ 8.0, 2.0, 1.0, 1.0 ] ] -> float )
}
```

## List Comprehension

Never supports list comprehension. Each list consists of a series of
generators and filers and expression which yields list elements.

```swift
func cl() -> [_] -> int
{
    [ x * x | x in [10, 20, 30, 40, 50] -> int ] -> int
}
```

The following example presents both generators and filters.

```swift
func cl() -> [_] -> int
{
    [ x * x | x in [1, 2, 3, 4, 5, 6, 7, 8] -> int; (x * x % 2) == 0 ] -> int
}
```

List comprehension may also invoke other functions.

```swift
func cl() -> [_] -> float
{
    func grad(d -> float) -> float
    {
        d * 2.0 * 3.14159265 / 360.0
    }
    
    [ f(y) | f in [ sin, cos ] -> (float) -> float;
             y in [ grad(0.0), grad(30.0), grad(45.0), grad(60.0), grad(90.0) ] -> float ] -> float
}
```

or even return list of closures.

```swift
func cl() -> [_] -> (float) -> float
{
    func grad(d -> float) -> float
    {
        d * 2.0 * 3.14159265 / 360.0
    }
    
    [ g | f in [ sin, cos ] -> (float) -> float;
          g in [ let func(x -> float) -> float { f(grad(x)) } ] -> (float) -> float ] -> (float) -> float
}
```

## Mathematical Functions
Never supports a few built-in mathematical functions - ```sin(x)```,
```cos(x)```, ```tan(x)```, ```exp(x)```, ```log(x)```, ```sqrt(x)```
and ```pow(x,y)```. These functions are also first class so they may be passed
in between functions as any other function.

```swift
func deg2rad(deg -> float) -> float
{
    deg * 3.14159 / 180
}

func get_func() -> (float) -> float
{
    cos
}

func main() -> float
{
    get_func()(deg2rad(60.0))
}
```

Together with arrays mathematical functions can be used to express and calculate
vector rotations. Code snippet included below rotates vector ```[[ 10.0, 0.0 ]]```
by 0, 45, 90, 180, 270 and 360 degrees.

```swift
func print_vect(vect[D1, D2] -> float) -> int
{
    printf(vect[0, 0]);
    printf(vect[0, 1]);
    0
}

func rotate_matrix(alpha -> float) -> [_,_] -> float
{
    [ [ cos(alpha), -sin(alpha) ],
      [ sin(alpha), cos(alpha)  ] ] -> float
}

func main() -> int
{
    let vect = [[ 10.0, 0.0 ]] -> float;

    print_vect(vect * rotate_matrix(0.0));
    print_vect(vect * rotate_matrix(3.14159 / 4.0));
    print_vect(vect * rotate_matrix(3.14159 / 2.0));
    print_vect(vect * rotate_matrix(3.14159));
    print_vect(vect * rotate_matrix(3.0 * 3.14159 / 2.0));
    print_vect(vect * rotate_matrix(2.0 * 3.14159))
}
```

## Exceptions
During program execution some operations may fail. One well known example of them
is division by zero. Another one is dereferencing array out of its bounds.
A well written program should handle such situations and respond in another way.

Never can handle internal errors using exceptions handlers specified after
every function. Such handlers can execute arbitrary code. If an exception
happens inside exception handler it replaces exception being processed.

The following code shows how exception ```invalid_domain``` raised when negative
parameter passed to ```sqrt``` function is passed.

```swift
func main() -> int
{
    sqrt(-1)
}
catch (invalid_domain)
{
    -1
}
```

Exception need not be processed in the same function where they occurred. They
are passed down call stack. First function which defines exception handler
is used. Also any exception can be caught by parameterless exception handler.

```swift
func three(d -> int, c -> int) -> int
{
    let t = [ 1, 2, 3 ] -> int;

    t[0] = d;
    170 / d
}

func two(d -> int) -> int
{
    three(d, 199)
}
catch (wrong_array_size)
{
    0
}
catch (index_out_of_bounds)
{
    d + 102
}

func one(d -> int) -> int
{
    two(d)
}

func main() -> int
{
    one(0)
}
catch (division_by_zero)
{
    155
}
```

In the above example exception division by zero is caught by handler defined
in function ```main```. If index out of bound was raised it would be caught
by exception handler defined in function ```two```. Please also note that
exception handlers can access function parameters. All bindings and nested
functions are not accessible.

## Console Output
Never implements simple `print(int x) -> int` and `printf(float x) -> float` functions.
The function writes an integer or float parameter `x` (with a new line character)
to standard output and returns passed value.  By default `printf` uses `"%.2f\n"` formatting.

```swift
func main() -> float
{
    printf(123.456)
}
```

It is also possible to print string of characters.

```swift
func main() -> int
{
    let txt = "answer is ";
    let value = 200;

    prints(txt + str(value) + "\n");
    
    0
}
```

They may be concatenated with integers or floats.

```swift
func print_vect(vect[D1, D2] -> float) -> int
{
    prints("[" + vect[0, 0] + "," + vect[0, 1] + "]\n");
    0
}
```

String can also be assigned and compared.

```swift
func main() -> int
{
    let s1 = "string one\n";
    let s2 = "text two\n";

    prints(s1);
    prints(s2);
    
    s2 = s1;
    
    prints(s2);

    0
}
```

```swift
func main() -> int
{
    let s1 = "text equal";
    let s2 = "text equal";

    assert(if (s1 == s2) { 1 } else { 0 } == 1)
}
```

## Embedded Never
Never language can be embedded in Unix shell and C code.

### Shell
```swift
#!/usr/bin/nev

func add(a -> int, b -> int, c -> int) -> int
{
    a + b + c
}

func main(a -> int, b -> int) -> int
{
    add(a, b, 1)
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
    const char * prog_str = "func main(a -> int, b -> int) -> int { 10 * (a + b) }";

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

## More Information

You can find more information about Never at the following pages:

* [Tail Recursion](https://never-lang.github.io/never/tailrec.html)
* [Neural Network in Never](https://never-lang.github.io/never/perceptron.html)

## Contact
If you find it useful for any purpose I will be glad to hear from you.

If you need help to develop a programming or domain specific language please contact me.

