#include "asgen.h"
#include <stdio.h>
#include "cc.h"

gen_as(struct tokenized_unit* unit, FILE* out)
{
   unsigned long i;
#define c(a) unit->data[a]
   for (i = 0; i < unit->length; i++)
   {
      printf_s("\n%c", c(i));
      switch (c(i))
      {
      case TK_TYP:                                       /*type,len,ident*/
         if (i + 5 < unit->length && c(i + 5) == TK_BRL) /* consider it a function definition*/
         {
            fputs(unit->idents[(int)c(i + 2)], out);
            fputs(":\n", out);
         }
      }
   }
}
