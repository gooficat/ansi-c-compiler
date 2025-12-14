#include "arr.h"
#include <malloc.h>
#include <math.h>
#include <stdio.h>
#include "defs.h"

void rsz_arr_f(struct generic_arr ptr arr, u8 sz)
{
   u64 spowmul = arr->powmul * arr->powmul;
   if (spowmul < arr->len)
   {
      ++arr->powmul;
   retry:;
      void* inb = realloc(arr->get, sz * (arr->powmul * arr->powmul));
      if (inb)
      {
         arr->get = inb;
      }
      else
      {
         printf_s("Failed to resize array! Retrying. This may cause a loop.");
         goto retry;
      }
   }
}
