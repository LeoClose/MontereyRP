#define PIKADO_MAX          16
#define PIKADO_DOMET        1.5
#define PIKADO_NAPUSTI      10.0
#define PIKADO_POCETNO      301
#define PIKADO_CIJENA       5
#define PIKADO_STRELICA     3
#define PIKADO_NISAN_MS     50
#define PIKADO_POTEZ_MS     1500
#define PIKADO_CX           320.0
#define PIKADO_CY           200.0
#define PIKADO_R            120.0
#define PIKADO_AX           0.8
#define PIKADO_RASIPANJE    5
#define PIKADO_POMAK        5.0
#define PIKADO_IDEAL        75.0
#define PIKADO_IDEAL_OD     55.0
#define PIKADO_IDEAL_DO     90.0
#define PIKADO_TOCNO        5.5
#define PIKADO_BAR_X        240.0
#define PIKADO_BAR_Y        408.0
#define PIKADO_BAR_W        160.0
#define PIKADO_PLOCA        40
#define PIKADO_LOPTA        1.032
#define PIKADO_PRSTEN_CRVENA 0xFF2A1FFF
#define PIKADO_PRSTEN_ZELENA 0x22E05AFF
#define PIKADO_TD           13
#define PIKADO_KREM         0xF2EBD9FF
#define PIKADO_CRNA         0x1C1C1CFF
#define PIKADO_CRVENA       0xC0392BFF
#define PIKADO_ZELENA       0x1E8449FF

enum { PF_CEKA, PF_IGRA, PF_PAUZA }
enum { PTD_NISAN, PTD_STRELICA = PTD_NISAN + 3, PTD_INFO = PTD_STRELICA + PIKADO_STRELICA, PTD_BAR, PTD_SNAGA, PTD_ZONA, PTD_POLJE, PTD_BROJ }
enum E_PIKADO { bool:pkIma, pkDb, Float:pkX, Float:pkY, Float:pkZ, Float:pkA, pkInt, pkSvijet, STREAMER_TAG_3D_TEXT_LABEL:pkTekst,
    pkFaza, pkIgrac[2], pkBodovi[2], pkNaPotezu, pkBacio, pkPrije, pkNisani, Float:pkAx, Float:pkAy, Float:pkHx[PIKADO_STRELICA], Float:pkHy[PIKADO_STRELICA], pkToken, bool:pkSam, pkUkupno,
    Float:pkSx, Float:pkSy, Float:pkSnaga, bool:pkGore, bool:pkDrzi, pkNisanOd, pkPolje, Float:pkIdeal }

static const gPikadoBroj[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };

static gPikado[PIKADO_MAX][E_PIKADO], gPikadoIgrac[MAX_PLAYERS] = { -1, ... }, gApPikado[MAX_PLAYERS][PIKADO_MAX];
static PlayerText:gPkTd[MAX_PLAYERS][PIKADO_TD], bool:gPkTdIma[MAX_PLAYERS];
static Text:gPikadoPloca[PIKADO_PLOCA], gPikadoPlocaN;

PikadoBodovi(Float:x, Float:y)
{
    new Float:r = VectorSize(x, y, 0.0);
    if (r <= 6.35) return 50;
    if (r <= 15.9) return 25;
    if (r > 170.0) return 0;
    new Float:a = atan2(x, y);
    if (a < 0.0) a += 360.0;
    new b = gPikadoBroj[floatround((a + 9.0) / 18.0, floatround_floor) % 20];
    if (r > 162.0) return b * 2;
    if (r > 99.0 && r <= 107.0) return b * 3;
    return b;
}

static PikadoNaEkran(Float:mx, Float:my, &Float:px, &Float:py)
{
    new Float:k = PIKADO_R / 170.0;
    px = PIKADO_CX + mx * k * PIKADO_AX;
    py = PIKADO_CY - my * k;
}

static PikadoBrojPoz(i, &Float:px, &Float:py)
{
    new Float:k = PIKADO_R / 170.0, Float:a = float(i) * 18.0;
    px = PIKADO_CX + 186.0 * k * PIKADO_AX * floatsin(a, degrees);
    py = PIKADO_CY - 186.0 * k * floatcos(a, degrees) - 6.0;
}

static PikadoKrugPloce(Float:rmm, boja)
{
    if (gPikadoPlocaN >= PIKADO_PLOCA) return;
    new Float:r = rmm * PIKADO_R / 170.0 * PIKADO_LOPTA;
    new Text:td = TextDrawCreate(PIKADO_CX - r * PIKADO_AX, PIKADO_CY - r, "LD_POOL:ball");
    TextDrawFont(td, TEXT_DRAW_FONT_SPRITE_DRAW);
    TextDrawTextSize(td, r * 2.0 * PIKADO_AX, r * 2.0);
    TextDrawColour(td, boja);
    gPikadoPloca[gPikadoPlocaN++] = td;
}

static PikadoPlocaStvori()
{
    new broj[4], Text:td = TextDrawCreate(0.0, 0.0, "_");
    TextDrawLetterSize(td, 0.0, 50.0);
    TextDrawUseBox(td, true);
    TextDrawBoxColour(td, 0x000000A0);
    TextDrawTextSize(td, 640.0, 0.0);
    gPikadoPloca[gPikadoPlocaN++] = td;
    PikadoKrugPloce(200.0, 0x111111FF);
    PikadoKrugPloce(170.0, PIKADO_PRSTEN_CRVENA);
    PikadoKrugPloce(162.0, PIKADO_KREM);
    PikadoKrugPloce(107.0, PIKADO_PRSTEN_ZELENA);
    PikadoKrugPloce(99.0, PIKADO_CRNA);
    PikadoKrugPloce(15.9, PIKADO_ZELENA);
    PikadoKrugPloce(6.35, PIKADO_CRVENA);
    for (new i = 0; i < 20 && gPikadoPlocaN < PIKADO_PLOCA; i++)
    {
        new Float:px, Float:py;
        PikadoBrojPoz(i, px, py);
        valstr(broj, gPikadoBroj[i]);
        td = TextDrawCreate(px, py, broj);
        TextDrawFont(td, TEXT_DRAW_FONT_2);
        TextDrawLetterSize(td, 0.26, 1.15);
        TextDrawAlignment(td, TEXT_DRAW_ALIGN_CENTER);
        TextDrawColour(td, 0xF5F5F5FF);
        TextDrawSetOutline(td, 0);
        TextDrawSetShadow(td, 0);
        gPikadoPloca[gPikadoPlocaN++] = td;
    }
}

static PikadoPolje(Float:x, Float:y, tekst[], size = sizeof tekst)
{
    new Float:r = VectorSize(x, y, 0.0);
    if (r <= 6.35) return format(tekst, size, "~r~BULL"), 1;
    if (r <= 15.9) return format(tekst, size, "~g~25"), 2;
    if (r > 170.0) return format(tekst, size, "~w~-"), 3;
    new Float:a = atan2(x, y);
    if (a < 0.0) a += 360.0;
    new i = floatround((a + 9.0) / 18.0, floatround_floor) % 20, b = gPikadoBroj[i];
    if (r > 162.0) return format(tekst, size, "~r~D%d", b), 100 + i;
    if (r > 99.0 && r <= 107.0) return format(tekst, size, "~g~T%d", b), 200 + i;
    return format(tekst, size, "~w~%d", b), 300 + i;
}

InitPikado()
{
    assert(PikadoBodovi(0.0, 0.0) == 50 && PikadoBodovi(0.0, 10.0) == 25 && PikadoBodovi(0.0, 103.0) == 60 && PikadoBodovi(0.0, 166.0) == 40);
    assert(PikadoBodovi(0.0, 130.0) == 20 && PikadoBodovi(0.0, 200.0) == 0 && PikadoBodovi(103.0, 0.0) == 18 && PikadoBodovi(0.0, -130.0) == 3);
    PikadoPlocaStvori();
    assert(gPikadoPlocaN < PIKADO_PLOCA);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS dart_boards (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM dart_boards LIMIT %d", PIKADO_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gPikado[s][pkDb] = DB_GetFieldInt(res, 0);
        gPikado[s][pkX] = DB_GetFieldFloat(res, 1);
        gPikado[s][pkY] = DB_GetFieldFloat(res, 2);
        gPikado[s][pkZ] = DB_GetFieldFloat(res, 3);
        gPikado[s][pkA] = DB_GetFieldFloat(res, 4);
        gPikado[s][pkInt] = DB_GetFieldInt(res, 5);
        gPikado[s][pkSvijet] = DB_GetFieldInt(res, 6);
        PikadoStvori(s);
    }
    DB_FreeResultSet(res);
}

PikadoConnect(playerid)
{
    gPikadoIgrac[playerid] = -1;
    gPkTdIma[playerid] = false;
}

static PikadoOznaka()
{
    new tekst[48];
    format(tekst, sizeof tekst, "Pikado\n{FFFFFF}$%d", PIKADO_CIJENA);
    return tekst;
}

static PikadoStvori(s)
{
    gPikado[s][pkIma] = true;
    gPikado[s][pkIgrac][0] = gPikado[s][pkIgrac][1] = INVALID_PLAYER_ID;
    gPikado[s][pkNisani] = INVALID_PLAYER_ID;
    gPikado[s][pkFaza] = PF_CEKA;
    gPikado[s][pkTekst] = CreateDynamic3DTextLabel(PikadoOznaka(), 0xF39C12FF, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.2, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gPikado[s][pkSvijet], gPikado[s][pkInt]);
    InterakcijaOznaka(gPikado[s][pkTekst]);
}

static PikadoPoruka(s, const tekst[], igrac = INVALID_PLAYER_ID, a = 0, b = 0)
{
    for (new i = 0; i < 2; i++)
    {
        new p = gPikado[s][pkIgrac][i];
        if (p == INVALID_PLAYER_ID) continue;
        new poruka[160];
        format(poruka, sizeof poruka, T(p, tekst), igrac != INVALID_PLAYER_ID ? GetRPIme(igrac) : "", a, b);
        IgraObavijest(p, T(p, "Pikado"), PIKADO_ZELENA, poruka);
    }
}

static PikadoTdUnisti(playerid)
{
    if (!gPkTdIma[playerid]) return;
    gPkTdIma[playerid] = false;
    for (new i = 0; i < PIKADO_TD; i++) PlayerTextDrawDestroy(playerid, gPkTd[playerid][i]);
    for (new i = 0; i < gPikadoPlocaN; i++) TextDrawHideForPlayer(playerid, gPikadoPloca[i]);
}

static PlayerText:PikadoPravokutnik(playerid, Float:w, Float:h, boja)
{
    return TdSlika(playerid, -50.0, -50.0, w, h, boja);
}

static PlayerText:PikadoTocka(playerid, Float:r, boja)
{
    return TdSlika(playerid, -50.0, -50.0, r * 2.0, r * 2.0, boja, "LD_BEAT:chit");
}

static PikadoTdStvori(playerid)
{
    PikadoTdUnisti(playerid);
    gPkTdIma[playerid] = true;
    for (new i = 0; i < gPikadoPlocaN; i++) TextDrawShowForPlayer(playerid, gPikadoPloca[i]);
    for (new d = 0; d < PIKADO_STRELICA; d++) gPkTd[playerid][PTD_STRELICA + d] = PikadoTocka(playerid, 3.5, 0xF1C40FFF);
    gPkTd[playerid][PTD_NISAN] = PikadoPravokutnik(playerid, 18.0 * PIKADO_AX, 1.5, 0x00E5FFFF);
    gPkTd[playerid][PTD_NISAN + 1] = PikadoPravokutnik(playerid, 1.5 * PIKADO_AX, 18.0, 0x00E5FFFF);
    gPkTd[playerid][PTD_NISAN + 2] = PikadoTocka(playerid, 1.6, 0xFFFFFFFF);
    gPkTd[playerid][PTD_BAR] = CreatePlayerTextDraw(playerid, PIKADO_BAR_X - 2.0, PIKADO_BAR_Y - 2.0, "LD_SPAC:white");
    gPkTd[playerid][PTD_SNAGA] = CreatePlayerTextDraw(playerid, PIKADO_BAR_X, PIKADO_BAR_Y, "LD_SPAC:white");
    gPkTd[playerid][PTD_ZONA] = CreatePlayerTextDraw(playerid, PIKADO_BAR_X + (PIKADO_IDEAL - PIKADO_TOCNO) / 100.0 * PIKADO_BAR_W, PIKADO_BAR_Y, "LD_SPAC:white");
    for (new i = PTD_BAR; i <= PTD_ZONA; i++) PlayerTextDrawFont(playerid, gPkTd[playerid][i], TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, gPkTd[playerid][PTD_BAR], PIKADO_BAR_W + 4.0, 12.0);
    PlayerTextDrawColour(playerid, gPkTd[playerid][PTD_BAR], 0x000000AA);
    PlayerTextDrawColour(playerid, gPkTd[playerid][PTD_SNAGA], 0xF1C40FFF);
    PlayerTextDrawTextSize(playerid, gPkTd[playerid][PTD_ZONA], PIKADO_TOCNO * 2.0 / 100.0 * PIKADO_BAR_W, 8.0);
    PlayerTextDrawColour(playerid, gPkTd[playerid][PTD_ZONA], 0x2ECC7199);
    PlayerTextDrawShow(playerid, gPkTd[playerid][PTD_BAR]);
    gPkTd[playerid][PTD_POLJE] = CreatePlayerTextDraw(playerid, PIKADO_CX, PIKADO_CY - 190.0, "_");
    PlayerTextDrawFont(playerid, gPkTd[playerid][PTD_POLJE], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, gPkTd[playerid][PTD_POLJE], 0.45, 1.8);
    PlayerTextDrawAlignment(playerid, gPkTd[playerid][PTD_POLJE], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetOutline(playerid, gPkTd[playerid][PTD_POLJE], 1);
    PlayerTextDrawShow(playerid, gPkTd[playerid][PTD_POLJE]);
    gPkTd[playerid][PTD_BROJ] = CreatePlayerTextDraw(playerid, -50.0, -50.0, "_");
    PlayerTextDrawFont(playerid, gPkTd[playerid][PTD_BROJ], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, gPkTd[playerid][PTD_BROJ], 0.26, 1.15);
    PlayerTextDrawAlignment(playerid, gPkTd[playerid][PTD_BROJ], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawColour(playerid, gPkTd[playerid][PTD_BROJ], 0xF1C40FFF);
    PlayerTextDrawSetOutline(playerid, gPkTd[playerid][PTD_BROJ], 0);
    PlayerTextDrawSetShadow(playerid, gPkTd[playerid][PTD_BROJ], 0);
    gPkTd[playerid][PTD_INFO] = CreatePlayerTextDraw(playerid, PIKADO_CX, PIKADO_CY + 168.0, "_");
    PlayerTextDrawFont(playerid, gPkTd[playerid][PTD_INFO], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, gPkTd[playerid][PTD_INFO], 0.24, 1.1);
    PlayerTextDrawAlignment(playerid, gPkTd[playerid][PTD_INFO], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetOutline(playerid, gPkTd[playerid][PTD_INFO], 1);
    PlayerTextDrawShow(playerid, gPkTd[playerid][PTD_INFO]);
}

static PikadoPomakni(playerid, PlayerText:td, Float:px, Float:py, Float:w, Float:h)
{
    PlayerTextDrawHide(playerid, td);
    PlayerTextDrawSetPos(playerid, td, px - w / 2.0, py - h / 2.0);
    PlayerTextDrawShow(playerid, td);
}


static PikadoTraka(playerid, i, Float:udio, Float:h)
{
    if (udio <= 0.0)
    {
        PlayerTextDrawHide(playerid, gPkTd[playerid][i]);
        return;
    }
    PlayerTextDrawTextSize(playerid, gPkTd[playerid][i], udio * PIKADO_BAR_W, h);
    if (i == PTD_SNAGA && gPikadoIgrac[playerid] != -1) PlayerTextDrawColour(playerid, gPkTd[playerid][i], floatabs(udio * 100.0 - gPikado[gPikadoIgrac[playerid]][pkIdeal]) <= PIKADO_TOCNO ? 0x2ECC71FF : 0xF1C40FFF);
    PlayerTextDrawShow(playerid, gPkTd[playerid][i]);
}

static PikadoNisanCrtaj(playerid, Float:mx, Float:my)
{
    new Float:px, Float:py;
    PikadoNaEkran(mx, my, px, py);
    PikadoPomakni(playerid, gPkTd[playerid][PTD_NISAN], px, py, 18.0 * PIKADO_AX, 1.5);
    PikadoPomakni(playerid, gPkTd[playerid][PTD_NISAN + 1], px, py, 1.5 * PIKADO_AX, 18.0);
    PikadoPomakni(playerid, gPkTd[playerid][PTD_NISAN + 2], px, py, 3.2, 3.2);
}

static PikadoStrelicaCrtaj(playerid, d, Float:mx, Float:my)
{
    new Float:px, Float:py;
    PikadoNaEkran(mx, my, px, py);
    PikadoPomakni(playerid, gPkTd[playerid][PTD_STRELICA + d], px, py, 7.0, 7.0);
}

static PikadoInfo(s, playerid)
{
    new tekst[256], n = gPikado[s][pkNaPotezu];
    if (gPikado[s][pkSam]) format(tekst, sizeof tekst, T(playerid, "~w~Preostalo: ~y~%d~w~  |  Strelica: %d/%d  |  Ukupno bacanja: %d~n~WASD nisan, drzi/pusti LMB baci, RMB izlaz"),
        gPikado[s][pkBodovi][n], gPikado[s][pkBacio], PIKADO_STRELICA, gPikado[s][pkUkupno]);
    else format(tekst, sizeof tekst, T(playerid, "~w~%s: ~y~%d~w~  |  %s: ~y~%d~n~~w~Strelica: %d/%d~n~WASD nisan, drzi/pusti LMB baci, RMB izlaz"),
        GetRPIme(gPikado[s][pkIgrac][n]), gPikado[s][pkBodovi][n], GetRPIme(gPikado[s][pkIgrac][1 - n]), gPikado[s][pkBodovi][1 - n], gPikado[s][pkBacio], PIKADO_STRELICA);
    PlayerTextDrawSetString(playerid, gPkTd[playerid][PTD_INFO], tekst);
}

static PikadoNisanKraj(s)
{
    new p = gPikado[s][pkNisani];
    gPikado[s][pkNisani] = INVALID_PLAYER_ID;
    gPikado[s][pkDrzi] = false;
    if (p == INVALID_PLAYER_ID || !IsPlayerConnected(p)) return;
    PikadoTdUnisti(p);
    TogglePlayerControllable(p, true);
}

static PikadoKraj(s)
{
    gPikado[s][pkToken]++;
    PikadoNisanKraj(s);
    for (new i = 0; i < 2; i++)
    {
        if (gPikado[s][pkIgrac][i] != INVALID_PLAYER_ID) gPikadoIgrac[gPikado[s][pkIgrac][i]] = -1;
        gPikado[s][pkIgrac][i] = INVALID_PLAYER_ID;
    }
    gPikado[s][pkFaza] = PF_CEKA;
    gPikado[s][pkSam] = false;
    OkladaZatvori(OK_PIKADO, s);
}

static PikadoPobjeda(s, pobjednik)
{
    new p = gPikado[s][pkIgrac][pobjednik], w[1];
    if (gPikado[s][pkSam])
    {
        if (p != INVALID_PLAYER_ID) PikadoPoruka(s, "[Pikado] %s je zavrsio 301 u %d strelica!", p, gPikado[s][pkUkupno]);
        PikadoKraj(s);
        return;
    }
    if (p != INVALID_PLAYER_ID) PikadoPoruka(s, "[Pikado] %s je pobijedio!", p);
    w[0] = p;
    OkladaIsplati(OK_PIKADO, s, w, p != INVALID_PLAYER_ID);
    PikadoKraj(s);
}

PikadoOdlazi(playerid)
{
    new s = gPikadoIgrac[playerid];
    if (s == -1) return;
    new i = gPikado[s][pkIgrac][0] == playerid ? 0 : 1;
    if (gPikado[s][pkFaza] != PF_CEKA && gPikado[s][pkIgrac][1 - i] != INVALID_PLAYER_ID)
    {
        PikadoPoruka(s, "[Pikado] %s je napustio igru.", playerid);
        OkladaIzadi(playerid, false);
        PikadoPobjeda(s, 1 - i);
        return;
    }
    if (gPikado[s][pkNisani] == playerid) PikadoNisanKraj(s);
    gPikado[s][pkIgrac][i] = INVALID_PLAYER_ID;
    gPikadoIgrac[playerid] = -1;
    OkladaIzadi(playerid, true);
    if (gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID) PikadoKraj(s);
}

PikadoOkladaStol(playerid)
{
    new s = gPikadoIgrac[playerid];
    if (s == -1) return -1;
    new i = gPikado[s][pkIgrac][0] == playerid ? 0 : 1;
    return gPikado[s][pkFaza] == PF_CEKA && gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID ? s : -2 - s;
}

static PikadoBlizu(playerid)
{
    for (new s = 0; s < PIKADO_MAX; s++)
        if (gPikado[s][pkIma] && GetPlayerVirtualWorld(playerid) == gPikado[s][pkSvijet] && GetPlayerInterior(playerid) == gPikado[s][pkInt] && IsPlayerInRangeOfPoint(playerid, PIKADO_DOMET, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.0)) return s;
    return -1;
}

static PikadoPotez(s)
{
    new p = gPikado[s][pkIgrac][gPikado[s][pkNaPotezu]];
    gPikado[s][pkFaza] = PF_IGRA;
    gPikado[s][pkBacio] = 0;
    gPikado[s][pkPrije] = gPikado[s][pkBodovi][gPikado[s][pkNaPotezu]];
    for (new i = 0; i < 2; i++)
    {
        new q = gPikado[s][pkIgrac][i];
        if (q == INVALID_PLAYER_ID) continue;
        if (q == p) GameTextForPlayer(q, T(q, "~g~Vi ste na potezu~n~~w~RMB - nisani"), 3000, 4);
        else GameTextForPlayer(q, T(q, "~y~Protivnik je na potezu"), 3000, 4);
    }
}

PikadoCilja(playerid)
{
    new s = gPikadoIgrac[playerid];
    return s != -1 && gPikado[s][pkNisani] == playerid;
}

PikadoKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = gPikadoIgrac[playerid];
    if (s != -1)
    {
        PikadoOdlazi(playerid);
        IgraObavijest(playerid, T(playerid, "Pikado"), PIKADO_ZELENA, T(playerid, "Napustili ste pikado."));
        return 1;
    }
    s = PikadoBlizu(playerid);
    if (s == -1) return 0;
    new i = gPikado[s][pkIgrac][0] == INVALID_PLAYER_ID ? 0 : gPikado[s][pkIgrac][1] == INVALID_PLAYER_ID ? 1 : -1;
    if (i == -1 || gPikado[s][pkFaza] != PF_CEKA) return IgraObavijest(playerid, T(playerid, "Pikado"), OBAVIJEST_GRESKA, T(playerid, "Ovdje se vec igra.")), 1;
    new poruka[96];
    if (!DajNovac(playerid, -PIKADO_CIJENA))
    {
        format(poruka, sizeof poruka, T(playerid, "Igra kosta $%d."), PIKADO_CIJENA);
        return IgraObavijest(playerid, T(playerid, "Pikado"), OBAVIJEST_GRESKA, poruka), 1;
    }
    if (!OkladaUdi(playerid, OK_PIKADO, s)) return DajNovac(playerid, PIKADO_CIJENA), 1;
    BlagajnaDodaj(PIKADO_CIJENA);
    gPikado[s][pkIgrac][i] = playerid;
    gPikadoIgrac[playerid] = s;
    if (gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID) return IgraObavijest(playerid, T(playerid, "Pikado"), PIKADO_ZELENA, T(playerid, "Pridruzili ste se pikadu (301). Ceka se protivnik ili RMB za igru sami (ALT za odlazak, /oklada [iznos] za ulog)."), 0), 1;
    gPikado[s][pkSam] = false;
    gPikado[s][pkBodovi][0] = gPikado[s][pkBodovi][1] = PIKADO_POCETNO;
    gPikado[s][pkNaPotezu] = random(2);
    PikadoPoruka(s, "[Pikado] Igra 301 pocinje! Prvi baca %s.", gPikado[s][pkIgrac][gPikado[s][pkNaPotezu]]);
    PikadoPotez(s);
    return 1;
}

static PikadoSam(s, playerid)
{
    new i = gPikado[s][pkIgrac][0] == playerid ? 0 : 1;
    OkladaZatvori(OK_PIKADO, s);
    gPikado[s][pkSam] = true;
    gPikado[s][pkUkupno] = 0;
    gPikado[s][pkBodovi][i] = PIKADO_POCETNO;
    gPikado[s][pkNaPotezu] = i;
    IgraObavijest(playerid, T(playerid, "Pikado"), PIKADO_ZELENA, T(playerid, "[Pikado] Igrate 301 sami. Spustite rezultat na 0 u sto manje strelica."), 0);
    PikadoPotez(s);
}

static Float:PikadoRub(Float:v)
{
    return v < -225.0 ? -225.0 : v > 225.0 ? 225.0 : v;
}

static PikadoNjihanje(s)
{
    new Float:a = 10.0 + float(GetTickCount() - gPikado[s][pkNisanOd]) * 0.003;
    if (a > 40.0) a = 40.0;
    new Float:t = float(GetTickCount() % 360000) / 1000.0;
    gPikado[s][pkSx] = a * (0.65 * floatsin(t * 97.0, degrees) + 0.35 * floatsin(t * 233.0 + 20.0, degrees));
    gPikado[s][pkSy] = a * 0.8 * (0.6 * floatsin(t * 131.0 + 40.0, degrees) + 0.4 * floatsin(t * 263.0, degrees));
}

forward PikadoNisanTick(s, token);
public PikadoNisanTick(s, token)
{
    new p = gPikado[s][pkNisani];
    if (token != gPikado[s][pkToken] || p == INVALID_PLAYER_ID) return;
    if (!IsPlayerConnected(p) || gPikadoIgrac[p] != s)
    {
        PikadoNisanKraj(s);
        return;
    }
    new KEY:tipke, gore, strana;
    GetPlayerKeys(p, tipke, gore, strana);
    new Float:brzina = (tipke & KEY_WALK) ? PIKADO_POMAK * 0.3 : PIKADO_POMAK;
    gPikado[s][pkAx] = PikadoRub(gPikado[s][pkAx] + float(strana) / 128.0 * brzina);
    gPikado[s][pkAy] = PikadoRub(gPikado[s][pkAy] - float(gore) / 128.0 * brzina);
    PikadoNjihanje(s);
    if (gPikado[s][pkDrzi])
    {
        gPikado[s][pkSnaga] += gPikado[s][pkGore] ? 5.0 : -5.0;
        if (gPikado[s][pkSnaga] >= 100.0) gPikado[s][pkSnaga] = 100.0, gPikado[s][pkGore] = false;
        if (gPikado[s][pkSnaga] <= 0.0) gPikado[s][pkSnaga] = 0.0, gPikado[s][pkGore] = true;
        PikadoTraka(p, PTD_SNAGA, gPikado[s][pkSnaga] / 100.0, 8.0);
    }
    new Float:x = gPikado[s][pkAx] + gPikado[s][pkSx], Float:y = gPikado[s][pkAy] + gPikado[s][pkSy], polje[16], kod = PikadoPolje(x, y, polje);
    PikadoNisanCrtaj(p, x, y);
    if (kod != gPikado[s][pkPolje])
    {
        gPikado[s][pkPolje] = kod;
        PlayerTextDrawSetString(p, gPkTd[p][PTD_POLJE], polje);
        PlayerTextDrawHide(p, gPkTd[p][PTD_BROJ]);
        if (kod >= 100)
        {
            new Float:px, Float:py, broj[4];
            PikadoBrojPoz(kod % 100, px, py);
            valstr(broj, gPikadoBroj[kod % 100]);
            PlayerTextDrawSetPos(p, gPkTd[p][PTD_BROJ], px, py);
            PlayerTextDrawSetString(p, gPkTd[p][PTD_BROJ], broj);
            PlayerTextDrawShow(p, gPkTd[p][PTD_BROJ]);
        }
    }
    SetTimerEx("PikadoNisanTick", PIKADO_NISAN_MS, false, "ii", s, token);
}

static PikadoNisani(playerid, s)
{
    gPikado[s][pkNisani] = playerid;
    new Float:kut = float(random(360)), Float:r = float(random(150));
    gPikado[s][pkAx] = r * floatsin(kut, degrees);
    gPikado[s][pkAy] = r * floatcos(kut, degrees);
    gPikado[s][pkDrzi] = false;
    gPikado[s][pkNisanOd] = GetTickCount();
    gPikado[s][pkPolje] = -1;
    TogglePlayerControllable(playerid, false);
    SetPlayerFacingAngle(playerid, gPikado[s][pkA]);
    PikadoTdStvori(playerid);
    for (new i = 0; i < gPikado[s][pkBacio]; i++) PikadoStrelicaCrtaj(playerid, i, gPikado[s][pkHx][i], gPikado[s][pkHy][i]);
    PikadoInfo(s, playerid);
    SetTimerEx("PikadoNisanTick", PIKADO_NISAN_MS, false, "ii", s, gPikado[s][pkToken]);
}

static PikadoBaci(playerid, s)
{
    new n = gPikado[s][pkNaPotezu], d = gPikado[s][pkBacio], Float:greska = gPikado[s][pkSnaga] - gPikado[s][pkIdeal];
    new Float:x = gPikado[s][pkAx] + gPikado[s][pkSx] + float(random(PIKADO_RASIPANJE * 2 + 1) - PIKADO_RASIPANJE);
    new Float:y = gPikado[s][pkAy] + gPikado[s][pkSy] + float(random(PIKADO_RASIPANJE * 2 + 1) - PIKADO_RASIPANJE);
    if (floatabs(greska) > PIKADO_TOCNO) y += greska * 1.5;
    gPikado[s][pkDrzi] = false;
    gPikado[s][pkNisanOd] = GetTickCount();
    PikadoTraka(playerid, PTD_SNAGA, 0.0, 8.0);
    gPikado[s][pkHx][d] = x;
    gPikado[s][pkHy][d] = y;
    gPikado[s][pkBacio]++;
    gPikado[s][pkUkupno]++;
    ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, false, false, false, false, 0);
    PikadoStrelicaCrtaj(playerid, d, x, y);
    new b = PikadoBodovi(x, y), ostalo = gPikado[s][pkBodovi][n] - b;
    PikadoPoruka(s, "[Pikado] %s pogada %d (preostalo %d).", playerid, b, ostalo < 0 ? gPikado[s][pkPrije] : ostalo);
    if (!ostalo)
    {
        gPikado[s][pkBodovi][n] = 0;
        PikadoNisanKraj(s);
        PikadoPobjeda(s, n);
        return;
    }
    if (ostalo < 0)
    {
        gPikado[s][pkBodovi][n] = gPikado[s][pkPrije];
        PikadoPoruka(s, "[Pikado] Previse! %s vraca bodove na pocetak poteza.", playerid);
    }
    else gPikado[s][pkBodovi][n] = ostalo;
    PikadoInfo(s, playerid);
    if (ostalo > 0 && gPikado[s][pkBacio] < PIKADO_STRELICA) return;
    gPikado[s][pkFaza] = PF_PAUZA;
    SetTimerEx("PikadoSljedeci", PIKADO_POTEZ_MS, false, "ii", s, gPikado[s][pkToken]);
}

forward PikadoSljedeci(s, token);
public PikadoSljedeci(s, token)
{
    if (token != gPikado[s][pkToken] || gPikado[s][pkFaza] != PF_PAUZA) return;
    PikadoNisanKraj(s);
    if (!gPikado[s][pkSam]) gPikado[s][pkNaPotezu] = 1 - gPikado[s][pkNaPotezu];
    PikadoPotez(s);
}

PikadoTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    new s = gPikadoIgrac[playerid];
    if (s == -1 || !((newkeys | oldkeys) & (KEY_AIM | KEY_FIRE))) return 0;
    new i = gPikado[s][pkIgrac][0] == playerid ? 0 : 1, bool:rmb = (newkeys & KEY_AIM) && !(oldkeys & KEY_AIM);
    if (rmb && gPikado[s][pkFaza] == PF_CEKA && gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID)
    {
        if (!IsPlayerInRangeOfPoint(playerid, PIKADO_DOMET, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.0)) return IgraObavijest(playerid, T(playerid, "Pikado"), OBAVIJEST_GRESKA, T(playerid, "Stanite na liniju bacanja.")), 1;
        PikadoSam(s, playerid);
        PikadoNisani(playerid, s);
        return 1;
    }
    if (gPikado[s][pkFaza] != PF_IGRA || gPikado[s][pkIgrac][gPikado[s][pkNaPotezu]] != playerid) return 0;
    if (rmb)
    {
        if (gPikado[s][pkNisani] == playerid)
        {
            PikadoNisanKraj(s);
            return 1;
        }
        if (!IsPlayerInRangeOfPoint(playerid, PIKADO_DOMET, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.0)) return IgraObavijest(playerid, T(playerid, "Pikado"), OBAVIJEST_GRESKA, T(playerid, "Stanite na liniju bacanja.")), 1;
        PikadoNisani(playerid, s);
        return 1;
    }
    if (gPikado[s][pkNisani] != playerid) return 0;
    if ((newkeys & KEY_FIRE) && !(oldkeys & KEY_FIRE) && !gPikado[s][pkDrzi])
    {
        gPikado[s][pkDrzi] = true;
        gPikado[s][pkGore] = true;
        gPikado[s][pkSnaga] = 0.0;
        gPikado[s][pkIdeal] = PIKADO_IDEAL_OD + float(random(floatround(PIKADO_IDEAL_DO - PIKADO_IDEAL_OD) + 1));
        return 1;
    }
    if (!(newkeys & KEY_FIRE) && (oldkeys & KEY_FIRE) && gPikado[s][pkDrzi])
    {
        PikadoBaci(playerid, s);
        return 1;
    }
    return 0;
}

PikadoTick()
{
    for (new s = 0; s < PIKADO_MAX; s++)
    {
        if (!gPikado[s][pkIma]) continue;
        for (new i = 0; i < 2; i++)
        {
            new p = gPikado[s][pkIgrac][i];
            if (p == INVALID_PLAYER_ID || gPikado[s][pkNisani] == p) continue;
            if (GetPlayerVirtualWorld(p) != gPikado[s][pkSvijet] || !IsPlayerInRangeOfPoint(p, PIKADO_NAPUSTI, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ]))
            {
                IgraObavijest(p, T(p, "Pikado"), PIKADO_ZELENA, T(p, "Napustili ste pikado."));
                PikadoOdlazi(p);
            }
        }
    }
}

ShowPikado(playerid)
{
    new list[1024], red[96], len = 0;
    DodajStupac(list, len, T(playerid, "+ Novo mjesto za pikado (stanite na liniju bacanja, gledajte prema zidu)"));
    for (new s = 0, n = 0; s < PIKADO_MAX; s++)
    {
        if (!gPikado[s][pkIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Pikado (%s) - obrisi"), gPikado[s][pkDb], CestaZona(gPikado[s][pkX], gPikado[s][pkY]));
        DodajStupac(list, len, red);
        gApPikado[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_PIKADO, DIALOG_STYLE_LIST, T(playerid, "Pikado"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_pikado(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0, Float:x, Float:y, Float:z, Float:a;
        while (s < PIKADO_MAX && gPikado[s][pkIma]) s++;
        if (s == PIKADO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj pikado ploca.")), ShowPikado(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO dart_boards (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));
        gPikado[s][pkDb] = ZadnjiId();
        gPikado[s][pkX] = x;
        gPikado[s][pkY] = y;
        gPikado[s][pkZ] = z;
        gPikado[s][pkA] = a;
        gPikado[s][pkInt] = GetPlayerInterior(playerid);
        gPikado[s][pkSvijet] = GetPlayerVirtualWorld(playerid);
        PikadoStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pikado ploca je postavljena."));
        return ShowPikado(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= PIKADO_MAX) return 1;
    new s = gApPikado[playerid][i];
    if (!gPikado[s][pkIma]) return ShowPikado(playerid);
    for (new k = 0; k < 2; k++) if (gPikado[s][pkIgrac][k] != INVALID_PLAYER_ID) PikadoOdlazi(gPikado[s][pkIgrac][k]);
    PikadoKraj(s);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM dart_boards WHERE id = %d", gPikado[s][pkDb]));
    DestroyDynamic3DTextLabel(gPikado[s][pkTekst]);
    gPikado[s][pkIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pikado ploca je obrisana."));
    return ShowPikado(playerid);
}
