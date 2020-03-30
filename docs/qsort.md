---
title: Quick Sort
---

# Sorting - Quick Sort

Data sorting is one of fundamental problems introduced at algorithm courses.
One of well known algorithms which is frequently used to efficiently sort data
is called quick sort. This blog presents quick sort algorithm implementation
in Never programming language.

## Introduction

There are many sorting algorithms described with different properties such
as time complexity, stability and others.

Time complexity detemines how much time is spent to sort given data. Many sorting
algorithms have O(n^2) or O(n\*log(n)) complexities which denotes how many operations are needed
when n number of items is sorted. In special cases, for example when sorting
integer items from finite range, it may take O(n) operations to sort them. In general, though,
sorting algorithms use O(n\*log(n)) or O(n^2) operations. Quick sort time
complexity depends on data which is being sorted. In many cases it will take
O(n\*log(n)) operations to sort data. However, in some cases it may take O(n^2)
operations. Despite this drawback, the algorithm frequently presents O(n\*log(n))
characteristics and effitently sorts data.

Another mentioned property is stability.
In short, when sorting records which consit of two attributes by the first
attribute, the order of records by the second attribute will be kept. To illustrate
this property lets imagine that we are sorting list of names by the first name.
When, before sorting, two names with the same first name are in order, then
after sorting, their mutual order will be kept. Unfortunatelly this property
is not kept by quick sort algorithm. In some cases this may limit its usage.

Quick sort basic idea is to split data into two sequences which are recursively
sorted. During every pass items in each sequence lower than some chosen item
(called pivot) are moved to the left part and larger than it are moved to the
right. As long as they are to the left or to the right of the pivot, their
mutual order is not considered. When such subsequences are sorted next,
then their order will be sorted. Also, please note, that the pivot is at its
correct place when sorting each sequence. (Btw, this property is used to quickly
choose median of given data, but we will skip this algorithm in this article).

Quick sort algorithm is often considered as toublesome as choosing the pivot
and easily moving items to the left or right side of the sequence may be tidous.
Later I will show how to avoid such obstacles and make quick sort implementation
as easy as other sorting algorithms.

## Algorithm

In the beginning lets perpare some utility functions. Which will be needed to
visualize and implement workings of the algorithm.

```never
func print_tab(tab[D] : int) -> int
{
    var i = 0;
    
    for (i = 0; i < D; i = i + 1)
        prints(tab[i] + " ");
        
    prints("\n");

    0
}

func swap(a : int, b : int) -> int
{
    var t = 0;
    
    t = a;
    a = b;
    b = t;
    
    0
}

func randomize(seed : int) -> () -> int
{
    let v = seed;
    func rand() -> int
    {
        v = (v * 11 + 12) % 21474
    }
    rand
}
```

The first of the above functions prints out array which is passed. Function
```swap``` swaps two items. Last, ```randomize``` returns ```rand``` closer
which is used to generate random values. Quick sort algorithm uses random
places to choose pivot. This way we can try to avoid O(n^2) algorithm complexity.

```never
func qsort_rec(tab[D] : int,
               beg : int, end : int,
               rand() -> int) -> int
{
    var i = 0;
    var j = beg - 1;
    var p = 0;
    
    if (beg < end - 1)
    {
        p = beg + rand() % (end - beg);
        swap(tab[p], tab[end - 1]);

        prints(beg + " " + end + " " + p + "\n");
        
        for (i = beg; i < end; i = i + 1)
        {
            if (tab[i] < tab[end - 1])
            {
               j = j + 1;
               swap(tab[j], tab[i])
            }
        };
    
        j = j + 1;
        swap(tab[j], tab[end - 1]);
    
        qsort_rec(tab, beg, j, rand);
        qsort_rec(tab, j + 1, end, rand)
    }
    else
    {
        0
    };
    
    0
}

func qsort(tab[D] : int) -> int
{
    var rand = randomize(6);

    qsort_rec(tab, 0, D, rand)
}
```

Functions ```qsort``` and ```qsort_rec``` implement quick sort algorithm. Function
```qsort``` is used to initialize recursive invocations of sequence sortings.
Array size in Never is passed together with array. To quickly split array
into two parts we will use start and end index of the array instead. However,
the first invocation of ```qsort_rec``` takes first and last indexes based on array
size. Function ```qsort_rec``` chooses pivot ```p``` and then moves item
in this position to the end of a sequence. This trick lets to easily implement
the algorithm. Also starting index ```j``` where items are moved is initalized
to ```-1```. After sequence pass an item moved to the end is placed in correct
position. Then sorting continues in left and right subsequences.


```never
func main() -> int
{
    var tab = [ 4, 9, 2, 7, 5, 6, 8, 3, 10, 1 ] : int;
    
    print_tab(tab);
    
    qsort(tab);
    
    print_tab(tab);
    
    0
}
```

The above algorithm is demonstrated in function ```main``` to sort sample data.

## Summary

In this article I presented quick sort algorithm implementation in Never
programming language. Of course it is possible to implement it using any other
language. The trick to move pivot to the end of sequence sorted in each
recursive pass lets to implement it easily.


