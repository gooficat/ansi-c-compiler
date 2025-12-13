#pragma once

#define LINE_MAX_LEN 128
#define CHUNK_MAX_LEN 32

cc(const char* source_path, const char* dest_path);

struct tokenized_unit
{
   char* data;
   unsigned long length;
   unsigned short powmul;
   char** idents;
   unsigned char num_idents; /*256 may be limiting. reconsider TODO*/
};

enum
{
   TK_EOF = '\0', /* ♦ */
   TK_BKL = '{',
   TK_BKR = '}',
   TK_BRL = '(',
   TK_BRR = ')',
   TK_IDT = '%',
   TK_TYP = '#'
};
struct type
{
   const char* name;
   unsigned char size;
};
