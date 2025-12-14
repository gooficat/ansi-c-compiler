#pragma once
#include <stdbool.h>
#include <stdio.h>
#include "defs.h"

struc_t
{
   FILE ptr file;
   int buffer;
}
filestream_s;

extern const str FMODE_RT;
extern const str FMODE_RB;
extern const str FMODE_WT;
extern const str FMODE_WB;

#define fsgetc(f) (f).buffer = fgetc((f).file)

void fsgettk(filestream_s ptr stream, str tkbuf, u8 maxlen);