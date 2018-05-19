#include <stdarg.h>
#include <stdio.h>
#include "utils.h"

void print_error_msg(int line_no, const char * format, ...)
{
   va_list args;

   va_start(args, format);

   printf("%s:%d: error: ", utils_file_name, line_no);
   vprintf(format, args);

   va_end(args);
}

void print_warning_msg(int line_no, const char * format, ...)
{
   va_list args;

   va_start(args, format);

   printf("%s:%d: warning: ", utils_file_name, line_no);
   vprintf(format, args);

   va_end(args);
}

