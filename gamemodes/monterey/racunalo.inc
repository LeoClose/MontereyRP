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
#define PC_TD               48
#define PC_X                120.0
#define PC_Y                80.0

enum { PC_BANKA_POSALJI, PC_BANKA_OTPLATI, PC_OGLAS }
enum { PCE_POCETNA, PCE_BANKA, PCE_OGLASI, PCE_DIONICE }
enum { PTD_START, PTD_TRAKA, PTD_PROZOR, PTD_NASLOV = PTD_PROZOR + 3, PTD_ZATVORI, PTD_SADRZAJ, PTD_START_PANEL, PTD_UGASI = PTD_START_PANEL + 2, PTD_APP, PTD_AKCIJA = PTD_APP + 4, PTD_DIONICA = PTD_AKCIJA + 2, PTD_IMENOVANI = PTD_DIONICA + 5 }

static const gDionicaIme[][] = { "Monterey Motors", "LS Nafta", "Vinewood Media", "Pacific Banka", "Grove Tech" };
static const gDionicaPocetna[] = { 120, 80, 60, 150, 40 };
static gDionica[sizeof gDionicaIme], gDionicaStara[sizeof gDionicaIme], gDionicaSat;
static gPcAkcija[MAX_PLAYERS], gPcDionica[MAX_PLAYERS], gOglasZadnji[MAX_PLAYERS], gPcEkran[MAX_PLAYERS], bool:gPcOtvoren[MAX_PLAYERS], bool:gPcStart[MAX_PLAYERS];
static PlayerText:gPcTd[MAX_PLAYERS][PC_TD], gPcBroj[MAX_PLAYERS], PlayerText:gPc[MAX_PLAYERS][PTD_IMENOVANI];
static const gPcAplikacije[][] = { "Pocetna", "Banka", "Oglasnik", "Dionice" };
static const gPcIkona[][] = { "", "$", "@", "%" };
static const gPcIkonaBoja[] = { 0, 0x2E7D32FF, 0xEF6C00FF, 0x6A1B9AFF };

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

static PcKomada(playerid, d)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT shares FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d);
    new n = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return n;
}

static bool:PcBlizu(playerid)
{
    return gUlogiran[playerid] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && NamjestajModelBlizu(playerid, RACUNALO_MODEL, RACUNALO_DOMET) != 0;
}

static PlayerText:PcKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, PC_X + x, PC_Y + y, "LD_SPAC:white");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_SPRITE_DRAW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawColour(playerid, td, boja);
    gPcTd[playerid][gPcBroj[playerid]++] = td;
    return td;
}

static PlayerText:PcTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font = TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, PC_X + x, PC_Y + y, tekst);
    PlayerTextDrawFont(playerid, td, font);
    PlayerTextDrawLetterSize(playerid, td, sirina, visina);
    PlayerTextDrawColour(playerid, td, boja);
    PlayerTextDrawSetShadow(playerid, td, 0);
    PlayerTextDrawSetOutline(playerid, td, 0);
    PlayerTextDrawSetProportional(playerid, td, true);
    PlayerTextDrawAlignment(playerid, td, poravnanje);
    gPcTd[playerid][gPcBroj[playerid]++] = td;
    return td;
}

static PlayerText:PcGumb(playerid, Float:x, Float:y, Float:w, boja, const tekst[] = "_", tekstBoja = 0xFFFFFFFF)
{
    new PlayerText:td = PcTekst(playerid, x, y, tekst, tekstBoja, 0.17, 0.85, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 10.0, w);
    PlayerTextDrawSetSelectable(playerid, td, true);
    return td;
}

static PcCisti(tekst[])
{
    for (new i = 0; tekst[i]; i++) if (tekst[i] == '~') tekst[i] = '-';
}

static PcPostavi(playerid, dio, const tekst[], bool:vidljivo = true)
{
    PlayerTextDrawSetString(playerid, gPc[playerid][dio], tekst[0] ? tekst : "_");
    if (vidljivo) PlayerTextDrawShow(playerid, gPc[playerid][dio]);
    else PlayerTextDrawHide(playerid, gPc[playerid][dio]);
}

static PcStart(playerid, bool:otvori)
{
    gPcStart[playerid] = otvori;
    for (new i = PTD_START_PANEL; i <= PTD_UGASI; i++)
    {
        if (otvori) PlayerTextDrawShow(playerid, gPc[playerid][i]);
        else PlayerTextDrawHide(playerid, gPc[playerid][i]);
    }
}

static PcEkran(playerid, ekran)
{
    new tekst[1024], red[160];
    gPcEkran[playerid] = ekran;
    PcStart(playerid, false);
    for (new i = 0; i < 2; i++) PlayerTextDrawHide(playerid, gPc[playerid][PTD_AKCIJA + i]);
    for (new d = 0; d < sizeof gDionicaIme; d++) PlayerTextDrawHide(playerid, gPc[playerid][PTD_DIONICA + d]);
    if (ekran == PCE_POCETNA)
    {
        for (new i = PTD_PROZOR; i <= PTD_SADRZAJ; i++) PlayerTextDrawHide(playerid, gPc[playerid][i]);
        PlayerTextDrawHide(playerid, gPc[playerid][PTD_TRAKA]);
        return;
    }
    for (new i = PTD_PROZOR; i <= PTD_ZATVORI; i++) PlayerTextDrawShow(playerid, gPc[playerid][i]);
    PcPostavi(playerid, PTD_NASLOV, T(playerid, gPcAplikacije[ekran]));
    PcPostavi(playerid, PTD_TRAKA, T(playerid, gPcAplikacije[ekran]));
    switch (ekran)
    {
        case PCE_BANKA:
        {
            if (!gInv[playerid][ITEM_KARTICA]) strcat(tekst, T(playerid, "Nemate bankovnu karticu."));
            else if (!BankaTekst(playerid, tekst)) strcat(tekst, T(playerid, "Nemate bankovni racun."));
            else
            {
                PcPostavi(playerid, PTD_AKCIJA, T(playerid, "Posalji novac"));
                PcPostavi(playerid, PTD_AKCIJA + 1, T(playerid, "Otplati kredit"));
            }
        }
        case PCE_OGLASI:
        {
            new oglas[48], ime[MAX_PLAYER_NAME + 1], broj[16];
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT substr(ad.text, 1, 44), replace(a.name, '_', ' '), IFNULL((SELECT number FROM phone_numbers WHERE account = ad.account), '-') FROM ads ad JOIN accounts a ON a.id = ad.account WHERE ad.created > %d ORDER BY ad.id DESC LIMIT 6", gettime() - OGLAS_TRAJE);
            new n = DB_GetRowCount(res);
            for (new i = 0; i < n; i++, DB_SelectNextRow(res))
            {
                DB_GetFieldString(res, 0, oglas, sizeof oglas);
                DB_GetFieldString(res, 1, ime, sizeof ime);
                DB_GetFieldString(res, 2, broj, sizeof broj);
                PcCisti(oglas);
                PcCisti(ime);
                format(red, sizeof red, "%s~n~~b~   %s, %s~l~~n~", oglas, ime, broj);
                strcat(tekst, red);
            }
            DB_FreeResultSet(res);
            if (!n) strcat(tekst, T(playerid, "Nema oglasa."));
            format(red, sizeof red, T(playerid, "Objavi ($%d)"), OGLAS_CIJENA);
            PcPostavi(playerid, PTD_AKCIJA, red);
        }
        case PCE_DIONICE:
        {
            strcat(tekst, T(playerid, "Kliknite tvrtku za kupnju ili prodaju."));
            for (new d = 0; d < sizeof gDionicaIme; d++)
            {
                new razlika = gDionica[d] - gDionicaStara[d];
                format(red, sizeof red, "%s   $%d   %s%s%d~l~   (%d)", gDionicaIme[d], gDionica[d], razlika > 0 ? ("~g~") : razlika < 0 ? ("~r~") : ("~l~"), razlika > 0 ? ("+") : (""), razlika, PcKomada(playerid, d));
                PcPostavi(playerid, PTD_DIONICA + d, red);
            }
        }
    }
    PcPostavi(playerid, PTD_SADRZAJ, tekst);
}

static PcOtvori(playerid)
{
    new sat, minuta, d, m, g, vrijeme[8], datum[12], dobrodosli[64];
    gettime(sat, minuta);
    getdate(g, m, d);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    format(datum, sizeof datum, "%02d.%02d.%d", d, m, g);
    format(dobrodosli, sizeof dobrodosli, "%s~n~%s", GetRPIme(playerid), vrijeme);
    gPcOtvoren[playerid] = true;
    gPcBroj[playerid] = 0;
    PcKutija(playerid, 0.0, 0.0, 400.0, 248.0, 0x111111FF);
    PcKutija(playerid, 180.0, 248.0, 40.0, 8.0, 0x222222FF);
    PcKutija(playerid, 140.0, 256.0, 120.0, 4.0, 0x222222FF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 220.0, 0x0B4F8AFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 110.0, 0x1366ADFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 50.0, 0x1A7BC8FF);
    PcTekst(playerid, 386.0, 10.0, dobrodosli, 0xFFFFFFDD, 0.2, 1.0, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        new Float:y = 12.0 + float(i - 1) * 50.0;
        gPc[playerid][PTD_APP + i] = PcKutija(playerid, 16.0, y, 30.0, 30.0, gPcIkonaBoja[i]);
        PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_APP + i], true);
        PcTekst(playerid, 31.0, y + 7.0, gPcIkona[i], 0xFFFFFFFF, 0.42, 1.7, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
        new PlayerText:ime = PcTekst(playerid, 31.0, y + 32.0, T(playerid, gPcAplikacije[i]), 0xFFFFFFFF, 0.15, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawSetShadow(playerid, ime, 1);
    }
    PcKutija(playerid, 4.0, 224.0, 392.0, 20.0, 0x0D141CF2);
    gPc[playerid][PTD_START] = PcKutija(playerid, 8.0, 226.0, 16.0, 16.0, 0x00000001);
    PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_START], true);
    for (new i = 0; i < 4; i++) PcKutija(playerid, 10.0 + float(i % 2) * 7.0, 228.0 + float(i / 2) * 7.0, 5.5, 5.5, 0x4FC3F7FF);
    gPc[playerid][PTD_TRAKA] = PcGumb(playerid, 70.0, 229.0, 70.0, 0xFFFFFF22);
    PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_TRAKA], false);
    PcTekst(playerid, 390.0, 226.0, vrijeme, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    PcTekst(playerid, 390.0, 234.0, datum, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    gPc[playerid][PTD_PROZOR] = PcKutija(playerid, 83.0, 23.0, 300.0, 186.0, 0x00000055);
    gPc[playerid][PTD_PROZOR + 1] = PcKutija(playerid, 80.0, 20.0, 300.0, 186.0, 0xFFFFFFFF);
    gPc[playerid][PTD_PROZOR + 2] = PcKutija(playerid, 80.0, 20.0, 300.0, 16.0, 0xF0F0F0FF);
    gPc[playerid][PTD_NASLOV] = PcTekst(playerid, 88.0, 23.0, "_", 0x202020FF, 0.18, 0.85, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_ZATVORI] = PcGumb(playerid, 371.0, 23.0, 16.0, 0xE81123FF, "X");
    gPc[playerid][PTD_SADRZAJ] = PcTekst(playerid, 88.0, 42.0, "_", 0x202020FF, 0.18, 0.95, TEXT_DRAW_FONT_1);
    for (new k = 0; k < sizeof gDionicaIme; k++) gPc[playerid][PTD_DIONICA + k] = PcGumb(playerid, 230.0, 60.0 + float(k) * 20.0, 280.0, 0xE8E8E8FF, "_", 0x202020FF);
    for (new i = 0; i < 2; i++) gPc[playerid][PTD_AKCIJA + i] = PcGumb(playerid, 180.0 + float(i) * 100.0, 190.0, 90.0, 0x0078D4FF);
    gPc[playerid][PTD_START_PANEL] = PcKutija(playerid, 4.0, 164.0, 130.0, 60.0, 0x1B2430F2);
    gPc[playerid][PTD_START_PANEL + 1] = PcTekst(playerid, 12.0, 170.0, GetRPIme(playerid), 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_UGASI] = PcGumb(playerid, 69.0, 204.0, 110.0, 0x2C3746FF, T(playerid, "Ugasi"));
    for (new i = 0; i < gPcBroj[playerid]; i++) PlayerTextDrawShow(playerid, gPcTd[playerid][i]);
    PcEkran(playerid, PCE_POCETNA);
    SelectTextDraw(playerid, 0x4FC3F7FF);
}

PcZatvori(playerid)
{
    if (!gPcOtvoren[playerid]) return;
    gPcOtvoren[playerid] = false;
    for (new i = 0; i < gPcBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gPcTd[playerid][i]);
    gPcBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
}

PcDisconnect(playerid)
{
    gPcOtvoren[playerid] = false;
    gPcBroj[playerid] = 0;
}

CMD:computer(playerid, params[])
{
    if (!gUlogiran[playerid] || IsRanjen(playerid)) return 1;
    if (gPcOtvoren[playerid])
    {
        PcZatvori(playerid);
        return 1;
    }
    if (!PcBlizu(playerid)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste kod racunala."));
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s pali racunalo.", GetRPIme(playerid), "", 1);
    PcOtvori(playerid);
    return 1;
}

CMD:racunalo(playerid, params[]) return cmd_computer(playerid, params);

static PcUnos(playerid, akcija, const tekst[])
{
    gPcAkcija[playerid] = akcija;
    return PrikaziDialog(playerid, DLG_PC_UNOS, DIALOG_STYLE_INPUT, T(playerid, "Racunalo"), tekst, T(playerid, "Potvrdi"), T(playerid, "Natrag"));
}

static PcNatrag(playerid)
{
    if (!gPcOtvoren[playerid]) return 1;
    SelectTextDraw(playerid, 0x4FC3F7FF);
    if (!PcBlizu(playerid))
    {
        PcZatvori(playerid);
        return 1;
    }
    PcEkran(playerid, gPcEkran[playerid]);
    return 1;
}

PcKlik(playerid, PlayerText:td)
{
    if (!gPcOtvoren[playerid]) return 0;
    if (!PcBlizu(playerid))
    {
        PcZatvori(playerid);
        return 1;
    }
    if (td == gPc[playerid][PTD_UGASI])
    {
        PcZatvori(playerid);
        return 1;
    }
    if (td == gPc[playerid][PTD_START])
    {
        PcStart(playerid, !gPcStart[playerid]);
        return 1;
    }
    if (td == gPc[playerid][PTD_ZATVORI])
    {
        PcEkran(playerid, PCE_POCETNA);
        return 1;
    }
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        if (td != gPc[playerid][PTD_APP + i]) continue;
        PcEkran(playerid, i);
        return 1;
    }
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        if (td != gPc[playerid][PTD_DIONICA + d]) continue;
        new tekst[192];
        gPcDionica[playerid] = d;
        format(tekst, sizeof tekst, T(playerid, "%s: $%d po dionici, imate %d.~n~Unesite broj dionica (kupnja s racuna, prodaja uz %d%% provizije):"), gDionicaIme[d], gDionica[d], PcKomada(playerid, d), DIONICA_PROVIZIJA);
        for (new i = 0; tekst[i]; i++) if (tekst[i] == '~' && tekst[i + 1] == 'n' && tekst[i + 2] == '~')
        {
            tekst[i] = '\n';
            strdel(tekst, i + 1, i + 3);
        }
        PrikaziDialog(playerid, DLG_PC_DIONICA, DIALOG_STYLE_INPUT, gDionicaIme[d], tekst, T(playerid, "Kupi"), T(playerid, "Prodaj"));
        return 1;
    }
    for (new i = 0; i < 2; i++)
    {
        if (td != gPc[playerid][PTD_AKCIJA + i]) continue;
        if (gPcEkran[playerid] == PCE_BANKA) PcUnos(playerid, i ? PC_BANKA_OTPLATI : PC_BANKA_POSALJI, T(playerid, i ? ("Unesite iznos:") : ("Unesite: [id/ime primatelja] [iznos]")));
        else if (gPcEkran[playerid] == PCE_OGLASI)
        {
            new tekst[96];
            format(tekst, sizeof tekst, T(playerid, "Tekst oglasa (najvise %d znakova, cijena $%d s racuna):"), OGLAS_LEN, OGLAS_CIJENA);
            PcUnos(playerid, PC_OGLAS, tekst);
        }
        return 1;
    }
    return 1;
}

DLG:pc_dionica(playerid, response, const inputtext[])
{
    if (!gPcOtvoren[playerid]) return 1;
    new d = gPcDionica[playerid], n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 5 ? strval(inputtext) : 0, ima = PcKomada(playerid, d);
    if (n <= 0 || !PcBlizu(playerid)) return PcNatrag(playerid);
    if (response)
    {
        if (ima + n > DIONICA_MAX_KOM) SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise %d dionica po tvrtki."), DIONICA_MAX_KOM);
        else if (BankaPromijeni(playerid, -n * gDionica[d]))
        {
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stock_holdings (account, stock, shares) VALUES (%d, %d, %d)", gAccountId[playerid], d, ima + n));
            SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d dionica (%s) za $%d."), n, gDionicaIme[d], n * gDionica[d]);
        }
        return PcNatrag(playerid);
    }
    if (n > ima)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko dionica."));
        return PcNatrag(playerid);
    }
    new iznos = n * gDionica[d];
    iznos -= iznos * DIONICA_PROVIZIJA / 100;
    if (!BankaPromijeni(playerid, iznos)) return PcNatrag(playerid);
    if (ima == n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET shares = %d WHERE account = %d AND stock = %d", ima - n, gAccountId[playerid], d));
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
    return PcNatrag(playerid);
}

DLG:pc_unos(playerid, response, const inputtext[])
{
    if (!gPcOtvoren[playerid]) return 1;
    if (!response || !inputtext[0] || !PcBlizu(playerid)) return PcNatrag(playerid);
    switch (gPcAkcija[playerid])
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
            if (sad - gOglasZadnji[playerid] < OGLAS_PAUZA)
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), OGLAS_PAUZA - (sad - gOglasZadnji[playerid]));
                return PcNatrag(playerid);
            }
            strcat(tekst, inputtext);
            RemoveBoje(tekst);
            if (!tekst[0] || !BankaPromijeni(playerid, -OGLAS_CIJENA)) return PcNatrag(playerid);
            gOglasZadnji[playerid] = sad;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO ads (account, text, created) VALUES (%d, '%q', %d)", gAccountId[playerid], tekst, sad));
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT number FROM phone_numbers WHERE account = %d", gAccountId[playerid]);
            if (DB_GetRowCount(res)) DB_GetFieldString(res, 0, broj, sizeof broj);
            DB_FreeResultSet(res);
            for (new i = 0; i < MAX_PLAYERS; i++) if (gUlogiran[i]) SendClientMessage(i, 0x7CB342FF, T(i, "[Oglas] %s (%s, tel. %s)"), tekst, GetRPIme(playerid), broj);
        }
    }
    return PcNatrag(playerid);
}
