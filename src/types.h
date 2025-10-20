#ifndef HH_TYPES
#define HH_TYPES

#ifndef NULL
#define NULL ((void *) 0)
#endif

#define true (1)
#define false (0)

#define ERROR_VALUE (0xC0000008)

typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef signed char        i8;
typedef signed short       i16;
typedef signed int         i32;
typedef signed long long   i64;
typedef float              f32;
typedef double             f64;

typedef u8 uchar;
typedef u16 ushort;
typedef u32 uint;
typedef u64 ulong;
typedef i64 llong;

typedef u8 byte;
typedef u16 word;
typedef u32 dword;
typedef u64 qword;

typedef u8 boolean;
typedef u16 wchar;

typedef char *String;
typedef wchar *wString;

#endif