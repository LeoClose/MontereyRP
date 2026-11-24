#define KAMP_MAX            8
#define KAMP_DOMET          2.0
#define KAMP_ISPRED         6.0
#define KAMP_OTKUP_DIO      3
#define KAMP_SKIN           161
#define KAMP_PRODAJA_DOMET  30.0

static const gKampModel[] = { PRIKOLICA_VELIKA };
static const gKampIme[][] = { "Kamp prikolica" };
static const gKampCijena[] = { 35000 };

enum E_KAMP { bool:kpIma, kpDb, Float:kpX, Float:kpY, Float:kpZ, Float:kpA, STREAMER_TAG_3D_TEXT_LABEL:kpTekst, kpIkona, kpAktor }
static gKamp[KAMP_MAX][E_KAMP], gKampU[MAX_PLAYERS] = { -1, ... }, gKampV[MAX_PLAYERS], gApKamp[MAX_PLAYERS][KAMP_MAX];

InitKamp()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS trailer_shops (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM trailer_shops LIMIT %d", KAMP_MAX);
    for (new s = 0, n = DB_GetRowCount(res); s < n; s++, DB_SelectNextRow(res))
    {
        gKamp[s][kpDb] = DB_GetFieldInt(res, 0);
        gKamp[s][kpX] = DB_GetFieldFloat(res, 1);
        gKamp[s][kpY] = DB_GetFieldFloat(res, 2);
        gKamp[s][kpZ] = DB_GetFieldFloat(res, 3);
        gKamp[s][kpA] = DB_GetFieldFloat(res, 4);
        KampStvori(s);
    }
    DB_FreeResultSet(res);
}

static KampStvori(s)
{
    gKamp[s][kpIma] = true;
    gKamp[s][kpAktor] = CreateActor(KAMP_SKIN, gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ], gKamp[s][kpA]);
    SetActorInvulnerable(gKamp[s][kpAktor], true);
    gKamp[s][kpTekst] = CreateDynamic3DTextLabel("Kamp prikolice", COLOR_ORG, gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ] + 1.2, 15.0);
    InterakcijaOznaka(gKamp[s][kpTekst]);
    gKamp[s][kpIkona] = MapaIkona(gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ], 51);
}

KampKey(playerid)
{
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPlayerVirtualWorld(playerid) || GetPlayerInterior(playerid)) return 0;
    for (new s = 0; s < KAMP_MAX; s++)
    {
        if (!gKamp[s][kpIma] || !IsPlayerInRangeOfPoint(playerid, KAMP_DOMET, gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ])) continue;
        new list[256], red[96], len = 0, v = KampZaProdaju(playerid, s);
        gKampU[playerid] = s;
        gKampV[playerid] = v;
        for (new i = 0; i < sizeof gKampModel; i++)
        {
            format(red, sizeof red, T(playerid, "Kupi: %s - $%d"), T(playerid, gKampIme[i]), gKampCijena[i]);
            DodajStupac(list, len, red);
        }
        if (v)
        {
            format(red, sizeof red, T(playerid, "Prodaj svoju kamp prikolicu - $%d"), VlasnikCijena(v) / KAMP_OTKUP_DIO);
            DodajStupac(list, len, red);
        }
        AktorKaze(playerid, gKamp[s][kpAktor], "Prodavac prikolica", v ? ("Lijepa prikolica! Zelite kupiti novu ili prodati ovu?") : ("Dobar dan! Treba vam dom na kotacima?"));
        PrikaziDialog(playerid, DLG_KAMP_KUPI, DIALOG_STYLE_LIST, T(playerid, "Kamp prikolice"), list, T(playerid, "Kupi"), T(playerid, "Zatvori"));
        return 1;
    }
    return 0;
}

DLG:kamp_kupi(playerid, response, listitem)
{
    new s = gKampU[playerid];
    if (!response || s == -1 || listitem < 0 || !gKamp[s][kpIma] || !IsPlayerInRangeOfPoint(playerid, KAMP_DOMET, gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ])) return 1;
    if (listitem >= sizeof gKampModel)
    {
        new v = KampZaProdaju(playerid, s), body[160];
        if (!v || v != gKampV[playerid]) return 1;
        format(body, sizeof body, T(playerid, "Prodati kamp prikolicu za $%d? Namjestaj se vraca u skladiste."), VlasnikCijena(v) / KAMP_OTKUP_DIO);
        return PrikaziDialog(playerid, DLG_KAMP_PRODAJ, DIALOG_STYLE_MSGBOX, T(playerid, "Kamp prikolica"), body, T(playerid, "Prodaj"), T(playerid, "Natrag"));
    }
    new model = gKampModel[listitem], cijena = gKampCijena[listitem], c = random(128);
    if (KampImaPrikolicu(playerid))
    {
        AktorKaze(playerid, gKamp[s][kpAktor], "Prodavac prikolica", "Vec imate kamp prikolicu, jedna je dovoljna.");
        return 1;
    }
    if (BrojVlasnistava(gAccountId[playerid]) >= VLASNISTVO_MAX) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete imati najvise %d vozila."), VLASNISTVO_MAX);
    if (gNovac[playerid] < cijena)
    {
        AktorKaze(playerid, gKamp[s][kpAktor], "Prodavac prikolica", "Nemate dovoljno novca? Navratite kad skupite.");
        return 1;
    }
    new Float:a = gKamp[s][kpA], Float:x = gKamp[s][kpX] - KAMP_ISPRED * floatsin(a, degrees), Float:y = gKamp[s][kpY] + KAMP_ISPRED * floatcos(a, degrees), Float:z = gKamp[s][kpZ] + 0.5;
    if (!SlobodnoParkiranje(x, y, z, a, 4.5, 10.0)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo trenutno nije moguce isporuciti."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO player_vehicles (owner, model, c1, c2, x, y, z, a, price) VALUES (%d, %d, %d, %d, %f, %f, %f, %f, %d)", gAccountId[playerid], model, c, c, x, y, z, a, cijena));
    new db = ZadnjiId();
    if (VlasnickoStvori(db, gAccountId[playerid], model, c, c, x, y, z, a, cijena) == INVALID_VEHICLE_ID)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_vehicles WHERE id = %d", db));
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo trenutno nije moguce isporuciti."));
    }
    DajNovac(playerid, -cijena);
    AktorKaze(playerid, gKamp[s][kpAktor], "Prodavac prikolica", "Odlican izbor! Prikolica vas ceka ispred, sretan put.");
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste kamp prikolicu za $%d. Zakacite je vozilom (/zakaci), a /kamp otvara izbornik."), cijena);
}

static KampImaPrikolicu(playerid)
{
    new vozila[VLASNISTVO_MAX];
    for (new i = 0, n = VlasnistvaIgraca(playerid, vozila); i < n; i++) if (JePrikolica(GetVehicleModel(vozila[i]))) return 1;
    return 0;
}

static KampZaProdaju(playerid, s)
{
    new vozila[VLASNISTVO_MAX], Float:x, Float:y, Float:z;
    for (new i = 0, n = VlasnistvaIgraca(playerid, vozila); i < n; i++)
    {
        new v = vozila[i];
        if (!JePrikolica(GetVehicleModel(v)) || GetVehicleVirtualWorld(v)) continue;
        GetVehiclePos(v, x, y, z);
        if (VectorSize(x - gKamp[s][kpX], y - gKamp[s][kpY], z - gKamp[s][kpZ]) <= KAMP_PRODAJA_DOMET) return v;
    }
    return 0;
}

static gKampOpcija[MAX_PLAYERS][4], gKampOpcija_n[MAX_PLAYERS];
enum { KO_UDJI, KO_IZADJI, KO_BRAVA, KO_NAMJESTAJ, KO_PRODAJ }

static KampProdavac(playerid, v)
{
    for (new s = 0; s < KAMP_MAX; s++) if (gKamp[s][kpIma] && KampZaProdaju(playerid, s) == v) return s;
    return -1;
}

KampVrata(playerid, v, bool:unutra)
{
    new list[256], red[96], len = 0, n = 0, bool:vlasnik = VlasnikVozila(v) == gAccountId[playerid];
    gKampV[playerid] = v;
    gKampU[playerid] = vlasnik && !unutra ? KampProdavac(playerid, v) : -1;
    gKampOpcija[playerid][n++] = unutra ? KO_IZADJI : KO_UDJI;
    DodajStupac(list, len, unutra ? T(playerid, "Izadji") : T(playerid, "Udji"));
    if (ImaKljuc(playerid, v))
    {
        gKampOpcija[playerid][n++] = KO_BRAVA;
        DodajStupac(list, len, PrikolicaZakljucana(v) ? T(playerid, "Otkljucaj") : T(playerid, "Zakljucaj"));
    }
    if (vlasnik && unutra)
    {
        gKampOpcija[playerid][n++] = KO_NAMJESTAJ;
        DodajStupac(list, len, T(playerid, "Namjestaj"));
    }
    if (gKampU[playerid] != -1)
    {
        gKampOpcija[playerid][n++] = KO_PRODAJ;
        format(red, sizeof red, T(playerid, "Prodaj ($%d)"), VlasnikCijena(v) / KAMP_OTKUP_DIO);
        DodajStupac(list, len, red);
    }
    gKampOpcija_n[playerid] = n;
    return PrikaziDialog(playerid, DLG_KAMP, DIALOG_STYLE_LIST, T(playerid, "Kamp prikolica"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:kamp(playerid, response, listitem)
{
    new v = gKampV[playerid];
    if (!response || !v || listitem < 0 || listitem >= gKampOpcija_n[playerid] || !JePrikolica(GetVehicleModel(v))) return 1;
    new bool:unutra = PrikolicaUnutra(playerid) == v;
    if (!unutra && PrikolicaKodVrata(playerid) != v) return 1;
    switch (gKampOpcija[playerid][listitem])
    {
        case KO_UDJI:
        {
            if (PrikolicaZakljucana(v) && !ImaKljuc(playerid, v)) return ObavijestGreska(playerid, T(playerid, "Prikolica je zakljucana."));
            PrikolicaUdji(playerid, v);
        }
        case KO_IZADJI: if (unutra) PrikolicaIzadji(playerid);
        case KO_BRAVA:
        {
            if (!ImaKljuc(playerid, v)) return 1;
            new bool:novo = !PrikolicaZakljucana(v);
            PrikolicaZakljucaj(v, novo);
            PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, novo ? ("* %s zakljucava kamp prikolicu.") : ("* %s otkljucava kamp prikolicu."), GetRPIme(playerid), "", 1);
        }
        case KO_NAMJESTAJ: if (unutra && VlasnikVozila(v) == gAccountId[playerid]) return NamjestajMeni(playerid, -VlasnikDb(v));
        case KO_PRODAJ:
        {
            new s = gKampU[playerid], body[160];
            if (s == -1 || KampZaProdaju(playerid, s) != v) return 1;
            format(body, sizeof body, T(playerid, "Prodati kamp prikolicu za $%d? Namjestaj se vraca u skladiste."), VlasnikCijena(v) / KAMP_OTKUP_DIO);
            return PrikaziDialog(playerid, DLG_KAMP_PRODAJ, DIALOG_STYLE_MSGBOX, T(playerid, "Kamp prikolica"), body, T(playerid, "Prodaj"), T(playerid, "Natrag"));
        }
    }
    return 1;
}

DLG:kamp_prodaj(playerid, response)
{
    new v = gKampV[playerid], s = gKampU[playerid];
    if (!response || !v || s == -1 || !gKamp[s][kpIma] || VlasnikVozila(v) != gAccountId[playerid] || !JePrikolica(GetVehicleModel(v)) || KampZaProdaju(playerid, s) != v) return 1;
    if (!PrikolicaPrazna(v)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U prikolici je netko."));
    new otkup = VlasnikCijena(v) / KAMP_OTKUP_DIO, db = VlasnikDb(v);
    AktorKaze(playerid, gKamp[s][kpAktor], "Prodavac prikolica", "Dogovoreno, evo vasih novaca. Bilo je zadovoljstvo.");
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE furniture SET house = 0 WHERE house = %d", -db));
    VlasnikOtpad(v);
    DajNovac(playerid, otkup);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste kamp prikolicu za $%d."), otkup);
}

ShowKampove(playerid)
{
    new list[1024], red[96], len = 0;
    DodajStupac(list, len, T(playerid, "+ Novo prodajno mjesto (prikolice se pojavljuju ispred vas)"));
    for (new s = 0, n = 0; s < KAMP_MAX; s++)
    {
        if (!gKamp[s][kpIma]) continue;
        format(red, sizeof red, T(playerid, "[%d] Kamp prikolice (%s) - obrisi"), gKamp[s][kpDb], CestaZona(gKamp[s][kpX], gKamp[s][kpY]));
        DodajStupac(list, len, red);
        gApKamp[playerid][n++] = s;
    }
    return PrikaziDialog(playerid, DLG_APANEL_KAMP, DIALOG_STYLE_LIST, T(playerid, "Kamp prikolice"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_kamp(playerid, response, listitem)
{
    if (!response) return ShowAdminPanel(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        new s = 0;
        while (s < KAMP_MAX && gKamp[s][kpIma]) s++;
        if (s == KAMP_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Prodajno mjesto se postavlja samo izvana.")), ShowKampove(playerid);
        GetPlayerPos(playerid, gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ]);
        GetPlayerFacingAngle(playerid, gKamp[s][kpA]);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO trailer_shops (x, y, z, a) VALUES (%f, %f, %f, %f)", gKamp[s][kpX], gKamp[s][kpY], gKamp[s][kpZ], gKamp[s][kpA]));
        gKamp[s][kpDb] = ZadnjiId();
        KampStvori(s);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prodajno mjesto kamp prikolica je postavljeno."));
        return ShowKampove(playerid);
    }
    new n = 0;
    for (new s = 0; s < KAMP_MAX; s++) if (gKamp[s][kpIma]) n++;
    if (listitem - 1 >= n) return ShowKampove(playerid);
    new s = gApKamp[playerid][listitem - 1];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM trailer_shops WHERE id = %d", gKamp[s][kpDb]));
    DestroyDynamic3DTextLabel(gKamp[s][kpTekst]);
    DestroyActor(gKamp[s][kpAktor]);
    MapaIkonaMakni(gKamp[s][kpIkona]);
    gKamp[s][kpIma] = false;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Prodajno mjesto kamp prikolica je obrisano."));
    return ShowKampove(playerid);
}
