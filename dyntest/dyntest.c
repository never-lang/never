#include <stdio.h>

void __attribute__ ((constructor)) init_dyntest(void)
{
    printf("dyntest initalized\n"); 
}

void __attribute__ ((destructor)) destroy_dyntest(void)
{
    printf("dyntest destructed\n"); 
}

char * test_conc_str(const char * a, const char * b)
{
    static char c[20];
    
    sprintf(c, "dt%s%s", a, b);
    
    return c;
}

char * test_conc_int_str(int d, const char * s)
{
    static char c[20];
    
    sprintf(c, "dt%d:%s", d, s);

    return c;
}


