#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "symtab.h"

void test_one()
{
    symtab * tab = symtab_new(32, NULL);
    
    symtab_delete(tab);
}

void test_two()
{
    symtab * tab = symtab_new(32, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
        
    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_three);
    
    assert(symtab_lookup_func(tab, "func_one") == func_one); 
    assert(symtab_lookup_func(tab, "func_two") == NULL);
    assert(symtab_lookup_func(tab, "func_three") == func_three); 
        
    func_delete(func_one);
    func_delete(func_three);
    symtab_delete(tab);
}

void test_three()
{
    symtab * tab = symtab_new(4, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);
        
    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_two);
    symtab_add_func(tab, func_three);
    symtab_add_func(tab, func_four);
    
    assert(symtab_lookup_func(tab, "func_one") == func_one); 
    assert(symtab_lookup_func(tab, "func_two") == func_two);
    assert(symtab_lookup_func(tab, "func_three") == func_three); 
        
    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    
    symtab_delete(tab);
}

void test_four()
{
    symtab * tab_one = symtab_new(32, NULL);
    symtab * tab_two = symtab_new(32, tab_one);

    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);
        
    symtab_add_func(tab_one, func_one);
    symtab_add_func(tab_one, func_two);
    symtab_add_func(tab_two, func_three);
    symtab_add_func(tab_two, func_four);
    
    assert(symtab_lookup_func(tab_two, "func_one") == func_one); 
    assert(symtab_lookup_func(tab_two, "func_two") == func_two);
    assert(symtab_lookup_func(tab_two, "func_three") == func_three); 
    assert(symtab_lookup_func(tab_two, "func_four") == func_four);
    assert(symtab_lookup_func(tab_two, "func_five") == NULL);
        
    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    
    symtab_delete(tab_one);
    symtab_delete(tab_two);
}

int main(int argc, char * argv[])
{
    /* test_one();
    test_two();
    test_three(); */
    test_four();

    return 0;
}

