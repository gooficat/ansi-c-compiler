#include <stdio.h>
#include "asm.h"

#include "fancc.h"

int main(void)
{
   init_fancc;

   fancc(RED | GREEN, "Hello, World!\n");
   assemble("C:/Users/User/Projects/ansi-c-compiler/tests/test.s");

   SETCOLOR(WHITE);
   return 0;
}
