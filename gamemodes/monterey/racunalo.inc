#define KLAD_CUVAJ_DANA     30
#define RACUNALO_MODEL      2190
#define RACUNALO_CIJENA     2500
#define RACUNALO_DOMET      2.0
#define OGLAS_CIJENA        500
#define OGLAS_LEN           100
#define OGLAS_PAUZA         60
#define OGLAS_TRAJE         86400
#define OBJAVA_LEN          100
#define OBJAVA_PAUZA        30
#define OBJAVA_REDOVA       8
#define DIONICA_MAX_KOM     10000
#define DIONICA_PROVIZIJA   1
#define DIONICA_KORAK_S     10
#define DIONICA_KORAKA      6
#define SVIJECA             20
#define PC_TD               256
#define PC_OBAVIJEST_MS     6000
#define KLAD_RED            5
#define KLAD_NAVY           0x0E2A47FF
#define KLAD_NAVY2          0x16406BFF
#define KLAD_ZUTO           0xFFD200FF
#define KLAD_SIVO           0xE6EAF0FF
#define KLAD_OK             "(CASE WHEN l.pick <= 3 THEN e.result = l.pick WHEN l.pick = 4 THEN (e.hscore + e.ascore) * 10 > l.line WHEN l.pick = 5 THEN (e.hscore + e.ascore) * 10 < l.line WHEN l.pick = 6 THEN e.hscore * 10 + l.line > e.ascore * 10 ELSE e.ascore * 10 + l.line > e.hscore * 10 END)"
#define KLAD_PUSH           "(CASE WHEN l.pick IN (4, 5) THEN (e.hscore + e.ascore) * 10 = l.line WHEN l.pick = 6 THEN e.hscore * 10 + l.line = e.ascore * 10 WHEN l.pick = 7 THEN e.ascore * 10 + l.line = e.hscore * 10 ELSE 0 END)"
#define KLAD_PAROVA         8
#define KLAD_MAX_ULOG       10000
#define KLAD_MAX_KOEF       100000
#define KLAD_MAX_DOBITAK    1000000
#define KLAD_ZATVARA_S      60
#define KLAD_OBRACUN        40
#define KLAD_PROVJERA_S     60
#define LS_PAUZA            5
#define LS_RED_Y            42.0
#define LS_RED_H            17.0
#define LS_SLOT_X           151.0
#define LS_AV_S             62.0
#define LS_PR_X             175.0
#define LS_PR_S             150.0
#define PC_X                120.0
#define PC_Y                80.0
#define PC_SX               1.30667
#define PC_SY               1.18280
#define GRAF_X              90.0
#define GRAF_Y              70.0
#define GRAF_H              96.0
#define GRAF_SIRINA         14.0

enum { PC_BANKA_POSALJI, PC_BANKA_OTPLATI, PC_OGLAS, PC_KOLICINA, PC_OBJAVA, PC_KOMENTAR, PC_PORUKA, PC_PRIJATELJ, PC_PORUKA_KOME, PC_TRAZI, PC_GRUPA_IME, PC_GRUPA_DODAJ, PC_GRUPA_PORUKA }
#define LS_GRUPA_IME        24
enum { LS_FEED, LS_OBJAVA, LS_PRIJATELJI, LS_PORUKE, LS_RAZGOVOR, LS_PROFIL, LS_GRUPE, LS_GRUPA, LS_TRAZI, LS_CLANOVI, LS_DODAJ }
enum { PP_NISTA, PP_OBJAVA, PP_KOMENTAR, PP_GRUPA, PP_CLAN, PP_OGLAS }
enum { LS_NISTA, LS_POSLAO, LS_PRIMIO, LS_PRIJATELJ }
enum { KT_BOK, KT_LISTIC, KT_TAB, KT_KAT = KT_TAB + 3, KT_LIGA = KT_KAT + 5, KT_MEC = KT_LIGA + 7, KT_RED, KT_KOEF = KT_RED + KLAD_RED, KT_STR_L = KT_KOEF + KLAD_RED * 3, KT_STR, KT_STR_D, KT_NASLOV, KT_PAR, KT_UKUPNO = KT_PAR + KLAD_PAROVA, KT_MINUS, KT_ULOG, KT_PLUS, KT_DOBITAK, KT_UPLATI, KLAD_TD }
enum { PCE_POCETNA, PCE_BANKA, PCE_OGLASI, PCE_DIONICE, PCE_LIFESTALKER, PCE_KLADIONICA }
enum { SV_O, SV_H, SV_L, SV_C }
enum { PTD_START, PTD_TRAKA, PTD_PROZOR = PTD_TRAKA + 6, PTD_NASLOV = PTD_PROZOR + 3, PTD_PROZOR_MIN, PTD_ZATVORI, PTD_SADRZAJ, PTD_START_PANEL, PTD_UGASI = PTD_START_PANEL + 17, PTD_APP,
    PTD_AKCIJA = PTD_APP + 6, PTD_DIONICA = PTD_AKCIJA + 4, PTD_GRAF = PTD_DIONICA + 5, PTD_TIJELO, PTD_FITILJ = PTD_TIJELO + SVIJECA, PTD_OBJAVA = PTD_FITILJ + SVIJECA, PTD_KOL = PTD_OBJAVA + OBJAVA_REDOVA, PTD_APPIME = PTD_KOL + 3, PTD_LSTAB = PTD_APPIME + 6, PTD_LSAVATAR = PTD_LSTAB + 6, PTD_LSPROFIL = PTD_LSAVATAR + OBJAVA_REDOVA, PTD_LSPOKRIV = PTD_LSPROFIL + 1, PTD_LSINFO = PTD_LSPOKRIV + 8, PTD_KLAD = PTD_LSINFO + 1, PTD_SAT = PTD_KLAD + KLAD_TD, PTD_DATUM, PTD_OBAVIJEST, PTD_OBAVIJEST_TEKST, PTD_JEZIK, PTD_JEZIK_MENI, PTD_KARTICA = PTD_JEZIK_MENI + BROJ_JEZIKA, PTD_MREZA = PTD_KARTICA + 5, PTD_IMENOVANI = PTD_MREZA + 3 }

static const gDionicaIme[][] = { "Monterey Motors", "LS Nafta", "Vinewood Media", "Pacific Banka", "Grove Tech" };
static const gDionicaKratko[][] = { "MMOT", "LSNF", "VWMD", "PACB", "GRVT" };
static const gDionicaPocetna[] = { 12000, 8000, 6000, 15000, 4000 };
static gDionica[sizeof gDionicaIme], gSvijeca[sizeof gDionicaIme][SVIJECA][4], gSvijecaBroj[sizeof gDionicaIme], gTrenutna[sizeof gDionicaIme][4], gDionicaSat, gDionicaKorak;
static gPcAkcija[MAX_PLAYERS], gPcDionica[MAX_PLAYERS], gOglasZadnji[MAX_PLAYERS], gObjavaZadnja[MAX_PLAYERS], gPcEkran[MAX_PLAYERS], bool:gPcOtvoren[MAX_PLAYERS], bool:gPcStart[MAX_PLAYERS], gPcObjava[MAX_PLAYERS][OBJAVA_REDOVA], gPcKomadaPamti[MAX_PLAYERS][sizeof gDionicaIme], gPcTrosak[MAX_PLAYERS][sizeof gDionicaIme], gPcGrafN[MAX_PLAYERS], gPcGrafMin[MAX_PLAYERS], gPcGrafMax[MAX_PLAYERS], gPcKolicina[MAX_PLAYERS], gLsPogled[MAX_PLAYERS], gLsCilj[MAX_PLAYERS], gLsOdabran[MAX_PLAYERS], gLsZadnje[MAX_PLAYERS], gLsAvatara[MAX_PLAYERS];
static const gLsTab[][] = { "Objave", "Prijatelji", "Poruke", "Grupe", "Trazi", "Profil" };
static const gLsTabPogled[] = { LS_FEED, LS_PRIJATELJI, LS_PORUKE, LS_GRUPE, LS_TRAZI, LS_PROFIL };
static gPcPotvrda[MAX_PLAYERS], bool:gPcMojiOglasi[MAX_PLAYERS], gPcOglasOdabran[MAX_PLAYERS], gLsTrazi[MAX_PLAYERS][24], bool:gLsSamoPrijatelji[MAX_PLAYERS];
static PlayerText:gPcTd[MAX_PLAYERS][PC_TD], gPcBroj[MAX_PLAYERS], PlayerText:gPc[MAX_PLAYERS][PTD_IMENOVANI];
static gPcOtvorene[MAX_PLAYERS], gPcPozOd[MAX_PLAYERS], gPcPozDo[MAX_PLAYERS], bool:gPcJezikMeni[MAX_PLAYERS], bool:gPcPuno;
static const gPcAplikacije[][] = { "Pocetna", "Banka", "Oglasnik", "Dionice", "LifeStalker", "Kladionica" };
static const gPcIkonaSlika[][] = { "LD_SPAC:white", "LD_POOL:ball" };
enum E_PC_IKONA { piApp, Float:piX, Float:piY, Float:piW, Float:piH, piBoja, piSlika }
static const gPcIkonaDio[][E_PC_IKONA] = {
    { 1, 5.0, 8.0, 20.0, 14.0, 0xFFFFFFFF, 0 }, { 1, 5.0, 11.0, 20.0, 3.0, 0x1B5E20FF, 0 }, { 1, 8.0, 16.0, 5.0, 4.0, 0xFFD54FFF, 0 },
    { 2, 7.0, 6.0, 16.0, 18.0, 0xFFFFFFFF, 0 }, { 2, 9.0, 8.0, 12.0, 3.0, 0x424242FF, 0 }, { 2, 9.0, 13.0, 12.0, 1.5, 0x9E9E9EFF, 0 }, { 2, 9.0, 17.0, 9.0, 1.5, 0x9E9E9EFF, 0 },
    { 3, 6.0, 24.0, 18.0, 1.0, 0xFFFFFFFF, 0 }, { 3, 7.0, 17.0, 4.0, 7.0, 0xFFFFFFFF, 0 }, { 3, 13.0, 13.0, 4.0, 11.0, 0xFFFFFFFF, 0 }, { 3, 19.0, 8.0, 4.0, 16.0, 0xFFFFFFFF, 0 },
    { 4, 6.0, 8.0, 18.0, 12.0, 0xFFFFFFFF, 0 }, { 4, 9.0, 20.0, 4.0, 3.0, 0xFFFFFFFF, 0 },
    { 5, 8.0, 5.0, 14.0, 20.0, 0xFFFFFFFF, 0 }, { 5, 10.0, 8.0, 10.0, 1.5, 0x00897BFF, 0 }, { 5, 10.0, 12.0, 10.0, 1.0, 0x9E9E9EFF, 0 }, { 5, 10.0, 15.0, 7.0, 1.0, 0x9E9E9EFF, 0 },
    { 5, 15.0, 16.0, 10.0, 10.0, 0xFFD54FFF, 1 }, { 5, 18.5, 19.5, 3.0, 3.0, 0xF57F17FF, 1 }
};
static const gPcJezici[BROJ_JEZIKA][] = { "Hrvatski", "English", "Russkiy", "Espanol", "Turkce" };
static PlayerText:gPcIkonaTd[MAX_PLAYERS][sizeof gPcIkonaDio];
static const gPcIkonaBoja[] = { 0, 0x2E7D32FF, 0xEF6C00FF, 0x6A1B9AFF, 0xE91E63FF, 0x00897BFF };
static gKladPar[MAX_PLAYERS][KLAD_PAROVA][16], gKladTip[MAX_PLAYERS][KLAD_PAROVA], gKladKoefPar[MAX_PLAYERS][KLAD_PAROVA], gKladN[MAX_PLAYERS], gKladStr[MAX_PLAYERS], bool:gKladIma[MAX_PLAYERS], gKladRed[MAX_PLAYERS][KLAD_RED][16], gKladSat;
static gPcObavijestDo[MAX_PLAYERS], gKladLiga[MAX_PLAYERS] = { -1, ... }, gKladMecId[MAX_PLAYERS][16], gKladMecLin[MAX_PLAYERS][3], gKladLin[MAX_PLAYERS][KLAD_PAROVA], gKladTab[MAX_PLAYERS], gKladKat[MAX_PLAYERS], gKladUlog[MAX_PLAYERS], gKladDetalj[MAX_PLAYERS], gKladRedKoef[MAX_PLAYERS][KLAD_RED][3], gKladRedIme[MAX_PLAYERS][KLAD_RED][24], gKladParIme[MAX_PLAYERS][KLAD_PAROVA][24], gKladRedListic[MAX_PLAYERS][KLAD_RED];
static const gKladTabovi[][] = { "Ponuda", "U igri", "Zavrseni" };
static const gKladKategorije[][] = { "Sve", "Nogomet", "Kosarka", "Hokej", "Am. nogomet" };
static const gKladKatSql[][] = { "1", "league IN ('EPL','LaLiga','SerieA','Bundes','Ligue1','UCL','UEL')", "league = 'NBA'", "league = 'NHL'", "league = 'NFL'" };
static const gKladLige[][] = { "EPL", "LaLiga", "SerieA", "Bundes", "Ligue1", "UCL", "UEL" };
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
    new koef = 100;
    for (new i = 0; i < KLAD_PAROVA; i++) koef = KladPomnozi(koef, 500);
    assert(koef == KLAD_MAX_KOEF && KladPomnozi(100, 250) == 250 && KladDobitak(KLAD_MAX_ULOG, KLAD_MAX_KOEF) == KLAD_MAX_DOBITAK);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_events (id TEXT PRIMARY KEY, league TEXT NOT NULL, home TEXT NOT NULL, away TEXT NOT NULL, start INTEGER NOT NULL, status INTEGER NOT NULL DEFAULT 0, result INTEGER NOT NULL DEFAULT 0, odd1 INTEGER NOT NULL DEFAULT 0, oddx INTEGER NOT NULL DEFAULT 0, odd2 INTEGER NOT NULL DEFAULT 0, updated INTEGER NOT NULL DEFAULT 0, hscore INTEGER NOT NULL DEFAULT 0, ascore INTEGER NOT NULL DEFAULT 0, tline INTEGER NOT NULL DEFAULT 0, odd_over INTEGER NOT NULL DEFAULT 0, odd_under INTEGER NOT NULL DEFAULT 0, sline INTEGER NOT NULL DEFAULT 0, odd_sph INTEGER NOT NULL DEFAULT 0, odd_spa INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_slips (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, stake INTEGER NOT NULL, odds INTEGER NOT NULL, payout INTEGER NOT NULL, status INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL, settled INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS bet_legs (slip INTEGER NOT NULL, event TEXT NOT NULL, pick INTEGER NOT NULL, odd INTEGER NOT NULL, line INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (slip, event, pick))"));
    new staro = gettime() - KLAD_CUVAJ_DANA * 86400;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bet_legs WHERE slip IN (SELECT id FROM bet_slips WHERE status != 0 AND settled > 0 AND settled < %d)", staro));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bet_slips WHERE status != 0 AND settled > 0 AND settled < %d", staro));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM bet_events WHERE status >= 2 AND start < %d AND id NOT IN (SELECT event FROM bet_legs)", staro));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ads (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_holdings (account INTEGER NOT NULL, stock INTEGER NOT NULL, shares INTEGER NOT NULL, cost INTEGER NOT NULL DEFAULT 0, PRIMARY KEY (account, stock))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS stock_candles (id INTEGER PRIMARY KEY AUTOINCREMENT, stock INTEGER NOT NULL, o INTEGER NOT NULL, h INTEGER NOT NULL, l INTEGER NOT NULL, c INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_posts (id INTEGER PRIMARY KEY AUTOINCREMENT, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL, shared INTEGER NOT NULL DEFAULT 0)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_likes (post INTEGER NOT NULL, account INTEGER NOT NULL, PRIMARY KEY (post, account))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_accounts (account INTEGER PRIMARY KEY, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_friends (a INTEGER NOT NULL, b INTEGER NOT NULL, accepted INTEGER NOT NULL DEFAULT 0, created INTEGER NOT NULL, PRIMARY KEY (a, b))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_comments (id INTEGER PRIMARY KEY AUTOINCREMENT, post INTEGER NOT NULL, account INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_blocks (a INTEGER NOT NULL, b INTEGER NOT NULL, PRIMARY KEY (a, b))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_groups (id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL, owner INTEGER NOT NULL, created INTEGER NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_group_members (grp INTEGER NOT NULL, account INTEGER NOT NULL, PRIMARY KEY (grp, account))"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS social_group_messages (id INTEGER PRIMARY KEY AUTOINCREMENT, grp INTEGER NOT NULL, sender INTEGER NOT NULL, text TEXT NOT NULL, created INTEGER NOT NULL)"));
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

static Float:PcPX(Float:x) return 4.0 + (x - 80.0) * PC_SX;
static Float:PcPY(Float:y) return 4.0 + (y - 20.0) * PC_SY;
static Float:PcX(Float:x) return gPcPuno ? PcPX(x) : x;
static Float:PcY(Float:y) return gPcPuno ? PcPY(y) : y;
static Float:PcW(Float:w) return gPcPuno ? w * PC_SX : w;
static Float:PcH(Float:h) return gPcPuno ? h * PC_SY : h;

static PlayerText:PcSlika(playerid, Float:x, Float:y, Float:w, Float:h, boja, const slika[] = "LD_SPAC:white")
{
    return gPcTd[playerid][gPcBroj[playerid]++] = TdSlika(playerid, PC_X + PcX(x), PC_Y + PcY(y), PcW(w), PcH(h), boja, slika);
}

static PlayerText:PcKutija(playerid, Float:x, Float:y, Float:w, Float:h, boja)
{
    return PcSlika(playerid, x, y, w, h, boja);
}

static PlayerText:PcTekst(playerid, Float:x, Float:y, const tekst[], boja, Float:sirina, Float:visina, TEXT_DRAW_FONT:font = TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN:poravnanje = TEXT_DRAW_ALIGN_LEFT)
{
    new PlayerText:td = TdTekst(playerid, PC_X + PcX(x), PC_Y + PcY(y), tekst, boja, PcW(sirina), PcH(visina), font, poravnanje);
    PlayerTextDrawSetProportional(playerid, td, true);
    return gPcTd[playerid][gPcBroj[playerid]++] = td;
}

static PlayerText:PcGumb(playerid, Float:x, Float:y, Float:w, boja, const tekst[] = "_", tekstBoja = 0xFFFFFFFF)
{
    new PlayerText:td = PcTekst(playerid, x, y, tekst, tekstBoja, 0.17, 0.85, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, boja);
    PlayerTextDrawTextSize(playerid, td, 10.0, PcW(w));
    PlayerTextDrawSetSelectable(playerid, td, true);
    return td;
}

static PlayerText:PcModel(playerid, Float:x, Float:y, Float:w, Float:h)
{
    new PlayerText:td = CreatePlayerTextDraw(playerid, PC_X + PcX(x), PC_Y + PcY(y), "_");
    PlayerTextDrawFont(playerid, td, TEXT_DRAW_FONT_MODEL_PREVIEW);
    PlayerTextDrawTextSize(playerid, td, PcW(w), PcH(h));
    PlayerTextDrawBackgroundColour(playerid, td, 0xE4E6EBFF);
    PlayerTextDrawSetPreviewModel(playerid, td, DEFAULT_SKIN);
    PlayerTextDrawSetPreviewRot(playerid, td, 0.0, 0.0, 0.0, 1.0);
    gPcTd[playerid][gPcBroj[playerid]++] = td;
    return td;
}

static PlayerText:PcRed(playerid, Float:x, Float:y, Float:w)
{
    new PlayerText:td = PcTekst(playerid, x, y, "_", 0x202020FF, 0.15, 0.74, TEXT_DRAW_FONT_1);
    PlayerTextDrawUseBox(playerid, td, true);
    PlayerTextDrawBoxColour(playerid, td, 0xF2F2F2FF);
    PlayerTextDrawTextSize(playerid, td, PC_X + PcX(x + w), 10.0);
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

static PcAplikacijaVidljiva(playerid, i, bool:vidljivo)
{
    new PlayerText:dijelovi[2];
    dijelovi[0] = gPc[playerid][PTD_APP + i];
    dijelovi[1] = gPc[playerid][PTD_APPIME + i];
    for (new k = 0; k < 2; k++)
    {
        if (vidljivo) PlayerTextDrawShow(playerid, dijelovi[k]);
        else PlayerTextDrawHide(playerid, dijelovi[k]);
    }
    for (new d = 0; d < sizeof gPcIkonaDio; d++)
    {
        if (gPcIkonaDio[d][piApp] != i) continue;
        if (vidljivo) PlayerTextDrawShow(playerid, gPcIkonaTd[playerid][d]);
        else PlayerTextDrawHide(playerid, gPcIkonaTd[playerid][d]);
    }
}

static PcRadnaPovrsina(playerid, bool:vidljivo)
{
    for (new i = gPcPozOd[playerid]; i < gPcPozDo[playerid]; i++)
    {
        if (vidljivo) PlayerTextDrawShow(playerid, gPcTd[playerid][i]);
        else PlayerTextDrawHide(playerid, gPcTd[playerid][i]);
    }
}

static PcTrakaOsvjezi(playerid)
{
    new k = 0;
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        new PlayerText:td = gPc[playerid][PTD_TRAKA + i];
        if (!(gPcOtvorene[playerid] & (1 << i)))
        {
            PlayerTextDrawHide(playerid, td);
            continue;
        }
        PlayerTextDrawSetPos(playerid, td, PC_X + 58.0 + 56.0 * float(k++), PC_Y + 229.0);
        PlayerTextDrawBoxColour(playerid, td, gPcEkran[playerid] == i ? 0xFFFFFF44 : 0xFFFFFF16);
        PlayerTextDrawShow(playerid, td);
    }
}

static PcJezikMeni(playerid, bool:otvori)
{
    gPcJezikMeni[playerid] = otvori;
    for (new j = 0; j < BROJ_JEZIKA; j++)
    {
        if (otvori) PlayerTextDrawShow(playerid, gPc[playerid][PTD_JEZIK_MENI + j]);
        else PlayerTextDrawHide(playerid, gPc[playerid][PTD_JEZIK_MENI + j]);
    }
}

static PcStart(playerid, bool:otvori)
{
    gPcStart[playerid] = otvori;
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        if (12.0 + float(i - 1) * 42.0 + 40.0 < 100.0 || gPcEkran[playerid] != PCE_POCETNA) continue;
        PcAplikacijaVidljiva(playerid, i, !otvori);
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
    PlayerTextDrawSetPos(playerid, fitilj, PC_X + PcPX(cx - 0.5), PC_Y + PcPY(yh));
    PlayerTextDrawTextSize(playerid, fitilj, PC_SX, (yl - yh > 1.0 ? yl - yh : 1.0) * PC_SY);
    PlayerTextDrawColour(playerid, fitilj, boja);
    PlayerTextDrawSetPos(playerid, tijelo, PC_X + PcPX(cx - 4.0), PC_Y + PcPY(yg));
    PlayerTextDrawTextSize(playerid, tijelo, 8.0 * PC_SX, (yd - yg > 1.0 ? yd - yg : 1.0) * PC_SY);
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
    new DBResult:res = strfind(upit, "%d", false, strfind(upit, "%d") + 1) == -1 ? DB_ExecuteQuery(gDB, upit, acc) : DB_ExecuteQuery(gDB, upit, acc, acc), n = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
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
    new red[320], tekst[OBJAVA_LEN + 1], prikaz[OBJAVA_LEN + 40], ime[MAX_PLAYER_NAME + 1], izvor[MAX_PLAYER_NAME + 1], vrijeme[24], upit[640];
    format(upit, sizeof upit, "SELECT p.id, p.account, IFNULL(o.text, p.text), replace(a.name, '_', ' '), p.created, (SELECT COUNT(*) FROM social_likes WHERE post = p.id), (SELECT COUNT(*) FROM social_comments WHERE post = p.id), IFNULL(replace(oa.name, '_', ' '), '') FROM social_posts p JOIN accounts a ON a.id = p.account LEFT JOIN social_posts o ON o.id = p.shared LEFT JOIN accounts oa ON oa.id = o.account %s ORDER BY p.id DESC LIMIT %d", uvjet, OBJAVA_REDOVA - od);
    new DBResult:res = DB_ExecuteQuery(gDB, upit, acc), n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 2, tekst, sizeof tekst);
        DB_GetFieldString(res, 3, ime, sizeof ime);
        DB_GetFieldString(res, 7, izvor, sizeof izvor);
        LsTekstObjave(tekst, izvor, prikaz, sizeof prikaz);
        TdCisti(ime);
        LsPrije(playerid, DB_GetFieldInt(res, 4), vrijeme, sizeof vrijeme);
        format(red, sizeof red, T(playerid, "~b~%s  ~l~%s   ~r~+%d  ~l~%d komentara~n~~l~%s"), ime, vrijeme, DB_GetFieldInt(res, 5), DB_GetFieldInt(res, 6), prikaz);
        LsRed(playerid, od + r, DB_GetFieldInt(res, 1), DB_GetFieldInt(res, 0), red);
    }
    DB_FreeResultSet(res);
    return n;
}

static LsTekstObjave(tekst[], izvor[], out[], len)
{
    TdCisti(tekst);
    TdCisti(izvor);
    if (izvor[0]) format(out, len, "~y~%s: ~l~%s", izvor, tekst);
    else format(out, len, "%s", tekst);
}

static bool:LsBlok(a, b)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_blocks WHERE (a = %d AND b = %d) OR (a = %d AND b = %d)", a, b, b, a), bool:ima = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return ima;
}

static bool:LsBlokirao(a, b)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_blocks WHERE a = %d AND b = %d", a, b), bool:ima = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return ima;
}

static LsGrupaPodaci(grp, ime[], len)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT owner, name FROM social_groups WHERE id = %d", grp), vlasnik = 0;
    ime[0] = EOS;
    if (DB_GetRowCount(res))
    {
        vlasnik = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, ime, len);
        TdCisti(ime);
    }
    DB_FreeResultSet(res);
    return vlasnik;
}

static bool:LsClan(grp, acc)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_group_members WHERE grp = %d AND account = %d", grp, acc), bool:ima = DB_GetRowCount(res) > 0;
    DB_FreeResultSet(res);
    return ima;
}

static PcAkcijaPoz(playerid, i, Float:x, Float:w)
{
    new PlayerText:td = gPc[playerid][PTD_AKCIJA + i];
    PlayerTextDrawSetPos(playerid, td, PC_X + PcPX(x), PC_Y + PcPY(190.0));
    PlayerTextDrawTextSize(playerid, td, 10.0, w * PC_SX);
}

static LsAkcije(playerid, const prva[], const druga[] = "", const treca[] = "", const cetvrta[] = "")
{
    new n = 1;
    if (druga[0]) n++;
    if (treca[0]) n++;
    if (cetvrta[0]) n++;
    new Float:w = (230.0 - 6.0 * float(n - 1)) / float(n);
    for (new i = 0; i < 4; i++)
    {
        PcAkcijaPoz(playerid, i, 145.0 + w / 2.0 + float(i) * (w + 6.0), w);
        PlayerTextDrawHide(playerid, gPc[playerid][PTD_AKCIJA + i]);
    }
    PcPostavi(playerid, PTD_AKCIJA, prva, prva[0] != 0);
    PcPostavi(playerid, PTD_AKCIJA + 1, druga, druga[0] != 0);
    PcPostavi(playerid, PTD_AKCIJA + 2, treca, treca[0] != 0);
    PcPostavi(playerid, PTD_AKCIJA + 3, cetvrta, cetvrta[0] != 0);
}

static bool:PcPotvrdi(playerid, i, kod)
{
    if (gPcPotvrda[playerid] == kod) return true;
    gPcPotvrda[playerid] = kod;
    PcPostavi(playerid, PTD_AKCIJA + i, T(playerid, "Potvrdi?"));
    return false;
}

static LsFeed(playerid)
{
    new me = gAccountId[playerid], uvjet[512];
    format(uvjet, sizeof uvjet, "WHERE p.account NOT IN (SELECT b FROM social_blocks WHERE a = %d UNION SELECT a FROM social_blocks WHERE b = %d)", me, me);
    if (gLsSamoPrijatelji[playerid]) format(uvjet, sizeof uvjet, "%s AND (p.account = %d OR p.account IN (SELECT CASE WHEN a = %d THEN b ELSE a END FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1))", uvjet, me, me, me, me);
    new n = LsObjave(playerid, uvjet, 0, 0);
    if (n) PcPostavi(playerid, PTD_SADRZAJ, "_");
    else PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Jos nema objava. Budite prvi!"));
    LsAkcije(playerid, T(playerid, "Objavi"), gLsSamoPrijatelji[playerid] ? T(playerid, "Prikaz: prijatelji") : T(playerid, "Prikaz: svi"));
}

static LsObjava(playerid)
{
    new post = gLsCilj[playerid], me = gAccountId[playerid], red[320], tekst[OBJAVA_LEN + 1], prikaz[OBJAVA_LEN + 40], ime[MAX_PLAYER_NAME + 1], izvor[MAX_PLAYER_NAME + 1], vrijeme[24];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT p.account, IFNULL(o.text, p.text), replace(a.name, '_', ' '), p.created, (SELECT COUNT(*) FROM social_likes WHERE post = p.id), (SELECT COUNT(*) FROM social_likes WHERE post = p.id AND account = %d), IFNULL(replace(oa.name, '_', ' '), '') FROM social_posts p JOIN accounts a ON a.id = p.account LEFT JOIN social_posts o ON o.id = p.shared LEFT JOIN accounts oa ON oa.id = o.account WHERE p.id = %d", me, post);
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
    DB_GetFieldString(res, 6, izvor, sizeof izvor);
    LsTekstObjave(tekst, izvor, prikaz, sizeof prikaz);
    TdCisti(ime);
    LsPrije(playerid, DB_GetFieldInt(res, 3), vrijeme, sizeof vrijeme);
    format(red, sizeof red, T(playerid, "~b~%s  ~l~%s   ~r~+%d~n~~l~%s"), ime, vrijeme, DB_GetFieldInt(res, 4), prikaz);
    DB_FreeResultSet(res);
    LsRed(playerid, 0, autor, autor, red);
    res = DB_ExecuteQuery(gDB, "SELECT account, text, name, created, id FROM (SELECT c.id, c.account, c.text, replace(a.name, '_', ' ') AS name, c.created FROM social_comments c JOIN accounts a ON a.id = c.account WHERE c.post = %d ORDER BY c.id DESC LIMIT %d) ORDER BY id", post, OBJAVA_REDOVA - 1);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, tekst, sizeof tekst);
        DB_GetFieldString(res, 2, ime, sizeof ime);
        TdCisti(tekst);
        TdCisti(ime);
        LsPrije(playerid, DB_GetFieldInt(res, 3), vrijeme, sizeof vrijeme);
        new cid = DB_GetFieldInt(res, 4);
        format(red, sizeof red, "~p~%s  ~l~%s%s~n~~l~%s", ime, vrijeme, acc == me && cid == gLsOdabran[playerid] ? (" ~r~<") : (""), tekst);
        LsRed(playerid, r + 1, acc, acc == me ? -cid : acc, red);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, "_");
    new brisi[32];
    if (gLsOdabran[playerid]) strcat(brisi, T(playerid, "Obrisi komentar"));
    else if (autor == me) strcat(brisi, T(playerid, "Obrisi objavu"));
    LsAkcije(playerid, svida ? T(playerid, "Ne svidja mi se") : T(playerid, "Svidja mi se"), T(playerid, "Komentiraj"), T(playerid, "Podijeli"), brisi);
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
    if (n < OBJAVA_REDOVA && !gLsOdabran[playerid])
    {
        new izuzmi[256], broj;
        format(izuzmi, sizeof izuzmi, "(SELECT b FROM social_friends WHERE a = %d UNION SELECT a FROM social_friends WHERE b = %d UNION SELECT b FROM social_blocks WHERE a = %d UNION SELECT a FROM social_blocks WHERE b = %d)", me, me, me, me);
        res = DB_ExecuteQuery(gDB, "SELECT x.acc, COUNT(*), replace(ac.name, '_', ' ') FROM (SELECT CASE WHEN f2.a = f1.o THEN f2.b ELSE f2.a END AS acc FROM (SELECT CASE WHEN a = %d THEN b ELSE a END AS o FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1) f1 JOIN social_friends f2 ON (f2.a = f1.o OR f2.b = f1.o) AND f2.accepted = 1) x JOIN accounts ac ON ac.id = x.acc WHERE x.acc != %d AND x.acc NOT IN %s GROUP BY x.acc ORDER BY COUNT(*) DESC LIMIT %d", me, me, me, me, izuzmi, OBJAVA_REDOVA - n);
        if (!DB_GetRowCount(res))
        {
            DB_FreeResultSet(res);
            res = DB_ExecuteQuery(gDB, "SELECT s.account, 0, replace(ac.name, '_', ' ') FROM social_accounts s JOIN accounts ac ON ac.id = s.account WHERE s.account != %d AND s.account NOT IN %s ORDER BY s.created DESC LIMIT %d", me, izuzmi, OBJAVA_REDOVA - n);
        }
        broj = DB_GetRowCount(res);
        for (new r = 0; r < broj; r++, DB_SelectNextRow(res))
        {
            new acc = DB_GetFieldInt(res, 0), zajedno = DB_GetFieldInt(res, 1);
            DB_GetFieldString(res, 2, ime, sizeof ime);
            TdCisti(ime);
            if (zajedno) format(red, sizeof red, T(playerid, "~y~Prijedlog: ~b~%s~n~~l~Zajednickih prijatelja: %d"), ime, zajedno);
            else format(red, sizeof red, T(playerid, "~y~Prijedlog: ~b~%s~n~~l~Mozda ga poznajete"), ime);
            LsRed(playerid, n + r, acc, acc, red);
        }
        DB_FreeResultSet(res);
        n += broj;
    }
    if (n) PcPostavi(playerid, PTD_SADRZAJ, "_");
    else PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Jos nemate prijatelja."));
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
    if (n) PcPostavi(playerid, PTD_SADRZAJ, "_");
    else PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Jos nemate poruka."));
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
    if (acc != me && LsBlokirao(acc, me))
    {
        format(tekst, sizeof tekst, T(playerid, "~b~%s~n~~n~~l~Profil nije dostupan."), ime);
        PcPostavi(playerid, PTD_LSINFO, tekst);
        PcPostavi(playerid, PTD_SADRZAJ, "_");
        PlayerTextDrawSetPreviewModel(playerid, gPc[playerid][PTD_LSPROFIL], LsSkin(acc));
        PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPROFIL]);
        for (new k = 4; k <= 6; k++) PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPOKRIV + k]);
        gLsAvatara[playerid] = OBJAVA_REDOVA;
        return 1;
    }
    new prijatelja = LsBroj("SELECT COUNT(*) FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1", acc);
    new objava = LsBroj("SELECT COUNT(*) FROM social_posts WHERE account = %d", acc);
    new lajkova = LsBroj("SELECT COUNT(*) FROM social_likes l JOIN social_posts p ON p.id = l.post WHERE p.account = %d", acc);
    format(tekst, sizeof tekst, T(playerid, "~b~%s~n~%s~n~~n~~l~Prijatelja: %d~n~Objava: %d~n~Svidjanja: %d"), ime, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"), prijatelja, objava, lajkova);
    PcPostavi(playerid, PTD_LSINFO, tekst);
    PcPostavi(playerid, PTD_SADRZAJ, "_");
    PlayerTextDrawSetPreviewModel(playerid, gPc[playerid][PTD_LSPROFIL], LsSkin(acc));
    PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPROFIL]);
    for (new k = 4; k <= 6; k++) PlayerTextDrawShow(playerid, gPc[playerid][PTD_LSPOKRIV + k]);
    LsObjave(playerid, "WHERE p.account = %d", acc, OBJAVA_REDOVA - 4);
    for (new k = OBJAVA_REDOVA - 4; k < OBJAVA_REDOVA; k++) PlayerTextDrawHide(playerid, gPc[playerid][PTD_LSAVATAR + k]);
    gLsAvatara[playerid] = OBJAVA_REDOVA;
    if (acc == me)
    {
        LsAkcije(playerid, T(playerid, "Objavi"));
        return 1;
    }
    new prijatelj[32];
    if (!LsBlokirao(acc, me))
    {
        switch (LsOdnos(me, acc))
        {
            case LS_PRIJATELJ: strcat(prijatelj, T(playerid, "Ukloni"));
            case LS_POSLAO: strcat(prijatelj, T(playerid, "Otkazi"));
            case LS_PRIMIO: strcat(prijatelj, T(playerid, "Prihvati"));
            default: if (!LsBlokirao(me, acc)) strcat(prijatelj, T(playerid, "Dodaj"));
        }
    }
    if (prijatelj[0]) LsAkcije(playerid, T(playerid, "Poruka"), prijatelj, LsBlokirao(me, acc) ? T(playerid, "Odblokiraj") : T(playerid, "Blokiraj"));
    else LsAkcije(playerid, T(playerid, "Poruka"), LsBlokirao(me, acc) ? T(playerid, "Odblokiraj") : T(playerid, "Blokiraj"));
    return 1;
}

static LsDodaj(playerid)
{
    new grp = gLsCilj[playerid], me = gAccountId[playerid], red[160], ime[MAX_PLAYER_NAME + 1], naziv[LS_GRUPA_IME + 1];
    if (LsGrupaPodaci(grp, naziv, sizeof naziv) != me)
    {
        gLsPogled[playerid] = LS_GRUPE;
        LsGrupe(playerid);
        return;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x.acc, replace(a.name, '_', ' ') FROM (SELECT CASE WHEN a = %d THEN b ELSE a END AS acc FROM social_friends WHERE (a = %d OR b = %d) AND accepted = 1) x JOIN accounts a ON a.id = x.acc WHERE x.acc NOT IN (SELECT account FROM social_group_members WHERE grp = %d) ORDER BY a.name LIMIT %d", me, me, me, grp, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        TdCisti(ime);
        format(red, sizeof red, "~g~+  ~b~%s~n~%s", ime, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"));
        LsRed(playerid, r, acc, acc, red);
    }
    DB_FreeResultSet(res);
    if (n) PcPostavi(playerid, PTD_SADRZAJ, "_");
    else PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Svi vasi prijatelji su vec u grupi."));
    LsAkcije(playerid, T(playerid, "Natrag"));
}

static LsGrupaDodaj(playerid, grp, acc)
{
    new me = gAccountId[playerid], naziv[LS_GRUPA_IME + 1];
    if (LsGrupaPodaci(grp, naziv, sizeof naziv) != me) return 1;
    if (!acc || acc == me || LsBlok(me, acc)) return PcJavi(playerid, true, T(playerid, "Korisnik nije pronadjen."));
    if (LsOdnos(me, acc) != LS_PRIJATELJ) return PcJavi(playerid, true, T(playerid, "U grupu mozete dodati samo prijatelje."));
    if (LsClan(grp, acc)) return PcJavi(playerid, true, T(playerid, "Korisnik je vec u grupi."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_group_members (grp, account) VALUES (%d, %d)", grp, acc));
    PcLifeStalker(playerid);
    LsOsvjezi(LS_CLANOVI, grp, playerid);
    LsOsvjezi(LS_GRUPA, grp, playerid);
    LsOsvjezi(LS_DODAJ, grp, playerid);
    new t = IgracRacuna(acc);
    if (t != INVALID_PLAYER_ID && gPcOtvoren[t])
    {
        new obavijest[144];
        format(obavijest, sizeof obavijest, T(t, "~p~LifeStalker~n~~w~%s vas je dodao u grupu %s."), GetRPIme(playerid), naziv);
        PcObavijest(t, obavijest);
        if (gPcEkran[t] == PCE_LIFESTALKER && gLsPogled[t] == LS_GRUPE) PcLifeStalker(t);
    }
    return PcJavi(playerid, false, T(playerid, "Clan je dodan u grupu."));
}

static LsClanovi(playerid)
{
    new grp = gLsCilj[playerid], me = gAccountId[playerid], red[160], ime[MAX_PLAYER_NAME + 1], naziv[LS_GRUPA_IME + 1], vlasnik = LsGrupaPodaci(grp, naziv, sizeof naziv);
    if (!vlasnik || !LsClan(grp, me))
    {
        gLsPogled[playerid] = LS_GRUPE;
        LsGrupe(playerid);
        return;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT m.account, replace(a.name, '_', ' ') FROM social_group_members m JOIN accounts a ON a.id = m.account WHERE m.grp = %d ORDER BY m.account = %d DESC, a.name LIMIT %d", grp, vlasnik, OBJAVA_REDOVA);
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        TdCisti(ime);
        new opis[48], oznaka[64];
        if (acc == vlasnik) strcat(opis, T(playerid, "~y~Vlasnik grupe"));
        else strcat(opis, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"));
        if (acc == gLsOdabran[playerid]) strcat(oznaka, T(playerid, " ~r~Klikni opet za izbacivanje"));
        if (vlasnik == me && acc != me) format(red, sizeof red, "~r~-  ~b~%s%s~n~%s", ime, oznaka, opis);
        else format(red, sizeof red, "~b~%s~n~%s", ime, opis);
        LsRed(playerid, r, acc, acc, red);
    }
    DB_FreeResultSet(res);
    PcPostavi(playerid, PTD_SADRZAJ, "_");
    if (vlasnik == me) LsAkcije(playerid, T(playerid, "Dodaj clana"), T(playerid, "Obrisi grupu"), T(playerid, "Natrag"));
    else LsAkcije(playerid, T(playerid, "Napusti"), T(playerid, "Natrag"));
}

static LsGrupe(playerid)
{
    new me = gAccountId[playerid], red[160], ime[LS_GRUPA_IME + 1];
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT g.id, g.name, (SELECT COUNT(*) FROM social_group_members WHERE grp = g.id), g.owner FROM social_groups g JOIN social_group_members m ON m.grp = g.id AND m.account = %d ORDER BY g.id DESC LIMIT %d", me, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        DB_GetFieldString(res, 1, ime, sizeof ime);
        TdCisti(ime);
        format(red, sizeof red, T(playerid, "~b~%s~n~~l~Clanova: %d"), ime, DB_GetFieldInt(res, 2));
        LsRed(playerid, r, DB_GetFieldInt(res, 3), DB_GetFieldInt(res, 0), red);
    }
    DB_FreeResultSet(res);
    if (n) PcPostavi(playerid, PTD_SADRZAJ, "_");
    else PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Niste ni u jednoj grupi."));
    LsAkcije(playerid, T(playerid, "Nova grupa"));
}

static LsGrupa(playerid)
{
    new grp = gLsCilj[playerid], me = gAccountId[playerid], red[256], tekst[OBJAVA_LEN + 1], ime[MAX_PLAYER_NAME + 1], naziv[LS_GRUPA_IME + 1], vrijeme[24];
    new vlasnik = LsGrupaPodaci(grp, naziv, sizeof naziv);
    if (!vlasnik || !LsClan(grp, me))
    {
        gLsPogled[playerid] = LS_GRUPE;
        LsGrupe(playerid);
        return;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT sender, text, created, name FROM (SELECT m.id, m.sender, m.text, m.created, replace(a.name, '_', ' ') AS name FROM social_group_messages m JOIN accounts a ON a.id = m.sender WHERE m.grp = %d ORDER BY m.id DESC LIMIT %d) ORDER BY id", grp, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new od = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, tekst, sizeof tekst);
        DB_GetFieldString(res, 3, ime, sizeof ime);
        TdCisti(tekst);
        TdCisti(ime);
        LsPrije(playerid, DB_GetFieldInt(res, 2), vrijeme, sizeof vrijeme);
        format(red, sizeof red, "%s%s  ~l~%s~n~~l~%s", od == me ? ("~g~") : ("~b~"), ime, vrijeme, tekst);
        LsRed(playerid, r, od, od, red);
    }
    DB_FreeResultSet(res);
    format(red, sizeof red, T(playerid, "Grupa: %s~n~~n~Jos nema poruka."), naziv);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : red);
    LsAkcije(playerid, T(playerid, "Posalji"), T(playerid, "Clanovi"), vlasnik == me ? ("+") : (""));
}

static LsTrazi(playerid)
{
    new me = gAccountId[playerid], red[160], ime[MAX_PLAYER_NAME + 1], uzorak[24];
    if (!gLsTrazi[playerid][0])
    {
        PcPostavi(playerid, PTD_SADRZAJ, T(playerid, "Pretrazite korisnike po imenu."));
        LsAkcije(playerid, T(playerid, "Pretrazi"));
        return;
    }
    strcat(uzorak, gLsTrazi[playerid]);
    for (new i = 0; uzorak[i]; i++) if (uzorak[i] == ' ') uzorak[i] = '_';
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT a.id, replace(a.name, '_', ' ') FROM accounts a JOIN social_accounts s ON s.account = a.id WHERE a.name LIKE '%%%q%%' AND a.id NOT IN (SELECT a FROM social_blocks WHERE b = %d) ORDER BY a.name LIMIT %d", uzorak, me, OBJAVA_REDOVA);
    new n = DB_GetRowCount(res);
    for (new r = 0; r < n; r++, DB_SelectNextRow(res))
    {
        new acc = DB_GetFieldInt(res, 0);
        DB_GetFieldString(res, 1, ime, sizeof ime);
        TdCisti(ime);
        format(red, sizeof red, "~b~%s~n~%s", ime, IgracRacuna(acc) != INVALID_PLAYER_ID ? ("~g~Online") : ("~l~Offline"));
        LsRed(playerid, r, acc, acc, red);
    }
    DB_FreeResultSet(res);
    TdCisti(uzorak);
    format(red, sizeof red, T(playerid, "Nema rezultata za: %s"), gLsTrazi[playerid]);
    PcPostavi(playerid, PTD_SADRZAJ, n ? ("_") : red);
    LsAkcije(playerid, T(playerid, "Nova pretraga"));
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
    PlayerTextDrawSetPos(playerid, gPc[playerid][PTD_SADRZAJ], PC_X + PcPX(LS_SLOT_X + 46.0), PC_Y + PcPY(46.0));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_accounts (account, created) VALUES (%d, %d)", me, gettime()));
    new zahtjeva = LsBroj("SELECT COUNT(*) FROM social_friends WHERE b = %d AND accepted = 0", me);
    new poruka = LsBroj("SELECT COUNT(*) FROM social_messages WHERE receiver = %d AND seen = 0", me);
    new pogled = gLsPogled[playerid] == LS_OBJAVA ? LS_FEED : gLsPogled[playerid] == LS_RAZGOVOR ? LS_PORUKE : gLsPogled[playerid] == LS_GRUPA || gLsPogled[playerid] == LS_CLANOVI || gLsPogled[playerid] == LS_DODAJ ? LS_GRUPE : gLsPogled[playerid], aktivan = 0;
    for (new k = 0; k < sizeof gLsTabPogled; k++) if (gLsTabPogled[k] == pogled) aktivan = k;
    for (new k = 0; k < sizeof gLsTab; k++)
    {
        new broj = gLsTabPogled[k] == LS_PRIJATELJI ? zahtjeva : gLsTabPogled[k] == LS_PORUKE ? poruka : 0;
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
        case LS_GRUPE: LsGrupe(playerid);
        case LS_GRUPA: LsGrupa(playerid);
        case LS_TRAZI: LsTrazi(playerid);
        case LS_CLANOVI: LsClanovi(playerid);
        case LS_DODAJ: LsDodaj(playerid);
        default: LsFeed(playerid);
    }
    new n = gLsAvatara[playerid];
    new PlayerText:pokriv = gPc[playerid][PTD_LSPOKRIV + 7];
    PlayerTextDrawSetPos(playerid, pokriv, PC_X + PcPX(140.0), PC_Y + PcPY(LS_RED_Y + float(n) * LS_RED_H - 1.0));
    PlayerTextDrawTextSize(playerid, pokriv, 52.0 * PC_SX, (207.0 - LS_RED_Y - float(n) * LS_RED_H) * PC_SY);
    PlayerTextDrawShow(playerid, pokriv);
}


static LsOtvori(playerid, pogled, cilj = 0)
{
    gLsPogled[playerid] = pogled;
    gLsCilj[playerid] = cilj;
    gLsOdabran[playerid] = 0;
    gPcPotvrda[playerid] = PP_NISTA;
    PcLifeStalker(playerid);
    return 1;
}

static PcJavi(playerid, bool:greska, const tekst[])
{
    if (!gPcOtvoren[playerid]) return SendClientMessage(playerid, greska ? COLOR_ERROR : COLOR_INFO, tekst);
    new s[200];
    format(s, sizeof s, "%s%s", greska ? ("~r~") : ("~g~"), tekst);
    PcObavijest(playerid, s);
    return 1;
}

static PcPricekaj(playerid, sekundi)
{
    new s[64];
    format(s, sizeof s, T(playerid, "Pricekajte jos %d s."), sekundi);
    return PcJavi(playerid, true, s);
}

static LsOsvjezi(pogled, cilj, osim = INVALID_PLAYER_ID)
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (i != osim && gPcOtvoren[i] && gPcEkran[i] == PCE_LIFESTALKER && gLsPogled[i] == pogled && (cilj == -1 || gLsCilj[i] == cilj)) PcLifeStalker(i);
}

static LsJaviDrugom(t, playerid, const poruka[])
{
    if (t == INVALID_PLAYER_ID || !gPcOtvoren[t]) return;
    new s[160];
    format(s, sizeof s, T(t, poruka), GetRPIme(playerid));
    PcObavijest(t, s);
    if (gPcEkran[t] == PCE_LIFESTALKER && (gLsPogled[t] == LS_PRIJATELJI || (gLsPogled[t] == LS_PROFIL && gLsCilj[t] == gAccountId[playerid]))) PcLifeStalker(t);
}

static bool:LsPauza(playerid)
{
    new sad = gettime();
    if (sad - gLsZadnje[playerid] < LS_PAUZA)
    {
        PcPricekaj(playerid, LS_PAUZA - (sad - gLsZadnje[playerid]));
        return true;
    }
    gLsZadnje[playerid] = sad;
    return false;
}

static LsZahtjev(playerid, acc)
{
    new me = gAccountId[playerid];
    if (!acc || acc == me) return PcJavi(playerid, true, T(playerid, "Korisnik nije pronadjen."));
    if (LsBlok(me, acc)) return PcJavi(playerid, true, T(playerid, "Korisnik nije pronadjen."));
    switch (LsOdnos(me, acc))
    {
        case LS_PRIJATELJ: return PcJavi(playerid, true, T(playerid, "Vec ste prijatelji."));
        case LS_POSLAO: return PcJavi(playerid, true, T(playerid, "Zahtjev je vec poslan."));
        case LS_PRIMIO: return LsPrihvati(playerid, acc);
    }
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_friends (a, b, created) VALUES (%d, %d, %d)", me, acc, gettime()));
    new t = IgracRacuna(acc);
    LsJaviDrugom(t, playerid, "~p~LifeStalker~n~~w~%s vam je poslao zahtjev za prijateljstvo.");
    return PcJavi(playerid, false, T(playerid, "Zahtjev za prijateljstvo poslan."));
}

static LsPrihvati(playerid, acc)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE social_friends SET accepted = 1 WHERE a = %d AND b = %d", acc, gAccountId[playerid]));
    new t = IgracRacuna(acc);
    LsJaviDrugom(t, playerid, "~p~LifeStalker~n~~w~%s je prihvatio vas zahtjev za prijateljstvo.");
    return PcJavi(playerid, false, T(playerid, "Sada ste prijatelji."));
}

static LsUkloni(playerid, acc)
{
    new me = gAccountId[playerid];
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_friends WHERE (a = %d AND b = %d) OR (a = %d AND b = %d)", me, acc, acc, me));
    new t = IgracRacuna(acc);
    if (t != INVALID_PLAYER_ID && gPcOtvoren[t] && gPcEkran[t] == PCE_LIFESTALKER && (gLsPogled[t] == LS_PRIJATELJI || (gLsPogled[t] == LS_PROFIL && gLsCilj[t] == me))) PcLifeStalker(t);
}

static LsKlik(playerid, PlayerText:td)
{
    new me = gAccountId[playerid];
    for (new k = 0; k < sizeof gLsTab; k++)
    {
        if (td != gPc[playerid][PTD_LSTAB + k]) continue;
        new pogled = gLsTabPogled[k];
        LsOtvori(playerid, pogled, pogled == LS_PROFIL ? me : 0);
        if (pogled == LS_TRAZI && !gLsTrazi[playerid][0]) return PcUnos(playerid, PC_TRAZI, T(playerid, "Unesite ime ili dio imena:"));
        return 1;
    }
    for (new r = 0; r < OBJAVA_REDOVA; r++)
    {
        new v = gPcObjava[playerid][r];
        if (td != gPc[playerid][PTD_OBJAVA + r] || !v) continue;
        switch (gLsPogled[playerid])
        {
            case LS_FEED, LS_PROFIL: return LsOtvori(playerid, LS_OBJAVA, v);
            case LS_OBJAVA:
            {
                if (v > 0) return LsOtvori(playerid, LS_PROFIL, v);
                gLsOdabran[playerid] = gLsOdabran[playerid] == -v ? 0 : -v;
                gPcPotvrda[playerid] = PP_NISTA;
                PcLifeStalker(playerid);
                return 1;
            }
            case LS_PORUKE: return LsOtvori(playerid, LS_RAZGOVOR, v);
            case LS_GRUPE: return LsOtvori(playerid, LS_GRUPA, v);
            case LS_GRUPA, LS_TRAZI: return LsOtvori(playerid, LS_PROFIL, v);
            case LS_DODAJ: return LsGrupaDodaj(playerid, gLsCilj[playerid], v);
            case LS_CLANOVI:
            {
                new naziv[LS_GRUPA_IME + 1];
                new grp = gLsCilj[playerid];
                if (LsGrupaPodaci(grp, naziv, sizeof naziv) != me || v == me) return LsOtvori(playerid, LS_PROFIL, v);
                if (gLsOdabran[playerid] != v)
                {
                    gLsOdabran[playerid] = v;
                    PcLifeStalker(playerid);
                    return 1;
                }
                gLsOdabran[playerid] = 0;
                DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_group_members WHERE grp = %d AND account = %d", grp, v));
                PcLifeStalker(playerid);
                LsOsvjezi(LS_CLANOVI, grp, playerid);
                LsOsvjezi(LS_DODAJ, grp, playerid);
                new t = IgracRacuna(v);
                if (t != INVALID_PLAYER_ID && gPcOtvoren[t] && gPcEkran[t] == PCE_LIFESTALKER && gLsCilj[t] == grp && gLsPogled[t] == LS_GRUPA) PcLifeStalker(t);
                return PcJavi(playerid, false, T(playerid, "Clan je uklonjen iz grupe."));
            }
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
    for (new i = 0; i < 4; i++)
    {
        if (td != gPc[playerid][PTD_AKCIJA + i]) continue;
        new tekst[160], cilj = gLsCilj[playerid];
        switch (gLsPogled[playerid])
        {
            case LS_FEED:
            {
                if (!i) return LsUnosObjave(playerid);
                gLsSamoPrijatelji[playerid] = !gLsSamoPrijatelji[playerid];
                PcLifeStalker(playerid);
            }
            case LS_OBJAVA:
            {
                if (i == 1)
                {
                    format(tekst, sizeof tekst, T(playerid, "Vas komentar (najvise %d znakova):"), OBJAVA_LEN);
                    return PcUnos(playerid, PC_KOMENTAR, tekst);
                }
                if (i == 2) return LsPodijeli(playerid, cilj);
                if (i == 3)
                {
                    new kom = gLsOdabran[playerid];
                    if (!PcPotvrdi(playerid, i, kom ? PP_KOMENTAR : PP_OBJAVA)) return 1;
                    if (!kom)
                    {
                        LsObrisiObjavu(playerid);
                        PcLifeStalker(playerid);
                        return 1;
                    }
                    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_comments WHERE id = %d AND account = %d", kom, me));
                    gLsOdabran[playerid] = 0;
                    gPcPotvrda[playerid] = PP_NISTA;
                    PcLifeStalker(playerid);
                    LsOsvjezi(LS_OBJAVA, cilj, playerid);
                    LsOsvjezi(LS_FEED, -1, playerid);
                    return PcJavi(playerid, false, T(playerid, "Komentar je obrisan."));
                }
                new DBResult:res = DB_ExecuteQuery(gDB, "SELECT 1 FROM social_likes WHERE post = %d AND account = %d", cilj, me);
                new bool:ima = DB_GetRowCount(res) > 0;
                DB_FreeResultSet(res);
                if (ima) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_likes WHERE post = %d AND account = %d", cilj, me));
                else DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_likes (post, account) VALUES (%d, %d)", cilj, me));
                PcLifeStalker(playerid);
                LsOsvjezi(LS_OBJAVA, cilj, playerid);
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
                if (LsBlok(me, cilj)) return PcJavi(playerid, true, T(playerid, "Ne mozete poslati poruku ovom korisniku."));
                format(tekst, sizeof tekst, T(playerid, "Vasa poruka (najvise %d znakova):"), OBJAVA_LEN);
                return PcUnos(playerid, PC_PORUKA, tekst);
            }
            case LS_PROFIL:
            {
                if (cilj == me)
                {
                    if (!i) LsUnosObjave(playerid);
                    return 1;
                }
                if (!i)
                {
                    if (LsBlok(me, cilj)) return PcJavi(playerid, true, T(playerid, "Ne mozete poslati poruku ovom korisniku."));
                    return LsOtvori(playerid, LS_RAZGOVOR, cilj);
                }
                new bool:prijatelj = !LsBlokirao(cilj, me) && (LsOdnos(me, cilj) != LS_NISTA || !LsBlokirao(me, cilj));
                if (i == 1 && prijatelj)
                {
                    switch (LsOdnos(me, cilj))
                    {
                        case LS_PRIJATELJ, LS_POSLAO: LsUkloni(playerid, cilj);
                        case LS_PRIMIO: LsPrihvati(playerid, cilj);
                        default: LsZahtjev(playerid, cilj);
                    }
                }
                else if (LsBlokirao(me, cilj))
                {
                    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_blocks WHERE a = %d AND b = %d", me, cilj));
                    PcJavi(playerid, false, T(playerid, "Korisnik je odblokiran."));
                }
                else
                {
                    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO social_blocks (a, b) VALUES (%d, %d)", me, cilj));
                    LsUkloni(playerid, cilj);
                    PcJavi(playerid, false, T(playerid, "Korisnik je blokiran."));
                }
                PcLifeStalker(playerid);
            }
            case LS_GRUPE: if (!i) return PcUnos(playerid, PC_GRUPA_IME, T(playerid, "Naziv nove grupe (najvise 24 znaka):"));
            case LS_GRUPA:
            {
                if (i == 2) return LsOtvori(playerid, LS_DODAJ, cilj);
                if (i) return LsOtvori(playerid, LS_CLANOVI, cilj);
                format(tekst, sizeof tekst, T(playerid, "Vasa poruka (najvise %d znakova):"), OBJAVA_LEN);
                return PcUnos(playerid, PC_GRUPA_PORUKA, tekst);
            }
            case LS_CLANOVI:
            {
                new naziv[LS_GRUPA_IME + 1], vlasnik = LsGrupaPodaci(cilj, naziv, sizeof naziv);
                if (!vlasnik || !LsClan(cilj, me)) return LsOtvori(playerid, LS_GRUPE);
                if (vlasnik != me)
                {
                    if (i) return LsOtvori(playerid, LS_GRUPA, cilj);
                    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_group_members WHERE grp = %d AND account = %d", cilj, me));
                    LsOsvjezi(LS_CLANOVI, cilj, playerid);
                    PcJavi(playerid, false, T(playerid, "Napustili ste grupu."));
                    return LsOtvori(playerid, LS_GRUPE);
                }
                new akcija = !i ? 0 : i + 1;
                switch (akcija)
                {
                    case 0: return LsOtvori(playerid, LS_DODAJ, cilj);
                    case 2:
                    {
                        if (!PcPotvrdi(playerid, i, PP_GRUPA)) return 1;
                        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_group_messages WHERE grp = %d", cilj));
                        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_group_members WHERE grp = %d", cilj));
                        DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_groups WHERE id = %d", cilj));
                        LsOsvjezi(LS_GRUPA, cilj, playerid);
                        LsOsvjezi(LS_CLANOVI, cilj, playerid);
                        PcJavi(playerid, false, T(playerid, "Grupa je obrisana."));
                        return LsOtvori(playerid, LS_GRUPE);
                    }
                    default: return LsOtvori(playerid, LS_GRUPA, cilj);
                }
            }
            case LS_TRAZI: if (!i) return PcUnos(playerid, PC_TRAZI, T(playerid, "Unesite ime ili dio imena:"));
            case LS_DODAJ: return LsOtvori(playerid, LS_CLANOVI, cilj);
        }
        return 1;
    }
    return 0;
}

static LsObrisiObjavu(playerid)
{
    new post = gLsCilj[playerid], DBResult:res = DB_ExecuteQuery(gDB, "SELECT account FROM social_posts WHERE id = %d", post), autor = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    if (autor != gAccountId[playerid]) return;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_likes WHERE post IN (SELECT id FROM social_posts WHERE id = %d OR shared = %d)", post, post));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_comments WHERE post IN (SELECT id FROM social_posts WHERE id = %d OR shared = %d)", post, post));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM social_posts WHERE id = %d OR shared = %d", post, post));
    gLsPogled[playerid] = LS_FEED;
    gLsCilj[playerid] = 0;
    LsOsvjezi(LS_FEED, -1, playerid);
    LsOsvjezi(LS_OBJAVA, post, playerid);
    LsOsvjezi(LS_PROFIL, autor, playerid);
    PcJavi(playerid, false, T(playerid, "Objava je obrisana."));
}

static LsPodijeli(playerid, post)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT CASE WHEN shared > 0 THEN shared ELSE id END, account FROM social_posts WHERE id = %d", post);
    if (!DB_GetRowCount(res)) return DB_FreeResultSet(res);
    new izvor = DB_GetFieldInt(res, 0), me = gAccountId[playerid];
    DB_FreeResultSet(res);
    res = DB_ExecuteQuery(gDB, "SELECT account FROM social_posts WHERE id = %d", izvor);
    new autor = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    if (!autor || LsPauza(playerid)) return 0;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_posts (account, text, created, shared) VALUES (%d, '', %d, %d)", me, gettime(), izvor));
    LsOsvjezi(LS_FEED, -1, playerid);
    LsOsvjezi(LS_PROFIL, me, playerid);
    new t = IgracRacuna(autor);
    if (autor != me && t != INVALID_PLAYER_ID && gPcOtvoren[t])
    {
        new obavijest[144];
        format(obavijest, sizeof obavijest, T(t, "~p~LifeStalker~n~~w~%s je podijelio vasu objavu."), GetRPIme(playerid));
        PcObavijest(t, obavijest);
    }
    return PcJavi(playerid, false, T(playerid, "Objava je podijeljena s prijateljima."));
}

static LsGrupaJavi(grp, playerid, const tekst[])
{
    new naziv[LS_GRUPA_IME + 1], kratko[48], obavijest[200];
    LsGrupaPodaci(grp, naziv, sizeof naziv);
    strcat(kratko, tekst);
    TdCisti(kratko);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT account FROM social_group_members WHERE grp = %d", grp);
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new t = IgracRacuna(DB_GetFieldInt(res, 0));
        if (t == INVALID_PLAYER_ID || t == playerid || !gPcOtvoren[t]) continue;
        if (gPcEkran[t] == PCE_LIFESTALKER && gLsPogled[t] == LS_GRUPA && gLsCilj[t] == grp)
        {
            PcLifeStalker(t);
            continue;
        }
        format(obavijest, sizeof obavijest, "~p~%s~n~~w~%s: %s", naziv, GetRPIme(playerid), kratko);
        PcObavijest(t, obavijest);
    }
    DB_FreeResultSet(res);
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
        case PC_TRAZI:
        {
            gLsTrazi[playerid][0] = EOS;
            strcat(gLsTrazi[playerid], inputtext, 21);
            RemoveBoje(gLsTrazi[playerid]);
            gLsPogled[playerid] = LS_TRAZI;
            return 1;
        }
        case PC_GRUPA_IME:
        {
            new naziv[LS_GRUPA_IME + 1];
            strcat(naziv, inputtext);
            RemoveBoje(naziv);
            if (!naziv[0] || LsPauza(playerid)) return 1;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_groups (name, owner, created) VALUES ('%q', %d, %d)", naziv, me, sad));
            new grp = ZadnjiId();
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_group_members (grp, account) VALUES (%d, %d)", grp, me));
            gLsPogled[playerid] = LS_GRUPA;
            gLsCilj[playerid] = grp;
            return 1;
        }
        case PC_PORUKA_KOME:
        {
            new acc = LsRacun(inputtext);
            if (!acc || acc == me) return PcJavi(playerid, true, T(playerid, "Korisnik nije pronadjen."));
            if (LsBlok(me, acc)) return PcJavi(playerid, true, T(playerid, "Ne mozete poslati poruku ovom korisniku."));
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
        LsOsvjezi(LS_OBJAVA, post, playerid);
        LsOsvjezi(LS_FEED, -1, playerid);
        new t = IgracRacuna(autor);
        if (autor != me && t != INVALID_PLAYER_ID)
        {
            new obavijest[144];
            format(obavijest, sizeof obavijest, T(t, "~p~LifeStalker~n~~w~%s je komentirao vasu objavu."), GetRPIme(playerid));
            PcObavijest(t, obavijest);
        }
        return 1;
    }
    if (akcija == PC_GRUPA_PORUKA)
    {
        new grp = gLsCilj[playerid];
        if (!LsClan(grp, me)) return 1;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_group_messages (grp, sender, text, created) VALUES (%d, %d, '%q', %d)", grp, me, tekst, sad));
        LsGrupaJavi(grp, playerid, tekst);
        return 1;
    }
    new p = gLsCilj[playerid];
    if (LsBlok(me, p)) return PcJavi(playerid, true, T(playerid, "Ne mozete poslati poruku ovom korisniku."));
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
    if (ekran != PCE_POCETNA) gPcOtvorene[playerid] |= 1 << ekran;
    PcStart(playerid, false);
    PcJezikMeni(playerid, false);
    PcRadnaPovrsina(playerid, ekran == PCE_POCETNA);
    PcTrakaOsvjezi(playerid);
    PcSakrij(playerid, PTD_AKCIJA, 4);
    gPcPotvrda[playerid] = PP_NISTA;
    for (new i = 0; i < 2; i++) PcAkcijaPoz(playerid, i, 180.0 + float(i) * 100.0, 90.0);
    PcSakrij(playerid, PTD_DIONICA, sizeof gDionicaIme);
    PcSakrij(playerid, PTD_GRAF, 1 + SVIJECA * 2);
    PcSakrij(playerid, PTD_OBJAVA, OBJAVA_REDOVA);
    PcSakrij(playerid, PTD_KOL, 3);
    PcSakrij(playerid, PTD_LSTAB, sizeof gLsTab);
    PcSakrij(playerid, PTD_LSAVATAR, PTD_KLAD - PTD_LSAVATAR);
    PcSakrij(playerid, PTD_KLAD, KLAD_TD);
    PcSakrij(playerid, PTD_KARTICA, 5);
    PcSakrij(playerid, PTD_MREZA, 3);
    if (ekran == PCE_POCETNA)
    {
        for (new i = PTD_PROZOR; i <= PTD_SADRZAJ; i++) PlayerTextDrawHide(playerid, gPc[playerid][i]);
        return;
    }
    PlayerTextDrawColour(playerid, gPc[playerid][PTD_PROZOR + 2], gPcIkonaBoja[ekran]);
    for (new i = PTD_PROZOR; i <= PTD_ZATVORI; i++) PlayerTextDrawShow(playerid, gPc[playerid][i]);
    PcPostavi(playerid, PTD_NASLOV, T(playerid, gPcAplikacije[ekran]));
    PlayerTextDrawSetPos(playerid, gPc[playerid][PTD_SADRZAJ], PC_X + PcPX(88.0), PC_Y + PcPY(ekran == PCE_DIONICE ? 52.0 : 42.0));
    for (new i = 0; i < 4; i++) PlayerTextDrawBoxColour(playerid, gPc[playerid][PTD_AKCIJA + i], ekran == PCE_DIONICE ? (i ? 0xC62828FF : 0x2E7D32FF) : ekran == PCE_LIFESTALKER ? 0xE91E63FF : 0x0078D4FF);
    switch (ekran)
    {
        case PCE_BANKA:
        {
            if (!gInv[playerid][ITEM_KARTICA]) strcat(tekst, T(playerid, "Nemate bankovnu karticu."));
            else if (!BankaTekst(playerid, tekst)) strcat(tekst, T(playerid, "Nemate bankovni racun."));
            else
            {
                BankaPovijest(playerid, tekst, sizeof tekst, 9);
                new broj[24];
                format(broj, sizeof broj, "**** **** **** %04d", gAccountId[playerid] % 10000);
                PlayerTextDrawSetString(playerid, gPc[playerid][PTD_KARTICA + 3], broj);
                for (new k = 0; k < 5; k++) PlayerTextDrawShow(playerid, gPc[playerid][PTD_KARTICA + k]);
                PcPostavi(playerid, PTD_AKCIJA, T(playerid, "Posalji novac"));
                PcPostavi(playerid, PTD_AKCIJA + 1, T(playerid, "Otplati kredit"));
            }
        }
        case PCE_OGLASI:
        {
            new oglas[48], ime[MAX_PLAYER_NAME + 1], broj[16];
            if (gPcMojiOglasi[playerid])
            {
                new DBResult:moji = DB_ExecuteQuery(gDB, "SELECT id, substr(text, 1, 44) FROM ads WHERE account = %d AND created > %d ORDER BY id DESC LIMIT %d", gAccountId[playerid], gettime() - OGLAS_TRAJE, OBJAVA_REDOVA);
                new n = DB_GetRowCount(moji);
                for (new r = 0; r < n; r++, DB_SelectNextRow(moji))
                {
                    new id = DB_GetFieldInt(moji, 0);
                    DB_GetFieldString(moji, 1, oglas, sizeof oglas);
                    TdCisti(oglas);
                    format(red, sizeof red, "%s%s", oglas, id == gPcOglasOdabran[playerid] ? (" ~r~<") : (""));
                    gPcObjava[playerid][r] = id;
                    PcPostavi(playerid, PTD_OBJAVA + r, red);
                }
                DB_FreeResultSet(moji);
                if (n) strcat(tekst, "_");
                else strcat(tekst, T(playerid, "Nemate aktivnih oglasa."));
                PcPostavi(playerid, PTD_AKCIJA, T(playerid, "Svi oglasi"));
                if (gPcOglasOdabran[playerid]) PcPostavi(playerid, PTD_AKCIJA + 1, T(playerid, "Obrisi oglas"));
                PcPostavi(playerid, PTD_SADRZAJ, tekst);
                return;
            }
            new DBResult:res = DB_ExecuteQuery(gDB, "SELECT substr(ad.text, 1, 44), replace(a.name, '_', ' '), IFNULL((SELECT number FROM phone_numbers WHERE account = ad.account), '-') FROM ads ad JOIN accounts a ON a.id = ad.account WHERE ad.created > %d ORDER BY ad.id DESC LIMIT 8", gettime() - OGLAS_TRAJE);
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
            PcPostavi(playerid, PTD_AKCIJA + 1, T(playerid, "Moji oglasi"));
        }
        case PCE_DIONICE:
        {
            PlayerTextDrawShow(playerid, gPc[playerid][PTD_GRAF]);
            for (new g = 0; g < 3; g++) PlayerTextDrawShow(playerid, gPc[playerid][PTD_MREZA + g]);
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
    gLsTrazi[playerid][0] = EOS;
    gLsSamoPrijatelji[playerid] = false;
    PcUcitajKomade(playerid);
    PcKutija(playerid, 0.0, 0.0, 400.0, 248.0, 0x111111FF);
    PcKutija(playerid, 180.0, 248.0, 40.0, 8.0, 0x222222FF);
    PcKutija(playerid, 140.0, 256.0, 120.0, 4.0, 0x222222FF);
    PcKutija(playerid, 199.0, 1.0, 2.0, 2.0, 0x2A2A2AFF);
    PcKutija(playerid, 388.0, 245.0, 4.0, 1.5, 0x4CD964FF);
    PcTekst(playerid, 200.0, 244.3, "KALTRIX", 0x777777FF, 0.1, 0.35, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    gPcPozOd[playerid] = gPcBroj[playerid];
    PcKutija(playerid, 4.0, 4.0, 392.0, 220.0, 0x0B4F8AFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 110.0, 0x1366ADFF);
    PcKutija(playerid, 4.0, 4.0, 392.0, 50.0, 0x1A7BC8FF);
    PcTekst(playerid, 290.0, 110.0, "Monterey", 0xFFFFFF22, 0.9, 3.0, TEXT_DRAW_FONT_0, TEXT_DRAW_ALIGN_CENTER);
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        new Float:y = 12.0 + float(i - 1) * 42.0, Float:x = 16.0;
        gPc[playerid][PTD_APP + i] = PcKutija(playerid, x, y, 30.0, 30.0, gPcIkonaBoja[i]);
        PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_APP + i], true);
        for (new di = 0; di < sizeof gPcIkonaDio; di++)
            if (gPcIkonaDio[di][piApp] == i) gPcIkonaTd[playerid][di] = PcSlika(playerid, x + gPcIkonaDio[di][piX], y + gPcIkonaDio[di][piY], gPcIkonaDio[di][piW], gPcIkonaDio[di][piH], gPcIkonaDio[di][piBoja], gPcIkonaSlika[gPcIkonaDio[di][piSlika]]);
        new PlayerText:ime = gPc[playerid][PTD_APPIME + i] = PcTekst(playerid, x + 15.0, y + 32.0, T(playerid, gPcAplikacije[i]), 0xFFFFFFFF, 0.15, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
        PlayerTextDrawSetShadow(playerid, ime, 1);
    }
    gPcPozDo[playerid] = gPcBroj[playerid];
    PcKutija(playerid, 4.0, 224.0, 392.0, 20.0, 0x0D141CF2);
    gPc[playerid][PTD_START] = PcKutija(playerid, 8.0, 226.0, 16.0, 16.0, 0x00000001);
    PlayerTextDrawSetSelectable(playerid, gPc[playerid][PTD_START], true);
    for (new i = 0; i < 4; i++) PcKutija(playerid, 10.0 + float(i % 2) * 7.0, 228.0 + float(i / 2) * 7.0, 5.5, 5.5, 0x4FC3F7FF);
    gPc[playerid][PTD_JEZIK] = PcGumb(playerid, 346.0, 229.5, 16.0, 0xFFFFFF01, T(playerid, "HR"));
    gPc[playerid][PTD_SAT] = PcTekst(playerid, 390.0, 226.0, vrijeme, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    gPc[playerid][PTD_DATUM] = PcTekst(playerid, 390.0, 234.0, datum, 0xFFFFFFFF, 0.16, 0.75, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    gPcPuno = true;
    gPc[playerid][PTD_PROZOR] = PcKutija(playerid, 80.0, 20.0, 300.0, 186.0, 0x00000000);
    gPc[playerid][PTD_PROZOR + 1] = PcKutija(playerid, 80.0, 20.0, 300.0, 186.0, 0xFFFFFFFF);
    gPc[playerid][PTD_PROZOR + 2] = PcKutija(playerid, 80.0, 20.0, 300.0, 16.0, 0xF0F0F0FF);
    gPc[playerid][PTD_NASLOV] = PcTekst(playerid, 88.0, 23.0, "_", 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_PROZOR_MIN] = PcGumb(playerid, 358.0, 23.0, 12.0, 0xF0F0F001, "-", 0xFFFFFFFF);
    gPc[playerid][PTD_ZATVORI] = PcGumb(playerid, 371.0, 23.0, 16.0, 0xE81123FF, "X");
    gPc[playerid][PTD_KARTICA] = PcKutija(playerid, 298.0, 44.0, 76.0, 46.0, 0x1B5E20FF);
    gPc[playerid][PTD_KARTICA + 1] = PcKutija(playerid, 304.0, 58.0, 11.0, 8.0, 0xFFD54FFF);
    gPc[playerid][PTD_KARTICA + 2] = PcTekst(playerid, 304.0, 47.0, "MONTEREY BANK", 0xFFFFFFFF, 0.12, 0.55, TEXT_DRAW_FONT_2);
    gPc[playerid][PTD_KARTICA + 3] = PcTekst(playerid, 304.0, 70.0, "_", 0xFFFFFFFF, 0.13, 0.6, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_KARTICA + 4] = PcTekst(playerid, 304.0, 79.0, GetRPIme(playerid), 0xC8E6C9FF, 0.12, 0.55, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_SADRZAJ] = PcTekst(playerid, 88.0, 42.0, "_", 0x202020FF, 0.18, 0.95, TEXT_DRAW_FONT_1);
    for (new k = 0; k < sizeof gDionicaIme; k++) gPc[playerid][PTD_DIONICA + k] = PcGumb(playerid, 110.0 + float(k) * 60.0, 40.0, 54.0, 0xDDDDDDFF, gDionicaKratko[k], 0x202020FF);
    gPc[playerid][PTD_GRAF] = PcKutija(playerid, GRAF_X - 2.0, GRAF_Y - 4.0, SVIJECA * GRAF_SIRINA + 4.0, GRAF_H + 8.0, 0xF5F5F5FF);
    for (new mr = 0; mr < 3; mr++) gPc[playerid][PTD_MREZA + mr] = PcKutija(playerid, GRAF_X, GRAF_Y + GRAF_H * float(mr + 1) / 4.0, SVIJECA * GRAF_SIRINA, 0.5, 0xDDDDDDFF);
    for (new i = 0; i < SVIJECA; i++) gPc[playerid][PTD_TIJELO + i] = PcKutija(playerid, GRAF_X, GRAF_Y, 1.0, 1.0, 0x00000000);
    for (new i = 0; i < SVIJECA; i++) gPc[playerid][PTD_FITILJ + i] = PcKutija(playerid, GRAF_X, GRAF_Y, 1.0, 1.0, 0x00000000);
    for (new r = 0; r < OBJAVA_REDOVA; r++) gPc[playerid][PTD_LSAVATAR + r] = PcModel(playerid, LS_SLOT_X - LS_AV_S / 2.0, LS_RED_Y + float(r) * LS_RED_H - 1.0, LS_AV_S, LS_AV_S);
    gPc[playerid][PTD_LSPROFIL] = PcModel(playerid, LS_PR_X - LS_PR_S / 2.0, 40.0, LS_PR_S, LS_PR_S);
    gPc[playerid][PTD_LSPOKRIV] = PcKutija(playerid, 80.0, 36.0, 60.0, 170.0, 0xE91E63FF);
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
    for (new i = 0; i < 4; i++) gPc[playerid][PTD_AKCIJA + i] = PcGumb(playerid, 180.0 + float(i % 2) * 100.0, 190.0, 90.0, 0x0078D4FF);
    gPcPuno = false;
    gPc[playerid][PTD_LSPOKRIV + 1] = PcKutija(playerid, 30.0, 224.0, 131.0, 3.5, 0x0D141CFF);
    gPc[playerid][PTD_LSPOKRIV + 2] = PcKutija(playerid, 30.0, 227.5, 131.0, 16.5, 0x0D141CFF);
    gPc[playerid][PTD_LSPOKRIV + 3] = PcKutija(playerid, 30.0, 244.0, 131.0, 4.0, 0x111111FF);
    for (new i = 1; i < sizeof gPcAplikacije; i++) gPc[playerid][PTD_TRAKA + i] = PcGumb(playerid, 58.0, 229.0, 52.0, 0xFFFFFF16, T(playerid, gPcAplikacije[i]));
    for (new j = 0; j < BROJ_JEZIKA; j++) gPc[playerid][PTD_JEZIK_MENI + j] = PcGumb(playerid, 346.0, 152.0 + 14.0 * float(j), 54.0, j == gJezik[playerid] ? 0x2C5A8CF2 : 0x1B2430F2, gPcJezici[j]);
    new ime[MAX_PLAYER_NAME + 1], slovo[2];
    format(ime, sizeof ime, "%s", GetRPIme(playerid));
    format(slovo, sizeof slovo, "%c", ime[0]);
    gPc[playerid][PTD_START_PANEL] = PcKutija(playerid, 4.0, 100.0, 150.0, 124.0, 0x1B2430F2);
    gPc[playerid][PTD_START_PANEL + 1] = PcKutija(playerid, 4.0, 100.0, 150.0, 26.0, 0x24344AFF);
    gPc[playerid][PTD_START_PANEL + 2] = PcSlika(playerid, 9.0, 104.0, 18.0, 18.0, 0x4FC3F7FF, "LD_POOL:ball");
    gPc[playerid][PTD_START_PANEL + 3] = PcTekst(playerid, 18.0, 107.5, slovo, 0x0D141CFF, 0.25, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    gPc[playerid][PTD_START_PANEL + 4] = PcTekst(playerid, 32.0, 104.0, ime, 0xFFFFFFFF, 0.18, 0.85, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_START_PANEL + 5] = PcTekst(playerid, 32.0, 114.0, "KALTRIX OS 11", 0x8AA0B6FF, 0.13, 0.6, TEXT_DRAW_FONT_1);
    for (new k = 0; k < 5; k++)
    {
        gPc[playerid][PTD_START_PANEL + 6 + k] = PcKutija(playerid, 10.0, 131.0 + 15.0 * float(k), 8.0, 8.0, gPcIkonaBoja[k + 1]);
        new PlayerText:red = gPc[playerid][PTD_START_PANEL + 11 + k] = PcRed(playerid, 24.0, 130.5 + 15.0 * float(k), 124.0);
        PlayerTextDrawBoxColour(playerid, red, 0x24344AFF);
        PlayerTextDrawColour(playerid, red, 0xFFFFFFFF);
        PlayerTextDrawSetString(playerid, red, T(playerid, gPcAplikacije[k + 1]));
    }
    gPc[playerid][PTD_START_PANEL + 16] = PcKutija(playerid, 8.0, 206.0, 142.0, 0.6, 0x3A4656FF);
    gPc[playerid][PTD_UGASI] = PcGumb(playerid, 115.0, 211.0, 66.0, 0xC62828FF, T(playerid, "Ugasi"));
    for (new i = 0; i < gPcBroj[playerid]; i++) PlayerTextDrawShow(playerid, gPcTd[playerid][i]);
    gPc[playerid][PTD_OBAVIJEST] = PcTekst(playerid, 266.0, 192.0, "_", 0xFFFFFFFF, 0.1, 0.15, TEXT_DRAW_FONT_1);
    gPc[playerid][PTD_OBAVIJEST_TEKST] = PcTekst(playerid, 266.0, 196.0, "_", 0xFFFFFFFF, 0.14, 0.7, TEXT_DRAW_FONT_1);
    for (new k = 0; k < 2; k++)
    {
        new PlayerText:td = gPc[playerid][k ? PTD_OBAVIJEST_TEKST : PTD_OBAVIJEST];
        PlayerTextDrawUseBox(playerid, td, true);
        PlayerTextDrawBoxColour(playerid, td, k ? 0x1B2430F8 : 0x4FC3F7FF);
        PlayerTextDrawTextSize(playerid, td, PC_X + 390.0, 0.0);
    }
    gPcObavijestDo[playerid] = 0;
    gPcOtvorene[playerid] = 0;
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

bool:PcOtvoren(playerid)
{
    return gPcOtvoren[playerid];
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
    for (new k = 0; k < 5; k++)
    {
        if (td != gPc[playerid][PTD_START_PANEL + 11 + k]) continue;
        PcEkran(playerid, k + 1);
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
        gPcOtvorene[playerid] &= ~(1 << gPcEkran[playerid]);
        PcEkran(playerid, PCE_POCETNA);
        return 1;
    }
    if (td == gPc[playerid][PTD_PROZOR_MIN])
    {
        PcEkran(playerid, PCE_POCETNA);
        return 1;
    }
    if (td == gPc[playerid][PTD_JEZIK])
    {
        PcJezikMeni(playerid, !gPcJezikMeni[playerid]);
        return 1;
    }
    for (new j = 0; j < BROJ_JEZIKA; j++)
    {
        if (td != gPc[playerid][PTD_JEZIK_MENI + j]) continue;
        JezikPromijeni(playerid, j);
        PcZatvori(playerid);
        PcOtvori(playerid);
        return 1;
    }
    for (new i = 1; i < sizeof gPcAplikacije; i++)
    {
        if (td != gPc[playerid][PTD_TRAKA + i]) continue;
        PcEkran(playerid, gPcEkran[playerid] == i ? PCE_POCETNA : i);
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
    if (gPcEkran[playerid] == PCE_OGLASI && gPcMojiOglasi[playerid])
    {
        for (new r = 0; r < OBJAVA_REDOVA; r++)
        {
            if (td != gPc[playerid][PTD_OBJAVA + r] || !gPcObjava[playerid][r]) continue;
            gPcOglasOdabran[playerid] = gPcOglasOdabran[playerid] == gPcObjava[playerid][r] ? 0 : gPcObjava[playerid][r];
            PcEkran(playerid, PCE_OGLASI);
            return 1;
        }
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
                if (gPcMojiOglasi[playerid])
                {
                    if (!i)
                    {
                        gPcMojiOglasi[playerid] = false;
                        PcEkran(playerid, PCE_OGLASI);
                        return 1;
                    }
                    if (!PcPotvrdi(playerid, i, PP_OGLAS)) return 1;
                    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM ads WHERE id = %d AND account = %d", gPcOglasOdabran[playerid], gAccountId[playerid]));
                    gPcOglasOdabran[playerid] = 0;
                    PcJavi(playerid, false, T(playerid, "Oglas je obrisan."));
                    for (new k = 0; k < MAX_PLAYERS; k++) if (gPcOtvoren[k] && gPcEkran[k] == PCE_OGLASI) PcEkran(k, PCE_OGLASI);
                    return 1;
                }
                if (i)
                {
                    gPcMojiOglasi[playerid] = true;
                    gPcOglasOdabran[playerid] = 0;
                    PcEkran(playerid, PCE_OGLASI);
                    return 1;
                }
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
    new d = gPcDionica[playerid], ima = PcKomada(playerid, d), cijena = gDionica[d], poruka[128];
    if (n <= 0) return 1;
    if (kupnja)
    {
        new iznos = DionicaKupnja(n, cijena);
        if (ima + n > DIONICA_MAX_KOM)
        {
            format(poruka, sizeof poruka, T(playerid, "Najvise %d dionica po tvrtki."), DIONICA_MAX_KOM);
            return PcJavi(playerid, true, poruka);
        }
        if (!BankaPromijeni(playerid, -iznos, "Kupnja dionica")) return 1;
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO stock_holdings (account, stock, shares, cost) VALUES (%d, %d, %d, %d)", gAccountId[playerid], d, ima + n, gPcTrosak[playerid][d] + iznos));
        gPcKomadaPamti[playerid][d] = ima + n;
        gPcTrosak[playerid][d] += iznos;
        format(poruka, sizeof poruka, T(playerid, "Kupili ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
        return PcJavi(playerid, false, poruka);
    }
    if (n > ima) return PcJavi(playerid, true, T(playerid, "Nemate toliko dionica."));
    new iznos = DionicaProdaja(n, cijena);
    if (!BankaPromijeni(playerid, iznos, "Prodaja dionica")) return 1;
    if (ima == n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM stock_holdings WHERE account = %d AND stock = %d", gAccountId[playerid], d));
    gPcTrosak[playerid][d] = ima == n ? 0 : gPcTrosak[playerid][d] - gPcTrosak[playerid][d] * n / ima;
    if (ima != n) DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE stock_holdings SET shares = %d, cost = %d WHERE account = %d AND stock = %d", ima - n, gPcTrosak[playerid][d], gAccountId[playerid], d));
    gPcKomadaPamti[playerid][d] = ima - n;
    format(poruka, sizeof poruka, T(playerid, "Prodali ste %d dionica (%s) za $%d."), n, gDionicaIme[d], iznos);
    return PcJavi(playerid, false, poruka);
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

static KladGrupa(tip)
{
    return tip <= 3 ? 0 : tip <= 5 ? 1 : 2;
}

static KladNaListicu(playerid, const id[], grupa)
{
    for (new k = 0; k < gKladN[playerid]; k++) if (KladGrupa(gKladTip[playerid][k]) == grupa && !strcmp(gKladPar[playerid][k], id)) return k;
    return -1;
}

static bool:KladOdabran(playerid, const id[], tip)
{
    new k = KladNaListicu(playerid, id, KladGrupa(tip));
    return k != -1 && gKladTip[playerid][k] == tip;
}

static KladUkupno(playerid)
{
    new koef = 100;
    for (new k = 0; k < gKladN[playerid]; k++) koef = KladPomnozi(koef, gKladKoefPar[playerid][k]);
    return koef;
}

static KladPomnozi(koef, o)
{
    new k = koef * o / 100;
    return k > KLAD_MAX_KOEF ? KLAD_MAX_KOEF : k;
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
        PlayerTextDrawLetterSize(playerid, td, PcW(0.11), PcH(0.6));
    }
    gPc[playerid][PTD_KLAD + KT_MEC] = PcTekst(playerid, 144.0, 51.0, "_", 0x202020FF, 0.14, 0.7, TEXT_DRAW_FONT_1);
    for (new r = 0; r < KLAD_RED; r++)
    {
        new PlayerText:td = gPc[playerid][PTD_KLAD + KT_RED + r] = PcRed(playerid, 142.0, 63.0 + float(r) * 26.5, 156.0);
        PlayerTextDrawLetterSize(playerid, td, PcW(0.14), PcH(0.7));
        PlayerTextDrawBoxColour(playerid, td, 0xF4F6F9FF);
        for (new o = 0; o < 3; o++)
        {
            td = gPc[playerid][PTD_KLAD + KT_KOEF + r * 3 + o] = PcGumb(playerid, 168.0 + float(o) * 53.0, 81.0 + float(r) * 26.5, 49.0, KLAD_SIVO, "_", 0x202020FF);
            PlayerTextDrawLetterSize(playerid, td, PcW(0.14), PcH(0.72));
        }
    }
    gPc[playerid][PTD_KLAD + KT_STR_L] = PcGumb(playerid, 203.0, 198.0, 12.0, 0x0078D4FF, "~<~");
    gPc[playerid][PTD_KLAD + KT_STR] = PcGumb(playerid, 221.0, 198.0, 18.0, 0xDDDDDDFF, "_", 0x202020FF);
    gPc[playerid][PTD_KLAD + KT_STR_D] = PcGumb(playerid, 239.0, 198.0, 12.0, 0x0078D4FF, "~>~");
    gPc[playerid][PTD_KLAD + KT_NASLOV] = PcTekst(playerid, 306.0, 39.0, "_", KLAD_ZUTO, 0.17, 0.85, TEXT_DRAW_FONT_2);
    for (new k = 0; k < KLAD_PAROVA; k++)
    {
        new PlayerText:td = gPc[playerid][PTD_KLAD + KT_PAR + k] = PcRed(playerid, 304.0, 52.0 + float(k) * 12.0, 74.0);
        PlayerTextDrawLetterSize(playerid, td, PcW(0.12), PcH(0.6));
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
        new na = KladNaListicu(playerid, gKladRed[playerid][r], 0);
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
    new o[3][3], lin[3];
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
        KladGumb(playerid, 0, i, gumb, o[0][i], KladOdabran(playerid, gKladMecId[playerid], i + 1));
    }
    if (lin[1])
    {
        KladLinija(lin[1], l, sizeof l, false);
        format(red, sizeof red, T(playerid, "Ukupno (%s)"), l);
        KladPokazi(playerid, KT_RED + 1, red);
        KladKoef(o[1][0], k, sizeof k);
        format(gumb, sizeof gumb, T(playerid, "Vise  %s"), k);
        KladGumb(playerid, 1, 0, gumb, o[1][0], KladOdabran(playerid, gKladMecId[playerid], 4));
        KladKoef(o[1][1], k, sizeof k);
        format(gumb, sizeof gumb, T(playerid, "Manje  %s"), k);
        KladGumb(playerid, 1, 1, gumb, o[1][1], KladOdabran(playerid, gKladMecId[playerid], 5));
    }
    if (o[2][0])
    {
        KladLinija(lin[2], l, sizeof l);
        format(red, sizeof red, T(playerid, "Hendikep (domacin %s)"), l);
        KladPokazi(playerid, KT_RED + 2, red);
        KladKoef(o[2][0], k, sizeof k);
        format(gumb, sizeof gumb, "%s %s  %s", T(playerid, "Dom"), l, k);
        KladGumb(playerid, 2, 0, gumb, o[2][0], KladOdabran(playerid, gKladMecId[playerid], 6));
        KladLinija(-lin[2], l, sizeof l);
        KladKoef(o[2][1], k, sizeof k);
        format(gumb, sizeof gumb, "%s %s  %s", T(playerid, "Gost"), l, k);
        KladGumb(playerid, 2, 1, gumb, o[2][1], KladOdabran(playerid, gKladMecId[playerid], 7));
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
    new k = KladNaListicu(playerid, id, KladGrupa(tip));
    if (k != -1 && gKladTip[playerid][k] == tip)
    {
        KladMakni(playerid, k);
        return;
    }
    if (k == -1)
    {
        if (gKladN[playerid] >= KLAD_PAROVA)
        {
            new poruka[96];
            format(poruka, sizeof poruka, T(playerid, "Na listicu moze biti najvise %d parova."), KLAD_PAROVA);
            PcJavi(playerid, true, poruka);
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
    if (!gKladN[playerid]) return PcJavi(playerid, true, T(playerid, "Listic je prazan. Odaberite utakmice."));
    new ulog = gKladUlozi[gKladUlog[playerid]], koef = 100;
    for (new i = 0; i < gKladN[playerid]; i++)
    {
        new DBResult:res = DB_ExecuteQuery(gDB, "SELECT CASE %d WHEN 1 THEN odd1 WHEN 2 THEN oddx WHEN 3 THEN odd2 WHEN 4 THEN odd_over WHEN 5 THEN odd_under WHEN 6 THEN odd_sph ELSE odd_spa END, CASE %d WHEN 4 THEN tline WHEN 5 THEN tline WHEN 6 THEN sline WHEN 7 THEN -sline ELSE 0 END FROM bet_events WHERE id = '%q' AND status = 0 AND start > %d", gKladTip[playerid][i], gKladTip[playerid][i], gKladPar[playerid][i], gettime() + KLAD_ZATVARA_S);
        new o = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0, linija = DB_GetRowCount(res) ? DB_GetFieldInt(res, 1) : 0;
        DB_FreeResultSet(res);
        if (o <= 100) return PcJavi(playerid, true, T(playerid, "Jedna od utakmica je vec pocela. Uklonite je s listica."));
        gKladKoefPar[playerid][i] = o;
        gKladLin[playerid][i] = linija;
        koef = KladPomnozi(koef, o);
    }
    new dobitak = KladDobitak(ulog, koef);
    if (!BankaPromijeni(playerid, -ulog, "Uplata oklade")) return 1;
    BlagajnaDodaj(ulog);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bet_slips (account, stake, odds, payout, status, created) VALUES (%d, %d, %d, %d, 0, %d)", gAccountId[playerid], ulog, koef, dobitak, gettime()));
    new listic = ZadnjiId();
    for (new i = 0; i < gKladN[playerid]; i++)
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO bet_legs (slip, event, pick, odd, line) VALUES (%d, '%q', %d, %d, %d)", listic, gKladPar[playerid][i], gKladTip[playerid][i], gKladKoefPar[playerid][i], gKladLin[playerid][i]));
    gKladN[playerid] = 0;
    new poruka[128];
    format(poruka, sizeof poruka, T(playerid, "Listic #%d uplacen: ulog $%d, moguci dobitak $%d."), listic, ulog, dobitak);
    return PcJavi(playerid, false, poruka);
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
        for (new k = 0, nk = DB_GetRowCount(res); k < nk; k++, DB_SelectNextRow(res)) koef = KladPomnozi(koef, DB_GetFieldInt(res, 0));
        DB_FreeResultSet(res);
        new dobitak = KladDobitak(iznos[i], koef);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE bet_slips SET status = 1, payout = %d, settled = %d WHERE id = %d", dobitak, sad, id[i]));
        BlagajnaDodaj(-dobitak);
        if (!BankaUplati(racun[i], dobitak, "Dobitak oklade"))
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
            if (BankaOtplati(playerid, n)) PcJavi(playerid, false, T(playerid, "Transakcija uspjesna."));
        }
        case PC_KOLICINA:
        {
            new n = ProvjeriBroj(inputtext) && strlen(inputtext) <= 5 ? strval(inputtext) : 0;
            if (n >= 1 && n <= DIONICA_MAX_KOM) gPcKolicina[playerid] = n;
        }
        case PC_KOMENTAR, PC_PORUKA, PC_PRIJATELJ, PC_PORUKA_KOME, PC_TRAZI, PC_GRUPA_IME, PC_GRUPA_PORUKA: LsUnos(playerid, gPcAkcija[playerid], inputtext);
        case PC_OBJAVA:
        {
            new tekst[OBJAVA_LEN + 1], sad = gettime();
            if (sad - gObjavaZadnja[playerid] < OBJAVA_PAUZA)
            {
                PcPricekaj(playerid, OBJAVA_PAUZA - (sad - gObjavaZadnja[playerid]));
                return PcNatrag(playerid);
            }
            strcat(tekst, inputtext);
            RemoveBoje(tekst);
            if (!tekst[0]) return PcNatrag(playerid);
            gObjavaZadnja[playerid] = sad;
            DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO social_posts (account, text, created) VALUES (%d, '%q', %d)", gAccountId[playerid], tekst, sad));
            LsOsvjezi(LS_FEED, -1, playerid);
            LsOsvjezi(LS_PROFIL, gAccountId[playerid], playerid);
        }
        case PC_OGLAS:
        {
            new tekst[OGLAS_LEN + 1], broj[16] = "-", sad = gettime();
            if (sad - gOglasZadnji[playerid] < OGLAS_PAUZA)
            {
                PcPricekaj(playerid, OGLAS_PAUZA - (sad - gOglasZadnji[playerid]));
                return PcNatrag(playerid);
            }
            strcat(tekst, inputtext);
            RemoveBoje(tekst);
            if (!tekst[0] || !BankaPromijeni(playerid, -OGLAS_CIJENA, "Oglas")) return PcNatrag(playerid);
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
