---
title: Tribute to Alonso C.
---

# Tribute to Alonso C.



## Currying Functions

```swift
func add(x -> int, y -> int) -> int
{
    x + y
}
```

```swift
func cadd(x -> int) -> (int) -> int
{
    let func (y -> int) -> int
    {
        x + y
    }
}
```

```swift
func main() -> int
{
    print(add(5, 7));
    print(cadd(5)(7));
    print(cadd(2)(cadd(3)(7)));

    0
}
```

# Summary

