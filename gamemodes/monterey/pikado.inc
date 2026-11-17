#define PIKADO_MAX          16
#define PIKADO_MODEL        2049
#define PIKADO_KUT          0.0
#define PIKADO_UDALJENOST   2.37
#define PIKADO_VISINA       1.73
#define PIKADO_DOMET        1.5
#define PIKADO_NAPUSTI      10.0
#define PIKADO_POCETNO      301
#define PIKADO_STRELICA     3
#define PIKADO_NISAN_MS     50
#define PIKADO_POTEZ_MS     1500
#define PIKADO_CX           320.0
#define PIKADO_CY           210.0
#define PIKADO_R            110.0
#define PIKADO_NJIHANJE     22.0
#define PIKADO_RASIPANJE    8
#define PIKADO_POMAK        5.0
#define PIKADO_TD           33

enum { PF_CEKA, PF_IGRA, PF_PAUZA }
enum { PTD_KRUG, PTD_BROJ = PTD_KRUG + 7, PTD_NISAN = PTD_BROJ + 20, PTD_STRELICA, PTD_INFO = PTD_STRELICA + PIKADO_STRELICA }
enum E_PIKADO { bool:pkIma, pkDb, Float:pkX, Float:pkY, Float:pkZ, Float:pkA, pkInt, pkSvijet, STREAMER_TAG_OBJECT:pkObj, STREAMER_TAG_3D_TEXT_LABEL:pkTekst,
    pkFaza, pkIgrac[2], pkBodovi[2], pkNaPotezu, pkBacio, pkPrije, pkNisani, Float:pkAx, Float:pkAy, Float:pkHx[PIKADO_STRELICA], Float:pkHy[PIKADO_STRELICA], pkToken }

static const gPikadoBroj[20] = { 20, 1, 18, 4, 13, 6, 10, 15, 2, 17, 3, 19, 7, 16, 8, 11, 14, 9, 12, 5 };
static const Float:gPikadoKrug[7] = { 225.0, 170.0, 162.0, 107.0, 99.0, 15.9, 6.35 };
static const gPikadoBoja[7] = { 0x111111FF, 0xB03A2EFF, 0xE8DCC0FF, 0x1E8449FF, 0xE8DCC0FF, 0x1E8449FF, 0xB03A2EFF };

static gPikado[PIKADO_MAX][E_PIKADO], gPikadoIgrac[MAX_PLAYERS] = { -1, ... }, gApPikado[MAX_PLAYERS][PIKADO_MAX];
static PlayerText:gPkTd[MAX_PLAYERS][PIKADO_TD], bool:gPkTdIma[MAX_PLAYERS];

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

InitPikado()
{
    assert(PikadoBodovi(0.0, 0.0) == 50 && PikadoBodovi(0.0, 10.0) == 25 && PikadoBodovi(0.0, 103.0) == 60 && PikadoBodovi(0.0, 166.0) == 40);
    assert(PikadoBodovi(0.0, 130.0) == 20 && PikadoBodovi(0.0, 200.0) == 0 && PikadoBodovi(103.0, 0.0) == 18 && PikadoBodovi(0.0, -130.0) == 3);
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

static PikadoStvori(s)
{
    new Float:fx = -floatsin(gPikado[s][pkA], degrees), Float:fy = floatcos(gPikado[s][pkA], degrees);
    gPikado[s][pkIma] = true;
    gPikado[s][pkIgrac][0] = gPikado[s][pkIgrac][1] = INVALID_PLAYER_ID;
    gPikado[s][pkNisani] = INVALID_PLAYER_ID;
    gPikado[s][pkFaza] = PF_CEKA;
    gPikado[s][pkObj] = CreateDynamicObject(PIKADO_MODEL, gPikado[s][pkX] + fx * PIKADO_UDALJENOST, gPikado[s][pkY] + fy * PIKADO_UDALJENOST, gPikado[s][pkZ] + PIKADO_VISINA, 0.0, 0.0, gPikado[s][pkA] + PIKADO_KUT, gPikado[s][pkSvijet], gPikado[s][pkInt]);
    gPikado[s][pkTekst] = CreateDynamic3DTextLabel("Pikado\n[Y]", COLOR_ORG, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.2, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gPikado[s][pkSvijet], gPikado[s][pkInt]);
}

static PikadoPoruka(s, const tekst[], igrac = INVALID_PLAYER_ID, a = 0, b = 0)
{
    for (new i = 0; i < 2; i++)
    {
        new p = gPikado[s][pkIgrac][i];
        if (p == INVALID_PLAYER_ID) continue;
        if (igrac != INVALID_PLAYER_ID) SendClientMessage(p, 0x2DD9A9FF, T(p, tekst), GetRPIme(igrac), a, b);
        else SendClientMessage(p, 0x2DD9A9FF, T(p, tekst));
    }
}

static PikadoTdUnisti(playerid)
{
    if (!gPkTdIma[playerid]) return;
    gPkTdIma[playerid] = false;
    for (new i = 0; i < PIKADO_TD; i++) PlayerTextDrawDestroy(playerid, gPkTd[playerid][i]);
}

static PlayerText:PikadoKrug(playerid, Float:x, Float:y, Float:r, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x - r, y - r, "LD_BEAT:chit");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, r * 2.0, r * 2.0);
    PlayerTextDrawColour(playerid, td, boja);
    return td;
}

static PikadoTdStvori(playerid)
{
    new Float:k = PIKADO_R / 170.0, tekst[4];
    PikadoTdUnisti(playerid);
    gPkTdIma[playerid] = true;
    for (new i = 0; i < sizeof gPikadoKrug; i++) gPkTd[playerid][PTD_KRUG + i] = PikadoKrug(playerid, PIKADO_CX, PIKADO_CY, gPikadoKrug[i] * k, gPikadoBoja[i]);
    for (new i = 0; i < 20; i++)
    {
        new Float:a = float(i) * 18.0, PlayerText:td;
        valstr(tekst, gPikadoBroj[i]);
        td = CreatePlayerTextDraw(playerid, PIKADO_CX + 196.0 * k * floatsin(a, degrees), PIKADO_CY - 196.0 * k * floatcos(a, degrees) - 5.0, tekst);
        PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
        PlayerTextDrawLetterSize(playerid, td, 0.22, 1.0);
        PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
        PlayerTextDrawSetOutline(playerid, td, 0);
        PlayerTextDrawSetShadow(playerid, td, 0);
        gPkTd[playerid][PTD_BROJ + i] = td;
    }
    gPkTd[playerid][PTD_NISAN] = PikadoKrug(playerid, PIKADO_CX, PIKADO_CY, 4.0, 0x00E5FFFF);
    for (new i = 0; i < PIKADO_STRELICA; i++) gPkTd[playerid][PTD_STRELICA + i] = PikadoKrug(playerid, -50.0, -50.0, 3.0, 0x000000FF);
    gPkTd[playerid][PTD_INFO] = CreatePlayerTextDraw(playerid, PIKADO_CX, PIKADO_CY + 160.0, "_");
    PlayerTextDrawFont(playerid, gPkTd[playerid][PTD_INFO], TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, gPkTd[playerid][PTD_INFO], 0.24, 1.1);
    PlayerTextDrawAlignment(playerid, gPkTd[playerid][PTD_INFO], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawSetOutline(playerid, gPkTd[playerid][PTD_INFO], 1);
    for (new i = 0; i < PTD_STRELICA; i++) PlayerTextDrawShow(playerid, gPkTd[playerid][i]);
    PlayerTextDrawShow(playerid, gPkTd[playerid][PTD_INFO]);
}

static PikadoPostaviKrug(playerid, PlayerText:td, Float:mx, Float:my, Float:r)
{
    new Float:k = PIKADO_R / 170.0;
    PlayerTextDrawHide(playerid, td);
    PlayerTextDrawSetPos(playerid, td, PIKADO_CX + mx * k - r, PIKADO_CY - my * k - r);
    PlayerTextDrawShow(playerid, td);
}

static PikadoInfo(s, playerid)
{
    new tekst[160], n = gPikado[s][pkNaPotezu];
    format(tekst, sizeof tekst, T(playerid, "~w~%s: ~y~%d~w~  |  %s: ~y~%d~n~~w~Strelica: %d/%d  |  A/D/W/S nisan, LMB bacanje, RMB odustani"),
        GetRPIme(gPikado[s][pkIgrac][n]), gPikado[s][pkBodovi][n], GetRPIme(gPikado[s][pkIgrac][1 - n]), gPikado[s][pkBodovi][1 - n], gPikado[s][pkBacio], PIKADO_STRELICA);
    PlayerTextDrawSetString(playerid, gPkTd[playerid][PTD_INFO], tekst);
}

static PikadoNisanKraj(s)
{
    new p = gPikado[s][pkNisani];
    gPikado[s][pkNisani] = INVALID_PLAYER_ID;
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
    OkladaZatvori(OK_PIKADO, s);
}

static PikadoPobjeda(s, pobjednik)
{
    new p = gPikado[s][pkIgrac][pobjednik], w[1];
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
    if (gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID) OkladaZatvori(OK_PIKADO, s);
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

PikadoKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = gPikadoIgrac[playerid];
    if (s != -1)
    {
        PikadoOdlazi(playerid);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Napustili ste pikado."));
        return 1;
    }
    s = PikadoBlizu(playerid);
    if (s == -1) return 0;
    new i = gPikado[s][pkIgrac][0] == INVALID_PLAYER_ID ? 0 : gPikado[s][pkIgrac][1] == INVALID_PLAYER_ID ? 1 : -1;
    if (i == -1 || gPikado[s][pkFaza] != PF_CEKA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovdje se vec igra.")), 1;
    if (!OkladaUdi(playerid, OK_PIKADO, s)) return 1;
    gPikado[s][pkIgrac][i] = playerid;
    gPikadoIgrac[playerid] = s;
    if (gPikado[s][pkIgrac][1 - i] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pridruzili ste se pikadu (301). Ceka se protivnik (Y za odlazak, /oklada [iznos] za ulog).")), 1;
    gPikado[s][pkBodovi][0] = gPikado[s][pkBodovi][1] = PIKADO_POCETNO;
    gPikado[s][pkNaPotezu] = random(2);
    PikadoPoruka(s, "[Pikado] Igra 301 pocinje! Prvi baca %s.", gPikado[s][pkIgrac][gPikado[s][pkNaPotezu]]);
    PikadoPotez(s);
    return 1;
}

static Float:PikadoRub(Float:v)
{
    return v < -225.0 ? -225.0 : v > 225.0 ? 225.0 : v;
}

static PikadoNjihanje(s, &Float:x, &Float:y)
{
    new Float:t = float(GetTickCount() % 360000) / 1000.0;
    x = gPikado[s][pkAx] + PIKADO_NJIHANJE * floatsin(t * 97.0, degrees);
    y = gPikado[s][pkAy] + PIKADO_NJIHANJE * 0.8 * floatsin(t * 131.0 + 40.0, degrees);
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
    gPikado[s][pkAx] = PikadoRub(gPikado[s][pkAx] + float(strana) / 128.0 * PIKADO_POMAK);
    gPikado[s][pkAy] = PikadoRub(gPikado[s][pkAy] - float(gore) / 128.0 * PIKADO_POMAK);
    new Float:x, Float:y;
    PikadoNjihanje(s, x, y);
    PikadoPostaviKrug(p, gPkTd[p][PTD_NISAN], x, y, 4.0);
    SetTimerEx("PikadoNisanTick", PIKADO_NISAN_MS, false, "ii", s, token);
}

static PikadoNisani(playerid, s)
{
    gPikado[s][pkNisani] = playerid;
    gPikado[s][pkAx] = 0.0;
    gPikado[s][pkAy] = 0.0;
    TogglePlayerControllable(playerid, false);
    SetPlayerFacingAngle(playerid, gPikado[s][pkA]);
    PikadoTdStvori(playerid);
    for (new i = 0; i < gPikado[s][pkBacio]; i++) PikadoPostaviKrug(playerid, gPkTd[playerid][PTD_STRELICA + i], gPikado[s][pkHx][i], gPikado[s][pkHy][i], 3.0);
    PikadoInfo(s, playerid);
    SetTimerEx("PikadoNisanTick", PIKADO_NISAN_MS, false, "ii", s, gPikado[s][pkToken]);
}

static PikadoBaci(playerid, s)
{
    new Float:x, Float:y, n = gPikado[s][pkNaPotezu], d = gPikado[s][pkBacio];
    PikadoNjihanje(s, x, y);
    x += float(random(PIKADO_RASIPANJE * 2 + 1) - PIKADO_RASIPANJE);
    y += float(random(PIKADO_RASIPANJE * 2 + 1) - PIKADO_RASIPANJE);
    gPikado[s][pkHx][d] = x;
    gPikado[s][pkHy][d] = y;
    gPikado[s][pkBacio]++;
    ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, false, false, false, false, 0);
    PikadoPostaviKrug(playerid, gPkTd[playerid][PTD_STRELICA + d], x, y, 3.0);
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
    gPikado[s][pkNaPotezu] = 1 - gPikado[s][pkNaPotezu];
    PikadoPotez(s);
}

PikadoTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    new s = gPikadoIgrac[playerid];
    if (s == -1 || gPikado[s][pkFaza] != PF_IGRA || gPikado[s][pkIgrac][gPikado[s][pkNaPotezu]] != playerid) return 0;
    if ((newkeys & KEY_AIM) && !(oldkeys & KEY_AIM))
    {
        if (gPikado[s][pkNisani] == playerid)
        {
            PikadoNisanKraj(s);
            return 1;
        }
        if (!IsPlayerInRangeOfPoint(playerid, PIKADO_DOMET, gPikado[s][pkX], gPikado[s][pkY], gPikado[s][pkZ] + 1.0)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Stanite na liniju bacanja.")), 1;
        PikadoNisani(playerid, s);
        return 1;
    }
    if (gPikado[s][pkNisani] == playerid && (newkeys & KEY_FIRE) && !(oldkeys & KEY_FIRE))
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
                SendClientMessage(p, COLOR_INFO, T(p, "Napustili ste pikado."));
                PikadoOdlazi(p);
            }
        }
    }
}

ShowPikado(playerid)
{
    new list[1024], red[96], len = 0;
    DodajStupac(list, len, T(playerid, "+ Nova pikado ploca (stanite na liniju bacanja, gledajte prema zidu)"));
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
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
        gPikado[s][pkDb] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
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
    DestroyDynamicObject(gPikado[s][pkObj]);
    DestroyDynamic3DTextLabel(gPikado[s][pkTekst]);
    gPikado[s][pkIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pikado ploca je obrisana."));
    return ShowPikado(playerid);
}
