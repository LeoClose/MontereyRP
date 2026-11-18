#define DROGA_MAFIJA        1
#define BILJKA_MAX          400
#define BILJKA_PO_KUCI      6
#define BILJKA_MODEL        19473
#define BILJKA_RAST_S       7200
#define BILJKA_SUHA_S       10800
#define BILJKA_PRINOS       25
#define BILJKA_PRINOS_RND   15
#define BILJKA_DOMET        1.5
#define DOB_MAX             8
#define DOB_SKIN            29
#define DOB_DOMET           2.0
#define DOB_SJEME_CIJENA    60
#define DOB_KEM_CIJENA      450
#define DOB_SJEME_DAN       150
#define DOB_KEM_DAN         40
#define UVOZ_MAX            16
#define UVOZ_CIJENA         6000
#define UVOZ_KOLICINA       150
#define UVOZ_PAUZA_S        7200
#define UVOZ_ROK_MS         900000
#define UVOZ_DOMET          6.0
#define ORUZJE_CIJENA       25000
#define ORUZJE_PAUZA_S      10800
#define LAB_PASTA_PO_KEM    10
#define LAB_RAST_S          900
#define LAB_MAX_SERIJA      200
#define METH_KEM            3
#define METH_PRINOS         30
#define METH_MS             100
#define METH_TRAJANJE_MS    30000
#define METH_EKSPLOZIJA_MS  1500
#define METH_ZONA_OD        55.0
#define METH_ZONA_DO        75.0
#define METH_BAR_X          240.0
#define METH_BAR_Y          330.0
#define METH_BAR_W          160.0
#define DROGA_BOJA          0x8E44ADE6

enum E_BILJKA { bool:biIma, biDb, biKuca, biVlasnik, Float:biX, Float:biY, Float:biZ, biPosadjeno, biZalito, STREAMER_TAG_OBJECT:biObj, biFaza }
enum E_DOB { bool:dbIma, dbDb, Float:dbX, Float:dbY, Float:dbZ, Float:dbA, dbInt, dbSvijet, dbAktor, STREAMER_TAG_3D_TEXT_LABEL:dbTekst, dbSjeme, dbKem, dbDan }
enum E_UVOZ { bool:uvIma, uvDb, Float:uvX, Float:uvY, Float:uvZ }

static gBiljka[BILJKA_MAX][E_BILJKA], gDob[DOB_MAX][E_DOB], gUvoz[UVOZ_MAX][E_UVOZ];
static gDrogaBiljka[MAX_PLAYERS], gDrogaDob[MAX_PLAYERS], gDrogaKupuje[MAX_PLAYERS], gDrogaUvoz[MAX_PLAYERS] = { -1, ... }, gDrogaUvozToken[MAX_PLAYERS];
static gDrogaUvozOrg[MAX_PLAYERS], gDrogaUvozVrsta[MAX_PLAYERS], gDrogaUvozZadnji[MAX_HQ][3], gApDroga[MAX_PLAYERS][DOB_MAX + UVOZ_MAX];
static bool:gMeth[MAX_PLAYERS], Float:gMethTemp[MAX_PLAYERS], Float:gMethBrzina[MAX_PLAYERS], gMethOd[MAX_PLAYERS], gMethDobro[MAX_PLAYERS], gMethUkupno[MAX_PLAYERS], gMethVruce[MAX_PLAYERS], gMethToken[MAX_PLAYERS];
static PlayerText:gMethTd[MAX_PLAYERS][5];
static const gOruzjeSanduk[][2] = { { ITEM_AK47, 2 }, { ITEM_M4, 1 }, { ITEM_DEAGLE, 2 }, { ITEM_UZI, 2 }, { ITEM_MUN_PUSKA, 300 }, { ITEM_MUN_PISTOLJ, 150 }, { ITEM_MUN_SMG, 200 } };

InitDroga()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS weed_plants (id INTEGER PRIMARY KEY AUTOINCREMENT, house INTEGER NOT NULL, owner INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, planted INTEGER NOT NULL, watered INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_suppliers (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_imports (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_labs (org INTEGER PRIMARY KEY, cocaine INTEGER NOT NULL DEFAULT 0, ready INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE INDEX IF NOT EXISTS weed_plants_house ON weed_plants (house)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, house, owner, x, y, z, planted, watered FROM weed_plants ORDER BY id LIMIT %d", BILJKA_MAX);
    for (new b = 0, n = DB_GetRowCount(res); b < n; b++, DB_SelectNextRow(res))
    {
        gBiljka[b][biDb] = DB_GetFieldInt(res, 0);
        gBiljka[b][biKuca] = DB_GetFieldInt(res, 1);
        gBiljka[b][biVlasnik] = DB_GetFieldInt(res, 2);
        gBiljka[b][biX] = DB_GetFieldFloat(res, 3);
        gBiljka[b][biY] = DB_GetFieldFloat(res, 4);
        gBiljka[b][biZ] = DB_GetFieldFloat(res, 5);
        gBiljka[b][biPosadjeno] = DB_GetFieldInt(res, 6);
        gBiljka[b][biZalito] = DB_GetFieldInt(res, 7);
        BiljkaStvori(b);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM drug_suppliers ORDER BY id LIMIT %d", DOB_MAX);
    for (new p = 0, n = DB_GetRowCount(res); p < n; p++, DB_SelectNextRow(res))
    {
        gDob[p][dbDb] = DB_GetFieldInt(res, 0);
        gDob[p][dbX] = DB_GetFieldFloat(res, 1);
        gDob[p][dbY] = DB_GetFieldFloat(res, 2);
        gDob[p][dbZ] = DB_GetFieldFloat(res, 3);
        gDob[p][dbA] = DB_GetFieldFloat(res, 4);
        gDob[p][dbInt] = DB_GetFieldInt(res, 5);
        gDob[p][dbSvijet] = DB_GetFieldInt(res, 6);
        DobStvori(p);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z FROM drug_imports ORDER BY id LIMIT %d", UVOZ_MAX);
    for (new u = 0, n = DB_GetRowCount(res); u < n; u++, DB_SelectNextRow(res))
    {
        gUvoz[u][uvIma] = true;
        gUvoz[u][uvDb] = DB_GetFieldInt(res, 0);
        gUvoz[u][uvX] = DB_GetFieldFloat(res, 1);
        gUvoz[u][uvY] = DB_GetFieldFloat(res, 2);
        gUvoz[u][uvZ] = DB_GetFieldFloat(res, 3);
    }
    DB_FreeResultSet(res);
}

bool:DrogaKuha(playerid)
{
    return gMeth[playerid];
}

DrogaConnect(playerid)
{
    gDrogaUvoz[playerid] = -1;
    gDrogaUvozToken[playerid]++;
    gMeth[playerid] = false;
    gMethToken[playerid]++;
}

DrogaOdlazi(playerid)
{
    gDrogaUvoz[playerid] = -1;
    gDrogaUvozToken[playerid]++;
    if (gMeth[playerid]) MethKraj(playerid, false);
}

static bool:DrogaMafija(playerid)
{
    return gOrg[playerid] && GetOrgTip(gOrg[playerid]) == DROGA_MAFIJA;
}

static BiljkaFaza(b)
{
    new sad = gettime();
    if (sad - gBiljka[b][biZalito] > BILJKA_SUHA_S) return 4;
    new rast = sad - gBiljka[b][biPosadjeno];
    return rast >= BILJKA_RAST_S ? 3 : rast >= BILJKA_RAST_S * 2 / 3 ? 2 : rast >= BILJKA_RAST_S / 3 ? 1 : 0;
}

static BiljkaStvori(b)
{
    new faza = BiljkaFaza(b), Float:z = gBiljka[b][biZ] - (faza == 0 ? 0.75 : faza == 1 ? 0.45 : faza == 2 ? 0.2 : faza == 4 ? 0.5 : 0.0);
    if (gBiljka[b][biIma]) DestroyDynamicObject(gBiljka[b][biObj]);
    gBiljka[b][biIma] = true;
    gBiljka[b][biFaza] = faza;
    gBiljka[b][biObj] = CreateDynamicObject(BILJKA_MODEL, gBiljka[b][biX], gBiljka[b][biY], z, faza == 4 ? 25.0 : 0.0, 0.0, 0.0, KUCA_SVIJET + gBiljka[b][biKuca], KucaInterijerDb(gBiljka[b][biKuca]), -1, 30.0);
    if (faza == 4) SetDynamicObjectMaterial(gBiljka[b][biObj], 0, 3922, "bistro", "Marble2", 0xFF6E5A2A);
}

static BiljkaUkloni(b)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM weed_plants WHERE id = %d", gBiljka[b][biDb]));
    DestroyDynamicObject(gBiljka[b][biObj]);
    gBiljka[b][biIma] = false;
}

static BiljkaBlizu(playerid)
{
    new svijet = GetPlayerVirtualWorld(playerid) - KUCA_SVIJET;
    for (new b = 0; b < BILJKA_MAX; b++)
        if (gBiljka[b][biIma] && gBiljka[b][biKuca] == svijet && IsPlayerInRangeOfPoint(playerid, BILJKA_DOMET, gBiljka[b][biX], gBiljka[b][biY], gBiljka[b][biZ])) return b;
    return -1;
}

DrogaTick()
{
    static sekunde;
    if (++sekunde < 60) return;
    sekunde = 0;
    for (new b = 0; b < BILJKA_MAX; b++) if (gBiljka[b][biIma] && BiljkaFaza(b) != gBiljka[b][biFaza]) BiljkaStvori(b);
}

DrogaPosadi(playerid)
{
    new kuca = KucaPristupDb(playerid);
    if (!kuca) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Sjemenke mozete posaditi u lonac u svojoj kuci (ili kuci koju unajmljujete)."));
    new n = 0, slobodno = -1;
    for (new b = 0; b < BILJKA_MAX; b++)
    {
        if (!gBiljka[b][biIma]) { if (slobodno == -1) slobodno = b; }
        else if (gBiljka[b][biKuca] == kuca) n++;
    }
    if (n >= BILJKA_PO_KUCI) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U kuci mozete imati najvise %d biljaka."), BILJKA_PO_KUCI);
    if (slobodno == -1 || BiljkaBlizu(playerid) != -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovdje ne mozete posaditi biljku."));
    if (!GiveItem(playerid, ITEM_SJEME, -1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate tog predmeta."));
    new b = slobodno, Float:a, sad = gettime();
    GetPlayerPos(playerid, gBiljka[b][biX], gBiljka[b][biY], gBiljka[b][biZ]);
    GetPlayerFacingAngle(playerid, a);
    gBiljka[b][biX] -= 0.8 * floatsin(a, degrees);
    gBiljka[b][biY] += 0.8 * floatcos(a, degrees);
    gBiljka[b][biZ] -= 1.0;
    gBiljka[b][biKuca] = kuca;
    gBiljka[b][biVlasnik] = gAccountId[playerid];
    gBiljka[b][biPosadjeno] = sad;
    gBiljka[b][biZalito] = sad;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO weed_plants (house, owner, x, y, z, planted, watered) VALUES (%d, %d, %f, %f, %f, %d, %d)", kuca, gAccountId[playerid], gBiljka[b][biX], gBiljka[b][biY], gBiljka[b][biZ], sad, sad));
    gBiljka[b][biDb] = ZadnjiId();
    gBiljka[b][biIma] = false;
    BiljkaStvori(b);
    ApplyAnimation(playerid, "BOMBER", "BOM_Plant", 4.1, false, false, false, false, 0);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s sadi sjemenku u lonac.", GetRPIme(playerid), "", 1);
    return 1;
}

static BiljkaMeni(playerid, b)
{
    new faza = BiljkaFaza(b), sad = gettime(), tekst[256], postotak = (sad - gBiljka[b][biPosadjeno]) * 100 / BILJKA_RAST_S, suho = (BILJKA_SUHA_S - (sad - gBiljka[b][biZalito])) / 60;
    if (faza != gBiljka[b][biFaza]) BiljkaStvori(b);
    gDrogaBiljka[playerid] = b;
    if (PolicajacNaDuznosti(playerid))
        return PrikaziDialog(playerid, DLG_BILJKA, DIALOG_STYLE_LIST, T(playerid, "Biljka marihuane"), T(playerid, "Zaplijeni i unisti biljku"), T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    if (KucaPristupDb(playerid) != gBiljka[b][biKuca]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo nije vasa biljka.")), 1;
    if (faza == 4) format(tekst, sizeof tekst, T(playerid, "{C0392B}Biljka se osusila.\n{FFFFFF}Ukloni biljku"));
    else format(tekst, sizeof tekst, T(playerid, "{AAAAAA}Rast: %d%%  |  Voda jos: %d min\n{FFFFFF}Zalij\n%sOberi\n{FFFFFF}Ukloni biljku"), postotak > 100 ? 100 : postotak, suho < 0 ? 0 : suho, faza == 3 ? ("{2ECC71}") : ("{777777}"));
    return PrikaziDialog(playerid, DLG_BILJKA, DIALOG_STYLE_LIST, T(playerid, "Biljka marihuane"), tekst, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:biljka(playerid, response, listitem)
{
    new b = gDrogaBiljka[playerid];
    if (!response || b < 0 || b >= BILJKA_MAX || !gBiljka[b][biIma] || BiljkaBlizu(playerid) != b) return 1;
    if (PolicajacNaDuznosti(playerid))
    {
        BiljkaUkloni(b);
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s cupa biljku marihuane i stavlja je u vrecicu za dokaze.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (KucaPristupDb(playerid) != gBiljka[b][biKuca]) return 1;
    new faza = BiljkaFaza(b);
    if (faza == 4)
    {
        if (listitem == 1) BiljkaUkloni(b);
        else BiljkaMeni(playerid, b);
        return 1;
    }
    if (listitem == 3)
    {
        BiljkaUkloni(b);
        return 1;
    }
    if (listitem == 1)
    {
        gBiljka[b][biZalito] = gettime();
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE weed_plants SET watered = %d WHERE id = %d", gBiljka[b][biZalito], gBiljka[b][biDb]));
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s zaljeva biljku.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (listitem != 2) return BiljkaMeni(playerid, b);
    if (faza != 3) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Biljka jos nije spremna za berbu.")), 1;
    new grama = BILJKA_PRINOS + random(BILJKA_PRINOS_RND + 1);
    if (!GiveItem(playerid, ITEM_MARIHUANA, grama)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi vise marihuane.")), 1;
    BiljkaUkloni(b);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s bere biljku marihuane.", GetRPIme(playerid), "", 1);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ubrali ste %d g marihuane."), grama);
    return 1;
}

static DobStvori(p)
{
    gDob[p][dbIma] = true;
    gDob[p][dbAktor] = CreateActor(DOB_SKIN, gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ], gDob[p][dbA]);
    SetActorVirtualWorld(gDob[p][dbAktor], gDob[p][dbSvijet]);
    SetActorInvulnerable(gDob[p][dbAktor], true);
    gDob[p][dbTekst] = CreateDynamic3DTextLabel("[Y]", 0xAAAAAAFF, gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ] + 1.2, 4.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, gDob[p][dbSvijet], gDob[p][dbInt]);
}

static DobZaliha(p)
{
    new dan = gettime() / 86400;
    if (gDob[p][dbDan] == dan) return;
    gDob[p][dbDan] = dan;
    gDob[p][dbSjeme] = DOB_SJEME_DAN;
    gDob[p][dbKem] = DOB_KEM_DAN;
}

static DobMeni(playerid, p)
{
    new tekst[512], bool:mafija = DrogaMafija(playerid);
    DobZaliha(p);
    gDrogaDob[playerid] = p;
    format(tekst, sizeof tekst, T(playerid, "Sjemenke marihuane ($%d/kom, zaliha %d)\n%sKemikalije ($%d/l, zaliha %d)\n%sUvoz koka paste ($%d, %d g)\n%sSanduk oruzja ($%d)"),
        DOB_SJEME_CIJENA, gDob[p][dbSjeme], mafija ? ("{FFFFFF}") : ("{777777}"), DOB_KEM_CIJENA, gDob[p][dbKem], mafija ? ("{FFFFFF}") : ("{777777}"), UVOZ_CIJENA, UVOZ_KOLICINA, mafija ? ("{FFFFFF}") : ("{777777}"), ORUZJE_CIJENA);
    return PrikaziDialog(playerid, DLG_DOBAVLJAC, DIALOG_STYLE_LIST, T(playerid, "Dobavljac"), tekst, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DrogaKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    for (new p = 0; p < DOB_MAX; p++)
    {
        if (!gDob[p][dbIma] || GetPlayerVirtualWorld(playerid) != gDob[p][dbSvijet] || GetPlayerInterior(playerid) != gDob[p][dbInt] || !IsPlayerInRangeOfPoint(playerid, DOB_DOMET, gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ])) continue;
        AktorKaze(playerid, gDob[p][dbAktor], "Dobavljac", "Sto trebas? Brzo.");
        DobMeni(playerid, p);
        return 1;
    }
    new b = BiljkaBlizu(playerid);
    if (b == -1) return 0;
    BiljkaMeni(playerid, b);
    return 1;
}

DLG:dobavljac(playerid, response, listitem)
{
    new p = gDrogaDob[playerid];
    if (!response || p < 0 || p >= DOB_MAX || !gDob[p][dbIma] || !IsPlayerInRangeOfPoint(playerid, DOB_DOMET, gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ])) return 1;
    if (listitem > 0 && !DrogaMafija(playerid))
    {
        AktorKaze(playerid, gDob[p][dbAktor], "Dobavljac", "To prodajem samo ljudima od povjerenja.");
        return DobMeni(playerid, p);
    }
    if (listitem >= 2) return DrogaUvozKreni(playerid, listitem == 3);
    gDrogaKupuje[playerid] = listitem;
    return PrikaziDialog(playerid, DLG_DOBAVLJAC_KOL, DIALOG_STYLE_INPUT, T(playerid, "Dobavljac"), T(playerid, "Unesite kolicinu:"), T(playerid, "Kupi"), T(playerid, "Natrag"));
}

DLG:dobavljac_kol(playerid, response, const inputtext[])
{
    new p = gDrogaDob[playerid];
    if (p < 0 || p >= DOB_MAX || !gDob[p][dbIma]) return 1;
    if (!response) return DobMeni(playerid, p);
    DobZaliha(p);
    new bool:kem = gDrogaKupuje[playerid] == 1, n = strval(inputtext), zaliha = kem ? gDob[p][dbKem] : gDob[p][dbSjeme], cijena = kem ? DOB_KEM_CIJENA : DOB_SJEME_CIJENA, item = kem ? ITEM_KEMIKALIJE : ITEM_SJEME;
    if (n <= 0 || n > zaliha) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dobavljac nema toliko (zaliha: %d)."), zaliha), DobMeni(playerid, p);
    if (gInv[playerid][item] + n > gItems[item][ITEM_MAX]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko (najvise %d)."), gItems[item][ITEM_MAX]), DobMeni(playerid, p);
    if (!DajNovac(playerid, -n * cijena)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), DobMeni(playerid, p);
    GiveItem(playerid, item, n);
    if (kem) gDob[p][dbKem] -= n;
    else gDob[p][dbSjeme] -= n;
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d %s %s za $%d."), n, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]), n * cijena);
    return 1;
}

static bool:DrogaHq(org, &Float:x, &Float:y, &Float:z)
{
    new Float:a;
    for (new i = 0; i < MAX_HQ; i++) if (OrgLokacija(i, x, y, z, a) == org) return true;
    return false;
}

static DrogaUvozKreni(playerid, bool:oruzje)
{
    new org = gOrg[playerid], slot = -1, sad = gettime(), k = oruzje ? 2 : 1, pauza = oruzje ? ORUZJE_PAUZA_S : UVOZ_PAUZA_S, Float:x, Float:y, Float:z;
    for (new i = 0; i < MAX_HQ && slot == -1; i++) if (gDrogaUvozZadnji[i][0] == org) slot = i;
    if (slot != -1 && sad - gDrogaUvozZadnji[slot][k] < pauza) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Sljedeca posiljka bit ce spremna za %d min."), (pauza - (sad - gDrogaUvozZadnji[slot][k])) / 60 + 1), 1;
    if (gDrogaUvoz[playerid] != -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec imate aktivnu posiljku.")), 1;
    if (oruzje && !DrogaHq(org, x, y, z)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vasa organizacija nema HQ za dostavu.")), 1;
    new tocke[UVOZ_MAX], n = 0;
    for (new u = 0; u < UVOZ_MAX; u++) if (gUvoz[u][uvIma]) tocke[n++] = u;
    if (!n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trenutno nema ruta za uvoz.")), 1;
    if (!DajNovac(playerid, oruzje ? -ORUZJE_CIJENA : -UVOZ_CIJENA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), 1;
    if (slot == -1) for (new i = 0; i < MAX_HQ && slot == -1; i++)
    {
        if (gDrogaUvozZadnji[i][0] && (sad - gDrogaUvozZadnji[i][1] < UVOZ_PAUZA_S || sad - gDrogaUvozZadnji[i][2] < ORUZJE_PAUZA_S)) continue;
        slot = i;
        gDrogaUvozZadnji[i][1] = 0;
        gDrogaUvozZadnji[i][2] = 0;
    }
    if (slot != -1)
    {
        gDrogaUvozZadnji[slot][0] = org;
        gDrogaUvozZadnji[slot][k] = sad;
    }
    new u = tocke[random(n)];
    gDrogaUvoz[playerid] = u;
    gDrogaUvozOrg[playerid] = org;
    gDrogaUvozVrsta[playerid] = oruzje ? 1 : 0;
    SetPlayerCheckpoint(playerid, gUvoz[u][uvX], gUvoz[u][uvY], gUvoz[u][uvZ], UVOZ_DOMET);
    SetTimerEx("DrogaUvozIstek", UVOZ_ROK_MS, false, "ii", playerid, ++gDrogaUvozToken[playerid]);
    if (oruzje) SendClientMessage(playerid, COLOR_INFO, T(playerid, "Sanduk oruzja ceka na %s. Dodite vozilom u roku od %d min."), CestaZona(gUvoz[u][uvX], gUvoz[u][uvY]), UVOZ_ROK_MS / 60000);
    else SendClientMessage(playerid, COLOR_INFO, T(playerid, "Posiljka ceka na %s. Dodite vozilom u roku od %d min."), CestaZona(gUvoz[u][uvX], gUvoz[u][uvY]), UVOZ_ROK_MS / 60000);
    return 1;
}

static DrogaUvozGotov(playerid)
{
    gDrogaUvoz[playerid] = -1;
    gDrogaUvozToken[playerid]++;
    DisablePlayerCheckpoint(playerid);
}

static DrogaPolicija(const poruka[], u)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, poruka), CestaZona(gUvoz[u][uvX], gUvoz[u][uvY]));
}

static DrogaOruzjeDostava(playerid)
{
    new org = gDrogaUvozOrg[playerid], Float:x, Float:y, Float:z;
    if (!DrogaHq(org, x, y, z) || !IsPlayerInRangeOfPoint(playerid, UVOZ_DOMET + 2.0, x, y, z)) return 0;
    if (!IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dovezite sanduk vozilom do ulaza u HQ."));
        return 1;
    }
    for (new k = 0; k < sizeof gOruzjeSanduk; k++) SefDodaj(org, gOruzjeSanduk[k][0], gOruzjeSanduk[k][1]);
    OrgDnevnik(org, playerid, DNEVNIK_SANDUK);
    DrogaUvozGotov(playerid);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Sanduk oruzja je dostavljen u sef organizacije (/sef)."));
    return 1;
}

forward DrogaUvozIstek(playerid, token);
public DrogaUvozIstek(playerid, token)
{
    if (token != gDrogaUvozToken[playerid] || gDrogaUvoz[playerid] == -1) return;
    gDrogaUvoz[playerid] = -1;
    DisablePlayerCheckpoint(playerid);
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Posiljka je propala, niste stigli na vrijeme."));
}

DrogaCheckpoint(playerid)
{
    new u = gDrogaUvoz[playerid];
    if (u == -1) return 0;
    if (gDrogaUvozVrsta[playerid] == 2) return DrogaOruzjeDostava(playerid);
    if (!IsPlayerInRangeOfPoint(playerid, UVOZ_DOMET + 2.0, gUvoz[u][uvX], gUvoz[u][uvY], gUvoz[u][uvZ])) return 0;
    if (!IsPlayerInAnyVehicle(playerid))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za posiljku trebate vozilo."));
        return 1;
    }
    if (gDrogaUvozVrsta[playerid] == 1)
    {
        new Float:x, Float:y, Float:z;
        if (!DrogaHq(gDrogaUvozOrg[playerid], x, y, z))
        {
            DrogaUvozGotov(playerid);
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vasa organizacija nema HQ za dostavu."));
            return 1;
        }
        gDrogaUvozVrsta[playerid] = 2;
        SetPlayerCheckpoint(playerid, x, y, z, UVOZ_DOMET);
        SetTimerEx("DrogaUvozIstek", UVOZ_ROK_MS, false, "ii", playerid, ++gDrogaUvozToken[playerid]);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Utovarili ste sanduk oruzja. Dostavite ga vozilom do HQ-a u roku od %d min."), UVOZ_ROK_MS / 60000);
        DrogaPolicija("Dojava: sumnjiv utovar sanduka u vozilo, %s.", u);
        return 1;
    }
    new n = UVOZ_KOLICINA;
    if (gInv[playerid][ITEM_KOKA] + n > gItems[ITEM_KOKA][ITEM_MAX]) n = gItems[ITEM_KOKA][ITEM_MAX] - gInv[playerid][ITEM_KOKA];
    if (n <= 0)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi vise koka paste."));
        return 1;
    }
    GiveItem(playerid, ITEM_KOKA, n);
    DrogaUvozGotov(playerid);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Preuzeli ste %d g koka paste. Preradite je u laboratoriju HQ-a (/lab)."), n);
    DrogaPolicija("Dojava: sumnjiva primopredaja paketa, %s.", u);
    return 1;
}

static bool:DrogaULabu(playerid)
{
    return gOrg[playerid] && GetPlayerVirtualWorld(playerid) == HQ_SVIJET + gOrg[playerid] && DrogaMafija(playerid);
}

static LabMeni(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT cocaine, ready FROM drug_labs WHERE org = %d", gOrg[playerid]), kokain = 0, gotovo = 0, tekst[512], stanje[96];
    if (DB_GetRowCount(res))
    {
        kokain = DB_GetFieldInt(res, 0);
        gotovo = DB_GetFieldInt(res, 1);
    }
    DB_FreeResultSet(res);
    if (!kokain) format(stanje, sizeof stanje, T(playerid, "{777777}Nema serije u obradi"));
    else if (gettime() < gotovo) format(stanje, sizeof stanje, T(playerid, "{F1C40F}Kokain u obradi: %d g (jos %d min)"), kokain, (gotovo - gettime()) / 60 + 1);
    else format(stanje, sizeof stanje, T(playerid, "{2ECC71}Pokupi gotov kokain: %d g"), kokain);
    format(tekst, sizeof tekst, T(playerid, "%s\n{FFFFFF}Preradi koka pastu u kokain (%d g paste + 1 l kemikalija = %d g)\nKuhaj metamfetamin (%d l kemikalija)"), stanje, LAB_PASTA_PO_KEM, LAB_PASTA_PO_KEM, METH_KEM);
    return PrikaziDialog(playerid, DLG_LAB, DIALOG_STYLE_LIST, T(playerid, "Laboratorij"), tekst, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

CMD:lab(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (!DrogaULabu(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Laboratorij se nalazi u HQ-u mafije."));
    if (gMeth[playerid]) return 1;
    return LabMeni(playerid);
}

CMD:laboratorij(playerid, params[]) return cmd_lab(playerid, params);

DLG:lab(playerid, response, listitem)
{
    if (!response || !DrogaULabu(playerid)) return 1;
    new org = gOrg[playerid];
    if (listitem == 0)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT cocaine, ready FROM drug_labs WHERE org = %d", org), kokain = 0, gotovo = 0;
        if (DB_GetRowCount(res))
        {
            kokain = DB_GetFieldInt(res, 0);
            gotovo = DB_GetFieldInt(res, 1);
        }
        DB_FreeResultSet(res);
        if (!kokain || gettime() < gotovo) return LabMeni(playerid);
        new n = gItems[ITEM_KOKAIN][ITEM_MAX] - gInv[playerid][ITEM_KOKAIN];
        if (n > kokain) n = kokain;
        if (n <= 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi vise kokaina.")), 1;
        GiveItem(playerid, ITEM_KOKAIN, n);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE drug_labs SET cocaine = cocaine - %d WHERE org = %d", n, org));
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Pokupili ste %d g kokaina."), n);
        return LabMeni(playerid);
    }
    if (listitem == 1) return PrikaziDialog(playerid, DLG_LAB_KOL, DIALOG_STYLE_INPUT, T(playerid, "Laboratorij"), T(playerid, "Koliko grama koka paste preradujete? (visekratnik od 10)"), T(playerid, "Preradi"), T(playerid, "Natrag"));
    if (listitem == 2) return MethPocni(playerid);
    return 1;
}

DLG:lab_kol(playerid, response, const inputtext[])
{
    if (!DrogaULabu(playerid)) return 1;
    if (!response) return LabMeni(playerid);
    new org = gOrg[playerid], n = strval(inputtext) / LAB_PASTA_PO_KEM * LAB_PASTA_PO_KEM, kem = n / LAB_PASTA_PO_KEM;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT cocaine FROM drug_labs WHERE org = %d", org), kokain = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    if (kokain) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prvo pokupite prethodnu seriju.")), LabMeni(playerid);
    if (n <= 0 || n > LAB_MAX_SERIJA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Serija moze imati 10 - %d g."), LAB_MAX_SERIJA), LabMeni(playerid);
    if (gInv[playerid][ITEM_KOKA] < n || gInv[playerid][ITEM_KEMIKALIJE] < kem) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trebate %d g koka paste i %d l kemikalija."), n, kem), LabMeni(playerid);
    GiveItem(playerid, ITEM_KOKA, -n);
    GiveItem(playerid, ITEM_KEMIKALIJE, -kem);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO drug_labs (org, cocaine, ready) VALUES (%d, %d, %d)", org, n, gettime() + LAB_RAST_S));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s mijesa kemikalije s koka pastom.", GetRPIme(playerid), "", 1);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Serija od %d g kokaina bit ce gotova za %d min."), n, LAB_RAST_S / 60);
    return 1;
}

static PlayerText:MethKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawShow(playerid, td);
    return td;
}

static MethPocni(playerid)
{
    if (gInv[playerid][ITEM_KEMIKALIJE] < METH_KEM) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trebate %d l kemikalija."), METH_KEM), 1;
    GiveItem(playerid, ITEM_KEMIKALIJE, -METH_KEM);
    gMeth[playerid] = true;
    gMethTemp[playerid] = 40.0;
    gMethBrzina[playerid] = 0.0;
    gMethOd[playerid] = GetTickCount();
    gMethDobro[playerid] = 0;
    gMethUkupno[playerid] = 0;
    gMethVruce[playerid] = 0;
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, true, false, false, false, 0);
    gMethTd[playerid][0] = MethKutija(playerid, METH_BAR_X - 3.0, METH_BAR_Y - 3.0, METH_BAR_W + 6.0, 16.0, 0x000000C0);
    gMethTd[playerid][1] = MethKutija(playerid, METH_BAR_X, METH_BAR_Y, METH_BAR_W, 10.0, 0x3A3A3AFF);
    gMethTd[playerid][2] = MethKutija(playerid, METH_BAR_X + METH_ZONA_OD / 100.0 * METH_BAR_W, METH_BAR_Y, (METH_ZONA_DO - METH_ZONA_OD) / 100.0 * METH_BAR_W, 10.0, 0x2ECC71AA);
    gMethTd[playerid][3] = MethKutija(playerid, METH_BAR_X + 85.0 / 100.0 * METH_BAR_W, METH_BAR_Y, 15.0 / 100.0 * METH_BAR_W, 10.0, 0xC0392BAA);
    gMethTd[playerid][4] = MethKutija(playerid, METH_BAR_X, METH_BAR_Y - 2.0, 2.0, 14.0, 0xFFFFFFFF);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), DROGA_BOJA, T(playerid, "Drzite temperaturu u zelenom.~n~W - grijanje, S - hladjenje, ENTER - odustani.~n~Crveno predugo = eksplozija!"), 0);
    SetTimerEx("MethTick", METH_MS, false, "ii", playerid, ++gMethToken[playerid]);
    return 1;
}

static MethKraj(playerid, bool:gotovo)
{
    gMeth[playerid] = false;
    gMethToken[playerid]++;
    for (new i = 0; i < sizeof gMethTd[]; i++) PlayerTextDrawDestroy(playerid, gMethTd[playerid][i]);
    if (!IsPlayerConnected(playerid)) return;
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
    if (!gotovo) return;
    new kvaliteta = gMethUkupno[playerid] ? gMethDobro[playerid] * 100 / gMethUkupno[playerid] : 0, grama = kvaliteta < 30 ? 0 : METH_PRINOS * kvaliteta / 100, poruka[128];
    if (grama > gItems[ITEM_METH][ITEM_MAX] - gInv[playerid][ITEM_METH]) grama = gItems[ITEM_METH][ITEM_MAX] - gInv[playerid][ITEM_METH];
    if (grama > 0) GiveItem(playerid, ITEM_METH, grama);
    format(poruka, sizeof poruka, T(playerid, "Kvaliteta: %d%%~n~Dobili ste %d g metamfetamina."), kvaliteta, grama);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), DROGA_BOJA, poruka);
}

forward MethTick(playerid, token);
public MethTick(playerid, token)
{
    if (token != gMethToken[playerid] || !gMeth[playerid]) return;
    if (!IsPlayerConnected(playerid) || !DrogaULabu(playerid))
    {
        MethKraj(playerid, false);
        return;
    }
    new KEY:tipke, gore, strana, sad = GetTickCount();
    GetPlayerKeys(playerid, tipke, gore, strana);
    gMethBrzina[playerid] = gMethBrzina[playerid] * 0.85 + float(random(21) - 10) / 100.0 + (gore < 0 ? 0.15 : gore > 0 ? -0.15 : 0.03);
    if (gMethBrzina[playerid] > 1.5) gMethBrzina[playerid] = 1.5;
    if (gMethBrzina[playerid] < -1.5) gMethBrzina[playerid] = -1.5;
    gMethTemp[playerid] += gMethBrzina[playerid];
    if (gMethTemp[playerid] < 0.0) gMethTemp[playerid] = 0.0, gMethBrzina[playerid] = 0.0;
    if (gMethTemp[playerid] > 100.0) gMethTemp[playerid] = 100.0, gMethBrzina[playerid] = 0.0;
    gMethUkupno[playerid]++;
    if (gMethTemp[playerid] >= METH_ZONA_OD && gMethTemp[playerid] <= METH_ZONA_DO) gMethDobro[playerid]++;
    if (gMethTemp[playerid] >= 85.0) gMethVruce[playerid] += METH_MS;
    else gMethVruce[playerid] = 0;
    PlayerTextDrawHide(playerid, gMethTd[playerid][4]);
    PlayerTextDrawSetPos(playerid, gMethTd[playerid][4], METH_BAR_X + gMethTemp[playerid] / 100.0 * METH_BAR_W - 1.0, METH_BAR_Y - 2.0);
    PlayerTextDrawShow(playerid, gMethTd[playerid][4]);
    if (gMethVruce[playerid] >= METH_EKSPLOZIJA_MS)
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        MethKraj(playerid, false);
        CreateExplosion(x, y, z, 12, 3.0);
        IgraObavijest(playerid, T(playerid, "Laboratorij"), OBAVIJEST_GRESKA, T(playerid, "Smjesa je eksplodirala! Kemikalije su izgubljene."));
        return;
    }
    if (sad - gMethOd[playerid] >= METH_TRAJANJE_MS)
    {
        MethKraj(playerid, true);
        return;
    }
    SetTimerEx("MethTick", METH_MS, false, "ii", playerid, token);
}

bool:DrogaTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gMeth[playerid]) return false;
    if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK))
    {
        MethKraj(playerid, false);
        IgraObavijest(playerid, T(playerid, "Laboratorij"), OBAVIJEST_GRESKA, T(playerid, "Odustali ste. Kemikalije su izgubljene."));
    }
    return true;
}

ShowDrogaAdmin(playerid)
{
    new list[2048], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novi dobavljac na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova tocka uvoza na mojoj poziciji"));
    for (new p = 0; p < DOB_MAX; p++)
    {
        if (!gDob[p][dbIma]) continue;
        format(row, sizeof row, T(playerid, "Dobavljac [%d] %s - obrisi"), gDob[p][dbDb], CestaZona(gDob[p][dbX], gDob[p][dbY]));
        DodajStupac(list, len, row);
        gApDroga[playerid][n++] = p;
    }
    for (new u = 0; u < UVOZ_MAX; u++)
    {
        if (!gUvoz[u][uvIma]) continue;
        format(row, sizeof row, T(playerid, "Uvoz [%d] %s - obrisi"), gUvoz[u][uvDb], CestaZona(gUvoz[u][uvX], gUvoz[u][uvY]));
        DodajStupac(list, len, row);
        gApDroga[playerid][n++] = DOB_MAX + u;
    }
    return PrikaziDialog(playerid, DLG_APANEL_DROGA, DIALOG_STYLE_LIST, T(playerid, "Droga"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_droga(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new p = 0;
        while (p < DOB_MAX && gDob[p][dbIma]) p++;
        if (p == DOB_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je najveci broj dobavljaca.")), ShowDrogaAdmin(playerid);
        GetPlayerPos(playerid, gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ]);
        GetPlayerFacingAngle(playerid, gDob[p][dbA]);
        gDob[p][dbInt] = GetPlayerInterior(playerid);
        gDob[p][dbSvijet] = GetPlayerVirtualWorld(playerid);
        gDob[p][dbDan] = 0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO drug_suppliers (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", gDob[p][dbX], gDob[p][dbY], gDob[p][dbZ], gDob[p][dbA], gDob[p][dbInt], gDob[p][dbSvijet]));
        gDob[p][dbDb] = ZadnjiId();
        DobStvori(p);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Dobavljac postavljen."));
        return ShowDrogaAdmin(playerid);
    }
    if (listitem == 1)
    {
        new u = 0;
        while (u < UVOZ_MAX && gUvoz[u][uvIma]) u++;
        if (u == UVOZ_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Tocka uvoza se postavlja samo izvana (najvise %d)."), UVOZ_MAX), ShowDrogaAdmin(playerid);
        GetPlayerPos(playerid, gUvoz[u][uvX], gUvoz[u][uvY], gUvoz[u][uvZ]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO drug_imports (x, y, z) VALUES (%f, %f, %f)", gUvoz[u][uvX], gUvoz[u][uvY], gUvoz[u][uvZ]));
        gUvoz[u][uvDb] = ZadnjiId();
        gUvoz[u][uvIma] = true;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tocka uvoza postavljena."));
        return ShowDrogaAdmin(playerid);
    }
    new i = listitem - 2;
    if (i < 0 || i >= sizeof gApDroga[]) return 1;
    new k = gApDroga[playerid][i];
    if (k < DOB_MAX)
    {
        if (!gDob[k][dbIma]) return ShowDrogaAdmin(playerid);
        DestroyActor(gDob[k][dbAktor]);
        DestroyDynamic3DTextLabel(gDob[k][dbTekst]);
        gDob[k][dbIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM drug_suppliers WHERE id = %d", gDob[k][dbDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Dobavljac obrisan."));
    }
    else
    {
        k -= DOB_MAX;
        if (!gUvoz[k][uvIma]) return ShowDrogaAdmin(playerid);
        gUvoz[k][uvIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM drug_imports WHERE id = %d", gUvoz[k][uvDb]));
        for (new p = 0; p < MAX_PLAYERS; p++) if (gDrogaUvoz[p] == k) DrogaUvozIstek(p, gDrogaUvozToken[p]);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tocka uvoza obrisana."));
    }
    return ShowDrogaAdmin(playerid);
}
