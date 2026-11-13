new gStatsLevel[MAX_PLAYERS];
new gStatsRespekti[MAX_PLAYERS];
new gStatsPlaytime[MAX_PLAYERS];
new gStatsLastUpdate[MAX_PLAYERS];
new gStatsUnsaved[MAX_PLAYERS];
new gStatsStranica[MAX_PLAYERS];
new bool:gStatsImaSljedecu[MAX_PLAYERS];

#define STATS_KUCA_PO_STRANICI 40

InitStats()
{
    return StatsEnsureColumn("level", "INTEGER NOT NULL DEFAULT 1")
        && StatsEnsureColumn("respekti", "INTEGER NOT NULL DEFAULT 0")
        && StatsEnsureColumn("playtime", "INTEGER NOT NULL DEFAULT 0");
}

static StatsEnsureColumn(const name[], const definition[])
{
    new DBResult:res = DB_ExecuteQuery(gDB, "PRAGMA table_info(accounts)");
    if (res == DBResult:0) return 0;
    new field[32], bool:found;
    if (DB_GetRowCount(res)) do
    {
        DB_GetFieldString(res, 1, field, sizeof field);
        if (!strcmp(field, name))
        {
            found = true;
            break;
        }
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);

    if (found) return 1;
    res = DB_ExecuteQuery(gDB, "ALTER TABLE accounts ADD COLUMN %s %s", name, definition);
    if (res == DBResult:0)
    {
        printf("[Stats] Dodavanje stupca %s nije uspjelo.", name);
        return 0;
    }
    DB_FreeResultSet(res);
    return 1;
}

StatsConnect(playerid)
{
    gStatsLevel[playerid] = 1;
    gStatsRespekti[playerid] = 0;
    gStatsPlaytime[playerid] = 0;
    gStatsLastUpdate[playerid] = 0;
    gStatsUnsaved[playerid] = 0;
    gStatsStranica[playerid] = 0;
    gStatsImaSljedecu[playerid] = false;
}

StatsLoad(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB,
        "SELECT level, respekti, playtime FROM accounts WHERE id = %d", gAccountId[playerid]);
    if (res == DBResult:0 || !DB_GetRowCount(res))
    {
        if (res != DBResult:0) DB_FreeResultSet(res);
        printf("[Stats] Ucitavanje statistike racuna %d nije uspjelo.", gAccountId[playerid]);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Statistika nije ucitana. Ponovno se spojite."));
        gUlogiran[playerid] = false;
        KickTimer(playerid);
        return 0;
    }
    gStatsLevel[playerid] = DB_GetFieldInt(res, 0);
    gStatsRespekti[playerid] = DB_GetFieldInt(res, 1);
    gStatsPlaytime[playerid] = DB_GetFieldInt(res, 2);
    DB_FreeResultSet(res);

    if (gStatsLevel[playerid] < 1) gStatsLevel[playerid] = 1;
    if (gStatsRespekti[playerid] < 0) gStatsRespekti[playerid] = 0;
    if (gStatsPlaytime[playerid] < 0) gStatsPlaytime[playerid] = 0;
    SetPlayerScore(playerid, gStatsLevel[playerid]);
    gStatsLastUpdate[playerid] = gettime();
    return 1;
}

static StatsPotrebno(level)
{
    new potrebno = 6;
    for (new i = 1; i < level; i++)
    {
        if (potrebno > 1073741823) return 2147483647;
        potrebno *= 2;
    }
    return potrebno;
}

StatsUpdate(playerid)
{
    if (!gUlogiran[playerid] || !gStatsLastUpdate[playerid]) return;
    new now = gettime(), elapsed = now - gStatsLastUpdate[playerid];
    if (elapsed <= 0) return;
    gStatsLastUpdate[playerid] = now;
    new sati = gStatsPlaytime[playerid] / 3600;
    gStatsPlaytime[playerid] += elapsed;
    gStatsUnsaved[playerid] += elapsed;
    gStatsRespekti[playerid] += gStatsPlaytime[playerid] / 3600 - sati;
    new potrebno = StatsPotrebno(gStatsLevel[playerid]);
    while (gStatsRespekti[playerid] >= potrebno)
    {
        gStatsRespekti[playerid] -= potrebno;
        gStatsLevel[playerid]++;
        SetPlayerScore(playerid, gStatsLevel[playerid]);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Cestitam, presli ste na sledeci level!"));
        potrebno = StatsPotrebno(gStatsLevel[playerid]);
    }
}

StatsTick()
{
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if (!gUlogiran[playerid]) continue;
        StatsUpdate(playerid);
        if (gStatsUnsaved[playerid] >= 60) StatsSave(playerid);
    }
}

StatsSave(playerid)
{
    if (!gAccountId[playerid] || !gStatsLastUpdate[playerid]) return 0;
    StatsUpdate(playerid);
    new DBResult:res = DB_ExecuteQuery(gDB,
        "UPDATE accounts SET level = %d, respekti = %d, playtime = %d WHERE id = %d",
        gStatsLevel[playerid], gStatsRespekti[playerid], gStatsPlaytime[playerid], gAccountId[playerid]);
    if (res == DBResult:0)
    {
        printf("[Stats] Spremanje statistike racuna %d nije uspjelo.", gAccountId[playerid]);
        return 0;
    }
    DB_FreeResultSet(res);
    gStatsUnsaved[playerid] = 0;
    return 1;
}

StatsSaveAll()
{
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
        if (gUlogiran[playerid]) StatsSave(playerid);
}

StatsDisconnect(playerid)
{
    if (gUlogiran[playerid]) StatsSave(playerid);
}

static StatsPrikazi(playerid, stranica)
{
    if (!gUlogiran[playerid]) return 1;
    StatsUpdate(playerid);

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT balance FROM bank_accounts WHERE account = %d", gAccountId[playerid]);
    if (res == DBResult:0)
    {
        printf("[Stats] Ucitavanje bankovnog racuna %d nije uspjelo.", gAccountId[playerid]);
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Podaci za statistiku nisu dostupni. Pokusajte ponovno."));
    }
    new banka[32];
    if (DB_GetRowCount(res)) format(banka, sizeof banka, "$%d", DB_GetFieldInt(res, 0));
    else strcopy(banka, T(playerid, "Nema"), sizeof banka);
    DB_FreeResultSet(res);

    res = DB_ExecuteQuery(gDB, "SELECT id FROM houses WHERE owner = %d ORDER BY id LIMIT %d OFFSET %d",
        gAccountId[playerid], STATS_KUCA_PO_STRANICI + 1, stranica * STATS_KUCA_PO_STRANICI);
    if (res == DBResult:0)
    {
        printf("[Stats] Ucitavanje kuca racuna %d nije uspjelo.", gAccountId[playerid]);
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Podaci za statistiku nisu dostupni. Pokusajte ponovno."));
    }
    new kuce[STATS_KUCA_PO_STRANICI * 13 + 1], row[16], brojKuca = DB_GetRowCount(res);
    for (new i = 0; i < brojKuca && i < STATS_KUCA_PO_STRANICI; i++)
    {
        if (i) strcat(kuce, i % 5 == 0 ? ("\n") : (", "));
        format(row, sizeof row, "%d", DB_GetFieldInt(res, 0));
        strcat(kuce, row);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
    if (!brojKuca) strcopy(kuce, T(playerid, "Nema"), sizeof kuce);

    new vlasnistva[VLASNISTVO_MAX], vozila[VLASNISTVO_MAX * 13 + 1];
    new brojVozila = VlasnistvaIgraca(playerid, vlasnistva);
    for (new i = 0; i < brojVozila; i++)
    {
        format(row, sizeof row, i ? (", %d") : ("%d"), vlasnistva[i]);
        strcat(vozila, row);
    }
    if (!brojVozila) strcopy(vozila, T(playerid, "Nema"), sizeof vozila);

    new org[MAX_ORG_NAME], body[2048];
    if (!GetOrgIme(gOrg[playerid], org)) strcopy(org, T(playerid, "Civil"), sizeof org);
    format(body, sizeof body,
        "{FFFFFF}%s\n{8AB4F8}%s: {FFFFFF}%d\n\
        {8AB4F8}%s: {FFFFFF}[%d/%d]\n{8AB4F8}%s: {FFFFFF}%d h %d min\n\
        {8AB4F8}%s: {FFFFFF}%s\n{8AB4F8}%s: {FFFFFF}%d\n\n\
        {FFFFFF}%s\n{8AB4F8}%s: {FFFFFF}$%d\n{8AB4F8}%s: {FFFFFF}%s\n\
        {8AB4F8}%s: {FFFFFF}%d\n\n{FFFFFF}%s\n{8AB4F8}%s: {FFFFFF}%s\n{8AB4F8}%s: {FFFFFF}%s",
        T(playerid, "Podaci sa servera"), T(playerid, "Level"), gStatsLevel[playerid], T(playerid, "Respekti"), gStatsRespekti[playerid],
        StatsPotrebno(gStatsLevel[playerid]), T(playerid, "Sati igre"),
        gStatsPlaytime[playerid] / 3600, (gStatsPlaytime[playerid] % 3600) / 60,
        T(playerid, "Organizacija"), org, T(playerid, "Wanted level"), GetWanted(playerid),
        T(playerid, "Sredstva"), T(playerid, "Novac u dzepu"), gNovac[playerid],
        T(playerid, "Novac u banci"), banka, T(playerid, "Casino Tokeni"), gInv[playerid][ITEM_ZETONI],
        T(playerid, "Imovina"), T(playerid, "Vozila (ID)"), vozila, T(playerid, "Kuce (ID)"), kuce);
    gStatsStranica[playerid] = stranica;
    gStatsImaSljedecu[playerid] = brojKuca > STATS_KUCA_PO_STRANICI;
    return PrikaziDialog(playerid, DLG_STATS, DIALOG_STYLE_MSGBOX, T(playerid, "Statistika"), body,
        T(playerid, gStatsImaSljedecu[playerid] ? ("Sljedeca") : ("U redu")),
        gStatsImaSljedecu[playerid] ? T(playerid, "Zatvori") : (""));
}

CMD:stats(playerid, params[])
{
    return StatsPrikazi(playerid, 0);
}

DLG:stats(playerid, response)
{
    if (response && gStatsImaSljedecu[playerid]) return StatsPrikazi(playerid, gStatsStranica[playerid] + 1);
    return 1;
}
