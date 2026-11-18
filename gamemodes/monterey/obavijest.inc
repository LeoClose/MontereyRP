#define OBAVIJEST_MS        4500
#define OBAVIJEST_SPOJI_MS  1200
#define OBAVIJEST_GRESKA    0xC0392BE6

static PlayerText:gObTd[MAX_PLAYERS][2], bool:gObIma[MAX_PLAYERS], gObToken[MAX_PLAYERS], gObOd[MAX_PLAYERS], gObTekst[MAX_PLAYERS][256];

IgraObavijestConnect(playerid)
{
    gObIma[playerid] = false;
    gObToken[playerid]++;
    gObOd[playerid] = 0;
}

IgraObavijest(playerid, const naslov[], boja, const tekst[], ms = OBAVIJEST_MS)
{
    new i = strfind(tekst, "] ");
    if (tekst[0] == '[' && i != -1) i += 2;
    else i = 0;
    if (gObOd[playerid] && GetTickCount() - gObOd[playerid] < OBAVIJEST_SPOJI_MS && strlen(gObTekst[playerid]) + strlen(tekst[i]) < sizeof gObTekst[] - 4) strcat(gObTekst[playerid], "~n~");
    else gObTekst[playerid][0] = EOS;
    strcat(gObTekst[playerid], tekst[i]);
    gObOd[playerid] = GetTickCount();
    ObPrikazi(playerid, naslov, boja, gObTekst[playerid]);
    if (ms > 0) SetTimerEx("IgraObavijestSkrij", ms, false, "ii", playerid, gObToken[playerid]);
    return 1;
}

IgraObavijestPanel(playerid, const naslov[], boja, const tekst[])
{
    gObOd[playerid] = 0;
    return ObPrikazi(playerid, naslov, boja, tekst);
}

IgraObavijestUkloni(playerid)
{
    IgraObavijestSkrij(playerid, ++gObToken[playerid]);
}

static ObPrikazi(playerid, const naslov[], boja, const tekst[])
{
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
    PlayerTextDrawBoxColour(playerid, gObTd[playerid][0], boja);
    PlayerTextDrawSetString(playerid, gObTd[playerid][0], naslov);
    PlayerTextDrawSetString(playerid, gObTd[playerid][1], tekst);
    PlayerTextDrawShow(playerid, gObTd[playerid][0]);
    PlayerTextDrawShow(playerid, gObTd[playerid][1]);
    gObToken[playerid]++;
    return 1;
}

forward IgraObavijestSkrij(playerid, token);
public IgraObavijestSkrij(playerid, token)
{
    if (token != gObToken[playerid] || !gObIma[playerid]) return;
    gObOd[playerid] = 0;
    PlayerTextDrawHide(playerid, gObTd[playerid][0]);
    PlayerTextDrawHide(playerid, gObTd[playerid][1]);
}
