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
#define PC_TD               185
#define PC_OBAVIJEST_MS     6000
#define KLAD_RED            4
#define KLAD_NAVY           0x0E2A47FF
#define KLAD_NAVY2          0x16406BFF
#define KLAD_ZUTO           0xFFD200FF
#define KLAD_SIVO           0xE6EAF0FF
#define KLAD_OK             "(CASE WHEN l.pick <= 3 THEN e.result = l.pick WHEN l.pick = 4 THEN (e.hscore + e.ascore) * 10 > l.line WHEN l.pick = 5 THEN (e.hscore + e.ascore) * 10 < l.line WHEN l.pick = 6 THEN e.hscore * 10 + l.line > e.ascore * 10 ELSE e.ascore * 10 + l.line > e.hscore * 10 END)"
#define KLAD_PUSH           "(CASE WHEN l.pick IN (4, 5) THEN (e.hscore + e.ascore) * 10 = l.line WHEN l.pick = 6 THEN e.hscore * 10 + l.line = e.ascore * 10 WHEN l.pick = 7 THEN e.ascore * 10 + l.line = e.hscore * 10 ELSE 0 END)"
#define KLAD_PAROVA         8
#define KLAD_MIN_ULOG       10
#define KLAD_MAX_ULOG       10000
#define KLAD_MAX_KOEF       100000
#define KLAD_MAX_DOBITAK    1000000
#define KLAD_ZATVARA_S      60
#define KLAD_OBRACUN        40
#define KLAD_PROVJERA_S     60
#define LS_PAUZA            5
#define LS_RED_Y            42.0
#define LS_RED_H            22.0
#define LS_SLOT_X           151.0
#define LS_AV_S             80.0
#define LS_PR_X             175.0
#define LS_PR_S             150.0
#define PC_X                120.0
#define PC_Y                80.0
#define GRAF_X              90.0
#define GRAF_Y              70.0
#define GRAF_H              96.0
#define GRAF_SIRINA         14.0

enum { PC_BANKA_POSALJI, PC_BANKA_OTPLATI, PC_OGLAS, PC_KOLICINA, PC_OBJAVA, PC_KOMENTAR, PC_PORUKA, PC_PRIJATELJ, PC_PORUKA_KOME }
enum { LS_FEED, LS_OBJAVA, LS_PRIJATELJI, LS_PORUKE, LS_RAZGOVOR, LS_PROFIL }
enum { LS_NISTA, LS_POSLAO, LS_PRIMIO, LS_PRIJATELJ }
enum { KT_BOK, KT_LISTIC, KT_TAB, KT_KAT = KT_TAB + 3, KT_LIGA = KT_KAT + 5, KT_MEC = KT_LIGA + 7, KT_RED, KT_KOEF = KT_RED + KLAD_RED, KT_STR_L = KT_KOEF + KLAD_RED * 3, KT_STR, KT_STR_D, KT_NASLOV, KT_PAR, KT_UKUPNO = KT_PAR + KLAD_PAROVA, KT_MINUS, KT_ULOG, KT_PLUS, KT_DOBITAK, KT_UPLATI, KLAD_TD }
enum { PCE_POCETNA, PCE_BANKA, PCE_OGLASI, PCE_DIONICE, PCE_LIFESTALKER, PCE_KLADIONICA }
enum { SV_O, SV_H, SV_L, SV_C }
enum { PTD_START, PTD_TRAKA, PTD_PROZOR, PTD_NASLOV = PTD_PROZOR + 3, PTD_ZATVORI, PTD_SADRZAJ, PTD_START_PANEL, PTD_UGASI = PTD_START_PANEL + 2, PTD_APP,
    PTD_AKCIJA = PTD_APP + 6, PTD_DIONICA = PTD_AKCIJA + 2, PTD_GRAF = PTD_DIONICA + 5, PTD_TIJELO, PTD_FITILJ = PTD_TIJELO + SVIJECA, PTD_OBJAVA = PTD_FITILJ + SVIJECA, PTD_KOL = PTD_OBJAVA + OBJAVA_REDOVA, PTD_APPZNAK = PTD_KOL + 3, PTD_APPIME = PTD_APPZNAK + 6, PTD_LSTAB = PTD_APPIME + 6, PTD_LSAVATAR = PTD_LSTAB + 4, PTD_LSPROFIL = PTD_LSAVATAR + OBJAVA_REDOVA, PTD_LSPOKRIV = PTD_LSPROFIL + 1, PTD_LSINFO = PTD_LSPOKRIV + 8, PTD_KLAD = PTD_LSINFO + 1, PTD_SAT = PTD_KLAD + KLAD_TD, PTD_DATUM, PTD_OBAVIJEST, PTD_OBAVIJEST_TEKST, PTD_IMENOVANI }

static const gDionicaIme[][] = { "Monterey Motors", "LS Nafta", "Vinewood Media", "Pacific Banka", "Grove Tech" };
static const gDionicaKratko[][] = { "MMOT", "LSNF", "VWMD", "PACB", "GRVT" };
static const gDionicaPocetna[] = { 12000, 8000, 6000, 15000, 4000 };
static gDionica[sizeof gDionicaIme], gSvijeca[sizeof gDionicaIme][SVIJECA][4], gSvijecaBroj[sizeof gDionicaIme], gTrenutna[sizeof gDionicaIme][4], gDionicaSat, gDionicaKorak;
static gPcAkcija[MAX_PLAYERS], gPcDionica[MAX_PLAYERS], gOglasZadnji[MAX_PLAYERS], gObjavaZadnja[MAX_PLAYERS], gPcEkran[MAX_PLAYERS], bool:gPcOtvoren[MAX_PLAYERS], bool:gPcStart[MAX_PLAYERS], gPcObjava[MAX_PLAYERS][OBJAVA_REDOVA], gPcKomadaPamti[MAX_PLAYERS][sizeof gDionicaIme], gPcTrosak[MAX_PLAYERS][sizeof gDionicaIme], gPcGrafN[MAX_PLAYERS], gPcGrafMin[MAX_PLAYERS], gPcGrafMax[MAX_PLAYERS], gPcKolicina[MAX_PLAYERS], gLsPogled[MAX_PLAYERS], gLsCilj[MAX_PLAYERS], gLsOdabran[MAX_PLAYERS], gLsZadnje[MAX_PLAYERS], gLsAvatara[MAX_PLAYERS];
static const gLsTab[][] = { "Objave", "Prijatelji", "Poruke", "Profil" };
static PlayerText:gPcTd[MAX_PLAYERS][PC_TD], gPcBroj[MAX_PLAYERS], PlayerText:gPc[MAX_PLAYERS][PTD_IMENOVANI];
static const gPcAplikacije[][] = { "Pocetna", "Banka", "Oglasnik", "Dionice", "LifeStalker", "Kladionica" };
static const gPcIkona[][] = { "", "$", "@", "%", "L", "K" };
static const gPcIkonaBoja[] = { 0, 0x2E7D32FF, 0xEF6C00FF, 0x6A1B9AFF, 0xE91E63FF, 0x00897BFF };
static gKladPar[MAX_PLAYERS][KLAD_PAROVA][16], gKladTip[MAX_PLAYERS][KLAD_PAROVA], gKladKoefPar[MAX_PLAYERS][KLAD_PAROVA], gKladN[MAX_PLAYERS], gKladStr[MAX_PLAYERS], bool:gKladIma[MAX_PLAYERS], gKladRed[MAX_PLAYERS][KLAD_RED][16], gKladSat;
static gPcObavijestDo[MAX_PLAYERS], gKladLiga[MAX_PLAYERS] = { -1, ... }, gKladMecId[MAX_PLAYERS][16], gKladMecLin[MAX_PLAYERS][3], gKladLin[MAX_PLAYERS][KLAD_PAROVA], gKladTab[MAX_PLAYERS], gKladKat[MAX_PLAYERS], gKladUlog[MAX_PLAYERS], gKladDetalj[MAX_PLAYERS], gKladRedKoef[MAX_PLAYERS][KLAD_RED][3], gKladRedIme[MAX_PLAYERS][KLAD_RED][24], gKladParIme[MAX_PLAYERS][KLAD_PAROVA][24], gKladRedListic[MAX_PLAYERS][KLAD_RED];
static const gKladTabovi[][] = { "Ponuda", "U igri", "Zavrseni" };
static const gKladKategorije[][] = { "Sve", "Nogomet", "Kosarka", "Hokej", "Am. nogomet" };
static const gKladKatSql[][] = { "1", "league IN ('EPL','LaLiga','SerieA','Bundes','Ligue1','UCL','UEL')", "league = 'NBA'", "league = 'NHL'", "league = 'NFL'" };
static const gKladLige[][] = { "EPL", "LaLiga", "SerieA", "Bundes", "Ligue1", "UCL", "UEL" };
static const gKladStupci[][] = { "hscore", "ascore", "tline", "odd_over", "odd_under", "sline", "odd_sph", "odd_spa" };
static const gKladUlozi[] = { 10, 20, 50, 100, 200, 500, 1000, 2000, 5000, 10000 };

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
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_events (id TEXT PRIMARY KEY, league TEXT NOT NULL, home TEXT NOT NULL, away TEXT NOT NULL, start INTEGER NOT NULL, status INTEGER NOT NULL DEFAULT 0, result INTEGER NOT NULL DEFAULT 0, odd1 INTEGER NOT NULL DEFAULT 0, oddx INTEGER NOT NULL DEFAULT 0, odd2 INTEGER NOT NULL DEFAULT 0, updated INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_slips (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, stake INTEGER NOT NULL, odds INTEGER NOT NULL, payout INTEGER NOT NULL, status INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL, settled INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_legs (slip INTEGER NOT NULL, event TEXT NOT NULL, pick INTEGER NOT NULL, odd INTEGER NOT NULL, line INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (slip, event))"));
    new DBResult:stupac = DB_ExecuteQuery(gDB, "SELECT line FROM bet_legs LIMIT 1");
    if (stupac) DB_FreeResultSet(stupac);
    else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE bet_legs ADD COLUMN line INTEGER NOT NULL DEFAULT 0"));
    for (new c = 0; c < sizeof gKladStupci; c++)
    {
        stupac = DB_ExecuteQuery(gDB, "SELECT %s FROM bet_events LIMIT 1", gKladStupci[c]);
        if (stupac) DB_FreeResultSet(stupac);
        else DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE bet_events ADD COLUMN %s INTEGER NOT NULL DEFAULT 0", gKladStupci[c]));
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ads (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_holdings (account INTEGER NOT NULL, stock INTEGER NOT NULL, shares INTEGER NOT NULL, PRIMARY KEY (account, stock))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_candles (id INTEGER PRIMARY KEY AUTOINCREMENT, stock INTEGER NOT NULL, o INTEGER NOT NULL, h INTEGER NOT NULL, l INTEGER NOT NULL, c INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_posts (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_likes (post INTEGER NOT NULL, account INTEGER NOT NULL, PRIMARY KEY (post, account))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_accounts (account INTEGER PRIMARY KEY, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_accounts (account, created) SELECT DISTINCT owner, %d FROM furniture WHERE model = %d", gettime(), RACUNALO_MODEL));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_friends (a INTEGER NOT NULL, b INTEGER NOT NULL, accepted INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL, PRIMARY KEY (a, b))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_comments (id INTEGER PRIMARY KEY AUTOINCREMENT, post INTEGER NOT NULL, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_messages (id INTEGER PRIMARY KEY AUTOINCREMENT, sender INTEGER NOT NULL, receiver INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL, seen INTEGER NOT NULL DEFAULT 0)"));
    for (new d = 0; d < sizeof gDionicaIme; d++)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT o, h, l, c FROM (SELECT id, o, h, l, c FROM stock_candles WHERE stock = %d ORDER BY id DESC LIMIT %d) ORDER BY id", d, SVIJECA);
        gSvijecaBroj[d] = DB_GetRowCount(res);
        for (new i = 0; i < gSvijecaBroj[d]; i++, DB_SelectNextRow(res)) for (new k = 0; k < 4; k++) gSvijeca[d][i][k] = DB_GetFieldInt(res, k);
        DB_FreeResultSet(res);
        gDionica[d] = gSvijecaBroj[d] ? gSvijeca[d][gSvijecaBroj[d] - 1][SV_C] : gDionicaPocetna[d];
        for (new k = 0; k < 4; k++) gTrenutna[d][k] = gDionica[d];
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT cost FROM stock_holdings LIMIT 1");
    if (res) DB_FreeResultSet(res);
    else
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "ALTER TABLE stock_holdings ADD COLUMN cost INTEGER NOT NULL DEFAULT 0"));
        for (new d = 0; d < sizeof gDionicaIme; d++) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET cost = shares * %d / 100 WHERE stock = %d", gDionica[d], d));
    }
    res = DB_ExecuteQuery(gDB, "SELECT 1 FROM server_flags WHERE name = 'pc_katalog'");
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
    for (new d = 0; d < sizeof gDionicaIme; d++) gPcKomadaPamti[playerid][d] = gPcTrosak[playerid][d] = 0;
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT stock, shares, cost FROM stock_holdings WHERE account = %d", gAccountId[playerid]);
    for (new i = 0, n = DB_GetRowCount(res); i < n; i++, DB_SelectNextRow(res))
    {
        new d = DB_GetFieldInt(res, 0);
        if (d < 0 || d >= sizeof gDionicaIme) continue;
        gPcKomadaPamti[playerid][d] = DB_GetFieldInt(res, 1);
        gPcTrosak[playerid][d] = DB_GetFieldInt(res, 2);
    }
    DB_FreeResultSet(res);
}

static bool:PcBlizu(playerid)
{
    return gUlogiran[playerid] && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT && (NamjestajModelBlizu(playerid, RACUNALO_MODEL, RACUNALO_DOMET) != 0 || KaficRacunalo(playerid) != 0);
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

static PlayerText:PcModel(playerid, Float:x, Float:y, Float:w, Float:h)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, PC_X + x, PC_Y + y, "_");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawTextSize(playerid, td, w, h);
    PlayerTextDrawBackgroundColour(playerid, td, 0xE4E6EBFF);
    PlayerTextDrawSetPreviewModel(playerid, td, DEFAULT_SKIN);
    PlayerTextDrawSetPreviewRot(playerid, td, 0.0, 0.0, 0.0, 1.0);
    gPcTd[playerid][gPcBroj[playerid]++] = td;
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
        if (12.0 + float(i - 1) * 42.0 + 40.0 < 164.0) continue;
        new const dijelovi[] = { PTD_APP, PTD_APPZNAK, PTD_APPIME };
        for (new k = 0; k < sizeof dijelovi; k++)
        {
            if (otvori) PlayerTextDrawHide(playerid, gPc[playerid][dijelovi[k] + i]);
            else PlayerTextDrawShow(playerid, gPc[playerid][dijelovi[k] + i]);
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
    new ima = PcKomada(playerid, d), trosak = gPcTrosak[playerid][d];
    if (ima)
    {
        new dobit = DionicaProdaja(ima, gDionica[d]) - trosak, posto = trosak ? floatround(float(dobit) * 1000.0 / float(trosak)) : 0, red[64];
        format(red, sizeof red, "   P/L: %s$%d (%s%d.%d%%)", dobit >= 0 ? ("~g~+") : ("~r~-"), dobit < 0 ? -dobit : dobit, dobit >= 0 ? ("+") : ("-"), (posto < 0 ? -posto : posto) / 10, (posto < 0 ? -posto : posto) % 10);
        strcat(tekst, red);
    }
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

static LsSkin(acc)
{
    new i = IgracRacuna(acc);
    if (i != INVALID_PLAYER_ID) return GetPlayerSkin(i);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT skin FROM player_style WHERE account = %d", acc), skin = DEFAULT_SKIN;
    if (DB_GetRowCount(res) && DB_GetFieldInt(res, 0) >= 0) skin = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return skin;
}

static LsIme(acc, ime[], len)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT replace(name, '_', ' ') FROM accounts WHERE id = %d", acc);
    ime[0] = 0;
    if (DB_GetRowCount(res)) DB_GetFieldString(res, 0, ime, len);
    DB_FreeResultSet(res);
    TdCisti(ime);
}

static LsRacun(const unos[])
{
    new ime[MAX_PLAYER_NAME + 1];
    strcat(ime, unos);
    for (new i = 0; ime[i]; i++) if (ime[i] == ' ') ime[i] = '_';
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT a.id FROM accounts a JOIN social_accounts s ON s.account = a.id WHERE a.name = '%q'", ime), acc = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return acc;
}

static LsOdnos(a, b)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT a, accepted FROM social_friends WHERE (a = %d AND b = %d) OR (a = %d AND b = %d)", a, b, b, a), o = LS_NISTA;
    if (DB_GetRowCount(res)) o = DB_GetFieldInt(res, 1) ? LS_PRIJATELJ : DB_GetFieldInt(res, 0) == a ? LS_POSLAO : LS_PRIMIO;
    DB_FreeResultSet(res);
    return o;
}

static LsBroj(const upit[], acc)
{
    new DBResult:res = DB_ExecuteQuery(gDB, upit, acc, acc), n = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return n;
}

static LsPrije(playerid, vrijeme, out[], len)
{
    new m = (gettime() - vrijeme) / 60;
    if (m < 60) format(out, len, T(playerid, "prije %d min"), m);
    else if (m < 1440) format(out, len, T(playerid, "prije %d h"), m / 60);
    else format(out, len, T(playerid, "prije %d d"), m / 1440);
}

static LsRed(playerid, r, acc, podatak, const tekst[])
{
    gPcObjava[playerid][r] = podatak;
    PcPostavi(playerid, PTD_OBJAVA + r, tekst);
    PlayerTextDrawSetPreviewModel(playerid, gPc[playerid][PTD_LSAVATAR + r], LsSkin(acc));
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSAVATAR + r]);
    gLsAvatara[playerid] = r + 1;
}

static LsObjave(playerid, const uvjet[], acc, od)
{
    new red[256], tekst[OBJAVA_LEN + 1], ime[MAX_PLAYER_NAME + 1], vrijeme[24], upit[512];
    format(upit, sizeof upit, "SELECT p.id, p.account, p.text, replace(a.name, '_', ' '), p.created, (SELECT COUNT(*) FROM social_likes WHERE post = p.id), (SELECT COUNT(*) FROM social_comments WHERE post = p.id) FROM social_posts p JOIN accounts a ON a.id = p.account %s ORDER BY p.id DESC LIMIT %d", uvjet, OBJAVA_REDOVA - od);
    new DBResult:res = DB_ExecuteQuery(gDB, upit, acc), n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 2, tekst, sizeof tekst);
        DB_GetFieldString(res, 3, ime, sizeof ime);
        TdCisti(tekst);
        TdCisti(ime);
        LsPrije(playerid, DB_GetFieldInt(res, 4), vrijeme, sizeof vrijeme);
        format(red, sizeof red, T(playerid, "~b~%s  ~l~%s   ~r~+%d  ~l~%d komentara~n~~l~%s"), ime, vrijeme, DB_GetFieldInt(res, 5), DB_GetFieldInt(res, 6), tekst);
        LsRed(playerid, od + r, DB_GetFieldInt(res, 1), DB_GetFieldInt(res, 0), red);
    }
    DB_FreeResultSet(res);
    return n;
}

static LsAkcije(playerid, const prva[], const druga[] = "")
{
    PcPostavi(playerid, PTD_AKCIJA, prva, prva[0] != 0);
    PcPostavi(playerid, PTD_AKCIJA + 1, druga, druga[0] != 0);
}

static LsFeed(playerid)
{
    new n = LsObjave(playerid, "", 0, 0);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : T(playerid, "Jos nema objava. Budite prvi!"));
    LsAkcije(playerid, T(playerid, "Objavi"));
}

static LsObjava(playerid)
{
    new post = gLsCilj[playerid], red[256], tekst[OBJAVA_LEN + 1], ime[MAX_PLAYER_NAME + 1], vrijeme[24];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT p.account, p.text, replace(a.name, '_', ' '), p.created, (SELECT COUNT(*) FROM social_likes WHERE post = p.id), (SELECT COUNT(*) FROM social_likes WHERE post = p.id AND account = %d) FROM social_posts p JOIN accounts a ON a.id = p.account WHERE p.id = %d", gAccountId[playerid], post);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        gLsPogled[playerid] = LS_FEED;
        LsFeed(playerid);
        return 1;
    }
    new autor = DB_GetFieldInt(res, 0), bool:svida = DB_GetFieldInt(res, 5) > 0;
    DB_GetFieldString(res, 1, tekst, sizeof tekst);
    DB_GetFieldString(res, 2, ime, sizeof ime);
    TdCisti(tekst);
    TdCisti(ime);
    LsPrije(playerid, DB_GetFieldInt(res, 3), vrijeme, sizeof vrijeme);
    format(red, sizeof red, T(playerid, "~b~%s  ~l~%s   ~r~+%d~n~~l~%s"), ime, vrijeme, DB_GetFieldInt(res, 4), tekst);
    DB_FreeResultSet(res);
    LsRed(playerid, 0, autor, autor, red);
    res = DB_ExecuteQuery(gDB, "SELECT account, text, name, created FROM (SELECT c.id, c.account, c.text, replace(a.name, '_', ' ') AS name, c.created FROM social_comments c JOIN accounts a ON a.id = c.account WHERE c.post = %d ORDER BY c.id DESC LIMIT %d) ORDER BY id", post, OBJAVA_REDOVA - 1);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 1, tekst, sizeof tekst);
        DB_GetFieldString(res, 2, ime, sizeof ime);
        TdCisti(tekst);
        TdCisti(ime);
        LsPrije(playerid, DB_GetFieldInt(res, 3), vrijeme, sizeof vrijeme);
        format(red, sizeof red, "~p~%s  ~l~%s~n~~l~%s", ime, vrijeme, tekst);
        LsRed(playerid, r + 1, DB_GetFieldInt(res, 0), DB_GetFieldInt(res, 0), red);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, "_");
    LsAkcije(playerid, svida ? T(playerid, "Ne svidja mi se") : T(playerid, "Svidja mi se"), T(playerid, "Komentiraj"));
    return 1;
}

static LsPrijatelji(playerid)
{
    new me = gAccountId[playerid], red[160], ime[MAX_PLAYER_NAME + 1];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x.acc, x.k, replace(a.name, '_', ' ') FROM (SELECT a AS acc, 0 AS k FROM social_friends WHERE b = %d AND accepted = 0 UNION ALL SELECT CASE WHEN a = %d THEN b ELSE a END, 1 FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1) x JOIN accounts a ON a.id = x.acc ORDER BY x.k LIMIT %d", me, me, me, me, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0), bool:zahtjev = DB_GetFieldInt(res, 1) == 0;
        DB_GetFieldString(res, 2, ime, sizeof ime);
        TdCisti(ime);
        if (zahtjev) format(red, sizeof red, T(playerid, "~b~%s~n~~r~Zeli vas za prijatelja%s"), ime, gLsOdabran[playerid] == acc ? (" <") : (""));
        else format(red, sizeof red, "~b~%s~n~%s", ime, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"));
        LsRed(playerid, r, acc, zahtjev ? -acc : acc, red);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : T(playerid, "Jos nemate prijatelja."));
    if (gLsOdabran[playerid]) LsAkcije(playerid, T(playerid, "Prihvati"), T(playerid, "Odbij"));
    else LsAkcije(playerid, T(playerid, "Dodaj prijatelja"));
}

static LsPoruke(playerid)
{
    new me = gAccountId[playerid], red[256], ime[MAX_PLAYER_NAME + 1], tekst[48], vrijeme[24], nove[24];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT m.partner, replace(a.name, '_', ' '), substr(msg.text, 1, 44), msg.created, (SELECT COUNT(*) FROM social_messages WHERE sender = m.partner AND receiver = %d AND seen = 0) FROM (SELECT CASE WHEN sender = %d THEN receiver ELSE sender END AS partner, MAX(id) AS zadnja FROM social_messages WHERE sender = %d OR receiver = %d GROUP BY partner) m JOIN social_messages msg ON msg.id = m.zadnja JOIN accounts a ON a.id = m.partner ORDER BY m.zadnja DESC LIMIT %d", me, me, me, me, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0), broj = DB_GetFieldInt(res, 4);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        DB_GetFieldString(res, 2, tekst, sizeof tekst);
        TdCisti(ime);
        TdCisti(tekst);
        LsPrije(playerid, DB_GetFieldInt(res, 3), vrijeme, sizeof vrijeme);
        nove[0] = 0;
        if (broj) format(nove, sizeof nove, T(playerid, "~r~%d novih"), broj);
        format(red, sizeof red, "~b~%s  ~l~%s  %s~n~~l~%s", ime, vrijeme, nove, tekst);
        LsRed(playerid, r, acc, acc, red);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : T(playerid, "Jos nemate poruka."));
    LsAkcije(playerid, T(playerid, "Nova poruka"));
}

static LsRazgovor(playerid)
{
    new me = gAccountId[playerid], p = gLsCilj[playerid], red[256], tekst[OBJAVA_LEN + 1], ja[MAX_PLAYER_NAME + 1], on[MAX_PLAYER_NAME + 1], vrijeme[24];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE social_messages SET seen = 1 WHERE sender = %d AND receiver = %d AND seen = 0", p, me));
    LsIme(me, ja, sizeof ja);
    LsIme(p, on, sizeof on);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT sender, text, created FROM (SELECT id, sender, text, created FROM social_messages WHERE (sender = %d AND receiver = %d) OR (sender = %d AND receiver = %d) ORDER BY id DESC LIMIT %d) ORDER BY id", me, p, p, me, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new od = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, tekst, sizeof tekst);
        TdCisti(tekst);
        LsPrije(playerid, DB_GetFieldInt(res, 2), vrijeme, sizeof vrijeme);
        format(red, sizeof red, "%s%s  ~l~%s~n~~l~%s", od == me ? ("~g~") : ("~b~"), od == me ? ja : on, vrijeme, tekst);
        LsRed(playerid, r, od, 0, red);
    }
    DB_FreeResultSet(res);
    format(red, sizeof red, T(playerid, "Razgovor s: %s~n~~n~Jos nema poruka."), on);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : red);
    LsAkcije(playerid, T(playerid, "Posalji"), T(playerid, "Profil"));
}

static LsProfil(playerid)
{
    new acc = gLsCilj[playerid], me = gAccountId[playerid], ime[MAX_PLAYER_NAME + 1], tekst[256];
    LsIme(acc, ime, sizeof ime);
    new prijatelja = LsBroj("SELECT COUNT(*) FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1", acc);
    new objava = LsBroj("SELECT COUNT(*) FROM social_posts WHERE account = %d", acc);
    new lajkova = LsBroj("SELECT COUNT(*) FROM social_likes l JOIN social_posts p ON p.id = l.post WHERE p.account = %d", acc);
    format(tekst, sizeof tekst, T(playerid, "~b~%s~n~%s~n~~n~~l~Prijatelja: %d~n~Objava: %d~n~Svidjanja: %d"), ime, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"), prijatelja, objava, lajkova);
    PcPostavi(playerid, PTD_LSINFO, tekst);
    PcPostavi(playerid, PTD_SADRZAJ, "_");
    PlayerTextDrawSetPreviewModel(playerid, gPc[playerid][PTD_LSPROFIL], LsSkin(acc));
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPROFIL]);
    for (new k = 4; k <= 6; k++) PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPOKRIV + k]);
    LsObjave(playerid, "WHERE p.account = %d", acc, OBJAVA_REDOVA - 3);
    for (new k = OBJAVA_REDOVA - 3; k < OBJAVA_REDOVA; k++) PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSAVATAR + k]);
    gLsAvatara[playerid] = OBJAVA_REDOVA;
    if (acc == me)
    {
        LsAkcije(playerid, T(playerid, "Objavi"));
        return 1;
    }
    switch (LsOdnos(me, acc))
    {
        case LS_PRIJATELJ: LsAkcije(playerid, T(playerid, "Poruka"), T(playerid, "Ukloni prijatelja"));
        case LS_POSLAO: LsAkcije(playerid, T(playerid, "Poruka"), T(playerid, "Zahtjev poslan"));
        case LS_PRIMIO: LsAkcije(playerid, T(playerid, "Poruka"), T(playerid, "Prihvati"));
        default: LsAkcije(playerid, T(playerid, "Poruka"), T(playerid, "Dodaj prijatelja"));
    }
    return 1;
}

static bool:LsImaRacun(playerid)
{
    new me = gAccountId[playerid], DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM furniture WHERE owner = %d AND model = %d LIMIT 1", me, RACUNALO_MODEL);
    new bool:vlasnik = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    if (vlasnik) DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_accounts (account, created) VALUES (%d, %d)", me, gettime()));
    res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_accounts WHERE account = %d", me);
    new bool:ima = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return ima;
}

static PcLifeStalker(playerid)
{
    new me = gAccountId[playerid], tekst[32];
    for (new r = 0; r < OBJAVA_REDOVA; r++)
    {
        gPcObjava[playerid][r] = 0;
        PlayerTextDrawHide(playerid, gPc[playerid][PTD_OBJAVA + r]);
        PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSAVATAR + r]);
    }
    PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSPROFIL]);
    PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSINFO]);
    for (new k = 0; k < 8; k++)
    {
        if (k < 4) PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPOKRIV + k]);
        else PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSPOKRIV + k]);
    }
    gLsAvatara[playerid] = 0;
    PlayerTextDrawSetPos(playerid, gPc[playerid][PTD_SADRZAJ], PC_X + LS_SLOT_X + 46.0, PC_Y + 46.0);
    if (!LsImaRacun(playerid))
    {
        for (new k = 0; k < sizeof gLsTab; k++) PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSTAB + k]);
        PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "LifeStalker racun imaju samo~n~vlasnici racunala.~n~~n~Kupite racunalo u trgovini~n~namjestaja."));
        LsAkcije(playerid, "");
        return;
    }
    new zahtjeva = LsBroj("SELECT COUNT(*) FROM social_friends WHERE b = %d AND accepted = 0", me);
    new poruka = LsBroj("SELECT COUNT(*) FROM social_messages WHERE receiver = %d AND seen = 0", me);
    new aktivan = gLsPogled[playerid] == LS_OBJAVA ? 0 : gLsPogled[playerid] == LS_RAZGOVOR ? 2 : gLsPogled[playerid] == LS_PRIJATELJI ? 1 : gLsPogled[playerid] == LS_PORUKE ? 2 : gLsPogled[playerid] == LS_PROFIL ? 3 : 0;
    for (new k = 0; k < sizeof gLsTab; k++)
    {
        new broj = k == 1 ? zahtjeva : k == 2 ? poruka : 0;
        if (broj) format(tekst, sizeof tekst, "%s (%d)", T(playerid, gLsTab[k]), broj);
        else format(tekst, sizeof tekst, "%s", T(playerid, gLsTab[k]));
        PlayerTextDrawBoxColour(playerid, gPc[playerid][PTD_LSTAB + k], k == aktivan ? 0xE91E63FF : 0xEEEEEEFF);
        PlayerTextDrawColour(playerid, gPc[playerid][PTD_LSTAB + k], k == aktivan ? 0xFFFFFFFF : 0x202020FF);
        PcPostavi(playerid, PTD_LSTAB + k, tekst);
    }
    switch (gLsPogled[playerid])
    {
        case LS_OBJAVA: LsObjava(playerid);
        case LS_PRIJATELJI: LsPrijatelji(playerid);
        case LS_PORUKE: LsPoruke(playerid);
        case LS_RAZGOVOR: LsRazgovor(playerid);
        case LS_PROFIL: LsProfil(playerid);
        default: LsFeed(playerid);
    }
    new n = gLsAvatara[playerid];
    if (n >= OBJAVA_REDOVA) return;
    new PlayerText:pokriv = gPc[playerid][PTD_LSPOKRIV + 7];
    PlayerTextDrawSetPos(playerid, pokriv, PC_X + 140.0, PC_Y + LS_RED_Y + float(n) * LS_RED_H - 1.0);
    PlayerTextDrawTextSize(playerid, pokriv, 52.0, 207.0 - LS_RED_Y - float(n) * LS_RED_H);
    PlayerTextDrawShow(playerid, pokriv);
}


static LsOtvori(playerid, pogled, cilj = 0)
{
    gLsPogled[playerid] = pogled;
    gLsCilj[playerid] = cilj;
    gLsOdabran[playerid] = 0;
    PcLifeStalker(playerid);
    return 1;
}

static bool:LsPauza(playerid)
{
    new sad = gettime();
    if (sad - gLsZadnje[playerid] < LS_PAUZA)
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Pricekajte jos %d s."), LS_PAUZA - (sad - gLsZadnje[playerid]));
        return true;
    }
    gLsZadnje[playerid] = sad;
    return false;
}

static LsZahtjev(playerid, acc)
{
    new me = gAccountId[playerid];
    if (!acc || acc == me) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Korisnik nije pronadjen."));
    switch (LsOdnos(me, acc))
    {
        case LS_PRIJATELJ: return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vec ste prijatelji."));
        case LS_POSLAO: return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Zahtjev je vec poslan."));
        case LS_PRIMIO: return LsPrihvati(playerid, acc);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_friends (a, b, created) VALUES (%d, %d, %d)", me, acc, gettime()));
    new t = IgracRacuna(acc);
    if (t != INVALID_PLAYER_ID) SendClientMessage(t, 0xE91E63FF, T(t, "[LifeStalker] %s vam je poslao zahtjev za prijateljstvo."), GetRPIme(playerid));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Zahtjev za prijateljstvo poslan."));
}

static LsPrihvati(playerid, acc)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE social_friends SET accepted = 1 WHERE a = %d AND b = %d", acc, gAccountId[playerid]));
    new t = IgracRacuna(acc);
    if (t != INVALID_PLAYER_ID) SendClientMessage(t, 0xE91E63FF, T(t, "[LifeStalker] %s je prihvatio vas zahtjev za prijateljstvo."), GetRPIme(playerid));
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Sada ste prijatelji."));
}

static LsUkloni(playerid, acc)
{
    new me = gAccountId[playerid];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_friends WHERE (a = %d AND b = %d) OR (a = %d AND b = %d)", me, acc, acc, me));
}

static LsKlik(playerid, PlayerText:td)
{
    for (new k = 0; k < sizeof gLsTab; k++)
    {
        if (td != gPc[playerid][PTD_LSTAB + k]) continue;
        return LsOtvori(playerid, k == 1 ? LS_PRIJATELJI : k == 2 ? LS_PORUKE : k == 3 ? LS_PROFIL : LS_FEED, k == 3 ? gAccountId[playerid] : 0);
    }
    for (new r = 0; r < OBJAVA_REDOVA; r++)
    {
        new v = gPcObjava[playerid][r];
        if (td != gPc[playerid][PTD_OBJAVA + r] || !v) continue;
        switch (gLsPogled[playerid])
        {
            case LS_FEED: return LsOtvori(playerid, LS_OBJAVA, v);
            case LS_PROFIL: return LsOtvori(playerid, LS_OBJAVA, v);
            case LS_OBJAVA: return LsOtvori(playerid, LS_PROFIL, v);
            case LS_PORUKE: return LsOtvori(playerid, LS_RAZGOVOR, v);
            case LS_PRIJATELJI:
            {
                if (v > 0) return LsOtvori(playerid, LS_PROFIL, v);
                gLsOdabran[playerid] = gLsOdabran[playerid] == -v ? 0 : -v;
                PcLifeStalker(playerid);
                return 1;
            }
        }
        return 1;
    }
    for (new i = 0; i < 2; i++)
    {
        if (td != gPc[playerid][PTD_AKCIJA + i]) continue;
        new tekst[160], cilj = gLsCilj[playerid];
        switch (gLsPogled[playerid])
        {
            case LS_FEED: if (!i) LsUnosObjave(playerid);
            case LS_OBJAVA:
            {
                if (i)
                {
                    format(tekst, sizeof tekst, T(playerid, "Vas komentar (najvise %d znakova):"), OBJAVA_LEN);
                    return PcUnos(playerid, PC_KOMENTAR, tekst);
                }
                new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_likes WHERE post = %d AND account = %d", cilj, gAccountId[playerid]);
                new bool:ima = DB_GetRowCount(res) > 0;
                DB_FreeResultSet(res);
                if (ima) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_likes WHERE post = %d AND account = %d", cilj, gAccountId[playerid]));
                else DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_likes (post, account) VALUES (%d, %d)", cilj, gAccountId[playerid]));
                PcLifeStalker(playerid);
            }
            case LS_PRIJATELJI:
            {
                new acc = gLsOdabran[playerid];
                if (!acc) return PcUnos(playerid, PC_PRIJATELJ, T(playerid, "Unesite ime i prezime korisnika:"));
                if (i) LsUkloni(playerid, acc);
                else LsPrihvati(playerid, acc);
                gLsOdabran[playerid] = 0;
                PcLifeStalker(playerid);
            }
            case LS_PORUKE: if (!i) return PcUnos(playerid, PC_PORUKA_KOME, T(playerid, "Kome saljete poruku? (ime i prezime)"));
            case LS_RAZGOVOR:
            {
                if (i) return LsOtvori(playerid, LS_PROFIL, cilj);
                format(tekst, sizeof tekst, T(playerid, "Vasa poruka (najvise %d znakova):"), OBJAVA_LEN);
                return PcUnos(playerid, PC_PORUKA, tekst);
            }
            case LS_PROFIL:
            {
                if (cilj == gAccountId[playerid])
                {
                    if (!i) LsUnosObjave(playerid);
                    return 1;
                }
                if (!i) return LsOtvori(playerid, LS_RAZGOVOR, cilj);
                switch (LsOdnos(gAccountId[playerid], cilj))
                {
                    case LS_PRIJATELJ: LsUkloni(playerid, cilj);
                    case LS_POSLAO: return 1;
                    default: LsZahtjev(playerid, cilj);
                }
                PcLifeStalker(playerid);
            }
        }
        return 1;
    }
    return 0;
}

static LsUnosObjave(playerid)
{
    new tekst[96];
    format(tekst, sizeof tekst, T(playerid, "Sto vam je na umu? (najvise %d znakova)"), OBJAVA_LEN);
    return PcUnos(playerid, PC_OBJAVA, tekst);
}

static LsUnos(playerid, akcija, const inputtext[])
{
    new tekst[OBJAVA_LEN + 1], me = gAccountId[playerid], sad = gettime();
    switch (akcija)
    {
        case PC_PRIJATELJ:
        {
            LsZahtjev(playerid, LsRacun(inputtext));
            return 1;
        }
        case PC_PORUKA_KOME:
        {
            new acc = LsRacun(inputtext);
            if (!acc || acc == me) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Korisnik nije pronadjen."));
            gLsPogled[playerid] = LS_RAZGOVOR;
            gLsCilj[playerid] = acc;
            return 1;
        }
    }
    strcat(tekst, inputtext);
    RemoveBoje(tekst);
    if (!tekst[0] || LsPauza(playerid)) return 1;
    if (akcija == PC_KOMENTAR)
    {
        new post = gLsCilj[playerid], DBResult:res = DB_ExecuteQuery(gDB, "SELECT account FROM social_posts WHERE id = %d", post), autor = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
        DB_FreeResultSet(res);
        if (!autor) return 1;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_comments (post, account, text, created) VALUES (%d, %d, '%q', %d)", post, me, tekst, sad));
        new t = IgracRacuna(autor);
        if (autor != me && t != INVALID_PLAYER_ID)
        {
            new obavijest[144];
            format(obavijest, sizeof obavijest, T(t, "~p~LifeStalker~n~~w~%s je komentirao vasu objavu."), GetRPIme(playerid));
            PcObavijest(t, obavijest);
        }
        return 1;
    }
    new p = gLsCilj[playerid];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_messages (sender, receiver, text, created) VALUES (%d, %d, '%q', %d)", me, p, tekst, sad));
    new t = IgracRacuna(p);
    if (t == INVALID_PLAYER_ID || !gPcOtvoren[t]) return 1;
    if (gPcEkran[t] == PCE_LIFESTALKER && gLsPogled[t] == LS_RAZGOVOR && gLsCilj[t] == me)
    {
        PcEkran(t, PCE_LIFESTALKER);
        return 1;
    }
    new obavijest[200], kratko[48];
    strcat(kratko, tekst);
    TdCisti(kratko);
    format(obavijest, sizeof obavijest, T(t, "~p~LifeStalker~n~~w~Nova poruka od %s:~n~%s"), GetRPIme(playerid), kratko);
    PcObavijest(t, obavijest);
    if (gPcEkran[t] == PCE_LIFESTALKER && gLsPogled[t] == LS_PORUKE) PcEkran(t, PCE_LIFESTALKER);
    return 1;
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
    PcSakrij(playerid, PTD_LSTAB, 4);
    PcSakrij(playerid, PTD_LSAVATAR, PTD_KLAD - PTD_LSAVATAR);
    PcSakrij(playerid, PTD_KLAD, KLAD_TD);
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
                TdCisti(oglas);
                TdCisti(ime);
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
        case PCE_KLADIONICA:
        {
            KladEkran(playerid);
            return;
        }
    }
    PcPostavi(playerid, PTD_SADRZAJ, tekst);
}

RacunaloTick()
{
    new sad = GetTickCount();
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gPcOtvoren[i] || !gPcObavijestDo[i] || sad - gPcObavijestDo[i] < 0) continue;
        gPcObavijestDo[i] = 0;
        PlayerTextDrawHide(i, gPc[i][PTD_OBAVIJEST]);
        PlayerTextDrawHide(i, gPc[i][PTD_OBAVIJEST_TEKST]);
    }
    if (++gKladSat >= KLAD_PROVJERA_S)
    {
        gKladSat = 0;
        KladObracun();
    }
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
    VrijemeLokalno(g, m, d, sat, minuta);
    format(vrijeme, sizeof vrijeme, "%02d:%02d", sat, minuta);
    format(datum, sizeof datum, "%02d.%02d.%d", d, m, g);
    gPcOtvoren[playerid] = true;
    gPcBroj[playerid] = 0;
    gPcKolicina[playerid] = 1;
    gKladUlog[playerid] = 3;
    gKladMecId[playerid][0] = EOS;
    gLsPogled[playerid] = LS_FEED;
    gLsOdabran[playerid] = 0;
    PcUcitajKomade(playerid);
    PcKutija(playerid, 0.0, 0.0, 400.0, 248.0, 0x111111FF);
    PcKutija(playerid, 180.0, 248.0, 40.0, 8.0, 0x222222FF);
    PcKutija(playerid, 140.0, 256.0, 120.0, 4.0, 0x222222FF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 220.0, 0x0B4F8AFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 110.0, 0x1366ADFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 50.0, 0x1A7BC8FF);
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        new Float:y = 12.0 + float(i - 1) * 42.0, Float:x = 16.0;
        gPc[playerid][PTD_APP + i] = PcKutija(playerid, x, y, 30.0, 30.0, gPcIkonaBoja[i]);
        PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_APP + i], true);
        gPc[playerid][PTD_APPZNAK + i] = PcTekst(playerid, x + 15.0, y + 7.0, gPcIkona[i], 0xFFFFFFFF, 0.42, 1.7, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
        new PlayerText:ime = gPc[playerid][PTD_APPIME + i] = PcTekst(playerid, x + 15.0, y + 32.0, T(playerid, gPcAplikacije[i]), 0xFFFFFFFF, 0.15, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawSetShadow(playerid, ime, 1);
    }
    PcKutija(playerid, 4.0, 224.0, 392.0, 20.0, 0x0D141CF2);
    gPc[playerid][PTD_START] = PcKutija(playerid, 8.0, 226.0, 16.0, 16.0, 0x00000001);
    PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_START], true);
    for (new i = 0; i < 4; i++) PcKutija(playerid, 10.0 + float(i % 2) * 7.0, 228.0 + float(i / 2) * 7.0, 5.5, 5.5, 0x4FC3F7FF);
    gPc[playerid][PTD_TRAKA] = PcGumb(playerid, 70.0, 229.0, 70.0, 0xFFFFFF22);
    PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_TRAKA], false);
    gPc[playerid][PTD_SAT] = PcTekst(playerid, 390.0, 226.0, vrijeme, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    gPc[playerid][PTD_DATUM] = PcTekst(playerid, 390.0, 234.0, datum, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
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
    for (new r = 0; r < OBJAVA_REDOVA; r++) gPc[playerid][PTD_LSAVATAR + r] = PcModel(playerid, LS_SLOT_X - LS_AV_S / 2.0, LS_RED_Y + float(r) * LS_RED_H - 1.0, LS_AV_S, LS_AV_S);
    gPc[playerid][PTD_LSPROFIL] = PcModel(playerid, LS_PR_X - LS_PR_S / 2.0, 40.0, LS_PR_S, LS_PR_S);
    gPc[playerid][PTD_LSPOKRIV] = PcKutija(playerid, 80.0, 36.0, 60.0, 170.0, 0xE91E63FF);
    gPc[playerid][PTD_LSPOKRIV + 1] = PcKutija(playerid, 100.0, 206.0, 100.0, 3.0, 0x07355CFF);
    gPc[playerid][PTD_LSPOKRIV + 2] = PcKutija(playerid, 100.0, 209.0, 100.0, 15.0, 0x0B4F8AFF);
    gPc[playerid][PTD_LSPOKRIV + 3] = PcKutija(playerid, 100.0, 224.0, 100.0, 20.0, 0x0D141BFF);
    gPc[playerid][PTD_LSPOKRIV + 4] = PcKutija(playerid, 140.0, 36.0, LS_PR_X - 25.0 - 140.0, 170.0, 0xFFFFFFFF);
    gPc[playerid][PTD_LSPOKRIV + 5] = PcKutija(playerid, LS_PR_X + 25.0, 36.0, LS_PR_S / 2.0 - 25.0, 60.0, 0xFFFFFFFF);
    gPc[playerid][PTD_LSPOKRIV + 6] = PcKutija(playerid, 140.0, 96.0, LS_PR_X + LS_PR_S / 2.0 - 140.0, 110.0, 0xFFFFFFFF);
    gPc[playerid][PTD_LSPOKRIV + 7] = PcKutija(playerid, 140.0, 36.0, 52.0, 170.0, 0xFFFFFFFF);
    gPc[playerid][PTD_LSINFO] = PcTekst(playerid, LS_PR_X + 32.0, 46.0, "_", 0x202020FF, 0.18, 0.95, TEXT_DRAW_FONT_1);
    KladStvori(playerid);
    for (new r = 0; r < OBJAVA_REDOVA; r++) gPc[playerid][PTD_OBJAVA + r] = PcRed(playerid, LS_SLOT_X + 10.0, LS_RED_Y + float(r) * LS_RED_H, 374.0 - LS_SLOT_X - 10.0);
    for (new k = 0; k < sizeof gLsTab; k++) gPc[playerid][PTD_LSTAB + k] = PcGumb(playerid, 110.0, 46.0 + float(k) * 22.0, 56.0, 0xEEEEEEFF, "_", 0x202020FF);
    gPc[playerid][PTD_KOL] = PcGumb(playerid, 168.0, 174.0, 14.0, 0x0078D4FF, "~<~");
    gPc[playerid][PTD_KOL + 1] = PcGumb(playerid, 230.0, 174.0, 100.0, 0xDDDDDDFF, "_", 0x202020FF);
    gPc[playerid][PTD_KOL + 2] = PcGumb(playerid, 292.0, 174.0, 14.0, 0x0078D4FF, "~>~");
    for (new i = 0; i < 2; i++) gPc[playerid][PTD_AKCIJA + i] = PcGumb(playerid, 180.0 + float(i) * 100.0, 190.0, 90.0, 0x0078D4FF);
    gPc[playerid][PTD_START_PANEL] = PcKutija(playerid, 4.0, 164.0, 130.0, 60.0, 0x1B2430F2);
    gPc[playerid][PTD_START_PANEL + 1] = PcTekst(playerid, 12.0, 170.0, GetRPIme(playerid), 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_UGASI] = PcGumb(playerid, 69.0, 204.0, 110.0, 0x2C3746FF, T(playerid, "Ugasi"));
    for (new i = 0; i < gPcBroj[playerid]; i++) PlayerTextDrawShow(playerid, gPcTd[playerid][i]);
    gPc[playerid][PTD_OBAVIJEST] = PcKutija(playerid, 262.0, 190.0, 130.0, 30.0, 0x1B2430F2);
    gPc[playerid][PTD_OBAVIJEST_TEKST] = PcTekst(playerid, 267.0, 193.0, "_", 0xFFFFFFFF, 0.14, 0.7, TEXT_DRAW_FONT_1);
    PlayerTextDrawTextSize(playerid, gPc[playerid][PTD_OBAVIJEST_TEKST], PC_X + 388.0, 0.0);
    gPcObavijestDo[playerid] = 0;
    PcEkran(playerid, PCE_POCETNA);
    SelectTextDraw(playerid, 0x4FC3F7FF);
}

RacunaloSat(const vrijeme[], const datum[])
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (!gPcOtvoren[i]) continue;
        PlayerTextDrawSetString(i, gPc[i][PTD_SAT], vrijeme);
        PlayerTextDrawSetString(i, gPc[i][PTD_DATUM], datum);
    }
}

static PcObavijest(playerid, const tekst[])
{
    if (!gPcOtvoren[playerid]) return;
    PlayerTextDrawSetString(playerid, gPc[playerid][PTD_OBAVIJEST_TEKST], tekst);
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_OBAVIJEST]);
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_OBAVIJEST_TEKST]);
    PlayerPlaySound(playerid, 1054, 0.0, 0.0, 0.0);
    gPcObavijestDo[playerid] = GetTickCount() + PC_OBAVIJEST_MS;
}

PcZatvori(playerid)
{
    if (!gPcOtvoren[playerid]) return;
    gPcOtvoren[playerid] = false;
    for (new i = 0; i < gPcBroj[playerid]; i++) PlayerTextDrawDestroy(playerid, gPcTd[playerid][i]);
    gPcBroj[playerid] = 0;
    CancelSelectTextDraw(playerid);
}

RacunaloKaficIstek(playerid)
{
    if (!gPcOtvoren[playerid] || NamjestajModelBlizu(playerid, RACUNALO_MODEL, RACUNALO_DOMET)) return 0;
    PcZatvori(playerid);
    return 1;
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
    if (!PcBlizu(playerid)) return SendClientMessage(playerid, COLOR_ERROR, KaficRacunalo(playerid, false) ? T(playerid, "Platite vrijeme na blagajni internet kafica.") : T(playerid, "Niste kod racunala."));
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
    if (gPcEkran[playerid] == PCE_KLADIONICA)
    {
        if (KladKlik(playerid, td)) PcEkran(playerid, PCE_KLADIONICA);
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
    if (gPcEkran[playerid] == PCE_LIFESTALKER && LsKlik(playerid, td)) return 1;
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
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stock_holdings (account, stock, shares, cost) VALUES (%d, %d, %d, %d)", gAccountId[playerid], d, ima + n, gPcTrosak[playerid][d] + iznos));
        gPcKomadaPamti[playerid][d] = ima + n;
        gPcTrosak[playerid][d] += iznos;
        return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Kupili ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
    }
    if (n > ima) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate toliko dionica."));
    new iznos = DionicaProdaja(n, cijena);
    if (!BankaPromijeni(playerid, iznos)) return 1;
    if (ima == n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d));
    gPcTrosak[playerid][d] = ima == n ? 0 : gPcTrosak[playerid][d] - gPcTrosak[playerid][d] * n / ima;
    if (ima != n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET shares = %d, cost = %d WHERE account = %d AND stock = %d", ima - n, gPcTrosak[playerid][d], gAccountId[playerid], d));
    gPcKomadaPamti[playerid][d] = ima - n;
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Prodali ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
}

static KladKoef(o, out[], size)
{
    if (o > 0) format(out, size, "%d.%02d", o / 100, o % 100);
    else format(out, size, "-");
}

static KladLinija(l, out[], size, bool:znak = true)
{
    new a = l < 0 ? -l : l;
    format(out, size, "%s%d.%d", l < 0 ? ("-") : znak ? ("+") : (""), a / 10, a % 10);
}

static KladTip(tip, linija, out[], size)
{
    new l[12];
    switch (tip)
    {
        case 1: format(out, size, "1");
        case 2: format(out, size, "X");
        case 3: format(out, size, "2");
        case 4, 5:
        {
            KladLinija(linija, l, sizeof l, false);
            format(out, size, "%s%s", tip == 4 ? ("O") : ("U"), l);
        }
        default:
        {
            KladLinija(linija, l, sizeof l);
            format(out, size, "%s%s", tip == 6 ? ("D") : ("G"), l);
        }
    }
}

static KladNaListicu(playerid, const id[])
{
    for (new k = 0; k < gKladN[playerid]; k++) if (!strcmp(gKladPar[playerid][k], id)) return k;
    return -1;
}

static KladUkupno(playerid)
{
    new koef = 100;
    for (new k = 0; k < gKladN[playerid]; k++) koef = koef * gKladKoefPar[playerid][k] / 100;
    return koef > KLAD_MAX_KOEF ? KLAD_MAX_KOEF : koef;
}

static KladDobitak(ulog, koef)
{
    new d = ulog * (koef / 100) + ulog * (koef % 100) / 100;
    return d > KLAD_MAX_DOBITAK ? KLAD_MAX_DOBITAK : d;
}

static KladBoja(playerid, t, boja, tekst = 0x202020FF)
{
    PlayerTextDrawBoxColour(playerid, gPc[playerid][PTD_KLAD + t], boja);
    PlayerTextDrawColour(playerid, gPc[playerid][PTD_KLAD + t], tekst);
}

static KladStvori(playerid)
{
    gPc[playerid][PTD_KLAD + KT_BOK] = PcKutija(playerid, 80.0, 50.0, 58.0, 156.0, KLAD_NAVY);
    gPc[playerid][PTD_KLAD + KT_LISTIC] = PcKutija(playerid, 302.0, 36.0, 78.0, 170.0, KLAD_NAVY);
    for (new t = 0; t < 3; t++) gPc[playerid][PTD_KLAD + KT_TAB + t] = PcGumb(playerid, 114.0 + float(t) * 58.0, 39.0, 52.0, KLAD_NAVY2, T(playerid, gKladTabovi[t]));
    for (new k = 0; k < sizeof gKladKategorije; k++) gPc[playerid][PTD_KLAD + KT_KAT + k] = PcGumb(playerid, 109.0, 55.0 + float(k) * 16.0, 54.0, KLAD_NAVY2, T(playerid, gKladKategorije[k]));
    for (new l = 0; l < sizeof gKladLige; l++)
    {
        new PlayerText:td = gPc[playerid][PTD_KLAD + KT_LIGA + l] = PcGumb(playerid, 153.0 + float(l) * 22.3, 52.0, 20.0, KLAD_SIVO, gKladLige[l], 0x202020FF);
        PlayerTextDrawLetterSize(playerid, td, 0.11, 0.6);
    }
    gPc[playerid][PTD_KLAD + KT_MEC] = PcTekst(playerid, 144.0, 51.0, "_", 0x202020FF, 0.14, 0.7, TEXT_DRAW_FONT_1);
    for (new r = 0; r < KLAD_RED; r++)
    {
        new PlayerText:td = gPc[playerid][PTD_KLAD + KT_RED + r] = PcRed(playerid, 142.0, 64.0 + float(r) * 30.0, 156.0);
        PlayerTextDrawLetterSize(playerid, td, 0.14, 0.7);
        PlayerTextDrawBoxColour(playerid, td, 0xF4F6F9FF);
        for (new o = 0; o < 3; o++)
        {
            td = gPc[playerid][PTD_KLAD + KT_KOEF + r * 3 + o] = PcGumb(playerid, 168.0 + float(o) * 53.0, 78.0 + float(r) * 30.0, 49.0, KLAD_SIVO, "_", 0x202020FF);
            PlayerTextDrawLetterSize(playerid, td, 0.14, 0.72);
        }
    }
    gPc[playerid][PTD_KLAD + KT_STR_L] = PcGumb(playerid, 203.0, 192.0, 12.0, 0x0078D4FF, "~<~");
    gPc[playerid][PTD_KLAD + KT_STR] = PcGumb(playerid, 221.0, 192.0, 18.0, 0xDDDDDDFF, "_", 0x202020FF);
    gPc[playerid][PTD_KLAD + KT_STR_D] = PcGumb(playerid, 239.0, 192.0, 12.0, 0x0078D4FF, "~>~");
    gPc[playerid][PTD_KLAD + KT_NASLOV] = PcTekst(playerid, 306.0, 39.0, "_", KLAD_ZUTO, 0.17, 0.85, TEXT_DRAW_FONT_2);
    for (new k = 0; k < KLAD_PAROVA; k++)
    {
        new PlayerText:td = gPc[playerid][PTD_KLAD + KT_PAR + k] = PcRed(playerid, 304.0, 52.0 + float(k) * 12.0, 74.0);
        PlayerTextDrawLetterSize(playerid, td, 0.12, 0.6);
        PlayerTextDrawBoxColour(playerid, td, KLAD_NAVY2);
        PlayerTextDrawColour(playerid, td, 0xFFFFFFFF);
    }
    gPc[playerid][PTD_KLAD + KT_UKUPNO] = PcTekst(playerid, 306.0, 150.0, "_", 0xFFFFFFFF, 0.14, 0.7, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_KLAD + KT_MINUS] = PcGumb(playerid, 312.0, 162.0, 12.0, KLAD_NAVY2, "-");
    gPc[playerid][PTD_KLAD + KT_ULOG] = PcTekst(playerid, 341.0, 163.0, "_", 0xFFFFFFFF, 0.16, 0.8, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    gPc[playerid][PTD_KLAD + KT_PLUS] = PcGumb(playerid, 370.0, 162.0, 12.0, KLAD_NAVY2, "+");
    gPc[playerid][PTD_KLAD + KT_DOBITAK] = PcTekst(playerid, 306.0, 176.0, "_", KLAD_ZUTO, 0.14, 0.7, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_KLAD + KT_UPLATI] = PcGumb(playerid, 341.0, 190.0, 70.0, KLAD_ZUTO, "_", 0x0E2A47FF);
}

static KladPokazi(playerid, t, const tekst[] = "")
{
    if (tekst[0]) PlayerTextDrawSetString(playerid, gPc[playerid][PTD_KLAD + t], tekst);
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_KLAD + t]);
}

static KladGumb(playerid, r, o, const tekst[], koef, bool:odabrano)
{
    KladBoja(playerid, KT_KOEF + r * 3 + o, odabrano ? KLAD_ZUTO : KLAD_SIVO, koef > 100 ? 0x202020FF : 0x9AA3ADFF);
    KladPokazi(playerid, KT_KOEF + r * 3 + o, tekst);
}

static KladSlip(playerid)
{
    new red[96], tekst[64], k[12], tip[12], ukupno = KladUkupno(playerid);
    KladPokazi(playerid, KT_NASLOV, T(playerid, "LISTIC"));
    for (new p = 0; p < gKladN[playerid]; p++)
    {
        KladKoef(gKladKoefPar[playerid][p], k, sizeof k);
        KladTip(gKladTip[playerid][p], gKladLin[playerid][p], tip, sizeof tip);
        format(red, sizeof red, "%s ~y~%s ~w~%s", gKladParIme[playerid][p], tip, k);
        KladPokazi(playerid, KT_PAR + p, red);
    }
    if (!gKladN[playerid]) KladPokazi(playerid, KT_PAR, T(playerid, "Prazan listic"));
    format(tekst, sizeof tekst, T(playerid, "Koef. %d.%02d"), ukupno / 100, ukupno % 100);
    KladPokazi(playerid, KT_UKUPNO, tekst);
    format(tekst, sizeof tekst, "$%d", gKladUlozi[gKladUlog[playerid]]);
    KladPokazi(playerid, KT_ULOG, tekst);
    KladPokazi(playerid, KT_MINUS);
    KladPokazi(playerid, KT_PLUS);
    format(tekst, sizeof tekst, T(playerid, "Dobitak: $%d"), gKladN[playerid] ? KladDobitak(gKladUlozi[gKladUlog[playerid]], ukupno) : 0);
    KladPokazi(playerid, KT_DOBITAK, tekst);
    KladPokazi(playerid, KT_UPLATI, T(playerid, "UPLATI"));
}

static KladPonuda(playerid)
{
    new red[160], liga[12], dom[32], gost[32], g, m, d, sat, minuta, k[12], gumb[24], uvjet[96];
    KladPokazi(playerid, KT_BOK);
    for (new c = 0; c < sizeof gKladKategorije; c++)
    {
        KladBoja(playerid, KT_KAT + c, c == gKladKat[playerid] ? KLAD_ZUTO : KLAD_NAVY2, c == gKladKat[playerid] ? 0x0E2A47FF : 0xFFFFFFFF);
        KladPokazi(playerid, KT_KAT + c);
    }
    if (gKladKat[playerid] == 1)
    {
        for (new l = 0; l < sizeof gKladLige; l++)
        {
            KladBoja(playerid, KT_LIGA + l, l == gKladLiga[playerid] ? KLAD_ZUTO : KLAD_SIVO);
            KladPokazi(playerid, KT_LIGA + l);
        }
    }
    if (gKladKat[playerid] == 1 && gKladLiga[playerid] != -1) format(uvjet, sizeof uvjet, "league = '%s'", gKladLige[gKladLiga[playerid]]);
    else format(uvjet, sizeof uvjet, "%s", gKladKatSql[gKladKat[playerid]]);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, league, home, away, start, odd1, oddx, odd2 FROM bet_events WHERE status = 0 AND start > %d AND odd1 > 0 AND odd2 > 0 AND %s ORDER BY start, id LIMIT %d OFFSET %d", gettime() + KLAD_ZATVARA_S, uvjet, KLAD_RED + 1, gKladStr[playerid] * KLAD_RED);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < KLAD_RED && r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 0, gKladRed[playerid][r], 16);
        DB_GetFieldString(res, 1, liga, sizeof liga);
        DB_GetFieldString(res, 2, dom, sizeof dom);
        DB_GetFieldString(res, 3, gost, sizeof gost);
        dom[16] = gost[16] = EOS;
        TdCisti(dom);
        TdCisti(gost);
        VrijemeTs(DB_GetFieldInt(res, 4), g, m, d, sat, minuta);
        format(red, sizeof red, "~b~%s ~l~%02d.%02d. %02d:%02d~n~%s - %s", liga, d, m, sat, minuta, dom, gost);
        KladPokazi(playerid, KT_RED + r, red);
        format(gKladRedIme[playerid][r], 24, "%.10s-%.10s", dom, gost);
        new na = KladNaListicu(playerid, gKladRed[playerid][r]);
        for (new o = 0; o < 3; o++)
        {
            gKladRedKoef[playerid][r][o] = DB_GetFieldInt(res, 5 + o);
            KladKoef(gKladRedKoef[playerid][r][o], k, sizeof k);
            format(gumb, sizeof gumb, "%s  %s", o == 0 ? ("1") : o == 1 ? ("X") : ("2"), k);
            KladGumb(playerid, r, o, gumb, gKladRedKoef[playerid][r][o], na != -1 && gKladTip[playerid][na] == o + 1);
        }
    }
    DB_FreeResultSet(res);
    gKladIma[playerid] = n > KLAD_RED;
    for (new r = n; r < KLAD_RED; r++) gKladRed[playerid][r][0] = EOS;
    if (!n) KladPokazi(playerid, KT_RED, T(playerid, "Trenutno nema dostupnih utakmica."));
}

static KladMec(playerid)
{
    new red[160], liga[12], dom[32], gost[32], g, m, d, sat, minuta, k[12], l[12], gumb[32];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT league, home, away, start, odd1, oddx, odd2, tline, odd_over, odd_under, sline, odd_sph, odd_spa FROM bet_events WHERE id = '%q' AND status = 0 AND start > %d", gKladMecId[playerid], gettime() + KLAD_ZATVARA_S);
    if (!DB_GetRowCount(res))
    {
        DB_FreeResultSet(res);
        gKladMecId[playerid][0] = EOS;
        KladPonuda(playerid);
        return;
    }
    KladPokazi(playerid, KT_BOK);
    for (new c = 0; c < sizeof gKladKategorije; c++)
    {
        KladBoja(playerid, KT_KAT + c, c == gKladKat[playerid] ? KLAD_ZUTO : KLAD_NAVY2, c == gKladKat[playerid] ? 0x0E2A47FF : 0xFFFFFFFF);
        KladPokazi(playerid, KT_KAT + c);
    }
    DB_GetFieldString(res, 0, liga, sizeof liga);
    DB_GetFieldString(res, 1, dom, sizeof dom);
    DB_GetFieldString(res, 2, gost, sizeof gost);
    TdCisti(dom);
    TdCisti(gost);
    VrijemeTs(DB_GetFieldInt(res, 3), g, m, d, sat, minuta);
    format(red, sizeof red, "~b~%s ~l~%02d.%02d. %02d:%02d  %s - %s", liga, d, m, sat, minuta, dom, gost);
    KladPokazi(playerid, KT_MEC, red);
    dom[16] = gost[16] = EOS;
    format(gKladRedIme[playerid][0], 24, "%.10s-%.10s", dom, gost);
    new o[3][3], lin[3], na = KladNaListicu(playerid, gKladMecId[playerid]), odabran = na == -1 ? 0 : gKladTip[playerid][na];
    for (new i = 0; i < 3; i++) o[0][i] = DB_GetFieldInt(res, 4 + i);
    lin[1] = DB_GetFieldInt(res, 7);
    o[1][0] = DB_GetFieldInt(res, 8);
    o[1][1] = DB_GetFieldInt(res, 9);
    lin[2] = DB_GetFieldInt(res, 10);
    o[2][0] = DB_GetFieldInt(res, 11);
    o[2][1] = DB_GetFieldInt(res, 12);
    DB_FreeResultSet(res);
    for (new r = 0; r < 3; r++) for (new i = 0; i < 3; i++) gKladRedKoef[playerid][r][i] = o[r][i];
    gKladMecLin[playerid][1] = lin[1];
    gKladMecLin[playerid][2] = lin[2];
    KladPokazi(playerid, KT_RED, T(playerid, "Konacan ishod"));
    for (new i = 0; i < 3; i++)
    {
        KladKoef(o[0][i], k, sizeof k);
        format(gumb, sizeof gumb, "%s  %s", i == 0 ? ("1") : i == 1 ? ("X") : ("2"), k);
        KladGumb(playerid, 0, i, gumb, o[0][i], odabran == i + 1);
    }
    if (lin[1])
    {
        KladLinija(lin[1], l, sizeof l, false);
        format(red, sizeof red, T(playerid, "Ukupno (%s)"), l);
        KladPokazi(playerid, KT_RED + 1, red);
        KladKoef(o[1][0], k, sizeof k);
        format(gumb, sizeof gumb, T(playerid, "Vise  %s"), k);
        KladGumb(playerid, 1, 0, gumb, o[1][0], odabran == 4);
        KladKoef(o[1][1], k, sizeof k);
        format(gumb, sizeof gumb, T(playerid, "Manje  %s"), k);
        KladGumb(playerid, 1, 1, gumb, o[1][1], odabran == 5);
    }
    if (o[2][0])
    {
        KladLinija(lin[2], l, sizeof l);
        format(red, sizeof red, T(playerid, "Hendikep (domacin %s)"), l);
        KladPokazi(playerid, KT_RED + 2, red);
        KladKoef(o[2][0], k, sizeof k);
        format(gumb, sizeof gumb, "%s %s  %s", T(playerid, "Dom"), l, k);
        KladGumb(playerid, 2, 0, gumb, o[2][0], odabran == 6);
        KladLinija(-lin[2], l, sizeof l);
        KladKoef(o[2][1], k, sizeof k);
        format(gumb, sizeof gumb, "%s %s  %s", T(playerid, "Gost"), l, k);
        KladGumb(playerid, 2, 1, gumb, o[2][1], odabran == 7);
    }
}

static KladListici(playerid)
{
    new red[160], status[24];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT s.id, s.stake, s.odds, s.payout, s.status, (SELECT COUNT(*) FROM bet_legs WHERE slip = s.id), (SELECT COUNT(*) FROM bet_legs l JOIN bet_events e ON e.id = l.event WHERE l.slip = s.id AND (e.status = 3 OR (e.status = 2 AND (%s OR %s)))) FROM bet_slips s WHERE s.account = %d AND s.status %s 0 ORDER BY s.id DESC LIMIT %d OFFSET %d", KLAD_OK, KLAD_PUSH, gAccountId[playerid], gKladTab[playerid] == 1 ? ("=") : ("!="), KLAD_RED + 1, gKladStr[playerid] * KLAD_RED);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < KLAD_RED && r < n; r++, DB_SelectNextRow(res))
    {
        gKladRedListic[playerid][r] = DB_GetFieldInt(res, 0);
        switch (DB_GetFieldInt(res, 4))
        {
            case 1: format(status, sizeof status, "~g~%s", T(playerid, "DOBITNI"));
            case 2: format(status, sizeof status, "~r~%s", T(playerid, "GUBITNI"));
            default: format(status, sizeof status, "~y~%s", T(playerid, "U TIJEKU"));
        }
        new koef = DB_GetFieldInt(res, 2);
        format(red, sizeof red, T(playerid, "~b~#%d ~l~%d/%d  koef. %d.%02d~n~ulog $%d  dobitak $%d  %s"), gKladRedListic[playerid][r], DB_GetFieldInt(res, 6), DB_GetFieldInt(res, 5), koef / 100, koef % 100, DB_GetFieldInt(res, 1), DB_GetFieldInt(res, 3), status);
        KladBoja(playerid, KT_RED + r, gKladRedListic[playerid][r] == gKladDetalj[playerid] ? 0xFFF4C2FF : 0xF4F6F9FF);
        KladPokazi(playerid, KT_RED + r, red);
    }
    DB_FreeResultSet(res);
    gKladIma[playerid] = n > KLAD_RED;
    for (new r = n; r < KLAD_RED; r++) gKladRedListic[playerid][r] = 0;
    if (!n) KladPokazi(playerid, KT_RED, T(playerid, "Nemate uplacenih listica."));
    if (!gKladDetalj[playerid])
    {
        KladPokazi(playerid, KT_NASLOV, T(playerid, "LISTIC"));
        KladPokazi(playerid, KT_PAR, T(playerid, "Odaberite listic"));
        return;
    }
    format(red, sizeof red, T(playerid, "LISTIC #%d"), gKladDetalj[playerid]);
    KladPokazi(playerid, KT_NASLOV, red);
    new dom[32], gost[32], k[12], tip[12], p = 0;
    res = DB_ExecuteQuery(gDB, "SELECT e.home, e.away, l.pick, l.odd, e.status, %s, %s, l.line FROM bet_legs l JOIN bet_events e ON e.id = l.event WHERE l.slip = %d AND (SELECT account FROM bet_slips WHERE id = %d) = %d LIMIT %d", KLAD_OK, KLAD_PUSH, gKladDetalj[playerid], gKladDetalj[playerid], gAccountId[playerid], KLAD_PAROVA);
    for (new nr = DB_GetRowCount(res); p < nr; p++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 0, dom, sizeof dom);
        DB_GetFieldString(res, 1, gost, sizeof gost);
        TdCisti(dom);
        TdCisti(gost);
        new st = DB_GetFieldInt(res, 4), ok = DB_GetFieldInt(res, 5), push = DB_GetFieldInt(res, 6);
        KladKoef(DB_GetFieldInt(res, 3), k, sizeof k);
        KladTip(DB_GetFieldInt(res, 2), DB_GetFieldInt(res, 7), tip, sizeof tip);
        format(red, sizeof red, "%.8s-%.8s ~y~%s ~w~%s %s", dom, gost, tip, k, st == 3 || (st == 2 && push) ? ("~b~-") : st != 2 ? ("~y~...") : ok ? ("~g~OK") : ("~r~X"));
        KladPokazi(playerid, KT_PAR + p, red);
    }
    DB_FreeResultSet(res);
}

static KladEkran(playerid)
{
    PcPostavi(playerid, PTD_SADRZAJ, "_", false);
    KladPokazi(playerid, KT_LISTIC);
    for (new t = 0; t < 3; t++)
    {
        KladBoja(playerid, KT_TAB + t, t == gKladTab[playerid] ? KLAD_ZUTO : KLAD_NAVY2, t == gKladTab[playerid] ? 0x0E2A47FF : 0xFFFFFFFF);
        KladPokazi(playerid, KT_TAB + t);
    }
    if (gKladTab[playerid]) KladListici(playerid);
    else
    {
        if (gKladMecId[playerid][0]) KladMec(playerid);
        else KladPonuda(playerid);
        KladSlip(playerid);
    }
    if (!gKladTab[playerid] && gKladMecId[playerid][0])
    {
        KladPokazi(playerid, KT_STR, T(playerid, "Natrag"));
        return;
    }
    new tekst[16];
    format(tekst, sizeof tekst, "%d", gKladStr[playerid] + 1);
    KladPokazi(playerid, KT_STR_L);
    KladPokazi(playerid, KT_STR, tekst);
    KladPokazi(playerid, KT_STR_D);
}

static KladDodaj(playerid, const id[], const ime[], tip, koef, linija)
{
    if (koef <= 100) return;
    new k = KladNaListicu(playerid, id);
    if (k != -1 && gKladTip[playerid][k] == tip)
    {
        KladMakni(playerid, k);
        return;
    }
    if (k == -1)
    {
        if (gKladN[playerid] >= KLAD_PAROVA)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Na listicu moze biti najvise %d parova."), KLAD_PAROVA);
            return;
        }
        k = gKladN[playerid]++;
        strcopy(gKladPar[playerid][k], id, 16);
        strcopy(gKladParIme[playerid][k], ime, 24);
    }
    gKladTip[playerid][k] = tip;
    gKladKoefPar[playerid][k] = koef;
    gKladLin[playerid][k] = linija;
}

static KladMakni(playerid, k)
{
    if (k >= gKladN[playerid]) return;
    gKladN[playerid]--;
    strcopy(gKladPar[playerid][k], gKladPar[playerid][gKladN[playerid]], 16);
    strcopy(gKladParIme[playerid][k], gKladParIme[playerid][gKladN[playerid]], 24);
    gKladTip[playerid][k] = gKladTip[playerid][gKladN[playerid]];
    gKladKoefPar[playerid][k] = gKladKoefPar[playerid][gKladN[playerid]];
    gKladLin[playerid][k] = gKladLin[playerid][gKladN[playerid]];
}

static KladUplati(playerid)
{
    if (!gKladN[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Listic je prazan. Odaberite utakmice."));
    new ulog = gKladUlozi[gKladUlog[playerid]], koef = 100;
    for (new i = 0; i < gKladN[playerid]; i++)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT CASE %d WHEN 1 THEN odd1 WHEN 2 THEN oddx WHEN 3 THEN odd2 WHEN 4 THEN odd_over WHEN 5 THEN odd_under WHEN 6 THEN odd_sph ELSE odd_spa END, CASE %d WHEN 4 THEN tline WHEN 5 THEN tline WHEN 6 THEN sline WHEN 7 THEN -sline ELSE 0 END FROM bet_events WHERE id = '%q' AND status = 0 AND start > %d", gKladTip[playerid][i], gKladTip[playerid][i], gKladPar[playerid][i], gettime() + KLAD_ZATVARA_S);
        new o = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0, linija = DB_GetRowCount(res) ? DB_GetFieldInt(res, 1) : 0;
        DB_FreeResultSet(res);
        if (o <= 100) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Jedna od utakmica je vec pocela. Uklonite je s listica."));
        gKladKoefPar[playerid][i] = o;
        gKladLin[playerid][i] = linija;
        koef = koef * o / 100;
    }
    if (koef > KLAD_MAX_KOEF) koef = KLAD_MAX_KOEF;
    new dobitak = KladDobitak(ulog, koef);
    if (!BankaPromijeni(playerid, -ulog)) return 1;
    BlagajnaDodaj(ulog);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bet_slips (account, stake, odds, payout, status, created) VALUES (%d, %d, %d, %d, 0, %d)", gAccountId[playerid], ulog, koef, dobitak, gettime()));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT last_insert_rowid()");
    new listic = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    for (new i = 0; i < gKladN[playerid]; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bet_legs (slip, event, pick, odd, line) VALUES (%d, '%q', %d, %d, %d)", listic, gKladPar[playerid][i], gKladTip[playerid][i], gKladKoefPar[playerid][i], gKladLin[playerid][i]));
    gKladN[playerid] = 0;
    return SendClientMessage(playerid, COLOR_INFO, T(playerid, "Listic #%d uplacen: ulog $%d, moguci dobitak $%d."), listic, ulog, dobitak);
}

static KladKlik(playerid, PlayerText:td)
{
    for (new t = 0; t < 3; t++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_TAB + t]) continue;
        gKladTab[playerid] = t;
        gKladStr[playerid] = 0;
        gKladDetalj[playerid] = 0;
        gKladMecId[playerid][0] = EOS;
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_STR] && gKladMecId[playerid][0])
    {
        gKladMecId[playerid][0] = EOS;
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_STR_L])
    {
        if (gKladStr[playerid] > 0) gKladStr[playerid]--;
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_STR_D])
    {
        if (gKladIma[playerid]) gKladStr[playerid]++;
        return 1;
    }
    if (gKladTab[playerid])
    {
        for (new r = 0; r < KLAD_RED; r++)
        {
            if (td != gPc[playerid][PTD_KLAD + KT_RED + r] || !gKladRedListic[playerid][r]) continue;
            gKladDetalj[playerid] = gKladRedListic[playerid][r];
            return 1;
        }
        return 0;
    }
    for (new c = 0; c < sizeof gKladKategorije; c++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_KAT + c]) continue;
        gKladKat[playerid] = c;
        gKladLiga[playerid] = -1;
        gKladStr[playerid] = 0;
        gKladMecId[playerid][0] = EOS;
        return 1;
    }
    for (new l = 0; l < sizeof gKladLige; l++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_LIGA + l]) continue;
        gKladLiga[playerid] = gKladLiga[playerid] == l ? -1 : l;
        gKladStr[playerid] = 0;
        return 1;
    }
    for (new i = 0; i < KLAD_RED * 3; i++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_KOEF + i]) continue;
        new r = i / 3, o = i % 3;
        if (gKladMecId[playerid][0])
        {
            if (r == 0) KladDodaj(playerid, gKladMecId[playerid], gKladRedIme[playerid][0], o + 1, gKladRedKoef[playerid][0][o], 0);
            else if (r == 1 && o < 2) KladDodaj(playerid, gKladMecId[playerid], gKladRedIme[playerid][0], 4 + o, gKladRedKoef[playerid][1][o], gKladMecLin[playerid][1]);
            else if (r == 2 && o < 2) KladDodaj(playerid, gKladMecId[playerid], gKladRedIme[playerid][0], 6 + o, gKladRedKoef[playerid][2][o], o ? -gKladMecLin[playerid][2] : gKladMecLin[playerid][2]);
        }
        else if (gKladRed[playerid][r][0]) KladDodaj(playerid, gKladRed[playerid][r], gKladRedIme[playerid][r], o + 1, gKladRedKoef[playerid][r][o], 0);
        return 1;
    }
    for (new r = 0; r < KLAD_RED; r++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_RED + r] || gKladMecId[playerid][0] || !gKladRed[playerid][r][0]) continue;
        strcopy(gKladMecId[playerid], gKladRed[playerid][r], 16);
        return 1;
    }
    for (new k = 0; k < KLAD_PAROVA; k++)
    {
        if (td != gPc[playerid][PTD_KLAD + KT_PAR + k]) continue;
        KladMakni(playerid, k);
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_MINUS])
    {
        if (gKladUlog[playerid] > 0) gKladUlog[playerid]--;
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_PLUS])
    {
        if (gKladUlog[playerid] < sizeof gKladUlozi - 1) gKladUlog[playerid]++;
        return 1;
    }
    if (td == gPc[playerid][PTD_KLAD + KT_UPLATI])
    {
        KladUplati(playerid);
        return 1;
    }
    return 0;
}

static KladPoruka(racun, const hr[], listic, iznos)
{
    new poruka[144], i = IgracRacuna(racun);
    if (i == INVALID_PLAYER_ID) format(poruka, sizeof poruka, hr, listic, iznos);
    else format(poruka, sizeof poruka, T(i, hr), listic, iznos);
    MobitelBankaRacun(racun, poruka);
}

static KladObracun()
{
    new id[KLAD_OBRACUN], racun[KLAD_OBRACUN], iznos[KLAD_OBRACUN], n, sad = gettime();
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT s.id, s.account, s.stake FROM bet_slips s WHERE s.status = 0 AND EXISTS (SELECT 1 FROM bet_legs l JOIN bet_events e ON e.id = l.event WHERE l.slip = s.id AND e.status = 2 AND NOT %s AND NOT %s) LIMIT %d", KLAD_OK, KLAD_PUSH, KLAD_OBRACUN);
    n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        id[i] = DB_GetFieldInt(res, 0);
        racun[i] = DB_GetFieldInt(res, 1);
        iznos[i] = DB_GetFieldInt(res, 2);
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < n; i++)
    {
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bet_slips SET status = 2, settled = %d WHERE id = %d", sad, id[i]));
        KladPoruka(racun[i], "Listic #%d je gubitni (ulog $%d).", id[i], iznos[i]);
    }
    res = DB_ExecuteQuery(gDB, "SELECT s.id, s.account, s.stake FROM bet_slips s WHERE s.status = 0 AND NOT EXISTS (SELECT 1 FROM bet_legs l LEFT JOIN bet_events e ON e.id = l.event WHERE l.slip = s.id AND NOT (IFNULL(e.status, 3) = 3 OR (e.status = 2 AND (%s OR %s)))) LIMIT %d", KLAD_OK, KLAD_PUSH, KLAD_OBRACUN);
    n = DB_GetRowCount(res);
    for (new i = 0; i < n; i++, DB_SelectNextRow(res))
    {
        id[i] = DB_GetFieldInt(res, 0);
        racun[i] = DB_GetFieldInt(res, 1);
        iznos[i] = DB_GetFieldInt(res, 2);
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < n; i++)
    {
        new koef = 100;
        res = DB_ExecuteQuery(gDB, "SELECT l.odd FROM bet_legs l JOIN bet_events e ON e.id = l.event WHERE l.slip = %d AND e.status = 2 AND %s", id[i], KLAD_OK);
        for (new k = 0, nk = DB_GetRowCount(res); k < nk; k++, DB_SelectNextRow(res)) koef = koef * DB_GetFieldInt(res, 0) / 100;
        DB_FreeResultSet(res);
        if (koef > KLAD_MAX_KOEF) koef = KLAD_MAX_KOEF;
        new dobitak = iznos[i] * (koef / 100) + iznos[i] * (koef % 100) / 100;
        if (dobitak > KLAD_MAX_DOBITAK) dobitak = KLAD_MAX_DOBITAK;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bet_slips SET status = 1, payout = %d, settled = %d WHERE id = %d", dobitak, sad, id[i]));
        BlagajnaDodaj(-dobitak);
        if (!BankaUplati(racun[i], dobitak))
        {
            new p = IgracRacuna(racun[i]);
            if (p == INVALID_PLAYER_ID || !DajNovac(p, dobitak)) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bank_accounts SET balance = balance + %d WHERE account = %d", dobitak, racun[i]));
        }
        KladPoruka(racun[i], "Listic #%d je dobitni! Isplaceno $%d na vas racun.", id[i], dobitak);
    }
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
        case PC_KOMENTAR, PC_PORUKA, PC_PRIJATELJ, PC_PORUKA_KOME: LsUnos(playerid, gPcAkcija[playerid], inputtext);
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
