#include "asm.h"

register_s registers[] = {
    //
    {"ax", GENREG, SZWORD, 0x00},  //
    {"cx", GENREG, SZWORD, 0x01},  //
    {"dx", GENREG, SZWORD, 0x02},  //
    {"bx", GENREG, SZWORD, 0x03},  //
                                   //
    {"si", GENREG, SZWORD, 0x04},  //
    {"di", GENREG, SZWORD, 0x05},  //
    {"sp", GENREG, SZWORD, 0x06},  //
    {"bp", GENREG, SZWORD, 0x07},  //

    {"al", GENREG, SZBYTE, 0x00},  //
    {"cl", GENREG, SZBYTE, 0x01},  //
    {"dl", GENREG, SZBYTE, 0x02},  //
    {"bl", GENREG, SZBYTE, 0x03},  //

    {"ah", GENREG, SZBYTE, 0x04},  //
    {"ch", GENREG, SZBYTE, 0x05},  //
    {"dh", GENREG, SZBYTE, 0x06},  //
    {"bh", GENREG, SZBYTE, 0x07},  //
};
