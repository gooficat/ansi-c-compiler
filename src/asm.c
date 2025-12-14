#include "fancc.h"
#undef addr
#include "arr.h"
#include "asm.h"
#include "file.h"

#define TOK_MAX 32

enu_m{
    TK_REG = '%',    // register
    TK_KW,           // keyword (either a label or an instruction. this will be deduced after
                     // tokenization)
    TK_LBDEC = ':',  // label declaration symbol (a colon)
    TK_CMA = ',',    // a comma (,)
    TK_LCMT = ';',   // line comment (;)
    TK_NUM = '$',    // a numerical value
} toktype_e;

struc_t
{
   toktype_e type;
   char* content;
}
token_s;
typedef array_type(token_s) tokenarr_t;

tokenarr_t fetch_tokens(filestream_s ptr in_stream);

void assemble(const str path)
{
   fancc(BLUE, "%s\n", path);
   filestream_s in_stream;
   fopen_s(&in_stream.file, path, FMODE_RT);
   tokenarr_t tokens = fetch_tokens(addr in_stream);
   fclose(in_stream.file);

   for (u64 i = 0; i != tokens.len; ++i)
   {
      if (tokens.get[i].content)
      {
         fancc(i, "%c%s\n", tokens.get[i].type, tokens.get[i].content);
         free(tokens.get[i].content);
      }
      else
         fancc(i, "%c\n", tokens.get[i].type);
   }
}

tokenarr_t fetch_tokens(filestream_s ptr in_stream)
{
   fsgetc(deref in_stream);
   char tk[TOK_MAX];

   tokenarr_t tokens = mk_arr(token_s);

   bool neof = true;
   fsgettk(in_stream, tk, TOK_MAX);
   while (neof)
   {
      if (!tk[0])
         break;
      fancc(GREEN, "%s\n", tk);
      u64 tkid = tokens.len;
      rsz_arr(tokens, token_s, 1);
      u8 tklen = (u8)strlen(tk) + 1;
      if (tklen == 2)  // 1 + null term
      {
         tokens.get[tkid].type = tk[0];
         if (tk[0] == '%')
         {
            fsgettk(in_stream, tk, TOK_MAX);
            tklen = (u8)strlen(tk) + 1;
            tokens.get[tkid].content = malloc(tklen);
            memcpy_s(tokens.get[tkid].content, tklen, tk, tklen);
         }
         else
            tokens.get[tkid].content = NULL;
      }
      else
      {
         tokens.get[tkid].content = malloc(tklen);
         memcpy_s(tokens.get[tkid].content, tklen, tk, tklen);
         if (isdigit(tk[0]))
         {
            tokens.get[tkid].type = TK_NUM;
         }
         else
         {
            fsgettk(in_stream, tk, TOK_MAX);
            if (tk[0] == ':')
               tokens.get[tkid].type = TK_LBDEC;
            else
            {
               tokens.get[tkid].type = TK_KW;
               continue;  // we have already fetched a new token, so skip the fetching at the end
            }
         }
      }

      fsgettk(in_stream, tk, TOK_MAX);
   }
   return tokens;
}
