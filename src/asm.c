#include "asm.h"
#include "fancc.h"
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

enu_m{
    ARG_IMM,  //
} argtype_e;

struc_t
{
   argtype_e type;  //
   bytes_e size;
   u64 val;
}
arg_s;

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
   gen_bytes(addr tokens, addr unit,
             false);  // I decided this was less confusing than just having it call itself again
                      // without the flag
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
      if (mark_labels)
         fancc(GREEN, "Labeling pass: ");
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
         }  // 130
         break;
      case TK_KW:
      {
         char* mnem = tokens->get[i].content;
         u64 st_beg = ++i;
         u8 st_argc = 0;
         arg_s args[MAX_ARGS];
         while (tokens->get[i + 1].type is TK_CMA)
         {
            if (tokens->get[i].type is TK_NUM)
            {
               args[st_argc].type = ARG_IMM;
               // args[st_argc].size = ;
               char* eoa = tokens->get[i].content + strlen(tokens->get[i].content);
               args[st_argc].val = strtoull(tokens->get[i].content, &eoa, 10);
               i += 2;
               fancc(YELLOW, "%llu\n", args[st_argc].val);
            }
            else
            {
               i += 2;
            }
         }

         switch (st_argc)
         {
         case 0:
            break;
         case 1:
            break;
         case 2:
            break;
         case 3:
            break;
         default:
            break;
         }

         fancc(BLUE, "call of %s with %hhu args, first arg at %c\n", mnem, st_argc);
      }
      break;
      case TK_CMT:
         fancc(FOREGROUND_INTENSITY | WHITE, "comment\n");
         ++i;
         while (tokens->get[i].type notis TK_CMT)
            ++i;
         break;
      default:
         fancc(RED, "Unhandled type %c\n", tokens->get[i].type);
         break;
      }
   }
   fancc(RED, "Byte pass %scomplete\n\n", mark_labels ? "with labeling " : "");
}
