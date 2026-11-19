#define KUGLANJE_MAX        16
#define KUGLANJE_CIJENA     8
#define KUGLANJE_BOJA       0x1E5AA8E6
#define KUGLANJE_CUNJ       1509
#define KUGLANJE_KUGLA      2114
#define KUGLANJE_DUZINA     18.29
#define KUGLANJE_SIRINA     0.53
#define KUGLANJE_RAZMAK     0.305
#define KUGLANJE_RED        0.264
#define KUGLANJE_CUNJ_R     0.06
#define KUGLANJE_KUGLA_R    0.11
#define KUGLANJE_KUGLA_Z    0.15
#define KUGLANJE_CUNJ_Z     0.1
#define KUGLANJE_LANAC      0.42
#define KUGLANJE_DOMET      1.5
#define KUGLANJE_NAPUSTI    30.0
#define KUGLANJE_NISAN_MS   50
#define KUGLANJE_PAUZA_MS   2500
#define KUGLANJE_MAX_KUT    5.0
#define KUGLANJE_BACANJA    21

enum { KG_CEKA, KG_IGRA, KG_KOTRLJA, KG_PAUZA }
enum E_KUGLANA { bool:kgIma, kgDb, Float:kgX, Float:kgY, Float:kgZ, Float:kgA, kgInt, kgSvijet, STREAMER_TAG_OBJECT:kgCunj[10], STREAMER_TAG_OBJECT:kgKugla, STREAMER_TAG_3D_TEXT_LABEL:kgTekst,
    kgFaza, kgIgrac[2], kgNaPotezu, bool:kgStoji[10], bool:kgPao[10], kgNisani, Float:kgKut, Float:kgSnaga, bool:kgGore, Float:kgBx, kgToken, bool:kgSam }

static const Float:gKgCunj[10][2] = {
    { 0.0, 0.0 },
    { -0.5, 1.0 }, { 0.5, 1.0 },
    { -1.0, 2.0 }, { 0.0, 2.0 }, { 1.0, 2.0 },
    { -1.5, 3.0 }, { -0.5, 3.0 }, { 0.5, 3.0 }, { 1.5, 3.0 }
};

static gKg[KUGLANJE_MAX][E_KUGLANA], gKgRoll[KUGLANJE_MAX][2][KUGLANJE_BACANJA], gKgN[KUGLANJE_MAX][2], gKuglanjeIgrac[MAX_PLAYERS] = { -1, ... }, gApKuglanje[MAX_PLAYERS][KUGLANJE_MAX];
static PlayerText:gKgTd[MAX_PLAYERS], PlayerText:gKgBar[MAX_PLAYERS][2], bool:gKgTdIma[MAX_PLAYERS];

KuglanjeRezultat(const b[], n)
{
    new zbroj = 0, i = 0;
    for (new f = 0; f < 10 && i < n; f++)
    {
        new a = b[i], c = i + 1 < n ? b[i + 1] : 0, d = i + 2 < n ? b[i + 2] : 0;
        if (a == 10)
        {
            zbroj += 10 + c + d;
            i++;
        }
        else if (a + c == 10)
        {
            zbroj += 10 + d;
            i += 2;
        }
        else
        {
            zbroj += a + c;
            i += 2;
        }
    }
    return zbroj;
}

KuglanjeOkvir(const b[], n, &u)
{
    new i = 0;
    for (new f = 0; f < 9; f++)
    {
        if (i >= n)
        {
            u = 0;
            return f;
        }
        if (b[i] == 10) i++;
        else if (i + 1 < n) i += 2;
        else
        {
            u = 1;
            return f;
        }
    }
    u = n - i;
    if (u >= 3 || (u == 2 && b[i] + b[i + 1] < 10)) return 10;
    return 9;
}

InitKuglanje()
{
    new b[KUGLANJE_BACANJA], u;
    for (new i = 0; i < 12; i++) b[i] = 10;
    assert(KuglanjeRezultat(b, 12) == 300 && KuglanjeOkvir(b, 12, u) == 10 && KuglanjeOkvir(b, 9, u) == 9 && u == 0 && KuglanjeOkvir(b, 10, u) == 9 && u == 1);
    for (new i = 0; i < 21; i++) b[i] = 5;
    assert(KuglanjeRezultat(b, 21) == 150 && KuglanjeOkvir(b, 21, u) == 10 && KuglanjeOkvir(b, 3, u) == 1 && u == 1);
    for (new i = 0; i < 20; i++) b[i] = 4;
    assert(KuglanjeRezultat(b, 20) == 80 && KuglanjeOkvir(b, 20, u) == 10 && KuglanjeOkvir(b, 19, u) == 9 && u == 1);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bowling_lanes (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM bowling_lanes LIMIT %d", KUGLANJE_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gKg[s][kgDb] = DB_GetFieldInt(res, 0);
        gKg[s][kgX] = DB_GetFieldFloat(res, 1);
        gKg[s][kgY] = DB_GetFieldFloat(res, 2);
        gKg[s][kgZ] = DB_GetFieldFloat(res, 3);
        gKg[s][kgA] = DB_GetFieldFloat(res, 4);
        gKg[s][kgInt] = DB_GetFieldInt(res, 5);
        gKg[s][kgSvijet] = DB_GetFieldInt(res, 6);
        KuglanjeStvori(s);
    }
    DB_FreeResultSet(res);
}

KuglanjeConnect(playerid)
{
    gKuglanjeIgrac[playerid] = -1;
    gKgTdIma[playerid] = false;
}

static KuglanjeSvijet(s, Float:lx, Float:ly, &Float:wx, &Float:wy)
{
    new Float:c = floatcos(gKg[s][kgA], degrees), Float:sn = floatsin(gKg[s][kgA], degrees);
    wx = gKg[s][kgX] + lx * c - ly * sn;
    wy = gKg[s][kgY] + lx * sn + ly * c;
}

static KuglanjeCunjPoz(k, &Float:lx, &Float:ly)
{
    lx = gKgCunj[k][0] * KUGLANJE_RAZMAK;
    ly = KUGLANJE_DUZINA + gKgCunj[k][1] * KUGLANJE_RED;
}

static KuglanjePostavi(s)
{
    for (new k = 0; k < 10; k++)
    {
        new Float:lx, Float:ly, Float:wx, Float:wy;
        KuglanjeCunjPoz(k, lx, ly);
        KuglanjeSvijet(s, lx, ly, wx, wy);
        gKg[s][kgStoji][k] = true;
        gKg[s][kgPao][k] = false;
        if (gKg[s][kgCunj][k])
        {
            StopDynamicObject(gKg[s][kgCunj][k]);
            SetDynamicObjectPos(gKg[s][kgCunj][k], wx, wy, gKg[s][kgZ] + KUGLANJE_CUNJ_Z);
            SetDynamicObjectRot(gKg[s][kgCunj][k], 0.0, 0.0, 0.0);
        }
        else
        {
            gKg[s][kgCunj][k] = CreateDynamicObject(KUGLANJE_CUNJ, wx, wy, gKg[s][kgZ] + KUGLANJE_CUNJ_Z, 0.0, 0.0, 0.0, gKg[s][kgSvijet], gKg[s][kgInt], -1, 60.0);
            SetDynamicObjectMaterial(gKg[s][kgCunj][k], 0, 3922, "bistro", "Marble2", 0xFFFFFFFF);
            SetDynamicObjectMaterial(gKg[s][kgCunj][k], 1, 3922, "bistro", "Marble2", 0xFFC0392B);
        }
    }
}

static KuglanjeMakniPale(s)
{
    for (new k = 0; k < 10; k++)
    {
        if (!gKg[s][kgPao][k]) continue;
        gKg[s][kgPao][k] = false;
        StopDynamicObject(gKg[s][kgCunj][k]);
        SetDynamicObjectPos(gKg[s][kgCunj][k], gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ] - 3.0);
    }
}

static KuglanjeKuglaNazad(s)
{
    new Float:wx, Float:wy;
    KuglanjeSvijet(s, 0.7, -0.6, wx, wy);
    if (gKg[s][kgKugla])
    {
        StopDynamicObject(gKg[s][kgKugla]);
        SetDynamicObjectPos(gKg[s][kgKugla], wx, wy, gKg[s][kgZ] + KUGLANJE_KUGLA_Z);
    }
    else
    {
        gKg[s][kgKugla] = CreateDynamicObject(KUGLANJE_KUGLA, wx, wy, gKg[s][kgZ] + KUGLANJE_KUGLA_Z, 0.0, 0.0, 0.0, gKg[s][kgSvijet], gKg[s][kgInt], -1, 80.0);
        SetDynamicObjectMaterial(gKg[s][kgKugla], 0, 3922, "bistro", "Marble2", 0xFF1E5AA8);
    }
}

static KuglanjeOznaka()
{
    new tekst[48];
    format(tekst, sizeof tekst, "Kuglanje\n{FFFFFF}[Y] $%d", KUGLANJE_CIJENA);
    return tekst;
}

static KuglanjeStvori(s)
{
    gKg[s][kgIma] = true;
    gKg[s][kgIgrac][0] = gKg[s][kgIgrac][1] = INVALID_PLAYER_ID;
    gKg[s][kgNisani] = INVALID_PLAYER_ID;
    gKg[s][kgFaza] = KG_CEKA;
    gKg[s][kgTekst] = CreateDynamic3DTextLabel(KuglanjeOznaka(), 0xF39C12FF, gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ] + 1.2, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gKg[s][kgSvijet], gKg[s][kgInt]);
    KuglanjePostavi(s);
    KuglanjeKuglaNazad(s);
}

static KuglanjePoruka(s, const tekst[], igrac = INVALID_PLAYER_ID, a = 0, b = 0)
{
    for (new i = 0; i < 2; i++)
    {
        new p = gKg[s][kgIgrac][i];
        if (p == INVALID_PLAYER_ID) continue;
        new poruka[160];
        format(poruka, sizeof poruka, T(p, tekst), igrac != INVALID_PLAYER_ID ? GetRPIme(igrac) : "", a, b);
        IgraObavijest(p, T(p, "Kuglanje"), KUGLANJE_BOJA, poruka);
    }
}

static KuglanjeTdUnisti(playerid)
{
    if (!gKgTdIma[playerid]) return;
    gKgTdIma[playerid] = false;
    PlayerTextDrawDestroy(playerid, gKgTd[playerid]);
    PlayerTextDrawDestroy(playerid, gKgBar[playerid][0]);
    PlayerTextDrawDestroy(playerid, gKgBar[playerid][1]);
}

static KuglanjeNisanKraj(s)
{
    new p = gKg[s][kgNisani];
    gKg[s][kgNisani] = INVALID_PLAYER_ID;
    if (p == INVALID_PLAYER_ID || !IsPlayerConnected(p)) return;
    KuglanjeTdUnisti(p);
    TogglePlayerControllable(p, true);
    SetCameraBehindPlayer(p);
}

static KuglanjeKameraVrati(s)
{
    for (new i = 0; i < 2; i++)
    {
        new q = gKg[s][kgIgrac][i];
        if (q == INVALID_PLAYER_ID || !IsPlayerConnected(q)) continue;
        SetCameraBehindPlayer(q);
        TogglePlayerControllable(q, true);
    }
}

static KuglanjeKraj(s)
{
    gKg[s][kgToken]++;
    KuglanjeNisanKraj(s);
    if (gKg[s][kgFaza] == KG_KOTRLJA || gKg[s][kgFaza] == KG_PAUZA) KuglanjeKameraVrati(s);
    for (new i = 0; i < 2; i++)
    {
        if (gKg[s][kgIgrac][i] != INVALID_PLAYER_ID) gKuglanjeIgrac[gKg[s][kgIgrac][i]] = -1;
        gKg[s][kgIgrac][i] = INVALID_PLAYER_ID;
    }
    gKg[s][kgFaza] = KG_CEKA;
    gKg[s][kgSam] = false;
    OkladaZatvori(OK_KUGLANJE, s);
    KuglanjePostavi(s);
    KuglanjeKuglaNazad(s);
}

static KuglanjePobjeda(s, pobjednik)
{
    new w[2], n = 0;
    if (pobjednik == -1)
    {
        for (new i = 0; i < 2; i++) if (gKg[s][kgIgrac][i] != INVALID_PLAYER_ID) w[n++] = gKg[s][kgIgrac][i];
        KuglanjePoruka(s, "[Kuglanje] Nerijeseno!");
    }
    else if (gKg[s][kgIgrac][pobjednik] != INVALID_PLAYER_ID)
    {
        w[n++] = gKg[s][kgIgrac][pobjednik];
        KuglanjePoruka(s, "[Kuglanje] %s je pobijedio s %d bodova!", w[0], KuglanjeRezultat(gKgRoll[s][pobjednik], gKgN[s][pobjednik]));
    }
    OkladaIsplati(OK_KUGLANJE, s, w, n);
    KuglanjeKraj(s);
}

KuglanjeOdlazi(playerid)
{
    new s = gKuglanjeIgrac[playerid];
    if (s == -1) return;
    new i = gKg[s][kgIgrac][0] == playerid ? 0 : 1;
    if (gKg[s][kgFaza] != KG_CEKA && gKg[s][kgIgrac][1 - i] != INVALID_PLAYER_ID)
    {
        KuglanjePoruka(s, "[Kuglanje] %s je napustio igru.", playerid);
        OkladaIzadi(playerid, false);
        KuglanjePobjeda(s, 1 - i);
        return;
    }
    if (gKg[s][kgNisani] == playerid) KuglanjeNisanKraj(s);
    if (gKg[s][kgFaza] == KG_KOTRLJA || gKg[s][kgFaza] == KG_PAUZA)
    {
        SetCameraBehindPlayer(playerid);
        TogglePlayerControllable(playerid, true);
    }
    gKg[s][kgIgrac][i] = INVALID_PLAYER_ID;
    gKuglanjeIgrac[playerid] = -1;
    OkladaIzadi(playerid, true);
    if (gKg[s][kgIgrac][1 - i] == INVALID_PLAYER_ID) KuglanjeKraj(s);
}

KuglanjeOkladaStaza(playerid)
{
    new s = gKuglanjeIgrac[playerid];
    if (s == -1) return -1;
    new i = gKg[s][kgIgrac][0] == playerid ? 0 : 1;
    return gKg[s][kgFaza] == KG_CEKA && gKg[s][kgIgrac][1 - i] == INVALID_PLAYER_ID ? s : -2 - s;
}

static KuglanjeBlizu(playerid)
{
    for (new s = 0; s < KUGLANJE_MAX; s++)
        if (gKg[s][kgIma] && GetPlayerVirtualWorld(playerid) == gKg[s][kgSvijet] && GetPlayerInterior(playerid) == gKg[s][kgInt] && IsPlayerInRangeOfPoint(playerid, KUGLANJE_DOMET, gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ] + 1.0)) return s;
    return -1;
}

static KuglanjeStanje(s, tekst[], size)
{
    new u, a = gKg[s][kgIgrac][0], b = gKg[s][kgIgrac][1];
    new fa = KuglanjeOkvir(gKgRoll[s][0], gKgN[s][0], u), fb = KuglanjeOkvir(gKgRoll[s][1], gKgN[s][1], u);
    if (gKg[s][kgSam])
    {
        new n = gKg[s][kgNaPotezu], f = n ? fb : fa;
        format(tekst, size, "%s: %d (%d/10)", GetRPIme(gKg[s][kgIgrac][n]), KuglanjeRezultat(gKgRoll[s][n], gKgN[s][n]), f > 9 ? 10 : f + 1);
        return;
    }
    format(tekst, size, "%s: %d (%d/10)  |  %s: %d (%d/10)", GetRPIme(a), KuglanjeRezultat(gKgRoll[s][0], gKgN[s][0]), fa > 9 ? 10 : fa + 1, GetRPIme(b), KuglanjeRezultat(gKgRoll[s][1], gKgN[s][1]), fb > 9 ? 10 : fb + 1);
}

static KuglanjePotez(s)
{
    new p = gKg[s][kgIgrac][gKg[s][kgNaPotezu]], tekst[128];
    gKg[s][kgFaza] = KG_IGRA;
    KuglanjeStanje(s, tekst, sizeof tekst);
    for (new i = 0; i < 2; i++)
    {
        new q = gKg[s][kgIgrac][i];
        if (q == INVALID_PLAYER_ID) continue;
        IgraObavijest(q, T(q, "Kuglanje"), KUGLANJE_BOJA, tekst);
        if (q == p) GameTextForPlayer(q, T(q, "~g~Vi ste na potezu~n~~w~RMB - nisani"), 3000, 4);
        else GameTextForPlayer(q, T(q, "~y~Protivnik je na potezu"), 3000, 4);
    }
}

KuglanjeKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = gKuglanjeIgrac[playerid];
    if (s != -1)
    {
        KuglanjeOdlazi(playerid);
        IgraObavijest(playerid, T(playerid, "Kuglanje"), KUGLANJE_BOJA, T(playerid, "Napustili ste kuglanu."));
        return 1;
    }
    s = KuglanjeBlizu(playerid);
    if (s == -1) return 0;
    new i = gKg[s][kgIgrac][0] == INVALID_PLAYER_ID ? 0 : gKg[s][kgIgrac][1] == INVALID_PLAYER_ID ? 1 : -1;
    if (i == -1 || gKg[s][kgFaza] != KG_CEKA) return IgraObavijest(playerid, T(playerid, "Kuglanje"), OBAVIJEST_GRESKA, T(playerid, "Na ovoj stazi se vec igra.")), 1;
    new poruka[96];
    if (!DajNovac(playerid, -KUGLANJE_CIJENA))
    {
        format(poruka, sizeof poruka, T(playerid, "Igra kosta $%d."), KUGLANJE_CIJENA);
        return IgraObavijest(playerid, T(playerid, "Kuglanje"), OBAVIJEST_GRESKA, poruka), 1;
    }
    if (!OkladaUdi(playerid, OK_KUGLANJE, s)) return DajNovac(playerid, KUGLANJE_CIJENA), 1;
    BlagajnaDodaj(KUGLANJE_CIJENA);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste $%d."), KUGLANJE_CIJENA);
    gKg[s][kgIgrac][i] = playerid;
    gKuglanjeIgrac[playerid] = s;
    if (gKg[s][kgIgrac][1 - i] == INVALID_PLAYER_ID) return IgraObavijest(playerid, T(playerid, "Kuglanje"), KUGLANJE_BOJA, T(playerid, "Pridruzili ste se kuglanju. Ceka se protivnik ili RMB za igru sami (Y za odlazak, /oklada [iznos] za ulog)."), 0), 1;
    gKg[s][kgSam] = false;
    gKgN[s][0] = gKgN[s][1] = 0;
    gKg[s][kgNaPotezu] = random(2);
    KuglanjePostavi(s);
    KuglanjeKuglaNazad(s);
    KuglanjePoruka(s, "[Kuglanje] Igra pocinje (10 okvira)! Prvi baca %s.", gKg[s][kgIgrac][gKg[s][kgNaPotezu]]);
    KuglanjePotez(s);
    return 1;
}

static KuglanjeNisanPostavi(s)
{
    new p = gKg[s][kgNisani], Float:wx, Float:wy, Float:cx, Float:cy, Float:tx, Float:ty, Float:z = gKg[s][kgZ];
    KuglanjeSvijet(s, gKg[s][kgBx], 0.0, wx, wy);
    KuglanjeSvijet(s, gKg[s][kgBx], -1.0, cx, cy);
    KuglanjeSvijet(s, gKg[s][kgBx] + KUGLANJE_DUZINA * floatsin(gKg[s][kgKut], degrees), KUGLANJE_DUZINA * floatcos(gKg[s][kgKut], degrees), tx, ty);
    SetDynamicObjectPos(gKg[s][kgKugla], wx, wy, z + KUGLANJE_KUGLA_Z);
    SetPlayerCameraPos(p, cx, cy, z + 2.3);
    SetPlayerCameraLookAt(p, tx, ty, z + 0.2);
    new tekst[48];
    format(tekst, sizeof tekst, "~w~%s %.1f", T(p, "KUT"), gKg[s][kgKut]);
    PlayerTextDrawSetString(p, gKgTd[p], tekst);
    SnagaBarPostavi(p, gKgBar[p][1], gKg[s][kgSnaga]);
}

forward KuglanjeNisanTick(s, token);
public KuglanjeNisanTick(s, token)
{
    new p = gKg[s][kgNisani];
    if (token != gKg[s][kgToken] || p == INVALID_PLAYER_ID) return;
    if (!IsPlayerConnected(p) || gKuglanjeIgrac[p] != s)
    {
        KuglanjeNisanKraj(s);
        return;
    }
    new KEY:tipke, gore, strana;
    GetPlayerKeys(p, tipke, gore, strana);
    if (strana)
    {
        gKg[s][kgKut] += float(strana) / 128.0 * ((tipke & KEY_WALK) ? 0.05 : 0.25);
        if (gKg[s][kgKut] > KUGLANJE_MAX_KUT) gKg[s][kgKut] = KUGLANJE_MAX_KUT;
        if (gKg[s][kgKut] < -KUGLANJE_MAX_KUT) gKg[s][kgKut] = -KUGLANJE_MAX_KUT;
    }
    if (tipke & KEY_FIRE)
    {
        gKg[s][kgSnaga] += gKg[s][kgGore] ? 6.0 : -6.0;
        if (gKg[s][kgSnaga] >= 100.0) gKg[s][kgSnaga] = 100.0, gKg[s][kgGore] = false;
        if (gKg[s][kgSnaga] <= 0.0) gKg[s][kgSnaga] = 0.0, gKg[s][kgGore] = true;
    }
    KuglanjeNisanPostavi(s);
    SetTimerEx("KuglanjeNisanTick", KUGLANJE_NISAN_MS, false, "ii", s, token);
}

static KuglanjeNisani(playerid, s)
{
    new Float:px, Float:py, Float:pz, Float:c = floatcos(gKg[s][kgA], degrees), Float:sn = floatsin(gKg[s][kgA], degrees);
    GetPlayerPos(playerid, px, py, pz);
    new Float:lx = (px - gKg[s][kgX]) * c + (py - gKg[s][kgY]) * sn, Float:m = KUGLANJE_SIRINA - KUGLANJE_KUGLA_R - 0.05;
    gKg[s][kgBx] = lx < -m ? -m : lx > m ? m : lx;
    gKg[s][kgNisani] = playerid;
    gKg[s][kgKut] = 0.0;
    gKg[s][kgSnaga] = 0.0;
    gKg[s][kgGore] = true;
    TogglePlayerControllable(playerid, false);
    SetPlayerFacingAngle(playerid, gKg[s][kgA]);
    gKgTd[playerid] = CreatePlayerTextDraw(playerid, 320.0, 370.0, "_");
    PlayerTextDrawAlignment(playerid, gKgTd[playerid], TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawLetterSize(playerid, gKgTd[playerid], 0.3, 1.3);
    PlayerTextDrawFont(playerid, gKgTd[playerid], TEXT_DRAW_FONT_2);
    PlayerTextDrawSetOutline(playerid, gKgTd[playerid], 1);
    PlayerTextDrawShow(playerid, gKgTd[playerid]);
    SnagaBarStvori(playerid, gKgBar[playerid][0], gKgBar[playerid][1]);
    gKgTdIma[playerid] = true;
    KuglanjeNisanPostavi(s);
    IgraObavijest(playerid, T(playerid, "Kuglanje"), KUGLANJE_BOJA, T(playerid, "A/D - smjer (ALT precizno), drzi LMB - snaga, pusti LMB - bacanje, RMB - odustani."), 0);
    SetTimerEx("KuglanjeNisanTick", KUGLANJE_NISAN_MS, false, "ii", s, gKg[s][kgToken]);
}

static KuglanjeSimuliraj(s, Float:bx, Float:kut, Float:snaga, &Float:kraj)
{
    new Float:t = floattan(kut, degrees), Float:rub = KUGLANJE_SIRINA - KUGLANJE_KUGLA_R, sansa = 25 + floatround(snaga * 0.4);
    kraj = KUGLANJE_DUZINA + 1.2;
    if (t != 0.0)
    {
        new Float:g = ((t > 0.0 ? rub : -rub) - bx) / t;
        if (g < kraj) kraj = g;
    }
    if (kraj < KUGLANJE_DUZINA - KUGLANJE_CUNJ_R) return 0;
    new red[10], n = 0, Float:smjer[10][2];
    for (new k = 0; k < 10; k++)
    {
        if (!gKg[s][kgStoji][k]) continue;
        new Float:lx, Float:ly;
        KuglanjeCunjPoz(k, lx, ly);
        if (ly > kraj + KUGLANJE_CUNJ_R) continue;
        new Float:put = bx + t * ly, Float:razlika = lx - put;
        if (floatabs(razlika) > KUGLANJE_KUGLA_R + KUGLANJE_CUNJ_R) continue;
        new Float:dx = razlika / (KUGLANJE_KUGLA_R + KUGLANJE_CUNJ_R), Float:d = VectorSize(dx, 1.0, 0.0);
        smjer[k][0] = dx / d;
        smjer[k][1] = 1.0 / d;
        gKg[s][kgStoji][k] = false;
        gKg[s][kgPao][k] = true;
        red[n++] = k;
    }
    for (new i = 0; i < n; i++)
    {
        new a = red[i], Float:ax, Float:ay;
        KuglanjeCunjPoz(a, ax, ay);
        for (new k = 0; k < 10; k++)
        {
            if (!gKg[s][kgStoji][k]) continue;
            new Float:lx, Float:ly;
            KuglanjeCunjPoz(k, lx, ly);
            new Float:dx = lx - ax, Float:dy = ly - ay, Float:d = VectorSize(dx, dy, 0.0);
            if (d > KUGLANJE_LANAC || (dx * smjer[a][0] + dy * smjer[a][1]) / d < 0.6 || random(100) >= sansa) continue;
            smjer[k][0] = dx / d;
            smjer[k][1] = dy / d;
            gKg[s][kgStoji][k] = false;
            gKg[s][kgPao][k] = true;
            red[n++] = k;
        }
    }
    return n;
}

static KuglanjeBaci(playerid, s)
{
    new Float:bx = gKg[s][kgBx], Float:snaga = gKg[s][kgSnaga], Float:kraj;
    new Float:kut = gKg[s][kgKut] + float(random(201) - 100) / 100.0 * (0.25 + snaga * 0.007);
    gKg[s][kgNisani] = INVALID_PLAYER_ID;
    KuglanjeTdUnisti(playerid);
    ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, false, false, false, false, 0);
    new pali = KuglanjeSimuliraj(s, bx, kut, snaga, kraj), Float:wx, Float:wy, Float:brzina = 5.0 + snaga * 0.07;
    KuglanjeSvijet(s, bx + floattan(kut, degrees) * kraj, kraj, wx, wy);
    MoveDynamicObject(gKg[s][kgKugla], wx, wy, gKg[s][kgZ] + KUGLANJE_KUGLA_Z, brzina, float(random(360)), 90.0, 0.0);
    new Float:ax, Float:ay, Float:bx2, Float:by2, Float:lx, Float:ly, Float:cx, Float:cy, Float:z = gKg[s][kgZ], vrijeme = floatround(kraj / brzina * 1000.0);
    KuglanjeSvijet(s, bx, -1.0, ax, ay);
    KuglanjeSvijet(s, bx + floattan(kut, degrees) * (kraj - 4.0) * 0.5, kraj - 4.0, bx2, by2);
    KuglanjeSvijet(s, bx + floattan(kut, degrees) * KUGLANJE_DUZINA, KUGLANJE_DUZINA, lx, ly);
    KuglanjeSvijet(s, 0.0, KUGLANJE_DUZINA + 0.4, cx, cy);
    for (new i = 0; i < 2; i++)
    {
        new q = gKg[s][kgIgrac][i];
        if (q == INVALID_PLAYER_ID) continue;
        InterpolateCameraPos(q, ax, ay, z + 2.3, bx2, by2, z + 1.3, vrijeme, CAMERA_MOVE);
        InterpolateCameraLookAt(q, lx, ly, z + 0.2, cx, cy, z + 0.2, vrijeme, CAMERA_MOVE);
    }
    gKg[s][kgFaza] = KG_KOTRLJA;
    new n = gKg[s][kgNaPotezu];
    gKgRoll[s][n][gKgN[s][n]++] = pali;
    SetTimerEx("KuglanjeUdarac", vrijeme, false, "ii", s, gKg[s][kgToken]);
}

forward KuglanjeUdarac(s, token);
public KuglanjeUdarac(s, token)
{
    if (token != gKg[s][kgToken] || gKg[s][kgFaza] != KG_KOTRLJA) return;
    for (new k = 0; k < 10; k++)
    {
        if (!gKg[s][kgPao][k]) continue;
        new Float:x, Float:y, Float:z;
        GetDynamicObjectPos(gKg[s][kgCunj][k], x, y, z);
        MoveDynamicObject(gKg[s][kgCunj][k], x + float(random(61) - 30) / 100.0, y + float(random(61) - 30) / 100.0, gKg[s][kgZ] + KUGLANJE_CUNJ_Z, 3.0, 90.0, 0.0, float(random(360)));
    }
    new n = gKg[s][kgNaPotezu], p = gKg[s][kgIgrac][n], pali = gKgRoll[s][n][gKgN[s][n] - 1];
    new bool:cisto = true;
    for (new k = 0; k < 10; k++) if (gKg[s][kgStoji][k]) cisto = false;
    if (cisto && pali == 10) KuglanjePoruka(s, "[Kuglanje] %s: STRIKE!", p);
    else if (cisto) KuglanjePoruka(s, "[Kuglanje] %s: SPARE!", p);
    else KuglanjePoruka(s, "[Kuglanje] %s rusi %d cunjeva.", p, pali);
    gKg[s][kgFaza] = KG_PAUZA;
    SetTimerEx("KuglanjeDalje", KUGLANJE_PAUZA_MS, false, "ii", s, token);
}

forward KuglanjeDalje(s, token);
public KuglanjeDalje(s, token)
{
    if (token != gKg[s][kgToken] || gKg[s][kgFaza] != KG_PAUZA) return;
    KuglanjeKameraVrati(s);
    new n = gKg[s][kgNaPotezu], u, f = KuglanjeOkvir(gKgRoll[s][n], gKgN[s][n], u);
    new bool:cisto = true;
    for (new k = 0; k < 10; k++) if (gKg[s][kgStoji][k]) cisto = false;
    KuglanjeMakniPale(s);
    KuglanjeKuglaNazad(s);
    if (f < 10 && u > 0)
    {
        if (cisto) KuglanjePostavi(s);
        gKg[s][kgFaza] = KG_IGRA;
        KuglanjePotez(s);
        return;
    }
    new o = 1 - n, uo, fo = gKg[s][kgSam] ? 10 : KuglanjeOkvir(gKgRoll[s][o], gKgN[s][o], uo);
    if (f >= 10 && gKg[s][kgSam])
    {
        KuglanjePoruka(s, "[Kuglanje] %s je zavrsio igru s %d bodova.", gKg[s][kgIgrac][n], KuglanjeRezultat(gKgRoll[s][n], gKgN[s][n]));
        KuglanjeKraj(s);
        return;
    }
    if (f >= 10 && fo >= 10)
    {
        new a = KuglanjeRezultat(gKgRoll[s][0], gKgN[s][0]), b = KuglanjeRezultat(gKgRoll[s][1], gKgN[s][1]);
        KuglanjePobjeda(s, a == b ? -1 : a > b ? 0 : 1);
        return;
    }
    if (fo < 10) gKg[s][kgNaPotezu] = o;
    KuglanjePostavi(s);
    KuglanjePotez(s);
}

static KuglanjeSam(s, playerid)
{
    new i = gKg[s][kgIgrac][0] == playerid ? 0 : 1;
    OkladaZatvori(OK_KUGLANJE, s);
    gKg[s][kgSam] = true;
    gKgN[s][0] = gKgN[s][1] = 0;
    gKg[s][kgNaPotezu] = i;
    KuglanjePostavi(s);
    KuglanjeKuglaNazad(s);
    IgraObavijest(playerid, T(playerid, "Kuglanje"), KUGLANJE_BOJA, T(playerid, "[Kuglanje] Igrate sami, 10 okvira."), 0);
    KuglanjePotez(s);
}

KuglanjeTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    new s = gKuglanjeIgrac[playerid];
    if (s != -1 && gKg[s][kgFaza] == KG_CEKA && (newkeys & KEY_AIM) && !(oldkeys & KEY_AIM) && gKg[s][kgIgrac][gKg[s][kgIgrac][0] == playerid ? 1 : 0] == INVALID_PLAYER_ID)
    {
        if (!IsPlayerInRangeOfPoint(playerid, KUGLANJE_DOMET, gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ] + 1.0)) return IgraObavijest(playerid, T(playerid, "Kuglanje"), OBAVIJEST_GRESKA, T(playerid, "Stanite na liniju bacanja.")), 1;
        KuglanjeSam(s, playerid);
        KuglanjeNisani(playerid, s);
        return 1;
    }
    if (s == -1 || gKg[s][kgFaza] != KG_IGRA || gKg[s][kgIgrac][gKg[s][kgNaPotezu]] != playerid) return 0;
    if ((newkeys & KEY_AIM) && !(oldkeys & KEY_AIM))
    {
        if (gKg[s][kgNisani] == playerid)
        {
            KuglanjeNisanKraj(s);
            KuglanjeKuglaNazad(s);
            return 1;
        }
        if (!IsPlayerInRangeOfPoint(playerid, KUGLANJE_DOMET, gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ] + 1.0)) return IgraObavijest(playerid, T(playerid, "Kuglanje"), OBAVIJEST_GRESKA, T(playerid, "Stanite na liniju bacanja.")), 1;
        KuglanjeNisani(playerid, s);
        return 1;
    }
    if (gKg[s][kgNisani] == playerid && !(newkeys & KEY_FIRE) && (oldkeys & KEY_FIRE))
    {
        KuglanjeBaci(playerid, s);
        return 1;
    }
    return 0;
}

KuglanjeTick()
{
    for (new s = 0; s < KUGLANJE_MAX; s++)
    {
        if (!gKg[s][kgIma]) continue;
        for (new i = 0; i < 2; i++)
        {
            new p = gKg[s][kgIgrac][i];
            if (p == INVALID_PLAYER_ID || gKg[s][kgNisani] == p) continue;
            if (GetPlayerVirtualWorld(p) != gKg[s][kgSvijet] || !IsPlayerInRangeOfPoint(p, KUGLANJE_NAPUSTI, gKg[s][kgX], gKg[s][kgY], gKg[s][kgZ]))
            {
                IgraObavijest(p, T(p, "Kuglanje"), KUGLANJE_BOJA, T(p, "Napustili ste kuglanu."));
                KuglanjeOdlazi(p);
            }
        }
    }
}

ShowKuglane(playerid)
{
    new list[1024], red[96], len = 0;
    DodajStupac(list, len, T(playerid, "+ Nova staza (stanite na liniju bacanja, gledajte prema cunjevima)"));
    for (new s = 0, n = 0; s < KUGLANJE_MAX; s++)
    {
        if (!gKg[s][kgIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Kuglana (%s) - obrisi"), gKg[s][kgDb], CestaZona(gKg[s][kgX], gKg[s][kgY]));
        DodajStupac(list, len, red);
        gApKuglanje[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_KUGLANJE, DIALOG_STYLE_LIST, T(playerid, "Kuglane"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_kuglanje(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0, Float:x, Float:y, Float:z, Float:a;
        while (s < KUGLANJE_MAX && gKg[s][kgIma]) s++;
        if (s == KUGLANJE_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj kuglana.")), ShowKuglane(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bowling_lanes (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));
        gKg[s][kgDb] = ZadnjiId();
        gKg[s][kgX] = x;
        gKg[s][kgY] = y;
        gKg[s][kgZ] = z;
        gKg[s][kgA] = a;
        gKg[s][kgInt] = GetPlayerInterior(playerid);
        gKg[s][kgSvijet] = GetPlayerVirtualWorld(playerid);
        KuglanjeStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Staza za kuglanje je postavljena."));
        return ShowKuglane(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= KUGLANJE_MAX) return 1;
    new s = gApKuglanje[playerid][i];
    if (!gKg[s][kgIma]) return ShowKuglane(playerid);
    for (new k = 0; k < 2; k++) if (gKg[s][kgIgrac][k] != INVALID_PLAYER_ID) KuglanjeOdlazi(gKg[s][kgIgrac][k]);
    KuglanjeKraj(s);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bowling_lanes WHERE id = %d", gKg[s][kgDb]));
    for (new k = 0; k < 10; k++)
    {
        DestroyDynamicObject(gKg[s][kgCunj][k]);
        gKg[s][kgCunj][k] = STREAMER_TAG_OBJECT:0;
    }
    DestroyDynamicObject(gKg[s][kgKugla]);
    gKg[s][kgKugla] = STREAMER_TAG_OBJECT:0;
    DestroyDynamic3DTextLabel(gKg[s][kgTekst]);
    gKg[s][kgIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Staza za kuglanje je obrisana."));
    return ShowKuglane(playerid);
}
