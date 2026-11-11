// Auto pijaca

#define PIJACA_MJESTA       30
#define PIJACA_DOMET        8.0
#define PIJACA_AUTO_DOMET   3.5
#define PIJACA_ZONA         4.0
#define PIJACA_ZAUZETO      3.0
#define PIJACA_SKIN         186
#define PIJACA_PRICA_DOMET  10.0
#define PIJACA_PRICA_MS     120000
#define PIJACA_TICK_MS      10000

enum E_PMJESTO { bool:pmIma, pmDb, Float:pmX, Float:pmY, Float:pmZ, Float:pmA, pmVozilo, STREAMER_TAG_3D_TEXT_LABEL:pmTekst }

static gPijacaMj[PIJACA_MJESTA][E_PMJESTO];
static bool:gPijacaIma, Float:gPijacaPos[3], STREAMER_TAG_PICKUP:gPijacaPickup, STREAMER_TAG_3D_TEXT_LABEL:gPijacaTekst;
static gPijacaVozilo[MAX_PLAYERS];
enum { PS_NISTA, PS_PJESKE, PS_VOZILO }
static bool:gPijacaAktorIma, gPijacaAktor, Float:gPijacaAktorPos[3], STREAMER_TAG_3D_TEXT_LABEL:gPijacaAktorTekst, gPijacaPozdrav[MAX_PLAYERS], gPijacaStanje[MAX_PLAYERS];
static const gPijacaVozilom[][] = {
    "Lijep auto! Zanima li vas koliko vrijedi?",
    "Dobar dan! Mijenjate auto? Svratite do mene.",
    "Hej, takav bi se kod mene brzo prodao!"
};
static const gPijacaPozdravi[][] = {
    "Dobar dan! Trazite dobar auto po povoljnoj cijeni?",
    "Dobrodosli na auto pijacu! Sve sto vidite je na prodaju.",
    "Hej, prijatelju! Imam auto bas za tebe."
};

InitPijaca()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS market (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS market_spots (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    SetTimer("PijacaTick", PIJACA_TICK_MS, true);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS market_seller (id INTEGER PRIMARY KEY CHECK (id = 1), x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z FROM market WHERE id = 1");
    if (DB_GetRowCount(res)) PijacaTocka(DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2));
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT x, y, z, a FROM market_seller WHERE id = 1");
    if (DB_GetRowCount(res)) PijacaAktor(DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3));
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM market_spots ORDER BY id LIMIT %d", PIJACA_MJESTA);
    for (new m = 0, n = DB_GetRowCount(res); m < n; m++)
    {
        gPijacaMj[m][pmIma] = true;
        gPijacaMj[m][pmDb] = DB_GetFieldInt(res, 0);
        gPijacaMj[m][pmX] = DB_GetFieldFloat(res, 1);
        gPijacaMj[m][pmY] = DB_GetFieldFloat(res, 2);
        gPijacaMj[m][pmZ] = DB_GetFieldFloat(res, 3);
        gPijacaMj[m][pmA] = DB_GetFieldFloat(res, 4);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT id, spot FROM player_vehicles WHERE owner = %d", VLASNIK_PIJACA);
    if (DB_GetRowCount(res)) do
    {
        new v = VlasnikPoDb(DB_GetFieldInt(res, 0)), m = PijacaMjestoPoDb(DB_GetFieldInt(res, 1));
        if (!v) continue;
        if (m == -1 || gPijacaMj[m][pmVozilo]) VlasnikOtpad(v);
        else PijacaNaMjesto(m, v);
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

static PijacaTocka(Float:x, Float:y, Float:z)
{
    if (gPijacaIma)
    {
        DestroyDynamicPickup(gPijacaPickup);
        DestroyDynamic3DTextLabel(gPijacaTekst);
    }
    gPijacaIma = true;
    gPijacaPos[0] = x;
    gPijacaPos[1] = y;
    gPijacaPos[2] = z;
    gPijacaPickup = CreateDynamicPickup(HQ_PICKUP, 1, x, y, z, 0, 0);
    gPijacaTekst = CreateDynamic3DTextLabel("Auto pijaca\nDovezite svoje vozilo i pritisnite Y za prodaju", COLOR_ORG, x, y, z + 0.5, 20.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

// Aktor

static PijacaAktorMakni()
{
    if (!gPijacaAktorIma) return 0;
    DestroyActor(gPijacaAktor);
    DestroyDynamic3DTextLabel(gPijacaAktorTekst);
    gPijacaAktorIma = false;
    return 1;
}

static PijacaAktor(Float:x, Float:y, Float:z, Float:a)
{
    PijacaAktorMakni();
    gPijacaAktorIma = true;
    gPijacaAktorPos[0] = x;
    gPijacaAktorPos[1] = y;
    gPijacaAktorPos[2] = z;
    gPijacaAktor = CreateActor(PIJACA_SKIN, x, y, z, a);
    SetActorVirtualWorld(gPijacaAktor, 0);
    gPijacaAktorTekst = CreateDynamic3DTextLabel("Prodavac", COLOR_ORG, x, y, z + 1.2, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
}

static bool:PijacaReci(playerid, const recenica[])
{
    if (!gPijacaAktorIma || !IsPlayerInRangeOfPoint(playerid, 20.0, gPijacaAktorPos[0], gPijacaAktorPos[1], gPijacaAktorPos[2])) return false;
    ApplyActorAnimation(gPijacaAktor, "PED", "IDLE_CHAT", 4.1, false, false, false, false, 2500);
    SendClientMessage(playerid, COLOR_CHAT, T(playerid, "%s kaze: %s"), T(playerid, "Prodavac"), T(playerid, recenica));
    return true;
}

static bool:PijacaSluzbeno(v)
{
    new tip = gOrgVehOrg[v] ? GetOrgTip(gOrgVehOrg[v]) : -1;
    return tip == ORG_LSPD || tip == ORG_MEDICINSKA || tip == ORG_VATROGASCI || tip == ORG_TAKSI || tip == ORG_MEHANICAR;
}

// aktor pozdravlja igrace u blizini svakih 10 sekundi
forward PijacaTick();
public PijacaTick()
{
    if (!gPijacaAktorIma) return;
    new sad = GetTickCount();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gUlogiran[i] || IsRanjen(i) || GetPlayerVirtualWorld(i) || !IsPlayerInRangeOfPoint(i, PIJACA_PRICA_DOMET, gPijacaAktorPos[0], gPijacaAktorPos[1], gPijacaAktorPos[2])) continue;
        new PLAYER_STATE:st = GetPlayerState(i), stanje = st == PLAYER_STATE_ONFOOT ? PS_PJESKE : (st == PLAYER_STATE_DRIVER || st == PLAYER_STATE_PASSENGER) ? PS_VOZILO : PS_NISTA;
        if (stanje == PS_NISTA || (stanje == PS_VOZILO && PijacaSluzbeno(GetPlayerVehicleID(i)))) continue;
        if (stanje == gPijacaStanje[i] && gPijacaPozdrav[i] && sad - gPijacaPozdrav[i] < PIJACA_PRICA_MS) continue;
        gPijacaStanje[i] = stanje;
        gPijacaPozdrav[i] = sad;
        PijacaReci(i, stanje == PS_VOZILO ? gPijacaVozilom[random(sizeof gPijacaVozilom)] : gPijacaPozdravi[random(sizeof gPijacaPozdravi)]);
        return;
    }
}

PijacaConnect(playerid)
{
    gPijacaPozdrav[playerid] = 0;
    gPijacaStanje[playerid] = PS_NISTA;
}

static PijacaMjestoPoDb(db)
{
    for (new m = 0; m < PIJACA_MJESTA; m++) if (gPijacaMj[m][pmIma] && gPijacaMj[m][pmDb] == db) return m;
    return -1;
}

static PijacaMjestoVozila(v)
{
    for (new m = 0; m < PIJACA_MJESTA; m++) if (gPijacaMj[m][pmIma] && gPijacaMj[m][pmVozilo] == v) return m;
    return -1;
}

static PijacaCijena(v)
{
    return VlasnikCijena(v) * 2 / 3;
}

static PijacaNaMjesto(m, v)
{
    new natpis[96];
    gPijacaMj[m][pmVozilo] = v;
    format(natpis, sizeof natpis, "Auto pijaca\n%s\n$%d\n[Y] kupi", gImenaVozila[GetVehicleModel(v) - 400], PijacaCijena(v));
    gPijacaMj[m][pmTekst] = CreateDynamic3DTextLabel(natpis, COLOR_ORG, 0.0, 0.0, 1.2, 15.0, INVALID_PLAYER_ID, v, 0, 0, 0);
    VoziloZakljucaj(v, true);
}

static PijacaOslobodi(m)
{
    if (!gPijacaMj[m][pmVozilo]) return;
    DestroyDynamic3DTextLabel(gPijacaMj[m][pmTekst]);
    gPijacaMj[m][pmVozilo] = 0;
}

static PijacaSlobodno()
{
    new Float:x, Float:y, Float:z;
    for (new m = 0; m < PIJACA_MJESTA; m++)
    {
        if (!gPijacaMj[m][pmIma] || gPijacaMj[m][pmVozilo]) continue;
        new bool:zauzeto = false;
        for (new i = 0; i < gVozilaCount && !zauzeto; i++)
        {
            if (GetVehicleVirtualWorld(gVozila[i])) continue;
            GetVehiclePos(gVozila[i], x, y, z);
            zauzeto = VectorSize(x - gPijacaMj[m][pmX], y - gPijacaMj[m][pmY], z - gPijacaMj[m][pmZ]) < PIJACA_ZAUZETO;
        }
        if (!zauzeto) return m;
    }
    return -1;
}

PijacaZona(Float:x, Float:y, Float:z)
{
    for (new m = 0; m < PIJACA_MJESTA; m++)
        if (gPijacaMj[m][pmIma] && VectorSize(x - gPijacaMj[m][pmX], y - gPijacaMj[m][pmY], z - gPijacaMj[m][pmZ]) < PIJACA_ZONA) return true;
    return false;
}

static PijacaAutoBlizu(playerid)
{
    new Float:x, Float:y, Float:z;
    for (new m = 0; m < PIJACA_MJESTA; m++)
    {
        if (!gPijacaMj[m][pmVozilo]) continue;
        GetVehiclePos(gPijacaMj[m][pmVozilo], x, y, z);
        if (IsPlayerInRangeOfPoint(playerid, PIJACA_AUTO_DOMET, x, y, z)) return gPijacaMj[m][pmVozilo];
    }
    return 0;
}

static bool:PijacaKodTocke(playerid)
{
    return gPijacaIma && !GetPlayerVirtualWorld(playerid) && !GetPlayerInterior(playerid) && IsPlayerInRangeOfPoint(playerid, PIJACA_DOMET, gPijacaPos[0], gPijacaPos[1], gPijacaPos[2]);
}

PijacaKey(playerid)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || ImaLisice(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    new PLAYER_STATE:st = GetPlayerState(playerid), body[192];
    if (st == PLAYER_STATE_DRIVER && PijacaKodTocke(playerid))
    {
        new v = GetPlayerVehicleID(playerid), otkup = VlasnikCijena(v) / 3;
        if (gOrgVehOrg[v])
        {
            if (!PijacaReci(playerid, "Vozila organizacija ne otkupljujem, zao mi je.")) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca ne otkupljuje vozila organizacija."));
            return 1;
        }
        if (VlasnikVozila(v) != gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo nije vase vozilo.")), 1;
        if (otkup < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca ne otkupljuje ovo vozilo.")), 1;
        gPijacaVozilo[playerid] = v;
        format(body, sizeof body, T(playerid, "Prodati %s pijaci za $%d?\nPijaca ce ga dalje prodavati za $%d."), gImenaVozila[GetVehicleModel(v) - 400], otkup, PijacaCijena(v));
        PrikaziDialog(playerid, DLG_PIJACA_PRODAJ, DIALOG_STYLE_MSGBOX, T(playerid, "Auto pijaca"), body, T(playerid, "Prodaj"), T(playerid, "Natrag"));
        return 1;
    }
    if (st != PLAYER_STATE_ONFOOT) return 0;
    new v = PijacaAutoBlizu(playerid);
    if (!v) return 0;
    gPijacaVozilo[playerid] = v;
    format(body, sizeof body, T(playerid, "Kupiti %s za $%d?"), gImenaVozila[GetVehicleModel(v) - 400], PijacaCijena(v));
    PrikaziDialog(playerid, DLG_PIJACA_KUPI, DIALOG_STYLE_MSGBOX, T(playerid, "Auto pijaca"), body, T(playerid, "Kupi"), T(playerid, "Natrag"));
    return 1;
}

DLG:pijaca_prodaj(playerid, response)
{
    new v = gPijacaVozilo[playerid];
    if (!response || GetPlayerVehicleID(playerid) != v || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || !PijacaKodTocke(playerid)) return 1;
    if (VlasnikVozila(v) != gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo nije vase vozilo."));
    new otkup = VlasnikCijena(v) / 3, model = GetVehicleModel(v);
    if (otkup < 1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca ne otkupljuje ovo vozilo."));
    if (!DajNovac(playerid, otkup)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete primiti toliko novca."));
    ClearGepek(v);
    new m = PijacaSlobodno();
    if (m == -1)
    {
        VlasnikOtpad(v);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %s za $%d. Pijaca je puna, pa je vozilo otislo na otpad."), gImenaVozila[model - 400], otkup);
    }
    VlasnikPrenesi(v, VLASNIK_PIJACA);
    VlasnikParkiraj(v, gPijacaMj[m][pmX], gPijacaMj[m][pmY], gPijacaMj[m][pmZ], gPijacaMj[m][pmA]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET spot = %d WHERE id = %d", gPijacaMj[m][pmDb], VlasnikDb(v)));
    SetVehicleToRespawn(v); // takes the seller out and puts the car on its spot (VlasnickoSpawn), repaired
    PijacaNaMjesto(m, v);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %s pijaci za $%d."), gImenaVozila[model - 400], otkup);
    PijacaReci(playerid, "Hvala, ovaj ce se brzo prodati.");
    return 1;
}

DLG:pijaca_kupi(playerid, response)
{
    new v = gPijacaVozilo[playerid], m = PijacaMjestoVozila(v);
    if (!response || m == -1 || VlasnikVozila(v) != VLASNIK_PIJACA || PijacaAutoBlizu(playerid) != v) return 1;
    new cijena = PijacaCijena(v);
    if (BrojVlasnistava(gAccountId[playerid]) >= VLASNISTVO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete imati najvise %d vozila."), VLASNISTVO_MAX);
    if (!DajNovac(playerid, -cijena)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    VlasnikPrenesi(v, gAccountId[playerid]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE player_vehicles SET spot = 0 WHERE id = %d", VlasnikDb(v)));
    PijacaOslobodi(m);
    VoziloZakljucaj(v, false);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %s za $%d. Odvezite ga s pijace i parkirajte s /parkiraj."), gImenaVozila[GetVehicleModel(v) - 400], cijena);
    PijacaReci(playerid, "Odlican izbor! Sretno na cesti.");
    return 1;
}

// Admin panel

ShowPijacaAdmin(playerid)
{
    new n = 0, title[64];
    for (new m = 0; m < PIJACA_MJESTA; m++) if (gPijacaMj[m][pmIma]) n++;
    format(title, sizeof title, T(playerid, "Auto pijaca (%d/%d mjesta)"), n, PIJACA_MJESTA);
    return PrikaziDialog(playerid, DLG_APANEL_PIJACA, DIALOG_STYLE_LIST, title,
        T(playerid, "Postavi mjesto otkupa ovdje\nDodaj parking mjesto ovdje (vozilo u kojem sjedite ili vasa pozicija)\nUkloni najblize parking mjesto\nPostavi prodavaca ovdje\nUkloni prodavaca"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PijacaDodajMjesto(playerid)
{
    new m = 0;
    while (m < PIJACA_MJESTA && gPijacaMj[m][pmIma]) m++;
    if (m == PIJACA_MJESTA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca vec ima najvise mjesta."));
    PozicijaIgraca(playerid, gPijacaMj[m][pmX], gPijacaMj[m][pmY], gPijacaMj[m][pmZ], gPijacaMj[m][pmA]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO market_spots (x, y, z, a) VALUES (%f, %f, %f, %f)", gPijacaMj[m][pmX], gPijacaMj[m][pmY], gPijacaMj[m][pmZ], gPijacaMj[m][pmA]));
    gPijacaMj[m][pmDb] = ZadnjiId();
    gPijacaMj[m][pmIma] = true;
    gPijacaMj[m][pmVozilo] = 0;
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Parking mjesto je dodano."));
}

static PijacaUkloniMjesto(playerid)
{
    new najblize = -1, Float:x, Float:y, Float:z, Float:d, Float:bd = 10.0;
    GetPlayerPos(playerid, x, y, z);
    for (new m = 0; m < PIJACA_MJESTA; m++)
    {
        if (!gPijacaMj[m][pmIma]) continue;
        d = VectorSize(x - gPijacaMj[m][pmX], y - gPijacaMj[m][pmY], z - gPijacaMj[m][pmZ]);
        if (d < bd) bd = d, najblize = m;
    }
    if (najblize == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema parking mjesta u blizini (10 m)."));
    new v = gPijacaMj[najblize][pmVozilo];
    PijacaOslobodi(najblize);
    if (v) VlasnikOtpad(v);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM market_spots WHERE id = %d", gPijacaMj[najblize][pmDb]));
    gPijacaMj[najblize][pmIma] = false;
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Parking mjesto je uklonjeno."));
}

DLG:apanel_pijaca(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    switch (listitem)
    {
        case 0:
        {
            if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca se postavlja samo izvana."));
            else
            {
                new Float:x, Float:y, Float:z;
                GetPlayerPos(playerid, x, y, z);
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO market (id, x, y, z) VALUES (1, %f, %f, %f)", x, y, z));
                PijacaTocka(x, y, z);
                SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Mjesto otkupa je postavljeno."));
            }
        }
        case 1:
        {
            if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca se postavlja samo izvana."));
            else PijacaDodajMjesto(playerid);
        }
        case 2: PijacaUkloniMjesto(playerid);
        case 3:
        {
            if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pijaca se postavlja samo izvana."));
            else
            {
                new Float:x, Float:y, Float:z, Float:a;
                GetPlayerPos(playerid, x, y, z);
                GetPlayerFacingAngle(playerid, a);
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO market_seller (id, x, y, z, a) VALUES (1, %f, %f, %f, %f)", x, y, z, a));
                PijacaAktor(x, y, z, a);
                SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prodavac stoji na vasem mjestu."));
            }
        }
        case 4:
        {
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM market_seller WHERE id = 1"));
            if (PijacaAktorMakni()) SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prodavac je uklonjen."));
            else SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prodavac nije postavljen."));
        }
    }
    return ShowPijacaAdmin(playerid);
}
