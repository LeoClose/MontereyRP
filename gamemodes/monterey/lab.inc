#define LAB_MJESTA_MAX      64
#define LAB_DOMET           1.8
#define LAB_SANDUK_SLOT     2
#define LAB_SANDUK_MODEL    1271
#define LAB_POD_MAX         48
#define LAB_SANDUK_PASTA    50
#define LAB_SANDUK_KEM      10
#define LAB_CIJENA_PASTA    2000
#define LAB_CIJENA_KEM      4000
#define LAB_NARUDZBA_MAX    5
#define LAB_KOKA_PASTA      50
#define LAB_KOKA_KEM        5
#define LAB_KOKA_RUNDI      5
#define LAB_KOKA_MS         50
#define LAB_METH_KEM        3
#define LAB_METH_PRINOS     30
#define LAB_METH_MS         100
#define LAB_METH_TRAJANJE   30000
#define LAB_METH_EKSPLOZIJA 1500
#define LAB_METH_OD         55.0
#define LAB_METH_DO         75.0
#define LAB_BAR_X           240.0
#define LAB_BAR_Y           330.0
#define LAB_BAR_W           160.0
#define LAB_BOJA            0x8E44ADE6
#define LUKA_DOMET          4.0
#define LUKA_NARUDZBI       16

enum { LM_SANDUCI, LM_KOKAIN, LM_METH, LM_SKLADISTE }
enum { LS_PASTA, LS_KEM, LS_KOKAIN, LS_METH }
enum E_LAB_MJESTO { bool:lmIma, lmDb, lmTer, lmTip, Float:lmX, Float:lmY, Float:lmZ, STREAMER_TAG_3D_TEXT_LABEL:lmTekst, STREAMER_TAG_OBJECT:lmObjekt }
enum { LIGRA_NEMA, LIGRA_METH, LIGRA_KOKA }

static const gLabMjestoIme[][] = { "Mjesto za sanduke", "Stol za kokain", "Stol za metamfetamin", "Skladiste laboratorija" };
static const gLabStupac[][] = { "pasta", "chemicals", "cocaine", "meth" };
static gLabMjesto[LAB_MJESTA_MAX][E_LAB_MJESTO], gLabSavjet[MAX_PLAYERS];
static gLabIgra[MAX_PLAYERS], gLabToken[MAX_PLAYERS], gLabOd[MAX_PLAYERS], PlayerText:gLabTd[MAX_PLAYERS][5];
static Float:gMethTemp[MAX_PLAYERS], Float:gMethBrzina[MAX_PLAYERS], gMethDobro[MAX_PLAYERS], gMethUkupno[MAX_PLAYERS], gMethVruce[MAX_PLAYERS];
static Float:gKokaPoz[MAX_PLAYERS], Float:gKokaSmjer[MAX_PLAYERS], Float:gKokaZona[MAX_PLAYERS], gKokaRunda[MAX_PLAYERS], gKokaPogodak[MAX_PLAYERS];
enum E_LAB_POD { bool:lpIma, lpItem, STREAMER_TAG_OBJECT:lpObjekt, Float:lpX, Float:lpY, Float:lpZ, lpSvijet, lpInt }
static gLabPod[LAB_POD_MAX][E_LAB_POD];
static Float:gLuka[3], bool:gLukaIma, STREAMER_TAG_3D_TEXT_LABEL:gLukaTekst, STREAMER_TAG_OBJECT:gLukaSanduci[LUKA_NARUDZBI];

InitLab()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS lab_spots (id INTEGER PRIMARY KEY AUTOINCREMENT, ter INTEGER NOT NULL, tip INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS lab_stock (org INTEGER PRIMARY KEY, pasta INTEGER NOT NULL DEFAULT 0, chemicals INTEGER NOT NULL DEFAULT 0, cocaine INTEGER NOT NULL DEFAULT 0, meth INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_orders (id INTEGER PRIMARY KEY AUTOINCREMENT, org INTEGER NOT NULL, tip INTEGER NOT NULL, crates INTEGER NOT NULL, remaining INTEGER NOT NULL, slot INTEGER NOT NULL, state INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS drug_dock (id INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO lab_stock (org) SELECT org FROM drug_labs WHERE cocaine > 0"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE lab_stock SET cocaine = cocaine + (SELECT cocaine FROM drug_labs WHERE drug_labs.org = lab_stock.org) WHERE org IN (SELECT org FROM drug_labs WHERE cocaine > 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE drug_labs SET cocaine = 0"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, ter, tip, x, y, z FROM lab_spots ORDER BY id LIMIT %d", LAB_MJESTA_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
    {
        gLabMjesto[k][lmDb] = DB_GetFieldInt(res, 0);
        gLabMjesto[k][lmTer] = DB_GetFieldInt(res, 1);
        gLabMjesto[k][lmTip] = DB_GetFieldInt(res, 2);
        gLabMjesto[k][lmX] = DB_GetFieldFloat(res, 3);
        gLabMjesto[k][lmY] = DB_GetFieldFloat(res, 4);
        gLabMjesto[k][lmZ] = DB_GetFieldFloat(res, 5);
        LabMjestoStvori(k);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM drug_dock WHERE id = 1");
    if (DB_GetRowCount(res))
    {
        for (new i = 0; i < 3; i++) gLuka[i] = DB_GetFieldFloat(res, i);
        LukaStvori();
    }
    DB_FreeResultSet(res);
    LukaSanduci();
}

static LabMjestoStvori(k)
{
    gLabMjesto[k][lmIma] = true;
    gLabMjesto[k][lmTekst] = CreateDynamic3DTextLabel(gLabMjestoIme[gLabMjesto[k][lmTip]], LAB_BOJA, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ] + 0.6, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gLabMjesto[k][lmTekst], LAB_DOMET);
    gLabMjesto[k][lmObjekt] = gLabMjesto[k][lmTip] == LM_SANDUCI ? CreateDynamicObject(1318, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ] + 0.2, 0.0, 0.0, 0.0, 0, 0) : STREAMER_TAG_OBJECT:0;
}

static LukaStvori()
{
    gLukaIma = true;
    gLukaTekst = CreateDynamic3DTextLabel("Luka - preuzimanje posiljki", LAB_BOJA, gLuka[0], gLuka[1], gLuka[2] + 0.8, 25.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gLukaTekst, LUKA_DOMET);
}

static LabStanje(org, &pasta, &kem, &kokain, &meth)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT pasta, chemicals, cocaine, meth FROM lab_stock WHERE org = %d", org);
    pasta = kem = kokain = meth = 0;
    if (DB_GetRowCount(res))
    {
        pasta = DB_GetFieldInt(res, 0);
        kem = DB_GetFieldInt(res, 1);
        kokain = DB_GetFieldInt(res, 2);
        meth = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
}

static LabDodaj(org, stupac, n)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO lab_stock (org) VALUES (%d)", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE lab_stock SET %s = MAX(0, %s + %d) WHERE org = %d", gLabStupac[stupac], gLabStupac[stupac], n, org));
}

static bool:LabMafija(playerid)
{
    return gOrg[playerid] && GetOrgTip(gOrg[playerid]) == 1;
}

static LabMjestoBlizu(playerid)
{
    new ter = LabMafija(playerid) ? TerLabDb(playerid) : 0;
    if (!ter) return -1;
    for (new k = 0; k < LAB_MJESTA_MAX; k++)
        if (gLabMjesto[k][lmIma] && gLabMjesto[k][lmTer] == ter && IsPlayerInRangeOfPoint(playerid, LAB_DOMET, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ])) return k;
    return -1;
}

bool:LabSanduk(item)
{
    return item == ITEM_SANDUK_KOKA || item == ITEM_SANDUK_KEM;
}

static LabNosi(playerid)
{
    if (gInv[playerid][ITEM_SANDUK_KOKA]) return ITEM_SANDUK_KOKA;
    if (gInv[playerid][ITEM_SANDUK_KEM]) return ITEM_SANDUK_KEM;
    return -1;
}

static LabSandukPrikazi(playerid, bool:nosi)
{
    if (nosi)
    {
        SetPlayerAttachedObject(playerid, LAB_SANDUK_SLOT, LAB_SANDUK_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.6, 0.6, 0.6);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        return;
    }
    RemovePlayerAttachedObject(playerid, LAB_SANDUK_SLOT);
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
}

static LabSandukBaci(playerid)
{
    new item = LabNosi(playerid);
    if (item == -1) return 0;
    new k = 0;
    while (k < LAB_POD_MAX && gLabPod[k][lpIma]) k++;
    if (k == LAB_POD_MAX)
    {
        k = 0;
        DestroyDynamicObject(gLabPod[k][lpObjekt]);
    }
    new Float:a;
    GetPlayerPos(playerid, gLabPod[k][lpX], gLabPod[k][lpY], gLabPod[k][lpZ]);
    GetPlayerFacingAngle(playerid, a);
    gLabPod[k][lpX] -= 0.7 * floatsin(a, degrees);
    gLabPod[k][lpY] += 0.7 * floatcos(a, degrees);
    gLabPod[k][lpSvijet] = GetPlayerVirtualWorld(playerid);
    gLabPod[k][lpInt] = GetPlayerInterior(playerid);
    gLabPod[k][lpItem] = item;
    gLabPod[k][lpObjekt] = CreateDynamicObject(LAB_SANDUK_MODEL, gLabPod[k][lpX], gLabPod[k][lpY], gLabPod[k][lpZ] - 0.65, 0.0, 0.0, a, gLabPod[k][lpSvijet], gLabPod[k][lpInt]);
    gLabPod[k][lpIma] = true;
    GiveItem(playerid, item, -1);
    LabSandukPrikazi(playerid, false);
    return 1;
}

static LabPodBlizu(playerid)
{
    new svijet = GetPlayerVirtualWorld(playerid);
    for (new k = 0; k < LAB_POD_MAX; k++)
        if (gLabPod[k][lpIma] && gLabPod[k][lpSvijet] == svijet && IsPlayerInRangeOfPoint(playerid, 1.6, gLabPod[k][lpX], gLabPod[k][lpY], gLabPod[k][lpZ])) return k;
    return -1;
}

static LabPodUzmi(playerid, k)
{
    if (!GiveItem(playerid, gLabPod[k][lpItem], 1)) return 0;
    DestroyDynamicObject(gLabPod[k][lpObjekt]);
    gLabPod[k][lpIma] = false;
    ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, false, false, false, 0);
    LabSandukPrikazi(playerid, true);
    if (LabMafija(playerid)) LabHqCilj(playerid);
    return 1;
}

LabSandukGepek(playerid, bool:stavi)
{
    LabSandukPrikazi(playerid, !stavi);
}

LabGepekAlt(playerid, v)
{
    new item = LabNosi(playerid);
    if (item != -1 && GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY && ImaKljuc(playerid, v))
    {
        if (GepekPrebaci(playerid, v, item, 1, true) != GEPEK_OK) return 0;
        PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s stavlja sanduk u prtljaznik.", GetRPIme(playerid), "", 1);
        return 1;
    }
    if (item != -1 || !LabMafija(playerid) || gInv[playerid][ITEM_KUTIJA]) return 0;
    new uzmi = GepekImaItem(v, ITEM_SANDUK_KOKA) ? ITEM_SANDUK_KOKA : GepekImaItem(v, ITEM_SANDUK_KEM) ? ITEM_SANDUK_KEM : -1;
    if (uzmi == -1 || GepekPrebaci(playerid, v, uzmi, 1, false) != GEPEK_OK) return 0;
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s vadi sanduk iz prtljaznika.", GetRPIme(playerid), "", 1);
    return 1;
}

static LabHqCilj(playerid)
{
    for (new k = 0; k < LAB_MJESTA_MAX; k++)
    {
        if (!gLabMjesto[k][lmIma] || gLabMjesto[k][lmTip] != LM_SANDUCI || TerOrgPoDb(gLabMjesto[k][lmTer]) != gOrg[playerid]) continue;
        SetPlayerCheckpoint(playerid, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ], LAB_DOMET);
        return;
    }
    ObavijestGreska(playerid, T(playerid, "Vasa organizacija nema laboratorij. Osvojite teritorij laboratorija."));
}

static LukaSanduci()
{
    for (new k = 0; k < LUKA_NARUDZBI; k++) if (gLukaSanduci[k]) DestroyDynamicObject(gLukaSanduci[k]), gLukaSanduci[k] = STREAMER_TAG_OBJECT:0;
    if (!gLukaIma) return;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT remaining FROM drug_orders WHERE state = 1 AND remaining > 0 LIMIT %d", LUKA_NARUDZBI);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++, DB_SelectNextRow(res))
        gLukaSanduci[k] = CreateDynamicObject(LAB_SANDUK_MODEL, gLuka[0] + float(k % 4) * 0.9 - 1.35, gLuka[1] + float(k / 4) * 0.9, gLuka[2] - 0.65, 0.0, 0.0, 0.0, 0, 0);
    DB_FreeResultSet(res);
}

LukaPristao(slot)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT DISTINCT org FROM drug_orders WHERE state = 1 AND remaining > 0");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new org = DB_GetFieldInt(res, 0);
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] == org) ObavijestGreska(i, T(i, "Niste preuzeli posiljku na vrijeme. Brod ju je odnio."));
    }
    DB_FreeResultSet(res);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM drug_orders WHERE state = 1"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE drug_orders SET state = 1 WHERE state = 0 AND slot <= %d", slot));
    res = DB_ExecuteQuery(gDB, "SELECT org, SUM(remaining) FROM drug_orders WHERE state = 1 AND remaining > 0 GROUP BY org");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new org = DB_GetFieldInt(res, 0), sanduka = DB_GetFieldInt(res, 1), tekst[144];
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        {
            if (!gUlogiran[i] || gOrg[i] != org) continue;
            format(tekst, sizeof tekst, T(i, "Posiljka je stigla u luku (%d sanduka). Preuzmite je prije dolaska sljedeceg broda."), sanduka);
            ObavijestInfo(i, T(i, "Luka"), tekst);
            if (gLukaIma) SetPlayerCheckpoint(i, gLuka[0], gLuka[1], gLuka[2], LUKA_DOMET);
        }
    }
    DB_FreeResultSet(res);
    LukaSanduci();
}

static LukaAlt(playerid)
{
    if (!gLukaIma || !LabMafija(playerid) || GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, LUKA_DOMET, gLuka[0], gLuka[1], gLuka[2])) return 0;
    if (LabNosi(playerid) != -1 || gInv[playerid][ITEM_KUTIJA]) return ObavijestGreska(playerid, T(playerid, "Vec nosite sanduk.")), 1;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, tip FROM drug_orders WHERE org = %d AND state = 1 AND remaining > 0 LIMIT 1", gOrg[playerid]);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        return ObavijestGreska(playerid, T(playerid, "Vasa organizacija nema posiljku u luci.")), 1;
    }
    new id = DB_GetFieldInt(res, 0), item = DB_GetFieldInt(res, 1) ? ITEM_SANDUK_KEM : ITEM_SANDUK_KOKA;
    DB_FreeResultSet(res);
    if (!GiveItem(playerid, item, 1)) return 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE drug_orders SET remaining = remaining - 1 WHERE id = %d", id));
    ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, false, false, false, 0);
    LabSandukPrikazi(playerid, true);
    LukaSanduci();
    LabHqCilj(playerid);
    ObavijestInfo(playerid, T(playerid, "Luka"), T(playerid, "Preuzeli ste sanduk. Stavite ga u vozilo (ALT kod prtljaznika) i odvezite u laboratorij vase organizacije."));
    return 1;
}

static LabSkladiste(playerid)
{
    new pasta, kem, kokain, meth, tekst[512];
    LabStanje(gOrg[playerid], pasta, kem, kokain, meth);
    format(tekst, sizeof tekst, T(playerid, "{AAAAAA}Zaliha: %d g koka paste, %d l kemikalija\n{2ECC71}Uzmi kokain (%d g)\n{2ECC71}Uzmi metamfetamin (%d g)"), pasta, kem, kokain, meth);
    return PrikaziDialog(playerid, DLG_LAB, DIALOG_STYLE_LIST, T(playerid, "Skladiste laboratorija"), tekst, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:lab(playerid, response, listitem)
{
    new k = LabMjestoBlizu(playerid);
    if (!response || k == -1 || gLabMjesto[k][lmTip] != LM_SKLADISTE || listitem < 1) return 1;
    new pasta, kem, kokain, meth, item = listitem == 1 ? ITEM_KOKAIN : ITEM_METH, ima;
    LabStanje(gOrg[playerid], pasta, kem, kokain, meth);
    ima = listitem == 1 ? kokain : meth;
    new n = gItems[item][ITEM_MAX] - gInv[playerid][item];
    if (n > ima) n = ima;
    if (n <= 0) return ObavijestGreska(playerid, T(playerid, "Nema nista za uzeti ili ne mozete nositi vise.")), LabSkladiste(playerid);
    GiveItem(playerid, item, n);
    LabDodaj(gOrg[playerid], listitem == 1 ? LS_KOKAIN : LS_METH, -n);
    new poruka[96];
    format(poruka, sizeof poruka, T(playerid, "Uzeli ste %d g %s."), n, T(playerid, gItems[item][ITEM_NAME]));
    ObavijestInfo(playerid, T(playerid, "Laboratorij"), poruka);
    return LabSkladiste(playerid);
}

static PlayerText:LabKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = TdSlika(playerid, x, y, w, h, boja);
    PlayerTextDrawShow(playerid, td);
    return td;
}

static LabIgraPocni(playerid, igra)
{
    gLabIgra[playerid] = igra;
    gLabOd[playerid] = GetTickCount();
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "BD_FIRE", "wash_up", 4.1, true, false, false, false, 0);
    gLabTd[playerid][0] = LabKutija(playerid, LAB_BAR_X - 3.0, LAB_BAR_Y - 3.0, LAB_BAR_W + 6.0, 16.0, 0x000000C0);
    gLabTd[playerid][1] = LabKutija(playerid, LAB_BAR_X, LAB_BAR_Y, LAB_BAR_W, 10.0, 0x3A3A3AFF);
}

static LabIgraKraj(playerid)
{
    gLabIgra[playerid] = LIGRA_NEMA;
    gLabToken[playerid]++;
    for (new i = 0; i < sizeof gLabTd[]; i++) PlayerTextDrawDestroy(playerid, gLabTd[playerid][i]);
    if (!IsPlayerConnected(playerid)) return;
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
}

static MethPocni(playerid)
{
    new pasta, kem, kokain, meth;
    LabStanje(gOrg[playerid], pasta, kem, kokain, meth);
    if (kem < LAB_METH_KEM)
    {
        new tekst[96];
        format(tekst, sizeof tekst, T(playerid, "U laboratoriju treba biti barem %d l kemikalija."), LAB_METH_KEM);
        return ObavijestGreska(playerid, tekst);
    }
    LabDodaj(gOrg[playerid], LS_KEM, -LAB_METH_KEM);
    gMethTemp[playerid] = 40.0;
    gMethBrzina[playerid] = 0.0;
    gMethDobro[playerid] = 0;
    gMethUkupno[playerid] = 0;
    gMethVruce[playerid] = 0;
    LabIgraPocni(playerid, LIGRA_METH);
    gLabTd[playerid][2] = LabKutija(playerid, LAB_BAR_X + LAB_METH_OD / 100.0 * LAB_BAR_W, LAB_BAR_Y, (LAB_METH_DO - LAB_METH_OD) / 100.0 * LAB_BAR_W, 10.0, 0x2ECC71AA);
    gLabTd[playerid][3] = LabKutija(playerid, LAB_BAR_X + 85.0 / 100.0 * LAB_BAR_W, LAB_BAR_Y, 15.0 / 100.0 * LAB_BAR_W, 10.0, 0xC0392BAA);
    gLabTd[playerid][4] = LabKutija(playerid, LAB_BAR_X, LAB_BAR_Y - 2.0, 2.0, 14.0, 0xFFFFFFFF);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), LAB_BOJA, T(playerid, "Drzite temperaturu u zelenom.~n~W - grijanje, S - hladjenje, ENTER - odustani.~n~Crveno predugo = eksplozija!"), 0);
    SetTimerEx("LabMethTick", LAB_METH_MS, false, "ii", playerid, ++gLabToken[playerid]);
    return 1;
}

forward LabMethTick(playerid, token);
public LabMethTick(playerid, token)
{
    if (token != gLabToken[playerid] || gLabIgra[playerid] != LIGRA_METH) return;
    if (!IsPlayerConnected(playerid))
    {
        LabIgraKraj(playerid);
        return;
    }
    new KEY:tipke, gore, strana;
    GetPlayerKeys(playerid, tipke, gore, strana);
    gMethBrzina[playerid] = gMethBrzina[playerid] * 0.85 + float(random(21) - 10) / 100.0 + (gore < 0 ? 0.15 : gore > 0 ? -0.15 : 0.03);
    if (gMethBrzina[playerid] > 1.5) gMethBrzina[playerid] = 1.5;
    if (gMethBrzina[playerid] < -1.5) gMethBrzina[playerid] = -1.5;
    gMethTemp[playerid] += gMethBrzina[playerid];
    if (gMethTemp[playerid] < 0.0) gMethTemp[playerid] = 0.0, gMethBrzina[playerid] = 0.0;
    if (gMethTemp[playerid] > 100.0) gMethTemp[playerid] = 100.0, gMethBrzina[playerid] = 0.0;
    gMethUkupno[playerid]++;
    if (gMethTemp[playerid] >= LAB_METH_OD && gMethTemp[playerid] <= LAB_METH_DO) gMethDobro[playerid]++;
    gMethVruce[playerid] = gMethTemp[playerid] >= 85.0 ? gMethVruce[playerid] + LAB_METH_MS : 0;
    PlayerTextDrawHide(playerid, gLabTd[playerid][4]);
    PlayerTextDrawSetPos(playerid, gLabTd[playerid][4], LAB_BAR_X + gMethTemp[playerid] / 100.0 * LAB_BAR_W - 1.0, LAB_BAR_Y - 2.0);
    PlayerTextDrawShow(playerid, gLabTd[playerid][4]);
    if (gMethVruce[playerid] >= LAB_METH_EKSPLOZIJA)
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        LabIgraKraj(playerid);
        CreateExplosion(x, y, z, 12, 3.0);
        IgraObavijest(playerid, T(playerid, "Laboratorij"), OBAVIJEST_GRESKA, T(playerid, "Smjesa je eksplodirala! Kemikalije su izgubljene."));
        return;
    }
    if (GetTickCount() - gLabOd[playerid] < LAB_METH_TRAJANJE)
    {
        SetTimerEx("LabMethTick", LAB_METH_MS, false, "ii", playerid, token);
        return;
    }
    new kvaliteta = gMethUkupno[playerid] ? gMethDobro[playerid] * 100 / gMethUkupno[playerid] : 0, grama = kvaliteta < 30 ? 0 : LAB_METH_PRINOS * kvaliteta / 100, poruka[128];
    LabIgraKraj(playerid);
    if (grama > 0) LabDodaj(gOrg[playerid], LS_METH, grama);
    format(poruka, sizeof poruka, T(playerid, "Kvaliteta: %d%%~n~%d g metamfetamina je u skladistu laboratorija."), kvaliteta, grama);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), LAB_BOJA, poruka);
}

static KokaZona(playerid)
{
    gKokaZona[playerid] = float(10 + random(66));
    PlayerTextDrawHide(playerid, gLabTd[playerid][2]);
    PlayerTextDrawSetPos(playerid, gLabTd[playerid][2], LAB_BAR_X + gKokaZona[playerid] / 100.0 * LAB_BAR_W, LAB_BAR_Y);
    PlayerTextDrawShow(playerid, gLabTd[playerid][2]);
}

static KokaPocni(playerid)
{
    new pasta, kem, kokain, meth;
    LabStanje(gOrg[playerid], pasta, kem, kokain, meth);
    if (pasta < LAB_KOKA_PASTA || kem < LAB_KOKA_KEM)
    {
        new tekst[128];
        format(tekst, sizeof tekst, T(playerid, "U laboratoriju treba biti barem %d g koka paste i %d l kemikalija."), LAB_KOKA_PASTA, LAB_KOKA_KEM);
        return ObavijestGreska(playerid, tekst);
    }
    LabDodaj(gOrg[playerid], LS_PASTA, -LAB_KOKA_PASTA);
    LabDodaj(gOrg[playerid], LS_KEM, -LAB_KOKA_KEM);
    gKokaPoz[playerid] = 0.0;
    gKokaSmjer[playerid] = 1.6;
    gKokaRunda[playerid] = 0;
    gKokaPogodak[playerid] = 0;
    LabIgraPocni(playerid, LIGRA_KOKA);
    gLabTd[playerid][2] = LabKutija(playerid, LAB_BAR_X, LAB_BAR_Y, 15.0 / 100.0 * LAB_BAR_W, 10.0, 0x2ECC71AA);
    gLabTd[playerid][3] = LabKutija(playerid, LAB_BAR_X, LAB_BAR_Y, 0.0, 0.0, 0);
    gLabTd[playerid][4] = LabKutija(playerid, LAB_BAR_X, LAB_BAR_Y - 2.0, 2.0, 14.0, 0xFFFFFFFF);
    KokaZona(playerid);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), LAB_BOJA, T(playerid, "Pritisnite SPACE kad je oznaka u zelenom.~n~5 pokusaja, svaki je brzi.~n~ENTER - odustani."), 0);
    SetTimerEx("LabKokaTick", LAB_KOKA_MS, false, "ii", playerid, ++gLabToken[playerid]);
    return 1;
}

forward LabKokaTick(playerid, token);
public LabKokaTick(playerid, token)
{
    if (token != gLabToken[playerid] || gLabIgra[playerid] != LIGRA_KOKA) return;
    if (!IsPlayerConnected(playerid))
    {
        LabIgraKraj(playerid);
        return;
    }
    gKokaPoz[playerid] += gKokaSmjer[playerid];
    if (gKokaPoz[playerid] >= 100.0) gKokaPoz[playerid] = 100.0, gKokaSmjer[playerid] = -gKokaSmjer[playerid];
    if (gKokaPoz[playerid] <= 0.0) gKokaPoz[playerid] = 0.0, gKokaSmjer[playerid] = -gKokaSmjer[playerid];
    PlayerTextDrawHide(playerid, gLabTd[playerid][4]);
    PlayerTextDrawSetPos(playerid, gLabTd[playerid][4], LAB_BAR_X + gKokaPoz[playerid] / 100.0 * LAB_BAR_W - 1.0, LAB_BAR_Y - 2.0);
    PlayerTextDrawShow(playerid, gLabTd[playerid][4]);
    if (GetTickCount() - gLabOd[playerid] > 40000)
    {
        KokaRunda(playerid, false);
        return;
    }
    SetTimerEx("LabKokaTick", LAB_KOKA_MS, false, "ii", playerid, token);
}

static KokaRunda(playerid, bool:pritisnuo)
{
    if (pritisnuo && gKokaPoz[playerid] >= gKokaZona[playerid] && gKokaPoz[playerid] <= gKokaZona[playerid] + 15.0) gKokaPogodak[playerid]++;
    if (pritisnuo) PlayerPlaySound(playerid, gKokaPoz[playerid] >= gKokaZona[playerid] && gKokaPoz[playerid] <= gKokaZona[playerid] + 15.0 ? 1056 : 1055, 0.0, 0.0, 0.0);
    if (pritisnuo && ++gKokaRunda[playerid] < LAB_KOKA_RUNDI)
    {
        gKokaSmjer[playerid] = (gKokaSmjer[playerid] < 0.0 ? -1.0 : 1.0) * (floatabs(gKokaSmjer[playerid]) + 0.6);
        KokaZona(playerid);
        return;
    }
    new grama = LAB_KOKA_PASTA * gKokaPogodak[playerid] / LAB_KOKA_RUNDI, poruka[128];
    LabIgraKraj(playerid);
    if (grama > 0) LabDodaj(gOrg[playerid], LS_KOKAIN, grama);
    format(poruka, sizeof poruka, T(playerid, "Pogodaka: %d/%d~n~%d g kokaina je u skladistu laboratorija."), gKokaPogodak[playerid], LAB_KOKA_RUNDI, grama);
    IgraObavijest(playerid, T(playerid, "Laboratorij"), LAB_BOJA, poruka);
}

bool:LabTipke(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (gLabIgra[playerid] == LIGRA_NEMA) return false;
    if ((newkeys & KEY_SECONDARY_ATTACK) && !(oldkeys & KEY_SECONDARY_ATTACK))
    {
        LabIgraKraj(playerid);
        IgraObavijest(playerid, T(playerid, "Laboratorij"), OBAVIJEST_GRESKA, T(playerid, "Odustali ste. Sastojci su izgubljeni."));
        return true;
    }
    if (gLabIgra[playerid] == LIGRA_KOKA && (newkeys & KEY_SPRINT) && !(oldkeys & KEY_SPRINT)) KokaRunda(playerid, true);
    return true;
}

bool:LabAktivan(playerid)
{
    return gLabIgra[playerid] != LIGRA_NEMA;
}

LabConnect(playerid)
{
    gLabIgra[playerid] = LIGRA_NEMA;
    gLabToken[playerid]++;
    gLabSavjet[playerid] = -1;
}

LabOdlazi(playerid)
{
    LabSandukBaci(playerid);
    if (gLabIgra[playerid] != LIGRA_NEMA) LabIgraKraj(playerid);
}

LabAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || gLabIgra[playerid] != LIGRA_NEMA) return 0;
    if (LukaAlt(playerid)) return 1;
    new k = LabMjestoBlizu(playerid);
    if (k == -1 || (gLabMjesto[k][lmTip] == LM_SANDUCI && LabNosi(playerid) == -1))
    {
        if (LabNosi(playerid) != -1)
        {
            LabSandukBaci(playerid);
            ApplyAnimation(playerid, "CARRY", "putdwn", 4.1, false, false, false, false, 0);
            return 1;
        }
        new p = LabPodBlizu(playerid);
        if (p != -1 && !gInv[playerid][ITEM_KUTIJA]) return LabPodUzmi(playerid, p);
        if (k == -1) return 0;
    }
    switch (gLabMjesto[k][lmTip])
    {
        case LM_SANDUCI:
        {
            new item = LabNosi(playerid), tekst[128];
            if (item == -1) return ObavijestGreska(playerid, T(playerid, "Ne nosite sanduk.")), 1;
            GiveItem(playerid, item, -1);
            LabSandukPrikazi(playerid, false);
            ApplyAnimation(playerid, "CARRY", "putdwn", 4.1, false, false, false, false, 0);
            if (item == ITEM_SANDUK_KOKA) LabDodaj(gOrg[playerid], LS_PASTA, LAB_SANDUK_PASTA);
            else LabDodaj(gOrg[playerid], LS_KEM, LAB_SANDUK_KEM);
            DisablePlayerCheckpoint(playerid);
            format(tekst, sizeof tekst, item == ITEM_SANDUK_KOKA ? T(playerid, "Ostavili ste sanduk: +%d g koka paste u laboratoriju.") : T(playerid, "Ostavili ste sanduk: +%d l kemikalija u laboratoriju."), item == ITEM_SANDUK_KOKA ? LAB_SANDUK_PASTA : LAB_SANDUK_KEM);
            ObavijestInfo(playerid, T(playerid, "Laboratorij"), tekst);
        }
        case LM_KOKAIN: KokaPocni(playerid);
        case LM_METH: MethPocni(playerid);
        case LM_SKLADISTE: LabSkladiste(playerid);
    }
    return 1;
}

LabTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i]) continue;
        if (LabNosi(i) != -1 && IsPlayerInAnyVehicle(i))
        {
            RemovePlayerFromVehicle(i);
            ObavijestGreska(i, T(i, "S sandukom ne mozete u vozilo. Stavite ga u prtljaznik (ALT kod prtljaznika)."));
        }
        else if (LabNosi(i) != -1 && IsRanjen(i))
        {
            LabSandukBaci(i);
            ObavijestGreska(i, T(i, "Ispustili ste sanduk."));
        }
        else if (LabNosi(i) != -1 && GetPlayerState(i) == PLAYER_STATE_ONFOOT && GetPlayerSpecialAction(i) != SPECIAL_ACTION_CARRY) LabSandukPrikazi(i, true);
        new k = gLabIgra[i] == LIGRA_NEMA ? LabMjestoBlizu(i) : -1;
        if (k == -1 && gLabIgra[i] == LIGRA_NEMA && LabNosi(i) == -1 && LabPodBlizu(i) != -1) k = 1000;
        if (k == 1000 && gLabSavjet[i] != 1000) Savjet(i, T(i, "~y~ALT~w~ - podigni sanduk"), 4000);
        if (k != -1 && k != 1000 && k != gLabSavjet[i])
        {
            switch (gLabMjesto[k][lmTip])
            {
                case LM_SANDUCI: Savjet(i, T(i, "~y~ALT~w~ - ostavi sanduk"), 4000);
                case LM_KOKAIN: Savjet(i, T(i, "~y~ALT~w~ - preradi kokain"), 4000);
                case LM_METH: Savjet(i, T(i, "~y~ALT~w~ - kuhaj metamfetamin"), 4000);
                case LM_SKLADISTE: Savjet(i, T(i, "~y~ALT~w~ - skladiste laboratorija"), 4000);
            }
        }
        gLabSavjet[i] = k;
    }
}

CMD:narudzba(playerid, params[])
{
    if (!gUlogiran[playerid] || !LabMafija(playerid) || GetPlayerVirtualWorld(playerid) != HQ_SVIJET + gOrg[playerid]) return ObavijestGreska(playerid, T(playerid, "Narudzbe se rade u HQ-u mafije.")), 1;
    if (!(gOrgPerms[playerid] & PERM_LEADER)) return ObavijestGreska(playerid, T(playerid, "Samo vodja organizacije moze naruciti robu.")), 1;
    new tekst[256];
    format(tekst, sizeof tekst, T(playerid, "Sanduk koka paste (%d g) - $%d\nSanduk kemikalija (%d l) - $%d"), LAB_SANDUK_PASTA, LAB_CIJENA_PASTA, LAB_SANDUK_KEM, LAB_CIJENA_KEM);
    return PrikaziDialog(playerid, DLG_NARUDZBA, DIALOG_STYLE_LIST, T(playerid, "Narudzba brodom"), tekst, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static gNarudzbaTip[MAX_PLAYERS];

DLG:narudzba(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem > 1) return 1;
    gNarudzbaTip[playerid] = listitem;
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Koliko sanduka narucujete? (1-%d)"), LAB_NARUDZBA_MAX);
    return PrikaziDialog(playerid, DLG_NARUDZBA_KOL, DIALOG_STYLE_INPUT, T(playerid, "Narudzba brodom"), tekst, T(playerid, "Naruci"), T(playerid, "Natrag"));
}

DLG:narudzba_kol(playerid, response, const inputtext[])
{
    new prazno[1];
    if (!response) return cmd_narudzba(playerid, prazno);
    if (!LabMafija(playerid) || !(gOrgPerms[playerid] & PERM_LEADER) || GetPlayerVirtualWorld(playerid) != HQ_SVIJET + gOrg[playerid]) return 1;
    new n = strval(inputtext), tip = gNarudzbaTip[playerid], cijena = n * (tip ? LAB_CIJENA_KEM : LAB_CIJENA_PASTA), org = gOrg[playerid], tekst[160];
    if (!ProvjeriBroj(inputtext) || n < 1 || n > LAB_NARUDZBA_MAX) return cmd_narudzba(playerid, prazno);
    if (!SefNovacDodaj(org, -cijena))
    {
        format(tekst, sizeof tekst, T(playerid, "U sefu organizacije nema $%d."), cijena);
        return ObavijestGreska(playerid, tekst);
    }
    new slot = BrodSljedeciDolazak(), minuta = (slot - gettime()) / 60 + 1;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO drug_orders (org, tip, crates, remaining, slot) VALUES (%d, %d, %d, %d, %d)", org, tip, n, n, slot));
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i] || gOrg[i] != org) continue;
        format(tekst, sizeof tekst, T(i, "Narucena roba (%d sanduka, $%d iz sefa) stize brodom za oko %d min."), n, cijena, minuta);
        ObavijestInfo(i, T(i, "Luka"), tekst);
    }
    return 1;
}

LabMjestoDodaj(playerid, tip)
{
    new ter = TerLabTu(playerid);
    if (!ter) return ObavijestGreska(playerid, T(playerid, "Morate stajati na teritoriju laboratorija.")), 0;
    new k = 0;
    while (k < LAB_MJESTA_MAX && gLabMjesto[k][lmIma]) k++;
    if (k == LAB_MJESTA_MAX) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), 0;
    gLabMjesto[k][lmTer] = ter;
    gLabMjesto[k][lmTip] = tip;
    GetPlayerPos(playerid, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO lab_spots (ter, tip, x, y, z) VALUES (%d, %d, %f, %f, %f)", ter, tip, gLabMjesto[k][lmX], gLabMjesto[k][lmY], gLabMjesto[k][lmZ]));
    gLabMjesto[k][lmDb] = ZadnjiId();
    LabMjestoStvori(k);
    return 1;
}

bool:LabMjestoIma(k)
{
    return k >= 0 && k < LAB_MJESTA_MAX && gLabMjesto[k][lmIma];
}

LabMjestoOpis(playerid, k, row[], len = sizeof row)
{
    format(row, len, T(playerid, "Lab [teritorij %d] %s - obrisi"), gLabMjesto[k][lmTer], T(playerid, gLabMjestoIme[gLabMjesto[k][lmTip]]));
}

LabMjestoObrisi(k)
{
    if (!LabMjestoIma(k)) return;
    DestroyDynamic3DTextLabel(gLabMjesto[k][lmTekst]);
    if (gLabMjesto[k][lmObjekt]) DestroyDynamicObject(gLabMjesto[k][lmObjekt]);
    gLabMjesto[k][lmIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM lab_spots WHERE id = %d", gLabMjesto[k][lmDb]));
}

LukaPostavi(playerid)
{
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Luka se postavlja samo izvana.")), 0;
    GetPlayerPos(playerid, gLuka[0], gLuka[1], gLuka[2]);
    if (gLukaIma) DestroyDynamic3DTextLabel(gLukaTekst);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO drug_dock (id, x, y, z) VALUES (1, %f, %f, %f)", gLuka[0], gLuka[1], gLuka[2]));
    LukaStvori();
    LukaSanduci();
    return 1;
}
