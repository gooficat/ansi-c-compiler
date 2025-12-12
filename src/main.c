#include "cc.h"

main(argc, argv) char* argv[];
{
   char *source_path, *destination_path;
   int err_code;
   if (argc != 3)
   {
      source_path = "C:/users/user/projects/ansi-c-compiler/tests/main.c";
      destination_path = "C:/users/user/projects/ansi-c-compiler/tests/out.asm";
   }
   else
   {
      source_path = argv[1];
      destination_path = argv[2];
   }

   err_code = cc(source_path, destination_path);
   if (err_code)
   {
      return err_code;
   }

   return 0;
}
