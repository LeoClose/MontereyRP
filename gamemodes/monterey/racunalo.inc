#define RACUNALO_MODEL      2190
#define RACUNALO_CIJENA     2500
#define RACUNALO_DOMET      2.0
#define OGLAS_CIJENA        500
#define OGLAS_LEN           100
#define OGLAS_PAUZA         60
#define OGLAS_TRAJE         86400
#define OBJAVA_LEN          100
#define OBJAVA_PAUZA        30
#define OBJAVA_REDOVA       6
#define DIONICA_MAX_KOM     10000
#define DIONICA_PROVIZIJA   1
#define DIONICA_KORAK_S     10
#define DIONICA_KORAKA      6
#define SVIJECA             20
#define PC_TD               110
#define PC_X                120.0
#define PC_Y                80.0
#define GRAF_X              90.0
#define GRAF_Y              70.0
#define GRAF_H              96.0
#define GRAF_SIRINA         14.0

enum { PC_BANKA_POSALJI, PC_BANKA_OTPLATI, PC_OGLAS, PC_KOLICINA, PC_OBJAVA }
enum { PCE_POCETNA, PCE_BANKA, PCE_OGLASI, PCE_DIONICE, PCE_LIFESTALKER }
enum { SV_O, SV_H, SV_L, SV_C }
enum { PTD_START, PTD_TRAKA, PTD_PROZOR, PTD_NASLOV = PTD_PROZOR + 3, PTD_ZATVORI, PTD_SADRZAJ, PTD_START_PANEL, PTD_UGASI = PTD_START_PANEL + 2, PTD_APP,
    PTD_AKCIJA = PTD_APP + 5, PTD_DIONICA = PTD_AKCIJA + 2, PTD_GRAF = PTD_DIONICA + 5, PTD_TIJELO, PTD_FITILJ = PTD_TIJELO + SVIJECA, PTD_OBJAVA = PTD_FITILJ + SVIJECA, PTD_KOL = PTD_OBJAVA + OBJAVA_REDOVA, PTD_APPZNAK = PTD_KOL + 3, PTD_APPIME = PTD_APPZNAK + 5, PTD_IMENOVANI = PTD_APPIME + 5 }

static const gDionicaIme[][] = { "Monterey Motors", "LS Nafta", "Vinewood Media", "Pacific Banka", "Grove Tech" };
static const gDionicaKratko[][] = { "MMOT", "LSNF", "VWMD", "PACB", "GRVT" };
static const gDionicaPocetna[] = { 12000, 8000, 6000, 15000, 4000 };
static gDionica[sizeof gDionicaIme], gSvijeca[sizeof gDionicaIme][SVIJECA][4], gSvijecaBroj[sizeof gDionicaIme], gTrenutna[sizeof gDionicaIme][4], gDionicaSat, gDionicaKorak;
static gPcAkcija[MAX_PLAYERS], gPcDionica[MAX_PLAYERS], gOglasZadnji[MAX_PLAYERS], gObjavaZadnja[MAX_PLAYERS], gPcEkran[MAX_PLAYERS], bool:gPcOtvoren[MAX_PLAYERS], bool:gPcStart[MAX_PLAYERS], gPcObjava[MAX_PLAYERS][OBJAVA_REDOVA], gPcKomadaPamti[MAX_PLAYERS][sizeof gDionicaIme], gPcGrafN[MAX_PLAYERS], gPcGrafMin[MAX_PLAYERS], gPcGrafMax[MAX_PLAYERS], gPcKolicina[MAX_PLAYERS];
static PlayerText:gPcTd[MAX_PLAYERS][PC_TD], gPcBroj[MAX_PLAYERS], PlayerText:gPc[MAX_PLAYERS][PTD_IMENOVANI];
static const gPcAplikacije[][] = { "Pocetna", "Banka", "Oglasnik", "Dionice", "LifeStalker" };
static const gPcIkona[][] = { "", "$", "@", "%", "L" };
static const gPcIkonaBoja[] = { 0, 0x2E7D32FF, 0xEF6C00FF, 0x6A1B9AFF, 0xE91E63FF };

static DionicaNova(cijena, pocetna, pomak)
{
    new nova = cijena + cijena * pomak / 1000 + (pocetna - cijena) / 200;
    return nova < 500 ? 500 : nova > pocetna * 10 ? pocetna * 10 : nova;
}

static DionicaKupnja(komada, cijena)
{
    return (komada * cijena + 99) / 100;
}

static DionicaProdaja(komada, cijena)
{
    new iznos = komada * cijena / 100;
    return iznos - iznos * DIONICA_PROVIZIJA / 100;
}

InitRacunalo()
{
    assert(DionicaNova(10000, 10000, 0) == 10000 && DionicaNova(10000, 10000, 10) == 10100 && DionicaNova(500, 10000, -15) == 539 && DionicaNova(100000, 10000, 15) == 100000 && DionicaNova(500, 500, -15) == 500);
    assert(DionicaKupnja(3, 12345) == 371 && DionicaProdaja(3, 12345) == 367 && DionicaKupnja(DIONICA_MAX_KOM, 150000) == 15000000);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ads (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_holdings (account INTEGER NOT NULL, stock INTEGER NOT NULL, shares INTEGER NOT NULL, PRIMARY KEY (account, stock))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_candles (id INTEGER PRIMARY KEY AUTOINCREMENT, stock INTEGER NOT NULL, o INTEGER NOT NULL, h INTEGER NOT NULL, l INTEGER NOT NULL, c INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_posts (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_likes (post INTEGER NOT NULL, account INTEGER NOT NULL, PRIMARY KEY (post, account))"));
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT o, h, l, c FROM (SELECT id, o, h, l, c FROM stock_candles WHERE stock = %d ORDER BY id DESC LIMIT %d) ORDER BY id", d, SVIJECA);
        gSvijecaBroj[d] = DB_GetRowCount(res);
        for (new i = 0; i < gSvijecaBroj[d]; i++, DB_SelectNextRow(res)) for (new k = 0; k < 4; k++) gSvijeca[d][i][k] = DB_GetFieldInt(res, k);
        DB_FreeResultSet(res);
        gDionica[d] = gSvijecaBroj[d] ? gSvijeca[d][gSvijecaBroj[d] - 1][SV_C] : gDionicaPocetna[d];
        for (new k = 0; k < 4; k++) gTrenutna[d][k] = gDionica[d];
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM server_flags WHERE name = 'pc_katalog'");
    new bool:dodano = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    if (dodano) return;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO furniture_catalog (shop, model, price, name) SELECT id, %d, %d, 'Racunalo' FROM furniture_shops WHERE id NOT IN (SELECT shop FROM furniture_catalog WHERE model = %d)", RACUNALO_MODEL, RACUNALO_CIJENA, RACUNALO_MODEL));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO server_flags (name) VALUES ('pc_katalog')"));
}

static PcKomada(playerid, d)
{
    return gPcKomadaPamti[playerid][d];
}

static PcUcitajKomade(playerid)
{
    for (new d = 0; d < sizeof gDionicaIme; d++) gPcKomadaPamti[playerid][d] = 0;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT stock, shares FROM stock_holdings WHERE account = %d", gAccountId[playerid]);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new d = DB_GetFieldInt(res, 0);
        if (d >= 0 && d < sizeof gDionicaIme) gPcKomadaPamti[playerid][d] = DB_GetFieldInt(res, 1);
    }
    DB_FreeResultSet(res);
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

static PlayerText:PcRed(playerid, Float:x, Float:y, Float:w)
{
    new PlayerText:td = PcTekst(playerid, x, y, "_", 0x202020FF, 0.16, 0.8, TEXT_DRAW_FONT_1);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, 0xF2F2F2FF);
    PlayerTextDrawTextSize(playerid, td, PC_X + x + w, 10.0);
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

static PcSakrij(playerid, od, broj)
{
    for (new i = 0; i < broj; i++) PlayerTextDrawHide(playerid, gPc[playerid][od + i]);
}

static PcStart(playerid, bool:otvori)
{
    gPcStart[playerid] = otvori;
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        if (12.0 + float(i - 1) * 50.0 + 42.0 < 164.0) continue;
        for (new k = PTD_APP; k <= PTD_APPIME; k += 5)
        {
            if (otvori) PlayerTextDrawHide(playerid, gPc[playerid][k + i]);
            else PlayerTextDrawShow(playerid, gPc[playerid][k + i]);
        }
    }
    for (new i = PTD_START_PANEL; i <= PTD_UGASI; i++)
    {
        if (otvori) PlayerTextDrawShow(playerid, gPc[playerid][i]);
        else PlayerTextDrawHide(playerid, gPc[playerid][i]);
    }
}

static PcSvijeca(playerid, i, const s[4], najmanje, najvise)
{
    new Float:visina = float(najvise - najmanje > 0 ? najvise - najmanje : 1), Float:cx = GRAF_X + GRAF_SIRINA / 2.0 + float(i) * GRAF_SIRINA, boja = s[SV_C] >= s[SV_O] ? 0x26A69AFF : 0xEF5350FF;
    new gore = s[SV_C] > s[SV_O] ? s[SV_C] : s[SV_O], dolje = s[SV_C] > s[SV_O] ? s[SV_O] : s[SV_C];
    new Float:yh = GRAF_Y + float(najvise - s[SV_H]) / visina * GRAF_H, Float:yl = GRAF_Y + float(najvise - s[SV_L]) / visina * GRAF_H;
    new Float:yg = GRAF_Y + float(najvise - gore) / visina * GRAF_H, Float:yd = GRAF_Y + float(najvise - dolje) / visina * GRAF_H;
    new PlayerText:fitilj = gPc[playerid][PTD_FITILJ + i], PlayerText:tijelo = gPc[playerid][PTD_TIJELO + i];
    PlayerTextDrawSetPos(playerid, fitilj, PC_X + cx - 0.5, PC_Y + yh);
    PlayerTextDrawTextSize(playerid, fitilj, 1.0, yl - yh > 1.0 ? yl - yh : 1.0);
    PlayerTextDrawColour(playerid, fitilj, boja);
    PlayerTextDrawSetPos(playerid, tijelo, PC_X + cx - 4.0, PC_Y + yg);
    PlayerTextDrawTextSize(playerid, tijelo, 8.0, yd - yg > 1.0 ? yd - yg : 1.0);
    PlayerTextDrawColour(playerid, tijelo, boja);
    PlayerTextDrawShow(playerid, fitilj);
    PlayerTextDrawShow(playerid, tijelo);
}

static PcGraf(playerid, bool:sve = true)
{
    new d = gPcDionica[playerid], n = gSvijecaBroj[d] < SVIJECA ? gSvijecaBroj[d] + 1 : SVIJECA, prvi = gSvijecaBroj[d] + 1 - n, najmanje = gTrenutna[d][SV_L], najvise = gTrenutna[d][SV_H];
    for (new i = 0; i < n - 1; i++)
    {
        if (gSvijeca[d][prvi + i][SV_L] < najmanje) najmanje = gSvijeca[d][prvi + i][SV_L];
        if (gSvijeca[d][prvi + i][SV_H] > najvise) najvise = gSvijeca[d][prvi + i][SV_H];
    }
    if (sve || n != gPcGrafN[playerid] || najmanje != gPcGrafMin[playerid] || najvise != gPcGrafMax[playerid])
    {
        gPcGrafN[playerid] = n;
        gPcGrafMin[playerid] = najmanje;
        gPcGrafMax[playerid] = najvise;
        for (new i = 0; i < n - 1; i++) PcSvijeca(playerid, i, gSvijeca[d][prvi + i], najmanje, najvise);
        for (new i = n; i < SVIJECA; i++)
        {
            PlayerTextDrawHide(playerid, gPc[playerid][PTD_TIJELO + i]);
            PlayerTextDrawHide(playerid, gPc[playerid][PTD_FITILJ + i]);
        }
    }
    PcSvijeca(playerid, n - 1, gTrenutna[d], najmanje, najvise);
    new tekst[160], pocetak = gSvijecaBroj[d] ? gSvijeca[d][prvi][SV_O] : gTrenutna[d][SV_O], promjena = pocetak ? (gDionica[d] - pocetak) * 1000 / pocetak : 0, apsolutno = promjena < 0 ? -promjena : promjena;
    format(tekst, sizeof tekst, T(playerid, "%s  ~l~$%d.%02d  %s%d.%d%%~l~   Imate: %d"), gDionicaIme[d], gDionica[d] / 100, gDionica[d] % 100, promjena >= 0 ? ("~g~+") : ("~r~-"), apsolutno / 10, apsolutno % 10, PcKomada(playerid, d));
    PcPostavi(playerid, PTD_SADRZAJ, tekst);
    new ukupno = gPcKolicina[playerid] * gDionica[d];
    format(tekst, sizeof tekst, T(playerid, "%d kom  ($%d.%02d)"), gPcKolicina[playerid], ukupno / 100, ukupno % 100);
    PcPostavi(playerid, PTD_KOL + 1, tekst);
    if (!sve) return;
    for (new k = 0; k < sizeof gDionicaIme; k++)
    {
        PlayerTextDrawBoxColour(playerid, gPc[playerid][PTD_DIONICA + k], k == d ? 0x0078D4FF : 0xDDDDDDFF);
        PlayerTextDrawColour(playerid, gPc[playerid][PTD_DIONICA + k], k == d ? 0xFFFFFFFF : 0x202020FF);
        PlayerTextDrawShow(playerid, gPc[playerid][PTD_DIONICA + k]);
    }
}

static PcLifeStalker(playerid)
{
    new red[256], tekst[OBJAVA_LEN + 1], ime[MAX_PLAYER_NAME + 1], sad = gettime();
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT p.id, p.text, replace(a.name, '_', ' '), p.created, (SELECT COUNT(*) FROM social_likes WHERE post = p.id), (SELECT COUNT(*) FROM social_likes WHERE post = p.id AND account = %d) FROM social_posts p JOIN accounts a ON a.id = p.account ORDER BY p.id DESC LIMIT %d", gAccountId[playerid], OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < OBJAVA_REDOVA; r++)
    {
        if (r >= n)
        {
            gPcObjava[playerid][r] = 0;
            PlayerTextDrawHide(playerid, gPc[playerid][PTD_OBJAVA + r]);
            continue;
        }
        new prije = (sad - DB_GetFieldInt(res, 3)) / 60, lajkova = DB_GetFieldInt(res, 4), moj = DB_GetFieldInt(res, 5);
        gPcObjava[playerid][r] = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, tekst, sizeof tekst);
        DB_GetFieldString(res, 2, ime, sizeof ime);
        PcCisti(tekst);
        PcCisti(ime);
        if (prije < 60) format(red, sizeof red, T(playerid, "~b~%s  ~l~prije %d min   %s+%d~n~~l~%s"), ime, prije, moj ? ("~r~") : ("~l~"), lajkova, tekst);
        else format(red, sizeof red, T(playerid, "~b~%s  ~l~prije %d h   %s+%d~n~~l~%s"), ime, prije / 60, moj ? ("~r~") : ("~l~"), lajkova, tekst);
        PcPostavi(playerid, PTD_OBJAVA + r, red);
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : T(playerid, "Jos nema objava. Budite prvi!"));
    PcPostavi(playerid, PTD_AKCIJA, T(playerid, "Objavi"));
}

static PcEkran(playerid, ekran)
{
    new tekst[1024], red[160];
    gPcEkran[playerid] = ekran;
    PcStart(playerid, false);
    PcSakrij(playerid, PTD_AKCIJA, 2);
    PcSakrij(playerid, PTD_DIONICA, sizeof gDionicaIme);
    PcSakrij(playerid, PTD_GRAF, 1 + SVIJECA * 2);
    PcSakrij(playerid, PTD_OBJAVA, OBJAVA_REDOVA);
    PcSakrij(playerid, PTD_KOL, 3);
    if (ekran == PCE_POCETNA)
    {
        for (new i = PTD_PROZOR; i <= PTD_SADRZAJ; i++) PlayerTextDrawHide(playerid, gPc[playerid][i]);
        PlayerTextDrawHide(playerid, gPc[playerid][PTD_TRAKA]);
        return;
    }
    for (new i = PTD_PROZOR; i <= PTD_ZATVORI; i++) PlayerTextDrawShow(playerid, gPc[playerid][i]);
    PcPostavi(playerid, PTD_NASLOV, T(playerid, gPcAplikacije[ekran]));
    PcPostavi(playerid, PTD_TRAKA, T(playerid, gPcAplikacije[ekran]));
    PlayerTextDrawSetPos(playerid, gPc[playerid][PTD_SADRZAJ], PC_X + 88.0, PC_Y + (ekran == PCE_DIONICE ? 52.0 : 42.0));
    for (new i = 0; i < 2; i++) PlayerTextDrawBoxColour(playerid, gPc[playerid][PTD_AKCIJA + i], ekran == PCE_DIONICE ? (i ? 0xC62828FF : 0x2E7D32FF) : ekran == PCE_LIFESTALKER ? 0xE91E63FF : 0x0078D4FF);
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
            PlayerTextDrawShow(playerid, gPc[playerid][PTD_GRAF]);
            PcPostavi(playerid, PTD_AKCIJA, T(playerid, "Kupi"));
            PcPostavi(playerid, PTD_AKCIJA + 1, T(playerid, "Prodaj"));
            PlayerTextDrawShow(playerid, gPc[playerid][PTD_KOL]);
            PlayerTextDrawShow(playerid, gPc[playerid][PTD_KOL + 2]);
            PcGraf(playerid);
            return;
        }
        case PCE_LIFESTALKER:
        {
            PcLifeStalker(playerid);
            return;
        }
    }
    PcPostavi(playerid, PTD_SADRZAJ, tekst);
}

RacunaloTick()
{
    if (++gDionicaSat < DIONICA_KORAK_S) return;
    gDionicaSat = 0;
    new bool:zatvori = ++gDionicaKorak >= DIONICA_KORAKA;
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        gDionica[d] = DionicaNova(gDionica[d], gDionicaPocetna[d], random(31) - 15);
        gTrenutna[d][SV_C] = gDionica[d];
        if (gDionica[d] > gTrenutna[d][SV_H]) gTrenutna[d][SV_H] = gDionica[d];
        if (gDionica[d] < gTrenutna[d][SV_L]) gTrenutna[d][SV_L] = gDionica[d];
        if (!zatvori) continue;
        if (gSvijecaBroj[d] == SVIJECA)
        {
            for (new i = 1; i < SVIJECA; i++) for (new k = 0; k < 4; k++) gSvijeca[d][i - 1][k] = gSvijeca[d][i][k];
            gSvijecaBroj[d]--;
        }
        for (new k = 0; k < 4; k++) gSvijeca[d][gSvijecaBroj[d]][k] = gTrenutna[d][k];
        gSvijecaBroj[d]++;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO stock_candles (stock, o, h, l, c) VALUES (%d, %d, %d, %d, %d)", d, gTrenutna[d][SV_O], gTrenutna[d][SV_H], gTrenutna[d][SV_L], gTrenutna[d][SV_C]));
        for (new k = 0; k < 4; k++) gTrenutna[d][k] = gDionica[d];
    }
    if (zatvori)
    {
        gDionicaKorak = 0;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_candles WHERE id NOT IN (SELECT id FROM stock_candles ORDER BY id DESC LIMIT %d)", SVIJECA * sizeof gDionicaIme));
    }
    for (new i = 0; i < MAX_PLAYERS; i++) if (gPcOtvoren[i] && gPcEkran[i] == PCE_DIONICE) PcGraf(i, zatvori);
}

static PcOtvori(playerid)
{
    new sat, minuta, d, m, g, vrijeme[8], datum[12];
    gettime(sat, minuta);
    getdate(g, m, d);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    format(datum, sizeof datum, "%02d.%02d.%d", d, m, g);
    gPcOtvoren[playerid] = true;
    gPcBroj[playerid] = 0;
    gPcKolicina[playerid] = 1;
    PcUcitajKomade(playerid);
    PcKutija(playerid, 0.0, 0.0, 400.0, 248.0, 0x111111FF);
    PcKutija(playerid, 180.0, 248.0, 40.0, 8.0, 0x222222FF);
    PcKutija(playerid, 140.0, 256.0, 120.0, 4.0, 0x222222FF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 220.0, 0x0B4F8AFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 110.0, 0x1366ADFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 50.0, 0x1A7BC8FF);
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        new Float:y = 12.0 + float(i - 1) * 50.0;
        gPc[playerid][PTD_APP + i] = PcKutija(playerid, 16.0, y, 30.0, 30.0, gPcIkonaBoja[i]);
        PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_APP + i], true);
        gPc[playerid][PTD_APPZNAK + i] = PcTekst(playerid, 31.0, y + 7.0, gPcIkona[i], 0xFFFFFFFF, 0.42, 1.7, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
        new PlayerText:ime = gPc[playerid][PTD_APPIME + i] = PcTekst(playerid, 31.0, y + 32.0, T(playerid, gPcAplikacije[i]), 0xFFFFFFFF, 0.15, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
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
    for (new k = 0; k < sizeof gDionicaIme; k++) gPc[playerid][PTD_DIONICA + k] = PcGumb(playerid, 110.0 + float(k) * 60.0, 40.0, 54.0, 0xDDDDDDFF, gDionicaKratko[k], 0x202020FF);
    gPc[playerid][PTD_GRAF] = PcKutija(playerid, GRAF_X - 2.0, GRAF_Y - 4.0, SVIJECA * GRAF_SIRINA + 4.0, GRAF_H + 8.0, 0xF5F5F5FF);
    for (new i = 0; i < SVIJECA; i++) gPc[playerid][PTD_TIJELO + i] = PcKutija(playerid, GRAF_X, GRAF_Y, 1.0, 1.0, 0x00000000);
    for (new i = 0; i < SVIJECA; i++) gPc[playerid][PTD_FITILJ + i] = PcKutija(playerid, GRAF_X, GRAF_Y, 1.0, 1.0, 0x00000000);
    for (new r = 0; r < OBJAVA_REDOVA; r++) gPc[playerid][PTD_OBJAVA + r] = PcRed(playerid, 86.0, 42.0 + float(r) * 24.0, 288.0);
    gPc[playerid][PTD_KOL] = PcGumb(playerid, 168.0, 174.0, 14.0, 0x0078D4FF, "~<~");
    gPc[playerid][PTD_KOL + 1] = PcGumb(playerid, 230.0, 174.0, 100.0, 0xDDDDDDFF, "_", 0x202020FF);
    gPc[playerid][PTD_KOL + 2] = PcGumb(playerid, 292.0, 174.0, 14.0, 0x0078D4FF, "~>~");
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
        gPcDionica[playerid] = d;
        PcGraf(playerid);
        return 1;
    }
    if (td == gPc[playerid][PTD_KOL] || td == gPc[playerid][PTD_KOL + 2])
    {
        new n = gPcKolicina[playerid] + (td == gPc[playerid][PTD_KOL] ? -1 : 1);
        gPcKolicina[playerid] = n < 1 ? 1 : n > DIONICA_MAX_KOM ? DIONICA_MAX_KOM : n;
        PcGraf(playerid, false);
        return 1;
    }
    if (td == gPc[playerid][PTD_KOL + 1])
    {
        new tekst[96];
        format(tekst, sizeof tekst, T(playerid, "Koliko dionica? (1-%d)"), DIONICA_MAX_KOM);
        return PcUnos(playerid, PC_KOLICINA, tekst);
    }
    for (new r = 0; r < OBJAVA_REDOVA; r++)
    {
        if (td != gPc[playerid][PTD_OBJAVA + r] || !gPcObjava[playerid][r]) continue;
        new post = gPcObjava[playerid][r], DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_likes WHERE post = %d AND account = %d", post, gAccountId[playerid]);
        new bool:ima = DB_GetRowCount(res) > 0;
        DB_FreeResultSet(res);
        if (ima) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_likes WHERE post = %d AND account = %d", post, gAccountId[playerid]));
        else DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_likes (post, account) VALUES (%d, %d)", post, gAccountId[playerid]));
        PcLifeStalker(playerid);
        return 1;
    }
    for (new i = 0; i < 2; i++)
    {
        if (td != gPc[playerid][PTD_AKCIJA + i]) continue;
        new tekst[160];
        switch (gPcEkran[playerid])
        {
            case PCE_BANKA: PcUnos(playerid, i ? PC_BANKA_OTPLATI : PC_BANKA_POSALJI, T(playerid, i ? ("Unesite iznos:") : ("Unesite: [id/ime primatelja] [iznos]")));
            case PCE_OGLASI:
            {
                format(tekst, sizeof tekst, T(playerid, "Tekst oglasa (najvise %d znakova, cijena $%d s racuna):"), OGLAS_LEN, OGLAS_CIJENA);
                PcUnos(playerid, PC_OGLAS, tekst);
            }
            case PCE_DIONICE:
            {
                PcDionicaTrgovina(playerid, i == 0, gPcKolicina[playerid]);
                PcGraf(playerid, false);
            }
            case PCE_LIFESTALKER:
            {
                format(tekst, sizeof tekst, T(playerid, "Sto vam je na umu? (najvise %d znakova)"), OBJAVA_LEN);
                PcUnos(playerid, PC_OBJAVA, tekst);
            }
        }
        return 1;
    }
    return 1;
}

static PcDionicaTrgovina(playerid, bool:kupnja, n)
{
    new d = gPcDionica[playerid], ima = PcKomada(playerid, d), cijena = gDionica[d];
    if (n <= 0) return 1;
    if (kupnja)
    {
        new iznos = DionicaKupnja(n, cijena);
        if (ima + n > DIONICA_MAX_KOM) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Najvise %d dionica po tvrtki."), DIONICA_MAX_KOM);
        if (!BankaPromijeni(playerid, -iznos)) return 1;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stock_holdings (account, stock, shares) VALUES (%d, %d, %d)", gAccountId[playerid], d, ima + n));
        gPcKomadaPamti[playerid][d] = ima + n;
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
    }
    if (n > ima) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko dionica."));
    new iznos = DionicaProdaja(n, cijena);
    if (!BankaPromijeni(playerid, iznos)) return 1;
    if (ima == n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d));
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET shares = %d WHERE account = %d AND stock = %d", ima - n, gAccountId[playerid], d));
    gPcKomadaPamti[playerid][d] = ima - n;
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
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
        case PC_KOLICINA:
        {
            new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 5 ? strval(inputtext) : 0;
            if (n >= 1 && n <= DIONICA_MAX_KOM) gPcKolicina[playerid] = n;
        }
        case PC_OBJAVA:
        {
            new tekst[OBJAVA_LEN + 1], sad = gettime();
            if (sad - gObjavaZadnja[playerid] < OBJAVA_PAUZA)
            {
                SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), OBJAVA_PAUZA - (sad - gObjavaZadnja[playerid]));
                return PcNatrag(playerid);
            }
            strcat(tekst, inputtext);
            RemoveBoje(tekst);
            if (!tekst[0]) return PcNatrag(playerid);
            gObjavaZadnja[playerid] = sad;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_posts (account, text, created) VALUES (%d, '%q', %d)", gAccountId[playerid], tekst, sad));
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
