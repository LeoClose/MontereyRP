#define BILIJAR_MAX         16
#define BILIJAR_KUGLI       16
#define BILIJAR_STOL        2964
#define BILIJAR_STAP        3004
#define BILIJAR_R           0.04
#define BILIJAR_RUPA        0.09
#define BILIJAR_X1          -0.964
#define BILIJAR_X2          0.976
#define BILIJAR_Y1          -0.51
#define BILIJAR_Y2          0.533
#define BILIJAR_VISINA      0.955
#define BILIJAR_DT          0.004
#define BILIJAR_TRENJE      0.9
#define BILIJAR_BANDA       0.75
#define BILIJAR_SUDAR       0.95
#define BILIJAR_KORAK       25
#define BILIJAR_OKVIRA      150
#define BILIJAR_ANIM_MS     100
#define BILIJAR_NISAN_MS    50
#define BILIJAR_DOMET       2.5
#define BILIJAR_NAPUSTI     12.0
#define BILIJAR_BIJELA_X    0.5

enum { BF_CEKA, BF_IGRA, BF_ANIM }
enum { BG_OTVORENO, BG_PUNE, BG_POLA }
enum E_BILIJAR { bool:bsIma, bsDb, Float:bsX, Float:bsY, Float:bsZ, Float:bsA, bsInt, bsSvijet, STREAMER_TAG_OBJECT:bsStol, STREAMER_TAG_3D_TEXT_LABEL:bsTekst,
    bsFaza, bsIgrac[2], bsNaPotezu, bsGrupa[2], bool:bsUbacena[BILIJAR_KUGLI], Float:bsKx[BILIJAR_KUGLI], Float:bsKy[BILIJAR_KUGLI], STREAMER_TAG_OBJECT:bsKugla[BILIJAR_KUGLI],
    STREAMER_TAG_OBJECT:bsStap, bsNisani, Float:bsKut, Float:bsSnaga, bool:bsSnagaGore, bsOkvir, bsOkvira, bsPrva, bsToken }

static const gBilijarModel[BILIJAR_KUGLI] = { 3003, 3002, 3100, 3101, 3102, 3103, 3104, 3105, 3106, 2995, 2996, 2997, 2998, 2999, 3000, 3001 };
static const Float:gBilijarSlaganje[BILIJAR_KUGLI][2] = {
    { 0.5, 0.0 }, { -0.3, 0.0 }, { -0.525, -0.04 }, { -0.375, 0.044 }, { -0.6, 0.079 }, { -0.525, 0.118 }, { -0.6, -0.157 }, { -0.45, -0.079 },
    { -0.45, 0.0 }, { -0.375, -0.044 }, { -0.45, 0.079 }, { -0.525, -0.118 }, { -0.6, -0.079 }, { -0.6, 0.0 }, { -0.6, 0.157 }, { -0.525, 0.04 }
};
static const Float:gBilijarRupe[][2] = { { 0.955, 0.51 }, { 0.955, -0.49 }, { 0.005, 0.55 }, { 0.007, -0.535 }, { -0.945, 0.513 }, { -0.945, -0.49 } };

static gBilijar[BILIJAR_MAX][E_BILIJAR], Float:gBilijarPut[BILIJAR_MAX][BILIJAR_OKVIRA][BILIJAR_KUGLI][2], gBilijarIgrac[MAX_PLAYERS] = { -1, ... }, gApBilijar[MAX_PLAYERS][BILIJAR_MAX];
static PlayerText:gBilijarTd[MAX_PLAYERS];

static BilijarSvijet(s, Float:lx, Float:ly, &Float:wx, &Float:wy)
{
    new Float:c = floatcos(gBilijar[s][bsA], degrees), Float:sn = floatsin(gBilijar[s][bsA], degrees);
    wx = gBilijar[s][bsX] + lx * c - ly * sn;
    wy = gBilijar[s][bsY] + lx * sn + ly * c;
}

static BilijarGrupa(k)
{
    return k >= 1 && k <= 7 ? BG_PUNE : k >= 9 ? BG_POLA : BG_OTVORENO;
}

InitBilijar()
{
    assert(BilijarGrupa(3) == BG_PUNE && BilijarGrupa(12) == BG_POLA && BilijarGrupa(8) == BG_OTVORENO);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS pool_tables (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM pool_tables LIMIT %d", BILIJAR_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gBilijar[s][bsDb] = DB_GetFieldInt(res, 0);
        gBilijar[s][bsX] = DB_GetFieldFloat(res, 1);
        gBilijar[s][bsY] = DB_GetFieldFloat(res, 2);
        gBilijar[s][bsZ] = DB_GetFieldFloat(res, 3);
        gBilijar[s][bsA] = DB_GetFieldFloat(res, 4);
        gBilijar[s][bsInt] = DB_GetFieldInt(res, 5);
        gBilijar[s][bsSvijet] = DB_GetFieldInt(res, 6);
        BilijarStvori(s);
    }
    DB_FreeResultSet(res);
}

static BilijarStvori(s)
{
    gBilijar[s][bsIma] = true;
    gBilijar[s][bsIgrac][0] = gBilijar[s][bsIgrac][1] = INVALID_PLAYER_ID;
    gBilijar[s][bsNisani] = INVALID_PLAYER_ID;
    gBilijar[s][bsStol] = CreateDynamicObject(BILIJAR_STOL, gBilijar[s][bsX], gBilijar[s][bsY], gBilijar[s][bsZ], 0.0, 0.0, gBilijar[s][bsA], gBilijar[s][bsSvijet], gBilijar[s][bsInt]);
    gBilijar[s][bsTekst] = CreateDynamic3DTextLabel("Bilijar\n[Y]", COLOR_ORG, gBilijar[s][bsX], gBilijar[s][bsY], gBilijar[s][bsZ] + 1.3, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gBilijar[s][bsSvijet], gBilijar[s][bsInt]);
    BilijarSlozi(s);
}

static BilijarKuglaPostavi(s, k)
{
    new Float:wx, Float:wy;
    BilijarSvijet(s, gBilijar[s][bsKx][k], gBilijar[s][bsKy][k], wx, wy);
    if (gBilijar[s][bsKugla][k]) SetDynamicObjectPos(gBilijar[s][bsKugla][k], wx, wy, gBilijar[s][bsZ] + BILIJAR_VISINA);
    else gBilijar[s][bsKugla][k] = CreateDynamicObject(gBilijarModel[k], wx, wy, gBilijar[s][bsZ] + BILIJAR_VISINA, 0.0, 0.0, 0.0, gBilijar[s][bsSvijet], gBilijar[s][bsInt], -1, 60.0);
}

static BilijarSlozi(s)
{
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        gBilijar[s][bsUbacena][k] = false;
        gBilijar[s][bsKx][k] = gBilijarSlaganje[k][0];
        gBilijar[s][bsKy][k] = gBilijarSlaganje[k][1];
        BilijarKuglaPostavi(s, k);
    }
    gBilijar[s][bsGrupa][0] = gBilijar[s][bsGrupa][1] = BG_OTVORENO;
}

static BilijarObjekteMakni(s)
{
    DestroyDynamicObject(gBilijar[s][bsStol]);
    DestroyDynamic3DTextLabel(gBilijar[s][bsTekst]);
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        if (gBilijar[s][bsKugla][k]) DestroyDynamicObject(gBilijar[s][bsKugla][k]);
        gBilijar[s][bsKugla][k] = STREAMER_TAG_OBJECT:0;
    }
}

static BilijarPoruka(s, const tekst[], igrac = INVALID_PLAYER_ID, drugi = INVALID_PLAYER_ID)
{
    for (new i = 0; i < 2; i++)
    {
        new p = gBilijar[s][bsIgrac][i];
        if (p == INVALID_PLAYER_ID) continue;
        if (drugi != INVALID_PLAYER_ID) SendClientMessage(p, 0x2DD9A9FF, T(p, tekst), GetRPIme(igrac), GetRPIme(drugi));
        else if (igrac != INVALID_PLAYER_ID) SendClientMessage(p, 0x2DD9A9FF, T(p, tekst), GetRPIme(igrac));
        else SendClientMessage(p, 0x2DD9A9FF, T(p, tekst));
    }
}

static BilijarNisanKraj(s)
{
    new p = gBilijar[s][bsNisani];
    gBilijar[s][bsNisani] = INVALID_PLAYER_ID;
    if (gBilijar[s][bsStap])
    {
        DestroyDynamicObject(gBilijar[s][bsStap]);
        gBilijar[s][bsStap] = STREAMER_TAG_OBJECT:0;
    }
    if (p == INVALID_PLAYER_ID || !IsPlayerConnected(p)) return;
    PlayerTextDrawDestroy(p, gBilijarTd[p]);
    TogglePlayerControllable(p, true);
    SetCameraBehindPlayer(p);
}

static BilijarKraj(s)
{
    BilijarToken(s);
    BilijarNisanKraj(s);
    for (new i = 0; i < 2; i++)
    {
        if (gBilijar[s][bsIgrac][i] != INVALID_PLAYER_ID) gBilijarIgrac[gBilijar[s][bsIgrac][i]] = -1;
        gBilijar[s][bsIgrac][i] = INVALID_PLAYER_ID;
    }
    gBilijar[s][bsFaza] = BF_CEKA;
    BilijarSlozi(s);
}

static BilijarToken(s)
{
    gBilijar[s][bsToken]++;
}

static BilijarPobjeda(s, pobjednik)
{
    new p = gBilijar[s][bsIgrac][pobjednik], g = gBilijar[s][bsIgrac][1 - pobjednik];
    if (p != INVALID_PLAYER_ID && g != INVALID_PLAYER_ID) BilijarPoruka(s, "[Bilijar] %s je pobijedio protiv %s!", p, g);
    BilijarKraj(s);
}

BilijarIgracOdlazi(playerid)
{
    new s = gBilijarIgrac[playerid];
    if (s == -1) return;
    new i = gBilijar[s][bsIgrac][0] == playerid ? 0 : 1;
    if (gBilijar[s][bsFaza] != BF_CEKA && gBilijar[s][bsIgrac][1 - i] != INVALID_PLAYER_ID)
    {
        BilijarPoruka(s, "[Bilijar] %s je napustio igru.", playerid);
        BilijarPobjeda(s, 1 - i);
        return;
    }
    if (gBilijar[s][bsNisani] == playerid) BilijarNisanKraj(s);
    gBilijar[s][bsIgrac][i] = INVALID_PLAYER_ID;
    gBilijarIgrac[playerid] = -1;
}

static BilijarBlizu(playerid, Float:domet)
{
    for (new s = 0; s < BILIJAR_MAX; s++)
        if (gBilijar[s][bsIma] && GetPlayerVirtualWorld(playerid) == gBilijar[s][bsSvijet] && GetPlayerInterior(playerid) == gBilijar[s][bsInt] && IsPlayerInRangeOfPoint(playerid, domet, gBilijar[s][bsX], gBilijar[s][bsY], gBilijar[s][bsZ] + 1.0)) return s;
    return -1;
}

static BilijarPotez(s)
{
    new p = gBilijar[s][bsIgrac][gBilijar[s][bsNaPotezu]];
    for (new i = 0; i < 2; i++)
    {
        new q = gBilijar[s][bsIgrac][i];
        if (q == INVALID_PLAYER_ID) continue;
        if (q == p) GameTextForPlayer(q, T(q, "~g~Vi ste na potezu~n~~w~RMB - nisani"), 3000, 4);
        else GameTextForPlayer(q, T(q, "~y~Protivnik je na potezu"), 3000, 4);
    }
}

BilijarKey(playerid)
{
    if (gBilijarIgrac[playerid] == -1 && BilijarBlizu(playerid, BILIJAR_DOMET) == -1) return 0;
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = gBilijarIgrac[playerid];
    if (s != -1)
    {
        BilijarIgracOdlazi(playerid);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Napustili ste bilijarski stol."));
        return 1;
    }
    s = BilijarBlizu(playerid, BILIJAR_DOMET);
    if (s == -1) return 0;
    new i = gBilijar[s][bsIgrac][0] == INVALID_PLAYER_ID ? 0 : gBilijar[s][bsIgrac][1] == INVALID_PLAYER_ID ? 1 : -1;
    if (i == -1 || gBilijar[s][bsFaza] != BF_CEKA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za ovim stolom se vec igra.")), 1;
    gBilijar[s][bsIgrac][i] = playerid;
    gBilijarIgrac[playerid] = s;
    if (gBilijar[s][bsIgrac][1 - i] == INVALID_PLAYER_ID) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Sjeli ste za bilijarski stol. Ceka se protivnik (Y za odlazak).")), 1;
    BilijarSlozi(s);
    gBilijar[s][bsFaza] = BF_IGRA;
    gBilijar[s][bsNaPotezu] = random(2);
    BilijarPoruka(s, "[Bilijar] Igra pocinje! Razbija %s.", gBilijar[s][bsIgrac][gBilijar[s][bsNaPotezu]]);
    BilijarPotez(s);
    return 1;
}

static BilijarNisanPostavi(s)
{
    new p = gBilijar[s][bsNisani], Float:bx, Float:by, Float:z = gBilijar[s][bsZ] + BILIJAR_VISINA, Float:h = gBilijar[s][bsKut], Float:dx = -floatsin(h, degrees), Float:dy = floatcos(h, degrees);
    BilijarSvijet(s, gBilijar[s][bsKx][0], gBilijar[s][bsKy][0], bx, by);
    if (gBilijar[s][bsStap]) SetDynamicObjectPos(gBilijar[s][bsStap], bx - dx * 0.085, by - dy * 0.085, z);
    else gBilijar[s][bsStap] = CreateDynamicObject(BILIJAR_STAP, bx - dx * 0.085, by - dy * 0.085, z, 7.0, 0.0, h + 180.0, gBilijar[s][bsSvijet], gBilijar[s][bsInt]);
    SetDynamicObjectRot(gBilijar[s][bsStap], 7.0, 0.0, h + 180.0);
    SetPlayerCameraPos(p, bx - dx * 1.1, by - dy * 1.1, z + 0.55);
    SetPlayerCameraLookAt(p, bx + dx * 0.6, by + dy * 0.6, z);
    new tekst[64], crte = floatround(gBilijar[s][bsSnaga] / 5.0);
    for (new i = 0; i < 20; i++) tekst[i] = i < crte ? 'I' : '.';
    tekst[20] = EOS;
    format(tekst, sizeof tekst, "~w~%s ~r~%s", T(p, "SNAGA"), tekst);
    PlayerTextDrawSetString(p, gBilijarTd[p], tekst);
}

forward BilijarNisanTick(s, token);
public BilijarNisanTick(s, token)
{
    new p = gBilijar[s][bsNisani];
    if (token != gBilijar[s][bsToken] || p == INVALID_PLAYER_ID) return;
    if (!IsPlayerConnected(p) || gBilijarIgrac[p] != s)
    {
        BilijarNisanKraj(s);
        return;
    }
    new KEY:tipke, gore, strana;
    GetPlayerKeys(p, tipke, gore, strana);
    if (strana) gBilijar[s][bsKut] -= float(strana) / 128.0 * ((tipke & KEY_WALK) ? 0.4 : 2.5);
    if (tipke & KEY_FIRE)
    {
        gBilijar[s][bsSnaga] += gBilijar[s][bsSnagaGore] ? 4.0 : -4.0;
        if (gBilijar[s][bsSnaga] >= 100.0) gBilijar[s][bsSnaga] = 100.0, gBilijar[s][bsSnagaGore] = false;
        if (gBilijar[s][bsSnaga] <= 0.0) gBilijar[s][bsSnaga] = 0.0, gBilijar[s][bsSnagaGore] = true;
    }
    BilijarNisanPostavi(s);
    SetTimerEx("BilijarNisanTick", BILIJAR_NISAN_MS, false, "ii", s, token);
}

BilijarTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    new s = gBilijarIgrac[playerid];
    if (s == -1 || gBilijar[s][bsFaza] != BF_IGRA || gBilijar[s][bsIgrac][gBilijar[s][bsNaPotezu]] != playerid) return 0;
    if ((newkeys & KEY_AIM) && !(oldkeys & KEY_AIM))
    {
        if (gBilijar[s][bsNisani] == playerid)
        {
            BilijarNisanKraj(s);
            ClearAnimations(playerid);
            return 1;
        }
        new Float:bx, Float:by, Float:px, Float:py, Float:pz;
        BilijarSvijet(s, gBilijar[s][bsKx][0], gBilijar[s][bsKy][0], bx, by);
        GetPlayerPos(playerid, px, py, pz);
        if (VectorSize(px - bx, py - by, 0.0) > 2.2) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pridite blize bijeloj kugli.")), 1;
        gBilijar[s][bsNisani] = playerid;
        gBilijar[s][bsKut] = atan2(by - py, bx - px) - 90.0;
        gBilijar[s][bsSnaga] = 0.0;
        gBilijar[s][bsSnagaGore] = true;
        TogglePlayerControllable(playerid, false);
        SetPlayerFacingAngle(playerid, gBilijar[s][bsKut]);
        ApplyAnimation(playerid, "POOL", "POOL_Long_Start", 4.1, false, true, true, true, 0);
        gBilijarTd[playerid] = CreatePlayerTextDraw(playerid, 320.0, 380.0, "_");
        PlayerTextDrawAlignment(playerid, gBilijarTd[playerid], TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawLetterSize(playerid, gBilijarTd[playerid], 0.3, 1.3);
        PlayerTextDrawFont(playerid, gBilijarTd[playerid], TEXT_DRAW_FONT_2);
        PlayerTextDrawSetOutline(playerid, gBilijarTd[playerid], 1);
        PlayerTextDrawShow(playerid, gBilijarTd[playerid]);
        BilijarNisanPostavi(s);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "A/D - nisan (ALT precizno), drzi LMB - snaga, pusti LMB - udarac, RMB - odustani."));
        SetTimerEx("BilijarNisanTick", BILIJAR_NISAN_MS, false, "ii", s, gBilijar[s][bsToken]);
        return 1;
    }
    if (gBilijar[s][bsNisani] == playerid && !(newkeys & KEY_FIRE) && (oldkeys & KEY_FIRE))
    {
        new Float:brzina = 0.4 + gBilijar[s][bsSnaga] / 100.0 * 4.1, Float:h = gBilijar[s][bsKut];
        BilijarNisanKraj(s);
        ApplyAnimation(playerid, "POOL", "POOL_Long_Shot", 4.1, false, true, true, false, 0);
        PlayerPlaySound(playerid, 31810, 0.0, 0.0, 0.0);
        new Float:c = floatcos(gBilijar[s][bsA], degrees), Float:sn = floatsin(gBilijar[s][bsA], degrees), Float:wx = -floatsin(h, degrees) * brzina, Float:wy = floatcos(h, degrees) * brzina;
        BilijarSimuliraj(s, wx * c + wy * sn, -wx * sn + wy * c);
        return 1;
    }
    return 0;
}

static BilijarSimuliraj(s, Float:vx0, Float:vy0)
{
    new Float:x[BILIJAR_KUGLI], Float:y[BILIJAR_KUGLI], Float:vx[BILIJAR_KUGLI], Float:vy[BILIJAR_KUGLI], bool:u[BILIJAR_KUGLI], okvir = 0, korak = 0, prva = -1;
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        x[k] = gBilijar[s][bsKx][k];
        y[k] = gBilijar[s][bsKy][k];
        vx[k] = vy[k] = 0.0;
        u[k] = gBilijar[s][bsUbacena][k];
    }
    vx[0] = vx0;
    vy[0] = vy0;
    new Float:d2 = (2.0 * BILIJAR_R) * (2.0 * BILIJAR_R), Float:pad = BILIJAR_TRENJE * BILIJAR_DT;
    for (;;)
    {
        new bool:mice = false;
        for (new k = 0; k < BILIJAR_KUGLI; k++)
        {
            if (u[k]) continue;
            new Float:v = VectorSize(vx[k], vy[k], 0.0);
            if (v <= pad)
            {
                vx[k] = vy[k] = 0.0;
                continue;
            }
            mice = true;
            vx[k] -= vx[k] / v * pad;
            vy[k] -= vy[k] / v * pad;
            x[k] += vx[k] * BILIJAR_DT;
            y[k] += vy[k] * BILIJAR_DT;
            for (new r = 0; r < sizeof gBilijarRupe; r++)
            {
                if (VectorSize(x[k] - gBilijarRupe[r][0], y[k] - gBilijarRupe[r][1], 0.0) >= BILIJAR_RUPA) continue;
                u[k] = true;
                x[k] = gBilijarRupe[r][0];
                y[k] = gBilijarRupe[r][1];
                vx[k] = vy[k] = 0.0;
                break;
            }
            if (u[k]) continue;
            if (x[k] < BILIJAR_X1 + BILIJAR_R && vx[k] < 0.0) x[k] = BILIJAR_X1 + BILIJAR_R, vx[k] = -vx[k] * BILIJAR_BANDA;
            if (x[k] > BILIJAR_X2 - BILIJAR_R && vx[k] > 0.0) x[k] = BILIJAR_X2 - BILIJAR_R, vx[k] = -vx[k] * BILIJAR_BANDA;
            if (y[k] < BILIJAR_Y1 + BILIJAR_R && vy[k] < 0.0) y[k] = BILIJAR_Y1 + BILIJAR_R, vy[k] = -vy[k] * BILIJAR_BANDA;
            if (y[k] > BILIJAR_Y2 - BILIJAR_R && vy[k] > 0.0) y[k] = BILIJAR_Y2 - BILIJAR_R, vy[k] = -vy[k] * BILIJAR_BANDA;
        }
        for (new a = 0; a < BILIJAR_KUGLI; a++)
        {
            if (u[a]) continue;
            for (new b = a + 1; b < BILIJAR_KUGLI; b++)
            {
                if (u[b]) continue;
                new Float:dx = x[b] - x[a], Float:dy = y[b] - y[a], Float:dd = dx * dx + dy * dy;
                if (dd >= d2 || dd == 0.0) continue;
                new Float:d = floatsqroot(dd), Float:nx = dx / d, Float:ny = dy / d, Float:rel = (vx[a] - vx[b]) * nx + (vy[a] - vy[b]) * ny;
                if (rel <= 0.0) continue;
                if (a == 0 && prva == -1) prva = b;
                new Float:imp = rel * (1.0 + BILIJAR_SUDAR) / 2.0, Float:pomak = (2.0 * BILIJAR_R - d) / 2.0;
                vx[a] -= imp * nx;
                vy[a] -= imp * ny;
                vx[b] += imp * nx;
                vy[b] += imp * ny;
                x[a] -= nx * pomak;
                y[a] -= ny * pomak;
                x[b] += nx * pomak;
                y[b] += ny * pomak;
                mice = true;
            }
        }
        if (++korak >= BILIJAR_KORAK || !mice)
        {
            korak = 0;
            for (new k = 0; k < BILIJAR_KUGLI; k++)
            {
                gBilijarPut[s][okvir][k][0] = x[k];
                gBilijarPut[s][okvir][k][1] = y[k];
            }
            if (++okvir >= BILIJAR_OKVIRA || !mice) break;
        }
    }
    gBilijar[s][bsOkvira] = okvir;
    gBilijar[s][bsOkvir] = 0;
    gBilijar[s][bsPrva] = prva;
    gBilijar[s][bsFaza] = BF_ANIM;
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        gBilijar[s][bsKx][k] = x[k];
        gBilijar[s][bsKy][k] = y[k];
    }
    SetTimerEx("BilijarAnim", BILIJAR_ANIM_MS, false, "ii", s, gBilijar[s][bsToken]);
}

forward BilijarAnim(s, token);
public BilijarAnim(s, token)
{
    if (token != gBilijar[s][bsToken] || gBilijar[s][bsFaza] != BF_ANIM) return;
    new f = gBilijar[s][bsOkvir]++;
    if (f >= gBilijar[s][bsOkvira])
    {
        BilijarPravila(s);
        return;
    }
    new Float:z = gBilijar[s][bsZ] + BILIJAR_VISINA;
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        if (gBilijar[s][bsUbacena][k]) continue;
        new Float:lx = gBilijarPut[s][f][k][0], Float:ly = gBilijarPut[s][f][k][1], Float:px = f ? gBilijarPut[s][f - 1][k][0] : lx, Float:py = f ? gBilijarPut[s][f - 1][k][1] : ly;
        new Float:put = VectorSize(lx - px, ly - py, 0.0);
        if (put < 0.001) continue;
        new Float:wx, Float:wy;
        BilijarSvijet(s, lx, ly, wx, wy);
        MoveDynamicObject(gBilijar[s][bsKugla][k], wx, wy, z, put * 1000.0 / BILIJAR_ANIM_MS);
    }
    SetTimerEx("BilijarAnim", BILIJAR_ANIM_MS, false, "ii", s, token);
}

static BilijarPravila(s)
{
    new n = gBilijar[s][bsNaPotezu], igrac = gBilijar[s][bsIgrac][n], moja = gBilijar[s][bsGrupa][n], bool:prekrsaj = false, bool:svoja = false, bool:osma = false, prvaGrupa = gBilijar[s][bsPrva] == -1 ? -1 : BilijarGrupa(gBilijar[s][bsPrva]);
    new bool:bijela = false, ubacene[BILIJAR_KUGLI], nu = 0, ostalo = 0;
    for (new k = 0; k < BILIJAR_KUGLI; k++)
    {
        if (gBilijar[s][bsUbacena][k]) continue;
        new Float:x = gBilijar[s][bsKx][k], Float:y = gBilijar[s][bsKy][k];
        for (new r = 0; r < sizeof gBilijarRupe; r++)
        {
            if (x != gBilijarRupe[r][0] || y != gBilijarRupe[r][1]) continue;
            gBilijar[s][bsUbacena][k] = true;
            ubacene[nu++] = k;
            break;
        }
    }
    for (new i = 0; i < nu; i++)
    {
        new k = ubacene[i];
        SetDynamicObjectPos(gBilijar[s][bsKugla][k], gBilijar[s][bsX], gBilijar[s][bsY], gBilijar[s][bsZ] - 2.0);
        if (k == 0) bijela = true;
        else if (k == 8) osma = true;
        else if (moja == BG_OTVORENO || BilijarGrupa(k) == moja) svoja = true;
    }
    if (bijela || gBilijar[s][bsPrva] == -1) prekrsaj = true;
    for (new k = 1; k < BILIJAR_KUGLI; k++) if (k != 8 && !gBilijar[s][bsUbacena][k] && (moja == BG_OTVORENO || BilijarGrupa(k) == moja)) ostalo++;
    if (moja != BG_OTVORENO && prvaGrupa != -1)
    {
        if (ostalo + (svoja ? 1 : 0) > 0 && prvaGrupa != moja) prekrsaj = true;
    }
    if (osma)
    {
        if (moja == BG_OTVORENO || ostalo > 0 || prekrsaj)
        {
            BilijarPoruka(s, "[Bilijar] %s je ubacio crnu osmicu prerano.", igrac);
            BilijarPobjeda(s, 1 - n);
        }
        else BilijarPobjeda(s, n);
        return;
    }
    if (moja == BG_OTVORENO && !prekrsaj)
    {
        for (new i = 0; i < nu; i++)
        {
            if (ubacene[i] == 0) continue;
            gBilijar[s][bsGrupa][n] = BilijarGrupa(ubacene[i]);
            gBilijar[s][bsGrupa][1 - n] = gBilijar[s][bsGrupa][n] == BG_PUNE ? BG_POLA : BG_PUNE;
            BilijarPoruka(s, gBilijar[s][bsGrupa][n] == BG_PUNE ? ("[Bilijar] %s igra s punim kuglama.") : ("[Bilijar] %s igra s praznim kuglama."), igrac);
            break;
        }
    }
    if (bijela)
    {
        gBilijar[s][bsUbacena][0] = false;
        gBilijar[s][bsKx][0] = BILIJAR_BIJELA_X;
        gBilijar[s][bsKy][0] = 0.0;
        for (new k = 1; k < BILIJAR_KUGLI; k++)
            if (!gBilijar[s][bsUbacena][k] && VectorSize(gBilijar[s][bsKx][k] - BILIJAR_BIJELA_X, gBilijar[s][bsKy][k] - gBilijar[s][bsKy][0], 0.0) < 2.0 * BILIJAR_R) gBilijar[s][bsKy][0] += 2.5 * BILIJAR_R;
        BilijarKuglaPostavi(s, 0);
    }
    gBilijar[s][bsFaza] = BF_IGRA;
    if (prekrsaj)
    {
        BilijarPoruka(s, "[Bilijar] Prekrsaj! %s je na potezu.", gBilijar[s][bsIgrac][1 - n]);
        gBilijar[s][bsNaPotezu] = 1 - n;
    }
    else if (!svoja) gBilijar[s][bsNaPotezu] = 1 - n;
    BilijarPotez(s);
}

BilijarTick()
{
    for (new s = 0; s < BILIJAR_MAX; s++)
    {
        if (!gBilijar[s][bsIma]) continue;
        for (new i = 0; i < 2; i++)
        {
            new p = gBilijar[s][bsIgrac][i];
            if (p == INVALID_PLAYER_ID || gBilijar[s][bsNisani] == p) continue;
            if (GetPlayerVirtualWorld(p) != gBilijar[s][bsSvijet] || !IsPlayerInRangeOfPoint(p, BILIJAR_NAPUSTI, gBilijar[s][bsX], gBilijar[s][bsY], gBilijar[s][bsZ]))
            {
                SendClientMessage(p, COLOR_INFO, T(p, "Napustili ste bilijarski stol."));
                BilijarIgracOdlazi(p);
            }
        }
    }
}

ShowBilijari(playerid)
{
    new list[1024], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "+ Novi bilijarski stol na mojoj poziciji"));
    for (new s = 0; s < BILIJAR_MAX; s++)
    {
        if (!gBilijar[s][bsIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Bilijarski stol (%s) - obrisi"), gBilijar[s][bsDb], CestaZona(gBilijar[s][bsX], gBilijar[s][bsY]));
        DodajStupac(list, len, red);
        gApBilijar[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_BILIJAR, DIALOG_STYLE_LIST, T(playerid, "Bilijarski stolovi"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_bilijar(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0, Float:x, Float:y, Float:z, Float:a;
        while (s < BILIJAR_MAX && gBilijar[s][bsIma]) s++;
        if (s == BILIJAR_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj bilijarskih stolova.")), ShowBilijari(playerid);
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        z -= 1.0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO pool_tables (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", x, y, z, a, GetPlayerInterior(playerid), GetPlayerVirtualWorld(playerid)));
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
        gBilijar[s][bsDb] = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        gBilijar[s][bsX] = x;
        gBilijar[s][bsY] = y;
        gBilijar[s][bsZ] = z;
        gBilijar[s][bsA] = a;
        gBilijar[s][bsInt] = GetPlayerInterior(playerid);
        gBilijar[s][bsSvijet] = GetPlayerVirtualWorld(playerid);
        BilijarStvori(s);
        SetPlayerPos(playerid, x + 1.6 * floatcos(a, degrees), y + 1.6 * floatsin(a, degrees), z + 1.0);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Bilijarski stol je postavljen."));
        return ShowBilijari(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= BILIJAR_MAX) return 1;
    new s = gApBilijar[playerid][i];
    if (!gBilijar[s][bsIma]) return ShowBilijari(playerid);
    for (new k = 0; k < 2; k++) if (gBilijar[s][bsIgrac][k] != INVALID_PLAYER_ID) BilijarIgracOdlazi(gBilijar[s][bsIgrac][k]);
    BilijarKraj(s);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM pool_tables WHERE id = %d", gBilijar[s][bsDb]));
    BilijarObjekteMakni(s);
    gBilijar[s][bsIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Bilijarski stol je obrisan."));
    return ShowBilijari(playerid);
}
