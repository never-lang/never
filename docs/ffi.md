---
title: FFI Demo
---

# Foreign Function Interface - Code Snippet

```C
/* data */

int pos_x = 0;
int pos_y = 0;

/* externalized functions */

int turn_left()
{
    printf("turn_left\n");
    return 0;
}

int turn_right()
{
    printf("turn_right\n");
    return 0;
}

int go_ahead(int dist)
{
    printf("go_ahead %d\n", dist);
    
    pos_x += dist;
    pos_y += dist;
    
    return dist;
}

int get_x() { return pos_x; }
int get_y() { return pos_y; }

int fire(int at_x, int at_y)
{
    printf("FIRE! @ (%d, %d)\n", at_x, at_y);
    return 0;
}

/* scripts */

int program_one(program * prog)
{
    const char * prog_str =
        "extern \"host\" func turn_left() -> int "
        "extern \"host\" func turn_right() -> int "
        "extern \"host\" func go_ahead(dist : int) -> int "
        "extern \"host\" func get_x() -> int "
        "extern \"host\" func get_y() -> int "
        "extern \"host\" func fire(at_x : int, at_y : int) -> int "
        " "
        "func on_key(dist : int) -> int "
        "{ "
        "   turn_left(); "
        "   go_ahead(dist); "
        "  "
        "   fire(get_x() + 20, get_y() + 30); "
        "  "
        "   0 "
        "}";

    return nev_compile_str_main(prog_str, "on_key", prog);
}

int program_two(program * prog)
{
    const char * prog_str =
        "extern \"host\" func turn_left() -> int "
        "extern \"host\" func turn_right() -> int "
        "extern \"host\" func go_ahead(dist : int) -> int "
        "extern \"host\" func get_x() -> int "
        "extern \"host\" func get_y() -> int "
        "extern \"host\" func fire(at_x : int, at_y : int) -> int "
        " "
        "func on_key(dist : int) -> int "
        "{ "
        "   turn_right(); "
        "   go_ahead(dist); "
        "  "
        "   fire(get_x() + 20, get_y() + 30); "
        "   fire(get_x() + 25, get_y() + 35); "
        "  "
        "   0 "
        "}";

    return nev_compile_str_main(prog_str, "on_key", prog);
}

int execute_prog(program * prog, int param1)
{
    int ret = 0;
    object result = { 0 };

    prog->params[0].int_value = param1;

    ret =
        nev_execute(prog, &result, DEFAULT_VM_MEM_SIZE, DEFAULT_VM_STACK_SIZE);
    if (ret == 0)
    {
        assert(result.type == OBJECT_INT && result.int_value == 0);
    }
        
    return 0;
}

/* demo */

void test_one()
{
    program * prog_one = program_new();
    program * prog_two = program_new();

    int ret1 = program_one(prog_one);
    int ret2 = program_two(prog_two);
    if (ret1 == 0 && ret2 == 0)
    {
        execute_prog(prog_one, 60);
        execute_prog(prog_two, 40);
    }

    program_delete(prog_one);
    program_delete(prog_two);
}

int main(int argc, char * argv[])
{
    test_one();
}
```

