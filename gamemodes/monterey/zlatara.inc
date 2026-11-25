#define ZLATARA_MAX             8
#define ZLATARA_SVIJET          9700
#define ZLATARA_INT             1
#define ZLATARA_DOMET           2.0
#define ZLATARA_KASA_DOMET      2.5
#define ZLATARA_SKIN            186
#define ZLATARA_OTKUP_POSTO     50
#define ZLATARA_PLJACKASA       2
#define ZLATARA_PLJACKA_S       240
#define VITRINA_MODEL           19325
#define VITRINA_DOMET           1.8
#define ZLATARA_PLJACKA_PAUZA   3600
#define ZLATARA_WANTED          2
#define PREPRODAVAC_MAX         8
#define PREPRODAVAC_SKIN        28
#define PREPRODAVAC_CIJENA      700
#define ZLATO_POCETNA           150000
#define ZLATO_RAZLIKA           3
#define ZLATO_MINUTA            60
#define ZLATO_MAX_KOM           100
#define SAT_SLOT                6
#define OGRLICA_SLOT            5
#define MODEL_LANAC             -2100
#define MODEL_OGRLICA           -2101

enum E_ZLATARA { bool:zlIma, zlDb, Float:zlX, Float:zlY, Float:zlZ, Float:zlA, bool:zlImaAktor, Float:zlAx, Float:zlAy, Float:zlAz, Float:zlAa, zlAktor, zlPljacka, zlPljackaKraj, zlPljackano,
    STREAMER_TAG_PICKUP:zlPickup, STREAMER_TAG_PICKUP:zlPickupU, STREAMER_TAG_3D_TEXT_LABEL:zlTekst, STREAMER_TAG_3D_TEXT_LABEL:zlTekstU, STREAMER_TAG_3D_TEXT_LABEL:zlTekstA, zlIkona }
enum E_PREPRODAVAC { bool:ppIma, ppDb, Float:ppX, Float:ppY, Float:ppZ, Float:ppA, ppInt, ppSvijet, ppAktor, STREAMER_TAG_3D_TEXT_LABEL:ppTekst }

static const Float:gZlataraUlaz[3] = { 1411.3, -37.25, 1000.95 };
static const gZlataraRoba[][2] = { { ITEM_SAT1, 1500 }, { ITEM_SAT2, 3000 }, { ITEM_SAT3, 8000 }, { ITEM_PRSTEN, 2500 }, { ITEM_LANAC, 4000 }, { ITEM_OGRLICA, 12000 }, { ITEM_SREBRNE_NAOCALE, 400 }, { ITEM_ZLATNE_NAOCALE, 1200 } };

static gZl[ZLATARA_MAX][E_ZLATARA], gPp[PREPRODAVAC_MAX][E_PREPRODAVAC], STREAMER_TAG_AREA:gPpZona[PREPRODAVAC_MAX];
enum { VT_CIJELA, VT_RAZBIJENA, VT_PRAZNA }
static const Float:gVitrinaPoz[][3] = {
    { 1412.370117, -46.308868, 1000.323303 }, { 1410.506103, -48.020141, 1000.323303 }, { 1407.304321, -48.020141, 1000.323303 },
    { 1405.376953, -46.308868, 1000.323303 }, { 1403.643066, -48.020141, 1000.323303 }, { 1400.441284, -48.020141, 1000.323303 },
    { 1398.466430, -46.308868, 1000.323303 }, { 1396.722534, -48.020141, 1000.323303 }, { 1393.520751, -48.020141, 1000.323303 },
    { 1391.995483, -46.308868, 1000.323303 }, { 1390.131469, -48.020141, 1000.323303 }, { 1386.929687, -48.020141, 1000.323303 }
};
static STREAMER_TAG_OBJECT:gVitrina[sizeof gVitrinaPoz], gVitrinaBroj = -1, gZlVitrina[ZLATARA_MAX][sizeof gVitrinaPoz], gZlPlijen[MAX_PLAYERS];
static gZlato, gZlatoSat, gApZlatara[MAX_PLAYERS], gZlataraOdabir[MAX_PLAYERS][sizeof gZlataraRoba], gSatNosi[MAX_PLAYERS], gOgrNosi[MAX_PLAYERS], gZlataraPoruka[MAX_PLAYERS];

static ZlatoNovo(cijena, pomak)
{
    new nova = cijena + cijena * pomak / 1000 + (ZLATO_POCETNA - cijena) / 100;
    return nova < ZLATO_POCETNA / 3 ? ZLATO_POCETNA / 3 : nova > ZLATO_POCETNA * 4 ? ZLATO_POCETNA * 4 : nova;
}

static ZlatoKupnja(komada)
{
    return (komada * (gZlato + gZlato * ZLATO_RAZLIKA / 100) + 99) / 100;
}

static ZlatoProdaja(komada)
{
    return komada * (gZlato - gZlato * ZLATO_RAZLIKA / 100) / 100;
}

static ZlataraCijena(item)
{
    for (new i = 0; i < sizeof gZlataraRoba; i++) if (gZlataraRoba[i][0] == item) return gZlataraRoba[i][1];
    return 0;
}

InitZlatara()
{
    assert(ZlatoNovo(ZLATO_POCETNA, 0) == ZLATO_POCETNA && ZlatoNovo(ZLATO_POCETNA, 10) == ZLATO_POCETNA + 1500 && ZlatoNovo(1, -10) == ZLATO_POCETNA / 3);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS jewelry_shops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, Aktor INTEGER NOT NULL DEFAULT 0, ax REAL NOT NULL DEFAULT 0, ay REAL NOT NULL DEFAULT 0, az REAL NOT NULL DEFAULT 0, aa REAL NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fences (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, world INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS gold_price (id INTEGER PRIMARY KEY, price INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS jewelry_watch (account INTEGER PRIMARY KEY, item INTEGER NOT NULL DEFAULT 0, x REAL NOT NULL DEFAULT 0, y REAL NOT NULL DEFAULT 0, z REAL NOT NULL DEFAULT 0, rx REAL NOT NULL DEFAULT 0, ry REAL NOT NULL DEFAULT 0, rz REAL NOT NULL DEFAULT 0, fit INTEGER NOT NULL DEFAULT 0, skin INTEGER NOT NULL DEFAULT -1, sx REAL NOT NULL DEFAULT 1, sy REAL NOT NULL DEFAULT 1, sz REAL NOT NULL DEFAULT 1, neck INTEGER NOT NULL DEFAULT 0)"));
    AddSimpleModel(-1, 19941, MODEL_LANAC, "nakit/zlatni_lanac.dff", "nakit/zlatni_lanac.txd");
    AddSimpleModel(-1, 19941, MODEL_OGRLICA, "nakit/ogrlica.dff", "nakit/ogrlica.txd");
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT price FROM gold_price WHERE id = 1");
    gZlato = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : ZLATO_POCETNA;
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, Aktor, ax, ay, az, aa FROM jewelry_shops ORDER BY id LIMIT %d", ZLATARA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gZl[s][zlDb] = DB_GetFieldInt(res, 0);
        gZl[s][zlX] = DB_GetFieldFloat(res, 1);
        gZl[s][zlY] = DB_GetFieldFloat(res, 2);
        gZl[s][zlZ] = DB_GetFieldFloat(res, 3);
        gZl[s][zlA] = DB_GetFieldFloat(res, 4);
        ZlataraStvori(s);
        if (!DB_GetFieldInt(res, 5)) continue;
        gZl[s][zlAx] = DB_GetFieldFloat(res, 6);
        gZl[s][zlAy] = DB_GetFieldFloat(res, 7);
        gZl[s][zlAz] = DB_GetFieldFloat(res, 8);
        gZl[s][zlAa] = DB_GetFieldFloat(res, 9);
        ZlataraAktor(s);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, interior, world FROM fences ORDER BY id LIMIT %d", PREPRODAVAC_MAX);
    for (new p = 0, n = DB_GetRowCount(res); p < n; p++, DB_SelectNextRow(res))
    {
        gPp[p][ppDb] = DB_GetFieldInt(res, 0);
        gPp[p][ppX] = DB_GetFieldFloat(res, 1);
        gPp[p][ppY] = DB_GetFieldFloat(res, 2);
        gPp[p][ppZ] = DB_GetFieldFloat(res, 3);
        gPp[p][ppA] = DB_GetFieldFloat(res, 4);
        gPp[p][ppInt] = DB_GetFieldInt(res, 5);
        gPp[p][ppSvijet] = DB_GetFieldInt(res, 6);
        PreprodavacStvori(p);
    }
    DB_FreeResultSet(res);
}

static ZlataraStvori(s)
{
    new svijet = ZLATARA_SVIJET + s;
    gZl[s][zlIma] = true;
    gZl[s][zlPljacka] = 0;
    gZl[s][zlPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ], 0, 0);
    gZl[s][zlIkona] = MapaIkona(gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ], 21);
    gZl[s][zlTekst] = CreateDynamic3DTextLabel("Zlatara", COLOR_ORG, gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gZl[s][zlTekst], ZLATARA_DOMET, SAVJET_ULAZ);
    gZl[s][zlPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gZlataraUlaz[0], gZlataraUlaz[1], gZlataraUlaz[2], svijet, ZLATARA_INT);
    gZl[s][zlTekstU] = CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, gZlataraUlaz[0], gZlataraUlaz[1], gZlataraUlaz[2] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, ZLATARA_INT);
    InterakcijaOznaka(gZl[s][zlTekstU], ZLATARA_DOMET, SAVJET_IZLAZ);
}

static ZlataraAktor(s)
{
    new svijet = ZLATARA_SVIJET + s;
    gZl[s][zlImaAktor] = true;
    gZl[s][zlAktor] = CreateActor(ZLATARA_SKIN, gZl[s][zlAx], gZl[s][zlAy], gZl[s][zlAz], gZl[s][zlAa]);
    SetActorVirtualWorld(gZl[s][zlAktor], svijet);
    SetActorInvulnerable(gZl[s][zlAktor], true);
    gZl[s][zlTekstA] = CreateDynamic3DTextLabel(" ", COLOR_ORG, gZl[s][zlAx], gZl[s][zlAy], gZl[s][zlAz] + 1.2, 5.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, ZLATARA_INT);
    InterakcijaOznaka(gZl[s][zlTekstA]);
}

static ZlataraMakniAktor(s)
{
    if (!gZl[s][zlImaAktor]) return;
    DestroyActor(gZl[s][zlAktor]);
    DestroyDynamic3DTextLabel(gZl[s][zlTekstA]);
    gZl[s][zlImaAktor] = false;
}

static PreprodavacStvori(p)
{
    gPp[p][ppIma] = true;
    gPp[p][ppAktor] = CreateActor(PREPRODAVAC_SKIN, gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ], gPp[p][ppA]);
    SetActorVirtualWorld(gPp[p][ppAktor], gPp[p][ppSvijet]);
    SetActorInvulnerable(gPp[p][ppAktor], true);
    gPpZona[p] = InterakcijaZona(gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ], 2.5, gPp[p][ppSvijet], gPp[p][ppInt]);
}

static ZlataraIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - ZLATARA_SVIJET;
    return s >= 0 && s < ZLATARA_MAX && gZl[s][zlIma] && GetPlayerInterior(playerid) == ZLATARA_INT ? s : -1;
}

static ZlataraVani(playerid, s)
{
    PrebaciIgraca(playerid, 0, 0, gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ], gZl[s][zlA] + 180.0);
}

ZlataraUlaz(playerid)
{
    new s = ZlataraIgraca(playerid);
    if (s != -1)
    {
        if (!IsPlayerInRangeOfPoint(playerid, ZLATARA_DOMET, gZlataraUlaz[0], gZlataraUlaz[1], gZlataraUlaz[2])) return 0;
        ZlataraVani(playerid, s);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < ZLATARA_MAX; s++)
    {
        if (!gZl[s][zlIma] || !IsPlayerInRangeOfPoint(playerid, ZLATARA_DOMET, gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ])) continue;
        PrebaciIgraca(playerid, ZLATARA_INT, ZLATARA_SVIJET + s, gZlataraUlaz[0], gZlataraUlaz[1], gZlataraUlaz[2], 90.0);
        if (gZl[s][zlImaAktor]) AktorKaze(playerid, gZl[s][zlAktor], "Zlatar", "Dobro dosli, slobodno razgledajte.");
        return 1;
    }
    return 0;
}

ZlataraKasa(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    new s = ZlataraIgraca(playerid);
    if (s == -1 || !gZl[s][zlImaAktor] || !IsPlayerInRangeOfPoint(playerid, ZLATARA_KASA_DOMET, gZl[s][zlAx], gZl[s][zlAy], gZl[s][zlAz])) return 0;
    if (gZl[s][zlPljacka]) return 0;
    new list[256], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "Kupi nakit"));
    DodajStupac(list, len, T(playerid, "Prodaj nakit"));
    format(row, sizeof row, T(playerid, "Kupi zlatne poluge ($%d.%02d)"), (gZlato + gZlato * ZLATO_RAZLIKA / 100) / 100, (gZlato + gZlato * ZLATO_RAZLIKA / 100) % 100);
    DodajStupac(list, len, row);
    format(row, sizeof row, T(playerid, "Prodaj zlatne poluge ($%d.%02d, imate %d)"), (gZlato - gZlato * ZLATO_RAZLIKA / 100) / 100, (gZlato - gZlato * ZLATO_RAZLIKA / 100) % 100, gInv[playerid][ITEM_ZLATO]);
    DodajStupac(list, len, row);
    AktorKaze(playerid, gZl[s][zlAktor], "Zlatar", "Izvolite, kako vam mogu pomoci?");
    PrikaziDialog(playerid, DLG_ZLATARA, DIALOG_STYLE_LIST, T(playerid, "Zlatara"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
    return 1;
}

DLG:zlatara(playerid, response, listitem)
{
    if (!response || ZlataraIgraca(playerid) == -1) return 1;
    new list[512], row[96], len = 0, n = 0;
    switch (listitem)
    {
        case 0:
        {
            format(list, sizeof list, "%s\t%s\n", T(playerid, "Predmet"), T(playerid, "Cijena"));
            len = strlen(list);
            for (new i = 0; i < sizeof gZlataraRoba; i++)
            {
                format(row, sizeof row, "%s\t$%d", T(playerid, gItems[gZlataraRoba[i][0]][ITEM_NAME]), gZlataraRoba[i][1]);
                DodajStupac(list, len, row);
            }
            return PrikaziDialog(playerid, DLG_ZLATARA_KUPI, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Kupi nakit"), list, T(playerid, "Kupi"), T(playerid, "Natrag"));
        }
        case 1:
        {
            format(list, sizeof list, "%s\t%s\n", T(playerid, "Predmet"), T(playerid, "Otkup"));
            len = strlen(list);
            for (new i = 0; i < sizeof gZlataraRoba; i++)
            {
                if (!gInv[playerid][gZlataraRoba[i][0]]) continue;
                gZlataraOdabir[playerid][n++] = gZlataraRoba[i][0];
                format(row, sizeof row, "%s (%d)\t$%d", T(playerid, gItems[gZlataraRoba[i][0]][ITEM_NAME]), gInv[playerid][gZlataraRoba[i][0]], gZlataraRoba[i][1] * ZLATARA_OTKUP_POSTO / 100);
                DodajStupac(list, len, row);
            }
            if (!n) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate nakita za prodaju.")), ZlataraKasa(playerid);
            for (new i = n; i < sizeof gZlataraRoba; i++) gZlataraOdabir[playerid][i] = -1;
            return PrikaziDialog(playerid, DLG_ZLATARA_PRODAJ, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Prodaj nakit"), list, T(playerid, "Prodaj"), T(playerid, "Natrag"));
        }
    }
    gApZlatara[playerid] = listitem;
    format(row, sizeof row, T(playerid, "Koliko poluga? (najvise %d)"), ZLATO_MAX_KOM);
    return PrikaziDialog(playerid, DLG_ZLATARA_ZLATO, DIALOG_STYLE_INPUT, T(playerid, "Zlatara"), row, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

DLG:zlatara_kupi(playerid, response, listitem)
{
    if (ZlataraIgraca(playerid) == -1) return 1;
    if (!response || listitem < 0 || listitem >= sizeof gZlataraRoba) return ZlataraKasa(playerid);
    new item = gZlataraRoba[listitem][0], cijena = gZlataraRoba[listitem][1];
    if (gNovac[playerid] < cijena) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca.")), ZlataraKasa(playerid);
    if (!GiveItem(playerid, item, 1)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko.")), ZlataraKasa(playerid);
    DajNovac(playerid, -cijena);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste: %s ($%d)."), T(playerid, gItems[item][ITEM_NAME]), cijena);
    if (item >= ITEM_SAT1 && item <= ITEM_SAT3 && gSatNosi[playerid] != item) return ZlataraSat(playerid, item);
    if (item == ITEM_ZLATNE_NAOCALE || item == ITEM_SREBRNE_NAOCALE) return OdjecaKapaPredmet(playerid, item);
    if ((item == ITEM_LANAC || item == ITEM_OGRLICA) && gOgrNosi[playerid] != item) return ZlataraOgrlica(playerid, item);
    return ZlataraKasa(playerid);
}

DLG:zlatara_prodaj(playerid, response, listitem)
{
    if (ZlataraIgraca(playerid) == -1) return 1;
    if (!response || listitem < 0 || listitem >= sizeof gZlataraRoba || gZlataraOdabir[playerid][listitem] == -1) return ZlataraKasa(playerid);
    new item = gZlataraOdabir[playerid][listitem], iznos = ZlataraCijena(item) * ZLATARA_OTKUP_POSTO / 100;
    if (!GiveItem(playerid, item, -1)) return ZlataraKasa(playerid);
    if (!DajNovac(playerid, iznos))
    {
        GiveItem(playerid, item, 1);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        return ZlataraKasa(playerid);
    }
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste: %s ($%d)."), T(playerid, gItems[item][ITEM_NAME]), iznos);
    return ZlataraKasa(playerid);
}

DLG:zlatara_zlato(playerid, response, const inputtext[])
{
    if (ZlataraIgraca(playerid) == -1) return 1;
    if (!response) return ZlataraKasa(playerid);
    new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 3 ? strval(inputtext) : 0;
    if (n <= 0) return ZlataraKasa(playerid);
    if (gApZlatara[playerid] == 2)
    {
        new iznos = ZlatoKupnja(n);
        if (gInv[playerid][ITEM_ZLATO] + n > ZLATO_MAX_KOM || gNovac[playerid] < iznos) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        else if (!GiveItem(playerid, ITEM_ZLATO, n)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        else
        {
            DajNovac(playerid, -iznos);
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d zlatnih poluga za $%d."), n, iznos);
        }
        return ZlataraKasa(playerid);
    }
    new iznos = ZlatoProdaja(n);
    if (gInv[playerid][ITEM_ZLATO] < n) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko zlatnih poluga."));
    else if (GiveItem(playerid, ITEM_ZLATO, -n))
    {
        if (DajNovac(playerid, iznos)) SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %d zlatnih poluga za $%d."), n, iznos);
        else
        {
            GiveItem(playerid, ITEM_ZLATO, n);
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
        }
    }
    return ZlataraKasa(playerid);
}

PreprodavacKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    for (new p = 0; p < PREPRODAVAC_MAX; p++)
    {
        if (!gPp[p][ppIma] || GetPlayerVirtualWorld(playerid) != gPp[p][ppSvijet] || GetPlayerInterior(playerid) != gPp[p][ppInt] || !IsPlayerInRangeOfPoint(playerid, ZLATARA_KASA_DOMET, gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ])) continue;
        new n = gInv[playerid][ITEM_UKRADENI_NAKIT], info[128];
        if (!n)
        {
            AktorKaze(playerid, gPp[p][ppAktor], "Preprodavac", "Nemas nista za mene. Gubi se.");
            return 1;
        }
        format(info, sizeof info, T(playerid, "Ukradeni nakit: %d kom. Preprodavac nudi $%d po komadu."), n, PREPRODAVAC_CIJENA);
        AktorKaze(playerid, gPp[p][ppAktor], "Preprodavac", "Sto imas za mene?");
        UpitKolicina(playerid, UPIT_PREPRODAVAC, p, 0, T(playerid, "Preprodavac"), info);
        return 1;
    }
    return 0;
}

PreprodavacUpit(playerid, p, kolicina)
{
    if (p < 0 || p >= PREPRODAVAC_MAX || !gPp[p][ppIma] || !IsPlayerInRangeOfPoint(playerid, ZLATARA_KASA_DOMET, gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ])) return 1;
    new n = gInv[playerid][ITEM_UKRADENI_NAKIT], poruka[144];
    if (kolicina <= 0 || kolicina > n) return format(poruka, sizeof poruka, T(playerid, "Imate samo %d komada."), n), ObavijestGreska(playerid, poruka);
    if (!GiveItem(playerid, ITEM_UKRADENI_NAKIT, -kolicina)) return 1;
    if (!DajNovac(playerid, kolicina * PREPRODAVAC_CIJENA))
    {
        GiveItem(playerid, ITEM_UKRADENI_NAKIT, kolicina);
        return ObavijestGreska(playerid, T(playerid, "Ne mozete nositi toliko."));
    }
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s predaje paket preprodavacu.", GetRPIme(playerid), "", 1);
    format(poruka, sizeof poruka, T(playerid, "Prodali ste %d komada ukradenog nakita za $%d."), kolicina, kolicina * PREPRODAVAC_CIJENA);
    return ObavijestInfo(playerid, T(playerid, "Preprodavac"), poruka);
}

static ZlataraSatPrikaci(playerid, item, const Float:p[9])
{
    SetPlayerAttachedObject(playerid, SAT_SLOT, gItems[item][ITEM_MODEL], 5, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
}

static ZlataraSatZadano(playerid, Float:p[9])
{
    new skin = GetPlayerSkin(playerid);
    if (skin < 0 || skin > 311) skin = 0;
    for (new i = 0; i < 9; i++) p[i] = gFitSat[skin][i];
}

static bool:ZlataraSatUcitaj(playerid, Float:p[9])
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z, rx, ry, rz, sx, sy, sz, fit FROM jewelry_watch WHERE account = %d AND skin = %d", gAccountId[playerid], GetPlayerSkin(playerid));
    new bool:ima = DB_GetRowCount(res) && DB_GetFieldInt(res, 9);
    if (ima) for (new i = 0; i < 9; i++) p[i] = DB_GetFieldFloat(res, i);
    DB_FreeResultSet(res);
    return ima;
}

static ZlataraSatSpremi(playerid, const Float:p[9])
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE jewelry_watch SET x = %f, y = %f, z = %f, rx = %f, ry = %f, rz = %f, sx = %f, sy = %f, sz = %f, skin = %d, fit = 1 WHERE account = %d", p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8], GetPlayerSkin(playerid), gAccountId[playerid]));
}

ZlataraSat(playerid, item)
{
    if (gSatNosi[playerid] == item)
    {
        gSatNosi[playerid] = 0;
        RemovePlayerAttachedObject(playerid, SAT_SLOT);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE jewelry_watch SET item = 0 WHERE account = %d", gAccountId[playerid]));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Skinuli ste: %s."), T(playerid, gItems[item][ITEM_NAME]));
    }
    new Float:p[9];
    gSatNosi[playerid] = item;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO jewelry_watch (account) VALUES (%d)", gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE jewelry_watch SET item = %d WHERE account = %d", item, gAccountId[playerid]));
    if (!ZlataraSatUcitaj(playerid, p))
    {
        ZlataraSatZadano(playerid, p);
        SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Sat je namjesten prema vasem izgledu. Ako ne pase: /namjestisat"));
    }
    ZlataraSatPrikaci(playerid, item, p);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Stavili ste: %s."), T(playerid, gItems[item][ITEM_NAME]));
}

static ZlataraOgrlicaPrikaci(playerid)
{
    new item = gOgrNosi[playerid], skin = GetPlayerSkin(playerid);
    if (!item) return RemovePlayerAttachedObject(playerid, OGRLICA_SLOT);
    if (skin < 0 || skin > 311) skin = 0;
    new Float:p[9];
    for (new i = 0; i < 9; i++) p[i] = item == ITEM_LANAC ? gFitLanac[skin][i] : gFitOgrlica[skin][i];
    return SetPlayerAttachedObject(playerid, OGRLICA_SLOT, item == ITEM_LANAC ? MODEL_LANAC : MODEL_OGRLICA, 1, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7], p[8]);
}

ZlataraOgrlica(playerid, item)
{
    new bool:skini = gOgrNosi[playerid] == item;
    gOgrNosi[playerid] = skini ? 0 : item;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO jewelry_watch (account) VALUES (%d)", gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE jewelry_watch SET neck = %d WHERE account = %d", gOgrNosi[playerid], gAccountId[playerid]));
    ZlataraOgrlicaPrikaci(playerid);
    if (skini) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Skinuli ste: %s."), T(playerid, gItems[item][ITEM_NAME]));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Stavili ste: %s."), T(playerid, gItems[item][ITEM_NAME]));
}

CMD:namjestisat(playerid, params[])
{
    if (!gUlogiran[playerid] || !gSatNosi[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne nosite sat."));
    EditAttachedObject(playerid, SAT_SLOT);
    return SendClientMessage(playerid, COLOR_POMOC, T(playerid, "Namjestite sat na ruci i spremite."));
}

CMD:fitwatch(playerid, params[]) return cmd_namjestisat(playerid, params);

bool:ZlataraUredi(playerid, EDIT_RESPONSE:response, index, Float:x, Float:y, Float:z, Float:rx, Float:ry, Float:rz, Float:sx, Float:sy, Float:sz)
{
    if (index != SAT_SLOT) return false;
    if (!gSatNosi[playerid]) return true;
    if (response == EDIT_RESPONSE_FINAL)
    {
        new Float:p[9];
        p[0] = x < -0.2 ? -0.2 : x > 0.2 ? 0.2 : x;
        p[1] = y < -0.2 ? -0.2 : y > 0.2 ? 0.2 : y;
        p[2] = z < -0.2 ? -0.2 : z > 0.2 ? 0.2 : z;
        p[3] = rx;
        p[4] = ry;
        p[5] = rz;
        p[6] = sx < 0.5 ? 0.5 : sx > 1.5 ? 1.5 : sx;
        p[7] = sy < 0.5 ? 0.5 : sy > 1.5 ? 1.5 : sy;
        p[8] = sz < 0.5 ? 0.5 : sz > 1.5 ? 1.5 : sz;
        ZlataraSatSpremi(playerid, p);
    }
    ZlataraSpawn(playerid);
    return true;
}

ZlataraSpawn(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT item, neck FROM jewelry_watch WHERE account = %d", gAccountId[playerid]);
    new item = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0, vrat = DB_GetRowCount(res) ? DB_GetFieldInt(res, 1) : 0, Float:p[9];
    DB_FreeResultSet(res);
    gOgrNosi[playerid] = (vrat == ITEM_LANAC || vrat == ITEM_OGRLICA) && gInv[playerid][vrat] ? vrat : 0;
    ZlataraOgrlicaPrikaci(playerid);
    gSatNosi[playerid] = item >= ITEM_SAT1 && item <= ITEM_SAT3 && gInv[playerid][item] ? item : 0;
    if (!gSatNosi[playerid])
    {
        RemovePlayerAttachedObject(playerid, SAT_SLOT);
        return;
    }
    if (!ZlataraSatUcitaj(playerid, p)) ZlataraSatZadano(playerid, p);
    ZlataraSatPrikaci(playerid, item, p);
}

ZlataraPredmetNestao(playerid, item)
{
    if (gOgrNosi[playerid] == item)
    {
        gOgrNosi[playerid] = 0;
        RemovePlayerAttachedObject(playerid, OGRLICA_SLOT);
    }
    if (gSatNosi[playerid] != item) return;
    gSatNosi[playerid] = 0;
    RemovePlayerAttachedObject(playerid, SAT_SLOT);
}

ZlataraDisconnect(playerid)
{
    gSatNosi[playerid] = 0;
    gOgrNosi[playerid] = 0;
}

static ZlataraEkipa(playerid, s)
{
    new n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] == gOrg[playerid] && ZlataraIgraca(i) == s && !IsRanjen(i)) n++;
    return n;
}

static VitrineNadji()
{
    if (gVitrinaBroj != -1) return;
    gVitrinaBroj = sizeof gVitrinaPoz;
    for (new k = 0; k < sizeof gVitrinaPoz; k++) gVitrina[k] = ObjektBlizu(gVitrinaPoz[k][0], gVitrinaPoz[k][1], gVitrinaPoz[k][2], 0.1, VITRINA_MODEL);
}

static VitrinaStakla(k)
{
    new bool:razbijena = false;
    for (new s = 0; s < ZLATARA_MAX && !razbijena; s++) if (gZl[s][zlIma] && gZl[s][zlPljacka] && gZlVitrina[s][k] != VT_CIJELA) razbijena = true;
    if (gVitrina[k]) SetDynamicObjectPos(gVitrina[k], gVitrinaPoz[k][0], gVitrinaPoz[k][1], gVitrinaPoz[k][2] - (razbijena ? 30.0 : 0.0));
}

static ZlataraVitrineReset(s)
{
    for (new k = 0; k < gVitrinaBroj; k++)
    {
        new bila = gZlVitrina[s][k];
        gZlVitrina[s][k] = VT_CIJELA;
        if (bila != VT_CIJELA) VitrinaStakla(k);
    }
}

static ZlataraVitrina(playerid, s)
{
    new naj = -1, Float:nd = VITRINA_DOMET;
    for (new k = 0; k < gVitrinaBroj; k++)
    {
        if (gZlVitrina[s][k] == VT_PRAZNA) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, gVitrinaPoz[k][0], gVitrinaPoz[k][1], gVitrinaPoz[k][2]);
        if (d < nd) { nd = d; naj = k; }
    }
    return naj;
}

ZlataraAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    new s = ZlataraIgraca(playerid);
    if (s == -1 || !gZl[s][zlPljacka] || gOrg[playerid] != gZl[s][zlPljacka]) return 0;
    new k = ZlataraVitrina(playerid, s);
    if (k == -1) return 0;
    if (gZlVitrina[s][k] == VT_CIJELA)
    {
        gZlVitrina[s][k] = VT_RAZBIJENA;
        ApplyAnimation(playerid, "FIGHT_D", "FightD_1", 4.1, false, false, false, false, 0);
        PlayerPlaySound(playerid, 1135, gVitrinaPoz[k][0], gVitrinaPoz[k][1], gVitrinaPoz[k][2]);
        VitrinaStakla(k);
        ObavijestInfo(playerid, T(playerid, "Pljacka zlatare"), T(playerid, "Razbili ste vitrinu. Pritisnite ALT da uzmete nakit."));
        return 1;
    }
    if (gInv[playerid][ITEM_UKRADENI_NAKIT] >= gItems[ITEM_UKRADENI_NAKIT][ITEM_MAX]) return ObavijestGreska(playerid, T(playerid, "Ne mozete nositi vise nakita.")), 1;
    gZlVitrina[s][k] = VT_PRAZNA;
    ApplyAnimation(playerid, "ROB_BANK", "CAT_Safe_Rob", 4.1, false, false, false, false, 0);
    GiveItem(playerid, ITEM_UKRADENI_NAKIT, 1);
    gZlPlijen[playerid]++;
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Uzeli ste nakit iz vitrine (ukupno %d)."), gZlPlijen[playerid]);
    ObavijestInfo(playerid, T(playerid, "Pljacka zlatare"), tekst);
    return 1;
}

static ZlataraSavjeti(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i] || gOrg[i] != gZl[s][zlPljacka] || ZlataraIgraca(i) != s || IsRanjen(i)) continue;
        new k = ZlataraVitrina(i, s);
        if (k == -1) continue;
        Savjet(i, gZlVitrina[s][k] == VT_CIJELA ? T(i, "~y~ALT~w~ - razbij vitrinu") : T(i, "~y~ALT~w~ - uzmi nakit"), 1200);
    }
}

ZlataraUpdate(playerid)
{
    if (!gUlogiran[playerid] || !gOrg[playerid]) return;
    new s = ZlataraIgraca(playerid), WEAPON:w = GetPlayerWeapon(playerid);
    if (s == -1 || gZl[s][zlPljacka] || !gZl[s][zlImaAktor] || w < WEAPON_COLT45 || w > WEAPON_MINIGUN || !OruzjeLegit(playerid, w) || GetPlayerTargetActor(playerid) != gZl[s][zlAktor]) return;
    new sad = gettime();
    if (ZlataraEkipa(playerid, s) < ZLATARA_PLJACKASA)
    {
        if (sad - gZlataraPoruka[playerid] < 10) return;
        gZlataraPoruka[playerid] = sad;
        new tekst[96];
        format(tekst, sizeof tekst, T(playerid, "Za pljacku zlatare treba vas najmanje %d iz organizacije."), ZLATARA_PLJACKASA);
        ObavijestGreska(playerid, tekst);
        return;
    }
    if (gZl[s][zlPljackano] && sad - gZl[s][zlPljackano] < ZLATARA_PLJACKA_PAUZA)
    {
        if (sad - gZlataraPoruka[playerid] < 10) return;
        gZlataraPoruka[playerid] = sad;
        ObavijestGreska(playerid, T(playerid, "Ova zlatara je nedavno opljackana."));
        return;
    }
    VitrineNadji();
    ZlataraVitrineReset(s);
    gZl[s][zlPljacka] = gOrg[playerid];
    gZl[s][zlPljackaKraj] = sad + ZLATARA_PLJACKA_S;
    gZl[s][zlPljackano] = sad;
    ApplyActorAnimation(gZl[s][zlAktor], "SHOP", "SHP_Rob_HandsUp", 4.1, false, false, false, true, 0);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gUlogiran[i]) continue;
        if (gOrg[i] == gOrg[playerid] && ZlataraIgraca(i) == s)
        {
            for (new k = 0; k < ZLATARA_WANTED; k++) DodajWanted(i);
            gZlPlijen[i] = 0;
            ObavijestInfo(i, T(i, "Pljacka zlatare"), T(i, "Pljacka je pocela! Razbijajte vitrine s ALT i uzmite nakit."));
        }
        else if (PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: pljacka zlatare u tijeku, %s."), CestaZona(gZl[s][zlX], gZl[s][zlY]));
    }
}

ZlataraTick()
{
    new sad = gettime();
    if (++gZlatoSat >= ZLATO_MINUTA)
    {
        gZlatoSat = 0;
        gZlato = ZlatoNovo(gZlato, random(21) - 10);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO gold_price (id, price) VALUES (1, %d)", gZlato));
    }
    for (new s = 0; s < ZLATARA_MAX; s++)
    {
        if (!gZl[s][zlIma] || !gZl[s][zlPljacka]) continue;
        new org = gZl[s][zlPljacka], prisutni[MAX_PLAYERS], n = 0;
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && gOrg[i] == org && ZlataraIgraca(i) == s && !IsRanjen(i)) prisutni[n++] = i;
        if (n && sad < gZl[s][zlPljackaKraj])
        {
            ZlataraSavjeti(s);
            continue;
        }
        new ukupno = 0;
        for (new k = 0; k < gVitrinaBroj; k++) if (gZlVitrina[s][k] == VT_PRAZNA) ukupno++;
        gZl[s][zlPljacka] = 0;
        ZlataraVitrineReset(s);
        if (gZl[s][zlImaAktor]) ClearActorAnimations(gZl[s][zlAktor]);
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        {
            if (!gUlogiran[i] || gOrg[i] != org) continue;
            if (!ukupno) ObavijestGreska(i, T(i, "Pljacka zlatare je propala."));
            else if (gZlPlijen[i])
            {
                new tekst[128];
                format(tekst, sizeof tekst, T(i, "Pljacka je gotova. Uzeli ste %d komada nakita. Prodajte ih preprodavacu."), gZlPlijen[i]);
                ObavijestInfo(i, T(i, "Pljacka zlatare"), tekst);
            }
            gZlPlijen[i] = 0;
        }
        if (ukupno) OrgDnevnik(org, INVALID_PLAYER_ID, DNEVNIK_ZLATARA, ukupno);
    }
}

ShowZlatare(playerid)
{
    new list[1536], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova zlatara na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novi preprodavac na mojoj poziciji"));
    for (new s = 0; s < ZLATARA_MAX; s++)
    {
        if (!gZl[s][zlIma]) continue;
        format(row, sizeof row, "[%d] %s%s", gZl[s][zlDb], CestaZona(gZl[s][zlX], gZl[s][zlY]), T(playerid, gZl[s][zlImaAktor] ? ("") : (" - bez prodavaca")));
        DodajStupac(list, len, row);
    }
    for (new p = 0; p < PREPRODAVAC_MAX; p++)
    {
        if (!gPp[p][ppIma]) continue;
        format(row, sizeof row, T(playerid, "Preprodavac [%d] %s"), gPp[p][ppDb], CestaZona(gPp[p][ppX], gPp[p][ppY]));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_ZLATARA, DIALOG_STYLE_LIST, T(playerid, "Zlatare"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_zlatara(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0;
        while (s < ZLATARA_MAX && gZl[s][zlIma]) s++;
        if (s == ZLATARA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Zlatara se postavlja samo izvana.")), ShowZlatare(playerid);
        GetPlayerPos(playerid, gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ]);
        GetPlayerFacingAngle(playerid, gZl[s][zlA]);
        gZl[s][zlImaAktor] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO jewelry_shops (x, y, z, a) VALUES (%f, %f, %f, %f)", gZl[s][zlX], gZl[s][zlY], gZl[s][zlZ], gZl[s][zlA]));
        gZl[s][zlDb] = ZadnjiId();
        ZlataraStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Zlatara dodana: %s. Udite i postavite prodavaca."), CestaZona(gZl[s][zlX], gZl[s][zlY]));
        return ShowZlatare(playerid);
    }
    if (listitem == 1)
    {
        new p = 0;
        while (p < PREPRODAVAC_MAX && gPp[p][ppIma]) p++;
        if (p == PREPRODAVAC_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dosegnut je maksimalan broj preprodavaca.")), ShowZlatare(playerid);
        GetPlayerPos(playerid, gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ]);
        GetPlayerFacingAngle(playerid, gPp[p][ppA]);
        gPp[p][ppInt] = GetPlayerInterior(playerid);
        gPp[p][ppSvijet] = GetPlayerVirtualWorld(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO fences (x, y, z, a, interior, world) VALUES (%f, %f, %f, %f, %d, %d)", gPp[p][ppX], gPp[p][ppY], gPp[p][ppZ], gPp[p][ppA], gPp[p][ppInt], gPp[p][ppSvijet]));
        gPp[p][ppDb] = ZadnjiId();
        PreprodavacStvori(p);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Preprodavac postavljen."));
        return ShowZlatare(playerid);
    }
    new n = listitem - 1;
    for (new s = 0; s < ZLATARA_MAX; s++)
    {
        if (!gZl[s][zlIma] || --n) continue;
        gApZlatara[playerid] = s;
        return PrikaziDialog(playerid, DLG_APANEL_ZLATARA_MENI, DIALOG_STYLE_LIST, T(playerid, "Zlatare"), T(playerid, "Postavi prodavaca na mojoj poziciji\nObrisi zlataru"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    for (new p = 0; p < PREPRODAVAC_MAX; p++)
    {
        if (!gPp[p][ppIma] || --n) continue;
        DestroyActor(gPp[p][ppAktor]);
        DestroyDynamicArea(gPpZona[p]);
        gPp[p][ppIma] = false;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM fences WHERE id = %d", gPp[p][ppDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Preprodavac obrisan."));
        break;
    }
    return ShowZlatare(playerid);
}

DLG:apanel_zlatara_meni(playerid, response, listitem)
{
    new s = gApZlatara[playerid];
    if (!response || !gZl[s][zlIma]) return ShowZlatare(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (ZlataraIgraca(playerid) != s) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate biti unutar te zlatare.")), ShowZlatare(playerid);
        ZlataraMakniAktor(s);
        GetPlayerPos(playerid, gZl[s][zlAx], gZl[s][zlAy], gZl[s][zlAz]);
        GetPlayerFacingAngle(playerid, gZl[s][zlAa]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE jewelry_shops SET Aktor = 1, ax = %f, ay = %f, az = %f, aa = %f WHERE id = %d", gZl[s][zlAx], gZl[s][zlAy], gZl[s][zlAz], gZl[s][zlAa], gZl[s][zlDb]));
        ZlataraAktor(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prodavac je postavljen."));
        return ShowZlatare(playerid);
    }
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gUlogiran[i] && ZlataraIgraca(i) == s) ZlataraVani(i, s);
    ZlataraMakniAktor(s);
    DestroyDynamicPickup(gZl[s][zlPickup]);
    MapaIkonaMakni(gZl[s][zlIkona]);
    DestroyDynamicPickup(gZl[s][zlPickupU]);
    DestroyDynamic3DTextLabel(gZl[s][zlTekst]);
    DestroyDynamic3DTextLabel(gZl[s][zlTekstU]);
    gZl[s][zlIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM jewelry_shops WHERE id = %d", gZl[s][zlDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Zlatara obrisana."));
    return ShowZlatare(playerid);
}

ZlataraLokacija(i, &Float:x, &Float:y, &Float:z, &Float:a)
{
    if (i >= ZLATARA_MAX) return -1;
    if (!gZl[i][zlIma]) return 0;
    x = gZl[i][zlX];
    y = gZl[i][zlY];
    z = gZl[i][zlZ];
    a = gZl[i][zlA];
    return 1;
}
