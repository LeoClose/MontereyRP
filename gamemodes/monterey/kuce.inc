#define KUCA_MAX            1000
#define KUCA_DOMET          2.0
#define KUCA_ADMIN_DOMET    15.0
#define KUCA_PRODAJA        1273
#define KUCA_PRODANA        1272
#define KUCA_MAX_CIJENA     100000000

enum E_KUCA { bool:kuIma, kuDb, Float:kuX, Float:kuY, Float:kuZ, kuCijena, kuVlasnik, kuVlasnikIme[MAX_PLAYER_NAME + 1],
    STREAMER_TAG_PICKUP:kuPickup, STREAMER_TAG_3D_TEXT_LABEL:kuTekst }

static gKuca[KUCA_MAX][E_KUCA];
static gKucaPonuda[MAX_PLAYERS], gApKuca[MAX_PLAYERS];

InitKuce()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS houses (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, model INTEGER NOT NULL DEFAULT 0, price INTEGER NOT NULL, owner INTEGER NOT NULL DEFAULT 0, snap INTEGER NOT NULL DEFAULT 0)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT COUNT(*) FROM houses");
    new prazno = DB_GetRowCount(res) && !DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    if (prazno) KuceUvezi();
    res = DB_ExecuteQuery(gDB, "SELECT h.id, h.x, h.y, h.z, h.price, h.owner, IFNULL(a.name, ''), h.snap FROM houses h LEFT JOIN accounts a ON a.id = h.owner ORDER BY h.id LIMIT %d", KUCA_MAX);
    for (new k = 0, n = DB_GetRowCount(res); k < n; k++)
    {
        gKuca[k][kuDb] = DB_GetFieldInt(res, 0);
        gKuca[k][kuX] = DB_GetFieldFloat(res, 1);
        gKuca[k][kuY] = DB_GetFieldFloat(res, 2);
        gKuca[k][kuZ] = DB_GetFieldFloat(res, 3);
        gKuca[k][kuCijena] = DB_GetFieldInt(res, 4);
        gKuca[k][kuVlasnik] = DB_GetFieldInt(res, 5);
        DB_GetFieldString(res, 6, gKuca[k][kuVlasnikIme], MAX_PLAYER_NAME + 1);
        if (DB_GetFieldInt(res, 7)) KucaTlo(k);
        KucaStvori(k);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static KuceUvezi()
{
    new DB:izvor = DB_Open("kuce.db");
    if (!izvor) return;
    new DBResult:res = DB_ExecuteQuery(izvor, "SELECT x, y, z, model, price FROM houses ORDER BY id");
    if (res && DB_GetRowCount(res))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "BEGIN"));
        do DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO houses (x, y, z, model, price, snap) VALUES (%f, %f, %f, %d, %d, 1)", DB_GetFieldFloat(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 4)));
        while (DB_SelectNextRow(res));
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

KucaKey(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);
    new k = KucaNajbliza(x, y, z, KUCA_DOMET);
    if (k == -1) return 0;
    new body[128];
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
    KucaMakni(k);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM houses WHERE id = %d", gKuca[k][kuDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Obrisano."));
    return ShowKuceAdmin(playerid);
}

KucaDisconnect(playerid)
{
    gKucaPonuda[playerid] = 0;
}
