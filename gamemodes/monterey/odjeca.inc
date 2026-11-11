#define ODJECA_MAX              16
#define ODJECA_SVIJET           4000
#define ODJECA_DOMET            2.0
#define ODJECA_KASA_DOMET       2.5
#define ODJECA_ARTIKALA         48
#define ODJECA_KATALOG          1
#define ODJECA_TD               16
#define ODJECA_SLOT_KAPA        8
#define ODJECA_SLOT_FRIZURA     7

enum { ODJ_PRODAVAONICA, ODJ_FRIZER }
enum { ODJ_SKIN, ODJ_KAPA, ODJ_FRIZURA }
enum { OTD_INFO, OTD_LIJEVO, OTD_DESNO, OTD_KUPI, OTD_IZLAZ, OTD_ODJECA, OTD_KAPE }

enum E_ODJECA { bool:odIma, odDb, odTip, Float:odX, Float:odY, Float:odZ, Float:odA, bool:odImaAktor, Float:odAx, Float:odAy, Float:odAz, Float:odAa, odAktor,
    bool:odImaKabina, Float:odKx, Float:odKy, Float:odKz, Float:odKa,
    STREAMER_TAG_PICKUP:odPickup, STREAMER_TAG_PICKUP:odPickupU, STREAMER_TAG_3D_TEXT_LABEL:odTekst, STREAMER_TAG_3D_TEXT_LABEL:odTekstU, STREAMER_TAG_3D_TEXT_LABEL:odTekstA }

enum E_PRIVJESAK { pkModel, Float:pkX, Float:pkY, Float:pkZ, Float:pkRx, Float:pkRy, Float:pkRz }

static const gOdjecaInterijer[2] = { 15, 2 };
static const Float:gOdjecaUlaz[2][3] = { { 207.737, -109.019, 1005.133 }, { 411.626, -21.433, 1001.805 } };
static const gOdjecaSkin[2] = { 211, 176 };
static const gOdjecaVrste[][] = { "Skin", "Kapa", "Frizura" };
static const gOdjecaZadano[][3] = {
    { ODJ_SKIN, 1, 150 }, { ODJ_SKIN, 2, 150 }, { ODJ_SKIN, 7, 200 }, { ODJ_SKIN, 14, 200 }, { ODJ_SKIN, 15, 250 }, { ODJ_SKIN, 19, 250 },
    { ODJ_SKIN, 22, 300 }, { ODJ_SKIN, 23, 300 }, { ODJ_SKIN, 29, 350 }, { ODJ_SKIN, 46, 500 }, { ODJ_SKIN, 47, 500 }, { ODJ_SKIN, 60, 400 },
    { ODJ_SKIN, 12, 300 }, { ODJ_SKIN, 13, 250 }, { ODJ_SKIN, 40, 400 }, { ODJ_SKIN, 41, 300 }, { ODJ_SKIN, 56, 250 }, { ODJ_SKIN, 69, 350 },
    { ODJ_KAPA, 19006, 80 }, { ODJ_KAPA, 19007, 80 }, { ODJ_KAPA, 19022, 120 }, { ODJ_KAPA, 18891, 50 }, { ODJ_KAPA, 18892, 50 }, { ODJ_KAPA, 18893, 50 },
    { ODJ_KAPA, 18926, 100 }, { ODJ_KAPA, 18927, 100 }, { ODJ_KAPA, 18939, 100 },
    { ODJ_FRIZURA, 18640, 150 }, { ODJ_FRIZURA, 18975, 150 }, { ODJ_FRIZURA, 19077, 200 }, { ODJ_FRIZURA, 19136, 250 }, { ODJ_FRIZURA, 19274, 200 }
};

static gOdj[ODJECA_MAX][E_ODJECA];
static gOdjArtikl[ODJECA_MAX][ODJECA_ARTIKALA][3], gOdjArtiklDb[ODJECA_MAX][ODJECA_ARTIKALA], gOdjArtikala[ODJECA_MAX];
static gApOdjeca[MAX_PLAYERS];
static gOdjecaU[MAX_PLAYERS], gOdjecaVrsta[MAX_PLAYERS], gOdjecaIdx[MAX_PLAYERS], gOdjecaStariSkin[MAX_PLAYERS], gOdjecaNosiSkin[MAX_PLAYERS];
static gOdjecaNosi[MAX_PLAYERS][2][E_PRIVJESAK];
static PlayerText:gOdjecaTd[MAX_PLAYERS][ODJECA_TD], gOdjecaBroj[MAX_PLAYERS], PlayerText:gOdjecaGumb[MAX_PLAYERS][7];

InitOdjeca()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS clothing_shops (id INTEGER PRIMARY KEY AUTOINCREMENT, type INTEGER NOT NULL DEFAULT 0, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, \
        Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0, \
        Kabina INTEGER NOT NULL DEFAULT 0, kx REAL NOT NULL DEFAULT 0, ky REAL NOT NULL DEFAULT 0, kz REAL NOT NULL DEFAULT 0, ka REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS store_items (id INTEGER PRIMARY KEY AUTOINCREMENT, store_type INTEGER NOT NULL, store INTEGER NOT NULL, kind INTEGER NOT NULL DEFAULT 0, model INTEGER NOT NULL, amount INTEGER NOT NULL DEFAULT 1, price INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS player_style (account INTEGER PRIMARY KEY, skin INTEGER NOT NULL DEFAULT -1, \
        kapa INTEGER NOT NULL DEFAULT 0, kx REAL NOT NULL DEFAULT 0, ky REAL NOT NULL DEFAULT 0, kz REAL NOT NULL DEFAULT 0, krx REAL NOT NULL DEFAULT 0, kry REAL NOT NULL DEFAULT 0, krz REAL NOT NULL DEFAULT 0, \
        frizura INTEGER NOT NULL DEFAULT 0, fx REAL NOT NULL DEFAULT 0, fy REAL NOT NULL DEFAULT 0, fz REAL NOT NULL DEFAULT 0, frx REAL NOT NULL DEFAULT 0, fry REAL NOT NULL DEFAULT 0, frz REAL NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, type, x, y, z, a, Aktor, ax, ay, az, aa, Kabina, kx, ky, kz, ka FROM clothing_shops ORDER BY id LIMIT %d", ODJECA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gOdj[s][odDb] = DB_GetFieldInt(res, 0);
        gOdj[s][odTip] = clamp(DB_GetFieldInt(res, 1), ODJ_PRODAVAONICA, ODJ_FRIZER);
        gOdj[s][odX] = DB_GetFieldFloat(res, 2);
        gOdj[s][odY] = DB_GetFieldFloat(res, 3);
        gOdj[s][odZ] = DB_GetFieldFloat(res, 4);
        gOdj[s][odA] = DB_GetFieldFloat(res, 5);
        OdjecaStvori(s);
        OdjecaUcitajArtikle(s);
        if (DB_GetFieldInt(res, 6))
        {
            gOdj[s][odAx] = DB_GetFieldFloat(res, 7);
            gOdj[s][odAy] = DB_GetFieldFloat(res, 8);
            gOdj[s][odAz] = DB_GetFieldFloat(res, 9);
            gOdj[s][odAa] = DB_GetFieldFloat(res, 10);
            OdjecaAktor(s);
        }
        gOdj[s][odImaKabina] = DB_GetFieldInt(res, 11) != 0;
        gOdj[s][odKx] = DB_GetFieldFloat(res, 12);
        gOdj[s][odKy] = DB_GetFieldFloat(res, 13);
        gOdj[s][odKz] = DB_GetFieldFloat(res, 14);
        gOdj[s][odKa] = DB_GetFieldFloat(res, 15);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static bool:OdjecaVrstaUTrgovini(s, vrsta)
{
    return gOdj[s][odTip] == ODJ_FRIZER ? vrsta == ODJ_FRIZURA : vrsta != ODJ_FRIZURA;
}

static OdjecaUcitajArtikle(s, bool:zadano = true)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, kind, model, price FROM store_items WHERE store_type = %d AND store = %d ORDER BY kind, id LIMIT %d", ODJECA_KATALOG, gOdj[s][odDb], ODJECA_ARTIKALA);
    gOdjArtikala[s] = 0;
    for (new n = DB_GetRowCount(res); gOdjArtikala[s] < n; DB_SelectNextRow(res))
    {
        new a = gOdjArtikala[s]++;
        gOdjArtiklDb[s][a] = DB_GetFieldInt(res, 0);
        gOdjArtikl[s][a][0] = DB_GetFieldInt(res, 1);
        gOdjArtikl[s][a][1] = DB_GetFieldInt(res, 2);
        gOdjArtikl[s][a][2] = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
    if (gOdjArtikala[s] || !zadano) return;
    for (new i = 0; i < sizeof gOdjecaZadano; i++)
    {
        if (!OdjecaVrstaUTrgovini(s, gOdjecaZadano[i][0])) continue;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO store_items (store_type, store, kind, model, price) VALUES (%d, %d, %d, %d, %d)", ODJECA_KATALOG, gOdj[s][odDb], gOdjecaZadano[i][0], gOdjecaZadano[i][1], gOdjecaZadano[i][2]));
    }
    OdjecaUcitajArtikle(s, false);
}

static OdjecaBrojVrste(s, vrsta)
{
    new n = vrsta == ODJ_SKIN ? 0 : 1;
    for (new a = 0; a < gOdjArtikala[s]; a++) if (gOdjArtikl[s][a][0] == vrsta) n++;
    return n;
}

static OdjecaArtikl(s, vrsta, idx)
{
    if (vrsta != ODJ_SKIN && !idx--) return -1;
    for (new a = 0; a < gOdjArtikala[s]; a++) if (gOdjArtikl[s][a][0] == vrsta && !idx--) return a;
    return -1;
}

static OdjecaStvori(s)
{
    new t = gOdj[s][odTip], svijet = ODJECA_SVIJET + s;
    gOdj[s][odIma] = true;
    gOdj[s][odPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ], 0, 0);
    gOdj[s][odTekst] = CreateDynamic3DTextLabel(t == ODJ_FRIZER ? ("Frizer\n[Y]") : ("Odjeca\n[Y]"), COLOR_ORG, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gOdj[s][odPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gOdjecaUlaz[t][0], gOdjecaUlaz[t][1], gOdjecaUlaz[t][2], svijet, gOdjecaInterijer[t]);
    gOdj[s][odTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gOdjecaUlaz[t][0], gOdjecaUlaz[t][1], gOdjecaUlaz[t][2] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gOdjecaInterijer[t]);
}

static OdjecaAktor(s)
{
    new t = gOdj[s][odTip], svijet = ODJECA_SVIJET + s;
    gOdj[s][odImaAktor] = true;
    gOdj[s][odAktor] = CreateActor(gOdjecaSkin[t], gOdj[s][odAx], gOdj[s][odAy], gOdj[s][odAz], gOdj[s][odAa]);
    SetActorVirtualWorld(gOdj[s][odAktor], svijet);
    gOdj[s][odTekstA] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gOdj[s][odAx], gOdj[s][odAy], gOdj[s][odAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gOdjecaInterijer[t]);
}

static OdjecaMakniAktor(s)
{
    if (!gOdj[s][odImaAktor]) return;
    DestroyActor(gOdj[s][odAktor]);
    DestroyDynamic3DTextLabel(gOdj[s][odTekstA]);
    gOdj[s][odImaAktor] = false;
}

static OdjecaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - ODJECA_SVIJET;
    return s >= 0 && s < ODJECA_MAX && gOdj[s][odIma] && GetPlayerInterior(playerid) == gOdjecaInterijer[gOdj[s][odTip]] ? s : -1;
}

static OdjecaVani(playerid, s)
{
    OdjecaZatvori(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ]);
    SetPlayerFacingAngle(playerid, gOdj[s][odA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

OdjecaUlaz(playerid)
{
    new s = OdjecaIgraca(playerid);
    if (s != -1)
    {
        new t = gOdj[s][odTip];
        if (!IsPlayerInRangeOfPoint(playerid, ODJECA_DOMET, gOdjecaUlaz[t][0], gOdjecaUlaz[t][1], gOdjecaUlaz[t][2])) return 0;
        OdjecaVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < ODJECA_MAX; s++)
    {
        if (!gOdj[s][odIma] || !IsPlayerInRangeOfPoint(playerid, ODJECA_DOMET, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ])) continue;
        new t = gOdj[s][odTip];
        SetPlayerInterior(playerid, gOdjecaInterijer[t]);
        SetPlayerVirtualWorld(playerid, ODJECA_SVIJET + s);
        SetPlayerPos(playerid, gOdjecaUlaz[t][0], gOdjecaUlaz[t][1], gOdjecaUlaz[t][2]);
        SetCameraBehindPlayer(playerid);
        if (!gOdj[s][odImaAktor]) return 1;
        if (t == ODJ_FRIZER) AktorKaze(playerid, gOdj[s][odAktor], "Frizer", "Dobar dan! Sjednite, odmah cu vas.");
        else AktorKaze(playerid, gOdj[s][odAktor], "Prodavacica", "Dobar dan! Slobodno pogledajte, kabina je spremna.");
        return 1;
    }
    return 0;
}

static OdjecaPrivjesak(playerid, i, model, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    new slot = i ? ODJECA_SLOT_FRIZURA : ODJECA_SLOT_KAPA;
    if (model) SetPlayerAttachedObject(playerid, slot, model, 2, x, y, z, rx, ry, rz);
    else RemovePlayerAttachedObject(playerid, slot);
}

OdjecaObuci(playerid)
{
    for (new i = 0; i < 2; i++) OdjecaPrivjesak(playerid, i, gOdjecaNosi[playerid][i][pkModel], gOdjecaNosi[playerid][i][pkX], gOdjecaNosi[playerid][i][pkY], gOdjecaNosi[playerid][i][pkZ], gOdjecaNosi[playerid][i][pkRx], gOdjecaNosi[playerid][i][pkRy], gOdjecaNosi[playerid][i][pkRz]);
}

OdjecaSkinIgraca(playerid)
{
    if (gOrgSkin[playerid] >= 0) return gOrgSkin[playerid];
    return gOdjecaNosiSkin[playerid] >= 0 ? gOdjecaNosiSkin[playerid] : DEFAULT_SKIN;
}

static OdjecaSpremi(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO player_style (account, skin, kapa, kx, ky, kz, krx, kry, krz, frizura, fx, fy, fz, frx, fry, frz) VALUES (%d, %d, %d, %f, %f, %f, %f, %f, %f, %d, %f, %f, %f, %f, %f, %f)",
        gAccountId[playerid], gOdjecaNosiSkin[playerid],
        gOdjecaNosi[playerid][0][pkModel], gOdjecaNosi[playerid][0][pkX], gOdjecaNosi[playerid][0][pkY], gOdjecaNosi[playerid][0][pkZ], gOdjecaNosi[playerid][0][pkRx], gOdjecaNosi[playerid][0][pkRy], gOdjecaNosi[playerid][0][pkRz],
        gOdjecaNosi[playerid][1][pkModel], gOdjecaNosi[playerid][1][pkX], gOdjecaNosi[playerid][1][pkY], gOdjecaNosi[playerid][1][pkZ], gOdjecaNosi[playerid][1][pkRx], gOdjecaNosi[playerid][1][pkRy], gOdjecaNosi[playerid][1][pkRz]));
}

OdjecaPrijava(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT skin, kapa, kx, ky, kz, krx, kry, krz, frizura, fx, fy, fz, frx, fry, frz FROM player_style WHERE account = %d", gAccountId[playerid]);
    gOdjecaNosiSkin[playerid] = -1;
    for (new i = 0; i < 2; i++) gOdjecaNosi[playerid][i][pkModel] = 0;
    if (DB_GetRowCount(res))
    {
        gOdjecaNosiSkin[playerid] = DB_GetFieldInt(res, 0);
        for (new i = 0; i < 2; i++)
        {
            new f = 1 + i * 7;
            gOdjecaNosi[playerid][i][pkModel] = DB_GetFieldInt(res, f);
            gOdjecaNosi[playerid][i][pkX] = DB_GetFieldFloat(res, f + 1);
            gOdjecaNosi[playerid][i][pkY] = DB_GetFieldFloat(res, f + 2);
            gOdjecaNosi[playerid][i][pkZ] = DB_GetFieldFloat(res, f + 3);
            gOdjecaNosi[playerid][i][pkRx] = DB_GetFieldFloat(res, f + 4);
            gOdjecaNosi[playerid][i][pkRy] = DB_GetFieldFloat(res, f + 5);
            gOdjecaNosi[playerid][i][pkRz] = DB_GetFieldFloat(res, f + 6);
        }
    }
    DB_FreeResultSet(res);
}

static PlayerText:OdjecaGumb(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, bool:gumb = true)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, 0.28, 1.3);
    PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 14.0, sirina);
    PlayerTextDrawSetSelectable(playerid, td, gumb);
    gOdjecaTd[playerid][gOdjecaBroj[playerid]++] = td;
    return td;
}

static OdjecaKamera(playerid)
{
    new s = gOdjecaU[playerid] - 1, Float:a = gOdj[s][odKa], Float:x = gOdj[s][odKx], Float:y = gOdj[s][odKy], Float:z = gOdj[s][odKz];
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees), Float:rx = floatcos(a, degrees), Float:ry = floatsin(a, degrees);
    new bool:glava = gOdjecaVrsta[playerid] != ODJ_SKIN, Float:udaljenost = glava ? 1.3 : 3.0, Float:pomak = glava ? 0.35 : 0.8, Float:visina = glava ? 0.7 : 0.2;
    SetPlayerCameraPos(playerid, x + fx * udaljenost - rx * pomak, y + fy * udaljenost - ry * pomak, z + visina + 0.1);
    SetPlayerCameraLookAt(playerid, x - rx * pomak, y - ry * pomak, z + visina);
}

static OdjecaPrikazi(playerid)
{
    new vrsta = gOdjecaVrsta[playerid], broj = OdjecaBrojVrste(gOdjecaU[playerid] - 1, vrsta), info[96];
    if (gOdjecaIdx[playerid] >= broj) gOdjecaIdx[playerid] = 0;
    new a = broj ? OdjecaArtikl(gOdjecaU[playerid] - 1, vrsta, gOdjecaIdx[playerid]) : -1;
    SetPlayerSkin(playerid, vrsta == ODJ_SKIN && a != -1 ? gOdjArtikl[gOdjecaU[playerid] - 1][a][1] : gOdjecaStariSkin[playerid]);
    OdjecaObuci(playerid);
    if (vrsta != ODJ_SKIN) OdjecaPrivjesak(playerid, vrsta - 1, a == -1 ? 0 : gOdjArtikl[gOdjecaU[playerid] - 1][a][1], 0.1, 0.0, 0.0, 0.0, 0.0, 0.0);
    if (!broj) format(info, sizeof info, T(playerid, "Nema artikala"));
    else if (a == -1) format(info, sizeof info, "%s~n~~g~$0~n~~w~%d/%d", vrsta == ODJ_KAPA ? T(playerid, "Bez kape") : T(playerid, "Bez frizure"), gOdjecaIdx[playerid] + 1, broj);
    else format(info, sizeof info, "%s %d~n~~g~$%d~n~~w~%d/%d", T(playerid, gOdjecaVrste[vrsta]), gOdjArtikl[gOdjecaU[playerid] - 1][a][1], gOdjArtikl[gOdjecaU[playerid] - 1][a][2], gOdjecaIdx[playerid] + 1, broj);
    PlayerTextDrawSetString(playerid, gOdjecaGumb[playerid][OTD_INFO], info);
    OdjecaKamera(playerid);
}

static OdjecaOtvori(playerid, s)
{
    new t = gOdj[s][odTip];
    gOdjecaU[playerid] = s + 1;
    gOdjecaVrsta[playerid] = t == ODJ_FRIZER ? ODJ_FRIZURA : ODJ_SKIN;
    gOdjecaIdx[playerid] = 0;
    gOdjecaStariSkin[playerid] = GetPlayerSkin(playerid);
    gOdjecaBroj[playerid] = 0;
    SetPlayerPos(playerid, gOdj[s][odKx], gOdj[s][odKy], gOdj[s][odKz]);
    SetPlayerFacingAngle(playerid, gOdj[s][odKa]);
    TogglePlayerControllable(playerid, false);
    new PlayerText:td = CreatePlayerTextDraw(playerid, 465.0, 118.0, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, 170.0, 225.0);
    PlayerTextDrawColour(playerid, td, 0x101418DD);
    gOdjecaTd[playerid][gOdjecaBroj[playerid]++] = td;
    OdjecaGumb(playerid, 550.0, 124.0, t == ODJ_FRIZER ? T(playerid, "FRIZER") : T(playerid, "ODJECA"), 0x00000000, 160.0, false);
    gOdjecaGumb[playerid][OTD_ODJECA] = PlayerText:INVALID_TEXT_DRAW;
    gOdjecaGumb[playerid][OTD_KAPE] = PlayerText:INVALID_TEXT_DRAW;
    if (t == ODJ_PRODAVAONICA)
    {
        gOdjecaGumb[playerid][OTD_ODJECA] = OdjecaGumb(playerid, 510.0, 152.0, T(playerid, "Odjeca"), 0x2E4A68CC, 72.0);
        gOdjecaGumb[playerid][OTD_KAPE] = OdjecaGumb(playerid, 590.0, 152.0, T(playerid, "Kape"), 0x2E4A68CC, 72.0);
    }
    gOdjecaGumb[playerid][OTD_INFO] = OdjecaGumb(playerid, 550.0, 190.0, "_", 0x00000088, 150.0, false);
    gOdjecaGumb[playerid][OTD_LIJEVO] = OdjecaGumb(playerid, 490.0, 255.0, "<", 0x333333CC, 30.0);
    gOdjecaGumb[playerid][OTD_DESNO] = OdjecaGumb(playerid, 610.0, 255.0, ">", 0x333333CC, 30.0);
    gOdjecaGumb[playerid][OTD_KUPI] = OdjecaGumb(playerid, 550.0, 290.0, T(playerid, "KUPI"), 0x2E7D32CC, 130.0);
    gOdjecaGumb[playerid][OTD_IZLAZ] = OdjecaGumb(playerid, 550.0, 318.0, T(playerid, "IZLAZ"), 0x8B1A1ACC, 130.0);
    for (new i = 0; i < gOdjecaBroj[playerid]; i++) PlayerTextDrawShow(playerid, gOdjecaTd[playerid][i]);
    OdjecaPrikazi(playerid);
    SelectTextDraw(playerid, 0xFFCC00FF);
}

OdjecaZatvori(playerid)
{
    if (!gOdjecaU[playerid]) return;
    gOdjecaU[playerid] = 0;
    for (new i = 0; i < gOdjecaBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gOdjecaTd[playerid][i]);
    gOdjecaBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
    SetPlayerSkin(playerid, gOdjecaStariSkin[playerid]);
    OdjecaObuci(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

static OdjecaKupi(playerid)
{
    new vrsta = gOdjecaVrsta[playerid], broj = OdjecaBrojVrste(gOdjecaU[playerid] - 1, vrsta);
    if (!broj) return;
    new a = OdjecaArtikl(gOdjecaU[playerid] - 1, vrsta, gOdjecaIdx[playerid]), cijena = a == -1 ? 0 : gOdjArtikl[gOdjecaU[playerid] - 1][a][2];
    if (!DajNovac(playerid, -cijena))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        return;
    }
    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    if (vrsta == ODJ_SKIN)
    {
        gOdjecaNosiSkin[playerid] = gOdjArtikl[gOdjecaU[playerid] - 1][a][1];
        if (gOrgSkin[playerid] < 0) gOdjecaStariSkin[playerid] = gOdjArtikl[gOdjecaU[playerid] - 1][a][1];
        OdjecaSpremi(playerid);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste novu odjecu ($%d)."), cijena);
        return;
    }
    new i = vrsta - 1;
    gOdjecaNosi[playerid][i][pkModel] = a == -1 ? 0 : gOdjArtikl[gOdjecaU[playerid] - 1][a][1];
    gOdjecaNosi[playerid][i][pkX] = 0.1;
    gOdjecaNosi[playerid][i][pkY] = 0.0;
    gOdjecaNosi[playerid][i][pkZ] = 0.0;
    gOdjecaNosi[playerid][i][pkRx] = 0.0;
    gOdjecaNosi[playerid][i][pkRy] = 0.0;
    gOdjecaNosi[playerid][i][pkRz] = 0.0;
    OdjecaSpremi(playerid);
    OdjecaZatvori(playerid);
    if (a == -1) return;
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d). Namjestite ga i spremite (ikona diskete)."), T(playerid, gOdjecaVrste[vrsta]), cijena);
    EditAttachedObject(playerid, i ? ODJECA_SLOT_FRIZURA : ODJECA_SLOT_KAPA);
}

OdjecaKlik(playerid, PlayerText:td)
{
    if (!gOdjecaU[playerid]) return 0;
    new broj = OdjecaBrojVrste(gOdjecaU[playerid] - 1, gOdjecaVrsta[playerid]);
    if (td == gOdjecaGumb[playerid][OTD_IZLAZ]) OdjecaZatvori(playerid);
    else if (td == gOdjecaGumb[playerid][OTD_KUPI]) OdjecaKupi(playerid);
    else if (td == gOdjecaGumb[playerid][OTD_LIJEVO] && broj)
    {
        gOdjecaIdx[playerid] = (gOdjecaIdx[playerid] + broj - 1) % broj;
        OdjecaPrikazi(playerid);
    }
    else if (td == gOdjecaGumb[playerid][OTD_DESNO] && broj)
    {
        gOdjecaIdx[playerid] = (gOdjecaIdx[playerid] + 1) % broj;
        OdjecaPrikazi(playerid);
    }
    else if (td == gOdjecaGumb[playerid][OTD_ODJECA] || td == gOdjecaGumb[playerid][OTD_KAPE])
    {
        gOdjecaVrsta[playerid] = td == gOdjecaGumb[playerid][OTD_KAPE] ? ODJ_KAPA : ODJ_SKIN;
        gOdjecaIdx[playerid] = 0;
        OdjecaPrikazi(playerid);
    }
    return 1;
}

OdjecaKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid) || gOdjecaU[playerid]) return 0;
    new s = OdjecaIgraca(playerid);
    if (s == -1 || !gOdj[s][odImaAktor] || !IsPlayerInRangeOfPoint(playerid, ODJECA_KASA_DOMET, gOdj[s][odAx], gOdj[s][odAy], gOdj[s][odAz])) return 0;
    if (!gOdj[s][odImaKabina])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kabina jos nije postavljena."));
        return 1;
    }
    if (gOdj[s][odTip] == ODJ_FRIZER) AktorKaze(playerid, gOdj[s][odAktor], "Frizer", "Kako cemo danas?");
    else AktorKaze(playerid, gOdj[s][odAktor], "Prodavacica", "Izvolite u kabinu, probajte sto vam se svidja.");
    OdjecaOtvori(playerid, s);
    return 1;
}

public OnPlayerEditAttachedObject(playerid, EDIT_RESPONSE:response, index, modelid, boneid, Float:fOffsetX, Float:fOffsetY, Float:fOffsetZ, Float:fRotX, Float:fRotY, Float:fRotZ, Float:fScaleX, Float:fScaleY, Float:fScaleZ)
{
    if (index != ODJECA_SLOT_KAPA && index != ODJECA_SLOT_FRIZURA) return 1;
    new i = index == ODJECA_SLOT_FRIZURA ? 1 : 0;
    if (response == EDIT_RESPONSE_FINAL && modelid == gOdjecaNosi[playerid][i][pkModel])
    {
        gOdjecaNosi[playerid][i][pkX] = fOffsetX;
        gOdjecaNosi[playerid][i][pkY] = fOffsetY;
        gOdjecaNosi[playerid][i][pkZ] = fOffsetZ;
        gOdjecaNosi[playerid][i][pkRx] = fRotX;
        gOdjecaNosi[playerid][i][pkRy] = fRotY;
        gOdjecaNosi[playerid][i][pkRz] = fRotZ;
        OdjecaSpremi(playerid);
    }
    OdjecaObuci(playerid);
    return 1;
}

OdjecaSpawn(playerid)
{
    if (gOdjecaU[playerid])
    {
        gOdjecaStariSkin[playerid] = OdjecaSkinIgraca(playerid);
        OdjecaZatvori(playerid);
    }
    OdjecaObuci(playerid);
}

OdjecaDisconnect(playerid)
{
    gOdjecaU[playerid] = 0;
    gOdjecaBroj[playerid] = 0;
    gOdjecaNosiSkin[playerid] = -1;
    for (new i = 0; i < 2; i++) gOdjecaNosi[playerid][i][pkModel] = 0;
}

ShowOdjece(playerid)
{
    new list[1536], row[112], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova prodavaonica odjece na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novi frizerski salon na mojoj poziciji"));
    for (new s = 0; s < ODJECA_MAX; s++)
    {
        if (!gOdj[s][odIma]) continue;
        format(row, sizeof row, "[%d] %s - %s%s%s", gOdj[s][odDb], CestaZona(gOdj[s][odX], gOdj[s][odY]), gOdj[s][odTip] == ODJ_FRIZER ? T(playerid, "Frizer") : T(playerid, "Odjeca"),
            gOdj[s][odImaAktor] ? ("") : T(playerid, " - bez blagajnika"), gOdj[s][odImaKabina] ? ("") : T(playerid, " - bez kabine"));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_ODJECA, DIALOG_STYLE_LIST, T(playerid, "Odjeca i frizeri"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static OdjecaNova(playerid, t)
{
    new s = 0;
    while (s < ODJECA_MAX && gOdj[s][odIma]) s++;
    if (s == ODJECA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trgovina se postavlja samo izvana."));
    GetPlayerPos(playerid, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ]);
    GetPlayerFacingAngle(playerid, gOdj[s][odA]);
    gOdj[s][odTip] = t;
    gOdj[s][odImaKabina] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO clothing_shops (type, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", t, gOdj[s][odX], gOdj[s][odY], gOdj[s][odZ], gOdj[s][odA]));
    gOdj[s][odDb] = ZadnjiId();
    OdjecaStvori(s);
    OdjecaUcitajArtikle(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Dodano: %s. Udite i postavite blagajnika i kabinu."), CestaZona(gOdj[s][odX], gOdj[s][odY]));
}

static OdjecaPostavi(playerid, s, bool:kabina)
{
    if (OdjecaIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutra."));
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (kabina)
    {
        gOdj[s][odImaKabina] = true;
        gOdj[s][odKx] = x;
        gOdj[s][odKy] = y;
        gOdj[s][odKz] = z;
        gOdj[s][odKa] = a;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE clothing_shops SET Kabina = 1, kx = %f, ky = %f, kz = %f, ka = %f WHERE id = %d", x, y, z, a, gOdj[s][odDb]));
        return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kabina je postavljena."));
    }
    OdjecaMakniAktor(s);
    gOdj[s][odAx] = x;
    gOdj[s][odAy] = y;
    gOdj[s][odAz] = z;
    gOdj[s][odAa] = a;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE clothing_shops SET Aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", x, y, z, a, gOdj[s][odDb]));
    OdjecaAktor(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik je postavljen."));
}

static OdjecaObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && OdjecaIgraca(i) == s) OdjecaVani(i, s);
    OdjecaMakniAktor(s);
    DestroyDynamicPickup(gOdj[s][odPickup]);
    DestroyDynamicPickup(gOdj[s][odPickupU]);
    DestroyDynamic3DTextLabel(gOdj[s][odTekst]);
    DestroyDynamic3DTextLabel(gOdj[s][odTekstU]);
    gOdj[s][odIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM clothing_shops WHERE id = %d", gOdj[s][odDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM store_items WHERE store_type = %d AND store = %d", ODJECA_KATALOG, gOdj[s][odDb]));
}

static ShowOdjecaKatalog(playerid)
{
    new s = gApOdjeca[playerid], list[2048], row[64], len = 0;
    if (!gOdj[s][odIma]) return ShowOdjece(playerid);
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Dodaj artikl"));
    for (new a = 0; a < gOdjArtikala[s]; a++)
    {
        format(row, sizeof row, "[%s] %d - $%d", T(playerid, gOdjecaVrste[gOdjArtikl[s][a][0]]), gOdjArtikl[s][a][1], gOdjArtikl[s][a][2]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_ODJECA_KATALOG, DIALOG_STYLE_LIST, T(playerid, "Katalog"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_odjeca(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0 || listitem == 1)
    {
        OdjecaNova(playerid, listitem == 0 ? ODJ_PRODAVAONICA : ODJ_FRIZER);
        return ShowOdjece(playerid);
    }
    new n = listitem - 1;
    for (new s = 0; s < ODJECA_MAX; s++)
    {
        if (!gOdj[s][odIma] || --n) continue;
        gApOdjeca[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_ODJECA_MENI, DIALOG_STYLE_LIST, T(playerid, "Odjeca i frizeri"), T(playerid, "Postavi blagajnika na mojoj poziciji\nPostavi kabinu na mojoj poziciji\nKatalog\nObrisi"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowOdjece(playerid);
}

DLG:apanel_odjeca_meni(playerid, response, listitem)
{
    new s = gApOdjeca[playerid];
    if (!response || !gOdj[s][odIma]) return ShowOdjece(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 2) return ShowOdjecaKatalog(playerid);
    if (listitem == 0 || listitem == 1) OdjecaPostavi(playerid, s, listitem == 1);
    else
    {
        OdjecaObrisi(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    }
    return ShowOdjece(playerid);
}

DLG:apanel_odjeca_katalog(playerid, response, listitem)
{
    new s = gApOdjeca[playerid];
    if (!response || !gOdj[s][odIma]) return ShowOdjece(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0) return PrikaziDialog(playerid, DLG_APANEL_ODJECA_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Katalog"), T(playerid, "Unesite: [vrsta: 0 = skin, 1 = kapa/naocale, 2 = frizura] [model] [cijena]"), T(playerid, "Dodaj"), T(playerid, "Natrag"));
    new a = listitem - 1;
    if (a >= 0 && a < gOdjArtikala[s])
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM store_items WHERE id = %d", gOdjArtiklDb[s][a]));
        OdjecaUcitajArtikle(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    }
    return ShowOdjecaKatalog(playerid);
}

DLG:apanel_odjeca_unos(playerid, response, const inputtext[])
{
    new s = gApOdjeca[playerid];
    if (!response || !gOdj[s][odIma]) return ShowOdjecaKatalog(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new idx = 0, p1[12], p2[12], p3[12];
    if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !SljedeciParam(inputtext, idx, p3) || !ProvjeriBroj(p1) || !ProvjeriBroj(p2) || !ProvjeriBroj(p3))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [vrsta: 0 = skin, 1 = kapa/naocale, 2 = frizura] [model] [cijena]"));
        return ShowOdjecaKatalog(playerid);
    }
    new vrsta = strval(p1), model = strval(p2), cijena = strval(p3);
    if (vrsta > ODJ_FRIZURA || !OdjecaVrstaUTrgovini(s, vrsta) || (vrsta == ODJ_SKIN ? (model > 311 || model == 74) : model < 300) || cijena > 1000000 || gOdjArtikala[s] >= ODJECA_ARTIKALA)
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan artikl."));
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO store_items (store_type, store, kind, model, price) VALUES (%d, %d, %d, %d, %d)", ODJECA_KATALOG, gOdj[s][odDb], vrsta, model, cijena));
        OdjecaUcitajArtikle(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Artikl dodan."));
    }
    return ShowOdjecaKatalog(playerid);
}
