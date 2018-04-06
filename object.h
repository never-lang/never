#ifndef __OBJECT_H__
#define __OBJECT_H__

#define TYPE_PTR       0
#define TYPE_BYTE_1    1
#define TYPE_BYTE_2    2
#define TYPE_BYTE_4    3
#define TYPE_INT       4
#define TYPE_UINT      5
#define TYPE_FLOAT     6
#define TYPE_CHAR_ARR  7
#define TYPE_BYTE_CODE 8

typedef struct object {
    unsigned short mark : 1;
    unsigned short size : 15;
    char types[0];
    unsigned int values[0];
} object;

object * object_new(unsigned short size, char * types);
void object_delete(object * obj);

unsigned int object_get_value(object * obj, unsigned int index);
void object_set_value(object * obj, unsigned int index, unsigned int value);

unsigned int * object_get_ptr(object * obj, unsigned int index);

#endif /* __OBJECT_H__ */


