#define RACUNALO_MODEL      2190
#define RACUNALO_CIJENA     2500
#define RACUNALO_DOMET      2.0
#define OGLAS_CIJENA        500
#define OGLAS_LEN           100
#define OGLAS_PAUZA         60
#define OGLAS_TRAJE         86400
#define DIONICA_MAX_KOM     10000
#define DIONICA_PROVIZIJA   1
#define DIONICA_MINUTA      5

enum { PC_BANKA_INFO, PC_BANKA_POSALJI, PC_BANKA_OTPLATI, PC_OGLAS, PC_KUPI, PC_PRODAJ }

static const gDionicaIme[][] = { "Monterey Motors", "LS Nafta", "Vinewood Media", "Pacific Banka", "Grove Tech" };
static const gDionicaPocetna[] = { 120, 80, 60, 150, 40 };
static gDionica[sizeof gDionicaIme], gDionicaStara[sizeof gDionicaIme], gDionicaSat;
static gPcAkcija[MAX_PLAYERS], gPcDionica[MAX_PLAYERS], gOglasZadnji[MAX_PLAYERS];

static DionicaNova(cijena, pocetna, pomak)
{
    new nova = cijena + cijena * pomak / 100 + (pocetna - cijena) / 20;
    return nova < 5 ? 5 : nova > pocetna * 10 ? pocetna * 10 : nova;
}

InitRacunalo()
{
    assert(DionicaNova(100, 100, 0) == 100 && DionicaNova(5, 100, -10) == 8 && DionicaNova(1000, 100, 10) == 1000 && DionicaNova(4, 4, -90) == 5);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ads (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stocks (id INTEGER PRIMARY KEY, price INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_holdings (account INTEGER NOT NULL, stock INTEGER NOT NULL, shares INTEGER NOT NULL, PRIMARY KEY (account, stock))"));
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT price FROM stocks WHERE id = %d", d);
        gDionica[d] = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : gDionicaPocetna[d];
        gDionicaStara[d] = gDionica[d];
        DB_FreeResultSet(res);
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM server_flags WHERE name = 'pc_katalog'");
    new bool:dodano = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    if (dodano) return;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture_catalog (shop, model, price, name) SELECT id, %d, %d, 'Racunalo' FROM furniture_shops WHERE id NOT IN (SELECT shop FROM furniture_catalog WHERE model = %d)", RACUNALO_MODEL, RACUNALO_CIJENA, RACUNALO_MODEL));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO server_flags (name) VALUES ('pc_katalog')"));
}

RacunaloTick()
{
    if (++gDionicaSat < DIONICA_MINUTA * 60) return;
    gDionicaSat = 0;
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        gDionicaStara[d] = gDionica[d];
        gDionica[d] = DionicaNova(gDionica[d], gDionicaPocetna[d], random(13) - 6);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stocks (id, price) VALUES (%d, %d)", d, gDionica[d]));
    }
}

static PcIzbornik(playerid)
{
    new list[128], len = 0;
    DodajStupac(list, len, T(playerid, "Internet bankarstvo"));
    DodajStupac(list, len, T(playerid, "Oglasnik"));
    DodajStupac(list, len, T(playerid, "Dionice"));
    return PrikaziDialog(playerid, DLG_PC, DIALOG_STYLE_LIST, T(playerid, "Racunalo"), list, T(playerid, "Odaberi"), T(playerid, "Ugasi"));
}

static bool:PcBlizu(playerid)
{
    return gUlogiran[playerid] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && NamjestajModelBlizu(playerid, RACUNALO_MODEL, RACUNALO_DOMET) != 0;
}

CMD:computer(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (!PcBlizu(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod racunala."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali racunalo.", GetRPIme(playerid), "", 1);
    return PcIzbornik(playerid);
}

CMD:racunalo(playerid, params[]) return cmd_computer(playerid, params);

static PcBanka(playerid)
{
    new list[128], len = 0;
    DodajStupac(list, len, T(playerid, "Informacije"));
    DodajStupac(list, len, T(playerid, "Posalji novac"));
    DodajStupac(list, len, T(playerid, "Otplati kredit"));
    return PrikaziDialog(playerid, DLG_PC_BANKA, DIALOG_STYLE_LIST, T(playerid, "Internet bankarstvo"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PcOglasi(playerid)
{
    new list[2048], row[160], len = 0, tekst[OGLAS_LEN + 1], ime[MAX_PLAYER_NAME + 1], broj[16];
    format(row, sizeof row, T(playerid, "{8AB4F8}+ Objavi oglas ($%d)"), OGLAS_CIJENA);
    DodajStupac(list, len, row);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT ad.text, replace(a.name, '_', ' '), IFNULL((SELECT number FROM phone_numbers WHERE account = ad.account), '-') FROM ads ad JOIN accounts a ON a.id = ad.account WHERE ad.created > %d ORDER BY ad.id DESC LIMIT 12", gettime() - OGLAS_TRAJE);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 0, tekst, sizeof tekst);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        DB_GetFieldString(res, 2, broj, sizeof broj);
        format(row, sizeof row, "%s {AAAAAA}(%s, %s)", tekst, ime, broj);
        if (!DodajStupac(list, len, row)) break;
    }
    DB_FreeResultSet(res);
    return PrikaziDialog(playerid, DLG_PC_OGLASI, DIALOG_STYLE_LIST, T(playerid, "Oglasnik"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PcKomada(playerid, d)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT shares FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d);
    new n = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return n;
}

static PcDionice(playerid)
{
    new list[512], row[96];
    format(list, sizeof list, "%s\t%s\t%s\t%s\n", T(playerid, "Dionica"), T(playerid, "Cijena"), T(playerid, "Promjena"), T(playerid, "Imate"));
    new len = strlen(list);
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        new razlika = gDionica[d] - gDionicaStara[d];
        format(row, sizeof row, "%s\t$%d\t%s%s%d\t%d", gDionicaIme[d], gDionica[d], razlika > 0 ? ("{4CAF50}") : razlika < 0 ? ("{E53935}") : ("{AAAAAA}"), razlika > 0 ? ("+") : (""), razlika, PcKomada(playerid, d));
        DodajStupac(list, len, row);
    }
    return PrikaziDialog(playerid, DLG_PC_DIONICE, DIALOG_STYLE_TABLIST_HEADERS, T(playerid, "Dionice"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static PcUnos(playerid, akcija, const tekst[])
{
    gPcAkcija[playerid] = akcija;
    return PrikaziDialog(playerid, DLG_PC_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Racunalo"), tekst, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

static PcNatrag(playerid, akcija)
{
    if (akcija <= PC_BANKA_OTPLATI) return PcBanka(playerid);
    if (akcija == PC_OGLAS) return PcOglasi(playerid);
    return PcDionice(playerid);
}

DLG:pc(playerid, response, listitem)
{
    if (!response || !PcBlizu(playerid)) return 1;
    switch (listitem)
    {
        case 0:
        {
            if (!gInv[playerid][ITEM_KARTICA]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovnu karticu."));
            return PcBanka(playerid);
        }
        case 1: return PcOglasi(playerid);
        case 2: return PcDionice(playerid);
    }
    return 1;
}

DLG:pc_banka(playerid, response, listitem)
{
    if (!PcBlizu(playerid)) return 1;
    if (!response) return PcIzbornik(playerid);
    if (listitem == 0)
    {
        new tekst[160];
        if (!BankaTekst(playerid, tekst)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate bankovni racun.")), PcBanka(playerid);
        for (new i = 0; tekst[i]; i++) if (tekst[i] == '~' && tekst[i + 1] == 'n' && tekst[i + 2] == '~')
        {
            tekst[i] = '\n';
            strdel(tekst, i + 1, i + 3);
        }
        gPcAkcija[playerid] = PC_BANKA_INFO;
        return PrikaziDialog(playerid, DLG_PC_INFO, DIALOG_STYLE_MSGBOX, T(playerid, "Informacije"), tekst, T(playerid, "Natrag"));
    }
    if (listitem == 1) return PcUnos(playerid, PC_BANKA_POSALJI, T(playerid, "Unesite: [id/ime primatelja] [iznos]"));
    return PcUnos(playerid, PC_BANKA_OTPLATI, T(playerid, "Unesite iznos:"));
}

DLG:pc_info(playerid)
{
    if (!PcBlizu(playerid)) return 1;
    return PcNatrag(playerid, gPcAkcija[playerid]);
}

DLG:pc_oglasi(playerid, response, listitem)
{
    if (!PcBlizu(playerid)) return 1;
    if (!response) return PcIzbornik(playerid);
    if (listitem != 0) return PcOglasi(playerid);
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Tekst oglasa (najvise %d znakova, cijena $%d s racuna):"), OGLAS_LEN, OGLAS_CIJENA);
    return PcUnos(playerid, PC_OGLAS, tekst);
}

DLG:pc_dionice(playerid, response, listitem)
{
    if (!PcBlizu(playerid)) return 1;
    if (!response) return PcIzbornik(playerid);
    if (listitem < 0 || listitem >= sizeof gDionicaIme) return PcDionice(playerid);
    gPcDionica[playerid] = listitem;
    new tekst[160];
    format(tekst, sizeof tekst, T(playerid, "%s: $%d po dionici, imate %d.~n~Unesite broj dionica (kupnja s racuna, prodaja uz %d%% provizije):"), gDionicaIme[listitem], gDionica[listitem], PcKomada(playerid, listitem), DIONICA_PROVIZIJA);
    for (new i = 0; tekst[i]; i++) if (tekst[i] == '~' && tekst[i + 1] == 'n' && tekst[i + 2] == '~')
    {
        tekst[i] = '\n';
        strdel(tekst, i + 1, i + 3);
    }
    gPcAkcija[playerid] = PC_KUPI;
    return PrikaziDialog(playerid, DLG_PC_DIONICA, DIALOG_STYLE_INPUT, gDionicaIme[listitem], tekst, T(playerid, "Kupi"), T(playerid, "Prodaj"));
}

DLG:pc_dionica(playerid, response, const inputtext[])
{
    if (!PcBlizu(playerid)) return 1;
    new d = gPcDionica[playerid], n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 5 ? strval(inputtext) : 0, ima = PcKomada(playerid, d);
    if (n <= 0) return PcDionice(playerid);
    if (response)
    {
        if (ima + n > DIONICA_MAX_KOM) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise %d dionica po tvrtki."), DIONICA_MAX_KOM), PcDionice(playerid);
        if (!BankaPromijeni(playerid, -n * gDionica[d])) return PcDionice(playerid);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stock_holdings (account, stock, shares) VALUES (%d, %d, %d)", gAccountId[playerid], d, ima + n));
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d dionica (%s) za $%d."), n, gDionicaIme[d], n * gDionica[d]);
        return PcDionice(playerid);
    }
    if (n > ima) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko dionica.")), PcDionice(playerid);
    new iznos = n * gDionica[d];
    iznos -= iznos * DIONICA_PROVIZIJA / 100;
    if (!BankaPromijeni(playerid, iznos)) return PcDionice(playerid);
    if (ima == n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET shares = %d WHERE account = %d AND stock = %d", ima - n, gAccountId[playerid], d));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
    return PcDionice(playerid);
}

DLG:pc_unos(playerid, response, const inputtext[])
{
    if (!PcBlizu(playerid)) return 1;
    new a = gPcAkcija[playerid];
    if (!response || !inputtext[0]) return PcNatrag(playerid, a);
    switch (a)
    {
        case PC_BANKA_POSALJI: BankaPosaljiUnos(playerid, inputtext);
        case PC_BANKA_OTPLATI:
        {
            new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 9 ? strval(inputtext) : 0;
            if (BankaOtplati(playerid, n)) SendClientMessage(playerid, COLOR_INFO, T(playerid, "Transakcija uspjesna."));
        }
        case PC_OGLAS:
        {
            new tekst[OGLAS_LEN + 1], broj[16] = "-", sad = gettime();
            if (sad - gOglasZadnji[playerid] < OGLAS_PAUZA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), OGLAS_PAUZA - (sad - gOglasZadnji[playerid])), PcOglasi(playerid);
            strcat(tekst, inputtext);
            RemoveBoje(tekst);
            if (!tekst[0] || !BankaPromijeni(playerid, -OGLAS_CIJENA)) return PcOglasi(playerid);
            gOglasZadnji[playerid] = sad;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO ads (account, text, created) VALUES (%d, '%q', %d)", gAccountId[playerid], tekst, sad));
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT number FROM phone_numbers WHERE account = %d", gAccountId[playerid]);
            if (DB_GetRowCount(res)) DB_GetFieldString(res, 0, broj, sizeof broj);
            DB_FreeResultSet(res);
            for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i]) SendClientMessage(i, 0x7CB342FF, T(i, "[Oglas] %s (%s, tel. %s)"), tekst, GetRPIme(playerid), broj);
        }
    }
    return PcNatrag(playerid, a);
}
