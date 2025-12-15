#include <stdint.h>
#include "fancc.h"
//
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

enu_m{
    ARG_IMM,  //
    ARG_REG,  //
    ARG_MEM   //
} argtype_e;

struc_t
{
   argtype_e type;  //
   bytes_e size;
   u64 val;
   u64 offset;  // more needed
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
   unit.bytes.len = 0;
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

void encode_statement(str mnem, arg_s args[], u8 st_argc, asm_unit_s ptr unit,
                      bool place_output)  // this boolean corresponds with mark labels
{
   switch (st_argc)
   {
   case 0:
      ++unit->bytes.len;
      break;
   case 1:
      switch (args[0].type)
      {
      case ARG_IMM:
         unit->bytes.len += args[0].size;
         if (place_output)
         {
         }
         break;
      case ARG_REG:
         unit->bytes.len += 1;  // must also add prefix
         if (place_output)
         {
         }
      case ARG_MEM:
         // TODO add memory deref encode
         if (place_output)
         {
         }
         break;
      }
      break;
   case 2:
      if (args[0].type == ARG_REG)
         break;
   case 3:
      break;
   default:
      break;
   }

   unit->bytes.len += 1;  // TODO add instruction profile checking system
}

void gen_bytes(tokenarr_t ptr tokens, asm_unit_s ptr unit, bool mark_labels)
{
   // if mark labels is true we will use the unit's byte array counter to store our current offset,
   // without actually filling in the array though.
   for (u64 i = 0; i < tokens->len; i++)
   {
      SETCOLOR(GREEN);
      if (mark_labels)
         printf("Labeling pass: ");
      else
         printf("Non-labeling pass: ");

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
         }  // 200
         break;
      case TK_KW:
      {
         str mnem = tokens->get[i++].content;
         fancc(BLUE, "call of %s\n", mnem);
         u8 st_argc = 0;
         arg_s args[MAX_ARGS];
         u8 is_eoargs = 2;  // is end of args
         while (is_eoargs)
         {
            if (is_eoargs == 1 or tokens->get[i + 1].type is TK_CMA)
               --is_eoargs;
            if (tokens->get[i].type is TK_NUM)
            {
               args[st_argc].type = ARG_IMM;
               char* eoa =
                   tokens->get[i].content + strlen(tokens->get[i].content);  // end of argument
               u64 asiz = strtoull(tokens->get[i].content, &eoa, 10);

               args[st_argc].val = asiz;
               i += 2;
               fancc(YELLOW, "arg of %llu\n", args[st_argc].val);
               if (asiz <= UINT64_MAX)
                  args[st_argc].size = SZQWORD;
               elif (asiz <= UINT32_MAX) args[st_argc].size = SZDWORD;
               elif (asiz <= UINT16_MAX) args[st_argc].size = SZWORD;
               else args[st_argc].size = SZBYTE;
               ++st_argc;
            }
            else
            {
               i += 2;
            }
         }
         fancc(PURPLE, "Mnemonic %s\n", mnem);
         encode_statement(mnem, args, st_argc, unit, mark_labels);
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
