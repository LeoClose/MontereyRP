#define KUCA_MAX            1000
#define KUCA_DOMET          2.0
#define KUCA_ADMIN_DOMET    15.0
#define KUCA_PRODAJA        1273
#define KUCA_PRODANA        1272
#define KUCA_MAX_CIJENA     100000000
#define KUCA_SVIJET         20000
#define KUCA_DAN            86400
#define KUCA_NAJAM_MS       600000
#define KUCA_OTKUP_POSTO    50
#define KUCA_MAX_NAJAM      100000

enum E_KUCA { bool:kuIma, kuDb, Float:kuX, Float:kuY, Float:kuZ, kuCijena, kuVlasnik, kuVlasnikIme[MAX_PLAYER_NAME + 1], kuInt, kuVel, kuNajam, bool:kuZakljucano,
    STREAMER_TAG_PICKUP:kuPickup, STREAMER_TAG_3D_TEXT_LABEL:kuTekst }

enum E_KUCA_INT { kiIme[16], kiVel, kiInt, Float:kiX, Float:kiY, Float:kiZ, kiCijena }
static const gKucaInt[][E_KUCA_INT] = {
    { "Mala kuca", 0, 11, 2283.04, -1140.28, 1050.90, 10000 },
    { "Mala kuca", 0, 1, 223.20, 1287.08, 1082.14, 15000 },
    { "Srednja kuca", 1, 8, 2365.30, -1135.60, 1050.88, 30000 },
    { "Srednja kuca", 1, 12, 2324.53, -1149.54, 1050.71, 35000 },
    { "Srednja kuca", 1, 10, 2270.38, -1210.35, 1047.56, 45000 },
    { "Velika kuca", 2, 7, 225.68, 1021.45, 1084.02, 90000 },
    { "Vila", 2, 5, 140.17, 1366.07, 1083.65, 180000 }
};

static gKuca[KUCA_MAX][E_KUCA];
static gKucaPonuda[MAX_PLAYERS], gApKuca[MAX_PLAYERS], gKucaSpawn[MAX_PLAYERS], gKucaMeni[MAX_PLAYERS], gKucaOtkup[MAX_PLAYERS];

InitKuce()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS houses (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, model INTEGER NOT NULL DEFAULT 0, price INTEGER NOT NULL, owner INTEGER NOT NULL DEFAULT 0, snap INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT interior, rent FROM houses LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE houses ADD COLUMN interior INTEGER NOT NULL DEFAULT -1"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE houses ADD COLUMN rent INTEGER NOT NULL DEFAULT 0"));
    }
    res = DB_ExecuteQuery(gDB, "SELECT locked FROM houses LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE houses ADD COLUMN locked INTEGER NOT NULL DEFAULT 1"));
    res = DB_ExecuteQuery(gDB, "SELECT size FROM houses LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE houses ADD COLUMN size INTEGER NOT NULL DEFAULT 1"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS house_renters (house INTEGER NOT NULL, account INTEGER NOT NULL, paid INTEGER NOT NULL, PRIMARY KEY (house, account))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS player_spawn (account INTEGER PRIMARY KEY, house INTEGER NOT NULL)"));
    KuceUvezi();
    res = DB_ExecuteQuery(gDB, "SELECT h.id, h.x, h.y, h.z, h.price, h.owner, IFNULL(a.name, ''), h.snap, h.interior, h.rent, h.locked, h.size FROM houses h LEFT JOIN accounts a ON a.id = h.owner ORDER BY h.id LIMIT %d", KUCA_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++)
    {
        gKuca[k][kuDb] = DB_GetFieldInt(res, 0);
        gKuca[k][kuX] = DB_GetFieldFloat(res, 1);
        gKuca[k][kuY] = DB_GetFieldFloat(res, 2);
        gKuca[k][kuZ] = DB_GetFieldFloat(res, 3);
        gKuca[k][kuCijena] = DB_GetFieldInt(res, 4);
        gKuca[k][kuVlasnik] = DB_GetFieldInt(res, 5);
        DB_GetFieldString(res, 6, gKuca[k][kuVlasnikIme], MAX_PLAYER_NAME + 1);
        gKuca[k][kuInt] = DB_GetFieldInt(res, 8);
        gKuca[k][kuNajam] = DB_GetFieldInt(res, 9);
        gKuca[k][kuZakljucano] = DB_GetFieldInt(res, 10) != 0;
        gKuca[k][kuVel] = clamp(DB_GetFieldInt(res, 11), 0, 2);
        if (gKuca[k][kuInt] < 0 || gKuca[k][kuInt] >= sizeof gKucaInt) gKuca[k][kuInt] = KucaZadaniInt(gKuca[k][kuVel], gKuca[k][kuCijena]);
        if (DB_GetFieldInt(res, 7)) KucaTlo(k);
        KucaStvori(k);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
    SetTimer("KuceNajam", KUCA_NAJAM_MS, true);
}

static KucaZadaniInt(vel, cijena)
{
    if (vel == 0) return 0;
    if (vel == 1) return 2;
    return cijena < 900000 ? 5 : 6;
}

static KuceUvezi()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS server_flags (name TEXT PRIMARY KEY)"));
    new DBResult:zastava = DB_ExecuteQuery(gDB, "SELECT (SELECT COUNT(*) FROM server_flags WHERE name = 'kuce_uvoz'), (SELECT COUNT(*) FROM houses)");
    new bool:uvezeno = DB_GetFieldInt(zastava, 0) > 0, bool:imaKuca = DB_GetFieldInt(zastava, 1) > 0;
    DB_FreeResultSet(zastava);
    if (uvezeno) return;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO server_flags (name) VALUES ('kuce_uvoz')"));
    if (imaKuca) return;
    new DB:izvor = DB_Open("kuce.db");
    if (!izvor) return;
    new DBResult:res = DB_ExecuteQuery(izvor, "SELECT x, y, z, model, price, size FROM houses ORDER BY id");
    if (res && DB_GetRowCount(res))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "BEGIN"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TEMP TABLE IF NOT EXISTS kuce_ok (id INTEGER PRIMARY KEY)"));
        do
        {
            new Float:x = DB_GetFieldFloat(res, 0), Float:y = DB_GetFieldFloat(res, 1), Float:z = DB_GetFieldFloat(res, 2);
            new DBResult:ima = DB_ExecuteQuery(gDB, "SELECT id FROM houses WHERE x BETWEEN %f AND %f AND y BETWEEN %f AND %f AND z BETWEEN %f AND %f LIMIT 1", x - 2.0, x + 2.0, y - 2.0, y + 2.0, z - 2.5, z + 2.5);
            new id = DB_GetRowCount(ima) ? DB_GetFieldInt(ima, 0) : 0;
            DB_FreeResultSet(ima);
            if (!id)
            {
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO houses (x, y, z, model, price, size, snap) VALUES (%f, %f, %f, %d, %d, %d, 1)", x, y, z, DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 4), DB_GetFieldInt(res, 5)));
                id = ZadnjiId();
            }
            else
            {
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET size = %d WHERE id = %d", DB_GetFieldInt(res, 5), id));
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET x = %f, y = %f, z = %f, model = %d, price = %d, snap = 1 WHERE id = %d AND owner = 0", x, y, z, DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 4), id));
            }
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO kuce_ok (id) VALUES (%d)", id));
        }
        while (DB_SelectNextRow(res));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM houses WHERE owner = 0 AND model != 0 AND id NOT IN (SELECT id FROM kuce_ok)"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DROP TABLE kuce_ok"));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "COMMIT"));
    }
    if (res) DB_FreeResultSet(res);
    DB_Close(izvor);
}

static KucaTlo(k)
{
    new Float:hx, Float:hy, Float:hz;
    if (Kolizija_Zraka(gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ] + 1.5, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ] - 15.0, hx, hy, hz)) gKuca[k][kuZ] = hz + 1.0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET z = %f, snap = 0 WHERE id = %d", gKuca[k][kuZ], gKuca[k][kuDb]));
}

static KucaNatpis(k, natpis[], velicina = sizeof natpis)
{
    if (!gKuca[k][kuVlasnik]) return format(natpis, velicina, "Kuca na prodaju\n$%d\n[Y]", gKuca[k][kuCijena]);
    new ime[MAX_PLAYER_NAME + 1];
    strcopy(ime, gKuca[k][kuVlasnikIme]);
    for (new i = 0; ime[i]; i++) if (ime[i] == '_') ime[i] = ' ';
    if (gKuca[k][kuNajam]) return format(natpis, velicina, "Kuca\nVlasnik: %s\nNajam: $%d/dan\n[Y]", ime, gKuca[k][kuNajam]);
    return format(natpis, velicina, "Kuca\nVlasnik: %s\n[Y]", ime);
}

static KucaStvori(k)
{
    new natpis[96];
    KucaNatpis(k, natpis);
    gKuca[k][kuIma] = true;
    gKuca[k][kuPickup] = CreateDynamicPickup(gKuca[k][kuVlasnik] ? KUCA_PRODANA : KUCA_PRODAJA, 1, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ], 0, 0);
    gKuca[k][kuTekst] = CreateDynamic3DTextLabel(natpis, COLOR_ORG, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0);
}

static KucaMakni(k)
{
    DestroyDynamicPickup(gKuca[k][kuPickup]);
    DestroyDynamic3DTextLabel(gKuca[k][kuTekst]);
    gKuca[k][kuIma] = false;
}

static KucaOsvjezi(k)
{
    KucaMakni(k);
    KucaStvori(k);
}

static KucaPostaviVlasnika(k, racun, const ime[])
{
    if (gKuca[k][kuVlasnik] != racun) NamjestajKucaIsprazni(gKuca[k][kuDb]);
    gKuca[k][kuVlasnik] = racun;
    gKuca[k][kuVlasnikIme][0] = EOS;
    if (racun) strcopy(gKuca[k][kuVlasnikIme], ime, MAX_PLAYER_NAME + 1);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET owner = %d WHERE id = %d", racun, gKuca[k][kuDb]));
    KucaOsvjezi(k);
}

static KucaNajbliza(Float:x, Float:y, Float:z, Float:domet)
{
    new najbliza = -1, Float:bd = domet;
    for (new k = 0; k < KUCA_MAX; k++)
    {
        if (!gKuca[k][kuIma]) continue;
        new Float:d = VectorSize(gKuca[k][kuX] - x, gKuca[k][kuY] - y, gKuca[k][kuZ] - z);
        if (d <= bd)
        {
            bd = d;
            najbliza = k;
        }
    }
    return najbliza;
}

static KucaPoDb(db)
{
    for (new k = 0; k < KUCA_MAX; k++) if (gKuca[k][kuIma] && gKuca[k][kuDb] == db) return k;
    return -1;
}

static KucaIgraca(playerid)
{
    new k = KucaPoDb(GetPlayerVirtualWorld(playerid) - KUCA_SVIJET);
    return k != -1 && GetPlayerInterior(playerid) == gKucaInt[gKuca[k][kuInt]][kiInt] ? k : -1;
}

KucaInterijerDb(db)
{
    new k = KucaPoDb(db);
    return k == -1 ? -1 : gKucaInt[gKuca[k][kuInt]][kiInt];
}

KucaIgracaDb(playerid)
{
    new k = KucaIgraca(playerid);
    return k != -1 && gKuca[k][kuVlasnik] == gAccountId[playerid] ? gKuca[k][kuDb] : 0;
}

static bool:KucaStanar(playerid, k)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM house_renters WHERE house = %d AND account = %d", gKuca[k][kuDb], gAccountId[playerid]);
    new bool:ima = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return ima;
}

static KucaUdi(playerid, k)
{
    new i = gKuca[k][kuInt];
    SetPlayerInterior(playerid, gKucaInt[i][kiInt]);
    SetPlayerVirtualWorld(playerid, KUCA_SVIJET + gKuca[k][kuDb]);
    SetPlayerPos(playerid, gKucaInt[i][kiX], gKucaInt[i][kiY], gKucaInt[i][kiZ]);
    SetCameraBehindPlayer(playerid);
}

static KucaVani(playerid, k)
{
    SetPlayerInterior(playerid, 0);
    SetPlayerVirtualWorld(playerid, 0);
    SetPlayerPos(playerid, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ]);
    SetCameraBehindPlayer(playerid);
}

static KucaIsprazni(k)
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && KucaIgraca(i) == k) KucaVani(i, k);
}

static KucaMakniSpawn(db)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_spawn WHERE house = %d", db));
    for (new i = 0; i < MAX_PLAYERS; i++) if (gKucaSpawn[i] == db) gKucaSpawn[i] = 0;
}

static KucaIsplati(racun, iznos)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!IsPlayerConnected(i) || !gUlogiran[i] || gAccountId[i] != racun) continue;
        DajNovac(i, iznos);
        return;
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET novac = novac + %d WHERE id = %d", iznos, racun));
}

static KucaOslobodi(k)
{
    KucaIsprazni(k);
    gKuca[k][kuZakljucano] = true;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET locked = 1 WHERE id = %d", gKuca[k][kuDb]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM house_renters WHERE house = %d", gKuca[k][kuDb]));
    KucaMakniSpawn(gKuca[k][kuDb]);
    gKuca[k][kuNajam] = 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET rent = 0 WHERE id = %d", gKuca[k][kuDb]));
    KucaPostaviVlasnika(k, 0, "");
}

static KucaIzlazMeni(playerid, k)
{
    new list[256];
    gKucaMeni[playerid] = k + 1;
    if (gKuca[k][kuVlasnik] == gAccountId[playerid]) strcat(list, T(playerid, "Izadi\nPreuredi kucu\nPostavi kao mjesto spawna\nIznajmljivanje\nNamjestaj"));
    else if (KucaStanar(playerid, k)) strcat(list, T(playerid, "Izadi\nPostavi kao mjesto spawna\nPrestani iznajmljivati"));
    else
    {
        KucaVani(playerid, k);
        return 1;
    }
    strcat(list, "\n");
    strcat(list, T(playerid, gKuca[k][kuZakljucano] ? ("Otkljucaj kucu") : ("Zakljucaj kucu")));
    return PrikaziDialog(playerid, DLG_KUCA_IZLAZ, DIALOG_STYLE_LIST, T(playerid, "Kuca"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

KucaKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return 0;
    new Float:x, Float:y, Float:z, k = KucaIgraca(playerid);
    if (k != -1)
    {
        new i = gKuca[k][kuInt];
        if (!IsPlayerInRangeOfPoint(playerid, KUCA_DOMET, gKucaInt[i][kiX], gKucaInt[i][kiY], gKucaInt[i][kiZ])) return 0;
        KucaIzlazMeni(playerid, k);
        return 1;
    }
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    GetPlayerPos(playerid, x, y, z);
    k = KucaNajbliza(x, y, z, KUCA_DOMET);
    if (k == -1) return 0;
    new body[128];
    if (gKuca[k][kuVlasnik] && (!gKuca[k][kuZakljucano] || gKuca[k][kuVlasnik] == gAccountId[playerid] || KucaStanar(playerid, k)))
    {
        KucaUdi(playerid, k);
        return 1;
    }
    if (gKuca[k][kuVlasnik] && gKuca[k][kuNajam])
    {
        gKucaPonuda[playerid] = k + 1;
        format(body, sizeof body, T(playerid, "Iznajmiti sobu za $%d dnevno? Prvi dan placate odmah."), gKuca[k][kuNajam]);
        PrikaziDialog(playerid, DLG_KUCA_NAJMI, DIALOG_STYLE_MSGBOX, T(playerid, "Najam kuce"), body, T(playerid, "Iznajmi"), T(playerid, "Odustani"));
        return 1;
    }
    if (!gKuca[k][kuVlasnik])
    {
        gKucaPonuda[playerid] = k + 1;
        format(body, sizeof body, T(playerid, "Kupiti ovu kucu za $%d?"), gKuca[k][kuCijena]);
        PrikaziDialog(playerid, DLG_KUCA, DIALOG_STYLE_MSGBOX, T(playerid, "Kuca na prodaju"), body, T(playerid, "Kupi"), T(playerid, "Odustani"));
        return 1;
    }
    if (gKuca[k][kuVlasnik] == gAccountId[playerid]) format(body, sizeof body, T(playerid, "Ovo je vasa kuca. Trzisna vrijednost: $%d."), gKuca[k][kuCijena]);
    else format(body, sizeof body, T(playerid, "Ova kuca ima vlasnika. Trzisna vrijednost: $%d."), gKuca[k][kuCijena]);
    PrikaziDialog(playerid, DLG_NONE, DIALOG_STYLE_MSGBOX, T(playerid, "Kuca"), body, T(playerid, "Zatvori"));
    return 1;
}

DLG:kuca(playerid, response)
{
    new k = gKucaPonuda[playerid] - 1;
    gKucaPonuda[playerid] = 0;
    if (!response || k < 0 || !gKuca[k][kuIma]) return 1;
    if (gKuca[k][kuVlasnik]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kucu je netko vec kupio."));
    if (!IsPlayerInRangeOfPoint(playerid, KUCA_DOMET * 2.0, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ])) return 1;
    if (!DajNovac(playerid, -gKuca[k][kuCijena])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    new ime[MAX_PLAYER_NAME + 1];
    GetPlayerName(playerid, ime, sizeof ime);
    KucaPostaviVlasnika(k, gAccountId[playerid], ime);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste kucu za $%d."), gKuca[k][kuCijena]);
}

ShowKuceAdmin(playerid)
{
    new Float:x, Float:y, Float:z, title[96];
    GetPlayerPos(playerid, x, y, z);
    new k = GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) ? -1 : KucaNajbliza(x, y, z, KUCA_ADMIN_DOMET);
    gApKuca[playerid] = k;
    if (k == -1) return PrikaziDialog(playerid, DLG_APANEL_KUCA, DIALOG_STYLE_LIST, T(playerid, "Kuce - nema kuce u blizini"), T(playerid, "Nova kuca na mojoj poziciji"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
    if (gKuca[k][kuVlasnik]) format(title, sizeof title, T(playerid, "Kuca [%d] - vlasnik: %s - $%d"), gKuca[k][kuDb], gKuca[k][kuVlasnikIme], gKuca[k][kuCijena]);
    else format(title, sizeof title, T(playerid, "Kuca [%d] - na prodaju - $%d"), gKuca[k][kuDb], gKuca[k][kuCijena]);
    return PrikaziDialog(playerid, DLG_APANEL_KUCA, DIALOG_STYLE_LIST, title, T(playerid, "Nova kuca na mojoj poziciji\nPostavi vlasnika\nPostavi trzisnu cijenu\nObrisi kucu"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static KucaNova(playerid)
{
    new k = 0;
    while (k < KUCA_MAX && gKuca[k][kuIma]) k++;
    if (k == KUCA_MAX || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Kuca se postavlja samo izvana."));
    GetPlayerPos(playerid, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ]);
    gKuca[k][kuCijena] = 50000;
    gKuca[k][kuVel] = 1;
    gKuca[k][kuInt] = KucaZadaniInt(1, 50000);
    gKuca[k][kuVlasnik] = 0;
    gKuca[k][kuVlasnikIme][0] = EOS;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO houses (x, y, z, price) VALUES (%f, %f, %f, %d)", gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ], gKuca[k][kuCijena]));
    gKuca[k][kuDb] = ZadnjiId();
    KucaStvori(k);
    gApKuca[playerid] = k;
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kuca dodana ($%d). Postavite joj cijenu."), gKuca[k][kuCijena]);
}

DLG:apanel_kuca(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new k = gApKuca[playerid];
    if (listitem == 0)
    {
        KucaNova(playerid);
        return ShowKuceAdmin(playerid);
    }
    if (k < 0 || !gKuca[k][kuIma]) return ShowKuceAdmin(playerid);
    if (listitem == 1) return PrikaziDialog(playerid, DLG_APANEL_KUCA_VLASNIK, DIALOG_STYLE_INPUT, T(playerid, "Vlasnik kuce"), T(playerid, "Unesite id/ime igraca (tocno ime ako je offline, 0 = na prodaju):"), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
    if (listitem == 2) return PrikaziDialog(playerid, DLG_APANEL_KUCA_CIJENA, DIALOG_STYLE_INPUT, T(playerid, "Trzisna cijena"), T(playerid, "Unesite novu cijenu:"), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
    new body[96];
    format(body, sizeof body, T(playerid, "Obrisati kucu [%d]?"), gKuca[k][kuDb]);
    return PrikaziDialog(playerid, DLG_APANEL_KUCA_BRISI, DIALOG_STYLE_MSGBOX, T(playerid, "Kuce"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
}

DLG:apanel_kuca_vlasnik(playerid, response, const inputtext[])
{
    new k = gApKuca[playerid];
    if (!response || k < 0 || !gKuca[k][kuIma]) return ShowKuceAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!strcmp(inputtext, "0"))
    {
        KucaPostaviVlasnika(k, 0, "");
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Kuca je sada na prodaju."));
        return ShowKuceAdmin(playerid);
    }
    new ime[MAX_PLAYER_NAME + 1], racun = 0, target = FindPlayer(inputtext);
    if (target != INVALID_PLAYER_ID && gUlogiran[target])
    {
        GetPlayerName(target, ime, sizeof ime);
        racun = gAccountId[target];
    }
    else
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, name FROM accounts WHERE name = '%q'", inputtext);
        if (DB_GetRowCount(res))
        {
            racun = DB_GetFieldInt(res, 0);
            DB_GetFieldString(res, 1, ime, sizeof ime);
        }
        DB_FreeResultSet(res);
    }
    if (!racun) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    else
    {
        KucaPostaviVlasnika(k, racun, ime);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vlasnik kuce: %s."), ime);
    }
    return ShowKuceAdmin(playerid);
}

DLG:apanel_kuca_cijena(playerid, response, const inputtext[])
{
    new k = gApKuca[playerid];
    if (!response || k < 0 || !gKuca[k][kuIma]) return ShowKuceAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new cijena = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || cijena < 1 || cijena > KUCA_MAX_CIJENA) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna cijena."));
    else
    {
        gKuca[k][kuCijena] = cijena;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET price = %d WHERE id = %d", cijena, gKuca[k][kuDb]));
        KucaOsvjezi(k);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Trzisna cijena: $%d."), cijena);
    }
    return ShowKuceAdmin(playerid);
}

DLG:apanel_kuca_brisi(playerid, response)
{
    new k = gApKuca[playerid];
    if (!response || k < 0 || !gKuca[k][kuIma]) return ShowKuceAdmin(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    KucaOslobodi(k);
    KucaMakni(k);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM houses WHERE id = %d", gKuca[k][kuDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    return ShowKuceAdmin(playerid);
}

KucaZakljucaj(playerid)
{
    new k = KucaIgraca(playerid);
    if (k == -1 && !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid))
    {
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        k = KucaNajbliza(x, y, z, KUCA_DOMET);
    }
    if (k == -1 || !gKuca[k][kuVlasnik] || (gKuca[k][kuVlasnik] != gAccountId[playerid] && !KucaStanar(playerid, k))) return 0;
    gKuca[k][kuZakljucano] = !gKuca[k][kuZakljucano];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET locked = %d WHERE id = %d", gKuca[k][kuZakljucano] ? 1 : 0, gKuca[k][kuDb]));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, gKuca[k][kuZakljucano] ? ("* %s zakljucava kucu.") : ("* %s otkljucava kucu."), GetRPIme(playerid), "", 1);
    return 1;
}

KucaDisconnect(playerid)
{
    gKucaPonuda[playerid] = 0;
    gKucaMeni[playerid] = 0;
    gKucaSpawn[playerid] = 0;
    gKucaOtkup[playerid] = 0;
}

DLG:kuca_izlaz(playerid, response, listitem)
{
    new k = gKucaMeni[playerid] - 1;
    gKucaMeni[playerid] = 0;
    if (!response || k < 0 || !gKuca[k][kuIma] || KucaIgraca(playerid) != k) return 1;
    new bool:vlasnik = gKuca[k][kuVlasnik] == gAccountId[playerid];
    if (!vlasnik && !KucaStanar(playerid, k)) return 1;
    if (listitem == 0)
    {
        KucaVani(playerid, k);
        return 1;
    }
    if (listitem == (vlasnik ? 5 : 3))
    {
        KucaZakljucaj(playerid);
        return KucaIzlazMeni(playerid, k);
    }
    if ((vlasnik && listitem == 2) || (!vlasnik && listitem == 1))
    {
        gKucaSpawn[playerid] = gKuca[k][kuDb];
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO player_spawn (account, house) VALUES (%d, %d)", gAccountId[playerid], gKuca[k][kuDb]));
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kuca je vase mjesto spawna."));
    }
    if (!vlasnik)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM house_renters WHERE house = %d AND account = %d", gKuca[k][kuDb], gAccountId[playerid]));
        if (gKucaSpawn[playerid] == gKuca[k][kuDb])
        {
            gKucaSpawn[playerid] = 0;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_spawn WHERE account = %d", gAccountId[playerid]));
        }
        KucaVani(playerid, k);
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Vise ne iznajmljujete ovu kucu."));
    }
    if (listitem == 4) return NamjestajMeni(playerid, gKuca[k][kuDb]);
    gKucaMeni[playerid] = k + 1;
    if (listitem == 3) return PrikaziDialog(playerid, DLG_KUCA_NAJAM, DIALOG_STYLE_INPUT, T(playerid, "Najam"), T(playerid, "Unesite dnevnu cijenu najma (0 = iskljuceno):"), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
    new list[512], row[64], len = 0;
    for (new i = 0; i < sizeof gKucaInt; i++)
    {
        if (gKucaInt[i][kiVel] != gKuca[k][kuVel]) continue;
        format(row, sizeof row, "%d. %s - $%d%s", i + 1, T(playerid, gKucaInt[i][kiIme]), gKucaInt[i][kiCijena], T(playerid, i == gKuca[k][kuInt] ? (" (trenutno)") : ("")));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_KUCA_PREUREDI, DIALOG_STYLE_LIST, T(playerid, "Preuredi kucu"), list, T(playerid, "Kupi"), T(playerid, "Natrag"));
}

DLG:kuca_preuredi(playerid, response, listitem)
{
    new k = gKucaMeni[playerid] - 1;
    gKucaMeni[playerid] = 0;
    if (k < 0 || !gKuca[k][kuIma] || gKuca[k][kuVlasnik] != gAccountId[playerid] || KucaIgraca(playerid) != k) return 1;
    if (!response) return KucaIzlazMeni(playerid, k);
    new izbor = -1;
    for (new i = 0; i < sizeof gKucaInt && izbor == -1; i++)
        if (gKucaInt[i][kiVel] == gKuca[k][kuVel] && !listitem--) izbor = i;
    if (izbor == -1) return 1;
    listitem = izbor;
    if (listitem == gKuca[k][kuInt]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ovo je vec vas interijer."));
    if (!DajNovac(playerid, -gKucaInt[listitem][kiCijena])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    new stari = gKuca[k][kuInt];
    NamjestajKucaIsprazni(gKuca[k][kuDb]);
    gKuca[k][kuInt] = listitem;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET interior = %d WHERE id = %d", listitem, gKuca[k][kuDb]));
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && GetPlayerVirtualWorld(i) == KUCA_SVIJET + gKuca[k][kuDb] && GetPlayerInterior(i) == gKucaInt[stari][kiInt]) KucaUdi(i, k);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste preuredenje ($%d)."), gKucaInt[listitem][kiCijena]);
}

DLG:kuca_najam(playerid, response, const inputtext[])
{
    new k = gKucaMeni[playerid] - 1;
    gKucaMeni[playerid] = 0;
    if (k < 0 || !gKuca[k][kuIma] || gKuca[k][kuVlasnik] != gAccountId[playerid]) return 1;
    if (!response) return KucaIzlazMeni(playerid, k);
    new najam = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || najam > KUCA_MAX_NAJAM) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna cijena."));
    gKuca[k][kuNajam] = najam;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE houses SET rent = %d WHERE id = %d", najam, gKuca[k][kuDb]));
    KucaOsvjezi(k);
    if (najam) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dnevni najam: $%d."), najam);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Najam je iskljucen. Postojeci stanari ostaju dok sami ne odu."));
}

DLG:kuca_najmi(playerid, response)
{
    new k = gKucaPonuda[playerid] - 1;
    gKucaPonuda[playerid] = 0;
    if (!response || k < 0 || !gKuca[k][kuIma] || !gKuca[k][kuVlasnik] || !gKuca[k][kuNajam]) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, KUCA_DOMET * 2.0, gKuca[k][kuX], gKuca[k][kuY], gKuca[k][kuZ]) || KucaStanar(playerid, k)) return 1;
    if (!DajNovac(playerid, -gKuca[k][kuNajam])) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    KucaIsplati(gKuca[k][kuVlasnik], gKuca[k][kuNajam]);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO house_renters (house, account, paid) VALUES (%d, %d, %d)", gKuca[k][kuDb], gAccountId[playerid], gettime()));
    KucaUdi(playerid, k);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Iznajmili ste sobu. Sljedeca naplata za 24 sata."));
}

static KucaNaplati(playerid)
{
    new sad = gettime(), DBResult:res = DB_ExecuteQuery(gDB, "SELECT r.house, h.rent, h.owner, r.paid FROM house_renters r JOIN houses h ON h.id = r.house WHERE r.account = %d", gAccountId[playerid]);
    for (new n = DB_GetRowCount(res), i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        new kuca = DB_GetFieldInt(res, 0), najam = DB_GetFieldInt(res, 1), vlasnik = DB_GetFieldInt(res, 2), dana = (sad - DB_GetFieldInt(res, 3)) / KUCA_DAN;
        if (dana <= 0) continue;
        if (najam && vlasnik && DajNovac(playerid, -najam * dana))
        {
            KucaIsplati(vlasnik, najam * dana);
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE house_renters SET paid = paid + %d WHERE house = %d AND account = %d", dana * KUCA_DAN, kuca, gAccountId[playerid]));
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Platili ste najam: $%d."), najam * dana);
            continue;
        }
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM house_renters WHERE house = %d AND account = %d", kuca, gAccountId[playerid]));
        if (gKucaSpawn[playerid] == kuca)
        {
            gKucaSpawn[playerid] = 0;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_spawn WHERE account = %d", gAccountId[playerid]));
        }
        new k = KucaPoDb(kuca);
        if (k != -1 && KucaIgraca(playerid) == k) KucaVani(playerid, k);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Iselili ste se iz iznajmljene kuce."));
    }
    DB_FreeResultSet(res);
}

forward KuceNajam();
public KuceNajam()
{
    for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && !IsPlayerNPC(i) && gUlogiran[i]) KucaNaplati(i);
}

KucaPrijava(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT house FROM player_spawn WHERE account = %d", gAccountId[playerid]);
    gKucaSpawn[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    KucaNaplati(playerid);
}

KucaSpawn(playerid)
{
    if (!gKucaSpawn[playerid]) return;
    new k = KucaPoDb(gKucaSpawn[playerid]);
    if (k != -1 && gKuca[k][kuVlasnik] && (gKuca[k][kuVlasnik] == gAccountId[playerid] || KucaStanar(playerid, k))) KucaUdi(playerid, k);
    else
    {
        gKucaSpawn[playerid] = 0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM player_spawn WHERE account = %d", gAccountId[playerid]));
    }
}

KucaProdajaPopis(playerid)
{
    new list[1024], row[96], len = 0, ima = 0;
    for (new k = 0; k < KUCA_MAX; k++)
    {
        if (!gKuca[k][kuIma] || gKuca[k][kuVlasnik] != gAccountId[playerid]) continue;
        format(row, sizeof row, "[%d] %s - $%d", gKuca[k][kuDb], CestaZona(gKuca[k][kuX], gKuca[k][kuY]), gKuca[k][kuCijena] * KUCA_OTKUP_POSTO / 100);
        DodajStupac(list, len, row);
        ima++;
    }
    if (!ima) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate kucu."));
    return PrikaziDialog(playerid, DLG_OPSTINA_KUCA, DIALOG_STYLE_LIST, T(playerid, "Prodaja kuce"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:opstina_kuca(playerid, response, listitem)
{
    if (!response) return 1;
    new n = listitem;
    for (new k = 0; k < KUCA_MAX; k++)
    {
        if (!gKuca[k][kuIma] || gKuca[k][kuVlasnik] != gAccountId[playerid] || n--) continue;
        new body[128];
        gKucaOtkup[playerid] = k + 1;
        format(body, sizeof body, T(playerid, "Prodati kucu [%d] gradu za $%d?"), gKuca[k][kuDb], gKuca[k][kuCijena] * KUCA_OTKUP_POSTO / 100);
        return PrikaziDialog(playerid, DLG_OPSTINA_KUCA_POTVRDA, DIALOG_STYLE_MSGBOX, T(playerid, "Prodaja kuce"), body, T(playerid, "Prodaj"), T(playerid, "Natrag"));
    }
    return 1;
}

DLG:opstina_kuca_potvrda(playerid, response)
{
    new k = gKucaOtkup[playerid] - 1;
    gKucaOtkup[playerid] = 0;
    if (!response) return KucaProdajaPopis(playerid);
    if (k < 0 || !gKuca[k][kuIma] || gKuca[k][kuVlasnik] != gAccountId[playerid]) return 1;
    new iznos = gKuca[k][kuCijena] * KUCA_OTKUP_POSTO / 100;
    KucaOslobodi(k);
    DajNovac(playerid, iznos);
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste kucu za $%d."), iznos);
}
