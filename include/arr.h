#pragma once

#include <malloc.h>
#include <stdlib.h>
#include "defs.h"

struct generic_arr
{
   u64 len;
   u64 powmul;
   void ptr get;
};

#define array_type(type)                                                                           \
   struct                                                                                          \
   {                                                                                               \
      u64 len;                                                                                     \
      u64 powmul;                                                                                  \
      type ptr get;                                                                                \
   }

typedef array_type(str) strarr_t;

#define mk_arr(t) {.get = malloc(sizeof(t)), .len = 0, .powmul = 1}

#define del_arr(arr) free(arr.get)

#define at(i) get[i]

void rsz_arr_f(struct generic_arr ptr arr, u8 sz);

#define rsz_arr(arr, type, amt)                                                                    \
   arr.len += amt;                                                                                 \
   rsz_arr_f((struct generic_arr ptr)addr arr, sizeof(type))
