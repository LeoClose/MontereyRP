#define PIZZA_MAX           16
#define PIZZA_SVIJET        3300
#define PIZZA_INT           5
#define PIZZA_GOSTIJU       4
#define PIZZA_NARUDZBI      12
#define PIZZA_STOLOVA       12
#define PIZZA_DOMET         1.6
#define PIZZA_KASA_DOMET    2.5
#define PIZZA_PREDAJA_DOMET 2.2
#define PIZZA_ULAZ_DOMET    2.0
#define PIZZA_SKIN          155
#define PIZZA_PLACA_SAT     200
#define PIZZA_AKTIVAN_S     120
#define PIZZA_STRPLJENJE    240
#define PIZZA_IGRAC_CEKA    150
#define PIZZA_GOST_MIN      20
#define PIZZA_GOST_RASPON   25
#define PIZZA_JEDE_S        15
#define PIZZA_HODA_S        30
#define PIZZA_IGRA_S        40
#define PIZZA_TRAKA_MS      50
#define PIZZA_JEDENJE_MS    4500
#define PIZZA_HRANA_SLOT    4
#define PIZZA_NOSI_SLOT     8
#define PIZZA_KUTIJA        19811
#define PIZZA_BOJA          0xE74C3CFF
#define PIZZA_TRAKA_X       210.0
#define PIZZA_TRAKA_W       220.0
#define PIZZA_PULT          0.9

enum { PT_KASA, PT_KUPAC, PT_ROSTILJ, PT_SLAGANJE, PT_POSAO, PT_POMFRIT, PT_BROJ }
enum { PV_BURGER, PV_PIZZA, PV_SALATA, PV_POMFRIT, PV_PICE }
enum { PG_NEMA, PG_DOLAZI, PG_KASA, PG_STOL_IDE, PG_STOL, PG_JEDE, PG_ODLAZI }
enum { PN_ROSTILJ, PN_POMFRIT, PN_SLAGANJE, PN_NOSI }
enum { PI_NEMA, PI_ROSTILJ, PI_POMFRIT, PI_SLAGANJE, PI_PEC, PI_PAKIRANJE }
enum { PZTD_POZADINA, PZTD_NASLOV, PZTD_DIO1, PZTD_DIO2, PZTD_DIO3, PZTD_OZNAKA, PZTD_UPUTA, PZTD_SLOJEVI, PZTD_BROJ }
enum { PZS_LIJEVO, PZS_DESNO, PZS_INFO, PZS_KUPI, PZS_IZADI, PZS_BROJ }

static const Float:gPzUnutra[4] = { 372.3520, -131.6510, 1001.4922, 0.0 };
static const Float:gPzIzlaz[3] = { 372.3520, -133.3000, 1001.4922 };
static Float:gPzTocka[PT_BROJ][4] = {
    { 375.60, -117.30, 1001.49, 180.0 },
    { 375.60, -119.70, 1001.49, 0.0 },
    { 373.80, -114.60, 1001.49, 0.0 },
    { 377.40, -115.00, 1001.49, 0.0 },
    { 369.1047, -114.7455, 1001.4995, 3.0862 },
    { 375.60, -114.60, 1001.49, 0.0 }
};
static const gPzTockaIme[PT_BROJ][] = { "Blagajnik", "Mjesto kupca", "Rostilj", "Slaganje i pec za pizzu", "Zaposlenje", "Friteza" };
static Float:gPzStol[PIZZA_STOLOVA][4], gPzStolDb[PIZZA_STOLOVA], gPzStolova;

static const gPzMeni[][] = { "Moo Kids Meal", "Beef Tower", "Meat Stack", "Buster", "Double D-Luxe", "Full Rack", "Salad Meal", "Pomfrit", "Sok" };
static const gPzVrsta[] = { PV_BURGER, PV_BURGER, PV_BURGER, PV_PIZZA, PV_PIZZA, PV_PIZZA, PV_SALATA, PV_POMFRIT, PV_PICE };
static const gPzCijena[] = { 6, 10, 14, 7, 11, 15, 8, 4, 3 };
static const Float:gPzZdravlje[] = { 20.0, 35.0, 50.0, 25.0, 40.0, 55.0, 20.0, 12.0, 8.0 };
static const gPzModel[] = { 2703, 2703, 2703, 19580, 19580, 19580, 19811, 19811, 2647 };
static const gPzRuka[] = { 2703, 2703, 2703, 2702, 2702, 2702, 19811, 19811, 2647 };
static const gPzStrane[] = { 1, 2, 2, 1, 1, 1, 1, 1, 1 };
static const gPzSastojak[][] = { "Donja lepinja", "Meso", "Sir", "Salata", "Rajcica", "Gornja lepinja", "Tijesto", "Umak", "Salama", "Gljive", "Preljev" };
static const gPzRecept[sizeof gPzMeni][7] = {
    { 0, 1, 2, 5, -1, -1, -1 },
    { 0, 1, 2, 3, 4, 5, -1 },
    { 0, 1, 2, 1, 2, 5, -1 },
    { 6, 7, 2, -1, -1, -1, -1 },
    { 6, 7, 2, 8, 9, -1, -1 },
    { 6, 7, 2, 8, 9, 2, -1 },
    { 3, 4, 2, 10, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1 },
    { -1, -1, -1, -1, -1, -1, -1 }
};
static const KEY:gPzTipka[] = { KEY_YES, KEY_NO, KEY_WALK, KEY_SPRINT };
static const gPzTipkaIme[][] = { "Y", "N", "ALT", "SPACE" };

enum E_PZ { bool:pzIma, pzDb, Float:pzX, Float:pzY, Float:pzZ, Float:pzA, pzAktor, pzSljedeci, pzBroj,
    STREAMER_TAG_PICKUP:pzPickup, STREAMER_TAG_PICKUP:pzPickupU, STREAMER_TAG_3D_TEXT_LABEL:pzTekst, STREAMER_TAG_3D_TEXT_LABEL:pzTekstU, STREAMER_TAG_3D_TEXT_LABEL:pzOznaka[PT_BROJ] }
static gPz[PIZZA_MAX][E_PZ];
enum E_PZG { pgLik, pgFaza, pgStol, pgOd, pgNar }
static gPzGost[PIZZA_MAX][PIZZA_GOSTIJU][E_PZG];
enum E_PZN { bool:pnIma, pnStanje, pnGost, pnIgrac, pnStavka, pnRadi, pnOd, pnBroj }
static gPzNar[PIZZA_MAX][PIZZA_NARUDZBI][E_PZN];

static gPzSmjena[MAX_PLAYERS], gPzIgra[MAX_PLAYERS], gPzRad[MAX_PLAYERS] = { -1, ... }, gPzKorak[MAX_PLAYERS], gPzStrana[MAX_PLAYERS], gPzIgraOd[MAX_PLAYERS], gPzTimer[MAX_PLAYERS];
static gPzSlojTipka[MAX_PLAYERS][7], Float:gPzPoz[MAX_PLAYERS], Float:gPzBrzina[MAX_PLAYERS], Float:gPzZelOd[MAX_PLAYERS], Float:gPzZelDo[MAX_PLAYERS];
static PlayerText:gPzTd[MAX_PLAYERS][PZTD_BROJ], bool:gPzTdIma[MAX_PLAYERS], PlayerText:gPzLista[MAX_PLAYERS], bool:gPzListaIma[MAX_PLAYERS], gPzListaTekst[MAX_PLAYERS][256];
static gPzSavjet[MAX_PLAYERS] = { -1, ... }, bool:gPzJede[MAX_PLAYERS], gPzApPopis[MAX_PLAYERS][PIZZA_MAX + PIZZA_STOLOVA];
static gPzRadio[MAX_PLAYERS], gPzAkcija[MAX_PLAYERS];
static gPzShop[MAX_PLAYERS] = { -1, ... }, gPzShopIdx[MAX_PLAYERS], PlayerText:gPzShopTd[MAX_PLAYERS][PZS_BROJ], gPzShopObj[MAX_PLAYERS] = { INVALID_OBJECT_ID, ... };

InitPizza()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS pizzerias (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS pizza_points (tip INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS pizza_tables (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT tip, x, y, z, a FROM pizza_points");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new t = DB_GetFieldInt(res, 0);
        if (t < 0 || t >= PT_BROJ) continue;
        for (new k = 0; k < 4; k++) gPzTocka[t][k] = DB_GetFieldFloat(res, k + 1);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM pizza_tables ORDER BY id LIMIT %d", PIZZA_STOLOVA);
    for (new n = DB_GetRowCount(res); gPzStolova < n; gPzStolova++, DB_SelectNextRow(res))
    {
        gPzStolDb[gPzStolova] = DB_GetFieldInt(res, 0);
        for (new k = 0; k < 4; k++) gPzStol[gPzStolova][k] = DB_GetFieldFloat(res, k + 1);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM pizzerias ORDER BY id LIMIT %d", PIZZA_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gPz[s][pzDb] = DB_GetFieldInt(res, 0);
        gPz[s][pzX] = DB_GetFieldFloat(res, 1);
        gPz[s][pzY] = DB_GetFieldFloat(res, 2);
        gPz[s][pzZ] = DB_GetFieldFloat(res, 3);
        gPz[s][pzA] = DB_GetFieldFloat(res, 4);
        PizzaStvori(s);
    }
    DB_FreeResultSet(res);
}

static PizzaStvori(s)
{
    new svijet = PIZZA_SVIJET + s;
    gPz[s][pzIma] = true;
    gPz[s][pzBroj] = 0;
    for (new g = 0; g < PIZZA_GOSTIJU; g++) gPzGost[s][g][pgLik] = INVALID_LIK, gPzGost[s][g][pgFaza] = PG_NEMA, gPzGost[s][g][pgStol] = -1;
    for (new n = 0; n < PIZZA_NARUDZBI; n++) gPzNar[s][n][pnIma] = false;
    gPz[s][pzPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gPz[s][pzX], gPz[s][pzY], gPz[s][pzZ], 0, 0);
    gPz[s][pzTekst] = CreateDynamic3DTextLabel("Pizzeria", PIZZA_BOJA, gPz[s][pzX], gPz[s][pzY], gPz[s][pzZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gPz[s][pzTekst], PIZZA_ULAZ_DOMET, SAVJET_ULAZ);
    gPz[s][pzPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gPzIzlaz[0], gPzIzlaz[1], gPzIzlaz[2], svijet, PIZZA_INT);
    gPz[s][pzTekstU] = CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, gPzIzlaz[0], gPzIzlaz[1], gPzIzlaz[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, PIZZA_INT);
    InterakcijaOznaka(gPz[s][pzTekstU], PIZZA_ULAZ_DOMET, SAVJET_IZLAZ);
    PizzaOznakeStvori(s);
    FirmaUcitaj(FIRMA_PIZZA, s, gPz[s][pzDb]);
}

static PizzaOznakeStvori(s)
{
    new svijet = PIZZA_SVIJET + s;
    gPz[s][pzAktor] = CreateActor(PIZZA_SKIN, gPzTocka[PT_KASA][0], gPzTocka[PT_KASA][1], gPzTocka[PT_KASA][2], gPzTocka[PT_KASA][3]);
    SetActorVirtualWorld(gPz[s][pzAktor], svijet);
    SetActorInvulnerable(gPz[s][pzAktor], true);
    for (new t = 0; t < PT_BROJ; t++)
    {
        gPz[s][pzOznaka][t] = STREAMER_TAG_3D_TEXT_LABEL:0;
        if (t == PT_KUPAC) continue;
        new Float:x = gPzTocka[t][0], Float:y = gPzTocka[t][1], Float:z = gPzTocka[t][2] + 1.1;
        if (t == PT_POSAO) z = gPzTocka[t][2] + 0.4;
        else if (t != PT_KASA)
        {
            x += 0.7 * floatsin(-gPzTocka[t][3], degrees);
            y += 0.7 * floatcos(-gPzTocka[t][3], degrees);
            z = gPzTocka[t][2] + 0.25;
        }
        gPz[s][pzOznaka][t] = CreateDynamic3DTextLabel(t == PT_KASA ? ("Blagajna") : t == PT_POSAO ? ("Pizzeria - posao\n/posao") : gPzTockaIme[t], PIZZA_BOJA, x, y, z, 6.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, PIZZA_INT);
        InterakcijaOznaka(gPz[s][pzOznaka][t], t == PT_KASA ? PIZZA_KASA_DOMET + 1.0 : PIZZA_DOMET + 0.8);
    }
}

static PizzaOznakeMakni(s)
{
    DestroyActor(gPz[s][pzAktor]);
    for (new t = 0; t < PT_BROJ; t++) if (gPz[s][pzOznaka][t]) DestroyDynamic3DTextLabel(gPz[s][pzOznaka][t]);
}

PizzaIgraca(playerid)
{
    new s = GetPlayerVirtualWorld(playerid) - PIZZA_SVIJET;
    return s >= 0 && s < PIZZA_MAX && gPz[s][pzIma] && GetPlayerInterior(playerid) == PIZZA_INT ? s : -1;
}

static bool:PizzaTu(playerid, t, Float:r = PIZZA_DOMET)
{
    return IsPlayerInRangeOfPoint(playerid, r, gPzTocka[t][0], gPzTocka[t][1], gPzTocka[t][2]);
}

PizzaUlaz(playerid)
{
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new s = PizzaIgraca(playerid);
    if (s != -1)
    {
        if (!IsPlayerInRangeOfPoint(playerid, PIZZA_ULAZ_DOMET, gPzIzlaz[0], gPzIzlaz[1], gPzIzlaz[2])) return 0;
        PizzaShopZatvori(playerid);
        PrebaciIgraca(playerid, 0, 0, gPz[s][pzX], gPz[s][pzY], gPz[s][pzZ], gPz[s][pzA] + 180.0);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma] || !IsPlayerInRangeOfPoint(playerid, PIZZA_ULAZ_DOMET, gPz[s][pzX], gPz[s][pzY], gPz[s][pzZ])) continue;
        PrebaciIgraca(playerid, PIZZA_INT, PIZZA_SVIJET + s, gPzUnutra[0], gPzUnutra[1], gPzUnutra[2], gPzUnutra[3]);
        return 1;
    }
    return 0;
}

static PizzaRadnika(s)
{
    new n = 0;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gPzSmjena[i] == s + 1) n++;
    return n;
}

static PizzaRadnicima(s, const tekst[], broj, bool:greska = false)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gPzSmjena[i] != s + 1) continue;
        new poruka[144];
        format(poruka, sizeof poruka, T(i, tekst), broj);
        if (greska) ObavijestGreska(i, poruka);
        else ObavijestInfo(i, T(i, "Pizzeria"), poruka);
    }
}

static PizzaPocetnoStanje(stavka)
{
    switch (gPzVrsta[stavka])
    {
        case PV_BURGER: return PN_ROSTILJ;
        case PV_POMFRIT: return PN_POMFRIT;
    }
    return PN_SLAGANJE;
}

static PizzaNovaNarudzba(s, gost, igrac, stavka)
{
    for (new n = 0; n < PIZZA_NARUDZBI; n++)
    {
        if (gPzNar[s][n][pnIma]) continue;
        gPzNar[s][n][pnIma] = true;
        gPzNar[s][n][pnStanje] = PizzaPocetnoStanje(stavka);
        gPzNar[s][n][pnGost] = gost;
        gPzNar[s][n][pnIgrac] = igrac;
        gPzNar[s][n][pnStavka] = stavka;
        gPzNar[s][n][pnRadi] = INVALID_PLAYER_ID;
        gPzNar[s][n][pnOd] = gettime();
        gPzNar[s][n][pnBroj] = ++gPz[s][pzBroj] % 100;
        PizzaRadnicima(s, "Nova narudzba #%d.", gPzNar[s][n][pnBroj]);
        return n;
    }
    return -1;
}

static PizzaJedi(playerid, stavka)
{
    if (gPzJede[playerid]) RemovePlayerAttachedObject(playerid, PIZZA_HRANA_SLOT);
    new Float:hp;
    GetPlayerHealth(playerid, hp);
    hp += gPzZdravlje[stavka];
    SetPlayerHealth(playerid, hp > 100.0 ? 100.0 : hp);
    SetPlayerAttachedObject(playerid, PIZZA_HRANA_SLOT, gPzRuka[stavka], 6, 0.06, 0.03, 0.02, 0.0, 0.0, 0.0);
    if (GetPlayerState(playerid) == PLAYER_STATE_ONFOOT)
        ApplyAnimation(playerid, gPzVrsta[stavka] == PV_PICE ? ("VENDING") : ("FOOD"), gPzVrsta[stavka] == PV_PICE ? ("VEND_Drink2_P") : ("EAT_Burger"), 4.1, false, false, false, false, 0, SYNC_ALL);
    gPzJede[playerid] = true;
    SetTimerEx("PizzaPojeo", PIZZA_JEDENJE_MS, false, "ii", playerid, gAccountId[playerid]);
}

forward PizzaPojeo(playerid, racun);
public PizzaPojeo(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || !gPzJede[playerid]) return;
    gPzJede[playerid] = false;
    RemovePlayerAttachedObject(playerid, PIZZA_HRANA_SLOT);
}

static PizzaPult(&Float:x, &Float:y)
{
    new Float:a = gPzTocka[PT_KASA][3];
    x = gPzTocka[PT_KASA][0] + PIZZA_PULT * floatsin(-a, degrees);
    y = gPzTocka[PT_KASA][1] + PIZZA_PULT * floatcos(-a, degrees);
}

static PizzaShopPrikazi(playerid)
{
    new r = gPzShopIdx[playerid], info[128], Float:x, Float:y;
    format(info, sizeof info, "%s~n~~g~$%d~n~~w~+%.0f HP~n~%d/%d", T(playerid, gPzMeni[r]), gPzCijena[r], gPzZdravlje[r], r + 1, sizeof gPzMeni);
    PlayerTextDrawSetString(playerid, gPzShopTd[playerid][PZS_INFO], info);
    if (gPzShopObj[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gPzShopObj[playerid]);
    PizzaPult(x, y);
    gPzShopObj[playerid] = CreatePlayerObject(playerid, gPzModel[r], x, y, gPzTocka[PT_KASA][2] + 0.05, gPzModel[r] == 19580 || gPzModel[r] == 2647 ? 0.0 : 90.0, 0.0, gPzTocka[PT_KASA][3] + 90.0);
}

static PizzaShopOtvori(playerid, s)
{
    new Float:a = gPzTocka[PT_KASA][3], Float:x, Float:y;
    PizzaPult(x, y);
    gPzShop[playerid] = s;
    gPzShopIdx[playerid] = 0;
    TogglePlayerControllable(playerid, false);
    SetPlayerCameraPos(playerid, x + 1.1 * floatsin(-a, degrees), y + 1.1 * floatcos(-a, degrees), gPzTocka[PT_KASA][2] + 0.7);
    SetPlayerCameraLookAt(playerid, x, y, gPzTocka[PT_KASA][2] + 0.05);
    gPzShopTd[playerid][PZS_LIJEVO] = SalonGumb(playerid, 230.0, 340.0, "<", 0x333333CC, 30.0);
    gPzShopTd[playerid][PZS_DESNO] = SalonGumb(playerid, 410.0, 340.0, ">", 0x333333CC, 30.0);
    gPzShopTd[playerid][PZS_INFO] = SalonGumb(playerid, 320.0, 322.0, "_", 0x00000088, 150.0, false);
    gPzShopTd[playerid][PZS_KUPI] = SalonGumb(playerid, 280.0, 412.0, T(playerid, "KUPI"), 0x2E7D32CC, 70.0);
    gPzShopTd[playerid][PZS_IZADI] = SalonGumb(playerid, 360.0, 412.0, T(playerid, "IZADI"), 0x1E5AA8CC, 70.0);
    PizzaShopPrikazi(playerid);
    SelectTextDraw(playerid, 0xD4A017FF);
    AktorKaze(playerid, gPz[s][pzAktor], "Blagajnik", "Dobar dan, sto ce biti?");
}

PizzaShopZatvori(playerid)
{
    if (gPzShop[playerid] == -1) return;
    gPzShop[playerid] = -1;
    for (new i = 0; i < PZS_BROJ; i++) PlayerTextDrawDestroy(playerid, gPzShopTd[playerid][i]);
    if (gPzShopObj[playerid] != INVALID_OBJECT_ID) DestroyPlayerObject(playerid, gPzShopObj[playerid]);
    gPzShopObj[playerid] = INVALID_OBJECT_ID;
    CancelSelectTextDraw(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

PizzaKlik(playerid, PlayerText:td)
{
    new s = gPzShop[playerid];
    if (s == -1) return 0;
    if (td == gPzShopTd[playerid][PZS_IZADI] || PizzaIgraca(playerid) != s)
    {
        PizzaShopZatvori(playerid);
        return 1;
    }
    if (td == gPzShopTd[playerid][PZS_LIJEVO] || td == gPzShopTd[playerid][PZS_DESNO])
    {
        gPzShopIdx[playerid] = (gPzShopIdx[playerid] + (td == gPzShopTd[playerid][PZS_DESNO] ? 1 : sizeof gPzMeni - 1)) % sizeof gPzMeni;
        PizzaShopPrikazi(playerid);
        return 1;
    }
    if (td != gPzShopTd[playerid][PZS_KUPI]) return 1;
    new r = gPzShopIdx[playerid];
    for (new n = 0; n < PIZZA_NARUDZBI; n++)
        if (gPzNar[s][n][pnIma] && gPzNar[s][n][pnIgrac] == playerid) return ObavijestGreska(playerid, T(playerid, "Vec cekate narudzbu.")), 1;
    if (!DajNovac(playerid, -gPzCijena[r])) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca.")), 1;
    if (FirmaImaVlasnika(FIRMA_PIZZA, s)) FirmaProdaja(FIRMA_PIZZA, s, gPzCijena[r], false);
    else BlagajnaDodaj(gPzCijena[r]);
    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    PizzaShopZatvori(playerid);
    if (gPzVrsta[r] != PV_PICE && PizzaRadnika(s))
    {
        new n = PizzaNovaNarudzba(s, -1, playerid, r);
        if (n != -1)
        {
            new tekst[128];
            format(tekst, sizeof tekst, T(playerid, "Platili ste $%d. Pricekajte, radnik ce vam donijeti narudzbu #%d."), gPzCijena[r], gPzNar[s][n][pnBroj]);
            ObavijestInfo(playerid, T(playerid, "Pizzeria"), tekst);
            return 1;
        }
    }
    AktorKaze(playerid, gPz[s][pzAktor], "Blagajnik", "Izvolite, dobar tek!");
    PizzaJedi(playerid, r);
    return 1;
}

static PizzaStolSlobodan(s)
{
    new zauzet[PIZZA_STOLOVA], slobodni[PIZZA_STOLOVA], n = 0;
    for (new g = 0; g < PIZZA_GOSTIJU; g++) if (gPzGost[s][g][pgFaza] != PG_NEMA && gPzGost[s][g][pgStol] >= 0) zauzet[gPzGost[s][g][pgStol]] = 1;
    for (new t = 0; t < gPzStolova; t++) if (!zauzet[t]) slobodni[n++] = t;
    return n ? slobodni[random(n)] : -1;
}

static PizzaGostRed(g, &Float:x, &Float:y, &Float:z)
{
    new Float:pomak = (float(g) - 1.5) * 0.9, Float:a = gPzTocka[PT_KUPAC][3] + 90.0;
    x = gPzTocka[PT_KUPAC][0] + pomak * floatsin(-a, degrees);
    y = gPzTocka[PT_KUPAC][1] + pomak * floatcos(-a, degrees);
    z = gPzTocka[PT_KUPAC][2];
}

static PizzaGostNovi(s)
{
    new g = 0;
    while (g < PIZZA_GOSTIJU && gPzGost[s][g][pgFaza] != PG_NEMA) g++;
    if (g == PIZZA_GOSTIJU) return;
    new lik = Lik_Stvori(gKupacSkinovi[random(sizeof gKupacSkinovi)], gPzUnutra[0], gPzUnutra[1], gPzUnutra[2], 0.0, PIZZA_SVIJET + s, PIZZA_INT);
    if (lik == INVALID_LIK) return;
    CivilPostavi(lik, true);
    gPzGost[s][g][pgLik] = lik;
    gPzGost[s][g][pgFaza] = PG_DOLAZI;
    gPzGost[s][g][pgStol] = -1;
    gPzGost[s][g][pgNar] = -1;
    gPzGost[s][g][pgOd] = gettime();
    SetTimerEx("PizzaGostKreni", 700, false, "iii", s, g, lik);
}

forward PizzaGostKreni(s, g, lik);
public PizzaGostKreni(s, g, lik)
{
    if (gPzGost[s][g][pgLik] != lik || gPzGost[s][g][pgFaza] != PG_DOLAZI) return;
    new Float:x, Float:y, Float:z;
    PizzaGostRed(g, x, y, z);
    Lik_Idi(lik, x, y, z, LIK_HOD);
}

static PizzaGostMakni(s, g)
{
    if (gPzGost[s][g][pgLik] != INVALID_LIK) Lik_Ukloni(gPzGost[s][g][pgLik]);
    gPzGost[s][g][pgLik] = INVALID_LIK;
    gPzGost[s][g][pgFaza] = PG_NEMA;
    gPzGost[s][g][pgStol] = -1;
    gPzGost[s][g][pgNar] = -1;
}

static PizzaGostOdlazi(s, g)
{
    new lik = gPzGost[s][g][pgLik];
    if (lik == INVALID_LIK)
    {
        PizzaGostMakni(s, g);
        return;
    }
    new bool:sjedi = gPzGost[s][g][pgFaza] == PG_STOL || gPzGost[s][g][pgFaza] == PG_JEDE;
    gPzGost[s][g][pgFaza] = PG_ODLAZI;
    gPzGost[s][g][pgStol] = -1;
    gPzGost[s][g][pgOd] = gettime();
    Lik_OcistiAnimaciju(lik);
    if (sjedi)
    {
        Lik_Animacija(lik, "FOOD", "FF_Sit_Out_180", 4.1, 0, 1, 1, 0, 0);
        SetTimerEx("PizzaGostIzlaz", 1500, false, "iii", s, g, lik);
    }
    else Lik_Idi(lik, gPzUnutra[0], gPzUnutra[1], gPzUnutra[2], LIK_HOD);
}

forward PizzaGostIzlaz(s, g, lik);
public PizzaGostIzlaz(s, g, lik)
{
    if (gPzGost[s][g][pgLik] != lik || gPzGost[s][g][pgFaza] != PG_ODLAZI) return;
    Lik_OcistiAnimaciju(lik);
    Lik_Idi(lik, gPzUnutra[0], gPzUnutra[1], gPzUnutra[2], LIK_HOD);
}

static PizzaGostNaKasi(s, g)
{
    new lik = gPzGost[s][g][pgLik], Float:x, Float:y, Float:z;
    PizzaGostRed(g, x, y, z);
    Lik_Stani(lik);
    Lik_Postavi(lik, x, y, z);
    Lik_Kut(lik, gPzTocka[PT_KUPAC][3]);
    new stavka = random(sizeof gPzMeni - 1), n = PizzaNovaNarudzba(s, g, INVALID_PLAYER_ID, stavka);
    if (n == -1)
    {
        PizzaGostOdlazi(s, g);
        return;
    }
    gPzGost[s][g][pgNar] = n;
    gPzGost[s][g][pgOd] = gettime();
    ApplyActorAnimation(gPz[s][pzAktor], "PED", "IDLE_chat", 4.1, false, false, false, false, 2500);
    new t = random(2) ? PizzaStolSlobodan(s) : -1;
    if (t == -1)
    {
        gPzGost[s][g][pgFaza] = PG_KASA;
        return;
    }
    gPzGost[s][g][pgStol] = t;
    gPzGost[s][g][pgFaza] = PG_STOL_IDE;
    Lik_Idi(lik, gPzStol[t][0], gPzStol[t][1], gPzStol[t][2], LIK_HOD);
}

static PizzaGostSjedne(s, g)
{
    new lik = gPzGost[s][g][pgLik], t = gPzGost[s][g][pgStol];
    if (t < 0)
    {
        gPzGost[s][g][pgFaza] = PG_KASA;
        return;
    }
    Lik_Stani(lik);
    Lik_Postavi(lik, gPzStol[t][0], gPzStol[t][1], gPzStol[t][2]);
    Lik_Kut(lik, gPzStol[t][3]);
    Lik_Animacija(lik, "FOOD", "FF_Sit_In", 4.1, 0, 1, 1, 1, 0);
    gPzGost[s][g][pgFaza] = PG_STOL;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gPzSmjena[i] == s + 1 && gPzRad[i] >= 0 && gPzRad[i] == gPzGost[s][g][pgNar] && gPzNar[s][gPzRad[i]][pnStanje] == PN_NOSI) SetPlayerCheckpoint(i, gPzStol[t][0], gPzStol[t][1], gPzStol[t][2], 1.0);
    SetTimerEx("PizzaGostSjedi", 1400, false, "iii", s, g, lik);
}

forward PizzaGostSjedi(s, g, lik);
public PizzaGostSjedi(s, g, lik)
{
    if (gPzGost[s][g][pgLik] != lik || gPzGost[s][g][pgFaza] != PG_STOL) return;
    Lik_Animacija(lik, "FOOD", "FF_Sit_Loop", 4.1, 1, 1, 1, 1, 0);
}

PizzaStigao(lik)
{
    for (new s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma]) continue;
        for (new g = 0; g < PIZZA_GOSTIJU; g++)
        {
            if (gPzGost[s][g][pgLik] != lik) continue;
            switch (gPzGost[s][g][pgFaza])
            {
                case PG_DOLAZI: PizzaGostNaKasi(s, g);
                case PG_STOL_IDE: PizzaGostSjedne(s, g);
                case PG_ODLAZI: PizzaGostMakni(s, g);
            }
            return 1;
        }
    }
    return 0;
}

static bool:PizzaMeta(s, n, &Float:x, &Float:y, &Float:z)
{
    new i = gPzNar[s][n][pnIgrac];
    if (i != INVALID_PLAYER_ID) return GetPlayerPos(i, x, y, z), true;
    new g = gPzNar[s][n][pnGost];
    if (g < 0 || gPzGost[s][g][pgLik] == INVALID_LIK) return false;
    Lik_Pozicija(gPzGost[s][g][pgLik], x, y, z);
    return true;
}

static PizzaNarudzbaMakni(s, n)
{
    new r = gPzNar[s][n][pnRadi];
    if (r != INVALID_PLAYER_ID && gPzRad[r] == n && gPzSmjena[r] == s + 1) PizzaRadnikPusti(r);
    gPzNar[s][n][pnIma] = false;
}

static PizzaKupacOdlazi(s, n)
{
    new kupac = gPzNar[s][n][pnIgrac], g = gPzNar[s][n][pnGost], stavka = gPzNar[s][n][pnStavka];
    PizzaRadnicima(s, "Kupac s narudzbom #%d je otisao nezadovoljan.", gPzNar[s][n][pnBroj], true);
    PizzaNarudzbaMakni(s, n);
    if (kupac != INVALID_PLAYER_ID && IsPlayerConnected(kupac))
    {
        DajNovac(kupac, gPzCijena[stavka]);
        if (FirmaImaVlasnika(FIRMA_PIZZA, s)) FirmaProdaja(FIRMA_PIZZA, s, -gPzCijena[stavka], false);
        ObavijestGreska(kupac, T(kupac, "Radnik je pokvario vasu narudzbu. Novac vam je vracen."));
    }
    if (g >= 0 && gPzGost[s][g][pgFaza] != PG_NEMA && gPzGost[s][g][pgFaza] != PG_ODLAZI)
    {
        gPzGost[s][g][pgNar] = -1;
        PizzaGostOdlazi(s, g);
    }
}

IgraTdMakni(playerid)
{
    if (!gPzTdIma[playerid]) return;
    gPzTdIma[playerid] = false;
    for (new k = 0; k < PZTD_BROJ; k++) if (gPzTd[playerid][k] != PlayerText:INVALID_TEXT_DRAW) PlayerTextDrawDestroy(playerid, gPzTd[playerid][k]), gPzTd[playerid][k] = PlayerText:INVALID_TEXT_DRAW;
}

IgraPanel(playerid, const naslov[], Float:visina)
{
    IgraTdMakni(playerid);
    for (new k = 0; k < PZTD_BROJ; k++) gPzTd[playerid][k] = PlayerText:INVALID_TEXT_DRAW;
    gPzTdIma[playerid] = true;
    new Float:y = 400.0 - visina;
    gPzTd[playerid][PZTD_POZADINA] = TdSlika(playerid, PIZZA_TRAKA_X - 10.0, y, PIZZA_TRAKA_W + 20.0, visina, 0x101010D0);
    gPzTd[playerid][PZTD_NASLOV] = TdTekst(playerid, PIZZA_TRAKA_X, y + 4.0, naslov, PIZZA_BOJA, 0.24, 1.1, TEXT_DRAW_FONT_2);
    PlayerTextDrawShow(playerid, gPzTd[playerid][PZTD_POZADINA]);
    PlayerTextDrawShow(playerid, gPzTd[playerid][PZTD_NASLOV]);
}

IgraTraka(playerid, Float:od, Float:zelDo, boja1, boja2, boja3, const uputa[])
{
    new Float:y = 372.0;
    gPzTd[playerid][PZTD_DIO1] = TdSlika(playerid, PIZZA_TRAKA_X, y, PIZZA_TRAKA_W * od / 100.0, 12.0, boja1);
    gPzTd[playerid][PZTD_DIO2] = TdSlika(playerid, PIZZA_TRAKA_X + PIZZA_TRAKA_W * od / 100.0, y, PIZZA_TRAKA_W * (zelDo - od) / 100.0, 12.0, boja2);
    gPzTd[playerid][PZTD_DIO3] = TdSlika(playerid, PIZZA_TRAKA_X + PIZZA_TRAKA_W * zelDo / 100.0, y, PIZZA_TRAKA_W * (100.0 - zelDo) / 100.0, 12.0, boja3);
    gPzTd[playerid][PZTD_OZNAKA] = TdSlika(playerid, PIZZA_TRAKA_X, y - 3.0, 2.0, 18.0, 0xFFFFFFFF);
    gPzTd[playerid][PZTD_UPUTA] = TdTekst(playerid, PIZZA_TRAKA_X, y + 16.0, uputa, 0xFFFFFFFF, 0.2, 0.9, TEXT_DRAW_FONT_1);
    for (new k = PZTD_DIO1; k <= PZTD_UPUTA; k++) PlayerTextDrawShow(playerid, gPzTd[playerid][k]);
    gPzZelOd[playerid] = od;
    gPzZelDo[playerid] = zelDo;
}

IgraOznakaPomakni(playerid, Float:poz)
{
    new PlayerText:td = gPzTd[playerid][PZTD_OZNAKA];
    if (!gPzTdIma[playerid] || td == PlayerText:INVALID_TEXT_DRAW) return;
    PlayerTextDrawHide(playerid, td);
    PlayerTextDrawSetPos(playerid, td, PIZZA_TRAKA_X + poz / 100.0 * PIZZA_TRAKA_W - 1.0, 369.0);
    PlayerTextDrawShow(playerid, td);
}

static PizzaNaslov(playerid, const vrsta[], s, n, naslov[], len = sizeof naslov)
{
    format(naslov, len, "%s - #%d %s", T(playerid, vrsta), gPzNar[s][n][pnBroj], T(playerid, gPzMeni[gPzNar[s][n][pnStavka]]));
}

static PizzaIgraKraj(playerid)
{
    if (gPzTimer[playerid]) KillTimer(gPzTimer[playerid]);
    gPzTimer[playerid] = 0;
    IgraTdMakni(playerid);
    if (gPzIgra[playerid] != PI_NEMA)
    {
        TogglePlayerControllable(playerid, true);
        ClearAnimations(playerid, SYNC_ALL);
    }
    gPzIgra[playerid] = PI_NEMA;
}

static PizzaRadnikPusti(playerid)
{
    new s = gPzSmjena[playerid] - 1, n = gPzRad[playerid];
    PizzaIgraKraj(playerid);
    if (s >= 0 && n >= 0 && gPzNar[s][n][pnIma] && gPzNar[s][n][pnRadi] == playerid)
    {
        gPzNar[s][n][pnRadi] = INVALID_PLAYER_ID;
        if (gPzNar[s][n][pnStanje] == PN_NOSI) gPzNar[s][n][pnStanje] = PizzaPocetnoStanje(gPzNar[s][n][pnStavka]);
    }
    if (n >= 0)
    {
        RemovePlayerAttachedObject(playerid, PIZZA_NOSI_SLOT);
        DisablePlayerCheckpoint(playerid);
    }
    gPzRad[playerid] = -1;
}

static PizzaUzmi(playerid, s, stanje)
{
    new najbolji = -1;
    for (new n = 0; n < PIZZA_NARUDZBI; n++)
        if (gPzNar[s][n][pnIma] && gPzNar[s][n][pnStanje] == stanje && gPzNar[s][n][pnRadi] == INVALID_PLAYER_ID && (najbolji == -1 || gPzNar[s][n][pnOd] < gPzNar[s][najbolji][pnOd])) najbolji = n;
    if (najbolji == -1) return -1;
    gPzNar[s][najbolji][pnRadi] = playerid;
    gPzRad[playerid] = najbolji;
    gPzIgraOd[playerid] = gettime();
    gPzAkcija[playerid] = gettime();
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "INT_SHOP", "shop_cashier", 4.1, true, false, false, true, 0, SYNC_ALL);
    return najbolji;
}

static PizzaTrakaPocni(playerid, igra, const vrsta[], s, n, Float:od, Float:zelDo, boja1, boja3, const uputa[], Float:brzina)
{
    new naslov[64];
    gPzIgra[playerid] = igra;
    gPzIgraOd[playerid] = gettime();
    gPzPoz[playerid] = 0.0;
    gPzBrzina[playerid] = brzina;
    PizzaNaslov(playerid, vrsta, s, n, naslov);
    IgraPanel(playerid, naslov, 60.0);
    IgraTraka(playerid, od, zelDo, boja1, 0x27AE60FF, boja3, uputa);
    if (gPzTimer[playerid]) KillTimer(gPzTimer[playerid]);
    gPzTimer[playerid] = SetTimerEx("PizzaTrakaKorak", PIZZA_TRAKA_MS, true, "i", playerid);
}

static PizzaPeci(playerid, igra, s, n)
{
    new uputa[128], Float:od = float(30 + random(45));
    PizzaPecenjeUputa(playerid, s, n, uputa);
    PizzaTrakaPocni(playerid, igra, igra == PI_PEC ? ("Pec za pizzu") : ("Rostilj"), s, n, od, od + 18.0, 0xC0392BFF, 0x2C2C2CFF, uputa, 1.3 + float(random(5)) * 0.1 + float(gPzStrana[playerid]) * 0.3);
}

static PizzaPecenjeUputa(playerid, s, n, uputa[], len = sizeof uputa)
{
    format(uputa, len, T(playerid, "~y~SPACE~w~ kad je zeleno (%d/%d). ~r~Crveno~w~ - sirovo, ~l~crno~w~ - zagorjelo."), gPzStrana[playerid] + 1, gPzStrane[gPzNar[s][n][pnStavka]]);
}

static PizzaRostiljPocni(playerid, s)
{
    new n = PizzaUzmi(playerid, s, PN_ROSTILJ);
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za rostilj.")), 1;
    gPzStrana[playerid] = 0;
    PizzaPeci(playerid, PI_ROSTILJ, s, n);
    return 1;
}

static PizzaPomfritPocni(playerid, s)
{
    new n = PizzaUzmi(playerid, s, PN_POMFRIT);
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za pomfrit.")), 1;
    gPzStrana[playerid] = 0;
    new Float:od = float(30 + random(45));
    PizzaTrakaPocni(playerid, PI_POMFRIT, "Friteza", s, n, od, od + 18.0, 0xF4D03FFF, 0x6E2C00FF, T(playerid, "Izvadite pomfrit: ~y~SPACE~w~ kad je zlatan (zeleno)."), 1.4 + float(random(5)) * 0.1);
    return 1;
}

static PizzaSlaganjePocni(playerid, s)
{
    new n = PizzaUzmi(playerid, s, PN_SLAGANJE), naslov[64];
    if (n == -1) return ObavijestGreska(playerid, T(playerid, "Nema narudzbi za slaganje.")), 1;
    gPzIgra[playerid] = PI_SLAGANJE;
    gPzKorak[playerid] = 0;
    for (new k = 0; k < 7; k++) gPzSlojTipka[playerid][k] = random(sizeof gPzTipka);
    PizzaNaslov(playerid, "Slaganje", s, n, naslov);
    IgraPanel(playerid, naslov, 120.0);
    gPzTd[playerid][PZTD_SLOJEVI] = TdTekst(playerid, PIZZA_TRAKA_X, 300.0, " ", 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_1);
    gPzTd[playerid][PZTD_UPUTA] = TdTekst(playerid, PIZZA_TRAKA_X, 386.0, T(playerid, "Pritisnite tipku za sljedeci sloj. Kriva tipka - kupac odlazi!"), 0xFFFFFFFF, 0.2, 0.9, TEXT_DRAW_FONT_1);
    PlayerTextDrawShow(playerid, gPzTd[playerid][PZTD_UPUTA]);
    PizzaSlojevi(playerid, s, n);
    return 1;
}

static PizzaSlojevi(playerid, s, n)
{
    new tekst[320], red[64], stavka = gPzNar[s][n][pnStavka];
    for (new k = 0; k < 7 && gPzRecept[stavka][k] != -1; k++)
    {
        format(red, sizeof red, "%s%s %s [%s]~n~", k < gPzKorak[playerid] ? ("~g~") : k == gPzKorak[playerid] ? ("~y~") : ("~w~"), k == gPzKorak[playerid] ? (">") : ("-"), T(playerid, gPzSastojak[gPzRecept[stavka][k]]), gPzTipkaIme[gPzSlojTipka[playerid][k]]);
        strcat(tekst, red);
    }
    PlayerTextDrawSetString(playerid, gPzTd[playerid][PZTD_SLOJEVI], tekst);
    PlayerTextDrawShow(playerid, gPzTd[playerid][PZTD_SLOJEVI]);
}

static PizzaPakiranjePocni(playerid, s, n)
{
    new Float:od = float(15 + random(60));
    PizzaTrakaPocni(playerid, PI_PAKIRANJE, "Pakiranje", s, n, od, od + 16.0, 0x3A3A3AFF, 0x3A3A3AFF, T(playerid, "Zatvorite vrecicu: ~y~SPACE~w~ kad je oznaka u zelenom."), 2.6);
}

forward PizzaTrakaKorak(playerid);
public PizzaTrakaKorak(playerid)
{
    new igra = gPzIgra[playerid];
    if (igra != PI_ROSTILJ && igra != PI_POMFRIT && igra != PI_PEC && igra != PI_PAKIRANJE) return;
    gPzPoz[playerid] += gPzBrzina[playerid];
    if (igra == PI_PAKIRANJE)
    {
        if (gPzPoz[playerid] >= 100.0 || gPzPoz[playerid] <= 0.0) gPzBrzina[playerid] = -gPzBrzina[playerid];
    }
    else if (gPzPoz[playerid] >= 100.0)
    {
        PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
        PizzaRadnikPusti(playerid);
        IgraObavijest(playerid, T(playerid, "Pizzeria"), OBAVIJEST_GRESKA, igra == PI_PEC ? T(playerid, "Pizza je zagorjela! Slozite je ponovno.") : T(playerid, "Zagorjelo je! Pokusajte ponovno."));
        return;
    }
    IgraOznakaPomakni(playerid, gPzPoz[playerid]);
}

static PizzaGotovo(playerid, s, n)
{
    PizzaIgraKraj(playerid);
    gPzAkcija[playerid] = gettime();
    gPzNar[s][n][pnStanje] = PN_NOSI;
    SetPlayerAttachedObject(playerid, PIZZA_NOSI_SLOT, PIZZA_KUTIJA, 6, 0.1, 0.02, -0.05, 0.0, 0.0, 0.0);
    new Float:x, Float:y, Float:z, tekst[128];
    if (PizzaMeta(s, n, x, y, z)) SetPlayerCheckpoint(playerid, x, y, z, 1.0);
    format(tekst, sizeof tekst, T(playerid, "Narudzba #%d je spremna. Odnesite je kupcu (oznacen) i pritisnite ALT kraj njega."), gPzNar[s][n][pnBroj]);
    ObavijestInfo(playerid, T(playerid, "Pizzeria"), tekst);
}

bool:PizzaKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    new igra = gPzIgra[playerid];
    if (igra == PI_NEMA) return false;
    new s = gPzSmjena[playerid] - 1, n = gPzRad[playerid];
    if (s < 0 || n < 0)
    {
        PizzaRadnikPusti(playerid);
        return true;
    }
    new stavka = gPzNar[s][n][pnStavka];
    if (igra == PI_SLAGANJE)
    {
        new pritisnuta = -1;
        for (new k = 0; k < sizeof gPzTipka; k++) if ((newkeys & gPzTipka[k]) && !(oldkeys & gPzTipka[k])) pritisnuta = k;
        if (pritisnuta == -1) return true;
        if (pritisnuta != gPzSlojTipka[playerid][gPzKorak[playerid]])
        {
            PlayerPlaySound(playerid, 1055, 0.0, 0.0, 0.0);
            PizzaIgraKraj(playerid);
            gPzRad[playerid] = -1;
            PizzaKupacOdlazi(s, n);
            return true;
        }
        PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
        if (++gPzKorak[playerid] >= 7 || gPzRecept[stavka][gPzKorak[playerid]] == -1)
        {
            IgraTdMakni(playerid);
            if (gPzVrsta[stavka] == PV_PIZZA)
            {
                gPzStrana[playerid] = 0;
                PizzaPeci(playerid, PI_PEC, s, n);
            }
            else PizzaPakiranjePocni(playerid, s, n);
            return true;
        }
        PizzaSlojevi(playerid, s, n);
        return true;
    }
    if (!(newkeys & KEY_SPRINT) || (oldkeys & KEY_SPRINT)) return true;
    new bool:pogodak = gPzPoz[playerid] >= gPzZelOd[playerid] && gPzPoz[playerid] <= gPzZelDo[playerid];
    PlayerPlaySound(playerid, pogodak ? 1056 : 1055, 0.0, 0.0, 0.0);
    if (!pogodak) return true;
    if (igra == PI_PAKIRANJE)
    {
        PizzaGotovo(playerid, s, n);
        return true;
    }
    if (++gPzStrana[playerid] < gPzStrane[stavka])
    {
        PizzaPeci(playerid, igra, s, n);
        return true;
    }
    if (igra == PI_PEC || igra == PI_POMFRIT)
    {
        PizzaPakiranjePocni(playerid, s, n);
        return true;
    }
    PizzaIgraKraj(playerid);
    gPzAkcija[playerid] = gettime();
    gPzNar[s][n][pnStanje] = PN_SLAGANJE;
    gPzNar[s][n][pnRadi] = INVALID_PLAYER_ID;
    gPzRad[playerid] = -1;
    IgraObavijest(playerid, T(playerid, "Pizzeria"), PIZZA_BOJA, T(playerid, "Peceno! Narudzba ide na slaganje."));
    return true;
}

static PizzaPredaj(playerid, s, n)
{
    new Float:x, Float:y, Float:z;
    if (!PizzaMeta(s, n, x, y, z) || !IsPlayerInRangeOfPoint(playerid, PIZZA_PREDAJA_DOMET, x, y, z)) return 0;
    new stavka = gPzNar[s][n][pnStavka], kupac = gPzNar[s][n][pnIgrac], g = gPzNar[s][n][pnGost], tekst[128];
    gPzNar[s][n][pnIma] = false;
    gPzRad[playerid] = -1;
    gPzAkcija[playerid] = gettime();
    RemovePlayerAttachedObject(playerid, PIZZA_NOSI_SLOT);
    DisablePlayerCheckpoint(playerid);
    ApplyAnimation(playerid, "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0, SYNC_ALL);
    if (kupac != INVALID_PLAYER_ID)
    {
        PizzaJedi(kupac, stavka);
        format(tekst, sizeof tekst, T(kupac, "%s vam je donio narudzbu. Dobar tek!"), GetRPIme(playerid));
        ObavijestInfo(kupac, T(kupac, "Pizzeria"), tekst);
    }
    else
    {
        FirmaProdaja(FIRMA_PIZZA, s, gPzCijena[stavka], false);
        if (g >= 0 && gPzGost[s][g][pgLik] != INVALID_LIK)
        {
            gPzGost[s][g][pgNar] = -1;
            if (gPzGost[s][g][pgFaza] == PG_STOL)
            {
                gPzGost[s][g][pgFaza] = PG_JEDE;
                gPzGost[s][g][pgOd] = gettime();
                Lik_Animacija(gPzGost[s][g][pgLik], "FOOD", "FF_Sit_Eat1", 4.1, 1, 1, 1, 1, 0);
            }
            else PizzaGostOdlazi(s, g);
        }
    }
    PlayerPlaySound(playerid, 1052, 0.0, 0.0, 0.0);
    ObavijestInfo(playerid, T(playerid, "Pizzeria"), T(playerid, "Narudzba predana. Za rad u pizzeriji placu dobivate na placi."));
    return 1;
}

PizzaAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    new s = PizzaIgraca(playerid);
    if (s == -1) return 0;
    if (gPzShop[playerid] != -1) return 1;
    if (PizzaTu(playerid, PT_POSAO)) return PizzaIzbornik(playerid, s), 1;
    if (gPzSmjena[playerid] == s + 1)
    {
        new n = gPzRad[playerid];
        if (n >= 0 && gPzNar[s][n][pnStanje] == PN_NOSI)
        {
            if (PizzaPredaj(playerid, s, n)) return 1;
            if (PizzaTu(playerid, PT_ROSTILJ) || PizzaTu(playerid, PT_SLAGANJE) || PizzaTu(playerid, PT_POMFRIT)) return ObavijestGreska(playerid, T(playerid, "Prvo odnesite narudzbu koju nosite.")), 1;
        }
        else
        {
            if (PizzaTu(playerid, PT_ROSTILJ)) return PizzaRostiljPocni(playerid, s);
            if (PizzaTu(playerid, PT_POMFRIT)) return PizzaPomfritPocni(playerid, s);
            if (PizzaTu(playerid, PT_SLAGANJE)) return PizzaSlaganjePocni(playerid, s);
        }
    }
    if (PizzaTu(playerid, PT_KUPAC, PIZZA_KASA_DOMET + 1.0))
    {
        PizzaShopOtvori(playerid, s);
        return 1;
    }
    return 0;
}

static PizzaIzbornik(playerid, s)
{
    if (PosaoIgraca(playerid) != POSAO_PIZZA) return UpitDaNe(playerid, UPIT_PIZZA, s, 0, T(playerid, "Pizzeria"), T(playerid, "Zelite li se zaposliti u pizzeriji?"));
    if (gPzSmjena[playerid] != s + 1) return UpitDaNe(playerid, UPIT_PIZZA, s, 1, T(playerid, "Pizzeria"), T(playerid, "Zelite li zapoceti smjenu?"));
    return UpitDaNe(playerid, UPIT_PIZZA, s, 2, T(playerid, "Pizzeria"), T(playerid, "Zelite li zavrsiti smjenu?"));
}

static PizzaPresvuci(playerid, bool:radno)
{
    Fade(playerid, 0);
    SetTimerEx("PizzaPresvuciKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "iii", playerid, gAccountId[playerid], radno);
}

forward PizzaPresvuciKraj(playerid, racun, radno);
public PizzaPresvuciKraj(playerid, racun, radno)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    SetPlayerSkin(playerid, radno && gPzSmjena[playerid] ? PIZZA_SKIN : OdjecaSkinIgraca(playerid));
    Fade(playerid, FADE_SLOJEVA + 1);
}

PizzaUpit(playerid, s, opcija, bool:da)
{
    if (!da || PizzaIgraca(playerid) != s || !PizzaTu(playerid, PT_POSAO, PIZZA_DOMET + 2.0)) return 1;
    if (opcija == 0 && PosaoIgraca(playerid) != POSAO_PIZZA)
    {
        RibarKraj(playerid);
        PosaoPostavi(playerid, POSAO_PIZZA);
        return ObavijestInfo(playerid, T(playerid, "Pizzeria"), T(playerid, "Zaposleni ste u pizzeriji. Pritisnite ALT ovdje (ili /posao) za pocetak smjene."));
    }
    if (PosaoIgraca(playerid) != POSAO_PIZZA) return 1;
    if (opcija == 1 && gPzSmjena[playerid] != s + 1)
    {
        PizzaSmjenaKraj(playerid);
        gPzSmjena[playerid] = s + 1;
        gPzAkcija[playerid] = gettime();
        if (gPz[s][pzSljedeci] < gettime()) gPz[s][pzSljedeci] = gettime() + 5;
        PizzaPresvuci(playerid, true);
        return ObavijestInfo(playerid, T(playerid, "Pizzeria"), T(playerid, "Smjena je pocela. Pecite na rostilju, frizi i u peci, slozite i zapakirajte narudzbu, pa je odnesite kupcu."));
    }
    if (opcija == 2 && gPzSmjena[playerid] == s + 1)
    {
        PizzaSmjenaKraj(playerid, true);
        return ObavijestInfo(playerid, T(playerid, "Pizzeria"), T(playerid, "Zavrsili ste smjenu."));
    }
    return 1;
}

PizzaPosao(playerid)
{
    new s = PizzaIgraca(playerid);
    if (s == -1 || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !PizzaTu(playerid, PT_POSAO)) return 0;
    PizzaIzbornik(playerid, s);
    return 1;
}

PizzaPlaca(playerid)
{
    new iznos = PIZZA_PLACA_SAT * gPzRadio[playerid] / 3600;
    gPzRadio[playerid] = 0;
    return iznos;
}

static PizzaListaMakni(playerid)
{
    if (gPzListaIma[playerid]) PlayerTextDrawDestroy(playerid, gPzLista[playerid]);
    gPzListaIma[playerid] = false;
    gPzListaTekst[playerid][0] = EOS;
}

PizzaSmjenaKraj(playerid, bool:fade = false)
{
    if (!gPzSmjena[playerid]) return;
    PizzaRadnikPusti(playerid);
    PizzaListaMakni(playerid);
    gPzSmjena[playerid] = 0;
    if (!IsPlayerConnected(playerid)) return;
    if (fade) PizzaPresvuci(playerid, false);
    else SetPlayerSkin(playerid, OdjecaSkinIgraca(playerid));
}

PizzaDisconnect(playerid)
{
    gPzSmjena[playerid] = 0;
    gPzRad[playerid] = -1;
    gPzListaIma[playerid] = false;
    gPzIgra[playerid] = PI_NEMA;
    gPzTdIma[playerid] = false;
    gPzJede[playerid] = false;
    gPzSavjet[playerid] = -1;
    gPzRadio[playerid] = 0;
    gPzShop[playerid] = -1;
    gPzShopObj[playerid] = INVALID_OBJECT_ID;
    if (gPzTimer[playerid]) KillTimer(gPzTimer[playerid]);
    gPzTimer[playerid] = 0;
    for (new s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma]) continue;
        for (new n = 0; n < PIZZA_NARUDZBI; n++)
        {
            if (!gPzNar[s][n][pnIma]) continue;
            if (gPzNar[s][n][pnIgrac] == playerid) PizzaNarudzbaMakni(s, n);
            else if (gPzNar[s][n][pnRadi] == playerid)
            {
                gPzNar[s][n][pnRadi] = INVALID_PLAYER_ID;
                if (gPzNar[s][n][pnStanje] == PN_NOSI) gPzNar[s][n][pnStanje] = PizzaPocetnoStanje(gPzNar[s][n][pnStavka]);
            }
        }
    }
}

PizzaCheckpoint(playerid)
{
    return gPzSmjena[playerid] && gPzRad[playerid] >= 0 ? 1 : 0;
}

static PizzaListaOsvjezi(playerid, s)
{
    static const stanje[][] = { "~r~rostilj", "~y~friteza", "~y~slaganje", "~g~nosi se" };
    new tekst[256], red[64], n = 0;
    strcat(tekst, T(playerid, "~r~NARUDZBE~w~~n~"));
    for (new k = 0; k < PIZZA_NARUDZBI; k++)
    {
        if (!gPzNar[s][k][pnIma]) continue;
        format(red, sizeof red, "#%d %s - %s~w~%s~n~", gPzNar[s][k][pnBroj], T(playerid, gPzMeni[gPzNar[s][k][pnStavka]]), T(playerid, stanje[gPzNar[s][k][pnStanje]]), gPzNar[s][k][pnRadi] == playerid ? (" *") : (""));
        strcat(tekst, red);
        n++;
    }
    if (!n) strcat(tekst, T(playerid, "~w~Nema narudzbi."));
    if (!strcmp(tekst, gPzListaTekst[playerid])) return;
    strcopy(gPzListaTekst[playerid], tekst, sizeof gPzListaTekst[]);
    if (!gPzListaIma[playerid])
    {
        gPzLista[playerid] = TdTekst(playerid, 500.0, 150.0, tekst, 0xFFFFFFFF, 0.22, 1.0, TEXT_DRAW_FONT_1);
        PlayerTextDrawUseBox(playerid, gPzLista[playerid], true);
        PlayerTextDrawBoxColour(playerid, gPzLista[playerid], 0x101010B0);
        PlayerTextDrawTextSize(playerid, gPzLista[playerid], 630.0, 0.0);
        gPzListaIma[playerid] = true;
    }
    else PlayerTextDrawSetString(playerid, gPzLista[playerid], tekst);
    PlayerTextDrawShow(playerid, gPzLista[playerid]);
}

static PizzaSavjeti(playerid, s)
{
    new k = -1;
    if (gPzIgra[playerid] == PI_NEMA && gPzRad[playerid] == -1)
    {
        if (PizzaTu(playerid, PT_ROSTILJ)) k = PT_ROSTILJ;
        else if (PizzaTu(playerid, PT_POMFRIT)) k = PT_POMFRIT;
        else if (PizzaTu(playerid, PT_SLAGANJE)) k = PT_SLAGANJE;
    }
    if (k != -1 && k != gPzSavjet[playerid])
        Savjet(playerid, k == PT_ROSTILJ ? T(playerid, "~y~ALT~w~ - peci meso") : k == PT_POMFRIT ? T(playerid, "~y~ALT~w~ - przi pomfrit") : T(playerid, "~y~ALT~w~ - slozi, ispeci pizzu i zapakiraj"), 4000);
    gPzSavjet[playerid] = k;
    new n = gPzRad[playerid], Float:x, Float:y, Float:z;
    if (n < 0 || gPzNar[s][n][pnStanje] != PN_NOSI) return;
    new g = gPzNar[s][n][pnGost];
    if (gPzNar[s][n][pnIgrac] == INVALID_PLAYER_ID && (g < 0 || gPzGost[s][g][pgFaza] != PG_STOL_IDE)) return;
    if (PizzaMeta(s, n, x, y, z)) SetPlayerCheckpoint(playerid, x, y, z, 1.0);
}

PizzaTick()
{
    new sad = gettime();
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gPzSmjena[i]) continue;
        new s = gPzSmjena[i] - 1;
        if (!IsPlayerConnected(i) || PizzaIgraca(i) != s || PosaoIgraca(i) != POSAO_PIZZA || IsRanjen(i))
        {
            PizzaSmjenaKraj(i);
            if (IsPlayerConnected(i)) ObavijestInfo(i, T(i, "Pizzeria"), T(i, "Izasli ste iz pizzerije i zavrsili smjenu."));
            continue;
        }
        if (gPzIgra[i] != PI_NEMA && sad - gPzIgraOd[i] >= PIZZA_IGRA_S)
        {
            PizzaRadnikPusti(i);
            ObavijestGreska(i, T(i, "Predugo vam je trebalo. Narudzba se vraca na cekanje."));
        }
        if (sad - gPzAkcija[i] < PIZZA_AKTIVAN_S && gPzRadio[i] < 3600) gPzRadio[i]++;
        PizzaListaOsvjezi(i, s);
        PizzaSavjeti(i, s);
    }
    for (new s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma]) continue;
        new radnika = PizzaRadnika(s);
        if (radnika && sad >= gPz[s][pzSljedeci])
        {
            gPz[s][pzSljedeci] = sad + PIZZA_GOST_MIN + random(PIZZA_GOST_RASPON);
            PizzaGostNovi(s);
        }
        for (new n = 0; n < PIZZA_NARUDZBI; n++)
        {
            if (!gPzNar[s][n][pnIma]) continue;
            new kupac = gPzNar[s][n][pnIgrac];
            if (kupac == INVALID_PLAYER_ID) continue;
            if (!IsPlayerConnected(kupac) || PizzaIgraca(kupac) != s || sad - gPzNar[s][n][pnOd] >= PIZZA_IGRAC_CEKA || !radnika)
            {
                new stavka = gPzNar[s][n][pnStavka];
                PizzaNarudzbaMakni(s, n);
                if (IsPlayerConnected(kupac))
                {
                    PizzaJedi(kupac, stavka);
                    ObavijestInfo(kupac, T(kupac, "Pizzeria"), T(kupac, "Blagajnik vam je sam predao narudzbu. Dobar tek!"));
                }
            }
        }
        for (new g = 0; g < PIZZA_GOSTIJU; g++)
        {
            new faza = gPzGost[s][g][pgFaza], proslo = sad - gPzGost[s][g][pgOd];
            switch (faza)
            {
                case PG_DOLAZI: if (proslo >= PIZZA_HODA_S) PizzaGostNaKasi(s, g);
                case PG_STOL_IDE: if (proslo >= PIZZA_HODA_S * 2) PizzaGostSjedne(s, g);
                case PG_JEDE: if (proslo >= PIZZA_JEDE_S) PizzaGostOdlazi(s, g);
                case PG_ODLAZI: if (proslo >= PIZZA_HODA_S) PizzaGostMakni(s, g);
            }
            if ((faza == PG_KASA || faza == PG_STOL || faza == PG_STOL_IDE) && proslo >= PIZZA_STRPLJENJE)
            {
                new n = gPzGost[s][g][pgNar];
                if (n >= 0 && gPzNar[s][n][pnIma] && gPzNar[s][n][pnGost] == g) PizzaKupacOdlazi(s, n);
                else PizzaGostOdlazi(s, g);
            }
        }
    }
}

CMD:pizzaint(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    new s = 0;
    while (s < PIZZA_MAX && !gPz[s][pzIma]) s++;
    AdminPrebaci(playerid, PIZZA_INT, s < PIZZA_MAX ? PIZZA_SVIJET + s : 0, gPzUnutra[0], gPzUnutra[1], gPzUnutra[2]);
    return 1;
}

ShowPizzaAdmin(playerid)
{
    new list[3072], red[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova pizzeria (ulaz na mojoj poziciji)"));
    for (new t = 0; t < PT_BROJ; t++)
    {
        format(red, sizeof red, T(playerid, "Postavi: %s (moja pozicija u interijeru)"), T(playerid, gPzTockaIme[t]));
        DodajStupac(list, len, red);
    }
    format(red, sizeof red, T(playerid, "+ Dodaj stol (moja pozicija) - %d/%d"), gPzStolova, PIZZA_STOLOVA);
    DodajStupac(list, len, red);
    DodajStupac(list, len, T(playerid, "Udji u interijer pizzerije"));
    for (new t = 0; t < gPzStolova; t++)
    {
        format(red, sizeof red, T(playerid, "Stol %d - obrisi"), t + 1);
        DodajStupac(list, len, red);
        gPzApPopis[playerid][n++] = PIZZA_MAX + t;
    }
    for (new s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Pizzeria (%s) - obrisi"), gPz[s][pzDb], CestaZona(gPz[s][pzX], gPz[s][pzY]));
        DodajStupac(list, len, red);
        gPzApPopis[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_PIZZA, DIALOG_STYLE_LIST, T(playerid, "Pizzerije"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PizzaStolMakni(t)
{
    for (new s = 0; s < PIZZA_MAX; s++)
    {
        if (!gPz[s][pzIma]) continue;
        for (new g = 0; g < PIZZA_GOSTIJU; g++)
        {
            if (gPzGost[s][g][pgFaza] == PG_NEMA || gPzGost[s][g][pgStol] < 0) continue;
            if (gPzGost[s][g][pgStol] == t) PizzaGostOdlazi(s, g);
            else if (gPzGost[s][g][pgStol] > t) gPzGost[s][g][pgStol]--;
        }
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM pizza_tables WHERE id = %d", gPzStolDb[t]));
    for (new k = t; k < gPzStolova - 1; k++)
    {
        gPzStolDb[k] = gPzStolDb[k + 1];
        for (new j = 0; j < 4; j++) gPzStol[k][j] = gPzStol[k + 1][j];
    }
    gPzStolova--;
}

DLG:apanel_pizza(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new Float:x, Float:y, Float:z, Float:a, bool:unutra = PizzaIgraca(playerid) != -1;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    if (listitem == 0)
    {
        new s = 0;
        while (s < PIZZA_MAX && gPz[s][pzIma]) s++;
        if (s == PIZZA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ulaz se postavlja samo izvana.")), ShowPizzaAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO pizzerias (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        gPz[s][pzDb] = ZadnjiId();
        gPz[s][pzX] = x;
        gPz[s][pzY] = y;
        gPz[s][pzZ] = z;
        gPz[s][pzA] = a;
        PizzaStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pizzeria je postavljena."));
        return ShowPizzaAdmin(playerid);
    }
    if (listitem >= 1 && listitem <= PT_BROJ + 1)
    {
        if (!unutra) return ObavijestGreska(playerid, T(playerid, "Morate biti u interijeru pizzerije (/pizzaint).")), ShowPizzaAdmin(playerid);
        if (listitem <= PT_BROJ)
        {
            new t = listitem - 1;
            gPzTocka[t][0] = x;
            gPzTocka[t][1] = y;
            gPzTocka[t][2] = z;
            gPzTocka[t][3] = a;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO pizza_points (tip, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", t, x, y, z, a));
            for (new s = 0; s < PIZZA_MAX; s++) if (gPz[s][pzIma]) PizzaOznakeMakni(s), PizzaOznakeStvori(s);
            if (t == PT_KASA) SetPlayerPos(playerid, x + 1.2 * floatsin(-a, degrees), y + 1.2 * floatcos(-a, degrees), z);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Tocka je postavljena za sve pizzerije."));
            return ShowPizzaAdmin(playerid);
        }
        if (gPzStolova >= PIZZA_STOLOVA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowPizzaAdmin(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO pizza_tables (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        gPzStolDb[gPzStolova] = ZadnjiId();
        gPzStol[gPzStolova][0] = x;
        gPzStol[gPzStolova][1] = y;
        gPzStol[gPzStolova][2] = z;
        gPzStol[gPzStolova][3] = a;
        gPzStolova++;
        ApplyAnimation(playerid, "FOOD", "FF_Sit_Loop", 4.1, true, true, true, true, 0, SYNC_ALL);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stol je dodan. Ovako ce NPC sjediti; ako ne izgleda dobro, obrisite stol i postavite se drugacije."));
        return ShowPizzaAdmin(playerid);
    }
    if (listitem == PT_BROJ + 2)
    {
        new prazno[1];
        return cmd_pizzaint(playerid, prazno);
    }
    new i = listitem - PT_BROJ - 3;
    if (i < 0 || i >= sizeof gPzApPopis[]) return 1;
    new k = gPzApPopis[playerid][i];
    if (k >= PIZZA_MAX)
    {
        if (k - PIZZA_MAX < gPzStolova) PizzaStolMakni(k - PIZZA_MAX);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Stol je obrisan."));
        return ShowPizzaAdmin(playerid);
    }
    new s = k;
    if (!gPz[s][pzIma]) return ShowPizzaAdmin(playerid);
    for (new p = 0, iZadnji = gIgracZadnji; p <= iZadnji; p++)
    {
        if (gPzSmjena[p] == s + 1) PizzaSmjenaKraj(p);
        if (gPzShop[p] == s) PizzaShopZatvori(p);
    }
    for (new g = 0; g < PIZZA_GOSTIJU; g++) PizzaGostMakni(s, g);
    for (new n = 0; n < PIZZA_NARUDZBI; n++) gPzNar[s][n][pnIma] = false;
    PizzaOznakeMakni(s);
    DestroyDynamicPickup(gPz[s][pzPickup]);
    DestroyDynamicPickup(gPz[s][pzPickupU]);
    DestroyDynamic3DTextLabel(gPz[s][pzTekst]);
    DestroyDynamic3DTextLabel(gPz[s][pzTekstU]);
    FirmaMakni(FIRMA_PIZZA, s);
    gPz[s][pzIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM pizzerias WHERE id = %d", gPz[s][pzDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pizzeria je obrisana."));
    return ShowPizzaAdmin(playerid);
}
