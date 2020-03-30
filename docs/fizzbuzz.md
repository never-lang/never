---
title: Fizz Buzz
---

Lets change a little bit the famous Fizz Buzz problem...

*"Write a program in Never programming language that prints 
the numbers from 1 to 100. But for multiples of three print “Fizz”
instead of the number and for the multiples of five print “Buzz”.
For numbers which are multiples of both three and five print “Fizz Buzz”."*

And the answer is...

```never
func fizz_buzz() -> int
{
    var i = 1;

    for (i = 1; i <= 100; i = i + 1)
    {
        /* if (i % 15 == 0) */
        if (i % 3 == 0 && i % 5 == 0)
        {
            prints("Fizz Buzz\n")
        }
        else if (i % 3 == 0)
        {
            prints("Fizz\n")
        }
        else if (i % 5 == 0)
        {
            prints("Buzz\n")
        }
        else
        {
            prints(i + "\n")
        }
    };
    
    0
}
```

Enjoy!


