#define MAX_ORG_NAME        32
#define ORG_RANK_LEN        32
#define ORG_VEH_RESPAWN     600
#define COLOR_ORG           0x8AB4F8FF
#define ORG_PANEL_FIXED     4
#define ORG_MEMBERS_SQL     "SELECT id, name, org_rank, org_perms FROM accounts WHERE org = %d ORDER BY (org_perms & 8) DESC, name COLLATE NOCASE, id"
#define ORG_MEMBER_AT_SQL   "SELECT id FROM accounts WHERE org = %d ORDER BY (org_perms & 8) DESC, name COLLATE NOCASE, id LIMIT 1 OFFSET %d"

#define PERM_INVITE         1
#define PERM_VEHICLES       2
#define PERM_SAFE           4
#define PERM_LEADER         8
#define PERM_COUNT          3

static const gPermName[PERM_COUNT][] = { "Pozivanje i izbacivanje", "Kupovina vozila", "Pristup sefu" };
new const gOrgTypeName[8][] = { "Banda", "Mafija", "Legalna", "LSPD", "Medicinska sluzba", "Vatrogasci", "Taksi sluzba", "Mehanicari" };
new gOrgNova;

new gOrg[MAX_PLAYERS];
new gOrgRank[MAX_PLAYERS][ORG_RANK_LEN];
new gOrgPerms[MAX_PLAYERS];
new gOrgSkin[MAX_PLAYERS] = {-1, ...};
new gOrgInvite[MAX_PLAYERS];
new gOrgEditAcc[MAX_PLAYERS];
new gOrgVehOrg[MAX_VEHICLES];
new gOrgVehDb[MAX_VEHICLES];

InitOrg()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS orgs (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE COLLATE NOCASE,\
        type INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO orgs (name, type, created) VALUES ('LS News', 2, %d)", gettime()));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_skins (\
        org INTEGER NOT NULL, skin INTEGER NOT NULL, PRIMARY KEY (org, skin))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_vehicles (\
        id INTEGER PRIMARY KEY AUTOINCREMENT, org INTEGER NOT NULL, model INTEGER NOT NULL,\
        x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, c1 INTEGER NOT NULL, c2 INTEGER NOT NULL)"));

    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, org, model, x, y, z, a, c1, c2 FROM org_vehicles");
    if (DB_GetRowCount(res)) do
    {
        KreirajOrgVozilo(DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 1), DB_GetFieldInt(res, 2),
            DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4), DB_GetFieldFloat(res, 5), DB_GetFieldFloat(res, 6),
            DB_GetFieldInt(res, 7), DB_GetFieldInt(res, 8));
    } while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
}

KreirajOrgVozilo(dbid, org, model, Float:x, Float:y, Float:z, Float:a, c1, c2)
{
    new veh = CreateVehicle(model, x, y, z, a, c1, c2, ORG_VEH_RESPAWN);
    if (veh == INVALID_VEHICLE_ID) return INVALID_VEHICLE_ID;
    gOrgVehOrg[veh] = org;
    gOrgVehDb[veh] = dbid;
    return veh;
}

OrgConnect(playerid)
{
    gOrg[playerid] = 0;
    gOrgRank[playerid][0] = EOS;
    gOrgPerms[playerid] = 0;
    gOrgSkin[playerid] = -1;
    gOrgInvite[playerid] = 0;
    gOrgEditAcc[playerid] = 0;
}

bool:GetOrgIme(org, name[], size = sizeof (name))
{
    name[0] = EOS;
    if (org <= 0) return false;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT name FROM orgs WHERE id = %d", org);
    new bool:found = DB_GetRowCount(res) > 0;
    if (found) DB_GetFieldString(res, 0, name, size);
    DB_FreeResultSet(res);
    return found;
}

bool:OrgSkinProvjera(org, skin)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM org_skins WHERE org = %d AND skin = %d", org, skin);
    new bool:has = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return has;
}

UcitajClana(playerid)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT org, org_rank, org_perms, org_skin FROM accounts WHERE id = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res))
    {
        gOrg[playerid] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, gOrgRank[playerid], ORG_RANK_LEN);
        gOrgPerms[playerid] = DB_GetFieldInt(res, 2);
        gOrgSkin[playerid] = DB_GetFieldInt(res, 3);
    }
    DB_FreeResultSet(res);
    new name[MAX_ORG_NAME];
    if (gOrg[playerid] && !GetOrgIme(gOrg[playerid], name)) OcistiClana(playerid);
    else if (gOrgSkin[playerid] >= 0 && !OrgSkinProvjera(gOrg[playerid], gOrgSkin[playerid])) gOrgSkin[playerid] = -1;
}

SpremiClana(playerid)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org = %d, org_rank = '%q', org_perms = %d, org_skin = %d WHERE id = %d",
        gOrg[playerid], gOrgRank[playerid], gOrgPerms[playerid], gOrgSkin[playerid], gAccountId[playerid]));
}

OcistiClana(playerid)
{
    gOrg[playerid] = 0;
    gOrgRank[playerid][0] = EOS;
    gOrgPerms[playerid] = 0;
    gOrgSkin[playerid] = -1;
    SpremiClana(playerid);
}

bool:ZaduzenjeProvjera(playerid, perm)
{
    if (gOrg[playerid] && (gOrgPerms[playerid] & (perm | PERM_LEADER))) return true;
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate to zaduzenje u organizaciji."));
    return false;
}

PosaljiOrg(org, const msg[], const a[] = "", const b[] = "", const c[] = "")
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && gUlogiran[i] && gOrg[i] == org) SendClientMessage(i, COLOR_ORG, T(i, msg), a, b, c);
}

PosaljiSvima(color, const msg[], const a[] = "", const b[] = "", const c[] = "", const d[] = "")
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && gUlogiran[i]) SendClientMessage(i, color, T(i, msg), a, b, c, d);
}

UlogiranProvjera(acc)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && gUlogiran[i] && gAccountId[i] == acc) return i;
    return INVALID_PLAYER_ID;
}

bool:ClanInfo(org, acc, name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], &perms)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT name, org_rank, org_perms FROM accounts WHERE id = %d AND org = %d", acc, org);
    new bool:found = org > 0 && DB_GetRowCount(res) > 0;
    if (found)
    {
        DB_GetFieldString(res, 0, name, sizeof name);
        DB_GetFieldString(res, 1, rank, sizeof rank);
        perms = DB_GetFieldInt(res, 2);
        for (new i = 0; name[i]; i++) if (name[i] == '_') name[i] = ' ';
    }
    DB_FreeResultSet(res);
    return found;
}

IzbaciClana(playerid, acc)
{
    new name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], perms;
    if (!ZaduzenjeProvjera(playerid, PERM_INVITE)) return 0;
    if (acc == gAccountId[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Za izlazak koristite /org."));
    if (!ClanInfo(gOrg[playerid], acc, name, rank, perms)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije u vasoj organizaciji."));
    if (perms & PERM_LEADER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Lidera moze maknuti samo admin."));
    PosaljiOrg(gOrg[playerid], "%s je izbacio %s iz organizacije.", GetRPIme(playerid), name);
    new t = UlogiranProvjera(acc);
    if (t != INVALID_PLAYER_ID) { OcistiClana(t); SetPlayerSkin(t, DEFAULT_SKIN); }
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org = 0, org_rank = '', org_perms = 0, org_skin = -1 WHERE id = %d", acc));
    return 1;
}

PostaviRank(playerid, acc, const input[])
{
    new name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], old[ORG_RANK_LEN], perms;
    if (!gOrg[playerid] || !(gOrgPerms[playerid] & PERM_LEADER)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo lider postavlja rankove."));
    if (!ClanInfo(gOrg[playerid], acc, name, old, perms)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije u vasoj organizaciji."));
    RestParam(input, 0, rank);
    RemoveBoje(rank);
    if (!rank[0]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Naziv ranka ne smije biti prazan."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org_rank = '%q' WHERE id = %d", rank, acc));
    new t = UlogiranProvjera(acc);
    if (t != INVALID_PLAYER_ID)
    {
        strcopy(gOrgRank[t], rank, ORG_RANK_LEN);
        if (t != playerid) SendClientMessage(t, COLOR_ORG, T(t, "Vas novi rank: %s"), rank);
    }
    return SendClientMessage(playerid, COLOR_ORG, T(playerid, "%s je sada: %s"), name, rank);
}

PrikaziZaduzenja(playerid)
{
    new name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], perms, list[256], row[48], title[64], len = 0;
    if (!ClanInfo(gOrg[playerid], gOrgEditAcc[playerid], name, rank, perms)) return 0;
    for (new i = 0; i < PERM_COUNT; i++)
    {
        format(row, sizeof row, T(playerid, "%s %s"), (perms & (1 << i)) ? ("[X]") : ("[  ]"), T(playerid, gPermName[i]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, T(playerid, "Zaduzenja: %s"), name);
    return PrikaziDialog(playerid, DLG_ORG_ZADUZENJA, DIALOG_STYLE_LIST, title, list, T(playerid, "Promijeni"), T(playerid, "Gotovo"));
}

ShowOrgPanel(playerid)
{
    static list[4096];
    new orgname[MAX_ORG_NAME], name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], row[96], title[96];
    if (!GetOrgIme(gOrg[playerid], orgname)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Niste clan nijedne organizacije."));
    list = "Clan\tRank\tStatus\n{8AB4F8}Odaberi skin\t\t\n{8AB4F8}Pozovi igraca\t\t\n{8AB4F8}Teritoriji organizacije\t\t\n{8AB4F8}Napusti organizaciju\t\t\n";
    new len = strlen(list);

    new online[MAX_PLAYERS], count = 0;
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (gOrg[i] == gOrg[playerid] && gUlogiran[i]) online[count++] = gAccountId[i];

    new DBResult:res = DB_ExecuteQuery(gDB, ORG_MEMBERS_SQL, gOrg[playerid]);
    if (DB_GetRowCount(res)) do
    {
        new acc = DB_GetFieldInt(res, 0), bool:on = false;
        for (new i = 0; i < count; i++) if (online[i] == acc) { on = true; break; }
        DB_GetFieldString(res, 1, name);
        DB_GetFieldString(res, 2, rank);
        for (new i = 0; name[i]; i++) if (name[i] == '_') name[i] = ' ';
        format(row, sizeof row, T(playerid, "%s%s\t%s\t%s"), (DB_GetFieldInt(res, 3) & PERM_LEADER) ? ("{FFD700}") : (""), name, rank,
            on ? T(playerid, "{33CC33}online") : T(playerid, "{777777}offline"));
    } while (DodajStupac(list, len, row) && DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    format(title, sizeof title, T(playerid, "%s - %s"), orgname, gOrgRank[playerid]);
    SlucajNaslov(playerid, title, sizeof title);
    return PrikaziDialog(playerid, DLG_ORG_PANEL, DIALOG_STYLE_TABLIST_HEADERS, title, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

PrikaziClana(playerid)
{
    new name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], perms, title[96];
    if (!ClanInfo(gOrg[playerid], gOrgEditAcc[playerid], name, rank, perms)) return ShowOrgPanel(playerid);
    format(title, sizeof title, T(playerid, "%s - %s"), name, rank);
    return PrikaziDialog(playerid, DLG_ORG_CLAN, DIALOG_STYLE_LIST, title, T(playerid, "Promijeni rank\nZaduzenja\nIzbaci iz organizacije"), T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:org_poziv(playerid, response)
{
    new org = gOrgInvite[playerid], name[MAX_ORG_NAME];
    gOrgInvite[playerid] = 0;
    if (!response || gOrg[playerid] || !GetOrgIme(org, name)) return 1;
    gOrg[playerid] = org;
    strcopy(gOrgRank[playerid], "Clan", ORG_RANK_LEN);
    gOrgPerms[playerid] = 0;
    gOrgSkin[playerid] = -1;
    SpremiClana(playerid);
    PosaljiOrg(org, "%s se pridruzio organizaciji %s.", GetRPIme(playerid), name);
    return 1;
}

DLG:org_zaduzenja(playerid, response, listitem)
{
    new acc = gOrgEditAcc[playerid], name[MAX_PLAYER_NAME + 1], rank[ORG_RANK_LEN], perms;
    if (!response || listitem < 0 || listitem >= PERM_COUNT) return PrikaziClana(playerid);
    if (!(gOrgPerms[playerid] & PERM_LEADER)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo lider dodjeljuje zaduzenja."));
    if (!ClanInfo(gOrg[playerid], acc, name, rank, perms)) return 1;
    if (perms & PERM_LEADER)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Lider ima sva zaduzenja."));
        return PrikaziClana(playerid);
    }
    perms ^= 1 << listitem;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org_perms = %d WHERE id = %d", perms, acc));
    new t = UlogiranProvjera(acc);
    if (t != INVALID_PLAYER_ID)
    {
        gOrgPerms[t] = perms;
        SendClientMessage(t, COLOR_ORG, T(t, "Zaduzenje '%s' vam je %s."), T(t, gPermName[listitem]), (perms & (1 << listitem)) ? T(t, "dodijeljeno") : T(t, "oduzeto"));
    }
    return PrikaziZaduzenja(playerid);
}

DLG:org_panel(playerid, response, listitem)
{
    if (!response) return 1;
    switch (listitem)
    {
        case 0: return OrgSkinovi(playerid);
        case 1: return PrikaziDialog(playerid, DLG_ORG_POZIV_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Pozovi igraca"), T(playerid, "Unesite id/ime igraca:"), T(playerid, "Pozovi"), T(playerid, "Natrag"));
        case 2: return ShowTerOrg(playerid);
        case 3: return PrikaziDialog(playerid, DLG_ORG_NAPUSTI, DIALOG_STYLE_MSGBOX, T(playerid, "Napusti organizaciju"), T(playerid, "Sigurno zelite napustiti organizaciju?"), T(playerid, "Napusti"), T(playerid, "Natrag"));
    }
    new DBResult:res = DB_ExecuteQuery(gDB, ORG_MEMBER_AT_SQL, gOrg[playerid], listitem - ORG_PANEL_FIXED);
    gOrgEditAcc[playerid] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return PrikaziClana(playerid);
}

DLG:org_clan(playerid, response, listitem)
{
    if (!response) return ShowOrgPanel(playerid);
    switch (listitem)
    {
        case 0: return PrikaziDialog(playerid, DLG_ORG_RANK_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Promijeni rank"), T(playerid, "Unesite novi naziv ranka:"), T(playerid, "Postavi"), T(playerid, "Natrag"));
        case 1:
        {
            if (!(gOrgPerms[playerid] & PERM_LEADER))
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo lider dodjeljuje zaduzenja."));
                return PrikaziClana(playerid);
            }
            return PrikaziZaduzenja(playerid) || ShowOrgPanel(playerid);
        }
        case 2: return PrikaziDialog(playerid, DLG_ORG_IZBACI, DIALOG_STYLE_MSGBOX, T(playerid, "Izbaci clana"), T(playerid, "Sigurno zelite izbaciti ovog clana?"), T(playerid, "Izbaci"), T(playerid, "Natrag"));
    }
    return 1;
}

DLG:org_poziv_unos(playerid, response, const inputtext[])
{
    if (response && inputtext[0]) OrgPozovi(playerid, inputtext);
    return ShowOrgPanel(playerid);
}

DLG:org_rank_unos(playerid, response, const inputtext[])
{
    if (response && inputtext[0]) PostaviRank(playerid, gOrgEditAcc[playerid], inputtext);
    return PrikaziClana(playerid);
}

DLG:org_izbaci(playerid, response)
{
    if (!response) return PrikaziClana(playerid);
    IzbaciClana(playerid, gOrgEditAcc[playerid]);
    return ShowOrgPanel(playerid);
}

DLG:org_napusti(playerid, response)
{
    if (!response) return ShowOrgPanel(playerid);
    OrgNapusti(playerid);
    if (gOrg[playerid]) ShowOrgPanel(playerid);
    return 1;
}

DLG:org_skin(playerid, response, listitem)
{
    if (!response) return 1;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT skin FROM org_skins WHERE org = %d ORDER BY skin LIMIT 1 OFFSET %d", gOrg[playerid], listitem);
    if (DB_GetRowCount(res))
    {
        gOrgSkin[playerid] = DB_GetFieldInt(res, 0);
        SetPlayerSkin(playerid, gOrgSkin[playerid]);
        SpremiClana(playerid);
    }
    DB_FreeResultSet(res);
    return 1;
}

CMD:org(playerid, params[])
{
    return ShowOrgPanel(playerid);
}

OrgPozovi(playerid, const params[])
{
    new idx = 0, p1[32], name[MAX_ORG_NAME], body[128];
    if (!ZaduzenjeProvjera(playerid, PERM_INVITE)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime]"));
    new target = FindPlayer(p1);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (gOrg[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac je vec u organizaciji."));
    GetOrgIme(gOrg[playerid], name);
    gOrgInvite[target] = gOrg[playerid];
    format(body, sizeof body, T(target, "%s vas poziva u organizaciju %s."), GetRPIme(playerid), name);
    PrikaziDialog(target, DLG_ORG_POZIV, DIALOG_STYLE_MSGBOX, T(target, "Poziv u organizaciju"), body, T(target, "Prihvati"), T(target, "Odbij"));
    SendClientMessage(playerid, COLOR_ORG, T(playerid, "Poziv poslan igracu %s."), GetRPIme(target));
    return 1;
}

OrgNapusti(playerid)
{
    if (!gOrg[playerid]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Niste clan nijedne organizacije."));
    if (gOrgPerms[playerid] & PERM_LEADER) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Lider ne moze napustiti organizaciju. Javite se adminu."));
    PosaljiOrg(gOrg[playerid], "%s je napustio organizaciju.", GetRPIme(playerid));
    OcistiClana(playerid);
    SetPlayerSkin(playerid, DEFAULT_SKIN);
    return 1;
}

OrgSkinovi(playerid)
{
    if (!gOrg[playerid]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Skin mogu birati samo clanovi organizacija."));
    new list[1024], row[16], len = 0, DBResult:res = DB_ExecuteQuery(gDB, "SELECT skin FROM org_skins WHERE org = %d ORDER BY skin", gOrg[playerid]);
    if (DB_GetRowCount(res)) do format(row, sizeof row, T(playerid, "Skin %d"), DB_GetFieldInt(res, 0)); while (DodajStupac(list, len, row) && DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    if (!list[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Admini jos nisu dodali skinove vasoj organizaciji."));
    return PrikaziDialog(playerid, DLG_ORG_SKIN, DIALOG_STYLE_LIST, T(playerid, "Odaberite skin"), list, T(playerid, "Odaberi"), T(playerid, "Odustani"));
}

KreirajOrg(playerid, const params[])
{
    new idx = 0, p1[8], name[MAX_ORG_NAME];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || strval(p1) > 5)
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [tip: 0 banda, 1 mafija, 2 legalna] [ime]"));
    RestParam(params, idx, name);
    RemoveBoje(name);
    if (!name[0]) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [tip] [ime]"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM orgs WHERE name = '%q'", name);
    new exists = DB_GetRowCount(res);
    DB_FreeResultSet(res);
    if (exists) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija s tim imenom vec postoji."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO orgs (name, type, created) VALUES ('%q', %d, %d)", name, strval(p1), gettime()));
    res = DB_ExecuteQuery(gDB, "SELECT id FROM orgs WHERE name = '%q'", name);
    new id = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    gOrgNova = id;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Organizacija '%s' (%s) stvorena, ID %d. Lidera postavite u admin panelu."), name, T(playerid, gOrgTypeName[strval(p1)]), id);
    return 1;
}

ObrisiOrg(playerid, const params[])
{
    new idx = 0, p1[8], name[MAX_ORG_NAME];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [org ID]"));
    new org = strval(p1);
    if (!GetOrgIme(org, name)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija ne postoji."));
    for (new v = 1; v < MAX_VEHICLES; v++)
        if (gOrgVehOrg[v] == org) { DestroyVehicle(v); gOrgVehOrg[v] = 0; }
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && gOrg[i] == org) { OrgConnect(i); SetPlayerSkin(i, DEFAULT_SKIN); SendClientMessage(i, COLOR_ORG, T(i, "Vasa organizacija je raspustena.")); }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org = 0, org_rank = '', org_perms = 0, org_skin = -1 WHERE org = %d", org));
    TeritorijOrgObrisana(org);
    ObrisiHq(org);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_skins WHERE org = %d", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_trunks WHERE veh IN (SELECT id FROM org_vehicles WHERE org = %d)", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_vehicles WHERE org = %d", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM orgs WHERE id = %d", org));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Organizacija '%s' obrisana."), name);
    return 1;
}

OrgPostaviLidera(playerid, const params[])
{
    new idx = 0, p1[8], p2[32], name[MAX_ORG_NAME];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || !SljedeciParam(params, idx, p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [org ID] [id/ime]"));
    new org = strval(p1);
    if (!GetOrgIme(org, name)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija ne postoji."));
    new target = FindPlayer(p2);
    if (target == INVALID_PLAYER_ID || !gUlogiran[target]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Igrac nije pronaden."));
    if (gOrg[target] != org) gOrgSkin[target] = -1;
    gOrg[target] = org;
    gOrgPerms[target] = PERM_LEADER | ((1 << PERM_COUNT) - 1);
    strcopy(gOrgRank[target], "Lider", ORG_RANK_LEN);
    SpremiClana(target);
    SendClientMessage(target, COLOR_ORG, T(target, "Postavljeni ste za lidera organizacije %s. Pogledajte /org."), name);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "%s je lider organizacije %s."), GetRPIme(target), name);
    return 1;
}

OrgMakni(playerid, const params[])
{
    new idx = 0, p1[32];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1)) return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [id/ime ili tocno ime offline igraca]"));
    new target = FindPlayer(p1);
    if (target != INVALID_PLAYER_ID && gUlogiran[target])
    {
        OcistiClana(target);
        SetPlayerSkin(target, DEFAULT_SKIN);
        SendClientMessage(target, COLOR_ORG, T(target, "Admin vas je maknuo iz organizacije."));
    }
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org = 0, org_rank = '', org_perms = 0, org_skin = -1 WHERE name = '%q' AND org != 0", p1));
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT changes()");
        new changed = DB_GetFieldInt(res, 0);
        DB_FreeResultSet(res);
        if (!changed) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nema takvog igraca u organizaciji (offline igrac: tocno ime)."));
    }
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Igrac maknut iz organizacije."));
    return 1;
}

OrgSkin(playerid, const params[])
{
    new idx = 0, p1[8], p2[8], p3[8], name[MAX_ORG_NAME];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || !SljedeciParam(params, idx, p2) || !SljedeciParam(params, idx, p3) || !ProvjeriBroj(p3))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [org ID] [dodaj/ukloni] [skin 0-311]"));
    new org = strval(p1), skin = strval(p3);
    if (!GetOrgIme(org, name)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija ne postoji."));
    if (skin > 311) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Skin mora biti izmedu 0 i 311."));
    if (!strcmp(p2, "dodaj", true))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO org_skins (org, skin) VALUES (%d, %d)", org, skin));
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Skin %d dodan organizaciji %s."), skin, name);
    }
    else if (!strcmp(p2, "ukloni", true))
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_skins WHERE org = %d AND skin = %d", org, skin));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE accounts SET org_skin = -1 WHERE org = %d AND org_skin = %d", org, skin));
        for (new i = 0; i < MAX_PLAYERS; i++) if (IsPlayerConnected(i) && gOrg[i] == org && gOrgSkin[i] == skin) gOrgSkin[i] = -1;
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Skin %d uklonjen iz organizacije %s."), skin, name);
    }
    else return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [org ID] [dodaj/ukloni] [skin 0-311]"));
    return 1;
}

OrgVozilo(playerid, const params[])
{
    new idx = 0, p1[8], p2[8], p3[8], p4[8], name[MAX_ORG_NAME];
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!SljedeciParam(params, idx, p1) || !ProvjeriBroj(p1) || !SljedeciParam(params, idx, p2) || !ProvjeriBroj(p2))
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Unesite: [org ID] [model 400-611] [boja1] [boja2] (parkira na vasem mjestu)"));
    new org = strval(p1), model = strval(p2);
    if (!GetOrgIme(org, name)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Organizacija ne postoji."));
    if (model < 400 || model > 611) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Model mora biti izmedu 400 i 611."));
    new c1 = SljedeciParam(params, idx, p3) ? strval(p3) : 0, c2 = SljedeciParam(params, idx, p4) ? strval(p4) : 0;
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    z += 1.0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO org_vehicles (org, model, x, y, z, a, c1, c2) VALUES (%d, %d, %f, %f, %f, %f, %d, %d)", org, model, x, y, z, a, c1, c2));
    new dbid = ZadnjiId();
    new veh = KreirajOrgVozilo(dbid, org, model, x, y, z, a, c1, c2);
    if (veh == INVALID_VEHICLE_ID)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_trunks WHERE veh = %d", dbid));
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_vehicles WHERE id = %d", dbid));
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se ne moze stvoriti."));
    }
    PutPlayerInVehicle(playerid, veh, 0);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo %d parkirano za %s. Brise se u admin panelu (Organizacije)."), model, name);
    return 1;
}

OrgUkloniVozilo(playerid)
{
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new veh = GetPlayerVehicleID(playerid);
    if (!veh || !gOrgVehOrg[veh]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste u vozilu organizacije."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM vehicle_trunks WHERE veh = %d", gOrgVehDb[veh]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_vehicles WHERE id = %d", gOrgVehDb[veh]));
    gOrgVehOrg[veh] = 0;
    DestroyVehicle(veh);
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo organizacije obrisano."));
    return 1;
}

#define MAX_HQ              32
#define HQ_SVIJET           1000
#define HQ_DOMET            2.0
#define HQ_PICKUP           1239

#define ORG_LSPD            3
#define ORG_MEDICINSKA      4
#define ORG_VATROGASCI      5
#define ORG_TAKSI           6
#define ORG_MEHANICAR       7

enum E_HQ { hOrg, hPickup, hPickupU, Text3D:hLabel, Text3D:hLabelU, hInt, Float:hX, Float:hY, Float:hZ, Float:hA, Float:hIx, Float:hIy, Float:hIz }
static gHq[MAX_HQ][E_HQ];
static gHqOrg[MAX_PLAYERS];

enum E_HQI { hiIme[24], hiTip, hiInt, Float:hiX, Float:hiY, Float:hiZ }
static const gHqInt[][E_HQI] = {
    { "Ryder",        0, 2,  2454.717, -1700.871, 1013.515 },
    { "Sweet",        0, 1,  2527.654, -1679.388, 1015.498 },
    { "CJ",           0, 3,  2496.049, -1695.238, 1014.742 },
    { "Crack den",    0, 5,  318.564,  1118.209,  1083.882 },
    { "Big Smoke",    0, 2,  2543.462, -1308.379, 1026.728 },
    { "House 8",      0, 8,  2365.300, -1135.600, 1050.880 },
    { "House 11",     0, 11, 2283.040, -1140.280, 1050.900 },
    { "House 12",     0, 12, 2324.530, -1149.540, 1050.710 },
    { "Madd Dogg",    1, 5,  1267.663, -781.323,  1091.906 },
    { "House 7",      1, 7,  225.680,  1021.450,  1084.020 },
    { "House 5",      1, 5,  140.170,  1366.070,  1083.650 },
    { "House 6",      1, 6,  234.190,  1063.730,  1084.210 },
    { "House 9",      1, 9,  83.030,   1322.280,  1083.870 },
    { "House 10",     1, 10, 24.040,   1340.170,  1084.370 },
    { "LSPD",         2, 6,  246.783,  63.900,    1003.640 },
    { "LVPD",         2, 3,  288.745,  169.350,   1007.171 },
    { "SFPD",         2, 10, 246.375,  109.245,   1003.218 },
    { "City Planning", 2, 3, 384.808,  173.804,   1008.382 }
};

static KreirajHq(i)
{
    new ime[MAX_ORG_NAME], natpis[MAX_ORG_NAME + 16], svijet = HQ_SVIJET + gHq[i][hOrg];
    GetOrgIme(gHq[i][hOrg], ime);
    format(natpis, sizeof natpis, "%s HQ\n[Y]", ime);
    gHq[i][hPickup] = CreatePickup(HQ_PICKUP, 1, gHq[i][hX], gHq[i][hY], gHq[i][hZ], 0);
    gHq[i][hLabel] = Create3DTextLabel(natpis, COLOR_ORG, gHq[i][hX], gHq[i][hY], gHq[i][hZ] + 0.5, 15.0, 0);
    gHq[i][hPickupU] = CreatePickup(HQ_PICKUP, 1, gHq[i][hIx], gHq[i][hIy], gHq[i][hIz], svijet);
    gHq[i][hLabelU] = Create3DTextLabel("[Y]", COLOR_ORG, gHq[i][hIx], gHq[i][hIy], gHq[i][hIz] + 0.5, 15.0, svijet);
}

static UkloniHq(i)
{
    DestroyPickup(gHq[i][hPickup]);
    DestroyPickup(gHq[i][hPickupU]);
    Delete3DTextLabel(gHq[i][hLabel]);
    Delete3DTextLabel(gHq[i][hLabelU]);
    gHq[i][hOrg] = 0;
}

InitHq()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_hq (org INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL, interior INTEGER NOT NULL, ix REAL NOT NULL, iy REAL NOT NULL, iz REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT org, x, y, z, a, interior, ix, iy, iz FROM org_hq LIMIT %d", MAX_HQ), i = 0;
    for (new n = DB_GetRowCount(res); n > 0; n--)
    {
        gHq[i][hOrg] = DB_GetFieldInt(res, 0);
        gHq[i][hX] = DB_GetFieldFloat(res, 1);
        gHq[i][hY] = DB_GetFieldFloat(res, 2);
        gHq[i][hZ] = DB_GetFieldFloat(res, 3);
        gHq[i][hA] = DB_GetFieldFloat(res, 4);
        gHq[i][hInt] = DB_GetFieldInt(res, 5);
        gHq[i][hIx] = DB_GetFieldFloat(res, 6);
        gHq[i][hIy] = DB_GetFieldFloat(res, 7);
        gHq[i][hIz] = DB_GetFieldFloat(res, 8);
        KreirajHq(i++);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

ObrisiHq(org)
{
    for (new i = 0; i < MAX_HQ; i++) if (gHq[i][hOrg] == org) UkloniHq(i);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_hq WHERE org = %d", org));
}

static HqPostavi(playerid, org, interior, Float:ix, Float:iy, Float:iz)
{
    new Float:x, Float:y, Float:z, Float:a, i = 0;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "HQ se postavlja samo izvana."));
    while (i < MAX_HQ && gHq[i][hOrg] != org) i++;
    if (i < MAX_HQ) UkloniHq(i);
    else
    {
        i = 0;
        while (i < MAX_HQ && gHq[i][hOrg]) i++;
        if (i == MAX_HQ) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Dostignut je limit HQ-ova."));
    }
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    gHq[i][hOrg] = org;
    gHq[i][hX] = x;
    gHq[i][hY] = y;
    gHq[i][hZ] = z;
    gHq[i][hA] = a;
    gHq[i][hInt] = interior;
    gHq[i][hIx] = ix;
    gHq[i][hIy] = iy;
    gHq[i][hIz] = iz;
    KreirajHq(i);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO org_hq (org, x, y, z, a, interior, ix, iy, iz) VALUES (%d, %f, %f, %f, %f, %d, %f, %f, %f)", org, x, y, z, a, interior, ix, iy, iz));
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "HQ je postavljen."));
}

static HqTip(org)
{
    new tip = GetOrgTip(org);
    return tip > 2 ? 2 : tip;
}

ShowHqIzbor(playerid, org)
{
    new list[512], len = 0, tip = HqTip(org);
    gHqOrg[playerid] = org;
    for (new i = 0; i < sizeof gHqInt; i++) if (gHqInt[i][hiTip] == tip) DodajStupac(list, len, gHqInt[i][hiIme]);
    DodajStupac(list, len, T(playerid, "{8AB4F8}Vlastiti ID i lokacija"));
    return PrikaziDialog(playerid, DLG_APANEL_HQ, DIALOG_STYLE_LIST, T(playerid, "Odaberite interijer za HQ"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_hq(playerid, response, listitem)
{
    if (!response) return ShowPopisOrg(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new n = listitem, tip = HqTip(gHqOrg[playerid]);
    for (new i = 0; i < sizeof gHqInt; i++)
    {
        if (gHqInt[i][hiTip] != tip || n--) continue;
        HqPostavi(playerid, gHqOrg[playerid], gHqInt[i][hiInt], gHqInt[i][hiX], gHqInt[i][hiY], gHqInt[i][hiZ]);
        return ShowPopisOrg(playerid);
    }
    return PrikaziDialog(playerid, DLG_APANEL_HQ_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Odaberite interijer za HQ"), T(playerid, "Unesite: [interior] [x] [y] [z]"), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

DLG:apanel_hq_unos(playerid, response, const inputtext[])
{
    new interior, Float:x, Float:y, Float:z;
    if (!response) return ShowHqIzbor(playerid, gHqOrg[playerid]);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (!DialogInterijer(inputtext, interior, x, y, z))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Unesite: [interior] [x] [y] [z]"));
        return ShowHqIzbor(playerid, gHqOrg[playerid]);
    }
    HqPostavi(playerid, gHqOrg[playerid], interior, x, y, z);
    return ShowPopisOrg(playerid);
}

bool:HqUlazIzlaz(playerid)
{
    if (IsRanjen(playerid)) return true;
    new svijet = GetPlayerVirtualWorld(playerid);
    for (new i = 0; i < MAX_HQ; i++)
    {
        new org = gHq[i][hOrg];
        if (!org) continue;
        if (svijet == HQ_SVIJET + org && IsPlayerInRangeOfPoint(playerid, HQ_DOMET, gHq[i][hIx], gHq[i][hIy], gHq[i][hIz]))
        {
            SetPlayerInterior(playerid, 0);
            SetPlayerVirtualWorld(playerid, 0);
            SetPlayerPos(playerid, gHq[i][hX], gHq[i][hY], gHq[i][hZ]);
            SetPlayerFacingAngle(playerid, gHq[i][hA] + 180.0);
            SetCameraBehindPlayer(playerid);
            return true;
        }
        if (svijet || GetPlayerInterior(playerid) || !IsPlayerInRangeOfPoint(playerid, HQ_DOMET, gHq[i][hX], gHq[i][hY], gHq[i][hZ])) continue;
        if (gOrg[playerid] != org && !gAdmin[playerid])
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate pristup ovom HQ-u."));
            return true;
        }
        if (GetWanted(playerid))
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Trazeni ste i ne mozete uci u HQ."));
            return true;
        }
        SetPlayerInterior(playerid, gHq[i][hInt]);
        SetPlayerVirtualWorld(playerid, HQ_SVIJET + org);
        SetPlayerPos(playerid, gHq[i][hIx], gHq[i][hIy], gHq[i][hIz]);
        SetCameraBehindPlayer(playerid);
        return true;
    }
    if (TrgovinaUlaz(playerid)) return true;
    if (OruzarnicaUlaz(playerid)) return true;
    if (NamjestajUlaz(playerid)) return true;
    if (OdjecaUlaz(playerid)) return true;
    if (OpstinaUlaz(playerid)) return true;
    return bool:PrikolicaUlaz(playerid);
}

CMD:ulaz(playerid, params[])
{
    if (!gUlogiran[playerid]) return 1;
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !HqUlazIzlaz(playerid)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod ulaza."));
    return 1;
}

CMD:enter(playerid, params[]) return cmd_ulaz(playerid, params);

GetOrgTip(org)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT type FROM orgs WHERE id = %d", org);
    new tip = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : -1;
    DB_FreeResultSet(res);
    return tip;
}
