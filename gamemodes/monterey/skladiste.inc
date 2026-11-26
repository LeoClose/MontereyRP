#define SK_VILJUSKARA       10
#define SK_RAMPI            6
#define SK_MODEL            530
#define SK_SKIN             27
#define SK_DOMET            2.5
#define SK_POVRAT_DOMET     20.0
#define SK_MIRUJE_S         900
#define SK_TRAKA_MS         40
#define SK_ZELENO           18.0
#define SK_ZELENO_TESKO     12.0
#define SK_CILJ_DOMET       4.0
#define SK_PRILAZ           2.6
#define SK_PRIJENOS         80
#define SK_RAMPA_BONUS      25
#define SK_ROK              200
#define SK_ROK_HITNO        100
#define SK_ROK_PRIJENOS     120
#define SK_NIZ_MAX          5
#define SK_LOMLJIVO_STETA   25.0
#define SK_BOJA             0xE67E22FF
#define SK_POLICA_MODEL     3761
#define SK_POLICA_A         15
#define SK_POLICA_B         5
#define SK_POLICA           (SK_POLICA_A + SK_POLICA_B)
#define SK_SLOTOVA          (SK_POLICA * sizeof gSkladisteMjesto)
#define SK_KUT_B            -135.0

enum { SKT_OBICNO, SKT_LOMLJIVO, SKT_TESKO, SKT_HITNO }

static const Float:gSkladisteZadano[3] = { 2770.0, -2456.0, 13.6 };
static const Float:gSkladisteHangar[2][3] = { { 2788.16, -2455.88, 16.73 }, { 2415.46, -2468.58, 16.73 } };
static const Float:gSkladistePolicaA[SK_POLICA_A][3] = {
    { 2783.78, -2501.84, 14.70 }, { 2783.78, -2486.96, 14.66 }, { 2783.78, -2463.82, 14.63 }, { 2783.78, -2448.48, 14.63 }, { 2783.78, -2425.35, 14.63 },
    { 2783.78, -2410.21, 14.67 }, { 2791.95, -2501.84, 14.63 }, { 2797.52, -2486.83, 14.63 }, { 2791.95, -2486.96, 14.63 }, { 2791.95, -2463.82, 14.63 },
    { 2797.52, -2448.34, 14.63 }, { 2791.95, -2448.48, 14.63 }, { 2791.95, -2425.35, 14.67 }, { 2791.95, -2410.21, 14.66 }, { 2797.52, -2410.08, 14.63 }
};
static const gSkladisteKopija[SK_POLICA_B] = { 2, 3, 9, 10, 11 };
static const Float:gSkladisteMjesto[][2] = { { -2.3, -1.92 }, { -0.9, -1.92 }, { 0.6, -1.92 }, { -0.9, -1.16 }, { 2.2, -1.16 }, { -2.3, 0.42 }, { 0.6, 0.42 } };
static const gSkladisteTipModel[] = { 2912, 1220, 1271, 2912 };
static const Float:gSkladisteTipZ[] = { 0.0, 0.35, 0.35, 0.0 };
static const gSkladisteTipCijena[] = { 40, 60, 55, 70 };
static const gSkladisteTipSansa[] = { 50, 20, 15, 15 };
static const gSkladisteTipIme[][] = { "Obicno", "Lomljivo", "Tesko", "Hitno" };

enum E_SK_VILJ { bool:svIma, svDb, svVozilo, svIgrac, svZadnje, Float:svX, Float:svY }
static gSkladisteVilj[SK_VILJUSKARA][E_SK_VILJ];
enum E_SK_RAMPA { bool:srIma, srDb, Float:srX, Float:srY, Float:srZ, Float:srA, STREAMER_TAG_3D_TEXT_LABEL:srTekst }
static gSkladisteRampa[SK_RAMPI][E_SK_RAMPA];
static Float:gSkladistePolica[SK_POLICA][4];
static Float:gSkladisteMjestoPosla[3], gSkladisteIkona, STREAMER_TAG_3D_TEXT_LABEL:gSkladisteTekst, bool:gSkladisteImaMjesto;
static gSkladisteFaza[MAX_PLAYERS], gSkladisteOd[MAX_PLAYERS], gSkladisteDo[MAX_PLAYERS], gSkladisteTip[MAX_PLAYERS], gSkladisteRok[MAX_PLAYERS], gSkladisteNiz[MAX_PLAYERS], gSkladistePromasaj[MAX_PLAYERS];
static Float:gSkladisteCilj[MAX_PLAYERS][3], Float:gSkladisteZdravlje[MAX_PLAYERS];
static STREAMER_TAG_OBJECT:gSkladisteTeret[MAX_PLAYERS], STREAMER_TAG_OBJECT:gSkladisteOstavljen[MAX_PLAYERS];
static PlayerText:gSkladisteHud[MAX_PLAYERS], bool:gSkladisteHudIma[MAX_PLAYERS];
static bool:gSkladisteIgra[MAX_PLAYERS], gSkladisteTimer[MAX_PLAYERS], Float:gSkladisteTraka[MAX_PLAYERS], Float:gSkladisteSmjer[MAX_PLAYERS], Float:gSkladisteZel[MAX_PLAYERS];
static gSkladisteApPopis[MAX_PLAYERS][SK_VILJUSKARA + SK_RAMPI];

InitSkladiste()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_warehouse (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_forklifts (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_ramps (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    for (new k = 0; k < 3; k++) gSkladisteMjestoPosla[k] = gSkladisteZadano[k];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM job_warehouse WHERE id = 1");
    if (DB_GetRowCount(res)) for (new k = 0; k < 3; k++) gSkladisteMjestoPosla[k] = DB_GetFieldFloat(res, k);
    DB_FreeResultSet(res);
    SkladisteMjestoStvori();
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM warehouse_forklifts ORDER BY id LIMIT %d", SK_VILJUSKARA);
    for (new b = 0, n = DB_GetRowCount(res); b < n; b++, DB_SelectNextRow(res))
        SkladisteViljStvori(b, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM warehouse_ramps ORDER BY id LIMIT %d", SK_RAMPI);
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
        SkladisteRampaStvori(r, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
    new Float:c = floatcos(SK_KUT_B, degrees), Float:s = floatsin(SK_KUT_B, degrees);
    for (new p = 0; p < SK_POLICA; p++)
    {
        if (p < SK_POLICA_A)
        {
            for (new k = 0; k < 3; k++) gSkladistePolica[p][k] = gSkladistePolicaA[p][k];
            gSkladistePolica[p][3] = 0.0;
        }
        else
        {
            new q = gSkladisteKopija[p - SK_POLICA_A], Float:ox = gSkladistePolicaA[q][0] - gSkladisteHangar[0][0], Float:oy = gSkladistePolicaA[q][1] - gSkladisteHangar[0][1];
            gSkladistePolica[p][0] = gSkladisteHangar[1][0] + ox * c - oy * s;
            gSkladistePolica[p][1] = gSkladisteHangar[1][1] + ox * s + oy * c;
            gSkladistePolica[p][2] = gSkladistePolicaA[q][2];
            gSkladistePolica[p][3] = SK_KUT_B;
            CreateDynamicObject(SK_POLICA_MODEL, gSkladistePolica[p][0], gSkladistePolica[p][1], gSkladistePolica[p][2], 0.0, 0.0, SK_KUT_B, 0, 0);
        }
        new oznaka[8];
        format(oznaka, sizeof oznaka, "%c%d", p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p));
        CreateDynamic3DTextLabel(oznaka, SK_BOJA, gSkladistePolica[p][0], gSkladistePolica[p][1], gSkladistePolica[p][2] + 2.4, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    }
}

static SkladisteBroj(p)
{
    return p < SK_POLICA_A ? p + 1 : p - SK_POLICA_A + 1;
}

static SkladisteMjestoStvori()
{
    if (gSkladisteImaMjesto)
    {
        MapaIkonaMakni(gSkladisteIkona);
        DestroyDynamic3DTextLabel(gSkladisteTekst);
    }
    gSkladisteImaMjesto = true;
    gSkladisteIkona = MapaIkona(gSkladisteMjestoPosla[0], gSkladisteMjestoPosla[1], gSkladisteMjestoPosla[2], 51);
    gSkladisteTekst = CreateDynamic3DTextLabel("Skladistar\n/posao", SK_BOJA, gSkladisteMjestoPosla[0], gSkladisteMjestoPosla[1], gSkladisteMjestoPosla[2] + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gSkladisteTekst, SK_DOMET);
}

static SkladisteViljStvori(b, db, Float:x, Float:y, Float:z, Float:a)
{
    gSkladisteVilj[b][svIma] = true;
    gSkladisteVilj[b][svDb] = db;
    gSkladisteVilj[b][svX] = x;
    gSkladisteVilj[b][svY] = y;
    gSkladisteVilj[b][svIgrac] = INVALID_PLAYER_ID;
    gSkladisteVilj[b][svVozilo] = CreateVehicle(SK_MODEL, x, y, z, a, 1, 1, -1);
}

static SkladisteRampaStvori(r, db, Float:x, Float:y, Float:z, Float:a)
{
    new oznaka[32];
    gSkladisteRampa[r][srIma] = true;
    gSkladisteRampa[r][srDb] = db;
    gSkladisteRampa[r][srX] = x;
    gSkladisteRampa[r][srY] = y;
    gSkladisteRampa[r][srZ] = z;
    gSkladisteRampa[r][srA] = a;
    format(oznaka, sizeof oznaka, "Utovarna rampa\nR%d", r + 1);
    gSkladisteRampa[r][srTekst] = CreateDynamic3DTextLabel(oznaka, SK_BOJA, x, y, z + 1.0, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

static SkladisteViljIgraca(playerid)
{
    for (new b = 0; b < SK_VILJUSKARA; b++) if (gSkladisteVilj[b][svIma] && gSkladisteVilj[b][svIgrac] == playerid) return b;
    return -1;
}

static SkladisteViljPoVozilu(v)
{
    for (new b = 0; b < SK_VILJUSKARA; b++) if (gSkladisteVilj[b][svIma] && gSkladisteVilj[b][svVozilo] == v) return b;
    return -1;
}

static bool:SkladisteKodPosla(playerid, Float:r = SK_DOMET)
{
    return !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gSkladisteMjestoPosla[0], gSkladisteMjestoPosla[1], gSkladisteMjestoPosla[2]);
}

static SkladisteHangarBlizu(Float:x, Float:y)
{
    return VectorSize(x - gSkladisteHangar[0][0], y - gSkladisteHangar[0][1], 0.0) <= VectorSize(x - gSkladisteHangar[1][0], y - gSkladisteHangar[1][1], 0.0) ? 0 : 1;
}

static SkladisteLokalno(p, Float:lx, Float:ly, &Float:x, &Float:y)
{
    new Float:c = floatcos(gSkladistePolica[p][3], degrees), Float:s = floatsin(gSkladistePolica[p][3], degrees);
    x = gSkladistePolica[p][0] + lx * c - ly * s;
    y = gSkladistePolica[p][1] + lx * s + ly * c;
}

static bool:SkladisteJeRampa(cilj)
{
    return cilj >= SK_SLOTOVA;
}

static bool:SkladisteCiljPostoji(cilj)
{
    return !SkladisteJeRampa(cilj) || gSkladisteRampa[cilj - SK_SLOTOVA][srIma];
}

static Float:SkladisteRazinaZ(cilj)
{
    return SkladisteJeRampa(cilj) ? -2.0 : gSkladisteMjesto[cilj % sizeof gSkladisteMjesto][1];
}

static STREAMER_TAG_OBJECT:SkladisteTeretStvori(playerid, cilj)
{
    new t = gSkladisteTip[playerid], Float:x, Float:y;
    if (SkladisteJeRampa(cilj))
    {
        new r = cilj - SK_SLOTOVA;
        return CreateDynamicObject(gSkladisteTipModel[t], gSkladisteRampa[r][srX], gSkladisteRampa[r][srY], gSkladisteRampa[r][srZ] - 0.95 + gSkladisteTipZ[t], 0.0, 0.0, gSkladisteRampa[r][srA], 0, 0);
    }
    new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto;
    SkladisteLokalno(p, 0.0, gSkladisteMjesto[m][0], x, y);
    return CreateDynamicObject(gSkladisteTipModel[t], x, y, gSkladistePolica[p][2] + gSkladisteMjesto[m][1] + gSkladisteTipZ[t], 0.0, 0.0, gSkladistePolica[p][3], 0, 0);
}

static SkladisteObjektMakni(&STREAMER_TAG_OBJECT:o)
{
    if (_:o) DestroyDynamicObject(o);
    o = STREAMER_TAG_OBJECT:0;
}

static SkladisteCiljPostavi(playerid, cilj)
{
    if (SkladisteJeRampa(cilj))
    {
        new r = cilj - SK_SLOTOVA;
        gSkladisteCilj[playerid][0] = gSkladisteRampa[r][srX];
        gSkladisteCilj[playerid][1] = gSkladisteRampa[r][srY];
        gSkladisteCilj[playerid][2] = gSkladisteRampa[r][srZ] - 0.95;
    }
    else
    {
        new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto;
        SkladisteLokalno(p, random(2) ? SK_PRILAZ : -SK_PRILAZ, gSkladisteMjesto[m][0], gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1]);
        gSkladisteCilj[playerid][2] = gSkladistePolica[p][2] - 1.9;
    }
    SetPlayerCheckpoint(playerid, gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1], gSkladisteCilj[playerid][2], 2.0);
}

static SkladisteOznaka(cilj, oznaka[], len = sizeof oznaka)
{
    if (SkladisteJeRampa(cilj)) return format(oznaka, len, "R%d", cilj - SK_SLOTOVA + 1);
    new p = cilj / sizeof gSkladisteMjesto, Float:z = gSkladisteMjesto[cilj % sizeof gSkladisteMjesto][1];
    return format(oznaka, len, "%c%d (%d)", p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p), z < -1.5 ? 1 : z < 0.0 ? 2 : 3);
}

static SkladisteOpis(playerid, bool:preuzmi, cilj, poruka[], len = sizeof poruka)
{
    if (SkladisteJeRampa(cilj)) return format(poruka, len, preuzmi ? T(playerid, "Preuzmite teret: utovarna rampa %d.") : T(playerid, "Odvezite teret: utovarna rampa %d."), cilj - SK_SLOTOVA + 1);
    new p = cilj / sizeof gSkladisteMjesto, Float:z = gSkladisteMjesto[cilj % sizeof gSkladisteMjesto][1];
    return format(poruka, len, preuzmi ? T(playerid, "Preuzmite teret: skladiste %c, polica %d, razina %d.") : T(playerid, "Odvezite teret: skladiste %c, polica %d, razina %d."), p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p), z < -1.5 ? 1 : z < 0.0 ? 2 : 3);
}

static SkladisteHudOsvjezi(playerid)
{
    new tekst[160], oznaka[16], ostalo = max(0, gSkladisteRok[playerid] - gettime());
    SkladisteOznaka(gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid], oznaka);
    format(tekst, sizeof tekst, T(playerid, "~y~Skladiste~n~~w~Teret: %s~n~Cilj: %s~n~Rok: %s%d:%02d~n~~w~Niz: x%d"), T(playerid, gSkladisteTipIme[gSkladisteTip[playerid]]), oznaka, ostalo ? ("") : ("~r~"), ostalo / 60, ostalo % 60, gSkladisteNiz[playerid]);
    if (!gSkladisteHudIma[playerid])
    {
        gSkladisteHud[playerid] = TdTekst(playerid, 500.0, 150.0, tekst, 0xFFFFFFFF, 0.22, 1.0, TEXT_DRAW_FONT_1);
        PlayerTextDrawShow(playerid, gSkladisteHud[playerid]);
        gSkladisteHudIma[playerid] = true;
    }
    else PlayerTextDrawSetString(playerid, gSkladisteHud[playerid], tekst);
}

static SkladisteHudMakni(playerid)
{
    if (gSkladisteHudIma[playerid]) PlayerTextDrawDestroy(playerid, gSkladisteHud[playerid]);
    gSkladisteHudIma[playerid] = false;
}

static SkladisteRampaZa(h)
{
    new popis[SK_RAMPI], n = 0;
    for (new r = 0; r < SK_RAMPI; r++) if (gSkladisteRampa[r][srIma] && SkladisteHangarBlizu(gSkladisteRampa[r][srX], gSkladisteRampa[r][srY]) == h) popis[n++] = r;
    if (!n) for (new r = 0; r < SK_RAMPI; r++) if (gSkladisteRampa[r][srIma]) popis[n++] = r;
    return n ? SK_SLOTOVA + popis[random(n)] : -1;
}

static SkladisteSlot(h)
{
    return (h == 0 ? random(SK_POLICA_A) : SK_POLICA_A + random(SK_POLICA_B)) * sizeof gSkladisteMjesto + random(sizeof gSkladisteMjesto);
}

static SkladisteTipNasumicno()
{
    new bacanje = random(100);
    for (new t = 0; t < sizeof gSkladisteTipSansa; t++)
    {
        if (bacanje < gSkladisteTipSansa[t]) return t;
        bacanje -= gSkladisteTipSansa[t];
    }
    return SKT_OBICNO;
}

static bool:SkladistePrijenos(playerid)
{
    new a = gSkladisteOd[playerid], b = gSkladisteDo[playerid];
    if (SkladisteJeRampa(a) || SkladisteJeRampa(b)) return false;
    return (a / sizeof gSkladisteMjesto < SK_POLICA_A) != (b / sizeof gSkladisteMjesto < SK_POLICA_A);
}

static SkladisteZadatak(playerid)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new h = SkladisteHangarBlizu(x, y), vrsta = random(100), od = -1, doo = -1;
    if (vrsta < 25) od = SkladisteRampaZa(h);
    else if (vrsta < 50) doo = SkladisteRampaZa(h);
    if (od == -1) od = SkladisteSlot(h);
    if (doo == -1 && od >= SK_SLOTOVA) doo = SkladisteSlot(h);
    if (doo == -1 && vrsta >= 50)
    {
        new dh = vrsta < 70 ? 1 - h : h;
        do doo = SkladisteSlot(dh);
        while (doo / sizeof gSkladisteMjesto == od / sizeof gSkladisteMjesto);
    }
    if (doo == -1) do doo = SkladisteSlot(h);
    while (doo / sizeof gSkladisteMjesto == od / sizeof gSkladisteMjesto);
    gSkladisteOd[playerid] = od;
    gSkladisteDo[playerid] = doo;
    gSkladisteTip[playerid] = SkladisteTipNasumicno();
    gSkladistePromasaj[playerid] = 0;
    gSkladisteRok[playerid] = gettime() + (gSkladisteTip[playerid] == SKT_HITNO ? SK_ROK_HITNO : SK_ROK) + (SkladistePrijenos(playerid) ? SK_ROK_PRIJENOS : 0);
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    gSkladisteTeret[playerid] = SkladisteTeretStvori(playerid, od);
    gSkladisteFaza[playerid] = 1;
    SkladisteCiljPostavi(playerid, od);
    SkladisteHudOsvjezi(playerid);
    new poruka[144];
    SkladisteOpis(playerid, true, od, poruka);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
    switch (gSkladisteTip[playerid])
    {
        case SKT_LOMLJIVO: Savjet(playerid, T(playerid, "~r~Lomljivo!~w~ Ne udarajte viljuskarom dok nosite teret."), 5000);
        case SKT_TESKO: Savjet(playerid, T(playerid, "~y~Tesko!~w~ Viljuske se teze namjestaju."), 5000);
        case SKT_HITNO: Savjet(playerid, T(playerid, "~y~Hitno!~w~ Kraci rok, veca zarada."), 5000);
    }
}

static SkladisteIgraKraj(playerid)
{
    if (!gSkladisteIgra[playerid]) return;
    gSkladisteIgra[playerid] = false;
    if (gSkladisteTimer[playerid]) KillTimer(gSkladisteTimer[playerid]);
    gSkladisteTimer[playerid] = 0;
    IgraTdMakni(playerid);
}

static SkladisteZadatakMakni(playerid)
{
    SkladisteIgraKraj(playerid);
    if (gSkladisteFaza[playerid]) DisablePlayerCheckpoint(playerid);
    gSkladisteFaza[playerid] = 0;
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    SkladisteObjektMakni(gSkladisteOstavljen[playerid]);
    SkladisteHudMakni(playerid);
}

static SkladisteViljOslobodi(b)
{
    new i = gSkladisteVilj[b][svIgrac];
    gSkladisteVilj[b][svIgrac] = INVALID_PLAYER_ID;
    if (i != INVALID_PLAYER_ID && IsPlayerConnected(i))
    {
        SkladisteZadatakMakni(i);
        gSkladisteNiz[i] = 0;
        if (GetPlayerSkin(i) == SK_SKIN) SetPlayerSkin(i, OdjecaSkinIgraca(i));
    }
    SetVehicleToRespawn(gSkladisteVilj[b][svVozilo]);
}

SkladisteKraj(playerid)
{
    new b = SkladisteViljIgraca(playerid);
    if (b != -1) SkladisteViljOslobodi(b);
    SkladisteZadatakMakni(playerid);
    gSkladisteNiz[playerid] = 0;
}

static SkladisteUzmi(playerid)
{
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        if (!gSkladisteVilj[b][svIma] || gSkladisteVilj[b][svIgrac] != INVALID_PLAYER_ID) continue;
        gSkladisteVilj[b][svIgrac] = playerid;
        gSkladisteVilj[b][svZadnje] = gettime();
        Fade(playerid, 0);
        SetTimerEx("SkladisteViljDan", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "iii", playerid, gAccountId[playerid], b);
        return 1;
    }
    return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih viljuskara."));
}

forward SkladisteViljDan(playerid, racun, b);
public SkladisteViljDan(playerid, racun, b)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    Fade(playerid, FADE_SLOJEVA + 1);
    if (gSkladisteVilj[b][svIgrac] != playerid) return;
    gSkladisteVilj[b][svZadnje] = gettime();
    gSkladisteNiz[playerid] = 0;
    SetPlayerSkin(playerid, SK_SKIN);
    PutPlayerInVehicle(playerid, gSkladisteVilj[b][svVozilo], 0);
    SkladisteZadatak(playerid);
    Savjet(playerid, T(playerid, "~y~SPACE~w~ - podigni ili spusti teret u oznaci~n~~y~/posao~w~ - vrati viljuskar"), 6000);
}

static SkladisteVrati(playerid)
{
    if (SkladisteViljIgraca(playerid) == -1) return 0;
    Fade(playerid, 0);
    SetTimerEx("SkladisteVratiKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

forward SkladisteVratiKraj(playerid, racun);
public SkladisteVratiKraj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    Fade(playerid, FADE_SLOJEVA + 1);
    new b = SkladisteViljIgraca(playerid);
    if (b == -1) return;
    new bool:uVozilu = GetPlayerVehicleID(playerid) == gSkladisteVilj[b][svVozilo];
    SkladisteViljOslobodi(b);
    if (uVozilu || !SkladisteKodPosla(playerid, 3.0)) SetPlayerPos(playerid, gSkladisteMjestoPosla[0], gSkladisteMjestoPosla[1], gSkladisteMjestoPosla[2]);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), T(playerid, "Vratili ste viljuskar. Zaradu dobivate na placi."));
}

static SkladisteIzbornik(playerid)
{
    if (PosaoIgraca(playerid) != POSAO_SKLADISTAR) return UpitDaNe(playerid, UPIT_SKLADISTE, 0, 0, T(playerid, "Skladistar"), T(playerid, "Zelite li se zaposliti kao skladistar?"));
    if (SkladisteViljIgraca(playerid) == -1) return UpitDaNe(playerid, UPIT_SKLADISTE, 0, 1, T(playerid, "Skladistar"), T(playerid, "Zelite li uzeti viljuskar?"));
    return UpitDaNe(playerid, UPIT_SKLADISTE, 0, 2, T(playerid, "Skladistar"), T(playerid, "Zelite li vratiti viljuskar?"));
}

SkladisteUpit(playerid, opcija, bool:da)
{
    if (!da || !SkladisteKodPosla(playerid, opcija == 2 ? SK_POVRAT_DOMET : SK_DOMET + 2.0)) return 1;
    if (opcija == 0 && PosaoIgraca(playerid) != POSAO_SKLADISTAR)
    {
        RibarKraj(playerid);
        PosaoPostavi(playerid, POSAO_SKLADISTAR);
        return ObavijestInfo(playerid, T(playerid, "Skladistar"), T(playerid, "Zaposleni ste kao skladistar. Pritisnite ALT ovdje (ili /posao) da dobijete viljuskar."));
    }
    if (PosaoIgraca(playerid) != POSAO_SKLADISTAR) return 1;
    if (opcija == 1 && SkladisteViljIgraca(playerid) == -1) return SkladisteUzmi(playerid);
    if (opcija == 2) return SkladisteVrati(playerid);
    return 1;
}

SkladistePosao(playerid)
{
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && SkladisteKodPosla(playerid)) return SkladisteIzbornik(playerid), 1;
    if (PosaoIgraca(playerid) != POSAO_SKLADISTAR) return 0;
    if (SkladisteViljIgraca(playerid) == -1) return ObavijestGreska(playerid, T(playerid, "Viljuskar uzimate na mjestu za posao u luci (oznaceno na karti).")), 1;
    if (!SkladisteKodPosla(playerid, SK_POVRAT_DOMET)) return ObavijestGreska(playerid, T(playerid, "Viljuskar vratite na mjesto za posao u luci.")), 1;
    return SkladisteVrati(playerid);
}

SkladisteAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || !SkladisteKodPosla(playerid)) return 0;
    return SkladisteIzbornik(playerid), 1;
}

static bool:SkladisteVozi(playerid)
{
    new b = SkladisteViljIgraca(playerid);
    return b != -1 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) == gSkladisteVilj[b][svVozilo];
}

SkladisteCheckpoint(playerid)
{
    if (!gSkladisteFaza[playerid] || !IsPlayerInRangeOfPoint(playerid, SK_CILJ_DOMET, gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1], gSkladisteCilj[playerid][2])) return 0;
    if (gSkladisteIgra[playerid]) return 1;
    if (!SkladisteVozi(playerid)) return ObavijestGreska(playerid, T(playerid, "Teret preuzimate i odlazete svojim viljuskarom.")), 1;
    new Float:z = SkladisteRazinaZ(gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid]);
    gSkladisteIgra[playerid] = true;
    gSkladisteTraka[playerid] = 0.0;
    gSkladisteSmjer[playerid] = z < -1.5 ? 1.6 : z < 0.0 ? 2.0 : 2.4;
    if (gSkladisteTip[playerid] == SKT_TESKO) gSkladisteSmjer[playerid] *= 1.35;
    gSkladisteZel[playerid] = float(10 + random(72));
    SkladistePanel(playerid);
    gSkladisteTimer[playerid] = SetTimerEx("SkladisteTrakaKorak", SK_TRAKA_MS, true, "i", playerid);
    return 1;
}

static Float:SkladisteZelenoSirina(playerid)
{
    return gSkladisteTip[playerid] == SKT_TESKO ? SK_ZELENO_TESKO : SK_ZELENO;
}

static SkladistePanel(playerid)
{
    IgraPanel(playerid, gSkladisteFaza[playerid] == 1 ? T(playerid, "Podizanje tereta") : T(playerid, "Odlaganje tereta"), 60.0, SK_BOJA);
    IgraTraka(playerid, gSkladisteZel[playerid], gSkladisteZel[playerid] + SkladisteZelenoSirina(playerid), 0x3A2A1AFF, 0x27AE60FF, 0x3A2A1AFF, T(playerid, "Namjestite viljuske: ~y~SPACE~w~ kad je oznaka u zelenom."));
    IgraOznakaPomakni(playerid, gSkladisteTraka[playerid]);
}

forward SkladisteTrakaKorak(playerid);
public SkladisteTrakaKorak(playerid)
{
    if (!gSkladisteIgra[playerid]) return;
    gSkladisteTraka[playerid] += gSkladisteSmjer[playerid];
    if (gSkladisteTraka[playerid] >= 100.0 || gSkladisteTraka[playerid] <= 0.0) gSkladisteSmjer[playerid] = -gSkladisteSmjer[playerid];
    IgraOznakaPomakni(playerid, gSkladisteTraka[playerid]);
}

static SkladistePredaj(playerid)
{
    new poruka[144], iznos = gSkladisteTipCijena[gSkladisteTip[playerid]], bool:kasni = gettime() > gSkladisteRok[playerid];
    if (SkladistePrijenos(playerid)) iznos += SK_PRIJENOS;
    if (SkladisteJeRampa(gSkladisteOd[playerid]) || SkladisteJeRampa(gSkladisteDo[playerid])) iznos += SK_RAMPA_BONUS;
    iznos += iznos * min(gSkladisteNiz[playerid], SK_NIZ_MAX) * 10 / 100;
    iznos -= iznos * min(gSkladistePromasaj[playerid], 5) * 10 / 100;
    if (kasni)
    {
        iznos /= 2;
        gSkladisteNiz[playerid] = 0;
        format(poruka, sizeof poruka, T(playerid, "Teret je dostavljen sa zakasnjenjem. +$%d na placi."), iznos);
    }
    else
    {
        if (!gSkladistePromasaj[playerid]) gSkladisteNiz[playerid]++;
        format(poruka, sizeof poruka, T(playerid, "Teret je dostavljen. +$%d na placi (niz x%d)."), iznos, gSkladisteNiz[playerid]);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = payday_earn + %d WHERE id = %d", iznos, gAccountId[playerid]));
    IgraObavijest(playerid, T(playerid, "Skladiste"), kasni ? OBAVIJEST_GRESKA : SK_BOJA, poruka);
}

bool:SkladisteKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gSkladisteIgra[playerid]) return false;
    if (!(newkeys & KEY_HANDBRAKE) || (oldkeys & KEY_HANDBRAKE)) return true;
    if (gSkladisteTraka[playerid] < gSkladisteZel[playerid] || gSkladisteTraka[playerid] > gSkladisteZel[playerid] + SkladisteZelenoSirina(playerid))
    {
        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
        gSkladistePromasaj[playerid]++;
        gSkladisteZel[playerid] = float(10 + random(72));
        SkladistePanel(playerid);
        return true;
    }
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    SkladisteIgraKraj(playerid);
    if (!SkladisteCiljPostoji(gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid]))
    {
        SkladisteZadatak(playerid);
        return true;
    }
    new t = gSkladisteTip[playerid], v = GetPlayerVehicleID(playerid);
    if (gSkladisteFaza[playerid] == 1)
    {
        new poruka[144];
        SkladisteObjektMakni(gSkladisteTeret[playerid]);
        gSkladisteTeret[playerid] = CreateDynamicObject(gSkladisteTipModel[t], 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0);
        AttachDynamicObjectToVehicle(gSkladisteTeret[playerid], v, 0.0, 1.1, -0.4 + gSkladisteTipZ[t], 0.0, 0.0, 0.0);
        GetVehicleHealth(v, gSkladisteZdravlje[playerid]);
        gSkladisteFaza[playerid] = 2;
        SkladisteCiljPostavi(playerid, gSkladisteDo[playerid]);
        SkladisteHudOsvjezi(playerid);
        SkladisteOpis(playerid, false, gSkladisteDo[playerid], poruka);
        ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
        return true;
    }
    DisablePlayerCheckpoint(playerid);
    gSkladisteFaza[playerid] = 0;
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    SkladisteObjektMakni(gSkladisteOstavljen[playerid]);
    gSkladisteOstavljen[playerid] = SkladisteTeretStvori(playerid, gSkladisteDo[playerid]);
    SkladistePredaj(playerid);
    SkladisteZadatak(playerid);
    return true;
}

SkladisteStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER) return;
    new b = SkladisteViljPoVozilu(GetPlayerVehicleID(playerid));
    if (b == -1) return;
    if (gSkladisteVilj[b][svIgrac] == playerid)
    {
        gSkladisteVilj[b][svZadnje] = gettime();
        return;
    }
    RemovePlayerFromVehicle(playerid);
    ObavijestGreska(playerid, gSkladisteVilj[b][svIgrac] == INVALID_PLAYER_ID ? T(playerid, "Viljuskar uzimate na mjestu za posao u luci (oznaceno na karti).") : T(playerid, "Ovaj viljuskar koristi drugi skladistar."));
}

static SkladisteRazbij(playerid)
{
    SkladisteIgraKraj(playerid);
    DisablePlayerCheckpoint(playerid);
    gSkladisteFaza[playerid] = 0;
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    gSkladisteNiz[playerid] = 0;
    IgraObavijest(playerid, T(playerid, "Skladiste"), OBAVIJEST_GRESKA, T(playerid, "Lomljivi teret se razbio. Niz je prekinut."));
    SkladisteZadatak(playerid);
}

SkladisteTick()
{
    new sad = gettime();
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        new i = gSkladisteVilj[b][svIgrac];
        if (!gSkladisteVilj[b][svIma] || i == INVALID_PLAYER_ID) continue;
        if (!IsPlayerConnected(i) || PosaoIgraca(i) != POSAO_SKLADISTAR || IsRanjen(i))
        {
            SkladisteKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Smjena je zavrsena. Viljuskar je vracen."));
            continue;
        }
        if (gSkladisteIgra[i] && (!SkladisteVozi(i) || !IsPlayerInRangeOfPoint(i, SK_CILJ_DOMET + 2.0, gSkladisteCilj[i][0], gSkladisteCilj[i][1], gSkladisteCilj[i][2]))) SkladisteIgraKraj(i);
        if (gSkladisteFaza[i] == 2 && gSkladisteTip[i] == SKT_LOMLJIVO)
        {
            new Float:hp;
            GetVehicleHealth(gSkladisteVilj[b][svVozilo], hp);
            if (hp < gSkladisteZdravlje[i] - SK_LOMLJIVO_STETA) SkladisteRazbij(i);
        }
        if (gSkladisteFaza[i] && gSkladisteHudIma[i]) SkladisteHudOsvjezi(i);
        if (GetPlayerVehicleID(i) == gSkladisteVilj[b][svVozilo])
        {
            gSkladisteVilj[b][svZadnje] = sad;
            continue;
        }
        if (sad - gSkladisteVilj[b][svZadnje] < SK_MIRUJE_S) continue;
        SkladisteViljOslobodi(b);
        ObavijestGreska(i, T(i, "Viljuskar je vracen jer ga 15 minuta niste koristili."));
    }
}

ShowSkladisteAdmin(playerid)
{
    new list[1536], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto zaposlenja skladistara na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Viljuskar na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Utovarna rampa na mojoj poziciji"));
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        if (!gSkladisteVilj[b][svIma]) continue;
        format(row, sizeof row, T(playerid, "Viljuskar [%d] %s - obrisi"), gSkladisteVilj[b][svDb], CestaZona(gSkladisteVilj[b][svX], gSkladisteVilj[b][svY]));
        DodajStupac(list, len, row);
        gSkladisteApPopis[playerid][n++] = b;
    }
    for (new r = 0; r < SK_RAMPI; r++)
    {
        if (!gSkladisteRampa[r][srIma]) continue;
        format(row, sizeof row, T(playerid, "Utovarna rampa R%d %s - obrisi"), r + 1, CestaZona(gSkladisteRampa[r][srX], gSkladisteRampa[r][srY]));
        DodajStupac(list, len, row);
        gSkladisteApPopis[playerid][n++] = SK_VILJUSKARA + r;
    }
    return PrikaziDialog(playerid, DLG_APANEL_SKLADISTE, DIALOG_STYLE_LIST, T(playerid, "Skladistari"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_skladiste(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    if (listitem <= 2)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowSkladisteAdmin(playerid);
        new v = GetPlayerVehicleID(playerid);
        if (v) GetVehiclePos(v, x, y, z), GetVehicleZAngle(v, a);
        else GetPlayerPos(playerid, x, y, z), GetPlayerFacingAngle(playerid, a);
    }
    if (listitem == 0)
    {
        gSkladisteMjestoPosla[0] = x;
        gSkladisteMjestoPosla[1] = y;
        gSkladisteMjestoPosla[2] = z;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_warehouse (id, x, y, z) VALUES (1, %f, %f, %f)", x, y, z));
        SkladisteMjestoStvori();
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto zaposlenja skladistara postavljeno."));
        return ShowSkladisteAdmin(playerid);
    }
    if (listitem == 1)
    {
        new b = 0;
        while (b < SK_VILJUSKARA && gSkladisteVilj[b][svIma]) b++;
        if (b == SK_VILJUSKARA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowSkladisteAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO warehouse_forklifts (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        SkladisteViljStvori(b, ZadnjiId(), x, y, z, a);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Viljuskar postavljen."));
        return ShowSkladisteAdmin(playerid);
    }
    if (listitem == 2)
    {
        if (GetPlayerVehicleID(playerid)) return ObavijestGreska(playerid, T(playerid, "Rampu postavljate pjeske.")), ShowSkladisteAdmin(playerid);
        new r = 0;
        while (r < SK_RAMPI && gSkladisteRampa[r][srIma]) r++;
        if (r == SK_RAMPI) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowSkladisteAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO warehouse_ramps (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        SkladisteRampaStvori(r, ZadnjiId(), x, y, z, a);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Utovarna rampa postavljena."));
        return ShowSkladisteAdmin(playerid);
    }
    new i = listitem - 3;
    if (i < 0 || i >= sizeof gSkladisteApPopis[]) return 1;
    new k = gSkladisteApPopis[playerid][i];
    if (k >= SK_VILJUSKARA)
    {
        k -= SK_VILJUSKARA;
        if (!gSkladisteRampa[k][srIma]) return ShowSkladisteAdmin(playerid);
        DestroyDynamic3DTextLabel(gSkladisteRampa[k][srTekst]);
        gSkladisteRampa[k][srIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_ramps WHERE id = %d", gSkladisteRampa[k][srDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Utovarna rampa obrisana."));
        return ShowSkladisteAdmin(playerid);
    }
    if (!gSkladisteVilj[k][svIma]) return ShowSkladisteAdmin(playerid);
    if (gSkladisteVilj[k][svIgrac] != INVALID_PLAYER_ID) SkladisteViljOslobodi(k);
    DestroyVehicle(gSkladisteVilj[k][svVozilo]);
    gSkladisteVilj[k][svIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_forklifts WHERE id = %d", gSkladisteVilj[k][svDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Viljuskar obrisan."));
    return ShowSkladisteAdmin(playerid);
}
