---
title: Tail Recursion
---

# Recursion

*To Understand Recursion, You Must First Understand Recursion.*

![Uroboros; Public Domain reproduced by Carlos Adanero][uroboros]

## Introduction
Many problems are expressed using themselves as subproblems. This leads to an interesting observation, that to solve a problem one needs to solve the same problem again. Of course such loop needs to be broken somewhere. Usually subproblems are of smaller size than the original. In mathematics a classical example of such a function is Fibonacci function which is defined as ```fib(1) = 1, fib(2) = 1, fib(n) == fib(n - 1) + fib(n - 2)```. Programmers also noticed that value of such functions can be automatically calculated by a computer. Unfortunately computers are electrical devices which have their limitations. One of them is speed of execution which is finite and thus larger problems take longer to execute. The other one is memory size which also influences size of problems which can be solved. In many functions the same subproblem is calculated repeatadely. As computers execute instructions they must be programmed to reuse previously calculated values. Otherwise they will blindly recalculate same problems again and again. For example, simple implementation of the Fibonacci function for each ```n``` would solve the same subproblems twice. In recursion of course! Which leads to exponential number of instructions invoked. Despite those limitations it is tempting to use recursion to express problems. They become easier to express and later understand. In many cases, when subproblem is divided by two, for example, in many practical problems this leads to very few levels of recursion which can be handled by physical computers. For problems with 1 million elements such division results only in 20 levels of recursion calls. Also it is easy to use memory and store results of already calculated functions. Such a technique, known as memoizations, significantly speeds to calculations. Another technique, which is described in this article, is known as tail recursion.

## Tail Recursion
The following code listing presents Fibonacci function expressed in [Never][never-lang] language. It is expressed almost in the same way as its mathematical definition. To calculate ```fib(20)``` function ```fib``` is called 21891 times. When executed on a typical laptop it takes approximately 0.6s to execute (unsing Never language).

```never
func fib(n : int) -> int 
{
    (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2)
}
```

Mathematicians and computer scientists began to think how this situation could be improved. Mathematicians noticed that when the recursion call is the last one results are evaluated differently. Function result is calculated as result of immediate subproblem. Thus, there are much fewer subproblems which are calculated. Computer scientists realized that the call can be optimized and avoided completely. Such recursive last calls turn into a loop which is executed again and again. In the above example the last function called is addition. How can it be turned into tail recursion?

First, it needs to be the last call. Second, it needs parameter ```n```. Also we may notice that result is a sum of two previous values, lets name them ```a``` and ```b```.

```never
func fib(n : int, a : int, b : int) -> int
{
    (n == 0) ? a : (n == 1) ? b : fib(n - 1, b, a + b)
}
```

The above listing presents tail recursive definition of the Fibonacci function. During each call its value is calculated by adding two previous values. What is most important there will be just 20 recursive calls. Now it takes only 0.004s to execute. 150 times faster and 1094 fewer function calls!

## Function Evaluation
Other functions can be turned into tail recursive as well.

```never
func gcd(x : int, y : int) -> int
{
    (y == 0) ? x : gcd(y, x % y)
}
```

The above function present greatest common divisor function.

```never
func factorial(n : int, val : int) -> int
{
    n == 0 ? val : factorial(n - 1, n * val)
}
```

```never
func power(a : int, n : int, val : int) -> int
{
    n == 0 ? val : power(a, n - 1, a * val)
}
```

Next two functions define factorial and power functions.

## Programming
Tail recursive functions can be used to create programs. When evaluating mathematical functions we were not interested how their value is calculated. However, when creating programs, we may use the fact that tail recursive function is executed in a loop. Now we are less interested in results as such. The program benefits from repeated calls made when recursive function is invoked.

To illustrate this technique lets have a look at the following examples. Different functions are executed over elements of an array. [Never][never-lang] language declares an array of four integers as ```{ 1, 2, 3, 4 } -> int``` and gets value of its elements using ```t[i]``` syntax. When an array is passed to a function its size is given in ```t[elems] -> int``` parameter.

```never
func tprint( t[elems] : int ) -> int
{
	func __tprint( val : int, i : int, t[elems] : int ) -> int
	{
        i < elems - 1 ? __tprint( print(t[i]), i + 1, t ) : t[i]
	}
	
	__tprint(t[0], 0, t)
}
func main() -> int
{
	tprint( [ 10, 20, 30, 40, 50, 60 ] : int )
}
```

The above function ```__tprint``` is invoked for every element of an array. Each element of the array is printed using ```print(t[i])``` function. Next the ```__tprint``` is invoked again to print the next value. To make program more readable function ```__tprint``` is defined within function ```tprint``` which takes the array as its parameter. This technique will recur in following examples.

```never
func tsum( t[elems] : int) -> int
{
	func __tsum( sum : int, i : int, t[elems] : int ) -> int
	{
		i < elems ? __tsum( sum + t[i], i + 1, t ) : sum
	}
	__tsum(0, 0, t)
}
func main() -> int
{
	tsum( [ 10, 20, 30, 40, 50, 60 ] : int )
}
```
To calculate sum of array elements in each recursive call sum is increased. Finally sum of all elements is returned.

```never
func tmin( t[elems] : int ) -> int
{
	func __tmin( min : int, i : int, t[elems] : int ) -> int
	{
		i < elems ? __tmin( t[i] < min ? t[i] : min, i + 1, t ) : min
	}
    __tmin(t[0], 0, t)
}
func main() -> int
{
	tmin( [ 60, 20, 10, 30, 50, 40, 80, 90, 100 ] : int )
}
```
Similar idea can be used to determine the lowest value within an array...

```never
func exists( e : int, t[elems] : int ) -> int
{
	func __exists( i : int, e : int, t[elems] : int ) -> int
	{
	    i < elems ? ( e == t[i] ? 1 : __exists( i + 1, e, t ) ) : 0
	}
	__exists( 0, e, t )
}
func main() -> int
{
	exists( 100, [ 60, 20, 10, 30, 50, 40, 80, 90, 100 ] : int )
}
```
...or used to determine if given value exists with an array. Recursive calls stop when sought after value is found.

```never
func add_five(e : int) -> int
{
	print(e + 5)
}

func tforeach( t[elems] : int, each(e : int) -> int) -> int
{
	func __tforeach( val : int, i : int, t[elems] : int ) -> int
	{
        i < elems ? __tforeach( each(t[i]), i + 1, t ) : 0
	}
	__tforeach(t[0], 0, t)
}
func main() -> int
{
	tforeach( [ 10, 20, 50, 30, 40 ] : int, add_five )
}
```
[Never][never-lang] supports first-call functions which can be passed to other functions. This property can be used to execute arbitrary function over all elements. In the above example function ```add_five``` is passed to ```tforeach``` function.

```never
func sum_mapi(i : int, e : int) -> int
{
    print(i + e)
}

func tmapi( t[elems] : int, mapi(i : int, e : int) -> int) -> int
{
	func __tmapi( val : int, i : int, t[elems] : int ) -> int
	{
	    i < elems ? __tmapi( mapi(i, t[i]), i + 1, t ) : 0
	}
	__tmapi(t[0], 0, t)
}
func main() -> int
{
	tmapi( [ 10, 20, 50, 30, 40 ] : int, sum_mapi )
}
```
The above listing presents ```mapi``` function which is invoked with element index and its value.

```never
func odd( e : int ) -> int
{
    e % 2
}
func do( e : int ) -> int
{
    print(e + 1)
}

func filter( t[elems] : int, if( int ) -> int, do( int ) -> int ) -> int
{
	func __filter( val : int, i : int, t[elems] : int, if( e : int ) -> int, do( e : int ) -> int ) -> int
	{
		i < elems ? __filter( if(t[i]) ? do(t[i]) : 0,  i + 1, t, if, do ) : 0
	}
	__filter( 0, 0, t, if, do )
}
func main() -> int
{
	filter( [ 61, 22, 11, 34, 58, 41, 83, 92, 101 ] : int,
            odd, do )
}
```
The idea to pass first-class functions can be extended. The above listing presents ```filter``` function which is executed over all elements of an array. When ```if``` function return value other than zero then function ```do``` is invoked.

## Summary
Examples presented in this article show how tail recursion can be used in practise. Both to calculate values of functions or used to execute code over elements of an array. Using tail
recursive functions lets to increase program execution speed and reduce memory size.

[uroboros]: https://never-lang.github.io/never/Serpiente_alquimica.jpg  "Uroboros"
[never-lang]: https://never-lang.github.io/never


