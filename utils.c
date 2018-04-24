#include <stdarg.h>
#include <stdio.h>

void print_error_msg(int lineNo, const char * format, ...)
{
   va_list args;

   va_start(args, format);

   printf("<stdin>:%d: error: ", lineNo);

   vprintf(format, args);
   printf("\n");

   va_end(args);
}

void print_warning_msg(int lineNo, const char * format, ...)
{
   va_list args;

   va_start(args, format);

   printf("<stdin>:%d: warning: ", lineNo);

   vprintf(format, args);
   printf("\n");

   va_end(args);
}

