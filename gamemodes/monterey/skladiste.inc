#define SK_VILJUSKARA       10
#define SK_MODEL            530
#define SK_DOMET            2.5
#define SK_POVRAT_DOMET     20.0
#define SK_MIRUJE_S         900
#define SK_TRAKA_MS         40
#define SK_ZELENO           18.0
#define SK_CILJ_DOMET       4.0
#define SK_PRILAZ           2.6
#define SK_PLACA            40
#define SK_PLACA_PRIJENOS   120
#define SK_PRIJENOS_SANSA   25
#define SK_BOJA             0xE67E22FF
#define SK_POLICA_MODEL     3761
#define SK_POLICA_A         15
#define SK_POLICA_B         5
#define SK_POLICA           (SK_POLICA_A + SK_POLICA_B)
#define SK_KUT_B            -135.0

static const Float:gSkladisteZadano[3] = { 2770.0, -2456.0, 13.6 };
static const Float:gSkladisteHangar[2][3] = { { 2788.16, -2455.88, 16.73 }, { 2415.46, -2468.58, 16.73 } };
static const Float:gSkladistePolicaA[SK_POLICA_A][3] = {
    { 2783.78, -2501.84, 14.70 }, { 2783.78, -2486.96, 14.66 }, { 2783.78, -2463.82, 14.63 }, { 2783.78, -2448.48, 14.63 }, { 2783.78, -2425.35, 14.63 },
    { 2783.78, -2410.21, 14.67 }, { 2791.95, -2501.84, 14.63 }, { 2797.52, -2486.83, 14.63 }, { 2791.95, -2486.96, 14.63 }, { 2791.95, -2463.82, 14.63 },
    { 2797.52, -2448.34, 14.63 }, { 2791.95, -2448.48, 14.63 }, { 2791.95, -2425.35, 14.67 }, { 2791.95, -2410.21, 14.66 }, { 2797.52, -2410.08, 14.63 }
};
static const gSkladisteKopija[SK_POLICA_B] = { 2, 3, 9, 10, 11 };
static const Float:gSkladisteMjesto[][2] = { { -2.3, -1.92 }, { -0.9, -1.92 }, { 0.6, -1.92 }, { -0.9, -1.16 }, { 2.2, -1.16 }, { -2.3, 0.42 }, { 0.6, 0.42 } };
static const gSkladisteTeretModel[] = { 2912, 1220 };
static const Float:gSkladisteTeretZ[] = { 0.0, 0.35 };

enum E_SK_VILJ { bool:svIma, svDb, svVozilo, svIgrac, svZadnje, Float:svX, Float:svY }
static gSkladisteVilj[SK_VILJUSKARA][E_SK_VILJ];
static Float:gSkladistePolica[SK_POLICA][4];
static Float:gSkladisteMjestoPosla[3], gSkladisteIkona, STREAMER_TAG_3D_TEXT_LABEL:gSkladisteTekst, bool:gSkladisteImaMjesto;
static gSkladisteFaza[MAX_PLAYERS], gSkladisteOd[MAX_PLAYERS], gSkladisteDo[MAX_PLAYERS], gSkladisteStrana[MAX_PLAYERS], gSkladisteVrsta[MAX_PLAYERS], Float:gSkladisteCilj[MAX_PLAYERS][3];
static STREAMER_TAG_OBJECT:gSkladisteTeret[MAX_PLAYERS], STREAMER_TAG_OBJECT:gSkladisteOstavljen[MAX_PLAYERS];
static bool:gSkladisteIgra[MAX_PLAYERS], gSkladisteTimer[MAX_PLAYERS], Float:gSkladisteTraka[MAX_PLAYERS], Float:gSkladisteSmjer[MAX_PLAYERS], Float:gSkladisteZel[MAX_PLAYERS];
static gSkladisteApPopis[MAX_PLAYERS][SK_VILJUSKARA];

InitSkladiste()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_warehouse (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_forklifts (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    for (new k = 0; k < 3; k++) gSkladisteMjestoPosla[k] = gSkladisteZadano[k];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM job_warehouse WHERE id = 1");
    if (DB_GetRowCount(res)) for (new k = 0; k < 3; k++) gSkladisteMjestoPosla[k] = DB_GetFieldFloat(res, k);
    DB_FreeResultSet(res);
    SkladisteMjestoStvori();
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM warehouse_forklifts ORDER BY id LIMIT %d", SK_VILJUSKARA);
    for (new b = 0, n = DB_GetRowCount(res); b < n; b++, DB_SelectNextRow(res))
        SkladisteViljStvori(b, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
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

static SkladisteLokalno(p, Float:lx, Float:ly, &Float:x, &Float:y)
{
    new Float:c = floatcos(gSkladistePolica[p][3], degrees), Float:s = floatsin(gSkladistePolica[p][3], degrees);
    x = gSkladistePolica[p][0] + lx * c - ly * s;
    y = gSkladistePolica[p][1] + lx * s + ly * c;
}

static STREAMER_TAG_OBJECT:SkladisteTeretStvori(playerid, cilj)
{
    new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto, Float:x, Float:y, v = gSkladisteVrsta[playerid];
    SkladisteLokalno(p, 0.0, gSkladisteMjesto[m][0], x, y);
    return CreateDynamicObject(gSkladisteTeretModel[v], x, y, gSkladistePolica[p][2] + gSkladisteMjesto[m][1] + gSkladisteTeretZ[v], 0.0, 0.0, gSkladistePolica[p][3], 0, 0);
}

static SkladisteObjektMakni(&STREAMER_TAG_OBJECT:o)
{
    if (_:o) DestroyDynamicObject(o);
    o = STREAMER_TAG_OBJECT:0;
}

static SkladisteCiljPostavi(playerid, cilj)
{
    new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto, Float:x, Float:y;
    gSkladisteStrana[playerid] = random(2) ? 1 : -1;
    SkladisteLokalno(p, SK_PRILAZ * float(gSkladisteStrana[playerid]), gSkladisteMjesto[m][0], x, y);
    gSkladisteCilj[playerid][0] = x;
    gSkladisteCilj[playerid][1] = y;
    gSkladisteCilj[playerid][2] = gSkladistePolica[p][2] - 1.9;
    SetPlayerCheckpoint(playerid, x, y, gSkladisteCilj[playerid][2], 2.0);
}

static SkladisteOpis(playerid, const uvod[], cilj, poruka[], len = sizeof poruka)
{
    new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto, razina = gSkladisteMjesto[m][1] < -1.5 ? 1 : gSkladisteMjesto[m][1] < 0.0 ? 2 : 3;
    format(poruka, len, T(playerid, uvod), p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p), razina);
}

static SkladisteZadatak(playerid)
{
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new h = VectorSize(x - gSkladisteHangar[0][0], y - gSkladisteHangar[0][1], 0.0) <= VectorSize(x - gSkladisteHangar[1][0], y - gSkladisteHangar[1][1], 0.0) ? 0 : 1;
    new dh = random(100) < SK_PRIJENOS_SANSA ? 1 - h : h, od, doo;
    od = h == 0 ? random(SK_POLICA_A) : SK_POLICA_A + random(SK_POLICA_B);
    do doo = dh == 0 ? random(SK_POLICA_A) : SK_POLICA_A + random(SK_POLICA_B);
    while (doo == od);
    gSkladisteOd[playerid] = od * sizeof gSkladisteMjesto + random(sizeof gSkladisteMjesto);
    gSkladisteDo[playerid] = doo * sizeof gSkladisteMjesto + random(sizeof gSkladisteMjesto);
    gSkladisteVrsta[playerid] = random(sizeof gSkladisteTeretModel);
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    gSkladisteTeret[playerid] = SkladisteTeretStvori(playerid, gSkladisteOd[playerid]);
    gSkladisteFaza[playerid] = 1;
    SkladisteCiljPostavi(playerid, gSkladisteOd[playerid]);
    new poruka[144];
    SkladisteOpis(playerid, "Preuzmite teret: skladiste %c, polica %d, razina %d.", gSkladisteOd[playerid], poruka);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
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
}

static SkladisteViljOslobodi(b)
{
    new i = gSkladisteVilj[b][svIgrac];
    gSkladisteVilj[b][svIgrac] = INVALID_PLAYER_ID;
    if (i != INVALID_PLAYER_ID && IsPlayerConnected(i)) SkladisteZadatakMakni(i);
    SetVehicleToRespawn(gSkladisteVilj[b][svVozilo]);
}

SkladisteKraj(playerid)
{
    new b = SkladisteViljIgraca(playerid);
    if (b != -1) SkladisteViljOslobodi(b);
    SkladisteZadatakMakni(playerid);
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
    new m = (gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid]) % sizeof gSkladisteMjesto;
    gSkladisteIgra[playerid] = true;
    gSkladisteTraka[playerid] = 0.0;
    gSkladisteSmjer[playerid] = gSkladisteMjesto[m][1] < -1.5 ? 1.6 : gSkladisteMjesto[m][1] < 0.0 ? 2.0 : 2.4;
    gSkladisteZel[playerid] = float(10 + random(72));
    SkladistePanel(playerid);
    gSkladisteTimer[playerid] = SetTimerEx("SkladisteTrakaKorak", SK_TRAKA_MS, true, "i", playerid);
    return 1;
}

static SkladistePanel(playerid)
{
    IgraPanel(playerid, gSkladisteFaza[playerid] == 1 ? T(playerid, "Podizanje tereta") : T(playerid, "Odlaganje na policu"), 60.0, SK_BOJA);
    IgraTraka(playerid, gSkladisteZel[playerid], gSkladisteZel[playerid] + SK_ZELENO, 0x3A2A1AFF, 0x27AE60FF, 0x3A2A1AFF, T(playerid, "Namjestite viljuske: ~y~SPACE~w~ kad je oznaka u zelenom."));
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

bool:SkladisteKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gSkladisteIgra[playerid]) return false;
    if (!(newkeys & KEY_HANDBRAKE) || (oldkeys & KEY_HANDBRAKE)) return true;
    if (gSkladisteTraka[playerid] < gSkladisteZel[playerid] || gSkladisteTraka[playerid] > gSkladisteZel[playerid] + SK_ZELENO)
    {
        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
        gSkladisteZel[playerid] = float(10 + random(72));
        SkladistePanel(playerid);
        return true;
    }
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    SkladisteIgraKraj(playerid);
    new poruka[144], v = gSkladisteVrsta[playerid];
    if (gSkladisteFaza[playerid] == 1)
    {
        SkladisteObjektMakni(gSkladisteTeret[playerid]);
        gSkladisteTeret[playerid] = CreateDynamicObject(gSkladisteTeretModel[v], 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0);
        AttachDynamicObjectToVehicle(gSkladisteTeret[playerid], GetPlayerVehicleID(playerid), 0.0, 1.1, -0.4 + gSkladisteTeretZ[v], 0.0, 0.0, 0.0);
        gSkladisteFaza[playerid] = 2;
        SkladisteCiljPostavi(playerid, gSkladisteDo[playerid]);
        SkladisteOpis(playerid, "Odvezite teret: skladiste %c, polica %d, razina %d.", gSkladisteDo[playerid], poruka);
        ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
        return true;
    }
    DisablePlayerCheckpoint(playerid);
    gSkladisteFaza[playerid] = 0;
    SkladisteObjektMakni(gSkladisteTeret[playerid]);
    SkladisteObjektMakni(gSkladisteOstavljen[playerid]);
    gSkladisteOstavljen[playerid] = SkladisteTeretStvori(playerid, gSkladisteDo[playerid]);
    new iznos = (gSkladisteOd[playerid] / sizeof gSkladisteMjesto < SK_POLICA_A) != (gSkladisteDo[playerid] / sizeof gSkladisteMjesto < SK_POLICA_A) ? SK_PLACA_PRIJENOS : SK_PLACA;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = payday_earn + %d WHERE id = %d", iznos, gAccountId[playerid]));
    format(poruka, sizeof poruka, T(playerid, "Teret je na polici. +$%d na placi."), iznos);
    IgraObavijest(playerid, T(playerid, "Skladiste"), SK_BOJA, poruka);
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
    new list[1024], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto zaposlenja skladistara na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Viljuskar na mojoj poziciji"));
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        if (!gSkladisteVilj[b][svIma]) continue;
        format(row, sizeof row, T(playerid, "Viljuskar [%d] %s - obrisi"), gSkladisteVilj[b][svDb], CestaZona(gSkladisteVilj[b][svX], gSkladisteVilj[b][svY]));
        DodajStupac(list, len, row);
        gSkladisteApPopis[playerid][n++] = b;
    }
    return PrikaziDialog(playerid, DLG_APANEL_SKLADISTE, DIALOG_STYLE_LIST, T(playerid, "Skladistari"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_skladiste(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    if (listitem <= 1)
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
    new i = listitem - 2;
    if (i < 0 || i >= sizeof gSkladisteApPopis[]) return 1;
    new b = gSkladisteApPopis[playerid][i];
    if (!gSkladisteVilj[b][svIma]) return ShowSkladisteAdmin(playerid);
    if (gSkladisteVilj[b][svIgrac] != INVALID_PLAYER_ID) SkladisteViljOslobodi(b);
    DestroyVehicle(gSkladisteVilj[b][svVozilo]);
    gSkladisteVilj[b][svIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_forklifts WHERE id = %d", gSkladisteVilj[b][svDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Viljuskar obrisan."));
    return ShowSkladisteAdmin(playerid);
}
