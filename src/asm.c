#include "fancc.h"
#undef addr
#include "asm.h"
#include "file.h"

enu_m{
    TK_REG = '%',    // register
    TK_KW,           // keyword (either a label or an instruction. this will be deduced after
                     // tokenization)
    TK_LBDEC = ':',  // label declaration symbol (a colon)
    TK_CMA = ',',    // a comma (,)
    TK_CMT = ';',    // comment (;). unlike gas, this must be followed by a terminator, another ;
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
void gen_bytes(tokenarr_t ptr tokens, asm_unit_s ptr unit, bool mark_labels);

void assemble(const str path)
{
   // fancc(BLUE, "%s\n", path);
   filestream_s in_stream;
   fopen_s(&in_stream.file, path, FMODE_RT);
   tokenarr_t tokens = fetch_tokens(addr in_stream);
   fclose(in_stream.file);

   asm_unit_s unit = {mk_arr(byte), mk_arr(label_s)};
   gen_bytes(addr tokens, addr unit, true);
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
      // fancc(GREEN, "%s\n", tk);
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

void gen_bytes(tokenarr_t ptr tokens, asm_unit_s ptr unit, bool mark_labels)
{
   for (u64 i = 0; i < tokens->len; i++)
   {
      switch (tokens->get[i].type)
      {
      case TK_LBDEC:
         fancc(YELLOW, "label: %s\n", tokens->get[i].content);
         if (mark_labels)
         {
            rsz_arr(unit->labels, label_s, 1);
            label_s label;
            label.address = unit->bytes.len;
            strcpy_s(label.name, TOK_MAX, tokens->get[i].content);

            unit->labels.get[unit->labels.len - 1] = label;
         }
         break;
      case TK_KW:
      {
         char* mnem = tokens->get[i].content;
         u64 st_beg = ++i;
         while (tokens->get[i + 1].type is TK_CMA)
            i += 2;
         u8 st_argc = (i - st_beg) / 2 + 1;  // I do not know why it works, but do not touch it
         fancc(BLUE, "Call of %s with %hhu args\n", mnem, st_argc);
         break;
      }
      case TK_CMT:
         ++i;
         while (tokens->get[i].type notis TK_CMT)
            ++i;
         break;
      default:
         fancc(RED, "Unhandled type %c\n", tokens->get[i].type);
         break;
      }
   }
}
