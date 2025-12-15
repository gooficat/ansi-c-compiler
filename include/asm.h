#pragma once
#include "arr.h"
#include "defs.h"

#define TOK_MAX 32
#define MAX_ARGS 3

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

enu_m{
    SZBYTE = 1,   //
    SZWORD = 2,   //
    SZDWORD = 4,  //
    SZQWORD = 8,  //
} bytes_e;

enu_m{
    GENREG,  //
    PTRREG,  //
    INDREG,  //
    SEGREG   //
} regtype_e;

struc_t
{
   const str name;
   regtype_e type;
   bytes_e len;
   u8 code;
}
register_s;

extern register_s registers[];
