#include "cc.h"
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "asgen.h"

struct stream
{
   FILE* file;
   int buffer;
};

const struct type types[] = {
    {"byte", 1},  /**/
    {"short", 2}, /**/
    {"int", 4},
};

fetch_chunk(chunk, strm) char* chunk;
struct stream* strm;
{
   int i = 0;
   if ((strm->buffer) == EOF)
      return 0;
   while (isspace(strm->buffer))
   {
      if ((strm->buffer) == EOF)
         return 0;
      strm->buffer = fgetc(strm->file);
   }
   do
      chunk[i++] = (char)tolower(strm->buffer);
   while (isalpha(strm->buffer = fgetc(strm->file)));

   chunk[i] = 0;
   return 1;
}

cc(source_path, dest_path) const char* source_path, *dest_path;
{
   struct stream src = {0, 0};
   char chunk_buf[CHUNK_MAX_LEN];
   char* output = NULL;
   FILE* dest_file;
   unsigned long i, old_size;

   struct tokenized_unit tokens;

   fopen_s(&src.file, source_path, "rt");

   printf_s("Now reading from:\n%s\n", source_path);

   tokens.data = malloc(4);
   tokens.length = 0;
   tokens.powmul = 2;
   tokens.idents = malloc(sizeof(char*));
   tokens.num_idents = 0;

   src.buffer = fgetc(src.file);

   while (fetch_chunk(chunk_buf, &src))
   {
      unsigned char tok_len = strlen(chunk_buf);
      old_size = tokens.length;
      for (i = 0; i < sizeof(types) / sizeof(struct type); i++)
         if (!strcmp(chunk_buf, types[i].name)) /*TODO add a more sophisticated type system!*/
         {
            tokens.length += 2;
            if (pow(2, tokens.powmul) < tokens.length)
               tokens.data = realloc(tokens.data, pow(2, ++tokens.powmul));
            tokens.data[old_size] = TK_TYP;
            tokens.data[old_size + 1] = (char)types[i].size;
            goto eoloop;
         }

      if (!isalnum(chunk_buf[0]))
      {
         ++tokens.length;
         if (pow(2, tokens.powmul) < tokens.length)
            tokens.data = realloc(tokens.data, pow(2, ++tokens.powmul));
         tokens.data[old_size] = chunk_buf[0];
         goto eoloop;
      }

      tokens.length += 3; /* + tok_len;*/
      if (pow(2, tokens.powmul) < tokens.length)
         tokens.data = realloc(tokens.data, pow(2, ++tokens.powmul));
      tokens.data[old_size] = TK_IDT;
      tokens.data[old_size + 1] = tok_len;

      {
         unsigned char ident_id = tokens.num_idents;
         unsigned long j;
         for (j = 0; j < ident_id; j++)
         {
            if (!strcmp(chunk_buf, tokens.idents[j]))
            {
               tokens.data[old_size + 2] = j;
               goto eoloop;
            }
         }
         tokens.idents = realloc(tokens.idents, ++tokens.num_idents * sizeof(char*));
         tokens.data[old_size + 2] = ident_id;
         tokens.idents[ident_id] = malloc(tok_len + 1);
         strcpy_s(tokens.idents[ident_id], tok_len + 1, chunk_buf);
         goto eoloop;
      }

      /* memcpy_s(tokens.data + old_size + 2, tok_len, chunk_buf, tok_len);*/

      printf_s("%d |%c|", tokens.data[old_size + 1], tokens.data[old_size + 1]);
   eoloop:
      printf_s("Token %s\n", chunk_buf);
   }
   for (i = 0; i < tokens.num_idents; i++)
   {
      printf_s("%s\n", tokens.idents[i], strlen(tokens.idents[i]) + 1);
   }
   for (i = 0; i < tokens.length; i++)
   {
      printf_s("%c", tokens.data[i]);
   }

   if (src.file)
      fclose(src.file);

   output = malloc(1);
   output[0] = 0;

   /*write output*/
   fopen_s(&dest_file, dest_path, "wt");
   gen_as(&tokens, dest_file);
   /* fwrite(output, sizeof(char), strlen(output), dest_file);*/

   if (dest_file)
      fclose(dest_file);

   return 0;
}
