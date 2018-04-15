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
    symtab_entry * entry;
    symtab * tab = symtab_new(32, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
        
    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_three);
    
    entry = symtab_lookup_func(tab, "func_one");
    assert(entry->arg_func_value == func_one); 
    
    entry = symtab_lookup_func(tab, "func_two");
    assert(entry == NULL);
    
    entry = symtab_lookup_func(tab, "func_three");
    assert(entry->arg_func_value == func_three); 
        
    func_delete(func_one);
    func_delete(func_three);
    symtab_delete(tab);
}

void test_three()
{
    symtab_entry * entry;
    symtab * tab = symtab_new(4, NULL);
    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);
        
    symtab_add_func(tab, func_one);
    symtab_add_func(tab, func_two);
    symtab_add_func(tab, func_three);
    symtab_add_func(tab, func_four);

    entry = symtab_lookup_func(tab, "func_one");     
    assert(entry->arg_func_value == func_one); 
    
    entry = symtab_lookup_func(tab, "func_two"); 
    assert(entry->arg_func_value == func_two);
    
    entry = symtab_lookup_func(tab, "func_three"); 
    assert(entry->arg_func_value == func_three); 
        
    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    
    symtab_delete(tab);
}

void test_four()
{
    symtab_entry * entry;
    symtab * tab_one = symtab_new(32, NULL);
    symtab * tab_two = symtab_new(32, tab_one);

    func * func_one = func_new(strdup("func_one"), NULL, NULL, NULL);
    func * func_two = func_new(strdup("func_two"), NULL, NULL, NULL);
    func * func_three = func_new(strdup("func_three"), NULL, NULL, NULL);
    func * func_four = func_new(strdup("func_four"), NULL, NULL, NULL);
    func * func_five = func_new(strdup("func_five"), NULL, NULL, NULL);
    func * func_six = func_new(strdup("func_six"), NULL, NULL, NULL);
        
    symtab_add_func(tab_one, func_one);
    symtab_add_func(tab_one, func_two);
    symtab_add_func(tab_two, func_three);
    symtab_add_func(tab_two, func_four);
    symtab_add_func(tab_two, func_five);
    symtab_add_func(tab_two, func_six);
    
    entry = symtab_lookup_func(tab_two, "func_one"); 
    assert(entry->arg_func_value == func_one); 
    
    entry = symtab_lookup_func(tab_two, "func_two"); 
    assert(entry->arg_func_value == func_two);
    
    entry = symtab_lookup_func(tab_two, "func_three"); 
    assert(entry->arg_func_value == func_three); 
    
    entry = symtab_lookup_func(tab_two, "func_four"); 
    assert(entry->arg_func_value == func_four);
    
    entry = symtab_lookup_func(tab_two, "func_five"); 
    assert(entry->arg_func_value == func_five);
    
    entry = symtab_lookup_func(tab_two, "func_six"); 
    assert(entry->arg_func_value == func_six);
    
    entry = symtab_lookup_func(tab_two, "func_seven"); 
    assert(entry == NULL);    
        
    func_delete(func_one);
    func_delete(func_two);
    func_delete(func_three);
    func_delete(func_four);
    func_delete(func_five);
    func_delete(func_six);
    
    symtab_delete(tab_one);
    symtab_delete(tab_two);
}

int main(int argc, char * argv[])
{
    test_one();
    test_two();
    test_three();
    test_four();

    return 0;
}

