#define NAMJESTAJ_MAX           8
#define NAMJESTAJ_SVIJET        9500
#define NAMJESTAJ_DOMET         2.0
#define NAMJESTAJ_KASA_DOMET    2.5
#define NAMJESTAJ_SKIN          217
#define NAMJESTAJ_TD            5
#define NAMJESTAJ_ROBA          48
#define NAMJESTAJ_OBJEKATA      4000
#define NAMJESTAJ_PO_KUCI       60
#define NAMJESTAJ_SKLADISTE     100
#define NAMJESTAJ_KAMERA        3.5
#define NAMJESTAJ_ISPRED        1.5

enum E_NAMJESTAJ { bool:nmIma, nmDb, nmInt, Float:nmX, Float:nmY, Float:nmZ, Float:nmA, bool:nmImaAktor, Float:nmAx, Float:nmAy, Float:nmAz, Float:nmAa, nmAktor,
    bool:nmImaIzlog, Float:nmIx, Float:nmIy, Float:nmIz, Float:nmIa,
    STREAMER_TAG_PICKUP:nmPickup, STREAMER_TAG_PICKUP:nmPickupU, STREAMER_TAG_3D_TEXT_LABEL:nmTekst, STREAMER_TAG_3D_TEXT_LABEL:nmTekstU, STREAMER_TAG_3D_TEXT_LABEL:nmTekstA }

enum E_NAM_INT { niIme[16], niInt, Float:niX, Float:niY, Float:niZ }
static const gNamjestajInt[][E_NAM_INT] = {
    { "Binco", 15, 207.737, -109.019, 1005.133 },
    { "Didier Sachs", 14, 204.328, -168.695, 1000.523 },
    { "Zip", 18, 161.391, -93.159, 1001.804 },
    { "Victim", 5, 225.028, -8.182, 1002.218 },
    { "Sub Urban", 1, 203.778, -48.492, 1001.804 }
};

enum E_NAM_ZADANO { nzModel, nzCijena, nzIme[24] }
static const gNamjestajZadano[][E_NAM_ZADANO] = {
    { 1700, 1500, "Bracni krevet" }, { 1701, 1500, "Bracni krevet 2" }, { 1745, 1200, "Krevet" }, { 1771, 900, "Krevet na kat" },
    { 1702, 1200, "Kauc" }, { 1703, 1200, "Kauc 2" }, { 1753, 2000, "Kozni kauc" }, { 1761, 1800, "Kauc 3" },
    { 1704, 600, "Fotelja" }, { 1708, 600, "Fotelja 2" }, { 1754, 800, "Kozna fotelja" }, { 1714, 400, "Uredska stolica" },
    { 1720, 200, "Stolica" }, { 2115, 700, "Kuhinjski stol" }, { 1433, 300, "Stolic" }, { 1742, 900, "Polica za knjige" },
    { 1741, 500, "Ormaric" }, { 1748, 800, "Televizor" }, { 1752, 1500, "Veliki televizor" }, { 2131, 1000, "Hladnjak" },
    { 948, 150, "Biljka" }, { 2069, 250, "Lampa" }
};

#assert sizeof gNamjestajZadano <= NAMJESTAJ_ROBA

static gNam[NAMJESTAJ_MAX][E_NAMJESTAJ];
static gApNamjestaj[MAX_PLAYERS];
static gNamRobaModel[NAMJESTAJ_MAX][NAMJESTAJ_ROBA], gNamRobaCijena[NAMJESTAJ_MAX][NAMJESTAJ_ROBA], gNamRobaIme[NAMJESTAJ_MAX][NAMJESTAJ_ROBA][24], gNamRobaDb[NAMJESTAJ_MAX][NAMJESTAJ_ROBA], gNamRobaBroj[NAMJESTAJ_MAX];
enum { NMTD_LIJEVO, NMTD_DESNO, NMTD_INFO, NMTD_KUPI, NMTD_IZADI }
static PlayerText:gNamTd[MAX_PLAYERS][NAMJESTAJ_TD], bool:gNamKatalog[MAX_PLAYERS], gNamU[MAX_PLAYERS] = { -1, ... }, gNamIdx[MAX_PLAYERS];
static gNamPregled[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... }, gNamKupac[NAMJESTAJ_MAX] = { INVALID_PLAYER_ID, ... };
static gNamObjDb[NAMJESTAJ_OBJEKATA], STREAMER_TAG_OBJECT:gNamObj[NAMJESTAJ_OBJEKATA];
static gNamPopis[MAX_PLAYERS][NAMJESTAJ_SKLADISTE], gNamPopisBroj[MAX_PLAYERS], gNamOdabran[MAX_PLAYERS], gNamKuca[MAX_PLAYERS], gNamUredi[MAX_PLAYERS] = { -1, ... };

InitNamjestaj()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS furniture_shops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL DEFAULT 0, aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0, izlog INTEGER NOT NULL DEFAULT 0, ix REAL NOT NULL DEFAULT 0, iy REAL NOT NULL DEFAULT 0, iz REAL NOT NULL DEFAULT 0, ia REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS furniture_catalog (id INTEGER PRIMARY KEY AUTOINCREMENT, shop INTEGER NOT NULL, model INTEGER NOT NULL, price INTEGER NOT NULL, name TEXT NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS furniture (id INTEGER PRIMARY KEY AUTOINCREMENT, owner INTEGER NOT NULL, model INTEGER NOT NULL, name TEXT NOT NULL, house INTEGER NOT NULL DEFAULT 0, x REAL NOT NULL DEFAULT 0, y REAL NOT NULL DEFAULT 0, z REAL NOT NULL DEFAULT 0, rx REAL NOT NULL DEFAULT 0, ry REAL NOT NULL DEFAULT 0, rz REAL NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, aktor, ax, ay, az, aa, izlog, ix, iy, iz, ia FROM furniture_shops ORDER BY id LIMIT %d", NAMJESTAJ_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gNam[s][nmDb] = DB_GetFieldInt(res, 0);
        gNam[s][nmX] = DB_GetFieldFloat(res, 1);
        gNam[s][nmY] = DB_GetFieldFloat(res, 2);
        gNam[s][nmZ] = DB_GetFieldFloat(res, 3);
        gNam[s][nmA] = DB_GetFieldFloat(res, 4);
        gNam[s][nmInt] = clamp(DB_GetFieldInt(res, 5), 0, sizeof gNamjestajInt - 1);
        NamjestajStvori(s);
        NamjestajUcitajRobu(s);
        if (DB_GetFieldInt(res, 6))
        {
            gNam[s][nmAx] = DB_GetFieldFloat(res, 7);
            gNam[s][nmAy] = DB_GetFieldFloat(res, 8);
            gNam[s][nmAz] = DB_GetFieldFloat(res, 9);
            gNam[s][nmAa] = DB_GetFieldFloat(res, 10);
            NamjestajAktor(s);
        }
        gNam[s][nmImaIzlog] = DB_GetFieldInt(res, 11) != 0;
        gNam[s][nmIx] = DB_GetFieldFloat(res, 12);
        gNam[s][nmIy] = DB_GetFieldFloat(res, 13);
        gNam[s][nmIz] = DB_GetFieldFloat(res, 14);
        gNam[s][nmIa] = DB_GetFieldFloat(res, 15);
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < NAMJESTAJ_OBJEKATA; i++) gNamObjDb[i] = 0;
    res = DB_ExecuteQuery(gDB, "SELECT id, model, house, x, y, z, rx, ry, rz FROM furniture WHERE house > 0");
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new kuca = DB_GetFieldInt(res, 2), interijer = KucaInterijerDb(kuca);
        if (interijer == -1)
        {
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET house = 0 WHERE id = %d", DB_GetFieldInt(res, 0)));
            continue;
        }
        NamjestajPostaviObjekt(DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 1), kuca, interijer, DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldFloat(res, 5), DB_GetFieldFloat(res, 6), DB_GetFieldFloat(res, 7), DB_GetFieldFloat(res, 8));
    }
    DB_FreeResultSet(res);
}

static NamjestajPostaviObjekt(db, model, kuca, interijer, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    for (new i = 0; i < NAMJESTAJ_OBJEKATA; i++)
    {
        if (gNamObjDb[i]) continue;
        gNamObjDb[i] = db;
        gNamObj[i] = CreateDynamicObject(model, x, y, z, rx, ry, rz, KUCA_SVIJET + kuca, interijer);
        return i;
    }
    return -1;
}

static NamjestajSlot(db)
{
    for (new i = 0; i < NAMJESTAJ_OBJEKATA; i++) if (gNamObjDb[i] == db) return i;
    return -1;
}

static NamjestajMakniObjekt(db)
{
    new i = NamjestajSlot(db);
    if (i == -1) return;
    DestroyDynamicObject(gNamObj[i]);
    gNamObjDb[i] = 0;
}

NamjestajKucaIsprazni(kuca)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM furniture WHERE house = %d", kuca);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res)) NamjestajMakniObjekt(DB_GetFieldInt(res, 0));
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET house = 0 WHERE house = %d", kuca));
}

static NamjestajUcitajRobu(s, bool:zadano = true)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, model, price, name FROM furniture_catalog WHERE shop = %d ORDER BY id LIMIT %d", gNam[s][nmDb], NAMJESTAJ_ROBA);
    gNamRobaBroj[s] = 0;
    for (new n = DB_GetRowCount(res); gNamRobaBroj[s] < n; DB_SelectNextRow(res))
    {
        new r = gNamRobaBroj[s]++;
        gNamRobaDb[s][r] = DB_GetFieldInt(res, 0);
        gNamRobaModel[s][r] = DB_GetFieldInt(res, 1);
        gNamRobaCijena[s][r] = DB_GetFieldInt(res, 2);
        DB_GetFieldString(res, 3, gNamRobaIme[s][r], 24);
    }
    DB_FreeResultSet(res);
    if (gNamRobaBroj[s] || !zadano) return;
    for (new i = 0; i < sizeof gNamjestajZadano; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture_catalog (shop, model, price, name) VALUES (%d, %d, %d, '%q')", gNam[s][nmDb], gNamjestajZadano[i][nzModel], gNamjestajZadano[i][nzCijena], gNamjestajZadano[i][nzIme]));
    NamjestajUcitajRobu(s, false);
}

static NamjestajStvori(s)
{
    new svijet = NAMJESTAJ_SVIJET + s, i = gNam[s][nmInt];
    gNam[s][nmIma] = true;
    gNam[s][nmPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ], 0, 0);
    gNam[s][nmTekst] = CreateDynamic3DTextLabel("Namjestaj\n[Y]", COLOR_ORG, gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    gNam[s][nmPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gNamjestajInt[i][niX], gNamjestajInt[i][niY], gNamjestajInt[i][niZ], svijet, gNamjestajInt[i][niInt]);
    gNam[s][nmTekstU] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gNamjestajInt[i][niX], gNamjestajInt[i][niY], gNamjestajInt[i][niZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gNamjestajInt[i][niInt]);
}

static NamjestajAktor(s)
{
    new svijet = NAMJESTAJ_SVIJET + s;
    gNam[s][nmImaAktor] = true;
    gNam[s][nmAktor] = CreateActor(NAMJESTAJ_SKIN, gNam[s][nmAx], gNam[s][nmAy], gNam[s][nmAz], gNam[s][nmAa]);
    SetActorVirtualWorld(gNam[s][nmAktor], svijet);
    gNam[s][nmTekstA] = CreateDynamic3DTextLabel("[Y]", COLOR_ORG, gNam[s][nmAx], gNam[s][nmAy], gNam[s][nmAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, gNamjestajInt[gNam[s][nmInt]][niInt]);
}

static NamjestajMakniAktor(s)
{
    if (!gNam[s][nmImaAktor]) return;
    DestroyActor(gNam[s][nmAktor]);
    DestroyDynamic3DTextLabel(gNam[s][nmTekstA]);
    gNam[s][nmImaAktor] = false;
}

static NamjestajIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - NAMJESTAJ_SVIJET;
    return s >= 0 && s < NAMJESTAJ_MAX && gNam[s][nmIma] && GetPlayerInterior(playerid) == gNamjestajInt[gNam[s][nmInt]][niInt] ? s : -1;
}

static NamjestajVani(playerid, s)
{
    NamjestajZatvori(playerid);
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ]);
    SetPlayerFacingAngle(playerid, gNam[s][nmA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

NamjestajUlaz(playerid)
{
    new s = NamjestajIgraca(playerid);
    if (s != -1)
    {
        new i = gNam[s][nmInt];
        if (!IsPlayerInRangeOfPoint(playerid, NAMJESTAJ_DOMET, gNamjestajInt[i][niX], gNamjestajInt[i][niY], gNamjestajInt[i][niZ])) return 0;
        NamjestajVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < NAMJESTAJ_MAX; s++)
    {
        if (!gNam[s][nmIma] || !IsPlayerInRangeOfPoint(playerid, NAMJESTAJ_DOMET, gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ])) continue;
        new i = gNam[s][nmInt];
        SetPlayerInterior(playerid, gNamjestajInt[i][niInt]);
        SetPlayerVirtualWorld(playerid, NAMJESTAJ_SVIJET + s);
        SetPlayerPos(playerid, gNamjestajInt[i][niX], gNamjestajInt[i][niY], gNamjestajInt[i][niZ]);
        SetCameraBehindPlayer(playerid);
        if (gNam[s][nmImaAktor]) AktorKaze(playerid, gNam[s][nmAktor], "Prodavac", "Dobrodosli. Namjestaj mozete pogledati na blagajni.");
        return 1;
    }
    return 0;
}

static NamjestajPrikazi(playerid)
{
    new s = gNamU[playerid], r = gNamIdx[playerid], info[96];
    format(info, sizeof info, "%s~n~~g~$%d~n~~w~%d/%d", gNamRobaIme[s][r], gNamRobaCijena[s][r], r + 1, gNamRobaBroj[s]);
    PlayerTextDrawSetString(playerid, gNamTd[playerid][NMTD_INFO], info);
    if (gNamPregled[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gNamPregled[playerid]);
    gNamPregled[playerid] = CreatePlayerObject(playerid, gNamRobaModel[s][r], gNam[s][nmIx], gNam[s][nmIy], gNam[s][nmIz] - 1.0, 0.0, 0.0, gNam[s][nmIa]);
}

static NamjestajOtvori(playerid, s)
{
    gNamKatalog[playerid] = true;
    gNamU[playerid] = s;
    gNamIdx[playerid] = 0;
    gNamKupac[s] = playerid;
    new Float:a = gNam[s][nmIa];
    TogglePlayerControllable(playerid, false);
    SetPlayerCameraPos(playerid, gNam[s][nmIx] - NAMJESTAJ_KAMERA * floatsin(a, degrees), gNam[s][nmIy] + NAMJESTAJ_KAMERA * floatcos(a, degrees), gNam[s][nmIz] + 1.0);
    SetPlayerCameraLookAt(playerid, gNam[s][nmIx], gNam[s][nmIy], gNam[s][nmIz] - 0.5);
    gNamTd[playerid][NMTD_LIJEVO] = SalonGumb(playerid, 230.0, 340.0, "<", 0x333333CC, 30.0);
    gNamTd[playerid][NMTD_DESNO] = SalonGumb(playerid, 410.0, 340.0, ">", 0x333333CC, 30.0);
    gNamTd[playerid][NMTD_INFO] = SalonGumb(playerid, 320.0, 330.0, "_", 0x00000088, 130.0, false);
    gNamTd[playerid][NMTD_KUPI] = SalonGumb(playerid, 280.0, 412.0, T(playerid, "KUPI"), 0x2E7D32CC, 70.0);
    gNamTd[playerid][NMTD_IZADI] = SalonGumb(playerid, 360.0, 412.0, T(playerid, "IZADI"), 0x1E5AA8CC, 70.0);
    NamjestajPrikazi(playerid);
    SelectTextDraw(playerid, 0xD4A017FF);
}

NamjestajZatvori(playerid)
{
    if (!gNamKatalog[playerid]) return;
    gNamKatalog[playerid] = false;
    new s = gNamU[playerid];
    if (s >= 0 && gNamKupac[s] == playerid) gNamKupac[s] = INVALID_PLAYER_ID;
    gNamU[playerid] = -1;
    for (new i = 0; i < NAMJESTAJ_TD; i++) PlayerTextDrawDestroy(playerid, gNamTd[playerid][i]);
    if (gNamPregled[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gNamPregled[playerid]);
    gNamPregled[playerid] = INVALID_OBJECT_ID;
    CancelSelectTextDraw(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

static NamjestajBroj(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM furniture WHERE owner = %d", gAccountId[playerid]);
    new n = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return n;
}

NamjestajKlik(playerid, PlayerText:td)
{
    if (!gNamKatalog[playerid]) return 0;
    new s = NamjestajIgraca(playerid);
    if (td == gNamTd[playerid][NMTD_IZADI] || s == -1 || s != gNamU[playerid])
    {
        NamjestajZatvori(playerid);
        return 1;
    }
    if (td == gNamTd[playerid][NMTD_LIJEVO] || td == gNamTd[playerid][NMTD_DESNO])
    {
        gNamIdx[playerid] = (gNamIdx[playerid] + (td == gNamTd[playerid][NMTD_DESNO] ? 1 : gNamRobaBroj[s] - 1)) % gNamRobaBroj[s];
        NamjestajPrikazi(playerid);
        return 1;
    }
    if (td != gNamTd[playerid][NMTD_KUPI]) return 1;
    new r = gNamIdx[playerid], cijena = gNamRobaCijena[s][r];
    if (NamjestajBroj(playerid) >= NAMJESTAJ_SKLADISTE) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Imate previse namjestaja (najvise %d)."), NAMJESTAJ_SKLADISTE);
    else if (!DajNovac(playerid, -cijena)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture (owner, model, name) VALUES (%d, %d, '%q')", gAccountId[playerid], gNamRobaModel[s][r], gNamRobaIme[s][r]));
        PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d). Postavite ga u kuci (Y na ulazu, Namjestaj)."), gNamRobaIme[s][r], cijena);
    }
    return 1;
}

NamjestajKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid) || gNamKatalog[playerid]) return 0;
    new s = NamjestajIgraca(playerid);
    if (s == -1 || !gNam[s][nmImaAktor] || !IsPlayerInRangeOfPoint(playerid, NAMJESTAJ_KASA_DOMET, gNam[s][nmAx], gNam[s][nmAy], gNam[s][nmAz])) return 0;
    new k = gNamKupac[s];
    if (k != INVALID_PLAYER_ID && k != playerid && IsPlayerConnected(k) && gNamKatalog[k] && gNamU[k] == s)
    {
        AktorKaze(playerid, gNam[s][nmAktor], "Prodavac", "Trenutno sluzim drugog kupca, pricekajte.");
        return 1;
    }
    if (!gNam[s][nmImaIzlog] || !gNamRobaBroj[s]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trgovina trenutno nema ponudu."));
    AktorKaze(playerid, gNam[s][nmAktor], "Prodavac", "Izvolite, pogledajte ponudu.");
    NamjestajOtvori(playerid, s);
    return 1;
}

NamjestajDisconnect(playerid)
{
    new s = gNamU[playerid];
    if (s >= 0 && gNamKupac[s] == playerid) gNamKupac[s] = INVALID_PLAYER_ID;
    gNamKatalog[playerid] = false;
    gNamU[playerid] = -1;
    gNamPregled[playerid] = INVALID_OBJECT_ID;
    gNamUredi[playerid] = -1;
    gNamKuca[playerid] = 0;
}

NamjestajMeni(playerid, kuca)
{
    new list[2048], row[64], len = 0, DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, name, house FROM furniture WHERE owner = %d AND (house = 0 OR house = %d) ORDER BY house DESC, id LIMIT %d", gAccountId[playerid], kuca, NAMJESTAJ_SKLADISTE);
    gNamKuca[playerid] = kuca;
    gNamPopisBroj[playerid] = 0;
    for (new n = DB_GetRowCount(res); gNamPopisBroj[playerid] < n; DB_SelectNextRow(res))
    {
        new ime[24];
        gNamPopis[playerid][gNamPopisBroj[playerid]++] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        if (DB_GetFieldInt(res, 2)) format(row, sizeof row, "%s", ime);
        else format(row, sizeof row, "%s {AAAAAA}%s", ime, T(playerid, "(nije postavljeno)"));
        DodajStupac(list, len, row);
    }
    DB_FreeResultSet(res);
    if (!gNamPopisBroj[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate namjestaja. Kupite ga u trgovini namjestaja."));
    return PrikaziDialog(playerid, DLG_NAMJESTAJ, DIALOG_STYLE_LIST, T(playerid, "Namjestaj"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static bool:NamjestajUKuci(playerid)
{
    return gNamKuca[playerid] && KucaIgracaDb(playerid) == gNamKuca[playerid];
}

DLG:namjestaj(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= gNamPopisBroj[playerid] || !NamjestajUKuci(playerid)) return 1;
    gNamOdabran[playerid] = gNamPopis[playerid][listitem];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT house FROM furniture WHERE id = %d AND owner = %d", gNamOdabran[playerid], gAccountId[playerid]);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return NamjestajMeni(playerid, gNamKuca[playerid]);
    }
    new bool:postavljeno = DB_GetFieldInt(res, 0) != 0;
    DB_FreeResultSet(res);
    if (postavljeno) return PrikaziDialog(playerid, DLG_NAMJESTAJ_OPCIJE, DIALOG_STYLE_LIST, T(playerid, "Namjestaj"), T(playerid, "Uredi poziciju\nSpremi u skladiste"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    return PrikaziDialog(playerid, DLG_NAMJESTAJ_OPCIJE, DIALOG_STYLE_LIST, T(playerid, "Namjestaj"), T(playerid, "Postavi"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:namjestaj_opcije(playerid, response, listitem)
{
    new kuca = gNamKuca[playerid], db = gNamOdabran[playerid];
    if (!NamjestajUKuci(playerid)) return 1;
    if (!response) return NamjestajMeni(playerid, kuca);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT model, house FROM furniture WHERE id = %d AND owner = %d", db, gAccountId[playerid]);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return NamjestajMeni(playerid, kuca);
    }
    new model = DB_GetFieldInt(res, 0), bool:postavljeno = DB_GetFieldInt(res, 1) != 0;
    DB_FreeResultSet(res);
    if (!postavljeno)
    {
        res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM furniture WHERE house = %d", kuca);
        new broj = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        if (broj >= NAMJESTAJ_PO_KUCI) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U kuci moze biti najvise %d komada namjestaja."), NAMJESTAJ_PO_KUCI);
        new Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        x -= NAMJESTAJ_ISPRED * floatsin(a, degrees);
        y += NAMJESTAJ_ISPRED * floatcos(a, degrees);
        z -= 1.0;
        new i = NamjestajPostaviObjekt(db, model, kuca, GetPlayerInterior(playerid), x, y, z, 0.0, 0.0, a);
        if (i == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nije moguce postaviti vise namjestaja na serveru."));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET house = %d, x = %f, y = %f, z = %f, rx = 0, ry = 0, rz = %f WHERE id = %d", kuca, x, y, z, a, db));
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
        gNamUredi[playerid] = db;
        EditDynamicObject(playerid, gNamObj[i]);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pomaknite namjestaj i kliknite na disketu za spremanje."));
    }
    if (listitem == 0)
    {
        new i = NamjestajSlot(db);
        if (i == -1) return 1;
        gNamUredi[playerid] = db;
        EditDynamicObject(playerid, gNamObj[i]);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pomaknite namjestaj i kliknite na disketu za spremanje."));
    }
    NamjestajMakniObjekt(db);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET house = 0 WHERE id = %d", db));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Namjestaj je spremljen u skladiste."));
    return NamjestajMeni(playerid, kuca);
}

public OnPlayerEditDynamicObject(playerid, STREAMER_TAG_OBJECT:objectid, response, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz)
{
    new db = gNamUredi[playerid];
    if (db == -1) return 1;
    new i = NamjestajSlot(db);
    if (i == -1 || gNamObj[i] != objectid)
    {
        gNamUredi[playerid] = -1;
        return 1;
    }
    if (response == _:EDIT_RESPONSE_UPDATE) return 1;
    gNamUredi[playerid] = -1;
    if (response == _:EDIT_RESPONSE_FINAL && NamjestajUKuci(playerid))
    {
        SetDynamicObjectPos(objectid, x, y, z);
        SetDynamicObjectRot(objectid, rx, ry, rz);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET x = %f, y = %f, z = %f, rx = %f, ry = %f, rz = %f WHERE id = %d", x, y, z, rx, ry, rz, db));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pozicija namjestaja je spremljena."));
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z, rx, ry, rz FROM furniture WHERE id = %d", db);
    if (DB_GetRowCount(res))
    {
        SetDynamicObjectPos(objectid, DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2));
        SetDynamicObjectRot(objectid, DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldFloat(res, 5));
    }
    DB_FreeResultSet(res);
    return 1;
}

ShowNamjestaje(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova trgovina namjestaja na mojoj poziciji"));
    for (new s = 0; s < NAMJESTAJ_MAX; s++)
    {
        if (!gNam[s][nmIma]) continue;
        format(row, sizeof row, "[%d] %s (%s)%s%s", gNam[s][nmDb], CestaZona(gNam[s][nmX], gNam[s][nmY]), gNamjestajInt[gNam[s][nmInt]][niIme], T(playerid, gNam[s][nmImaAktor] ? ("") : (" - bez blagajnika")), T(playerid, gNam[s][nmImaIzlog] ? ("") : (" - bez izloga")));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_NAMJESTAJ, DIALOG_STYLE_LIST, T(playerid, "Trgovine namjestaja"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static NamjestajNova(playerid, i)
{
    new s = 0;
    while (s < NAMJESTAJ_MAX && gNam[s][nmIma]) s++;
    if (s == NAMJESTAJ_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trgovina se postavlja samo izvana."));
    GetPlayerPos(playerid, gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ]);
    GetPlayerFacingAngle(playerid, gNam[s][nmA]);
    gNam[s][nmInt] = i;
    gNam[s][nmImaIzlog] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture_shops (x, y, z, a, interior) VALUES (%f, %f, %f, %f, %d)", gNam[s][nmX], gNam[s][nmY], gNam[s][nmZ], gNam[s][nmA], i));
    gNam[s][nmDb] = ZadnjiId();
    NamjestajStvori(s);
    NamjestajUcitajRobu(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trgovina namjestaja dodana: %s. Udite i postavite blagajnika i izlog."), CestaZona(gNam[s][nmX], gNam[s][nmY]));
}

static NamjestajBlagajnik(playerid, s)
{
    if (NamjestajIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te trgovine."));
    NamjestajMakniAktor(s);
    GetPlayerPos(playerid, gNam[s][nmAx], gNam[s][nmAy], gNam[s][nmAz]);
    GetPlayerFacingAngle(playerid, gNam[s][nmAa]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture_shops SET aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", gNam[s][nmAx], gNam[s][nmAy], gNam[s][nmAz], gNam[s][nmAa], gNam[s][nmDb]));
    NamjestajAktor(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Blagajnik je postavljen."));
}

static NamjestajIzlog(playerid, s)
{
    if (NamjestajIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te trgovine."));
    GetPlayerPos(playerid, gNam[s][nmIx], gNam[s][nmIy], gNam[s][nmIz]);
    GetPlayerFacingAngle(playerid, gNam[s][nmIa]);
    gNam[s][nmImaIzlog] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture_shops SET izlog = 1, ix = %f, iy = %f, iz = %f, ia = %f WHERE id = %d", gNam[s][nmIx], gNam[s][nmIy], gNam[s][nmIz], gNam[s][nmIa], gNam[s][nmDb]));
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Izlog je postavljen. Kamera gleda u smjeru u kojem ste gledali."));
}

static NamjestajObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i] && NamjestajIgraca(i) == s) NamjestajVani(i, s);
    NamjestajMakniAktor(s);
    DestroyDynamicPickup(gNam[s][nmPickup]);
    DestroyDynamicPickup(gNam[s][nmPickupU]);
    DestroyDynamic3DTextLabel(gNam[s][nmTekst]);
    DestroyDynamic3DTextLabel(gNam[s][nmTekstU]);
    gNam[s][nmIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM furniture_shops WHERE id = %d", gNam[s][nmDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM furniture_catalog WHERE shop = %d", gNam[s][nmDb]));
}

DLG:apanel_namjestaj(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new list[256], len = 0;
        for (new i = 0; i < sizeof gNamjestajInt; i++) DodajStupac(list, len, gNamjestajInt[i][niIme]);
        return PrikaziDialog(playerid, DLG_APANEL_NAMJESTAJ_INT, DIALOG_STYLE_LIST, T(playerid, "Odaberite interijer"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new n = listitem;
    for (new s = 0; s < NAMJESTAJ_MAX; s++)
    {
        if (!gNam[s][nmIma] || --n) continue;
        gApNamjestaj[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_NAMJESTAJ_MENI, DIALOG_STYLE_LIST, T(playerid, "Trgovine namjestaja"), T(playerid, "Postavi blagajnika na mojoj poziciji\nPostavi izlog na mojoj poziciji\nKatalog\nObrisi trgovinu"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowNamjestaje(playerid);
}

DLG:apanel_namjestaj_int(playerid, response, listitem)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (response && listitem >= 0 && listitem < sizeof gNamjestajInt) NamjestajNova(playerid, listitem);
    return ShowNamjestaje(playerid);
}

DLG:apanel_namjestaj_meni(playerid, response, listitem)
{
    new s = gApNamjestaj[playerid];
    if (!response || !gNam[s][nmIma]) return ShowNamjestaje(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    switch (listitem)
    {
        case 0: NamjestajBlagajnik(playerid, s);
        case 1: NamjestajIzlog(playerid, s);
        case 2: return ShowNamjestajKatalog(playerid);
        default:
        {
            NamjestajObrisi(s);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trgovina obrisana."));
        }
    }
    return ShowNamjestaje(playerid);
}

static ShowNamjestajKatalog(playerid)
{
    new s = gApNamjestaj[playerid], list[2048], row[80], len = 0;
    if (!gNam[s][nmIma]) return ShowNamjestaje(playerid);
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Dodaj artikl"));
    for (new r = 0; r < gNamRobaBroj[s]; r++)
    {
        format(row, sizeof row, "%s (%d) - $%d", gNamRobaIme[s][r], gNamRobaModel[s][r], gNamRobaCijena[s][r]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_NAMJESTAJ_KATALOG, DIALOG_STYLE_LIST, T(playerid, "Katalog"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_namjestaj_katalog(playerid, response, listitem)
{
    new s = gApNamjestaj[playerid];
    if (!response || !gNam[s][nmIma]) return ShowNamjestaje(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0) return PrikaziDialog(playerid, DLG_APANEL_NAMJESTAJ_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Katalog"), T(playerid, "Unesite: [model] [cijena] [naziv]"), T(playerid, "Dodaj"), T(playerid, "Natrag"));
    new r = listitem - 1;
    if (r >= 0 && r < gNamRobaBroj[s])
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM furniture_catalog WHERE id = %d", gNamRobaDb[s][r]));
        NamjestajUcitajRobu(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    }
    return ShowNamjestajKatalog(playerid);
}

DLG:apanel_namjestaj_unos(playerid, response, const inputtext[])
{
    new s = gApNamjestaj[playerid];
    if (!response || !gNam[s][nmIma]) return ShowNamjestajKatalog(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new idx = 0, p1[12], p2[12];
    if (!SljedeciParam(inputtext, idx, p1) || !SljedeciParam(inputtext, idx, p2) || !ProvjeriBroj(p1) || !ProvjeriBroj(p2))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [model] [cijena] [naziv]"));
        return ShowNamjestajKatalog(playerid);
    }
    while (inputtext[idx] == ' ') idx++;
    new model = strval(p1), cijena = strval(p2), ime[24];
    strcopy(ime, inputtext[idx], sizeof ime);
    if (!ime[0] || model < 300 || model > 20000 || cijena > 1000000 || gNamRobaBroj[s] >= NAMJESTAJ_ROBA)
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravan artikl."));
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture_catalog (shop, model, price, name) VALUES (%d, %d, %d, '%q')", gNam[s][nmDb], model, cijena, ime));
        NamjestajUcitajRobu(s, false);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Artikl dodan."));
    }
    return ShowNamjestajKatalog(playerid);
}
