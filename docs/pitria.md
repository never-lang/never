# Pythagorean Triangle


```never
record Triangle
{
    a : int;
    b : int;
    c : int;
}

func printTriangle(t[D] : Triangle) -> int
{
    let d = 0;
    
    for (d = 0; d < D; d = d + 1)
        prints("pythagorean triangle" +
               " a = " + t[d].a + 
               " b = " + t[d].b +
               " c = " + t[d].c + "\n")
}

func range() -> [_] : int
{
    [ 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 ] : int
}

func main() -> int
{
    let r = range();
    let pitria = [ Triangle(a, b, c) |
                    a in r;
                    b in r;
                    c in r;
                    a * a + b * b == c * c ] : Triangle;
    
    printTriangle(pitria);
    
    0
}
```

Output:

```plaintext
pythagorean triangle a = 3 b = 4 c = 5
pythagorean triangle a = 4 b = 3 c = 5
pythagorean triangle a = 5 b = 12 c = 13
pythagorean triangle a = 6 b = 8 c = 10
pythagorean triangle a = 8 b = 6 c = 10
pythagorean triangle a = 12 b = 5 c = 13
```

