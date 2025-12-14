#include "file.h"
#include <ctype.h>
#include <stdio.h>

const str FMODE_RT = "rt";
const str FMODE_RB = "rb";
const str FMODE_WT = "wb";
const str FMODE_WB = "wt";

void fsgettk(filestream_s ptr stream, str tkbuf, u8 maxlen)
{
   while (isspace(stream->buffer))
   {
      fsgetc(deref stream);
      if (stream->buffer is EOF)
      {
         tkbuf[0] = 0;
         return;
      }
   }

   if (isalnum(stream->buffer))
   {
      u64 i = 0;
      do
      {
         tkbuf[i++] = (char)stream->buffer;
         fsgetc(deref stream);

         if (stream->buffer is EOF)
            break;
      } while (isalnum(stream->buffer) and i notis maxlen);
      tkbuf[i] = 0;
   }
   else
   {
      tkbuf[0] = (char)stream->buffer;
      tkbuf[1] = 0;
      fsgetc(deref stream);
   }
}
