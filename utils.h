#ifndef __UTILS_H__
#define __UTILS_H__ 1

extern char * utils_file_name;

void print_error_msg(int line_no, const char * format, ...);

void print_warning_msg(int line_no, const char * format, ...);

#endif /* __UTILS_H__ */
