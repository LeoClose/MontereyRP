#define SK_VILJUSKARA       10
#define SK_RAMPI            6
#define SK_PALETA           16
#define SK_MODEL            530
#define SK_SKIN             27
#define SK_NPC_SKIN         260
#define SK_VOZAC_SKIN       202
#define SK_DOMET            2.5
#define SK_POVRAT_DOMET     20.0
#define SK_MIRUJE_S         900
#define SK_CILJ_DOMET       4.0
#define SK_PRILAZ           2.3
#define SK_PRIJENOS         80
#define SK_RAMPA_BONUS      25
#define SK_ROK              300
#define SK_ROK_HITNO        180
#define SK_ROK_PRIJENOS     180
#define SK_NIZ_MAX          5
#define SK_STETA            80.0
#define SK_LOMLJIVO_STETA   25.0
#define SK_TERET_MODEL      2912
#define SK_STRELICA_MODEL   1318
#define SK_PALETA_MODEL     3577
#define SK_PALETA_VISINA    0.12
#define SK_PALETA_SPUSTI    0.62
#define SK_PALETA_X         -0.265
#define SK_PALETA_Y         0.64
#define SK_PALETA_DOMET     1.5
#define SK_ZALIHA_POSTO     55
#define SK_VILJ_TIK_MS      100
#define SK_VILJ_HOD         1.5
#define SK_VILJ_JEDINICA    2500
#define SK_TERET_Y          1.45
#define SK_TERET_Z          -0.44
#define SK_VILJ_UZMI        0.6
#define SK_VILJ_PALETA      0.3
#define SK_VILJ_VRH         2.1
#define SK_VILJ_PODIGNI     0.05
#define SK_VILJ_NAPRIJED    1.0
#define SK_DOLAZAK_RAZMAK   20
#define SK_NPC_BRZINA       4.0
#define SK_CEKA_S           360
#define SK_DOLAZI_S         180
#define SK_ODLAZI_S         40
#define SK_LIK_ZONA         4000
#define SK_KAMION_MODEL     414
#define SK_KAMION_MJESTA    4
#define SK_KAMION_STRAG     -3.4
#define SK_KAMION_POD       -0.1
#define SK_BOJA             0xE67E22FF
#define SK_POLICA_MODEL     3761
#define SK_POLICA_A         15
#define SK_POLICA_B         5
#define SK_POLICA           (SK_POLICA_A + SK_POLICA_B)
#define SK_SLOTOVA          (SK_POLICA * sizeof gSkladisteMjesto)
#define SK_CILJ_PALETA      SK_SLOTOVA
#define SK_CILJ_RAMPA       (SK_SLOTOVA + SK_PALETA)
#define SK_KUT_B            -135.0

enum { SKT_OBICNO, SKT_LOMLJIVO, SKT_TESKO, SKT_HITNO }
enum { SKC_SLOT, SKC_PALETA, SKC_RAMPA }
enum { SKD_NEMA, SKD_DOLAZI, SKD_CEKA, SKD_ODLAZI }

static const Float:gSkladisteZadano[3] = { 2770.0, -2456.0, 13.6 };
static const Float:gSkladisteHangar[2][3] = { { 2788.16, -2455.88, 16.73 }, { 2415.46, -2468.58, 16.73 } };
static const Float:gSkladisteKutHangara[2] = { 0.0, SK_KUT_B };
static const Float:gSkladistePolicaA[SK_POLICA_A][3] = {
    { 2783.78, -2501.84, 14.70 }, { 2783.78, -2486.96, 14.66 }, { 2783.78, -2463.82, 14.63 }, { 2783.78, -2448.48, 14.63 }, { 2783.78, -2425.35, 14.63 },
    { 2783.78, -2410.21, 14.67 }, { 2791.95, -2501.84, 14.63 }, { 2797.52, -2486.83, 14.63 }, { 2791.95, -2486.96, 14.63 }, { 2791.95, -2463.82, 14.63 },
    { 2797.52, -2448.34, 14.63 }, { 2791.95, -2448.48, 14.63 }, { 2791.95, -2425.35, 14.67 }, { 2791.95, -2410.21, 14.66 }, { 2797.52, -2410.08, 14.63 }
};
static const gSkladisteKopija[SK_POLICA_B] = { 2, 3, 9, 10, 11 };
static const Float:gSkladisteMjesto[][2] = { { -2.3, -1.92 }, { -0.9, -1.92 }, { 0.6, -1.92 } };
static const gSkladisteTipCijena[] = { 40, 60, 55, 70 };
static const gSkladisteTipSansa[] = { 50, 20, 15, 15 };
static const gSkladisteTipIme[][] = { "Obicno", "Lomljivo", "Tesko", "Hitno" };
static const Float:gSkladisteKamionTeret[SK_KAMION_MJESTA][2] = { { -0.45, -2.4 }, { 0.45, -2.4 }, { -0.45, -1.6 }, { 0.45, -1.6 } };
static const Float:gSkladisteRadnik[][3] = { { 0.0, -9.0, 0.0 }, { 0.0, -0.3, -38.1 }, { 0.0, 6.5, 38.1 }, { 0.0, -9.0, -26.0 }, { 1.0, -0.3, 0.0 }, { 1.0, -9.0, 0.0 } };
static const Float:gSkladisteRedA[] = { -50.0, -38.1, -25.1, -13.0, 0.0, 13.0, 25.1, 38.1, 50.0 };
static const Float:gSkladisteRedB[] = { -12.0, 0.0, 12.0 };
static const Float:gSkladisteStupac[] = { -9.0, -0.3, 6.5 };
static const gSkladisteRadnikSkin[] = { 27, 260, 16, 27, 260, 16 };
static const gSkladisteRadnikAnim[][2][24] = { { "COP_AMBIENT", "Coplook_loop" }, { "SMOKING", "M_smklean_loop" }, { "PED", "IDLE_chat" }, { "COP_AMBIENT", "Coplook_think" }, { "PED", "IDLE_chat" }, { "SMOKING", "M_smklean_loop" } };

enum E_SK_VILJ { bool:svIma, svDb, svVozilo, svIgrac, svZadnje, svLik, svCvor, Float:svX, Float:svY, Float:svZ, svTeret }
static gSkladisteVilj[SK_VILJUSKARA][E_SK_VILJ];
enum E_SK_RAMPA { bool:srIma, srDb, Float:srX, Float:srY, Float:srZ, Float:srA, STREAMER_TAG_3D_TEXT_LABEL:srTekst, srStanje, srVozilo, srLik, srUtovareno, srRezerva, srVrijeme }
static gSkladisteRampa[SK_RAMPI][E_SK_RAMPA];
static STREAMER_TAG_OBJECT:gSkladisteRampaTeret[SK_RAMPI][4];
enum E_SK_PALETA { bool:spIma, spDb, Float:spX, Float:spY, Float:spZ, Float:spA, STREAMER_TAG_OBJECT:spObjekt, STREAMER_TAG_OBJECT:spTeret, spRezerva, STREAMER_TAG_3D_TEXT_LABEL:spTekst }
static gSkladistePaleta[SK_PALETA][E_SK_PALETA];
static Float:gSkladistePolica[SK_POLICA][4];
static STREAMER_TAG_OBJECT:gSkladisteZaliha[SK_SLOTOVA], gSkladisteRezerva[SK_SLOTOVA], gSkladisteSljedeci;
static Float:gSkladisteMjestoPosla[3], gSkladisteIkona, STREAMER_TAG_3D_TEXT_LABEL:gSkladisteTekst, bool:gSkladisteImaMjesto;
static gSkladisteFaza[MAX_PLAYERS], gSkladisteOd[MAX_PLAYERS], gSkladisteDo[MAX_PLAYERS], gSkladisteTip[MAX_PLAYERS], gSkladisteRok[MAX_PLAYERS], gSkladisteNiz[MAX_PLAYERS], bool:gSkladisteCeka[MAX_PLAYERS];
static gSkladisteSync[MAX_PLAYERS];
static Float:gSkladisteCilj[MAX_PLAYERS][3], Float:gSkladisteZdravlje[MAX_PLAYERS], Float:gSkladisteVisina[MAX_PLAYERS];
static STREAMER_TAG_OBJECT:gSkladisteStrelica[MAX_PLAYERS];
static PlayerText:gSkladisteHud[MAX_PLAYERS], bool:gSkladisteHudIma[MAX_PLAYERS], gSkladisteKontrola[MAX_PLAYERS];
static gSkladisteApPopis[MAX_PLAYERS][SK_VILJUSKARA + SK_RAMPI + SK_PALETA];

InitSkladiste()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_warehouse (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_forklifts (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_ramps (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS warehouse_pallets (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
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
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM warehouse_pallets ORDER BY id LIMIT %d", SK_PALETA);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
        SkladistePaletaStvori(k, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
    for (new p = 0; p < SK_POLICA; p++)
    {
        if (p < SK_POLICA_A)
        {
            for (new k = 0; k < 3; k++) gSkladistePolica[p][k] = gSkladistePolicaA[p][k];
            gSkladistePolica[p][3] = 0.0;
        }
        else
        {
            new q = gSkladisteKopija[p - SK_POLICA_A];
            SkladisteUHangar(1, gSkladistePolicaA[q][0] - gSkladisteHangar[0][0], gSkladistePolicaA[q][1] - gSkladisteHangar[0][1], gSkladistePolica[p][0], gSkladistePolica[p][1]);
            gSkladistePolica[p][2] = gSkladistePolicaA[q][2];
            gSkladistePolica[p][3] = SK_KUT_B;
            CreateDynamicObject(SK_POLICA_MODEL, gSkladistePolica[p][0], gSkladistePolica[p][1], gSkladistePolica[p][2], 0.0, 0.0, SK_KUT_B, 0, 0);
        }
        new oznaka[8];
        format(oznaka, sizeof oznaka, "%c%d", p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p));
        CreateDynamic3DTextLabel(oznaka, SK_BOJA, gSkladistePolica[p][0], gSkladistePolica[p][1], gSkladistePolica[p][2] + 2.4, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    }
    for (new k = 0; k < SK_SLOTOVA; k++) if (random(100) < SK_ZALIHA_POSTO) SkladisteZalihaStvori(k);
    for (new k = 0; k < sizeof gSkladisteRadnik; k++)
    {
        new h = floatround(gSkladisteRadnik[k][0]), Float:x, Float:y;
        SkladisteUHangar(h, gSkladisteRadnik[k][1], gSkladisteRadnik[k][2], x, y);
        new lik = Lik_Stvori(gSkladisteRadnikSkin[k], x, y, 13.6, float(random(360)));
        if (lik == INVALID_LIK) continue;
        Lik_Neranjiv(lik, true);
        Lik_Animacija(lik, gSkladisteRadnikAnim[k][0], gSkladisteRadnikAnim[k][1], 4.1, 1);
    }
    Lik_Zona(SK_LIK_ZONA, 2360.0, -2620.0, 2840.0, -2370.0, 0.0);
}

static SkladisteUHangar(h, Float:lx, Float:ly, &Float:x, &Float:y)
{
    new Float:c = floatcos(gSkladisteKutHangara[h], degrees), Float:s = floatsin(gSkladisteKutHangara[h], degrees);
    x = gSkladisteHangar[h][0] + lx * c - ly * s;
    y = gSkladisteHangar[h][1] + lx * s + ly * c;
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

static SkladisteTeretSakrij(b)
{
    AttachObjectToVehicle(gSkladisteVilj[b][svTeret], gSkladisteVilj[b][svVozilo], 0.0, 0.0, -30.0, 0.0, 0.0, 0.0);
}

static SkladisteViljStvori(b, db, Float:x, Float:y, Float:z, Float:a)
{
    gSkladisteVilj[b][svIma] = true;
    gSkladisteVilj[b][svDb] = db;
    gSkladisteVilj[b][svX] = x;
    gSkladisteVilj[b][svY] = y;
    gSkladisteVilj[b][svZ] = z;
    gSkladisteVilj[b][svIgrac] = INVALID_PLAYER_ID;
    gSkladisteVilj[b][svLik] = INVALID_LIK;
    gSkladisteVilj[b][svVozilo] = CreateVehicle(SK_MODEL, x, y, z, a, 1, 1, -1);
    gSkladisteVilj[b][svTeret] = CreateObject(SK_TERET_MODEL, x, y, z - 30.0, 0.0, 0.0, 0.0);
    SkladisteTeretSakrij(b);
    VoziloSet(gSkladisteVilj[b][svVozilo], VP_MOTOR, 1);
    SetTimerEx("SkladisteViljNpc", 2000, false, "i", b);
}

forward SkladisteViljNpc(b);
public SkladisteViljNpc(b)
{
    if (!gSkladisteVilj[b][svIma] || gSkladisteVilj[b][svIgrac] != INVALID_PLAYER_ID || gSkladisteVilj[b][svLik] != INVALID_LIK) return;
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(gSkladisteVilj[b][svVozilo], x, y, z);
    GetVehicleZAngle(gSkladisteVilj[b][svVozilo], a);
    new lik = Lik_Stvori(SK_NPC_SKIN, x, y, z, a);
    if (lik == INVALID_LIK) return;
    Lik_Neranjiv(lik, true);
    VoziloSet(gSkladisteVilj[b][svVozilo], VP_MOTOR, 1);
    Lik_UVozilo(lik, gSkladisteVilj[b][svVozilo], 0);
    gSkladisteVilj[b][svLik] = lik;
    gSkladisteVilj[b][svCvor] = -1;
    SetTimerEx("SkladisteViljKreni", 1500, false, "ii", b, lik);
}

static SkladisteRedova(h)
{
    return h == 0 ? sizeof gSkladisteRedA : sizeof gSkladisteRedB;
}

static SkladisteCvorPoz(h, c, &Float:x, &Float:y)
{
    SkladisteUHangar(h, gSkladisteStupac[c % sizeof gSkladisteStupac], h == 0 ? gSkladisteRedA[c / sizeof gSkladisteStupac] : gSkladisteRedB[c / sizeof gSkladisteStupac], x, y);
}

static SkladisteCvorSljedeci(h, c, Float:x, Float:y)
{
    new n = SkladisteRedova(h) * sizeof gSkladisteStupac;
    if (c < 0 || c >= n)
    {
        new najblizi = 0, Float:najmanje = 1.0e9;
        for (new k = 0; k < n; k++)
        {
            new Float:cx, Float:cy;
            SkladisteCvorPoz(h, k, cx, cy);
            new Float:d = VectorSize(cx - x, cy - y, 0.0);
            if (d < najmanje) najmanje = d, najblizi = k;
        }
        return najblizi;
    }
    new stupac = c % sizeof gSkladisteStupac, red = c / sizeof gSkladisteStupac, opcije[4], m = 0;
    if (stupac > 0) opcije[m++] = c - 1;
    if (stupac < sizeof gSkladisteStupac - 1) opcije[m++] = c + 1;
    if (red > 0) opcije[m++] = c - sizeof gSkladisteStupac;
    if (red < SkladisteRedova(h) - 1) opcije[m++] = c + sizeof gSkladisteStupac;
    return opcije[random(m)];
}

forward SkladisteViljKreni(b, lik);
public SkladisteViljKreni(b, lik)
{
    if (!gSkladisteVilj[b][svIma] || gSkladisteVilj[b][svLik] != lik || gSkladisteVilj[b][svIgrac] != INVALID_PLAYER_ID) return;
    new Float:x, Float:y, Float:z, Float:tx, Float:ty;
    GetVehiclePos(gSkladisteVilj[b][svVozilo], x, y, z);
    new h = SkladisteHangarBlizu(gSkladisteVilj[b][svX], gSkladisteVilj[b][svY]), c = SkladisteCvorSljedeci(h, gSkladisteVilj[b][svCvor], x, y);
    SkladisteCvorPoz(h, c, tx, ty);
    if (!Lik_Vozi(lik, tx, ty, gSkladisteVilj[b][svZ] - 0.6, SK_NPC_BRZINA))
    {
        SetTimerEx("SkladisteViljKreni", 3000, false, "ii", b, lik);
        return;
    }
    gSkladisteVilj[b][svCvor] = c;
}

static SkladisteViljNpcMakni(b)
{
    if (gSkladisteVilj[b][svLik] != INVALID_LIK) Lik_Ukloni(gSkladisteVilj[b][svLik]);
    gSkladisteVilj[b][svLik] = INVALID_LIK;
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
    gSkladisteRampa[r][srStanje] = SKD_NEMA;
    format(oznaka, sizeof oznaka, "Utovarna rampa\nR%d", r + 1);
    gSkladisteRampa[r][srTekst] = CreateDynamic3DTextLabel(oznaka, SK_BOJA, x, y, z + 1.0, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

static SkladistePaletaStvori(k, db, Float:x, Float:y, Float:z, Float:a)
{
    new oznaka[16];
    gSkladistePaleta[k][spIma] = true;
    gSkladistePaleta[k][spDb] = db;
    gSkladistePaleta[k][spX] = x;
    gSkladistePaleta[k][spY] = y;
    gSkladistePaleta[k][spZ] = z - 1.0;
    gSkladistePaleta[k][spA] = a;
    gSkladistePaleta[k][spRezerva] = 0;
    gSkladistePaleta[k][spTeret] = STREAMER_TAG_OBJECT:0;
    new Float:c = floatcos(a, degrees), Float:s = floatsin(a, degrees);
    gSkladistePaleta[k][spObjekt] = CreateDynamicObject(SK_PALETA_MODEL, x - (SK_PALETA_X * c - SK_PALETA_Y * s), y - (SK_PALETA_X * s + SK_PALETA_Y * c), z - 1.0 - SK_PALETA_SPUSTI, 0.0, 0.0, a, 0, 0);
    SetDynamicObjectMaterial(gSkladistePaleta[k][spObjekt], 0, 19279, "matlights", "invisible", 0);
    SetDynamicObjectMaterial(gSkladistePaleta[k][spObjekt], 2, 19279, "matlights", "invisible", 0);
    format(oznaka, sizeof oznaka, "Paleta P%d", k + 1);
    gSkladistePaleta[k][spTekst] = CreateDynamic3DTextLabel(oznaka, SK_BOJA, x, y, z + 0.8, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
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

static bool:SkladisteVozi(playerid)
{
    new b = SkladisteViljIgraca(playerid);
    return b != -1 && GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) == gSkladisteVilj[b][svVozilo];
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

static SkladisteVrsta(cilj)
{
    return cilj >= SK_CILJ_RAMPA ? SKC_RAMPA : cilj >= SK_CILJ_PALETA ? SKC_PALETA : SKC_SLOT;
}

static bool:SkladisteCiljPostoji(cilj)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA: return gSkladistePaleta[cilj - SK_CILJ_PALETA][spIma];
        case SKC_RAMPA: return gSkladisteRampa[cilj - SK_CILJ_RAMPA][srIma] && gSkladisteRampa[cilj - SK_CILJ_RAMPA][srStanje] == SKD_CEKA;
    }
    return true;
}

static SkladisteCiljHangar(cilj)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA: return SkladisteHangarBlizu(gSkladistePaleta[cilj - SK_CILJ_PALETA][spX], gSkladistePaleta[cilj - SK_CILJ_PALETA][spY]);
        case SKC_RAMPA: return SkladisteHangarBlizu(gSkladisteRampa[cilj - SK_CILJ_RAMPA][srX], gSkladisteRampa[cilj - SK_CILJ_RAMPA][srY]);
    }
    return cilj / sizeof gSkladisteMjesto < SK_POLICA_A ? 0 : 1;
}

static SkladisteVoziloSmjer(v, &Float:x, &Float:y, &Float:z, &Float:fx, &Float:fy)
{
    new Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    fx = -floatsin(a, degrees);
    fy = floatcos(a, degrees);
}

static SkladisteTeretPoz(cilj, &Float:x, &Float:y, &Float:z, &Float:a)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA:
        {
            new k = cilj - SK_CILJ_PALETA;
            x = gSkladistePaleta[k][spX];
            y = gSkladistePaleta[k][spY];
            z = gSkladistePaleta[k][spZ] + SK_PALETA_VISINA;
            a = gSkladistePaleta[k][spA];
            return;
        }
        case SKC_RAMPA:
        {
            new r = cilj - SK_CILJ_RAMPA, Float:fx, Float:fy;
            SkladisteVoziloSmjer(gSkladisteRampa[r][srVozilo], x, y, z, fx, fy);
            x += fx * SK_KAMION_STRAG;
            y += fy * SK_KAMION_STRAG;
            a = 0.0;
            return;
        }
    }
    new p = cilj / sizeof gSkladisteMjesto, m = cilj % sizeof gSkladisteMjesto;
    SkladisteLokalno(p, 0.0, gSkladisteMjesto[m][0], x, y);
    z = gSkladistePolica[p][2] + gSkladisteMjesto[m][1];
    a = gSkladistePolica[p][3];
}

static STREAMER_TAG_OBJECT:SkladisteTeretStvori(cilj)
{
    new Float:x, Float:y, Float:z, Float:a;
    SkladisteTeretPoz(cilj, x, y, z, a);
    return CreateDynamicObject(SK_TERET_MODEL, x, y, z, 0.0, 0.0, a, 0, 0);
}

static SkladisteObjektMakni(&STREAMER_TAG_OBJECT:o)
{
    if (_:o) DestroyDynamicObject(o);
    o = STREAMER_TAG_OBJECT:0;
}

static SkladisteZalihaStvori(k)
{
    if (!_:gSkladisteZaliha[k]) gSkladisteZaliha[k] = SkladisteTeretStvori(k);
}

static SkladisteUzmiPuno(h)
{
    new popis[SK_SLOTOVA], n = 0;
    for (new k = 0; k < SK_SLOTOVA; k++) if (_:gSkladisteZaliha[k] && !gSkladisteRezerva[k] && SkladisteCiljHangar(k) == h) popis[n++] = k;
    if (n) return popis[random(n)];
    for (new k = 0; k < SK_SLOTOVA; k++) if (!_:gSkladisteZaliha[k] && !gSkladisteRezerva[k] && SkladisteCiljHangar(k) == h) popis[n++] = k;
    if (!n) return -1;
    new k = popis[random(n)];
    SkladisteZalihaStvori(k);
    return k;
}

static SkladistePaletaNadji(h, bool:puna)
{
    new popis[SK_PALETA], n = 0;
    for (new k = 0; k < SK_PALETA; k++)
        if (gSkladistePaleta[k][spIma] && !gSkladistePaleta[k][spRezerva] && (_:gSkladistePaleta[k][spTeret] != 0) == puna && (h == -1 || SkladisteCiljHangar(SK_CILJ_PALETA + k) == h)) popis[n++] = k;
    return n ? SK_CILJ_PALETA + popis[random(n)] : -1;
}

static SkladisteRampaZaUtovar(h)
{
    new najbolja = -1;
    for (new r = 0; r < SK_RAMPI; r++)
    {
        if (!gSkladisteRampa[r][srIma] || gSkladisteRampa[r][srStanje] != SKD_CEKA) continue;
        if (gSkladisteRampa[r][srUtovareno] + gSkladisteRampa[r][srRezerva] >= SK_KAMION_MJESTA) continue;
        if (najbolja == -1 || SkladisteCiljHangar(SK_CILJ_RAMPA + r) == h) najbolja = r;
    }
    return najbolja == -1 ? -1 : SK_CILJ_RAMPA + najbolja;
}

static SkladisteRezerviraj(cilj, bool:uzmi, playerid)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_SLOT: gSkladisteRezerva[cilj] = uzmi ? playerid + 1 : 0;
        case SKC_PALETA: gSkladistePaleta[cilj - SK_CILJ_PALETA][spRezerva] = uzmi ? playerid + 1 : 0;
        case SKC_RAMPA:
        {
            new r = cilj - SK_CILJ_RAMPA;
            gSkladisteRampa[r][srRezerva] = max(0, gSkladisteRampa[r][srRezerva] + (uzmi ? 1 : -1));
        }
    }
}

static SkladisteRampaPrilaz(cilj, &Float:x, &Float:y, &Float:z)
{
    new Float:vx, Float:vy, Float:fx, Float:fy;
    SkladisteVoziloSmjer(gSkladisteRampa[cilj - SK_CILJ_RAMPA][srVozilo], vx, vy, z, fx, fy);
    x = vx + fx * (SK_KAMION_STRAG - 2.5);
    y = vy + fy * (SK_KAMION_STRAG - 2.5);
    z -= 1.0;
}

static SkladisteRampaPrati(playerid)
{
    if (gSkladisteFaza[playerid] != 2 || SkladisteVrsta(gSkladisteDo[playerid]) != SKC_RAMPA || !SkladisteCiljPostoji(gSkladisteDo[playerid])) return;
    new Float:x, Float:y, Float:z;
    SkladisteRampaPrilaz(gSkladisteDo[playerid], x, y, z);
    if (VectorSize(x - gSkladisteCilj[playerid][0], y - gSkladisteCilj[playerid][1], z - gSkladisteCilj[playerid][2]) < 1.0) return;
    gSkladisteCilj[playerid][0] = x;
    gSkladisteCilj[playerid][1] = y;
    gSkladisteCilj[playerid][2] = z;
    SetPlayerCheckpoint(playerid, x, y, z, 1.2);
}

static SkladisteCiljPostavi(playerid, cilj)
{
    new Float:x, Float:y, Float:z, Float:a;
    SkladisteTeretPoz(cilj, x, y, z, a);
    SkladisteObjektMakni(gSkladisteStrelica[playerid]);
    gSkladisteStrelica[playerid] = CreateDynamicObject(SK_STRELICA_MODEL, x, y, z + 1.4, 0.0, 0.0, 0.0, 0, 0, playerid);
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA:
        {
            gSkladisteCilj[playerid][0] = x;
            gSkladisteCilj[playerid][1] = y;
            gSkladisteCilj[playerid][2] = gSkladistePaleta[cilj - SK_CILJ_PALETA][spZ];
        }
        case SKC_RAMPA:
        {
            AttachDynamicObjectToVehicle(gSkladisteStrelica[playerid], gSkladisteRampa[cilj - SK_CILJ_RAMPA][srVozilo], 0.0, SK_KAMION_STRAG, 1.4, 0.0, 0.0, 0.0);
            SkladisteRampaPrilaz(cilj, gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1], gSkladisteCilj[playerid][2]);
        }
        default:
        {
            new p = cilj / sizeof gSkladisteMjesto;
            SkladisteLokalno(p, random(2) ? SK_PRILAZ : -SK_PRILAZ, gSkladisteMjesto[cilj % sizeof gSkladisteMjesto][0], gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1]);
            gSkladisteCilj[playerid][2] = gSkladistePolica[p][2] - 1.9;
        }
    }
    SetPlayerCheckpoint(playerid, gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1], gSkladisteCilj[playerid][2], 1.2);
}

static SkladisteOznaka(cilj, oznaka[], len = sizeof oznaka)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA: return format(oznaka, len, "P%d", cilj - SK_CILJ_PALETA + 1);
        case SKC_RAMPA:
        {
            new r = cilj - SK_CILJ_RAMPA;
            return format(oznaka, len, "R%d (%d/%d)", r + 1, gSkladisteRampa[r][srUtovareno], SK_KAMION_MJESTA);
        }
    }
    new p = cilj / sizeof gSkladisteMjesto;
    return format(oznaka, len, "%c%d", p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p));
}

static SkladisteOpis(playerid, bool:preuzmi, cilj, poruka[], len = sizeof poruka)
{
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA: return format(poruka, len, preuzmi ? T(playerid, "Preuzmite teret s palete P%d.") : T(playerid, "Odlozite teret na paletu P%d."), cilj - SK_CILJ_PALETA + 1);
        case SKC_RAMPA: return format(poruka, len, T(playerid, "Utovarite teret u kamion na rampi R%d: dovezite ga do straznjih vrata kamiona."), cilj - SK_CILJ_RAMPA + 1);
    }
    new p = cilj / sizeof gSkladisteMjesto;
    return format(poruka, len, T(playerid, "Preuzmite teret sa strelice: skladiste %c, polica %d."), p < SK_POLICA_A ? 'A' : 'B', SkladisteBroj(p));
}

static SkladisteHudOsvjezi(playerid)
{
    new tekst[192], oznaka[24], ostalo = max(0, gSkladisteRok[playerid] - gettime()), cilj = gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid];
    SkladisteOznaka(cilj, oznaka);
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

static bool:SkladistePrijenos(playerid)
{
    return SkladisteCiljHangar(gSkladisteOd[playerid]) != SkladisteCiljHangar(gSkladisteDo[playerid]) && SkladisteVrsta(gSkladisteDo[playerid]) == SKC_PALETA;
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

static SkladisteOtpusti(playerid)
{
    if (gSkladisteFaza[playerid])
    {
        DisablePlayerCheckpoint(playerid);
        if (gSkladisteFaza[playerid] == 1) SkladisteRezerviraj(gSkladisteOd[playerid], false, playerid);
        SkladisteRezerviraj(gSkladisteDo[playerid], false, playerid);
        new b = SkladisteViljIgraca(playerid);
        if (gSkladisteFaza[playerid] == 2 && b != -1) SkladisteTeretSakrij(b);
    }
    gSkladisteFaza[playerid] = 0;
    SkladisteObjektMakni(gSkladisteStrelica[playerid]);
}

static SkladisteZadatak(playerid)
{
    SkladisteOtpusti(playerid);
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new h = SkladisteHangarBlizu(x, y), od = -1, doo = SkladisteRampaZaUtovar(h);
    if (doo != -1)
    {
        new hr = SkladisteCiljHangar(doo);
        od = SkladistePaletaNadji(hr, true);
        if (od == -1) od = SkladisteUzmiPuno(hr);
    }
    else
    {
        new dh = random(100) < 25 ? 1 - h : h;
        doo = SkladistePaletaNadji(dh, false);
        if (doo == -1) doo = SkladistePaletaNadji(-1, false);
        if (doo != -1) od = SkladisteUzmiPuno(h);
    }
    if (od == -1 || doo == -1)
    {
        SkladisteHudMakni(playerid);
        if (!gSkladisteCeka[playerid]) ObavijestInfo(playerid, T(playerid, "Skladistar"), T(playerid, "Trenutno nema posla. Pricekajte vozilo za utovar ili slobodnu paletu."));
        gSkladisteCeka[playerid] = true;
        return;
    }
    gSkladisteCeka[playerid] = false;
    SkladisteRezerviraj(od, true, playerid);
    SkladisteRezerviraj(doo, true, playerid);
    gSkladisteOd[playerid] = od;
    gSkladisteDo[playerid] = doo;
    gSkladisteTip[playerid] = SkladisteTipNasumicno();
    gSkladisteRok[playerid] = gettime() + (gSkladisteTip[playerid] == SKT_HITNO ? SK_ROK_HITNO : SK_ROK) + (SkladistePrijenos(playerid) ? SK_ROK_PRIJENOS : 0);
    gSkladisteFaza[playerid] = 1;
    SkladisteCiljPostavi(playerid, od);
    SkladisteHudOsvjezi(playerid);
    new poruka[144];
    SkladisteOpis(playerid, true, od, poruka);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
    switch (gSkladisteTip[playerid])
    {
        case SKT_LOMLJIVO: Savjet(playerid, T(playerid, "~r~Lomljivo!~w~ Ne udarajte viljuskarom dok nosite teret."), 5000);
        case SKT_TESKO: Savjet(playerid, T(playerid, "~y~Tesko!~w~ Viljuske se s teretom sporije dizu."), 5000);
        case SKT_HITNO: Savjet(playerid, T(playerid, "~y~Hitno!~w~ Kraci rok, veca zarada."), 5000);
    }
}

static SkladisteZadatakMakni(playerid)
{
    SkladisteOtpusti(playerid);
    SkladisteHudMakni(playerid);
    gSkladisteCeka[playerid] = false;
    if (gSkladisteKontrola[playerid]) KillTimer(gSkladisteKontrola[playerid]);
    gSkladisteKontrola[playerid] = 0;
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
    SkladisteTeretSakrij(b);
    SetVehicleToRespawn(gSkladisteVilj[b][svVozilo]);
    SetTimerEx("SkladisteViljNpc", 2000, false, "i", b);
}

SkladisteLokacija(&Float:x, &Float:y, &Float:z)
{
    x = gSkladisteMjestoPosla[0];
    y = gSkladisteMjestoPosla[1];
    z = gSkladisteMjestoPosla[2];
    return 1;
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
    if (gSkladisteVilj[b][svIgrac] != playerid)
    {
        Fade(playerid, FADE_SLOJEVA + 1);
        return;
    }
    new Float:x, Float:y, Float:z;
    SkladisteViljNpcMakni(b);
    GetVehiclePos(gSkladisteVilj[b][svVozilo], x, y, z);
    SetPlayerPos(playerid, x + 1.5, y, z + 0.5);
    SetPlayerSkin(playerid, SK_SKIN);
    SetTimerEx("SkladisteViljSjedni", 700, false, "iii", playerid, racun, b);
}

forward SkladisteViljSjedni(playerid, racun, b);
public SkladisteViljSjedni(playerid, racun, b)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    Fade(playerid, FADE_SLOJEVA + 1);
    if (gSkladisteVilj[b][svIgrac] != playerid) return;
    gSkladisteVilj[b][svZadnje] = gettime();
    gSkladisteNiz[playerid] = 0;
    gSkladisteVisina[playerid] = 0.0;
    PutPlayerInVehicle(playerid, gSkladisteVilj[b][svVozilo], 0);
    VoziloSet(gSkladisteVilj[b][svVozilo], VP_MOTOR, 1);
    gSkladisteSljedeci = 0;
    if (gSkladisteKontrola[playerid]) KillTimer(gSkladisteKontrola[playerid]);
    gSkladisteKontrola[playerid] = SetTimerEx("SkladisteKontrola", SK_VILJ_TIK_MS, true, "i", playerid);
    SkladisteZadatak(playerid);
    Savjet(playerid, T(playerid, "~y~~k~~VEHICLE_TURRETUP~/~k~~VEHICLE_TURRETDOWN~~w~ - viljuske gore/dolje~n~Spustene viljuske uvucite pod teret i podignite~n~Na cilju ih spustite do kraja~n~~y~/posao~w~ - vrati viljuskar"), 8000);
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
    if (GetPlayerVehicleID(playerid) == gSkladisteVilj[b][svVozilo]) RemovePlayerFromVehicle(playerid);
    SkladisteViljOslobodi(b);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), T(playerid, "Smjena je zavrsena. Zaradu dobivate na placi."));
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
    if (SkladisteViljIgraca(playerid) != -1) return SkladisteVrati(playerid);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && SkladisteKodPosla(playerid)) return SkladisteIzbornik(playerid), 1;
    if (PosaoIgraca(playerid) != POSAO_SKLADISTAR) return 0;
    return ObavijestGreska(playerid, T(playerid, "Viljuskar uzimate na mjestu za posao u luci (oznaceno na karti).")), 1;
}

SkladisteAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || !SkladisteKodPosla(playerid)) return 0;
    return SkladisteIzbornik(playerid), 1;
}

SkladisteCheckpoint(playerid)
{
    if (!gSkladisteFaza[playerid] || !IsPlayerInRangeOfPoint(playerid, SK_CILJ_DOMET, gSkladisteCilj[playerid][0], gSkladisteCilj[playerid][1], gSkladisteCilj[playerid][2])) return 0;
    if (!SkladisteVozi(playerid)) ObavijestGreska(playerid, T(playerid, "Teret preuzimate i odlazete svojim viljuskarom."));
    return 1;
}

static bool:SkladistePoravnat(playerid, cilj)
{
    new Float:x, Float:y, Float:z, Float:fx, Float:fy, Float:tx, Float:ty, Float:tz, Float:ta;
    SkladisteVoziloSmjer(GetPlayerVehicleID(playerid), x, y, z, fx, fy);
    x += fx * SK_VILJ_NAPRIJED;
    y += fy * SK_VILJ_NAPRIJED;
    SkladisteTeretPoz(cilj, tx, ty, tz, ta);
    switch (SkladisteVrsta(cilj))
    {
        case SKC_PALETA: return VectorSize(x - tx, y - ty, 0.0) < SK_PALETA_DOMET;
        case SKC_RAMPA:
        {
            new Float:vx, Float:vy, Float:vz, Float:vfx, Float:vfy;
            SkladisteVoziloSmjer(gSkladisteRampa[cilj - SK_CILJ_RAMPA][srVozilo], vx, vy, vz, vfx, vfy);
            x += fx * (SK_VILJ_VRH - SK_VILJ_NAPRIJED);
            y += fy * (SK_VILJ_VRH - SK_VILJ_NAPRIJED);
            return VectorSize(x - tx, y - ty, 0.0) < 1.6 && fx * vfx + fy * vfy > 0.7;
        }
    }
    new p = cilj / sizeof gSkladisteMjesto, Float:c = floatcos(gSkladistePolica[p][3], degrees), Float:s = floatsin(gSkladistePolica[p][3], degrees);
    new Float:dx = x - gSkladistePolica[p][0], Float:dy = y - gSkladistePolica[p][1];
    new Float:lx = dx * c + dy * s, Float:ly = -dx * s + dy * c, Float:smx = fx * c + fy * s, Float:smy = -fx * s + fy * c;
    return floatabs(ly - gSkladisteMjesto[cilj % sizeof gSkladisteMjesto][0]) < 0.4 && floatabs(lx) > 0.9 && floatabs(lx) < 2.0 && floatabs(smy) < 0.26 && smx * lx < 0.0;
}

static SkladisteIznos(playerid)
{
    new iznos = gSkladisteTipCijena[gSkladisteTip[playerid]];
    if (SkladistePrijenos(playerid)) iznos += SK_PRIJENOS;
    if (SkladisteVrsta(gSkladisteDo[playerid]) == SKC_RAMPA) iznos += SK_RAMPA_BONUS;
    iznos += iznos * min(gSkladisteNiz[playerid], SK_NIZ_MAX) * 10 / 100;
    return iznos;
}

SkladisteSyncVilice(playerid, vozilo, sirovo)
{
    if (GetVehicleModel(vozilo) != SK_MODEL) return;
    gSkladisteSync[playerid] = sirovo & 0xFFFF;
}

static SkladisteTeretNaVilj(playerid)
{
    new b = SkladisteViljIgraca(playerid);
    if (b != -1) AttachObjectToVehicle(gSkladisteVilj[b][svTeret], gSkladisteVilj[b][svVozilo], 0.0, SK_TERET_Y, SK_TERET_Z + gSkladisteVisina[playerid], 0.0, 0.0, 0.0);
}

static SkladistePodigni(playerid)
{
    new od = gSkladisteOd[playerid], v = GetPlayerVehicleID(playerid), poruka[144];
    if (SkladisteVrsta(od) == SKC_PALETA) SkladisteObjektMakni(gSkladistePaleta[od - SK_CILJ_PALETA][spTeret]);
    else SkladisteObjektMakni(gSkladisteZaliha[od]);
    SkladisteRezerviraj(od, false, playerid);
    SkladisteTeretNaVilj(playerid);
    GetVehicleHealth(v, gSkladisteZdravlje[playerid]);
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    gSkladisteFaza[playerid] = 2;
    SkladisteCiljPostavi(playerid, gSkladisteDo[playerid]);
    SkladisteHudOsvjezi(playerid);
    SkladisteOpis(playerid, false, gSkladisteDo[playerid], poruka);
    ObavijestInfo(playerid, T(playerid, "Skladistar"), poruka);
}

static SkladisteSpusti(playerid)
{
    new doo = gSkladisteDo[playerid], poruka[144], iznos = SkladisteIznos(playerid);
    new b = SkladisteViljIgraca(playerid);
    if (b != -1) SkladisteTeretSakrij(b);
    SkladisteRezerviraj(doo, false, playerid);
    if (SkladisteVrsta(doo) == SKC_PALETA) gSkladistePaleta[doo - SK_CILJ_PALETA][spTeret] = SkladisteTeretStvori(doo);
    else SkladisteUtovari(doo - SK_CILJ_RAMPA);
    PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
    gSkladisteNiz[playerid]++;
    format(poruka, sizeof poruka, T(playerid, "Teret je dostavljen. +$%d na placi (niz x%d)."), iznos, gSkladisteNiz[playerid]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = payday_earn + %d WHERE id = %d", iznos, gAccountId[playerid]));
    IgraObavijest(playerid, T(playerid, "Skladiste"), SK_BOJA, poruka);
    SkladisteZadatak(playerid);
}

static SkladisteRazbij(playerid, bool:rok = false)
{
    new poruka[144], kazna = SkladisteIznos(playerid) / 2;
    gSkladisteNiz[playerid] = 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET payday_earn = MAX(0, payday_earn - %d) WHERE id = %d", kazna, gAccountId[playerid]));
    format(poruka, sizeof poruka, rok ? T(playerid, "Rok je istekao, narudzba nije izvrsena. -$%d s place, niz je prekinut.") : T(playerid, "Ostetili ste teret. -$%d s place, niz je prekinut."), kazna);
    IgraObavijest(playerid, T(playerid, "Skladiste"), OBAVIJEST_GRESKA, poruka);
    SkladisteZadatak(playerid);
}

forward SkladisteKontrola(playerid);
public SkladisteKontrola(playerid)
{
    if (!gSkladisteFaza[playerid] || !SkladisteVozi(playerid)) return;
    new cilj = gSkladisteFaza[playerid] == 1 ? gSkladisteOd[playerid] : gSkladisteDo[playerid];
    if (!SkladisteCiljPostoji(cilj))
    {
        SkladisteZadatak(playerid);
        return;
    }
    if (gSkladisteFaza[playerid] == 2 && SkladisteVrsta(cilj) == SKC_RAMPA)
    {
        if (SkladistePoravnat(playerid, cilj)) SkladisteSpusti(playerid);
        return;
    }
    new Float:h = float(min(gSkladisteSync[playerid], SK_VILJ_JEDINICA)) * SK_VILJ_HOD / float(SK_VILJ_JEDINICA);
    new smjer = h > gSkladisteVisina[playerid] + 0.005 ? 1 : h < gSkladisteVisina[playerid] - 0.005 ? -1 : 0;
    if (!smjer) return;
    gSkladisteVisina[playerid] = h;
    if (gSkladisteFaza[playerid] == 2) SkladisteTeretNaVilj(playerid);
    if (gSkladisteFaza[playerid] == 1 && smjer > 0 && h >= SK_VILJ_PODIGNI && h <= SK_VILJ_UZMI && SkladistePoravnat(playerid, cilj)) SkladistePodigni(playerid);
    else if (gSkladisteFaza[playerid] == 2 && smjer < 0 && SkladisteVrsta(cilj) == SKC_PALETA && h <= SK_VILJ_PALETA && SkladistePoravnat(playerid, cilj)) SkladisteSpusti(playerid);
}

static SkladisteUtovari(r)
{
    new n = gSkladisteRampa[r][srUtovareno];
    if (n >= SK_KAMION_MJESTA) return;
    new STREAMER_TAG_OBJECT:o = CreateDynamicObject(SK_TERET_MODEL, 0.0, 0.0, -50.0, 0.0, 0.0, 0.0, 0, 0);
    AttachDynamicObjectToVehicle(o, gSkladisteRampa[r][srVozilo], gSkladisteKamionTeret[n][0], gSkladisteKamionTeret[n][1], SK_KAMION_POD, 0.0, 0.0, 0.0);
    gSkladisteRampaTeret[r][n] = o;
    if (++gSkladisteRampa[r][srUtovareno] >= SK_KAMION_MJESTA) SkladisteDolazakOdlazi(r);
}

static bool:SkladisteUZoni(Float:x, Float:y)
{
    return x > 2360.0 && x < 2840.0 && y > -2620.0 && y < -2370.0;
}

static SkladisteDolazakStvori(r)
{
    new Float:x, Float:y, Float:z, bool:nasao = false;
    for (new k = 0; k < 10 && !nasao; k++)
    {
        new Float:kut = float(random(360));
        Lik_Cesta(gSkladisteRampa[r][srX] + 280.0 * floatcos(kut, degrees), gSkladisteRampa[r][srY] + 280.0 * floatsin(kut, degrees), gSkladisteRampa[r][srZ], x, y, z);
        nasao = !SkladisteUZoni(x, y) && Lik_Skriveno(x, y, z);
    }
    if (!nasao) return;
    new Float:a = atan2(gSkladisteRampa[r][srX] - x, gSkladisteRampa[r][srY] - y);
    new v = CreateVehicle(SK_KAMION_MODEL, x, y, z + 0.5, -a, -1, -1, -1);
    if (v == INVALID_VEHICLE_ID) return;
    new lik = Lik_Stvori(SK_VOZAC_SKIN, x, y, z, -a);
    if (lik == INVALID_LIK)
    {
        DestroyVehicle(v);
        return;
    }
    Lik_Neranjiv(lik, true);
    VoziloSet(v, VP_MOTOR, 1);
    Lik_UVozilo(lik, v, 0);
    gSkladisteRampa[r][srStanje] = SKD_DOLAZI;
    gSkladisteRampa[r][srVozilo] = v;
    gSkladisteRampa[r][srLik] = lik;
    gSkladisteRampa[r][srUtovareno] = 0;
    gSkladisteRampa[r][srRezerva] = 0;
    gSkladisteRampa[r][srVrijeme] = gettime();
    SetTimerEx("SkladisteDolazakKreni", 1000, false, "ii", r, v);
}

forward SkladisteDolazakKreni(r, v);
public SkladisteDolazakKreni(r, v)
{
    if (gSkladisteRampa[r][srStanje] != SKD_DOLAZI || gSkladisteRampa[r][srVozilo] != v) return;
    if (!Lik_Vozi(gSkladisteRampa[r][srLik], gSkladisteRampa[r][srX], gSkladisteRampa[r][srY], gSkladisteRampa[r][srZ], 10.0)) SkladisteDolazakParkiraj(r);
}

static SkladisteDolazakParkiraj(r)
{
    SetVehiclePos(gSkladisteRampa[r][srVozilo], gSkladisteRampa[r][srX], gSkladisteRampa[r][srY], gSkladisteRampa[r][srZ] + 0.5);
    SetVehicleZAngle(gSkladisteRampa[r][srVozilo], gSkladisteRampa[r][srA]);
    SkladisteDolazakCeka(r);
}

static SkladisteDolazakCeka(r)
{
    gSkladisteRampa[r][srStanje] = SKD_CEKA;
    gSkladisteRampa[r][srVrijeme] = gettime();
    VoziloSet(gSkladisteRampa[r][srVozilo], VP_PRTLJAZNIK, 1);
    SkladisteStraznjaVrata(gSkladisteRampa[r][srVozilo], true);
}

static SkladisteStraznjaVrata(v, bool:otvori)
{
    new VEHICLE_PANEL_STATUS:paneli, VEHICLE_DOOR_STATUS:vrata, VEHICLE_LIGHT_STATUS:svjetla, VEHICLE_TYRE_STATUS:gume;
    GetVehicleDamageStatus(v, paneli, vrata, svjetla, gume);
    vrata = VEHICLE_DOOR_STATUS:((_:vrata & ~0xFF00) | (otvori ? 0x0400 : 0));
    UpdateVehicleDamageStatus(v, paneli, vrata, svjetla, gume);
}

SkladisteStigao(lik)
{
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        if (!gSkladisteVilj[b][svIma] || gSkladisteVilj[b][svLik] != lik) continue;
        SetTimerEx("SkladisteViljKreni", 3000 + random(5000), false, "ii", b, lik);
        return 1;
    }
    for (new r = 0; r < SK_RAMPI; r++)
    {
        if (!gSkladisteRampa[r][srIma] || gSkladisteRampa[r][srStanje] == SKD_NEMA || gSkladisteRampa[r][srLik] != lik) continue;
        if (gSkladisteRampa[r][srStanje] == SKD_DOLAZI) SkladisteDolazakCeka(r);
        return 1;
    }
    return 0;
}

static SkladisteDolazakOdlazi(r)
{
    gSkladisteRampa[r][srStanje] = SKD_ODLAZI;
    gSkladisteRampa[r][srVrijeme] = gettime();
    VoziloSet(gSkladisteRampa[r][srVozilo], VP_PRTLJAZNIK, 0);
    SkladisteStraznjaVrata(gSkladisteRampa[r][srVozilo], false);
    Lik_Kruzi(gSkladisteRampa[r][srLik], 11.0);
}

static SkladisteDolazakMakni(r)
{
    if (gSkladisteRampa[r][srStanje] == SKD_NEMA) return;
    for (new k = 0; k < 4; k++) SkladisteObjektMakni(gSkladisteRampaTeret[r][k]);
    Lik_Ukloni(gSkladisteRampa[r][srLik]);
    DestroyVehicle(gSkladisteRampa[r][srVozilo]);
    gSkladisteRampa[r][srStanje] = SKD_NEMA;
    gSkladisteRampa[r][srRezerva] = 0;
}

static SkladisteDolasciTick(sad, radnika)
{
    new slobodna = -1, dolazi = 0, cekaju = 0;
    for (new r = 0; r < SK_RAMPI; r++)
    {
        if (!gSkladisteRampa[r][srIma]) continue;
        switch (gSkladisteRampa[r][srStanje])
        {
            case SKD_NEMA: if (slobodna == -1 || !random(2)) slobodna = r;
            case SKD_DOLAZI:
            {
                dolazi++;
                if (sad - gSkladisteRampa[r][srVrijeme] > SK_DOLAZI_S) SkladisteDolazakParkiraj(r);
            }
            case SKD_CEKA:
            {
                if (gSkladisteRampa[r][srUtovareno] + gSkladisteRampa[r][srRezerva] < SK_KAMION_MJESTA) cekaju++;
                if (!gSkladisteRampa[r][srRezerva] && sad - gSkladisteRampa[r][srVrijeme] > SK_CEKA_S) SkladisteDolazakOdlazi(r);
            }
            case SKD_ODLAZI: if (sad - gSkladisteRampa[r][srVrijeme] > SK_ODLAZI_S) SkladisteDolazakMakni(r);
        }
    }
    if (!radnika || slobodna == -1 || dolazi || cekaju >= radnika || sad < gSkladisteSljedeci) return;
    gSkladisteSljedeci = sad + SK_DOLAZAK_RAZMAK;
    SkladisteDolazakStvori(slobodna);
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
    new sad = gettime(), radnika = 0;
    for (new b = 0; b < SK_VILJUSKARA; b++)
    {
        if (!gSkladisteVilj[b][svIma]) continue;
        new i = gSkladisteVilj[b][svIgrac];
        if (i == INVALID_PLAYER_ID)
        {
            if (gSkladisteVilj[b][svLik] != INVALID_LIK && !random(3)) Lik_TipkeVozila(gSkladisteVilj[b][svLik], random(3) ? 0 : random(2) ? (_:KEY_ANALOG_UP) : (_:KEY_ANALOG_DOWN));
            continue;
        }
        if (!IsPlayerConnected(i) || PosaoIgraca(i) != POSAO_SKLADISTAR || IsRanjen(i))
        {
            SkladisteKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Smjena je zavrsena. Viljuskar je vracen."));
            continue;
        }
        radnika++;
        if (gSkladisteFaza[i] == 2)
        {
            new Float:hp;
            GetVehicleHealth(gSkladisteVilj[b][svVozilo], hp);
            if (hp < gSkladisteZdravlje[i] - (gSkladisteTip[i] == SKT_LOMLJIVO ? SK_LOMLJIVO_STETA : SK_STETA))
            {
                SkladisteRazbij(i);
                continue;
            }
        }
        if (gSkladisteFaza[i] && sad > gSkladisteRok[i])
        {
            SkladisteRazbij(i, true);
            continue;
        }
        SkladisteRampaPrati(i);
        if (gSkladisteFaza[i] && gSkladisteHudIma[i]) SkladisteHudOsvjezi(i);
        if (!gSkladisteFaza[i] && sad % 5 == 0) SkladisteZadatak(i);
        if (GetPlayerVehicleID(i) == gSkladisteVilj[b][svVozilo])
        {
            gSkladisteVilj[b][svZadnje] = sad;
            continue;
        }
        if (sad - gSkladisteVilj[b][svZadnje] < SK_MIRUJE_S) continue;
        SkladisteViljOslobodi(b);
        ObavijestGreska(i, T(i, "Viljuskar je vracen jer ga 15 minuta niste koristili."));
    }
    SkladisteDolasciTick(sad, radnika);
}

ShowSkladisteAdmin(playerid)
{
    new list[2048], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto zaposlenja skladistara na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Viljuskar na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Utovarna rampa na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Paleta na mojoj poziciji"));
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
    for (new k = 0; k < SK_PALETA; k++)
    {
        if (!gSkladistePaleta[k][spIma]) continue;
        format(row, sizeof row, T(playerid, "Paleta P%d %s - obrisi"), k + 1, CestaZona(gSkladistePaleta[k][spX], gSkladistePaleta[k][spY]));
        DodajStupac(list, len, row);
        gSkladisteApPopis[playerid][n++] = SK_VILJUSKARA + SK_RAMPI + k;
    }
    return PrikaziDialog(playerid, DLG_APANEL_SKLADISTE, DIALOG_STYLE_LIST, T(playerid, "Skladistari"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_skladiste(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    if (listitem <= 3)
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
    if (listitem == 2 || listitem == 3)
    {
        if (GetPlayerVehicleID(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo postavljate pjeske.")), ShowSkladisteAdmin(playerid);
        if (listitem == 2)
        {
            new r = 0;
            while (r < SK_RAMPI && gSkladisteRampa[r][srIma]) r++;
            if (r == SK_RAMPI) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowSkladisteAdmin(playerid);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO warehouse_ramps (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
            SkladisteRampaStvori(r, ZadnjiId(), x, y, z, a);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Utovarna rampa postavljena."));
            return ShowSkladisteAdmin(playerid);
        }
        new k = 0;
        while (k < SK_PALETA && gSkladistePaleta[k][spIma]) k++;
        if (k == SK_PALETA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowSkladisteAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO warehouse_pallets (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        SkladistePaletaStvori(k, ZadnjiId(), x, y, z, a);
        SetPlayerPos(playerid, x, y, z + 1.0);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Paleta postavljena."));
        return ShowSkladisteAdmin(playerid);
    }
    new i = listitem - 4;
    if (i < 0 || i >= sizeof gSkladisteApPopis[]) return 1;
    new k = gSkladisteApPopis[playerid][i];
    if (k >= SK_VILJUSKARA + SK_RAMPI)
    {
        k -= SK_VILJUSKARA + SK_RAMPI;
        if (!gSkladistePaleta[k][spIma]) return ShowSkladisteAdmin(playerid);
        DestroyDynamicObject(gSkladistePaleta[k][spObjekt]);
        SkladisteObjektMakni(gSkladistePaleta[k][spTeret]);
        DestroyDynamic3DTextLabel(gSkladistePaleta[k][spTekst]);
        gSkladistePaleta[k][spIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_pallets WHERE id = %d", gSkladistePaleta[k][spDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Paleta obrisana."));
        return ShowSkladisteAdmin(playerid);
    }
    if (k >= SK_VILJUSKARA)
    {
        k -= SK_VILJUSKARA;
        if (!gSkladisteRampa[k][srIma]) return ShowSkladisteAdmin(playerid);
        SkladisteDolazakMakni(k);
        DestroyDynamic3DTextLabel(gSkladisteRampa[k][srTekst]);
        gSkladisteRampa[k][srIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_ramps WHERE id = %d", gSkladisteRampa[k][srDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Utovarna rampa obrisana."));
        return ShowSkladisteAdmin(playerid);
    }
    if (!gSkladisteVilj[k][svIma]) return ShowSkladisteAdmin(playerid);
    if (gSkladisteVilj[k][svIgrac] != INVALID_PLAYER_ID) SkladisteViljOslobodi(k);
    SkladisteViljNpcMakni(k);
    DestroyObject(gSkladisteVilj[k][svTeret]);
    DestroyVehicle(gSkladisteVilj[k][svVozilo]);
    gSkladisteVilj[k][svIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM warehouse_forklifts WHERE id = %d", gSkladisteVilj[k][svDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Viljuskar obrisan."));
    return ShowSkladisteAdmin(playerid);
}
