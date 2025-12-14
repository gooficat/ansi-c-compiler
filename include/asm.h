#pragma once
#include "arr.h"
#include "defs.h"

#define TOK_MAX 32

void assemble(const str path);

typedef u8 byte;
typedef array_type(byte) bytearr_t;

struc_t
{
   char name[TOK_MAX];
   u64 address;
}
label_s;

struc_t
{
   bytearr_t bytes;
   array_type(label_s) labels;
}
asm_unit_s;