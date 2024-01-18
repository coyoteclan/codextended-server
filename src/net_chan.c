#include "server.h"

Netchan_Setup_t Netchan_Setup = (Netchan_Setup_t)0x808119C;
#if CODPATCH == 1
NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;
#else
NET_OutOfBandPrint_t NET_OutOfBandPrint = (NET_OutOfBandPrint_t)0x8080920;
#endif
NET_SendPacket_t NET_SendPacket = (NET_SendPacket_t)0x8080D28;

#define MAX_MSGLEN              32768

#if CODPATCH == 1
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x8080C38;
#else if CODPATCH == 5
NET_StringToAdr_t NET_StringToAdr = (NET_StringToAdr_t)0x80844E0;
#endif

const char  *NET_AdrToString (netadr_t a)
{
    static char s[64];

    if (a.type == NA_LOOPBACK)
    {
        Com_sprintf (s, sizeof(s), "loopback");
    }
    else if (a.type == NA_BOT)
    {
        Com_sprintf (s, sizeof(s), "bot");
    }
    else if (a.type == NA_IP)
    {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i:%hu",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3], BigShort(a.port));
    }
    else
    {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x:%hu",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9], 
        BigShort(a.port));
    }
    return s;
}

const char  *NET_BaseAdrToString (netadr_t a)
{
    static char s[64];

    if (a.type == NA_LOOPBACK)
    {
        Com_sprintf (s, sizeof(s), "loopback");
    }
    else if (a.type == NA_BOT)
    {
        Com_sprintf (s, sizeof(s), "bot");
    }
    else if (a.type == NA_IP)
    {
        Com_sprintf (s, sizeof(s), "%i.%i.%i.%i",
            a.ip[0], a.ip[1], a.ip[2], a.ip[3]);
    }
    else
    {
        Com_sprintf (s, sizeof(s), "%02x%02x%02x%02x.%02x%02x%02x%02x%02x%02x",
        a.ipx[0], a.ipx[1], a.ipx[2], a.ipx[3], a.ipx[4], a.ipx[5], a.ipx[6], a.ipx[7], a.ipx[8], a.ipx[9]);
    }
    return s;
}

qboolean NET_CompareAdr( netadr_t a, netadr_t b )
{
    if ( a.type != b.type )
    {
        return qfalse;
    }
    if ( a.type == NA_LOOPBACK )
    {
        return qtrue;
    }
    if ( a.type == NA_IP )
    {
        if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] && a.port == b.port ) {
            return qtrue;
        }
        return qfalse;
    }
    if ( a.type == NA_IPX )
    {
        if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) && a.port == b.port ) {
            return qtrue;
        }
        return qfalse;
    }
    Com_Printf( "NET_CompareAdr: bad address type\n" );
    return qfalse;
}

qboolean NET_CompareBaseAdr( netadr_t a, netadr_t b )
{
    if ( a.type != b.type )
    {
        return qfalse;
    }
    if ( a.type == NA_LOOPBACK )
    {
        return qtrue;
    }
    if ( a.type == NA_IP )
    {
        if ( a.ip[0] == b.ip[0] && a.ip[1] == b.ip[1] && a.ip[2] == b.ip[2] && a.ip[3] == b.ip[3] )
        {
            return qtrue;
        }
        return qfalse;
    }
    if ( a.type == NA_IPX )
    {
        if ( ( memcmp( a.ipx, b.ipx, 10 ) == 0 ) )
        {
            return qtrue;
        }
        return qfalse;
    }
    Com_Printf( "NET_CompareBaseAdr: bad address type\n" );
    return qfalse;
}