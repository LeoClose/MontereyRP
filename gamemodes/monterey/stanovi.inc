#define ZGRADA_MAX          64
#define JEDINICA_MAX        800
#define STAN_DOMET          1.5
#define HODNIK_INT          15
#define HODNIK_SVIJET       60000
#define STAN_SVIJET         70000
#define MOTEL_DAN           86400
#define MOTEL_PROVJERA_MS   60000

enum { ZGV_STAN = 1, ZGV_MOTEL = 2 }

static const Float:gHodnik[4] = { 2216.34, -1150.51, 1025.80, 270.0 };

enum E_ZGRADA { bool:zgIma, zgDb, zgVrsta, Float:zgX, Float:zgY, Float:zgZ, Float:zgA }
enum E_JED { bool:jeIma, jeDb, jeZgrada, jeBroj, Float:jeX, Float:jeY, Float:jeZ, Float:jeA, bool:jeUnutra, jeInt, jeCijena, jeVlasnik, jeVlasnikIme[MAX_PLAYER_NAME + 1], jeDo, bool:jeZakljucano, STREAMER_TAG_3D_TEXT_LABEL:jeTekst }

static gZgrada[ZGRADA_MAX][E_ZGRADA], gJed[JEDINICA_MAX][E_JED];

StanoviTablice()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS apartment_buildings (id INTEGER PRIMARY KEY, kind INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS apartment_units (id INTEGER PRIMARY KEY AUTOINCREMENT, building INTEGER NOT NULL, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, inside INTEGER NOT NULL, interior INTEGER NOT NULL, price INTEGER NOT NULL, owner INTEGER NOT NULL DEFAULT 0, paid INTEGER NOT NULL DEFAULT 0, locked INTEGER NOT NULL DEFAULT 1)"));
}

StanoviUvezi(DB:izvor)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM apartment_buildings");
    new n = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    if (n) return;
    res = DB_ExecuteQuery(izvor, "SELECT id, kind, x, y, z, a FROM buildings ORDER BY id");
    for (new i = 0, m = res ? DB_GetRowCount(res) : 0; i < m; i++, DB_SelectNextRow(res))
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO apartment_buildings (id, kind, x, y, z, a) VALUES (%d, %d, %f, %f, %f, %f)", DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldFloat(res, 5)));
    if (res) DB_FreeResultSet(res);
    res = DB_ExecuteQuery(izvor, "SELECT building, x, y, z, a, inside, interior, price FROM units ORDER BY id");
    for (new i = 0, m = res ? DB_GetRowCount(res) : 0; i < m; i++, DB_SelectNextRow(res))
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO apartment_units (building, x, y, z, a, inside, interior, price) VALUES (%d, %f, %f, %f, %f, %d, %d, %d)", DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldInt(res, 5), DB_GetFieldInt(res, 6), DB_GetFieldInt(res, 7)));
    if (res) DB_FreeResultSet(res);
}

InitStanovi()
{
    StanoviTablice();
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, kind, x, y, z, a FROM apartment_buildings ORDER BY id LIMIT %d", ZGRADA_MAX);
    for (new z = 0, n = DB_GetRowCount(res); z < n; z++, DB_SelectNextRow(res))
    {
        gZgrada[z][zgIma] = true;
        gZgrada[z][zgDb] = DB_GetFieldInt(res, 0);
        gZgrada[z][zgVrsta] = DB_GetFieldInt(res, 1);
        gZgrada[z][zgX] = DB_GetFieldFloat(res, 2);
        gZgrada[z][zgY] = DB_GetFieldFloat(res, 3);
        gZgrada[z][zgZ] = DB_GetFieldFloat(res, 4);
        gZgrada[z][zgA] = DB_GetFieldFloat(res, 5);
        new svijet = HODNIK_SVIJET + gZgrada[z][zgDb];
        CreateDynamicPickup(HQ_PICKUP, 1, gZgrada[z][zgX], gZgrada[z][zgY], gZgrada[z][zgZ], 0, 0);
        CreateDynamic3DTextLabel(gZgrada[z][zgVrsta] == ZGV_MOTEL ? ("Motel") : ("Stambena zgrada"), COLOR_ORG, gZgrada[z][zgX], gZgrada[z][zgY], gZgrada[z][zgZ] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
        InterakcijaZona(gZgrada[z][zgX], gZgrada[z][zgY], gZgrada[z][zgZ], STAN_DOMET, 0, 0, SAVJET_ULAZ);
        CreateDynamicPickup(HQ_PICKUP, 1, gHodnik[0], gHodnik[1], gHodnik[2], svijet, HODNIK_INT);
        CreateDynamic3DTextLabel("Izlaz", COLOR_ORG, gHodnik[0], gHodnik[1], gHodnik[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, svijet, HODNIK_INT);
        InterakcijaZona(gHodnik[0], gHodnik[1], gHodnik[2], STAN_DOMET, svijet, HODNIK_INT, SAVJET_IZLAZ);
    }
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT u.id, u.building, u.x, u.y, u.z, u.a, u.inside, u.interior, u.price, u.owner, IFNULL(a.name, ''), u.paid, u.locked FROM apartment_units u LEFT JOIN accounts a ON a.id = u.owner ORDER BY u.id LIMIT %d", JEDINICA_MAX);
    for (new j = 0, n = DB_GetRowCount(res); j < n; j++, DB_SelectNextRow(res))
    {
        new z = ZgradaPoDb(DB_GetFieldInt(res, 1));
        if (z == -1) continue;
        gJed[j][jeIma] = true;
        gJed[j][jeDb] = DB_GetFieldInt(res, 0);
        gJed[j][jeZgrada] = z;
        gJed[j][jeX] = DB_GetFieldFloat(res, 2);
        gJed[j][jeY] = DB_GetFieldFloat(res, 3);
        gJed[j][jeZ] = DB_GetFieldFloat(res, 4);
        gJed[j][jeA] = DB_GetFieldFloat(res, 5);
        gJed[j][jeUnutra] = DB_GetFieldInt(res, 6) != 0;
        gJed[j][jeInt] = DB_GetFieldInt(res, 7);
        gJed[j][jeCijena] = DB_GetFieldInt(res, 8);
        gJed[j][jeVlasnik] = DB_GetFieldInt(res, 9);
        DB_GetFieldString(res, 10, gJed[j][jeVlasnikIme], MAX_PLAYER_NAME + 1);
        gJed[j][jeDo] = DB_GetFieldInt(res, 11);
        gJed[j][jeZakljucano] = DB_GetFieldInt(res, 12) != 0;
        new broj = 1;
        for (new i = 0; i < j; i++) if (gJed[i][jeIma] && gJed[i][jeZgrada] == z) broj++;
        gJed[j][jeBroj] = broj;
        new svijet = gJed[j][jeUnutra] ? HODNIK_SVIJET + gZgrada[z][zgDb] : 0, interijer = gJed[j][jeUnutra] ? HODNIK_INT : 0;
        gJed[j][jeTekst] = CreateDynamic3DTextLabel("", COLOR_ORG, gJed[j][jeX], gJed[j][jeY], gJed[j][jeZ] + 0.5, 8.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, svijet, interijer);
        InterakcijaZona(gJed[j][jeX], gJed[j][jeY], gJed[j][jeZ], STAN_DOMET, svijet, interijer, SAVJET_ULAZ);
        JedNatpis(j);
    }
    DB_FreeResultSet(res);
    SetTimer("MoteliNaplata", MOTEL_PROVJERA_MS, true);
}

static ZgradaPoDb(db)
{
    for (new z = 0; z < ZGRADA_MAX; z++) if (gZgrada[z][zgIma] && gZgrada[z][zgDb] == db) return z;
    return -1;
}

static JedPoDb(db)
{
    for (new j = 0; j < JEDINICA_MAX; j++) if (gJed[j][jeIma] && gJed[j][jeDb] == db) return j;
    return -1;
}

static bool:JedMotel(j)
{
    return gZgrada[gJed[j][jeZgrada]][zgVrsta] == ZGV_MOTEL;
}

static JedNatpis(j)
{
    new natpis[96], ime[MAX_PLAYER_NAME + 1];
    strcopy(ime, gJed[j][jeVlasnikIme]);
    for (new i = 0; ime[i]; i++) if (ime[i] == '_') ime[i] = ' ';
    if (JedMotel(j))
    {
        if (gJed[j][jeVlasnik]) format(natpis, sizeof natpis, "Soba %d\nZauzeta", gJed[j][jeBroj]);
        else format(natpis, sizeof natpis, "Soba %d\nSlobodna: $%d/dan", gJed[j][jeBroj], gJed[j][jeCijena]);
    }
    else if (gJed[j][jeVlasnik]) format(natpis, sizeof natpis, "Stan %d\nVlasnik: %s", gJed[j][jeBroj], ime);
    else format(natpis, sizeof natpis, "Stan %d\nNa prodaju: $%d", gJed[j][jeBroj], gJed[j][jeCijena]);
    UpdateDynamic3DTextLabelText(gJed[j][jeTekst], COLOR_ORG, natpis);
}

static JedPostaviVlasnika(j, racun, const ime[], placeno)
{
    gJed[j][jeVlasnik] = racun;
    gJed[j][jeVlasnikIme][0] = EOS;
    if (racun) strcopy(gJed[j][jeVlasnikIme], ime, MAX_PLAYER_NAME + 1);
    gJed[j][jeDo] = placeno;
    gJed[j][jeZakljucano] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE apartment_units SET owner = %d, paid = %d, locked = 1 WHERE id = %d", racun, placeno, gJed[j][jeDb]));
    JedNatpis(j);
}

static JedIgraca(playerid)
{
    new svijet = GetPlayerVirtualWorld(playerid);
    if (svijet <= STAN_SVIJET) return -1;
    new j = JedPoDb(svijet - STAN_SVIJET), interijer, Float:x, Float:y, Float:z;
    if (j == -1) return -1;
    KucaIntPoz(gJed[j][jeInt], interijer, x, y, z);
    return GetPlayerInterior(playerid) == interijer ? j : -1;
}

static ZgradaIgraca(playerid)
{
    new svijet = GetPlayerVirtualWorld(playerid);
    if (GetPlayerInterior(playerid) != HODNIK_INT || svijet <= HODNIK_SVIJET || svijet >= STAN_SVIJET) return -1;
    return ZgradaPoDb(svijet - HODNIK_SVIJET);
}

static JedUdi(playerid, j)
{
    new interijer, Float:x, Float:y, Float:z;
    KucaIntPoz(gJed[j][jeInt], interijer, x, y, z);
    PrebaciIgraca(playerid, interijer, STAN_SVIJET + gJed[j][jeDb], x, y, z);
}

static JedVani(playerid, j)
{
    new z = gJed[j][jeZgrada];
    if (gJed[j][jeUnutra]) PrebaciIgraca(playerid, HODNIK_INT, HODNIK_SVIJET + gZgrada[z][zgDb], gJed[j][jeX], gJed[j][jeY], gJed[j][jeZ], gJed[j][jeA]);
    else PrebaciIgraca(playerid, 0, 0, gJed[j][jeX], gJed[j][jeY], gJed[j][jeZ], gJed[j][jeA]);
}

static JedIsprazni(j)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && JedIgraca(i) == j) JedVani(i, j);
}

static JedBlizu(playerid, svijet, interijer)
{
    for (new j = 0; j < JEDINICA_MAX; j++)
    {
        if (!gJed[j][jeIma]) continue;
        new z = gJed[j][jeZgrada], js = gJed[j][jeUnutra] ? HODNIK_SVIJET + gZgrada[z][zgDb] : 0;
        if (js != svijet || (gJed[j][jeUnutra] ? HODNIK_INT : 0) != interijer) continue;
        if (IsPlayerInRangeOfPoint(playerid, STAN_DOMET, gJed[j][jeX], gJed[j][jeY], gJed[j][jeZ])) return j;
    }
    return -1;
}

static JedImaIgrac(playerid, bool:motel)
{
    for (new j = 0; j < JEDINICA_MAX; j++) if (gJed[j][jeIma] && gJed[j][jeVlasnik] == gAccountId[playerid] && JedMotel(j) == motel) return j;
    return -1;
}

static JedVrata(playerid, j)
{
    if (gJed[j][jeVlasnik] == gAccountId[playerid] || (gJed[j][jeVlasnik] && !gJed[j][jeZakljucano]))
    {
        JedUdi(playerid, j);
        return 1;
    }
    if (gJed[j][jeVlasnik]) return ObavijestGreska(playerid, T(playerid, "Vrata su zakljucana.")), 1;
    new naslov[32], tekst[96];
    if (JedMotel(j))
    {
        format(naslov, sizeof naslov, T(playerid, "Soba %d"), gJed[j][jeBroj]);
        format(tekst, sizeof tekst, T(playerid, "Iznajmiti sobu za $%d na dan?"), gJed[j][jeCijena]);
    }
    else
    {
        format(naslov, sizeof naslov, T(playerid, "Stan %d"), gJed[j][jeBroj]);
        format(tekst, sizeof tekst, T(playerid, "Kupiti stan za $%d?"), gJed[j][jeCijena]);
    }
    return UpitDaNe(playerid, UPIT_STAN, j, 0, naslov, tekst), 1;
}

StanUpit(playerid, j, bool:da)
{
    if (!da || j < 0 || j >= JEDINICA_MAX || !gJed[j][jeIma] || gJed[j][jeVlasnik]) return 1;
    if (JedBlizu(playerid, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid)) != j) return 1;
    new bool:motel = JedMotel(j);
    if (JedImaIgrac(playerid, motel) != -1) return ObavijestGreska(playerid, motel ? T(playerid, "Vec iznajmljujete motelsku sobu.") : T(playerid, "Vec imate stan.")), 1;
    if (!DajNovac(playerid, -gJed[j][jeCijena])) return ObavijestGreska(playerid, T(playerid, "Nemate dovoljno novca.")), 1;
    new ime[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, ime, sizeof ime);
    JedPostaviVlasnika(j, gAccountId[playerid], ime, motel ? gettime() + MOTEL_DAN : 0);
    if (motel) ObavijestInfo(playerid, T(playerid, "Motel"), T(playerid, "Iznajmili ste sobu. Najam se automatski placa svaki dan."));
    else ObavijestInfo(playerid, T(playerid, "Stan"), T(playerid, "Kupili ste stan."));
    JedUdi(playerid, j);
    return 1;
}

StanF(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new j = JedIgraca(playerid);
    if (j != -1)
    {
        new interijer, Float:x, Float:y, Float:z;
        KucaIntPoz(gJed[j][jeInt], interijer, x, y, z);
        if (!IsPlayerInRangeOfPoint(playerid, STAN_DOMET + 0.5, x, y, z)) return 0;
        JedVani(playerid, j);
        return 1;
    }
    new svijet = GetPlayerVirtualWorld(playerid), interijer = GetPlayerInterior(playerid), zg = ZgradaIgraca(playerid);
    if (zg != -1)
    {
        if (IsPlayerInRangeOfPoint(playerid, STAN_DOMET, gHodnik[0], gHodnik[1], gHodnik[2]))
        {
            PrebaciIgraca(playerid, 0, 0, gZgrada[zg][zgX], gZgrada[zg][zgY], gZgrada[zg][zgZ], gZgrada[zg][zgA]);
            return 1;
        }
    }
    else if (svijet || interijer) return 0;
    else
    {
        for (new z = 0; z < ZGRADA_MAX; z++)
        {
            if (!gZgrada[z][zgIma] || !IsPlayerInRangeOfPoint(playerid, STAN_DOMET, gZgrada[z][zgX], gZgrada[z][zgY], gZgrada[z][zgZ])) continue;
            PrebaciIgraca(playerid, HODNIK_INT, HODNIK_SVIJET + gZgrada[z][zgDb], gHodnik[0], gHodnik[1], gHodnik[2], gHodnik[3]);
            return 1;
        }
    }
    j = JedBlizu(playerid, svijet, interijer);
    if (j == -1) return 0;
    return JedVrata(playerid, j);
}

StanZakljucaj(playerid)
{
    new j = JedIgraca(playerid);
    if (j == -1) j = JedBlizu(playerid, GetPlayerVirtualWorld(playerid), GetPlayerInterior(playerid));
    if (j == -1 || !gJed[j][jeVlasnik] || gJed[j][jeVlasnik] != gAccountId[playerid]) return 0;
    gJed[j][jeZakljucano] = !gJed[j][jeZakljucano];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE apartment_units SET locked = %d WHERE id = %d", gJed[j][jeZakljucano] ? 1 : 0, gJed[j][jeDb]));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gJed[j][jeZakljucano] ? ("* %s zakljucava vrata.") : ("* %s otkljucava vrata."), GetRPIme(playerid), "", 1);
    return 1;
}

forward MoteliNaplata();
public MoteliNaplata()
{
    new sad = gettime();
    for (new j = 0; j < JEDINICA_MAX; j++)
    {
        if (!gJed[j][jeIma] || !gJed[j][jeVlasnik] || !JedMotel(j) || gJed[j][jeDo] > sad) continue;
        new i = IgracRacuna(gJed[j][jeVlasnik]), bool:placeno = false;
        if (i != INVALID_PLAYER_ID) placeno = DajNovac(i, -gJed[j][jeCijena]);
        else
        {
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET novac = novac - %d WHERE id = %d AND novac >= %d", gJed[j][jeCijena], gJed[j][jeVlasnik], gJed[j][jeCijena]));
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT changes()");
            placeno = DB_GetFieldInt(res, 0) > 0;
            DB_FreeResultSet(res);
        }
        if (placeno)
        {
            gJed[j][jeDo] += MOTEL_DAN;
            if (gJed[j][jeDo] <= sad) gJed[j][jeDo] = sad + MOTEL_DAN;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE apartment_units SET paid = %d WHERE id = %d", gJed[j][jeDo], gJed[j][jeDb]));
            if (i != INVALID_PLAYER_ID) ObavijestInfo(i, T(i, "Motel"), T(i, "Platili ste najam motelske sobe za jos jedan dan."));
            continue;
        }
        JedIsprazni(j);
        JedPostaviVlasnika(j, 0, "", 0);
        if (i != INVALID_PLAYER_ID) ObavijestGreska(i, T(i, "Najam motelske sobe je istekao."));
    }
}
