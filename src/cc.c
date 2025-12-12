#include "cc.h"
#include <stdio.h>

cc(source, dest) const char* source, *dest;
{
   FILE *source_file, *dest_file;
   unsigned long line_num = 0;
   char line_buf[LINE_MAX_LEN];

   fopen_s(&source_file, source, "rt");

   printf_s("Now reading from:\n%s\n", source);

   while (fgets(line_buf, LINE_MAX_LEN, source_file))
   {
      printf_s("%llu: %s", line_num++, line_buf, LINE_MAX_LEN);
   }

   return 0;
}
