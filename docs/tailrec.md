# Recursion

*To understand recursion one needs to understand recursion*

![alt text][uroboros]

## Introduction
Many problems are expressed using themselves as subproblems. This leads to an interesting observation, that to solve a problem one needs to solve the same problem again. Of course such loop needs to be broken somewhere. Usually subproblems are of smaller size than the original. In mathematics a classical example of such a function is Fibonnaci function which is defined as ```fib(1) = 1, fib(2) = 1, fib(n) == fib(n - 1) + fib(n - 2)```. Computer scientists also noticed that value of such functions can be automatically calculated by a computer. Unfortunatelly computers are electrical devices which have their limitations. One of them is speed of execution which is finitive and thus larger problems take longer to execute. The other one is memory size which also influences size of problems which can be solved. Computers also execute instructions and they must be programmed to reuse same results again. Otherwise they will blindly recalulate same problems again and again. For example, simple implementation of the Fibonacci function for each ```n``` would solve the same subproblems twice. In recursion of course! Which leads to expotentian number of instructions invoked. Despite those limitations it is tempting to use recursion to express problems. They become easier to express and later understand. In many cases, when subproblem is divided by two, for example, in many practical problems this leads to very few levels of recursion which can be handled by physical computers. For problems with 1 milion elements such division results only in 20 levels of recursion calls. Also it is easy to use memory and store results of already calculated functions. Such a technique, known as memoizations, signifiantly speeds to calculations. Another technique, which is described in this article, is known as tail recursion.

## Tail Recursion
The following code listing presents Fibonacci function expressed in [Never][never-lang] language. It is expressed almost in the same way as its mathematical defintion. To calculate ```fib(20)``` function ```fib``` is called 21891 times. When executed on a typical laptop it takes approximately 0.6s to execute.

```
func fib(n -> int) -> int 
{
    return (n == 0) ? 1 : (n == 1) ? 1 : fib(n - 1) + fib(n - 2);
}
```

Mathematicians and computer scientists begain to think how this situation could be improved. Mathematicians noticed that when the recursion call is the last one results are evaluated differenlty. Function result is calculated as result of on immediate subproblem. Thus, there are much fewer subproblems which are calculated. Computer scientists realized that the call can be optimized and avoided completely. Such recursive last calls turn into a loop which is executed again and again. In the above example the last function called is addition. How can it be turned into tail recursion?

First, it needs to be the last call. Second, it needs parameter ```n```. Also we may notice that result is a sum of two previous values, lets name them ```a``` and ```b```.

```
func fib(n -> int, a -> int, b -> int) -> int
{
    return (n == 0) ? a : (n == 1) ? b : fib(n - 1, b, a + b);
}
```

The above listing presents tail recursive definition of the Fibnonacci function. During each call its value is calculated by adding two previous values. What is most important there will be just 20 recursive calls. Now it takes only 0.004s to execute. 150 times faster and 1094 fewer function calls!

## Mathematical Function
Other functions can be turned into tail recursive.

```
func gcd(x -> int, y -> int) -> int
{
    return (y == 0) ? x : gcd(y, x % y);
}
```

The above function present greatest common divisor function.

```
func factorial(n -> int, val -> int) -> int
{
    return n == 0 ? val : factorial(n - 1, n * val);
}
```
```
func power(a -> int, n -> int, val -> int) -> int
{
    return n == 0 ? val : power(a, n - 1, a * val);
}
```

Next two functions define factorial and power functions.

## Computer Science Function
Tail recursive functions can also be used to create programs. A special class of programming languages to which Never belongs can only use functions. There is no other way to run a program than to execute functions. Without tail recursion such programs would also suffer from speed and memory limitations.

The following listings present how tail recursions can be used to execute code over array elements.

```
func tsum( t[elems] -> int) -> int
{
	func __tsum( sum -> int, i -> int, t[elems] -> int ) -> int
	{
		return i < elems ? __tsum( sum + t[i], i + 1, t ) : sum;
	}
	return __tsum(0, 0, t);
}
func main() -> int
{
	return tsum( { 10, 20, 30, 40, 50, 60 } -> int );
}
```

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
	return tmin( { 60, 20, 10, 30, 50, 40, 80, 90, 100 } -> int );
}
```

```
func tprint( t[elems] -> int ) -> int
{
	func __tprint( val -> int, i -> int, t[elems] -> int ) -> int
	{
		return i < elems - 1 ? __tprint( print(t[i]), i + 1, t ) : t[i];
	}
	
	return __tprint(t[0], 0, t);
}
func main() -> int
{
	return tprint( { 10, 20, 30, 40, 50, 60 } -> int );
}
```

```
func exists( e -> int, t[elems] -> int ) -> int
{
	func __exists( i -> int, e -> int, t[elems] -> int ) -> int
	{
		return i < elems ? ( e == t[i] ? 1 : __exists( i + 1, e, t ) ) : 0;
	}
	return __exists( 0, e, t );
}

func main() -> int
{
	return exists( 100, { 60, 20, 10, 30, 50, 40, 80, 90, 100 } -> int );
}
```

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

```
func sum_mapi(i -> int, e -> int) -> int
{
	return print(i + e);
}

func tmapi( t[elems] -> int, mapi(i -> int, e -> int) -> int) -> int
{
	func __tmapi( val -> int, i -> int, t[elems] -> int ) -> int
	{
		return i < elems ? __tmapi( mapi(i, t[i]), i + 1, t ) : 0;
	}
	return __tmapi(t[0], 0, t);
}
func main() -> int
{
	return tmapi( { 10, 20, 50, 30, 40 } -> int, sum_mapi );
}
```

```
func odd( e -> int ) -> int
{
    return e % 2;
}
func do( e -> int ) -> int
{
    return print(e + 1);
}

func filter( t[elems] -> int, odd( int ) -> int, do( int ) -> int ) -> int
{
	func __filter( val -> int, i -> int, t[elems] -> int, odd( e -> int) -> int, do( e -> int ) -> int ) -> int
	{
		return i < elems ? __filter( odd(t[i]) ? do(t[i]) : 0,  i + 1, t, odd, do ) : 0;
	}
	return __filter( 0, 0, t, odd, do );
}
func main() -> int
{
	return filter( { 61, 22, 11, 34, 58, 41, 83, 92, 101 } -> int,
	               odd, do );
}
```

## Summary

[uroboros]: https://never-lang.github.io/never/Serpiente_alquimica.jpg  "Uroboros"
[never-lang]: https://never-lang.github.io/never


