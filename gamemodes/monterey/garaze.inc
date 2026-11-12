#define GARAZA_MAX          200
#define GARAZA_DOMET        7.0
#define GARAZA_ULAZ_DOMET   3.5
#define GARAZA_SJEDNI_MS    400
#define GARAZA_SVIJET       30000
#define GARAZA_INT          3
#define GARAZA_VOZILA       2
#define GARAZA_CIJENA       25000
#define GARAZA_OTKUP_POSTO  50
#define GARAZA_MAX_CIJENA   100000000

enum E_GARAZA { bool:gaIma, gaDb, Float:gaX, Float:gaY, Float:gaZ, Float:gaA, gaCijena, gaVlasnik, gaVlasnikIme[MAX_PLAYER_NAME + 1], bool:gaZakljucano,
    STREAMER_TAG_PICKUP:gaPickup, STREAMER_TAG_3D_TEXT_LABEL:gaTekst, STREAMER_TAG_PICKUP:gaPickupU, STREAMER_TAG_3D_TEXT_LABEL:gaTekstU }

static const Float:gGarazaUlaz[4] = { 612.2191, -123.9028, 997.9922, 270.0 };
static const Float:gGarazaMjesta[GARAZA_VOZILA][4] = { { 615.3, -122.9, 997.6, 90.0 }, { 615.3, -125.6, 997.6, 90.0 } };

static gGaraza[GARAZA_MAX][E_GARAZA];
static gGarazaPonuda[MAX_PLAYERS], gApGaraza[MAX_PLAYERS];

InitGaraze()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS garages (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, price INTEGER NOT NULL, owner INTEGER NOT NULL DEFAULT 0, locked INTEGER NOT NULL DEFAULT 1)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT g.id, g.x, g.y, g.z, g.a, g.price, g.owner, IFNULL(a.name, ''), g.locked FROM garages g LEFT JOIN accounts a ON a.id = g.owner ORDER BY g.id LIMIT %d", GARAZA_MAX);
    for (new g = 0, n = DB_GetRowCount(res); g < n; g++, DB_SelectNextRow(res))
    {
        gGaraza[g][gaDb] = DB_GetFieldInt(res, 0);
        gGaraza[g][gaX] = DB_GetFieldFloat(res, 1);
        gGaraza[g][gaY] = DB_GetFieldFloat(res, 2);
        gGaraza[g][gaZ] = DB_GetFieldFloat(res, 3);
        gGaraza[g][gaA] = DB_GetFieldFloat(res, 4);
        gGaraza[g][gaCijena] = DB_GetFieldInt(res, 5);
        gGaraza[g][gaVlasnik] = DB_GetFieldInt(res, 6);
        DB_GetFieldString(res, 7, gGaraza[g][gaVlasnikIme], MAX_PLAYER_NAME + 1);
        gGaraza[g][gaZakljucano] = DB_GetFieldInt(res, 8) != 0;
        GarazaStvori(g);
    }
    DB_FreeResultSet(res);
}

GarazaSvijet(db)
{
    return db ? GARAZA_SVIJET + db : 0;
}

bool:GarazaIzvana(db, &Float:x, &Float:y)
{
    for (new g = 0; g < GARAZA_MAX; g++)
    {
        if (!gGaraza[g][gaIma] || gGaraza[g][gaDb] != db) continue;
        x = gGaraza[g][gaX];
        y = gGaraza[g][gaY];
        return true;
    }
    return false;
}

GarazaSmjesti(v, db)
{
    SetVehicleVirtualWorld(v, GarazaSvijet(db));
    LinkVehicleToInterior(v, db ? GARAZA_INT : 0);
}

static GarazaStvori(g)
{
    new natpis[96], ime[MAX_PLAYER_NAME + 1];
    if (!gGaraza[g][gaVlasnik]) format(natpis, sizeof natpis, "Garaza na prodaju\n$%d\n[Y]", gGaraza[g][gaCijena]);
    else
    {
        strcopy(ime, gGaraza[g][gaVlasnikIme]);
        for (new i = 0; ime[i]; i++) if (ime[i] == '_') ime[i] = ' ';
        format(natpis, sizeof natpis, "Garaza\nVlasnik: %s\n[Y]", ime);
    }
    gGaraza[g][gaIma] = true;
    gGaraza[g][gaPickup] = CreateDynamicPickup(gGaraza[g][gaVlasnik] ? KUCA_PRODANA : KUCA_PRODAJA, 1, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ], 0, 0);
    gGaraza[g][gaTekst] = CreateDynamic3DTextLabel(natpis, COLOR_ORG, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
    new svijet = GarazaSvijet(gGaraza[g][gaDb]);
    gGaraza[g][gaPickupU] = CreateDynamicPickup(HQ_PICKUP, 1, gGarazaUlaz[0], gGarazaUlaz[1], gGarazaUlaz[2], svijet, GARAZA_INT);
    gGaraza[g][gaTekstU] = CreateDynamic3DTextLabel("Izlaz\n[Y]", COLOR_ORG, gGarazaUlaz[0], gGarazaUlaz[1], gGarazaUlaz[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, GARAZA_INT);
}

static GarazaMakni(g)
{
    DestroyDynamicPickup(gGaraza[g][gaPickup]);
    DestroyDynamic3DTextLabel(gGaraza[g][gaTekst]);
    DestroyDynamicPickup(gGaraza[g][gaPickupU]);
    DestroyDynamic3DTextLabel(gGaraza[g][gaTekstU]);
    gGaraza[g][gaIma] = false;
}

static GarazaOsvjezi(g)
{
    GarazaMakni(g);
    GarazaStvori(g);
}

static GarazaPoSvijetu(svijet)
{
    if (svijet <= GARAZA_SVIJET) return -1;
    for (new g = 0; g < GARAZA_MAX; g++) if (gGaraza[g][gaIma] && GarazaSvijet(gGaraza[g][gaDb]) == svijet) return g;
    return -1;
}

static GarazaIgraca(playerid)
{
    return GetPlayerInterior(playerid) == GARAZA_INT ? GarazaPoSvijetu(GetPlayerVirtualWorld(playerid)) : -1;
}

static GarazaNajbliza(playerid)
{
    new najbliza = -1, Float:naj = GARAZA_DOMET;
    for (new g = 0; g < GARAZA_MAX; g++)
    {
        if (!gGaraza[g][gaIma]) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ]);
        if (d > naj) continue;
        naj = d;
        najbliza = g;
    }
    return najbliza;
}

static GarazaPrebaci(v, Float:x, Float:y, Float:z, Float:a, db)
{
    new putnici[MAX_PLAYERS], sjedala[MAX_PLAYERS], n = 0, svijet = GarazaSvijet(db), interijer = db ? GARAZA_INT : 0;
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerInVehicle(i, v)) continue;
        putnici[n] = i;
        sjedala[n++] = GetPlayerVehicleSeat(i);
    }
    SetVehiclePos(v, x, y, z);
    SetVehicleZAngle(v, a);
    SetVehicleVelocity(v, 0.0, 0.0, 0.0);
    GarazaSmjesti(v, db);
    for (new i = 0; i < n; i++)
    {
        SetPlayerInterior(putnici[i], interijer);
        SetPlayerVirtualWorld(putnici[i], svijet);
        SetPlayerPos(putnici[i], x, y, z + 1.0);
        PutPlayerInVehicle(putnici[i], v, sjedala[i]);
        SetTimerEx("GarazaSjedni", GARAZA_SJEDNI_MS, false, "iiii", putnici[i], v, sjedala[i], svijet);
    }
}

forward GarazaSjedni(playerid, v, sjedalo, svijet);
public GarazaSjedni(playerid, v, sjedalo, svijet)
{
    if (!IsPlayerConnected(playerid) || !GetVehicleModel(v) || GetVehicleVirtualWorld(v) != svijet || GetPlayerVirtualWorld(playerid) != svijet) return;
    if (GetPlayerVehicleID(playerid) != v) PutPlayerInVehicle(playerid, v, sjedalo);
    SetCameraBehindPlayer(playerid);
}

static GarazaMjesto(db, v)
{
    new zauzeto[GARAZA_VOZILA];
    for (new i = 0; i < gVozilaCount; i++)
    {
        new o = gVozila[i], Float:px, Float:py, Float:pz, Float:pa;
        if (o == v || VlasnikGarazaDb(o) != db) continue;
        VlasnikParking(o, px, py, pz, pa);
        for (new m = 0; m < GARAZA_VOZILA; m++) if (VectorSize(px - gGarazaMjesta[m][0], py - gGarazaMjesta[m][1], 0.0) < 1.0) zauzeto[m] = 1;
    }
    for (new m = 0; m < GARAZA_VOZILA; m++) if (!zauzeto[m]) return m;
    return -1;
}

static GarazaBroj(db, v)
{
    new n = 0;
    for (new i = 0; i < gVozilaCount; i++) if (gVozila[i] != v && VlasnikGarazaDb(gVozila[i]) == db) n++;
    return n;
}

static GarazaUvezi(playerid, g, v)
{
    new db = gGaraza[g][gaDb], Float:x, Float:y, Float:z, Float:a;
    if (VlasnikGarazaDb(v) == db) VlasnikParking(v, x, y, z, a);
    else
    {
        if (GarazaBroj(db, v) >= GARAZA_VOZILA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza je puna (najvise %d vozila)."), GARAZA_VOZILA);
        new m = GarazaMjesto(db, v);
        if (m == -1) m = 0;
        x = gGarazaMjesta[m][0];
        y = gGarazaMjesta[m][1];
        z = gGarazaMjesta[m][2];
        a = gGarazaMjesta[m][3];
        VlasnikGaraza(v, db, x, y, z, a);
    }
    GarazaPrebaci(v, x, y, z, a, db);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vozilo je parkirano u garazi. Y za izlazak."));
}

GarazaParkiraj(playerid, v)
{
    new g = GarazaIgraca(playerid);
    if (g == -1) return 0;
    new db = gGaraza[g][gaDb], Float:x, Float:y, Float:z, Float:a;
    if (VlasnikGarazaDb(v) != db && GarazaBroj(db, v) >= GARAZA_VOZILA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza je puna (najvise %d vozila)."), GARAZA_VOZILA), 1;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    VlasnikGaraza(v, db, x, y, z, a);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vozilo je parkirano ovdje."));
    return 1;
}

static GarazaUdi(playerid, g)
{
    SetPlayerInterior(playerid, GARAZA_INT);
    SetPlayerVirtualWorld(playerid, GarazaSvijet(gGaraza[g][gaDb]));
    SetPlayerPos(playerid, gGarazaUlaz[0], gGarazaUlaz[1], gGarazaUlaz[2]);
    SetPlayerFacingAngle(playerid, gGarazaUlaz[3]);
    SetCameraBehindPlayer(playerid);
}

static GarazaVani(playerid, g)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ]);
    SetPlayerFacingAngle(playerid, gGaraza[g][gaA] + 180.0);
    SetCameraBehindPlayer(playerid);
}

static GarazaIsprazni(g)
{
    new db = gGaraza[g][gaDb], svijet = GarazaSvijet(db);
    for (new i = 0; i < gVozilaCount; i++)
    {
        new v = gVozila[i];
        if (VlasnikGarazaDb(v) != db) continue;
        VlasnikGaraza(v, 0, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ], gGaraza[g][gaA]);
        if (GetVehicleVirtualWorld(v) == svijet) GarazaPrebaci(v, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ], gGaraza[g][gaA], 0);
    }
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && GetPlayerVirtualWorld(i) == svijet && !IsPlayerInAnyVehicle(i)) GarazaVani(i, g);
}

static GarazaPostaviVlasnika(g, racun, const ime[])
{
    gGaraza[g][gaVlasnik] = racun;
    gGaraza[g][gaVlasnikIme][0] = EOS;
    if (racun) strcopy(gGaraza[g][gaVlasnikIme], ime, MAX_PLAYER_NAME + 1);
    gGaraza[g][gaZakljucano] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE garages SET owner = %d, locked = 1 WHERE id = %d", racun, gGaraza[g][gaDb]));
    GarazaOsvjezi(g);
}

static GarazaMeni(playerid, g)
{
    new body[256];
    gGarazaPonuda[playerid] = g + 1;
    format(body, sizeof body, T(playerid, "Udji u garazu\n%s\nProdaj gradu ($%d)"), gGaraza[g][gaZakljucano] ? T(playerid, "Otkljucaj") : T(playerid, "Zakljucaj"), gGaraza[g][gaCijena] * GARAZA_OTKUP_POSTO / 100);
    return PrikaziDialog(playerid, DLG_GARAZA_MENI, DIALOG_STYLE_LIST, T(playerid, "Garaza"), body, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static GarazaPrebaciBravu(playerid, g)
{
    gGaraza[g][gaZakljucano] = !gGaraza[g][gaZakljucano];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE garages SET locked = %d WHERE id = %d", gGaraza[g][gaZakljucano] ? 1 : 0, gGaraza[g][gaDb]));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gGaraza[g][gaZakljucano] ? ("* %s zakljucava garazu.") : ("* %s otkljucava garazu."), GetRPIme(playerid), "", 1);
}

GarazaZakljucaj(playerid)
{
    new g = GarazaIgraca(playerid);
    if (g == -1 && !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid)) g = GarazaNajbliza(playerid);
    if (g == -1 || !gGaraza[g][gaVlasnik] || gGaraza[g][gaVlasnik] != gAccountId[playerid]) return 0;
    GarazaPrebaciBravu(playerid, g);
    return 1;
}

GarazaKey(playerid)
{
    if (!gUlogiran[playerid]) return 0;
    new PLAYER_STATE:st = GetPlayerState(playerid), g = GarazaIgraca(playerid);
    if (g != -1)
    {
        if (st == PLAYER_STATE_DRIVER)
        {
            GarazaPrebaci(GetPlayerVehicleID(playerid), gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ], gGaraza[g][gaA], 0);
            return 1;
        }
        if (st != PLAYER_STATE_ONFOOT || !IsPlayerInRangeOfPoint(playerid, GARAZA_ULAZ_DOMET, gGarazaUlaz[0], gGarazaUlaz[1], gGarazaUlaz[2])) return 0;
        GarazaVani(playerid, g);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || (st != PLAYER_STATE_DRIVER && st != PLAYER_STATE_ONFOOT)) return 0;
    g = GarazaNajbliza(playerid);
    if (g == -1) return 0;
    new bool:vlasnik = gGaraza[g][gaVlasnik] && gGaraza[g][gaVlasnik] == gAccountId[playerid];
    if (st == PLAYER_STATE_DRIVER)
    {
        new v = GetPlayerVehicleID(playerid);
        if (!gGaraza[g][gaVlasnik]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza je na prodaju. Izadite iz vozila da je kupite.")), 1;
        if (gGaraza[g][gaZakljucano] && !vlasnik) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza je zakljucana.")), 1;
        if (!VlasnikDb(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U garazu se mogu parkirati samo vlastita vozila.")), 1;
        GarazaUvezi(playerid, g, v);
        return 1;
    }
    if (!gGaraza[g][gaVlasnik])
    {
        new body[128];
        gGarazaPonuda[playerid] = g + 1;
        format(body, sizeof body, T(playerid, "Kupiti ovu garazu za $%d?"), gGaraza[g][gaCijena]);
        PrikaziDialog(playerid, DLG_GARAZA_KUPI, DIALOG_STYLE_MSGBOX, T(playerid, "Garaza na prodaju"), body, T(playerid, "Kupi"), T(playerid, "Odustani"));
        return 1;
    }
    if (vlasnik) return GarazaMeni(playerid, g), 1;
    if (gGaraza[g][gaZakljucano]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza je zakljucana.")), 1;
    GarazaUdi(playerid, g);
    return 1;
}

DLG:garaza_kupi(playerid, response)
{
    new g = gGarazaPonuda[playerid] - 1;
    gGarazaPonuda[playerid] = 0;
    if (!response || g < 0 || !gGaraza[g][gaIma]) return 1;
    if (gGaraza[g][gaVlasnik]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garazu je netko vec kupio."));
    if (!IsPlayerInRangeOfPoint(playerid, GARAZA_DOMET * 2.0, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ])) return 1;
    if (!DajNovac(playerid, -gGaraza[g][gaCijena])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    new ime[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, ime, sizeof ime);
    GarazaPostaviVlasnika(g, gAccountId[playerid], ime);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste garazu za $%d."), gGaraza[g][gaCijena]);
}

DLG:garaza_meni(playerid, response, listitem)
{
    new g = gGarazaPonuda[playerid] - 1;
    if (!response || g < 0 || !gGaraza[g][gaIma] || gGaraza[g][gaVlasnik] != gAccountId[playerid]) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, GARAZA_DOMET * 2.0, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ])) return 1;
    if (listitem == 0) GarazaUdi(playerid, g);
    else if (listitem == 1) GarazaPrebaciBravu(playerid, g);
    else if (listitem == 2)
    {
        new body[128];
        format(body, sizeof body, T(playerid, "Prodati garazu gradu za $%d? Vozila iz nje ce biti parkirana ispred."), gGaraza[g][gaCijena] * GARAZA_OTKUP_POSTO / 100);
        return PrikaziDialog(playerid, DLG_GARAZA_PRODAJ, DIALOG_STYLE_MSGBOX, T(playerid, "Garaza"), body, T(playerid, "Prodaj"), T(playerid, "Odustani"));
    }
    return 1;
}

DLG:garaza_prodaj(playerid, response)
{
    new g = gGarazaPonuda[playerid] - 1;
    gGarazaPonuda[playerid] = 0;
    if (!response || g < 0 || !gGaraza[g][gaIma] || gGaraza[g][gaVlasnik] != gAccountId[playerid]) return 1;
    new iznos = gGaraza[g][gaCijena] * GARAZA_OTKUP_POSTO / 100;
    if (!DajNovac(playerid, iznos)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete primiti toliko novca."));
    GarazaIsprazni(g);
    GarazaPostaviVlasnika(g, 0, "");
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste garazu gradu za $%d."), iznos);
}

ShowGarazeAdmin(playerid)
{
    new list[2048], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Nova garaza na mojoj poziciji"));
    for (new g = 0; g < GARAZA_MAX; g++)
    {
        if (!gGaraza[g][gaIma]) continue;
        format(row, sizeof row, "[%d] %s - $%d%s", gGaraza[g][gaDb], CestaZona(gGaraza[g][gaX], gGaraza[g][gaY]), gGaraza[g][gaCijena], gGaraza[g][gaVlasnik] ? T(playerid, " - prodana") : T(playerid, ""));
        if (!DodajStupac(list, len, row)) break;
    }
    return PrikaziDialog(playerid, DLG_APANEL_GARAZA, DIALOG_STYLE_LIST, T(playerid, "Garaze"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static GarazaNova(playerid)
{
    new g = 0;
    while (g < GARAZA_MAX && gGaraza[g][gaIma]) g++;
    if (g == GARAZA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Garaza se postavlja samo izvana."));
    new v = GetPlayerVehicleID(playerid);
    if (v)
    {
        GetVehiclePos(v, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ]);
        GetVehicleZAngle(v, gGaraza[g][gaA]);
    }
    else
    {
        GetPlayerPos(playerid, gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ]);
        GetPlayerFacingAngle(playerid, gGaraza[g][gaA]);
    }
    gGaraza[g][gaCijena] = GARAZA_CIJENA;
    gGaraza[g][gaVlasnik] = 0;
    gGaraza[g][gaVlasnikIme][0] = EOS;
    gGaraza[g][gaZakljucano] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO garages (x, y, z, a, price) VALUES (%f, %f, %f, %f, %d)", gGaraza[g][gaX], gGaraza[g][gaY], gGaraza[g][gaZ], gGaraza[g][gaA], gGaraza[g][gaCijena]));
    gGaraza[g][gaDb] = ZadnjiId();
    GarazaStvori(g);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Garaza dodana ($%d). Postavite joj cijenu."), gGaraza[g][gaCijena]);
}

DLG:apanel_garaza(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        GarazaNova(playerid);
        return ShowGarazeAdmin(playerid);
    }
    new n = listitem;
    for (new g = 0; g < GARAZA_MAX; g++)
    {
        if (!gGaraza[g][gaIma] || --n) continue;
        gApGaraza[playerid] = g;
        return PrikaziDialog(playerid, DLG_APANEL_GARAZA_MENI, DIALOG_STYLE_LIST, T(playerid, "Garaze"), T(playerid, "Postavi cijenu\nOslobodi (makni vlasnika)\nObrisi garazu"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    return ShowGarazeAdmin(playerid);
}

DLG:apanel_garaza_meni(playerid, response, listitem)
{
    new g = gApGaraza[playerid];
    if (!response || g < 0 || g >= GARAZA_MAX || !gGaraza[g][gaIma]) return ShowGarazeAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0) return PrikaziDialog(playerid, DLG_APANEL_GARAZA_CIJENA, DIALOG_STYLE_INPUT, T(playerid, "Garaze"), T(playerid, "Unesite novu cijenu garaze:"), T(playerid, "Spremi"), T(playerid, "Natrag"));
    if (listitem == 1)
    {
        GarazaIsprazni(g);
        GarazaPostaviVlasnika(g, 0, "");
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Garaza je oslobodena."));
        return ShowGarazeAdmin(playerid);
    }
    return PrikaziDialog(playerid, DLG_APANEL_GARAZA_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Garaze"), T(playerid, "Obrisati ovu garazu? Vozila iz nje ce biti parkirana ispred."), T(playerid, "Obrisi"), T(playerid, "Natrag"));
}

DLG:apanel_garaza_cijena(playerid, response, const inputtext[])
{
    new g = gApGaraza[playerid];
    if (!response || g < 0 || g >= GARAZA_MAX || !gGaraza[g][gaIma]) return ShowGarazeAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 9 || strval(inputtext) < 1 || strval(inputtext) > GARAZA_MAX_CIJENA)
        return PrikaziDialog(playerid, DLG_APANEL_GARAZA_CIJENA, DIALOG_STYLE_INPUT, T(playerid, "Garaze"), T(playerid, "Unesite novu cijenu garaze:"), T(playerid, "Spremi"), T(playerid, "Natrag"));
    gGaraza[g][gaCijena] = strval(inputtext);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE garages SET price = %d WHERE id = %d", gGaraza[g][gaCijena], gGaraza[g][gaDb]));
    GarazaOsvjezi(g);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Cijena garaze je $%d."), gGaraza[g][gaCijena]);
    return ShowGarazeAdmin(playerid);
}

DLG:apanel_garaza_brisi(playerid, response)
{
    new g = gApGaraza[playerid];
    if (response && ProvjeriOvlasti(playerid, ALVL_ORG) && g >= 0 && g < GARAZA_MAX && gGaraza[g][gaIma])
    {
        GarazaIsprazni(g);
        GarazaMakni(g);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM garages WHERE id = %d", gGaraza[g][gaDb]));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Garaza je obrisana."));
    }
    return ShowGarazeAdmin(playerid);
}
