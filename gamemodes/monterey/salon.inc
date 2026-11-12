#define SALON_MAX           8
#define SALON_PONUDA        24
#define SALON_DOMET         2.0
#define SALON_SVIJET        7000
#define SALON_INT           1
#define SALON_TD            5
#define VLASNISTVO_MAX      5

enum { STD_LIJEVO, STD_DESNO, STD_INFO, STD_KUPI, STD_IZLAZ }
enum E_SALON { bool:slIma, slDb, Float:slX, Float:slY, Float:slZ, Float:slA, Float:slSx, Float:slSy, Float:slSz, Float:slSa,
    slBroj, slModel[SALON_PONUDA], slCijena[SALON_PONUDA], STREAMER_TAG_PICKUP:slPickup, STREAMER_TAG_3D_TEXT_LABEL:slTekst }

static gSalon[SALON_MAX][E_SALON];
static gApSalon[MAX_PLAYERS];
static gUSalonu[MAX_PLAYERS] = { -1, ... }, gSalonIdx[MAX_PLAYERS], gSalonVozilo[MAX_PLAYERS] = { INVALID_VEHICLE_ID, ... }, PlayerText:gSalonTd[MAX_PLAYERS][SALON_TD];
static gVlasnik[MAX_VEHICLES], gVlasnikDb[MAX_VEHICLES], gVlasnikCijena[MAX_VEHICLES], Float:gVlasnikPark[MAX_VEHICLES][4];

static const Float:gSalonAuto[4] = { 617.538, -1.990, 1001.2, 180.0 };
static const Float:gSalonKamera[3] = { 620.9, -7.6, 1002.4 };

static const gSalonPocetna[][2] = {
    { 410, 7000 }, { 404, 9000 }, { 492, 11000 }, { 547, 12000 }, { 540, 13000 }, { 550, 14000 },
    { 445, 15000 }, { 516, 16000 }, { 426, 18000 }, { 400, 32000 }, { 562, 55000 }, { 560, 60000 }
};

InitSalon()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS dealerships (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, sx REAL NOT NULL, sy REAL NOT NULL, sz REAL NOT NULL, sa REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS dealership_cars (dealer INTEGER NOT NULL, model INTEGER NOT NULL, price INTEGER NOT NULL, PRIMARY KEY (dealer, model))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS player_vehicles (id INTEGER PRIMARY KEY AUTOINCREMENT, owner INTEGER NOT NULL, model INTEGER NOT NULL, c1 INTEGER NOT NULL, c2 INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, price INTEGER NOT NULL DEFAULT 0, spot INTEGER NOT NULL DEFAULT 0)"));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a, sx, sy, sz, sa FROM dealerships ORDER BY id LIMIT %d", SALON_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++)
    {
        gSalon[s][slDb] = DB_GetFieldInt(res, 0);
        gSalon[s][slX] = DB_GetFieldFloat(res, 1);
        gSalon[s][slY] = DB_GetFieldFloat(res, 2);
        gSalon[s][slZ] = DB_GetFieldFloat(res, 3);
        gSalon[s][slA] = DB_GetFieldFloat(res, 4);
        gSalon[s][slSx] = DB_GetFieldFloat(res, 5);
        gSalon[s][slSy] = DB_GetFieldFloat(res, 6);
        gSalon[s][slSz] = DB_GetFieldFloat(res, 7);
        gSalon[s][slSa] = DB_GetFieldFloat(res, 8);
        SalonStvori(s);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT id, owner, model, c1, c2, x, y, z, a, price FROM player_vehicles");
    if (DB_GetRowCount(res)) do
    {
        VlasnickoStvori(DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 1), DB_GetFieldInt(res, 2), DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 4),
            DB_GetFieldFloat(res, 5), DB_GetFieldFloat(res, 6), DB_GetFieldFloat(res, 7), DB_GetFieldFloat(res, 8), DB_GetFieldInt(res, 9));
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

static SalonPonuda(s)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT model, price FROM dealership_cars WHERE dealer = %d ORDER BY price LIMIT %d", gSalon[s][slDb], SALON_PONUDA);
    gSalon[s][slBroj] = DB_GetRowCount(res);
    for (new i = 0; i < gSalon[s][slBroj]; i++)
    {
        gSalon[s][slModel][i] = DB_GetFieldInt(res, 0);
        gSalon[s][slCijena][i] = DB_GetFieldInt(res, 1);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static SalonStvori(s)
{
    gSalon[s][slIma] = true;
    gSalon[s][slPickup] = CreateDynamicPickup(HQ_PICKUP, 1, gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ], 0, 0);
    gSalon[s][slTekst] = CreateDynamic3DTextLabel("Salon automobila\n[Y]", COLOR_ORG, gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ] + 0.5, 15.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    SalonPonuda(s);
}

static VlasnickoStvori(db, owner, model, c1, c2, Float:x, Float:y, Float:z, Float:a, cijena)
{
    new v = CreateVehicle(model, x, y, z, a, c1, c2, -1);
    if (v == INVALID_VEHICLE_ID) return v;
    gVlasnik[v] = owner;
    gVlasnikDb[v] = db;
    gVlasnikCijena[v] = cijena;
    gVlasnikPark[v][0] = x;
    gVlasnikPark[v][1] = y;
    gVlasnikPark[v][2] = z;
    gVlasnikPark[v][3] = a;
    TuningPrimijeni(v);
    return v;
}

VlasnikVozila(v)
{
    return v > 0 && v < MAX_VEHICLES ? gVlasnik[v] : 0;
}

VlasnikMakni(v)
{
    if (v > 0 && v < MAX_VEHICLES) gVlasnik[v] = gVlasnikDb[v] = gVlasnikCijena[v] = 0;
}

VlasnickoSpawn(v)
{
    if (!gVlasnik[v]) return;
    SetVehiclePos(v, gVlasnikPark[v][0], gVlasnikPark[v][1], gVlasnikPark[v][2]);
    SetVehicleZAngle(v, gVlasnikPark[v][3]);
    if (gVlasnik[v] == VLASNIK_PIJACA) VoziloZakljucaj(v, true);
    TuningPrimijeni(v);
}

static SalonCijenaModela(model)
{
    new najniza = 0;
    for (new s = 0; s < SALON_MAX; s++)
    {
        if (!gSalon[s][slIma]) continue;
        for (new i = 0; i < gSalon[s][slBroj]; i++)
            if (gSalon[s][slModel][i] == model && (!najniza || gSalon[s][slCijena][i] < najniza)) najniza = gSalon[s][slCijena][i];
    }
    return najniza;
}

VlasnikCijena(v)
{
    if (!VlasnikVozila(v)) return 0;
    return gVlasnikCijena[v] ? gVlasnikCijena[v] : SalonCijenaModela(GetVehicleModel(v));
}

VlasnikParkiraj(v, Float:x, Float:y, Float:z, Float:a)
{
    gVlasnikPark[v][0] = x;
    gVlasnikPark[v][1] = y;
    gVlasnikPark[v][2] = z;
    gVlasnikPark[v][3] = a;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET x = %f, y = %f, z = %f, a = %f WHERE id = %d", x, y, z, a, gVlasnikDb[v]));
}

VlasnikPoDb(db)
{
    for (new i = 0; i < gVozilaCount; i++) if (gVlasnikDb[gVozila[i]] == db && gVlasnik[gVozila[i]]) return gVozila[i];
    return 0;
}

VlasnikOtpad(v)
{
    if (!VlasnikVozila(v)) return;
    TuningMakni(v);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_vehicles WHERE id = %d", gVlasnikDb[v]));
    DestroyVehicle(v);
}

VlasnikIme(v, ime[], size = sizeof (ime))
{
    if (!VlasnikVozila(v)) return false;
    if (gVlasnik[v] == VLASNIK_PIJACA)
    {
        ime[0] = EOS;
        strcat(ime, "Auto pijaca", size);
        return true;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT name FROM accounts WHERE id = %d", gVlasnik[v]);
    new ima = DB_GetRowCount(res) > 0;
    if (ima) DB_GetFieldString(res, 0, ime, size);
    DB_FreeResultSet(res);
    for (new i = 0; ime[i]; i++) if (ime[i] == '_') ime[i] = ' ';
    return ima;
}

VlasnistvaIgraca(playerid, vozila[], max = sizeof (vozila))
{
    new n = 0;
    for (new i = 0; i < gVozilaCount && n < max; i++) if (gVlasnik[gVozila[i]] && gVlasnik[gVozila[i]] == gAccountId[playerid]) vozila[n++] = gVozila[i];
    return n;
}

bool:VlasnikPrenesi(v, racun)
{
    if (!gVlasnik[v] || !racun) return false;
    gVlasnik[v] = racun;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET owner = %d WHERE id = %d", racun, gVlasnikDb[v]));
    return true;
}

VlasnikDb(v)
{
    return v > 0 && v < MAX_VEHICLES ? gVlasnikDb[v] : 0;
}

BrojVlasnistava(racun)
{
    new n = 0;
    for (new i = 0; i < gVozilaCount; i++) if (gVlasnik[gVozila[i]] == racun) n++;
    return n;
}

CMD:parkiraj(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    new v = GetPlayerVehicleID(playerid);
    if (!v || GetPlayerState(playerid) != PLAYER_STATE_DRIVER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Morate voziti."));
    if (gVlasnik[v] != gAccountId[playerid] || !gVlasnik[v]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo nije vase vozilo."));
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se parkira samo izvana."));
    new Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    if (PijacaZona(x, y, z)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Na auto pijaci se ne moze parkirati."));
    VlasnikParkiraj(v, x, y, z, a);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vozilo je parkirano ovdje."));
}

CMD:park(playerid, params[]) return cmd_parkiraj(playerid, params);

// Katalog

SalonUlaz(playerid)
{
    if (!gUlogiran[playerid] || gUSalonu[playerid] != -1 || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || ImaLisice(playerid)) return 0;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    for (new s = 0; s < SALON_MAX; s++)
    {
        if (!gSalon[s][slIma] || !IsPlayerInRangeOfPoint(playerid, SALON_DOMET, gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ])) continue;
        if (!gSalon[s][slBroj]) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Salon trenutno nema vozila u ponudi."));
        else SalonOtvori(playerid, s);
        return 1;
    }
    return 0;
}

PlayerText:SalonGumb(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, bool:gumb = true)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, x, y, tekst);
    PlayerTextDrawAlignment(playerid, td, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_2);
    PlayerTextDrawLetterSize(playerid, td, 0.4, 1.6);
    PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
    PlayerTextDrawSetOutline(playerid, td, 1);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 16.0, sirina);
    PlayerTextDrawSetSelectable(playerid, td, gumb);
    PlayerTextDrawShow(playerid, td);
    return td;
}

static SalonOtvori(playerid, s)
{
    gUSalonu[playerid] = s;
    gSalonIdx[playerid] = 0;
    SetPlayerInterior(playerid, SALON_INT);
    SetPlayerVirtualWorld(playerid, SALON_SVIJET + playerid);
    SetPlayerPos(playerid, gSalonKamera[0], gSalonKamera[1], gSalonAuto[2]);
    TogglePlayerControllable(playerid, false);
    SetPlayerCameraPos(playerid, gSalonKamera[0], gSalonKamera[1], gSalonKamera[2]);
    SetPlayerCameraLookAt(playerid, gSalonAuto[0], gSalonAuto[1], gSalonAuto[2]);
    gSalonTd[playerid][STD_LIJEVO] = SalonGumb(playerid, 230.0, 340.0, "<", 0x333333CC, 30.0);
    gSalonTd[playerid][STD_DESNO] = SalonGumb(playerid, 410.0, 340.0, ">", 0x333333CC, 30.0);
    gSalonTd[playerid][STD_INFO] = SalonGumb(playerid, 320.0, 330.0, "_", 0x00000088, 130.0, false);
    gSalonTd[playerid][STD_KUPI] = SalonGumb(playerid, 280.0, 412.0, T(playerid, "KUPI"), 0x2E7D32CC, 70.0);
    gSalonTd[playerid][STD_IZLAZ] = SalonGumb(playerid, 360.0, 412.0, T(playerid, "IZLAZ"), 0x8B1A1ACC, 70.0);
    SalonPrikazi(playerid);
    SelectTextDraw(playerid, 0xFFCC00FF);
}

static SalonPrikazi(playerid)
{
    new s = gUSalonu[playerid], i = gSalonIdx[playerid], model = gSalon[s][slModel][i], info[96];
    if (gSalonVozilo[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gSalonVozilo[playerid]);
    gSalonVozilo[playerid] = CreateVehicle(model, gSalonAuto[0], gSalonAuto[1], gSalonAuto[2], gSalonAuto[3], 1, 1, -1);
    if (gSalonVozilo[playerid] != INVALID_VEHICLE_ID)
    {
        LinkVehicleToInterior(gSalonVozilo[playerid], SALON_INT);
        SetVehicleVirtualWorld(gSalonVozilo[playerid], SALON_SVIJET + playerid);
    }
    format(info, sizeof info, "%s~n~~g~$%d~n~~w~%d/%d", gImenaVozila[model - 400], gSalon[s][slCijena][i], i + 1, gSalon[s][slBroj]);
    PlayerTextDrawSetString(playerid, gSalonTd[playerid][STD_INFO], info);
}

static SalonZatvori(playerid, bool:vani = true)
{
    new s = gUSalonu[playerid];
    if (s == -1) return;
    gUSalonu[playerid] = -1;
    if (gSalonVozilo[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gSalonVozilo[playerid]);
    gSalonVozilo[playerid] = INVALID_VEHICLE_ID;
    for (new i = 0; i < SALON_TD; i++) PlayerTextDrawDestroy(playerid, gSalonTd[playerid][i]);
    CancelSelectTextDraw(playerid);
    if (!vani) return;
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ]);
    SetPlayerFacingAngle(playerid, gSalon[s][slA] + 180.0);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
}

SalonDisconnect(playerid)
{
    SalonZatvori(playerid, false);
}

static SalonKupi(playerid)
{
    new s = gUSalonu[playerid];
    if (s == -1) return 1;
    new i = gSalonIdx[playerid], model = gSalon[s][slModel][i], cijena = gSalon[s][slCijena][i];
    if (BrojVlasnistava(gAccountId[playerid]) >= VLASNISTVO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete imati najvise %d vozila."), VLASNISTVO_MAX);
    if (gNovac[playerid] < cijena) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    new c1 = random(128), c2 = random(128);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO player_vehicles (owner, model, c1, c2, x, y, z, a, price) VALUES (%d, %d, %d, %d, %f, %f, %f, %f, %d)",
        gAccountId[playerid], model, c1, c2, gSalon[s][slSx], gSalon[s][slSy], gSalon[s][slSz], gSalon[s][slSa], cijena));
    new db = ZadnjiId();
    if (VlasnickoStvori(db, gAccountId[playerid], model, c1, c2, gSalon[s][slSx], gSalon[s][slSy], gSalon[s][slSz], gSalon[s][slSa], cijena) == INVALID_VEHICLE_ID)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_vehicles WHERE id = %d", db));
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo trenutno nije moguce isporuciti."));
    }
    DajNovac(playerid, -cijena);
    SalonZatvori(playerid);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %s za $%d. Vozilo vas ceka ispred salona (/parkiraj za novo mjesto)."), gImenaVozila[model - 400], cijena);
}

DLG:salon_kupi(playerid, response)
{
    if (response) SalonKupi(playerid);
    return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (OruzarnicaKlik(playerid, playertextid) || OdjecaKlik(playerid, playertextid) || MdtKlik(playerid, playertextid) || TuningKlik(playerid, playertextid)) return 1;
    new s = gUSalonu[playerid];
    if (s == -1) return 0;
    if (playertextid == gSalonTd[playerid][STD_LIJEVO] || playertextid == gSalonTd[playerid][STD_DESNO])
    {
        gSalonIdx[playerid] = (gSalonIdx[playerid] + (playertextid == gSalonTd[playerid][STD_DESNO] ? 1 : gSalon[s][slBroj] - 1)) % gSalon[s][slBroj];
        SalonPrikazi(playerid);
    }
    else if (playertextid == gSalonTd[playerid][STD_KUPI])
    {
        new i = gSalonIdx[playerid], body[128];
        format(body, sizeof body, T(playerid, "Kupiti %s za $%d?"), gImenaVozila[gSalon[s][slModel][i] - 400], gSalon[s][slCijena][i]);
        PrikaziDialog(playerid, DLG_SALON_KUPI, DIALOG_STYLE_MSGBOX, T(playerid, "Salon automobila"), body, T(playerid, "Kupi"), T(playerid, "Natrag"));
    }
    else if (playertextid == gSalonTd[playerid][STD_IZLAZ]) SalonZatvori(playerid);
    return 1;
}

public OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    if (clickedid == INVALID_TEXT_DRAW)
    {
        OruzarnicaZatvori(playerid);
        OdjecaZatvori(playerid);
        MdtZatvori(playerid);
        TuningMisIskljuci(playerid);
    }
    if (clickedid == INVALID_TEXT_DRAW && gUSalonu[playerid] != -1) SalonZatvori(playerid);
    return 0;
}

// Admin panel

ShowSaloni(playerid)
{
    new list[1024], row[96], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novi salon na mojoj poziciji"));
    for (new s = 0; s < SALON_MAX; s++)
    {
        if (!gSalon[s][slIma]) continue;
        format(row, sizeof row, T(playerid, "[%d] %s - %d vozila u ponudi"), gSalon[s][slDb], CestaZona(gSalon[s][slX], gSalon[s][slY]), gSalon[s][slBroj]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_SALON, DIALOG_STYLE_LIST, T(playerid, "Saloni automobila"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static ShowSalonMeni(playerid)
{
    return PrikaziDialog(playerid, DLG_APANEL_SALON_MENI, DIALOG_STYLE_LIST, T(playerid, "Saloni automobila"),
        T(playerid, "Postavi mjesto preuzimanja vozila ovdje\nDodaj vozilo u ponudu\nUkloni vozilo iz ponude\nObrisi salon"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static SalonNovi(playerid)
{
    new s = 0;
    while (s < SALON_MAX && gSalon[s][slIma]) s++;
    if (s == SALON_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Salon se postavlja samo izvana."));
    GetPlayerPos(playerid, gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ]);
    GetPlayerFacingAngle(playerid, gSalon[s][slA]);
    gSalon[s][slSx] = gSalon[s][slX];
    gSalon[s][slSy] = gSalon[s][slY];
    gSalon[s][slSz] = gSalon[s][slZ];
    gSalon[s][slSa] = gSalon[s][slA];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO dealerships (x, y, z, a, sx, sy, sz, sa) VALUES (%f, %f, %f, %f, %f, %f, %f, %f)",
        gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ], gSalon[s][slA], gSalon[s][slX], gSalon[s][slY], gSalon[s][slZ], gSalon[s][slA]));
    gSalon[s][slDb] = ZadnjiId();
    for (new i = 0; i < sizeof gSalonPocetna; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO dealership_cars (dealer, model, price) VALUES (%d, %d, %d)", gSalon[s][slDb], gSalonPocetna[i][0], gSalonPocetna[i][1]));
    SalonStvori(s);
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Salon dodan: %s. Postavite mjesto preuzimanja vozila."), CestaZona(gSalon[s][slX], gSalon[s][slY]));
}

static SalonObrisi(s)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUSalonu[i] == s) SalonZatvori(i);
    DestroyDynamicPickup(gSalon[s][slPickup]);
    DestroyDynamic3DTextLabel(gSalon[s][slTekst]);
    gSalon[s][slIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM dealership_cars WHERE dealer = %d", gSalon[s][slDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM dealerships WHERE id = %d", gSalon[s][slDb]));
}

static SalonPonudaPopis(playerid)
{
    new s = gApSalon[playerid], list[1024], row[64], len = 0;
    format(list, sizeof list, "%s\t%s\n", T(playerid, "Vozilo"), T(playerid, "Cijena"));
    len = strlen(list);
    for (new i = 0; i < gSalon[s][slBroj]; i++)
    {
        format(row, sizeof row, "%s\t$%d", gImenaVozila[gSalon[s][slModel][i] - 400], gSalon[s][slCijena][i]);
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_APANEL_SALON_UKLONI, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Ukloni vozilo iz ponude"), list, T(playerid, "Ukloni"), T(playerid, "Natrag"));
}

DLG:apanel_salon(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        SalonNovi(playerid);
        return ShowSaloni(playerid);
    }
    new n = listitem;
    for (new s = 0; s < SALON_MAX; s++)
    {
        if (!gSalon[s][slIma] || --n) continue;
        gApSalon[playerid] = s;
        return ShowSalonMeni(playerid);
    }
    return ShowSaloni(playerid);
}

DLG:apanel_salon_meni(playerid, response, listitem)
{
    new s = gApSalon[playerid];
    if (!response || !gSalon[s][slIma]) return ShowSaloni(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    switch (listitem)
    {
        case 0:
        {
            if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Salon se postavlja samo izvana."));
            else
            {
                PozicijaIgraca(playerid, gSalon[s][slSx], gSalon[s][slSy], gSalon[s][slSz], gSalon[s][slSa]);
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE dealerships SET sx = %f, sy = %f, sz = %f, sa = %f WHERE id = %d", gSalon[s][slSx], gSalon[s][slSy], gSalon[s][slSz], gSalon[s][slSa], gSalon[s][slDb]));
                SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto preuzimanja vozila je postavljeno."));
            }
        }
        case 1: return PrikaziDialog(playerid, DLG_APANEL_SALON_DODAJ, DIALOG_STYLE_INPUT, T(playerid, "Dodaj vozilo u ponudu"),
            T(playerid, "Unesite: [ime ili model vozila] [cijena]\nPostojece vozilo dobiva novu cijenu."), T(playerid, "Dodaj"), T(playerid, "Natrag"));
        case 2: return SalonPonudaPopis(playerid);
        case 3:
        {
            SalonObrisi(s);
            SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Salon obrisan."));
            return ShowSaloni(playerid);
        }
    }
    return ShowSalonMeni(playerid);
}

DLG:apanel_salon_dodaj(playerid, response, const inputtext[])
{
    new s = gApSalon[playerid];
    if (!response || !gSalon[s][slIma]) return ShowSalonMeni(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new unos[64], razmak = -1;
    RestParam(inputtext, 0, unos);
    for (new i = strlen(unos) - 1; i >= 0 && unos[i] == ' '; i--) unos[i] = EOS;
    for (new i = 0; unos[i]; i++) if (unos[i] == ' ') razmak = i;
    if (razmak < 1 || !ProvjeriBroj(unos[razmak + 1]) || strlen(unos[razmak + 1]) > 9)
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [ime ili model vozila] [cijena]")), ShowSalonMeni(playerid);
    new cijena = strval(unos[razmak + 1]);
    unos[razmak] = EOS;
    new model = ProvjeriBroj(unos) ? strval(unos) : ModelVozila(unos);
    if (model < 400 || model > 611) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo nije pronadeno.")), ShowSalonMeni(playerid);
    if (cijena < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Cijena mora biti veca od 0.")), ShowSalonMeni(playerid);
    new bool:ima = false;
    for (new i = 0; i < gSalon[s][slBroj]; i++) if (gSalon[s][slModel][i] == model) ima = true;
    if (!ima && gSalon[s][slBroj] >= SALON_PONUDA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ponuda je puna (najvise %d vozila)."), SALON_PONUDA), ShowSalonMeni(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO dealership_cars (dealer, model, price) VALUES (%d, %d, %d)", gSalon[s][slDb], model, cijena));
    SalonPonuda(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s je u ponudi za $%d."), gImenaVozila[model - 400], cijena);
    return ShowSalonMeni(playerid);
}

DLG:apanel_salon_ukloni(playerid, response, listitem)
{
    new s = gApSalon[playerid];
    if (!response || !gSalon[s][slIma]) return ShowSalonMeni(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem < 0 || listitem >= gSalon[s][slBroj]) return SalonPonudaPopis(playerid);
    new model = gSalon[s][slModel][listitem];
    for (new i = 0; i < MAX_PLAYERS; i++) if (gUSalonu[i] == s) SalonZatvori(i);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM dealership_cars WHERE dealer = %d AND model = %d", gSalon[s][slDb], model));
    SalonPonuda(s);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s je uklonjen iz ponude."), gImenaVozila[model - 400]);
    return SalonPonudaPopis(playerid);
}
