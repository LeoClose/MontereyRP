#define OBAVIJEST_MS        4500
#define OBAVIJEST_SPOJI_MS  1200
#define OBAVIJEST_GRESKA    0xC0392BE6
#define OBAVIJEST_ZVUK      1085

forward bool:DrogaKuha(playerid);
forward bool:KazinoAktivan(playerid);
forward bool:PokerAktivan(playerid);
forward bool:PcOtvoren(playerid);

static bool:gObDesno[MAX_PLAYERS], PlayerText:gObTd[MAX_PLAYERS][2], bool:gObVidi[MAX_PLAYERS], gObZadnji[MAX_PLAYERS][256], bool:gObIma[MAX_PLAYERS], gObToken[MAX_PLAYERS], gObOd[MAX_PLAYERS], gObTekst[MAX_PLAYERS][256];

bool:ObavijestStane(playerid, const tekst[])
{
    return !(gObOd[playerid] && GetTickCount() - gObOd[playerid] < OBAVIJEST_SPOJI_MS && strlen(gObTekst[playerid]) + strlen(tekst) >= sizeof gObTekst[] - 4);
}

IgraObavijestConnect(playerid)
{
    gObDesno[playerid] = false;
    gObIma[playerid] = false;
    gObVidi[playerid] = false;
    gObZadnji[playerid][0] = EOS;
    gObToken[playerid]++;
    gObOd[playerid] = 0;
}

IgraObavijest(playerid, const naslov[], boja, const tekst[], ms = OBAVIJEST_MS)
{
    new i = strfind(tekst, "] ");
    if (tekst[0] == '[' && i != -1) i += 2;
    else i = 0;
    if (gObVidi[playerid] && !strcmp(tekst[i], gObZadnji[playerid])) return 1;
    gObZadnji[playerid][0] = EOS;
    strcat(gObZadnji[playerid], tekst[i]);
    PlayerPlaySound(playerid, OBAVIJEST_ZVUK, 0.0, 0.0, 0.0);
    SavjetMakni(playerid);
    if (gObOd[playerid] && GetTickCount() - gObOd[playerid] < OBAVIJEST_SPOJI_MS && strlen(gObTekst[playerid]) + strlen(tekst[i]) < sizeof gObTekst[] - 4) strcat(gObTekst[playerid], "~n~");
    else gObTekst[playerid][0] = EOS;
    strcat(gObTekst[playerid], tekst[i]);
    gObOd[playerid] = GetTickCount();
    ObPrikazi(playerid, naslov, boja, gObTekst[playerid]);
    if (ms > 0) SetTimerEx("IgraObavijestSkrij", ms, false, "ii", playerid, gObToken[playerid]);
    return 1;
}

ObavijestGreska(playerid, const tekst[])
{
    return IgraObavijest(playerid, T(playerid, "Greska"), OBAVIJEST_GRESKA, tekst);
}

ObavijestInfo(playerid, const naslov[], const tekst[])
{
    return IgraObavijest(playerid, naslov, 0x27AE60E6, tekst);
}

IgraObavijestPanel(playerid, const naslov[], boja, const tekst[])
{
    gObOd[playerid] = 0;
    return ObPrikazi(playerid, naslov, boja, tekst);
}

bool:IgraObavijestVidljiva(playerid)
{
    return gObVidi[playerid];
}

IgraObavijestUkloni(playerid)
{
    IgraObavijestSkrij(playerid, ++gObToken[playerid]);
}

static const gObLat[64][] = {
    "A", "B", "V", "G", "D", "E", "Zh", "Z", "I", "Y", "K", "L", "M", "N", "O", "P",
    "R", "S", "T", "U", "F", "Kh", "Ts", "Ch", "Sh", "Sch", "", "Y", "", "E", "Yu", "Ya",
    "a", "b", "v", "g", "d", "e", "zh", "z", "i", "y", "k", "l", "m", "n", "o", "p",
    "r", "s", "t", "u", "f", "kh", "ts", "ch", "sh", "sch", "", "y", "", "e", "yu", "ya"
};

static ObTd(const ulaz[], izlaz[], size)
{
    new n = 0;
    for (new i = 0; ulaz[i] && n < size - 4; i++)
    {
        new c = ulaz[i] & 0xFF;
        if (c == '{' && ulaz[i + 7] == '}')
        {
            i += 7;
            continue;
        }
        if (c >= 0xC0)
        {
            for (new k = 0; gObLat[c - 0xC0][k] && n < size - 4; k++) izlaz[n++] = gObLat[c - 0xC0][k];
            continue;
        }
        if (c == 0xA8) c = 'E';
        else if (c == 0xB8) c = 'e';
        else if (c > 0x7E) continue;
        izlaz[n++] = c;
    }
    izlaz[n] = EOS;
}

static ObPrikazi(playerid, const naslov[], boja, const tekst[])
{
    new n[64], t[512];
    ObTd(naslov, n, sizeof n);
    ObTd(tekst, t, sizeof t);
    if (!gObIma[playerid])
    {
        gObIma[playerid] = true;
        gObTd[playerid][0] = CreatePlayerTextDraw(playerid, 24.0, 150.0, "_");
        PlayerTextDrawFont(playerid, gObTd[playerid][0], TEXT_DRAW_FONT_2);
        PlayerTextDrawLetterSize(playerid, gObTd[playerid][0], 0.24, 1.1);
        PlayerTextDrawTextSize(playerid, gObTd[playerid][0], 176.0, 0.0);
        PlayerTextDrawUseBox(playerid, gObTd[playerid][0], true);
        PlayerTextDrawColour(playerid, gObTd[playerid][0], 0xFFFFFFFF);
        PlayerTextDrawSetShadow(playerid, gObTd[playerid][0], 0);
        PlayerTextDrawSetOutline(playerid, gObTd[playerid][0], 0);
        gObTd[playerid][1] = CreatePlayerTextDraw(playerid, 24.0, 165.0, "_");
        PlayerTextDrawFont(playerid, gObTd[playerid][1], TEXT_DRAW_FONT_1);
        PlayerTextDrawLetterSize(playerid, gObTd[playerid][1], 0.22, 1.05);
        PlayerTextDrawTextSize(playerid, gObTd[playerid][1], 176.0, 0.0);
        PlayerTextDrawUseBox(playerid, gObTd[playerid][1], true);
        PlayerTextDrawBoxColour(playerid, gObTd[playerid][1], 0x000000B4);
        PlayerTextDrawColour(playerid, gObTd[playerid][1], 0xF0F0F0FF);
        PlayerTextDrawSetShadow(playerid, gObTd[playerid][1], 0);
        PlayerTextDrawSetOutline(playerid, gObTd[playerid][1], 1);
    }
    new bool:desno = TuningAktivan(playerid) || MdtOtvoren(playerid) || KazinoAktivan(playerid) || PokerAktivan(playerid) || PcOtvoren(playerid) || DrogaKuha(playerid);
    if (desno != gObDesno[playerid])
    {
        gObDesno[playerid] = desno;
        new Float:od = desno ? 464.0 : 24.0;
        PlayerTextDrawSetPos(playerid, gObTd[playerid][0], od, 150.0);
        PlayerTextDrawSetPos(playerid, gObTd[playerid][1], od, 165.0);
        PlayerTextDrawTextSize(playerid, gObTd[playerid][0], od + 152.0, 0.0);
        PlayerTextDrawTextSize(playerid, gObTd[playerid][1], od + 152.0, 0.0);
    }
    PlayerTextDrawBoxColour(playerid, gObTd[playerid][0], boja);
    PlayerTextDrawSetString(playerid, gObTd[playerid][0], n);
    PlayerTextDrawSetString(playerid, gObTd[playerid][1], t);
    PlayerTextDrawShow(playerid, gObTd[playerid][0]);
    PlayerTextDrawShow(playerid, gObTd[playerid][1]);
    gObVidi[playerid] = true;
    gObToken[playerid]++;
    return 1;
}

forward IgraObavijestSkrij(playerid, token);
public IgraObavijestSkrij(playerid, token)
{
    if (token != gObToken[playerid] || !gObIma[playerid]) return;
    gObOd[playerid] = 0;
    gObVidi[playerid] = false;
    gObZadnji[playerid][0] = EOS;
    PlayerTextDrawHide(playerid, gObTd[playerid][0]);
    PlayerTextDrawHide(playerid, gObTd[playerid][1]);
}
