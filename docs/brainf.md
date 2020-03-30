---
title: Brain F\*\*\*
---

# Brain F\*\*\*

## Introduction

Brain F\*\*\* language was created in 1990s by Urban Müller. Its notable feature
is its extreme minimalism. Each Brain F\*\*\* program consists of just eight instructions!
Primary purpose of the language was to amaze other programmers rather than
write any useful programs in it. Surprisingly it found its usage to demonstrate
expresiveness of other programming languages. This article presents Brain F\*\*\*
interpreter implemented in Never programming language. Compiler and interpreter
implementation is based on C implementation by Robert de Bath found on [RosettaCode site][1].

The following table lists Brain F\*\*\* instructions:

| Command  | Description  |
|----|---|
| >  | Move the memory pointer to the right |
| <  | Move the memory pointer to the left  |
| +  | Increment the memory cell under the memory pointer |
| -  | Decrement the memory cell under the memory pointer |
| .  | Print the character signified by the memory cell at the memory pointer |
| ,  | Read a character and store it in the memory cell at the memory pointer  |
| [  | Jump past the matchin \] if the memory cell under the memory pointer is 0  |
| ]  | Jump back to the mating \[ if the memory cell under the memory pointer is not 0 |

## Implementation

### Data Structures

```never
record BFI
{
    cmd : char;
    next : BFI;
    jmp : BFI;
}

record MEM
{
    val : int;
    next : MEM;
    prev : MEM;
}
```

Brain F\*\*\*\* compiler and interpreter uses ```BFI``` record to store
list of instuctions linked with ```next``` attribute. ```jmp``` attribute
is used to point where a jump should be executed when \[ or \] instruction
is executed. ```MEM``` record is a double linked list through ```next``` and
```prev``` attributes which stores memory values.

### Algorithms

```never
func compile(prog : string) -> BFI
{
    var i = 0;
    var n = BFI;
    var p = BFI;
    var j = BFI;
    var pgm = BFI;

    /*
     *  Only valid programs are accepted by the following function.
     *  Add each valid BF command onto the end of the program. 
     *  The 'j' variable points at the list of currently open '[' commands,
     *  one is matched off by each ']'.
     */
    
    for (i = 0; i < length(prog); i = i + 1) {
        printc(prog[i]);
    
        /* create program instruction */
        n = BFI('0', nil, nil);
    
        /* add it at the end program which begins at pgm */
        if (p != nil) {
            p.next = n
        } else {
            pgm = n
        };
        
        /* instruction name */
        n.cmd = prog[i];
        p = n;
    
        /* if opening \[ was found push instruction pointer to stack */
        if (prog[i] == '[') {
            n.jmp = j;
            j = n;
            0
            
        /* if closing \] was found pop instruction pointer from stack */
        } else if (prog[i] == ']') {
            n.jmp = j;
            j = j.jmp;
            n.jmp.jmp = n;
            0
        } else {
            0
        }
    };
    
    pgm
}
```

```never
func exec(pgm : BFI) -> int
{
    var m = MEM(0, nil, nil);
    var n = BFI;

    for (n = pgm; n != nil; n = n.next) {
        /* increase memory cell */
        if (n.cmd == '+') {
            m.val = m.val + 1
        /* decrease memory cell */
        } else if (n.cmd == '-') {
            m.val = m.val - 1
        /* print character */
        } else if (n.cmd == '.') {
            printc(chr(m.val));
            0
        /* read character */
        } else if (n.cmd == ',') {
            m.val = read()
        /* jump if value in memory is 0 */
        } else if (n.cmd == '[') {
            if (m.val == 0) {
                n = n.jmp;
                0
            } else {
                0
            }
        /* jump is value in memory is not 0 */
        } else if (n.cmd == ']') {
            if (m.val != 0) {
                n = n.jmp;
                0
            } else {
                0
            }
        /* move to previous memory cell */
        } else if (n.cmd == '<') {
            m = m.prev;
            0
        /* move to next memory cell. if not available extend memory */
        } else if (n.cmd == '>') {
            if (m.next == nil) {
                m.next = MEM(0, nil, nil);
                m.next.prev = m;
                0
            } else {
                0
            };

            m = m.next;
            0
        } else {
            0
        }
    };
    
    0
}
```

```never
func run(prog : string) -> int
{
    var pgm = BFI;

    pgm = compile(prog);

    prints("\n");

    exec(pgm);
    
    0
}
```

```never
func main() -> int
{
    /* Hello World! */
    run("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.>++.");
    #run("++++++++++[>+++++++>++++++++++>+++>+<<<<-]>++.>+.+++++++..+++.>++.<<+++++++++++++++.>.+++.------.--------.>+.>.");
    #prints("Adds two digits\n"); run(",>,[<+>-]<.");
    #run(",>,.<.");

    0
}
```

Brain F\*\*\*\* compiler and interpreter is implemeneted using two major
functions - ```compile``` and ```exec```. Former function creates a list of instructions
and marks jumps which should be executed when \[ or \] instruction is executed.
Later function executes instructions according to their specifiction.

## Summary

I hope you liked this entry!

[1]: https://rosettacode.org/wiki/Execute_Brain****/C "Rosetta Code BF"


