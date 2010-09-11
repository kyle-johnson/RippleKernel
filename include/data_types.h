#ifndef DATA_TYPES_HEADER
#define DATA_TYPES_HEADER
// typedeffs for u_long, u_short, etc

// define NULL if compiler hasn't
#ifndef NULL
#define NULL 0
#endif

typedef unsigned size_t;

// va_list data type... (eventually will be)used by printf
typedef unsigned char *va_list;

// signed data types
typedef signed char s_char;
typedef signed short s_short;
typedef signed int s_int;
typedef signed long s_long;
typedef signed long long s_huge;

// unsigned data types
typedef unsigned char u_char;
typedef unsigned short u_short;
typedef unsigned int u_int;
typedef unsigned long u_long;
typedef unsigned long long u_huge;

#endif
