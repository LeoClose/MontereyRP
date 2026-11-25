forward bool:ObavijestStane(playerid, const tekst[]);
#define PORUKA_POMOC        0x8AB4F8FF
#define PORUKA_INFO_MAX     110

bool:Poruka(playerid, boja, const fmt[], {Float, _}:...)
{
    static out[256];
    new n = (numargs() - 3) * 4;
    if (n)
    {
        new arg_start, arg_end;
        #emit CONST.alt     fmt
        #emit LCTRL         5
        #emit ADD
        #emit STOR.S.pri    arg_start
        #emit LOAD.S.alt    n
        #emit ADD
        #emit STOR.S.pri    arg_end
        while (arg_end > arg_start)
        {
            #emit LOAD.S.pri    arg_end
            #emit LOAD.I
            #emit PUSH.pri
            arg_end -= 4;
        }
        #emit PUSH.S        fmt
        #emit PUSH.C        256
        #emit PUSH.C        out
        n += 12;
        #emit PUSH.S        n
        #emit SYSREQ.C      format
        n += 4;
        #emit LCTRL         4
        #emit LOAD.S.alt    n
        #emit ADD
        #emit SCTRL         4
    }
    else format(out, sizeof out, "%s", fmt);
    if (playerid >= 0 && playerid < MAX_PLAYERS && gUlogiran[playerid])
    {
        if (boja == 0xFF6347FF) return bool:ObavijestGreska(playerid, out);
        if (boja == PORUKA_POMOC) return bool:IgraObavijest(playerid, T(playerid, "Pomoc"), 0x2E86C1E6, out);
        if (boja == 0xCCCCCCFF && strlen(out) <= PORUKA_INFO_MAX && ObavijestStane(playerid, out)) return bool:ObavijestInfo(playerid, T(playerid, "Obavijest"), out);
    }
    return SendClientMessage(playerid, boja, "%s", out);
}

#define SendClientMessage Poruka
