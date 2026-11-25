#define SEF_NOVAC_MAX       100000000
#define SEF_PREDMET_MAX     100000
#define SEF_NOVAC           (-1)

static gSefRed[MAX_PLAYERS][BROJ_ITEMA], gSefBroj[MAX_PLAYERS], gSefPredmet[MAX_PLAYERS], bool:gSefStavlja[MAX_PLAYERS];

InitSef()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_safes (org INTEGER PRIMARY KEY, money INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_safe_items (org INTEGER NOT NULL, item INTEGER NOT NULL, amount INTEGER NOT NULL, PRIMARY KEY (org, item))"));
}

SefOrgObrisana(org)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_safes WHERE org = %d", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_safe_items WHERE org = %d", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_log WHERE org = %d", org));
}

static SefKolicina(org, item)
{
    new DBResult:res = item == SEF_NOVAC ? DB_ExecuteQuery(gDB, "SELECT money FROM org_safes WHERE org = %d", org) : DB_ExecuteQuery(gDB, "SELECT amount FROM org_safe_items WHERE org = %d AND item = %d", org, item);
    new n = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return n;
}

bool:SefDodaj(org, item, n)
{
    new total = SefKolicina(org, item) + n;
    if (total < 0 || total > (item == SEF_NOVAC ? SEF_NOVAC_MAX : SEF_PREDMET_MAX)) return false;
    if (item == SEF_NOVAC) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO org_safes (org, money) VALUES (%d, %d)", org, total));
    else if (total) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO org_safe_items (org, item, amount) VALUES (%d, %d, %d)", org, item, total));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_safe_items WHERE org = %d AND item = %d", org, item));
    return true;
}

bool:SefNovacDodaj(org, n)
{
    return SefDodaj(org, SEF_NOVAC, n);
}

static bool:SefPristup(playerid)
{
    if (!gOrg[playerid] || GetPlayerVirtualWorld(playerid) != HQ_SVIJET + gOrg[playerid]) return false;
    new tip = GetOrgTip(gOrg[playerid]);
    return tip == 0 || tip == 1;
}

static SefMeni(playerid)
{
    new org = gOrg[playerid], list[2048], row[96], len;
    format(list, sizeof list, "%s\t%s\n", T(playerid, "Predmet"), T(playerid, "Kolicina"));
    len = strlen(list);
    format(row, sizeof row, T(playerid, "{2ECC71}Novac\t$%d"), SefKolicina(org, SEF_NOVAC));
    DodajStupac(list, len, row);
    format(row, sizeof row, "%s\t", T(playerid, "{8AB4F8}Ostavi novac"));
    DodajStupac(list, len, row);
    format(row, sizeof row, "%s\t", T(playerid, "{8AB4F8}Ostavi predmet"));
    DodajStupac(list, len, row);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT item, amount FROM org_safe_items WHERE org = %d ORDER BY item", org), count = 0;
    for (new i = 0, n = DB_GetRowCount(res); i < n && count < BROJ_ITEMA; i++, DB_SelectNextRow(res))
    {
        new item = DB_GetFieldInt(res, 0);
        if (item < 0 || item >= BROJ_ITEMA) continue;
        format(row, sizeof row, "%s\t%d %s", T(playerid, gItems[item][ITEM_NAME]), DB_GetFieldInt(res, 1), T(playerid, gItems[item][ITEM_UNIT]));
        if (DodajStupac(list, len, row)) gSefRed[playerid][count++] = item;
    }
    DB_FreeResultSet(res);
    gSefBroj[playerid] = count;
    return PrikaziDialog(playerid, DLG_SEF, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Sef organizacije"), list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

static SefUnos(playerid, item, bool:stavlja)
{
    gSefPredmet[playerid] = item;
    gSefStavlja[playerid] = stavlja;
    new naslov[32];
    if (item == SEF_NOVAC) strcat(naslov, T(playerid, "Novac"));
    else strcat(naslov, T(playerid, gItems[item][ITEM_NAME]));
    return PrikaziDialog(playerid, DLG_SEF_KOL, DIALOG_STYLE_INPUT, naslov, T(playerid, stavlja ? ("Koliko ostavljate?") : ("Koliko uzimate?")), T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

CMD:sef(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (!SefPristup(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Sef organizacije nalazi se u HQ-u bande ili mafije."));
    return SefMeni(playerid);
}

CMD:safe(playerid, params[]) return cmd_sef(playerid, params);

DLG:sef(playerid, response, listitem)
{
    if (!response || !SefPristup(playerid)) return 1;
    if (listitem == 0)
    {
        if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return SefMeni(playerid);
        return SefUnos(playerid, SEF_NOVAC, false);
    }
    if (listitem == 1) return SefUnos(playerid, SEF_NOVAC, true);
    if (listitem == 2)
    {
        new list[1024], row[64], len = 0, count = 0;
        for (new i = 0; i < BROJ_ITEMA; i++)
        {
            if (!gInv[playerid][i] || i == ITEM_KARTICA || i == ITEM_MOBITEL) continue;
            format(row, sizeof row, "%s\t%d %s", T(playerid, gItems[i][ITEM_NAME]), gInv[playerid][i], T(playerid, gItems[i][ITEM_UNIT]));
            if (DodajStupac(list, len, row)) gSefRed[playerid][count++] = i;
        }
        gSefBroj[playerid] = count;
        if (!count)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate predmeta koje mozete ostaviti."));
            return SefMeni(playerid);
        }
        return PrikaziDialog(playerid, DLG_SEF_STAVI, DIALOG_STYLE_TABLIST, T(playerid, "Ostavi predmet"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
    }
    new i = listitem - 3;
    if (i < 0 || i >= gSefBroj[playerid]) return SefMeni(playerid);
    if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return SefMeni(playerid);
    return SefUnos(playerid, gSefRed[playerid][i], false);
}

DLG:sef_stavi(playerid, response, listitem)
{
    if (!SefPristup(playerid)) return 1;
    if (!response || listitem < 0 || listitem >= gSefBroj[playerid]) return SefMeni(playerid);
    return SefUnos(playerid, gSefRed[playerid][listitem], true);
}

DLG:sef_kol(playerid, response, const inputtext[])
{
    if (!SefPristup(playerid)) return 1;
    if (!response) return SefMeni(playerid);
    new org = gOrg[playerid], item = gSefPredmet[playerid], n = strval(inputtext);
    if (!ProvjeriBroj(inputtext) || strlen(inputtext) > 9 || n <= 0)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Neispravna kolicina."));
        return SefMeni(playerid);
    }
    if (!gSefStavlja[playerid] && !ZaduzenjeProvjera(playerid, PERM_SAFE)) return SefMeni(playerid);
    if (item == SEF_NOVAC)
    {
        if (gSefStavlja[playerid])
        {
            if (!DajNovac(playerid, -n)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
            else if (!SefDodaj(org, SEF_NOVAC, n))
            {
                DajNovac(playerid, n);
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Sef je pun."));
            }
            else
            {
                OrgDnevnik(org, playerid, DNEVNIK_NOVAC_STAVI, n);
                SendClientMessage(playerid, COLOR_ORG, T(playerid, "Ostavili ste $%d u sef."), n);
            }
        }
        else if (!SefDodaj(org, SEF_NOVAC, -n)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U sefu nema toliko."));
        else if (!DajNovac(playerid, n))
        {
            SefDodaj(org, SEF_NOVAC, n);
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko novca."));
        }
        else
        {
            OrgDnevnik(org, playerid, DNEVNIK_NOVAC_UZMI, n);
            SendClientMessage(playerid, COLOR_ORG, T(playerid, "Uzeli ste $%d iz sefa."), n);
        }
        return SefMeni(playerid);
    }
    if (gSefStavlja[playerid])
    {
        if (gInv[playerid][item] < n) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko."));
        else if (!SefDodaj(org, item, n)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Sef je pun."));
        else
        {
            GiveItem(playerid, item, -n);
            OrgDnevnik(org, playerid, DNEVNIK_STAVI, n, item);
            SendClientMessage(playerid, COLOR_ORG, T(playerid, "Ostavili ste u sef: %d %s %s."), n, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
        }
    }
    else if (!SefDodaj(org, item, -n)) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "U sefu nema toliko."));
    else if (!GiveItem(playerid, item, n))
    {
        SefDodaj(org, item, n);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Ne mozete nositi toliko."));
    }
    else
    {
        OrgDnevnik(org, playerid, DNEVNIK_UZMI, n, item);
        SendClientMessage(playerid, COLOR_ORG, T(playerid, "Uzeli ste iz sefa: %d %s %s."), n, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
    }
    return SefMeni(playerid);
}

OrgDnevnikPrikazi(playerid)
{
    if (!ZaduzenjeProvjera(playerid, PERM_SAFE)) return ShowOrgPanel(playerid);
    static list[4096];
    new row[192], dogadaj[128], vrijeme[16], ime[MAX_PLAYER_NAME + 1], tekst[64], len, count = 0;
    format(list, sizeof list, "%s\t%s\t%s\n", T(playerid, "Vrijeme"), T(playerid, "Ime"), T(playerid, "Dogadaj"));
    len = strlen(list);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT strftime('%%d.%%m. %%H:%%M', vrijeme, 'unixepoch'), ime, tip, iznos, item, tekst FROM org_log WHERE org = %d ORDER BY id DESC LIMIT %d", gOrg[playerid], DNEVNIK_PRIKAZ);
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 0, vrijeme, sizeof vrijeme);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        DB_GetFieldString(res, 5, tekst, sizeof tekst);
        new tip = DB_GetFieldInt(res, 2), iznos = DB_GetFieldInt(res, 3), item = DB_GetFieldInt(res, 4);
        if ((tip == DNEVNIK_STAVI || tip == DNEVNIK_UZMI) && (item < 0 || item >= BROJ_ITEMA)) continue;
        switch (tip)
        {
            case DNEVNIK_NOVAC_STAVI: format(dogadaj, sizeof dogadaj, T(playerid, "Ostavio $%d u sef"), iznos);
            case DNEVNIK_NOVAC_UZMI: format(dogadaj, sizeof dogadaj, T(playerid, "Uzeo $%d iz sefa"), iznos);
            case DNEVNIK_STAVI: format(dogadaj, sizeof dogadaj, T(playerid, "Ostavio u sef: %d %s %s"), iznos, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
            case DNEVNIK_UZMI: format(dogadaj, sizeof dogadaj, T(playerid, "Uzeo iz sefa: %d %s %s"), iznos, T(playerid, gItems[item][ITEM_UNIT]), T(playerid, gItems[item][ITEM_NAME]));
            case DNEVNIK_SANDUK: format(dogadaj, sizeof dogadaj, T(playerid, "Dostavio sanduk oruzja"));
            case DNEVNIK_TER_ZAUZET: format(dogadaj, sizeof dogadaj, T(playerid, "Zauzet teritorij %s"), tekst);
            case DNEVNIK_TER_IZGUBLJEN: format(dogadaj, sizeof dogadaj, T(playerid, "Izgubljen teritorij %s"), tekst);
            case DNEVNIK_TRGOVINA: format(dogadaj, sizeof dogadaj, T(playerid, "Opljackao trgovinu: $%d (%s)"), iznos, tekst);
            case DNEVNIK_BANKA: format(dogadaj, sizeof dogadaj, T(playerid, "Opljackana banka: $%d"), iznos);
            case DNEVNIK_ZLATARA: format(dogadaj, sizeof dogadaj, T(playerid, "Opljackana zlatara (%d clanova)"), iznos);
            case DNEVNIK_ULAZ: format(dogadaj, sizeof dogadaj, T(playerid, "Pridruzio se organizaciji"));
            case DNEVNIK_IZLAZ: format(dogadaj, sizeof dogadaj, T(playerid, "Napustio organizaciju"));
            case DNEVNIK_IZBACEN: format(dogadaj, sizeof dogadaj, T(playerid, "Izbacio clana %s"), tekst);
            default: continue;
        }
        format(row, sizeof row, "%s\t%s\t%s", vrijeme, ime[0] ? ime : ("-"), dogadaj);
        if (DodajStupac(list, len, row)) count++;
    }
    DB_FreeResultSet(res);
    if (!count)
    {
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Dnevnik organizacije je prazan."));
        return ShowOrgPanel(playerid);
    }
    return PrikaziDialog(playerid, DLG_ORG_DNEVNIK, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Dnevnik organizacije"), list, T(playerid, "Natrag"));
}

DLG:org_dnevnik(playerid, response)
{
    #pragma unused response
    return ShowOrgPanel(playerid);
}
