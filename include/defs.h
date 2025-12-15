#pragma once

#define ptr *
#define addr &
#define deref *

#ifndef __cplusplus
#define and &&
#define or ||
#define not !
#define is ==
#endif

#define notis !=

#define OR |
#define NOT ~
#define AND &
#define XOR ^
#define SLEFT <<
#define SRIGHT >>

#define elif else if

typedef signed char i8;
typedef unsigned char u8;

#ifdef _WIN32
typedef signed short i16;
typedef signed long i32;
typedef signed long long i64;

typedef unsigned short u16;
typedef unsigned long u32;
typedef unsigned long long u64;

#ifdef _WIN64
#ifdef __GNUC__
typedef signed int i128 __attribute__((mode(TI)));
typedef unsigned int u128 __attribute__((mode(TI)));
#endif
#endif
#else
typedef signed short i16;
typedef signed int i32;
typedef signed long i64;

typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long u64;
#ifdef __X86_64__
typedef signed long long i128;
typedef unsigned long long u128;
#endif
#endif

typedef char ptr str;

#define struc_t typedef struct
#define enu_m typedef enum
#define unio_n typedef union