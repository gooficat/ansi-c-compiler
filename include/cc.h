#pragma once

#define LINE_MAX_LEN 128
#define CHUNK_MAX_LEN 32

cc(const char* source_path, const char* dest_path);

enum
{
   TK_EOF = '\0', /* ♦ */
   TK_EOL = '}',
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
