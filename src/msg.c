#include "shared.h"

#if CODPATCH == 1
MSG_BeginReading_t MSG_BeginReading = (MSG_BeginReading_t)0x807F174;
MSG_ReadLong_t MSG_ReadLong = (MSG_ReadLong_t)0x807F2F0;
MSG_ReadStringLine_t MSG_ReadStringLine = (MSG_ReadStringLine_t)0x807F3FC;
MSG_ReadString_t MSG_ReadString = (MSG_ReadString_t)0x807F320;
#else
MSG_BeginReading_t MSG_BeginReading = (MSG_BeginReading_t)0x807F96B;
MSG_ReadLong_t MSG_ReadLong = (MSG_ReadLong_t)0x80800E8;
MSG_ReadStringLine_t MSG_ReadStringLine = (MSG_ReadStringLine_t)0x8080212;
#endif

/*
============================================================================

                    BYTE ORDER FUNCTIONS

============================================================================
*/

#ifdef _SGI_SOURCE
#define __BIG_ENDIAN__
#endif

#ifdef __BIG_ENDIAN__
short   BigShort( short l )
{
    return l;
}
#else
short   BigShort( short l )
{
    byte b1,b2;
    b1 = l & 255;
    b2 = ( l >> 8 ) & 255;
    return ( b1 << 8 ) + b2;
}
#endif