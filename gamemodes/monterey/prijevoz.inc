#define PZ_DOMET            2.5
#define PZ_ISTOVAR          9.0
#define PZ_PONUDA           4
#define PZ_UDALJENOST_MIN   400.0
#define PZ_CESTA_MAX        60.0
#define PZ_PO_KM            60
#define PZ_KAMION_HP        300.0
#define PZ_BOJA             0xD35400FF
#define PZ_ILEGALNO_BOJA    0xC0392BFF
#define PZ_OTKACENO_S       180
#define PZ_ROK_DODATAK      60
#define PZ_BRZINA_MALI      14.0
#define PZ_BRZINA_VELIKI    11.0
#define PZ_BONUS_POSTO      20
#define PZ_KASNI_S          5
#define PZ_KASNI_MAX        50
#define PZ_ILEGALNO_POSTO   25
#define PZ_ZAUZETO          7.0
#define PZ_PREVRNUT_S       3
#define PZ_PAUZA_S          6
#define PZ_SAVJET_MS        3000
#define PZ_STOJI            0.05
#define PZ_NIVOA            5
#define PZ_ILEGALNO         8
#define PZ_GORIVO_MODEL     584
#define PZ_GORIVO_S         30
#define PZ_UTOVAR_MAX       240
#define PZ_TURA             3
#define PZ_SCENA_MAX        40
#define PZ_ISTOVAR_GORIVO_S 20
#define PZ_ISTOVAR_MAX      150
#define PZ_RADNIKA          2
#define PZ_STRAG_DOMET      2.4
#define PZ_DUCAN_DOMET      2.6
#define PZ_NOSI_SLOT        2
#define PZ_SANDUK_MODEL     2912
#define PZ_SCENA_BRZINA     8.0
#define PZ_VOZILO_Z         1.6

enum { PZF_NEMA, PZF_KACI, PZF_LUKA, PZF_UTOVAR, PZF_VOZI, PZF_POVRATAK, PZF_SCENA, PZF_ISTOVAR, PZF_VRACA }
enum { PZI_RUCNO, PZI_RADNICI, PZI_GORIVO }
enum { PZM_ODJECA, PZM_TRGOVINA, PZM_RESTORAN, PZM_NAMJESTAJ, PZM_ZLATARA, PZM_ORUZARNICA, PZM_TUNING, PZM_PUMPA, PZM_BROJ }
enum E_PZ_TERET { ptIme[32], ptNivo, ptKamion, ptPrikolica, ptBaza, ptMjesto, Float:ptDomet, ptXp }

static const gPrijevozTeret[][E_PZ_TERET] = {
    { "Odjeca", 1, 499, 0, 120, PZM_ODJECA, 1800.0, 1 },
    { "Namirnice", 1, 499, 0, 120, PZM_TRGOVINA, 1800.0, 1 },
    { "Hrana za restorane", 2, 414, 0, 180, PZM_RESTORAN, 3000.0, 2 },
    { "Namjestaj", 2, 456, 0, 200, PZM_NAMJESTAJ, 3000.0, 2 },
    { "Nakit", 3, 403, 591, 320, PZM_ZLATARA, 0.0, 3 },
    { "Oruzje i municija", 3, 403, 435, 350, PZM_ORUZARNICA, 0.0, 3 },
    { "Auto dijelovi", 4, 514, 591, 420, PZM_TUNING, 0.0, 4 },
    { "Gorivo", 4, 514, 584, 480, PZM_PUMPA, 0.0, 4 },
    { "Krijumcarena roba", PZ_NIVOA, 515, 435, 1400, -1, 0.0, 5 }
};
static const gPrijevozXp[PZ_NIVOA] = { 0, 15, 45, 100, 180 };
static const Float:gPrijevozZadano[3][4] = { { 2123.0557, -2274.9922, 19.1219, 46.2314 }, { 2186.3396, -2261.8914, 12.42, 227.1055 }, { 2214.293, -2236.873, 13.2, 315.0 } };
static const Float:gPrijevozHangar[4] = { 2173.937, -2249.679, 12.23, 225.0 };
static const Float:gPrijevozKamera[6] = { 2195.575, -2249.255, 18.0, 2177.968, -2254.134, 13.95 };
static const Float:gPrijevozKameraPut[2][3] = { { 2195.575, -2253.498, 17.0 }, { 2204.767, -2266.933, 16.0 } };
static const Float:gPrijevozKameraKraj[3] = { 2186.340, -2261.891, 13.9 };
static const Float:gPrijevozRuta[] = {
    2173.937, -2249.679, 12.230, 2178.180, -2253.922, 12.230, 2181.009, -2256.751, 12.300,
    2186.340, -2261.891, 12.420
};
static bool:gPrijevozOdustaje[MAX_PLAYERS];
static Float:gPrijevozPonudaDucan[MAX_PLAYERS][PZ_PONUDA][3], Float:gPrijevozDucan[MAX_PLAYERS][3], gPrijevozIstovarOd[MAX_PLAYERS], gPrijevozPreostalo[MAX_PLAYERS], gPrijevozIsporuceno[MAX_PLAYERS], bool:gPrijevozNosi[MAX_PLAYERS];
static gPrijevozRadnik[MAX_PLAYERS][PZ_RADNIKA], gPrijevozRadnikStanje[MAX_PLAYERS][PZ_RADNIKA];
static const gPrijevozRadnikSkin[] = { 16, 27, 260, 153 };
static gPrijevozScenaIgrac = INVALID_PLAYER_ID, gPrijevozScenaLik = INVALID_LIK, gPrijevozScenaTimer, gPrijevozScenaOd;

static Float:gPrijevozPoz[4][4], bool:gPrijevozIma[4], STREAMER_TAG_3D_TEXT_LABEL:gPrijevozTekst;
static gPrijevozFaza[MAX_PLAYERS], gPrijevozVrsta[MAX_PLAYERS], gPrijevozKamion[MAX_PLAYERS], gPrijevozPrikolica[MAX_PLAYERS], gPrijevozRok[MAX_PLAYERS], gPrijevozOtkaceno[MAX_PLAYERS];
static gPrijevozPrevrnut[MAX_PLAYERS], gPrijevozSavjet[MAX_PLAYERS], gPrijevozPauza[MAX_PLAYERS], bool:gPrijevozDojava[MAX_PLAYERS], Float:gPrijevozCilj[MAX_PLAYERS][3], Float:gPrijevozPut[MAX_PLAYERS], gPrijevozZona[MAX_PLAYERS][28];
static gPrijevozUtovar = INVALID_PLAYER_ID, gPrijevozUtovarOd, bool:gPrijevozViljuskar, gPrijevozUtovareno[MAX_PLAYERS], STREAMER_TAG_OBJECT:gPrijevozTeretObj[MAX_PLAYERS][PZ_TURA];
static gPrijevozPonuda[MAX_PLAYERS][PZ_PONUDA], Float:gPrijevozPonudaPoz[MAX_PLAYERS][PZ_PONUDA][3], gPrijevozPonudaBroj[MAX_PLAYERS];

#define PZ_VRACA_BRZINA     5.0
#define PZ_MENI_X           30.0
#define PZ_MENI_Y           110.0
#define PZ_MENI_W           300.0
#define PZ_MENI_RED         14.0
#define PZ_MENI_CEKAJ_MS    350
#define PZ_MENI_PONOVI_MS   120
enum { PZT_POZADINA, PZT_NASLOV, PZT_TRAKA, PZT_PODNASLOV, PZT_DNO, PZT_POMOC, PZT_ODABERI, PZT_NATRAG, PZT_RED }
static PlayerText:gPrijevozMeniTd[MAX_PLAYERS][PZT_RED + PZ_PONUDA * 3], gPrijevozMeniBroj[MAX_PLAYERS], bool:gPrijevozMeniMis[MAX_PLAYERS], gPrijevozMeniIdx[MAX_PLAYERS];
static gPrijevozMeniStrana[MAX_PLAYERS], gPrijevozMeniSmjer[MAX_PLAYERS], gPrijevozMeniPonovi[MAX_PLAYERS];

InitPrijevoz()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_trucker (id INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS trucker_xp (account INTEGER PRIMARY KEY, xp INTEGER NOT NULL DEFAULT 0, runs INTEGER NOT NULL DEFAULT 0)"));
    for (new id = 0; id < sizeof gPrijevozZadano; id++)
    {
        for (new k = 0; k < 4; k++) gPrijevozPoz[id][k] = gPrijevozZadano[id][k];
        gPrijevozIma[id] = true;
    }
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM job_trucker WHERE id IN (3, 4)");
    for (new r = 0, n = DB_GetRowCount(res); r < n; r++, DB_SelectNextRow(res))
    {
        new id = DB_GetFieldInt(res, 0) - 1;
        for (new k = 0; k < 4; k++) gPrijevozPoz[id][k] = DB_GetFieldFloat(res, k + 1);
        gPrijevozIma[id] = true;
    }
    DB_FreeResultSet(res);
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        gPrijevozKamion[i] = gPrijevozPrikolica[i] = INVALID_VEHICLE_ID;
        for (new w = 0; w < PZ_RADNIKA; w++) gPrijevozRadnik[i][w] = INVALID_LIK;
    }
    if (gPrijevozIma[0]) PrijevozDepoStvori();
    assert(PrijevozNivo(0) == 1 && PrijevozNivo(14) == 1 && PrijevozNivo(15) == 2 && PrijevozNivo(9999) == PZ_NIVOA);
    assert(PrijevozIsplata(1000, 0, 0) == 1200 && PrijevozIsplata(1000, 100, 0) == 800 && PrijevozIsplata(1000, 9999, 0) == 500 && PrijevozIsplata(1000, 0, 100) == 0);
}

static PrijevozDepoStvori()
{
    if (IsValidDynamic3DTextLabel(gPrijevozTekst)) DestroyDynamic3DTextLabel(gPrijevozTekst);
    gPrijevozTekst = CreateDynamic3DTextLabel("Prijevoz tereta\nALT", PZ_BOJA, gPrijevozPoz[0][0], gPrijevozPoz[0][1], gPrijevozPoz[0][2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gPrijevozTekst, PZ_DOMET);
}

PrijevozLokacija(&Float:x, &Float:y, &Float:z)
{
    if (!gPrijevozIma[0]) return 0;
    x = gPrijevozPoz[0][0];
    y = gPrijevozPoz[0][1];
    z = gPrijevozPoz[0][2];
    return 1;
}

static PrijevozNivo(xp)
{
    new n = 1;
    while (n < PZ_NIVOA && xp >= gPrijevozXp[n]) n++;
    return n;
}

static PrijevozIsplata(osnova, kasni, steta)
{
    new posto = kasni <= 0 ? 100 + PZ_BONUS_POSTO : 100 - clamp(kasni / PZ_KASNI_S, 0, PZ_KASNI_MAX);
    return osnova * posto / 100 * (100 - steta) / 100;
}

static PrijevozOsnova(t, Float:put)
{
    return gPrijevozTeret[t][ptBaza] + floatround(put / 1000.0 * float(PZ_PO_KM));
}

static PrijevozXp(playerid)
{
    new xp = 0, DBResult:res = DB_ExecuteQuery(gDB, "SELECT xp FROM trucker_xp WHERE account = %d", gAccountId[playerid]);
    if (DB_GetRowCount(res)) xp = DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    return xp;
}

static bool:PrijevozNaDepou(playerid, Float:r = PZ_DOMET)
{
    return gPrijevozIma[0] && !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gPrijevozPoz[0][0], gPrijevozPoz[0][1], gPrijevozPoz[0][2]);
}

static PrijevozMjesto(m, i, &Float:x, &Float:y, &Float:z)
{
    new Float:a;
    switch (m)
    {
        case PZM_ODJECA: return OdjecaLokacija(i, x, y, z, a);
        case PZM_TRGOVINA: return TrgovinaLokacija(i, x, y, z, a);
        case PZM_RESTORAN: return RestoranLokacija(i, x, y, z, a);
        case PZM_NAMJESTAJ: return NamjestajLokacija(i, x, y, z, a);
        case PZM_ZLATARA: return ZlataraLokacija(i, x, y, z, a);
        case PZM_ORUZARNICA: return OruzarnicaLokacija(i, x, y, z, a);
        case PZM_TUNING: return TuningLokacija(i, x, y, z, a);
        case PZM_PUMPA: return PumpaLokacija(i, x, y, z);
    }
    return -1;
}

static bool:PrijevozNasumicno(m, Float:domet, &Float:x, &Float:y, &Float:z, &Float:dx, &Float:dy, &Float:dz)
{
    new n = 0, r, Float:tx, Float:ty, Float:tz;
    for (new i = 0; (r = PrijevozMjesto(m, i, tx, ty, tz)) != -1; i++)
    {
        if (r <= 0) continue;
        new Float:d = VectorSize(tx - gPrijevozPoz[3][0], ty - gPrijevozPoz[3][1], 0.0);
        if (d < PZ_UDALJENOST_MIN || (domet > 0.0 && d > domet) || random(++n)) continue;
        x = tx, y = ty, z = tz;
    }
    if (!n) return false;
    dx = x, dy = y, dz = z;
    new Float:cx, Float:cy, Float:cz;
    if (Lik_Cesta(x, y, z, cx, cy, cz) <= PZ_CESTA_MAX) x = cx, y = cy, z = cz;
    return true;
}

static PrijevozPonude(playerid)
{
    if (!gPrijevozIma[1] || !gPrijevozIma[3]) return ObavijestGreska(playerid, T(playerid, "Prijevoz tereta jos nije postavljen.")), 1;
    new xp = PrijevozXp(playerid), nivo = PrijevozNivo(xp), dostupno[sizeof gPrijevozTeret], nd = 0, p = 0;
    for (new t = 0; t < sizeof gPrijevozTeret; t++) if (t != PZ_ILEGALNO && gPrijevozTeret[t][ptNivo] <= nivo) dostupno[nd++] = t;
    new bool:ilegalno = nivo >= PZ_NIVOA && random(100) < PZ_ILEGALNO_POSTO;
    for (new k = 0; k < PZ_PONUDA * 3 && p < PZ_PONUDA; k++)
    {
        new t = ilegalno ? PZ_ILEGALNO : dostupno[random(nd)], m = gPrijevozTeret[t][ptMjesto], Float:x, Float:y, Float:z, Float:dx, Float:dy, Float:dz;
        if (m == -1) m = random(PZM_BROJ);
        if (!PrijevozNasumicno(m, gPrijevozTeret[t][ptDomet], x, y, z, dx, dy, dz)) continue;
        gPrijevozPonudaDucan[playerid][p][0] = dx;
        gPrijevozPonudaDucan[playerid][p][1] = dy;
        gPrijevozPonudaDucan[playerid][p][2] = dz;
        ilegalno = false;
        gPrijevozPonuda[playerid][p] = t;
        gPrijevozPonudaPoz[playerid][p][0] = x;
        gPrijevozPonudaPoz[playerid][p][1] = y;
        gPrijevozPonudaPoz[playerid][p][2] = z;
        p++;
    }
    gPrijevozPonudaBroj[playerid] = p;
    if (!p) return ObavijestGreska(playerid, T(playerid, "Trenutno nema dostupnih tura.")), 1;
    PrijevozMeniOtvori(playerid, nivo, xp);
    return 1;
}

static PrijevozMeniOtvori(playerid, nivo, xp)
{
    PrijevozMeniZatvori(playerid);
    new n = gPrijevozPonudaBroj[playerid], tekst[96], KEY:tipke, gore, strana;
    gPrijevozMeniTd[playerid][PZT_POZADINA] = TdSlika(playerid, PZ_MENI_X, PZ_MENI_Y, PZ_MENI_W, 40.0, 0x1E5AA8F0);
    gPrijevozMeniTd[playerid][PZT_NASLOV] = TdTekst(playerid, PZ_MENI_X + PZ_MENI_W / 2.0, PZ_MENI_Y + 6.0, T(playerid, "Prijevoz tereta"), 0xFFFFFFFF, 0.6, 2.6, TEXT_DRAW_FONT_3, TEXT_DRAW_ALIGN_CENTER);
    gPrijevozMeniTd[playerid][PZT_TRAKA] = TdSlika(playerid, PZ_MENI_X, PZ_MENI_Y + 40.0, PZ_MENI_W, PZ_MENI_RED, 0x000000F0);
    format(tekst, sizeof tekst, T(playerid, "Nivo %d (%d XP)"), nivo, xp);
    gPrijevozMeniTd[playerid][PZT_PODNASLOV] = TdTekst(playerid, PZ_MENI_X + 4.0, PZ_MENI_Y + 42.0, tekst, 0x5B8DBFFF, 0.2, 1.0, TEXT_DRAW_FONT_2);
    for (new r = 0; r < n; r++)
    {
        new t = gPrijevozPonuda[playerid][r], Float:y = PZ_MENI_Y + 40.0 + PZ_MENI_RED * float(r + 1), Float:d = VectorSize(gPrijevozPonudaPoz[playerid][r][0] - gPrijevozPoz[3][0], gPrijevozPonudaPoz[playerid][r][1] - gPrijevozPoz[3][1], 0.0);
        gPrijevozMeniTd[playerid][PZT_RED + r * 3] = TdSlika(playerid, PZ_MENI_X, y, PZ_MENI_W, PZ_MENI_RED, 0x000000B4);
        format(tekst, sizeof tekst, "%s - %s", T(playerid, gPrijevozTeret[t][ptIme]), CestaZona(gPrijevozPonudaPoz[playerid][r][0], gPrijevozPonudaPoz[playerid][r][1]));
        new PlayerText:naziv = TdTekst(playerid, PZ_MENI_X + 5.0, y + 2.0, tekst, 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1);
        PlayerTextDrawTextSize(playerid, naziv, PZ_MENI_X + PZ_MENI_W - 2.0, PZ_MENI_RED - 2.0);
        PlayerTextDrawSetSelectable(playerid, naziv, true);
        gPrijevozMeniTd[playerid][PZT_RED + r * 3 + 1] = naziv;
        format(tekst, sizeof tekst, "%.1f km  $%d", d / 1000.0, PrijevozOsnova(t, d));
        gPrijevozMeniTd[playerid][PZT_RED + r * 3 + 2] = TdTekst(playerid, PZ_MENI_X + PZ_MENI_W - 5.0, y + 2.0, tekst, 0xFFFFFFFF, 0.22, 1.05, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_RIGHT);
    }
    new Float:dno = PZ_MENI_Y + 40.0 + PZ_MENI_RED * float(n + 1) + 4.0;
    gPrijevozMeniTd[playerid][PZT_DNO] = TdSlika(playerid, PZ_MENI_X, dno, PZ_MENI_W, 34.0, 0x000000D2);
    gPrijevozMeniTd[playerid][PZT_POMOC] = TdTekst(playerid, PZ_MENI_X + PZ_MENI_W / 2.0, dno + 22.0, T(playerid, "W/S: odabir  Lijevo: natrag  Desno: odaberi  Y: mis"), 0x8AA0B6FF, 0.16, 0.8, TEXT_DRAW_FONT_1, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawTextSize(playerid, gPrijevozMeniTd[playerid][PZT_POMOC], 10.0, PZ_MENI_W);
    PlayerTextDrawSetSelectable(playerid, gPrijevozMeniTd[playerid][PZT_POMOC], true);
    gPrijevozMeniTd[playerid][PZT_ODABERI] = TdTekst(playerid, PZ_MENI_X + PZ_MENI_W - 60.0, dno + 5.0, T(playerid, "Odaberi"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gPrijevozMeniTd[playerid][PZT_ODABERI], true);
    PlayerTextDrawBoxColour(playerid, gPrijevozMeniTd[playerid][PZT_ODABERI], 0x2E7D32FF);
    PlayerTextDrawTextSize(playerid, gPrijevozMeniTd[playerid][PZT_ODABERI], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gPrijevozMeniTd[playerid][PZT_ODABERI], true);
    gPrijevozMeniTd[playerid][PZT_NATRAG] = TdTekst(playerid, PZ_MENI_X + 60.0, dno + 5.0, T(playerid, "NATRAG"), 0xFFFFFFFF, 0.24, 1.1, TEXT_DRAW_FONT_2, TEXT_DRAW_ALIGN_CENTER);
    PlayerTextDrawUseBox(playerid, gPrijevozMeniTd[playerid][PZT_NATRAG], true);
    PlayerTextDrawBoxColour(playerid, gPrijevozMeniTd[playerid][PZT_NATRAG], 0x1E5AA8FF);
    PlayerTextDrawTextSize(playerid, gPrijevozMeniTd[playerid][PZT_NATRAG], 12.0, 90.0);
    PlayerTextDrawSetSelectable(playerid, gPrijevozMeniTd[playerid][PZT_NATRAG], true);
    for (new i = 0; i < PZT_RED; i++) PlayerTextDrawShow(playerid, gPrijevozMeniTd[playerid][i]);
    GetPlayerKeys(playerid, tipke, gore, strana);
    gPrijevozMeniBroj[playerid] = n;
    gPrijevozMeniIdx[playerid] = 0;
    gPrijevozMeniMis[playerid] = false;
    gPrijevozMeniStrana[playerid] = strana;
    gPrijevozMeniSmjer[playerid] = 0;
    TogglePlayerControllable(playerid, false);
    PrijevozMeniPrikazi(playerid);
}

static PrijevozMeniPrikazi(playerid)
{
    for (new r = 0; r < gPrijevozMeniBroj[playerid]; r++)
    {
        new bool:odabran = r == gPrijevozMeniIdx[playerid], boja = odabran ? 0x000000FF : (gPrijevozPonuda[playerid][r] == PZ_ILEGALNO ? 0xE74C3CFF : 0xFFFFFFFF);
        PlayerTextDrawColour(playerid, gPrijevozMeniTd[playerid][PZT_RED + r * 3], odabran ? 0xF0F0F0EE : 0x000000B4);
        PlayerTextDrawColour(playerid, gPrijevozMeniTd[playerid][PZT_RED + r * 3 + 1], boja);
        PlayerTextDrawColour(playerid, gPrijevozMeniTd[playerid][PZT_RED + r * 3 + 2], boja);
        for (new k = 0; k < 3; k++) PlayerTextDrawShow(playerid, gPrijevozMeniTd[playerid][PZT_RED + r * 3 + k]);
    }
}

static PrijevozMeniZatvori(playerid)
{
    new n = gPrijevozMeniBroj[playerid];
    if (!n) return;
    gPrijevozMeniBroj[playerid] = 0;
    for (new i = 0; i < PZT_RED + n * 3; i++) PlayerTextDrawDestroy(playerid, gPrijevozMeniTd[playerid][i]);
    if (gPrijevozMeniMis[playerid]) CancelSelectTextDraw(playerid);
    gPrijevozMeniMis[playerid] = false;
    TogglePlayerControllable(playerid, true);
}

static PrijevozMeniOdaberi(playerid)
{
    new p = gPrijevozMeniIdx[playerid];
    PrijevozMeniZatvori(playerid);
    if (p >= gPrijevozPonudaBroj[playerid] || gPrijevozFaza[playerid])
    {
        gPrijevozPonudaBroj[playerid] = 0;
        return;
    }
    if (IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
    PrijevozPocni(playerid, p);
}

PrijevozMeniAktivan(playerid)
{
    return gPrijevozMeniBroj[playerid] > 0;
}

PrijevozMeniEsc(playerid)
{
    gPrijevozMeniMis[playerid] = false;
}

PrijevozMeniKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gPrijevozMeniBroj[playerid]) return 0;
    if ((newkeys & KEY_YES) && !(oldkeys & KEY_YES))
    {
        gPrijevozMeniMis[playerid] = !gPrijevozMeniMis[playerid];
        if (gPrijevozMeniMis[playerid]) SelectTextDraw(playerid, 0x5B8DBFFF);
        else CancelSelectTextDraw(playerid);
    }
    return 1;
}

PrijevozMeniUpdate(playerid)
{
    if (!gPrijevozMeniBroj[playerid]) return;
    new KEY:tipke, gore, strana, smjer = 0, sad = GetTickCount();
    GetPlayerKeys(playerid, tipke, gore, strana);
    if (strana != gPrijevozMeniStrana[playerid])
    {
        gPrijevozMeniStrana[playerid] = strana;
        if (strana == KEY_RIGHT)
        {
            PrijevozMeniOdaberi(playerid);
            return;
        }
        if (strana == KEY_LEFT)
        {
            PrijevozMeniZatvori(playerid);
            return;
        }
    }
    if (gore < 0 || (tipke & KEY_SPRINT)) smjer = -1;
    else if (gore > 0 || (tipke & KEY_JUMP)) smjer = 1;
    if (!smjer)
    {
        gPrijevozMeniSmjer[playerid] = 0;
        return;
    }
    if (smjer == gPrijevozMeniSmjer[playerid] && sad < gPrijevozMeniPonovi[playerid]) return;
    gPrijevozMeniPonovi[playerid] = sad + (smjer == gPrijevozMeniSmjer[playerid] ? PZ_MENI_PONOVI_MS : PZ_MENI_CEKAJ_MS);
    gPrijevozMeniSmjer[playerid] = smjer;
    new n = gPrijevozMeniBroj[playerid];
    gPrijevozMeniIdx[playerid] = (gPrijevozMeniIdx[playerid] + smjer + n) % n;
    PrijevozMeniPrikazi(playerid);
}

PrijevozKlik(playerid, PlayerText:td)
{
    new n = gPrijevozMeniBroj[playerid];
    if (!n) return 0;
    if (td == gPrijevozMeniTd[playerid][PZT_POMOC])
    {
        gPrijevozMeniMis[playerid] = false;
        CancelSelectTextDraw(playerid);
    }
    else if (td == gPrijevozMeniTd[playerid][PZT_NATRAG]) PrijevozMeniZatvori(playerid);
    else if (td == gPrijevozMeniTd[playerid][PZT_ODABERI]) PrijevozMeniOdaberi(playerid);
    else for (new r = 0; r < n; r++)
    {
        if (td != gPrijevozMeniTd[playerid][PZT_RED + r * 3 + 1]) continue;
        if (r == gPrijevozMeniIdx[playerid]) PrijevozMeniOdaberi(playerid);
        else
        {
            gPrijevozMeniIdx[playerid] = r;
            PrijevozMeniPrikazi(playerid);
        }
        break;
    }
    return 1;
}

static bool:PrijevozZauzeto(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gPrijevozFaza[i]) continue;
        if (gPrijevozKamion[i] != INVALID_VEHICLE_ID && GetVehicleDistanceFromPoint(gPrijevozKamion[i], gPrijevozPoz[s][0], gPrijevozPoz[s][1], gPrijevozPoz[s][2]) < PZ_ZAUZETO) return true;
        if (gPrijevozPrikolica[i] != INVALID_VEHICLE_ID && GetVehicleDistanceFromPoint(gPrijevozPrikolica[i], gPrijevozPoz[s][0], gPrijevozPoz[s][1], gPrijevozPoz[s][2]) < PZ_ZAUZETO) return true;
    }
    return false;
}

static PrijevozPocni(playerid, p)
{
    new t = gPrijevozPonuda[playerid][p], model = gPrijevozTeret[t][ptPrikolica], k, tr = INVALID_VEHICLE_ID;
    gPrijevozPonudaBroj[playerid] = 0;
    if (model && !gPrijevozIma[2]) return ObavijestGreska(playerid, T(playerid, "Mjesto za prikolice jos nije postavljeno.")), 1;
    if (PrijevozZauzeto(1) || (model && PrijevozZauzeto(2))) return ObavijestGreska(playerid, T(playerid, "Mjesto za kamion je zauzeto, pricekajte malo.")), 1;
    if (gPrijevozScenaIgrac != INVALID_PLAYER_ID) return ObavijestGreska(playerid, T(playerid, "Hangar je trenutno zauzet, pricekajte malo.")), 1;
    k = CreateVehicle(gPrijevozTeret[t][ptKamion], gPrijevozHangar[0], gPrijevozHangar[1], gPrijevozHangar[2] + PZ_VOZILO_Z, gPrijevozHangar[3], 1, 1, -1);
    if (k == INVALID_VEHICLE_ID) return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih kamiona.")), 1;
    if (model)
    {
        tr = CreateVehicle(model, gPrijevozPoz[2][0], gPrijevozPoz[2][1], gPrijevozPoz[2][2] + 0.5, gPrijevozPoz[2][3], 1, 1, -1);
        if (tr == INVALID_VEHICLE_ID)
        {
            DestroyVehicle(k);
            return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih kamiona.")), 1;
        }
    }
    VoziloNapuni(k);
    gPrijevozKamion[playerid] = k;
    gPrijevozPrikolica[playerid] = tr;
    gPrijevozVrsta[playerid] = t;
    gPrijevozCilj[playerid][0] = gPrijevozPonudaPoz[playerid][p][0];
    gPrijevozCilj[playerid][1] = gPrijevozPonudaPoz[playerid][p][1];
    gPrijevozCilj[playerid][2] = gPrijevozPonudaPoz[playerid][p][2];
    for (new d = 0; d < 3; d++) gPrijevozDucan[playerid][d] = gPrijevozPonudaDucan[playerid][p][d];
    gPrijevozPut[playerid] = VectorSize(gPrijevozCilj[playerid][0] - gPrijevozPoz[3][0], gPrijevozCilj[playerid][1] - gPrijevozPoz[3][1], 0.0);
    gPrijevozZona[playerid][0] = EOS;
    strcat(gPrijevozZona[playerid], CestaZona(gPrijevozCilj[playerid][0], gPrijevozCilj[playerid][1]));
    gPrijevozDojava[playerid] = false;
    gPrijevozOtkaceno[playerid] = gPrijevozPrevrnut[playerid] = gPrijevozSavjet[playerid] = gPrijevozPauza[playerid] = 0;
    gPrijevozFaza[playerid] = PZF_SCENA;
    gPrijevozScenaOd = gettime();
    gPrijevozScenaIgrac = playerid;
    TogglePlayerControllable(playerid, false);
    Fade(playerid, 0);
    SetTimerEx("PrijevozScenaPocni", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

static PrijevozNakonKamiona(playerid)
{
    if (gPrijevozPrikolica[playerid] == INVALID_VEHICLE_ID)
    {
        PrijevozLuka(playerid);
        return;
    }
    gPrijevozFaza[playerid] = PZF_KACI;
    SetPlayerCheckpoint(playerid, gPrijevozPoz[2][0], gPrijevozPoz[2][1], gPrijevozPoz[2][2], 3.0);
    PrijevozPanel(playerid);
}

static PrijevozScenaKraj(playerid)
{
    if (gPrijevozScenaIgrac != playerid) return;
    if (gPrijevozScenaTimer) KillTimer(gPrijevozScenaTimer);
    gPrijevozScenaTimer = 0;
    if (gPrijevozScenaLik != INVALID_LIK) Lik_Ukloni(gPrijevozScenaLik);
    gPrijevozScenaLik = INVALID_LIK;
    gPrijevozScenaIgrac = INVALID_PLAYER_ID;
    MapaKamionDepoVrata(false, true);
}

static PrijevozScenaPrekini(playerid)
{
    PrijevozScenaKraj(playerid);
    if (!IsPlayerConnected(playerid)) return;
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    Fade(playerid, FADE_SLOJEVA + 1);
}

forward PrijevozScenaPocni(playerid, racun);
public PrijevozScenaPocni(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozFaza[playerid] != PZF_SCENA || gPrijevozScenaIgrac != playerid) return;
    new k = gPrijevozKamion[playerid];
    new lik = Lik_Stvori(GetPlayerSkin(playerid), gPrijevozHangar[0], gPrijevozHangar[1], gPrijevozHangar[2] + 1.0, gPrijevozHangar[3]);
    if (lik == INVALID_LIK)
    {
        PrijevozScenaPredaj(playerid, racun);
        return;
    }
    gPrijevozScenaLik = lik;
    Lik_Neranjiv(lik, true);
    VoziloSet(k, VP_MOTOR, 1);
    Lik_UVozilo(lik, k, 0);
    SetPlayerCameraPos(playerid, gPrijevozKamera[0], gPrijevozKamera[1], gPrijevozKamera[2]);
    SetPlayerCameraLookAt(playerid, gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], CAMERA_CUT);
    SetTimerEx("PrijevozOtkrij", 350, false, "ii", playerid, racun);
    MapaKamionDepoVrata(true);
    SetTimerEx("PrijevozScenaKreni", 2600, false, "ii", playerid, racun);
}

forward PrijevozScenaKreni(playerid, racun);
public PrijevozScenaKreni(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozScenaIgrac != playerid || gPrijevozScenaLik == INVALID_LIK) return;
    if (!Lik_VoziPutem(gPrijevozScenaLik, gPrijevozRuta, sizeof gPrijevozRuta / 3, PZ_SCENA_BRZINA))
    {
        PrijevozScenaStigao();
        return;
    }
    InterpolateCameraPos(playerid, gPrijevozKamera[0], gPrijevozKamera[1], gPrijevozKamera[2], gPrijevozKameraPut[0][0], gPrijevozKameraPut[0][1], gPrijevozKameraPut[0][2], 1800, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], 1800, CAMERA_MOVE);
    gPrijevozScenaTimer = SetTimerEx("PrijevozScenaKamera", 1800, false, "i", playerid);
}

forward PrijevozScenaKamera(playerid);
public PrijevozScenaKamera(playerid)
{
    gPrijevozScenaTimer = 0;
    if (gPrijevozScenaIgrac != playerid) return;
    InterpolateCameraPos(playerid, gPrijevozKameraPut[0][0], gPrijevozKameraPut[0][1], gPrijevozKameraPut[0][2], gPrijevozKameraPut[1][0], gPrijevozKameraPut[1][1], gPrijevozKameraPut[1][2], 2200, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], gPrijevozKameraKraj[0], gPrijevozKameraKraj[1], gPrijevozKameraKraj[2], 2200, CAMERA_MOVE);
}

static PrijevozScenaStigao()
{
    new playerid = gPrijevozScenaIgrac;
    if (playerid == INVALID_PLAYER_ID) return;
    if (gPrijevozScenaTimer) KillTimer(gPrijevozScenaTimer);
    gPrijevozScenaTimer = 0;
    MapaKamionDepoVrata(false, true);
    Fade(playerid, 0);
    if (gPrijevozFaza[playerid] == PZF_VRACA) SetTimerEx("PrijevozVracaGotov", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    else SetTimerEx("PrijevozScenaPredaj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
}

forward PrijevozVracaPocni(playerid, racun);
public PrijevozVracaPocni(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozFaza[playerid] != PZF_VRACA || gPrijevozScenaIgrac != playerid) return;
    new k = gPrijevozKamion[playerid];
    DisablePlayerCheckpoint(playerid);
    IgraObavijestUkloni(playerid);
    SetPlayerPos(playerid, gPrijevozPoz[0][0], gPrijevozPoz[0][1], gPrijevozPoz[0][2]);
    SetPlayerFacingAngle(playerid, gPrijevozPoz[0][3]);
    SetVehiclePos(k, gPrijevozRuta[sizeof gPrijevozRuta - 3], gPrijevozRuta[sizeof gPrijevozRuta - 2], gPrijevozRuta[sizeof gPrijevozRuta - 1] + PZ_VOZILO_Z);
    SetVehicleZAngle(k, gPrijevozHangar[3]);
    SetVehicleVelocity(k, 0.0, 0.0, 0.0);
    new lik = Lik_Stvori(GetPlayerSkin(playerid), gPrijevozRuta[sizeof gPrijevozRuta - 3], gPrijevozRuta[sizeof gPrijevozRuta - 2], gPrijevozRuta[sizeof gPrijevozRuta - 1] + 1.0, gPrijevozHangar[3]);
    if (lik == INVALID_LIK)
    {
        PrijevozVracaGotov(playerid, racun);
        return;
    }
    gPrijevozScenaLik = lik;
    Lik_Neranjiv(lik, true);
    VoziloSet(k, VP_MOTOR, 1);
    Lik_UVozilo(lik, k, 0);
    SetPlayerCameraPos(playerid, gPrijevozKameraPut[1][0], gPrijevozKameraPut[1][1], gPrijevozKameraPut[1][2]);
    SetPlayerCameraLookAt(playerid, gPrijevozKameraKraj[0], gPrijevozKameraKraj[1], gPrijevozKameraKraj[2], CAMERA_CUT);
    SetTimerEx("PrijevozOtkrij", 350, false, "ii", playerid, racun);
    MapaKamionDepoVrata(true);
    SetTimerEx("PrijevozVracaKreni", 1800, false, "ii", playerid, racun);
}

forward PrijevozVracaKreni(playerid, racun);
public PrijevozVracaKreni(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozScenaIgrac != playerid || gPrijevozScenaLik == INVALID_LIK) return;
    new Float:ruta[sizeof gPrijevozRuta], n = sizeof gPrijevozRuta / 3;
    for (new i = 0; i < n; i++) for (new d = 0; d < 3; d++) ruta[i * 3 + d] = gPrijevozRuta[(n - 1 - i) * 3 + d];
    if (!Lik_VoziPutem(gPrijevozScenaLik, ruta, n, PZ_VRACA_BRZINA, true, 1.0))
    {
        PrijevozScenaStigao();
        return;
    }
    InterpolateCameraPos(playerid, gPrijevozKameraPut[1][0], gPrijevozKameraPut[1][1], gPrijevozKameraPut[1][2], gPrijevozKameraPut[0][0], gPrijevozKameraPut[0][1], gPrijevozKameraPut[0][2], 2200, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, gPrijevozKameraKraj[0], gPrijevozKameraKraj[1], gPrijevozKameraKraj[2], gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], 2200, CAMERA_MOVE);
    gPrijevozScenaTimer = SetTimerEx("PrijevozVracaKamera", 2200, false, "i", playerid);
}

forward PrijevozVracaKamera(playerid);
public PrijevozVracaKamera(playerid)
{
    gPrijevozScenaTimer = 0;
    if (gPrijevozScenaIgrac != playerid) return;
    InterpolateCameraPos(playerid, gPrijevozKameraPut[0][0], gPrijevozKameraPut[0][1], gPrijevozKameraPut[0][2], gPrijevozKamera[0], gPrijevozKamera[1], gPrijevozKamera[2], 1800, CAMERA_MOVE);
    InterpolateCameraLookAt(playerid, gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], gPrijevozKamera[3], gPrijevozKamera[4], gPrijevozKamera[5], 1800, CAMERA_MOVE);
}

forward PrijevozVracaGotov(playerid, racun);
public PrijevozVracaGotov(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozFaza[playerid] != PZF_VRACA) return;
    PrijevozScenaKraj(playerid);
    gPrijevozFaza[playerid] = PZF_POVRATAK;
    PrijevozKraj(playerid);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetTimerEx("PrijevozOtkrij", 300, false, "ii", playerid, racun);
    PrijevozPonude(playerid);
}

PrijevozStigao(lik)
{
    if (lik == INVALID_LIK) return 0;
    if (lik == gPrijevozScenaLik)
    {
        PrijevozScenaStigao();
        return 1;
    }
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (gPrijevozFaza[i] != PZF_ISTOVAR) continue;
        for (new w = 0; w < PZ_RADNIKA; w++)
        {
            if (gPrijevozRadnik[i][w] != lik) continue;
            PrijevozRadnikStigao(i, w);
            return 1;
        }
    }
    return 0;
}

static PrijevozIstovarVrsta(playerid)
{
    new tr = gPrijevozPrikolica[playerid];
    if (tr == INVALID_VEHICLE_ID) return PZI_RUCNO;
    return GetVehicleModel(tr) == PZ_GORIVO_MODEL ? PZI_GORIVO : PZI_RADNICI;
}

static PrijevozStrag(playerid, &Float:x, &Float:y, &Float:z, Float:dalje = 0.8)
{
    new v = PrijevozTovar(playerid), Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    x += floatsin(a, degrees) * (sy / 2.0 + dalje);
    y -= floatcos(a, degrees) * (sy / 2.0 + dalje);
}

static PrijevozUzmiSanduk(playerid)
{
    if (gPrijevozPreostalo[playerid] <= 0) return;
    new n = --gPrijevozPreostalo[playerid];
    if (n < PZ_TURA && _:gPrijevozTeretObj[playerid][n])
    {
        DestroyDynamicObject(gPrijevozTeretObj[playerid][n]);
        gPrijevozTeretObj[playerid][n] = STREAMER_TAG_OBJECT:0;
    }
}

static PrijevozNosiMakni(playerid)
{
    if (!gPrijevozNosi[playerid]) return;
    gPrijevozNosi[playerid] = false;
    if (!IsPlayerConnected(playerid)) return;
    RemovePlayerAttachedObject(playerid, PZ_NOSI_SLOT);
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
}

static PrijevozIstovarMakni(playerid)
{
    PrijevozNosiMakni(playerid);
    for (new w = 0; w < PZ_RADNIKA; w++)
    {
        if (gPrijevozRadnik[playerid][w] != INVALID_LIK) Lik_Ukloni(gPrijevozRadnik[playerid][w]);
        gPrijevozRadnik[playerid][w] = INVALID_LIK;
    }
    if (gPrijevozFaza[playerid] == PZF_ISTOVAR && gPrijevozPrikolica[playerid] == INVALID_VEHICLE_ID && gPrijevozKamion[playerid] != INVALID_VEHICLE_ID) SkladisteStraznjaVrata(gPrijevozKamion[playerid], false);
}

static PrijevozIstovarGotov(playerid)
{
    gPrijevozRok[playerid] += gettime() - gPrijevozIstovarOd[playerid];
    PrijevozIstovari(playerid);
}

static PrijevozRadnikKreni(playerid, w)
{
    new lik = gPrijevozRadnik[playerid][w], Float:x, Float:y, Float:z;
    if (gPrijevozRadnikStanje[playerid][w] == 0)
    {
        PrijevozStrag(playerid, x, y, z);
        Lik_IdiDo(lik, x, y, z, LIK_HOD);
        return;
    }
    Lik_IdiDo(lik, gPrijevozDucan[playerid][0], gPrijevozDucan[playerid][1], gPrijevozDucan[playerid][2], LIK_HOD);
}

static PrijevozRadnikStigao(playerid, w)
{
    new lik = gPrijevozRadnik[playerid][w];
    switch (gPrijevozRadnikStanje[playerid][w])
    {
        case 0:
        {
            if (gPrijevozPreostalo[playerid] > 0)
            {
                PrijevozUzmiSanduk(playerid);
                Lik_PosebnaAkcija(lik, _:SPECIAL_ACTION_CARRY);
                Lik_PrikaciObjekt(lik, PZ_SANDUK_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.45, 0.45, 0.45);
                gPrijevozRadnikStanje[playerid][w] = 1;
            }
            else gPrijevozRadnikStanje[playerid][w] = 2;
            PrijevozRadnikKreni(playerid, w);
        }
        case 1:
        {
            Lik_SkiniObjekt(lik);
            Lik_PosebnaAkcija(lik, 0);
            if (++gPrijevozIsporuceno[playerid] >= PZ_TURA)
            {
                PrijevozIstovarGotov(playerid);
                return;
            }
            if (gPrijevozPreostalo[playerid] > 0)
            {
                gPrijevozRadnikStanje[playerid][w] = 0;
                PrijevozRadnikKreni(playerid, w);
                return;
            }
            Lik_Ukloni(lik);
            gPrijevozRadnik[playerid][w] = INVALID_LIK;
        }
        default:
        {
            Lik_Ukloni(lik);
            gPrijevozRadnik[playerid][w] = INVALID_LIK;
        }
    }
}

static PrijevozIstovarPocni(playerid)
{
    gPrijevozFaza[playerid] = PZF_ISTOVAR;
    gPrijevozIstovarOd[playerid] = gettime();
    gPrijevozPreostalo[playerid] = PZ_TURA;
    gPrijevozIsporuceno[playerid] = 0;
    gPrijevozNosi[playerid] = false;
    gPrijevozSavjet[playerid] = 0;
    gPrijevozPauza[playerid] = gettime() + PZ_PAUZA_S;
    DisablePlayerCheckpoint(playerid);
    switch (PrijevozIstovarVrsta(playerid))
    {
        case PZI_GORIVO: ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Istovar goriva je u tijeku, pricekajte."));
        case PZI_RUCNO:
        {
            SkladisteStraznjaVrata(gPrijevozKamion[playerid], true);
            SetPlayerCheckpoint(playerid, gPrijevozDucan[playerid][0], gPrijevozDucan[playerid][1], gPrijevozDucan[playerid][2], 1.5);
            ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Izadjite iz kamiona i odnesite sanduke u trgovinu (oznaceno na karti)."));
        }
        default:
        {
            for (new w = 0; w < PZ_RADNIKA; w++)
            {
                new lik = Lik_Stvori(gPrijevozRadnikSkin[random(sizeof gPrijevozRadnikSkin)], gPrijevozDucan[playerid][0] + float(w), gPrijevozDucan[playerid][1], gPrijevozDucan[playerid][2], 0.0);
                gPrijevozRadnik[playerid][w] = lik;
                if (lik == INVALID_LIK) continue;
                Lik_Neranjiv(lik, true);
                gPrijevozRadnikStanje[playerid][w] = 0;
                PrijevozRadnikKreni(playerid, w);
            }
            ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Radnici iz trgovine istovaruju teret, pricekajte."));
        }
    }
}

static bool:PrijevozRucnoAlt(playerid)
{
    if (gPrijevozFaza[playerid] != PZF_ISTOVAR || PrijevozIstovarVrsta(playerid) != PZI_RUCNO) return false;
    new Float:x, Float:y, Float:z;
    if (!gPrijevozNosi[playerid])
    {
        PrijevozStrag(playerid, x, y, z, 0.6);
        if (gPrijevozPreostalo[playerid] <= 0 || !IsPlayerInRangeOfPoint(playerid, PZ_STRAG_DOMET, x, y, z)) return false;
        PrijevozUzmiSanduk(playerid);
        gPrijevozNosi[playerid] = true;
        SetPlayerAttachedObject(playerid, PZ_NOSI_SLOT, PZ_SANDUK_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.45, 0.45, 0.45);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        return true;
    }
    if (!IsPlayerInRangeOfPoint(playerid, PZ_DUCAN_DOMET, gPrijevozDucan[playerid][0], gPrijevozDucan[playerid][1], gPrijevozDucan[playerid][2] + 1.0)) return false;
    PrijevozNosiMakni(playerid);
    if (++gPrijevozIsporuceno[playerid] >= PZ_TURA) PrijevozIstovarGotov(playerid);
    return true;
}

forward PrijevozScenaPredaj(playerid, racun);
public PrijevozScenaPredaj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozFaza[playerid] != PZF_SCENA) return;
    new bool:scena = gPrijevozScenaIgrac == playerid, k = gPrijevozKamion[playerid];
    PrijevozScenaKraj(playerid);
    if (scena)
    {
        SetVehiclePos(k, gPrijevozPoz[1][0], gPrijevozPoz[1][1], gPrijevozPoz[1][2] + PZ_VOZILO_Z);
        SetVehicleZAngle(k, gPrijevozPoz[1][3]);
    }
    VoziloSet(k, VP_MOTOR, 1);
    SetTimerEx("PrijevozScenaSjedni", 500, false, "iii", playerid, racun, 0);
}

forward PrijevozScenaSjedni(playerid, racun, pokusaj);
public PrijevozScenaSjedni(playerid, racun, pokusaj)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun || gPrijevozFaza[playerid] != PZF_SCENA) return;
    new k = gPrijevozKamion[playerid];
    if (GetPlayerVehicleID(playerid) != k || GetPlayerState(playerid) != PLAYER_STATE_DRIVER)
    {
        PutPlayerInVehicle(playerid, k, 0);
        if (pokusaj < 4)
        {
            SetTimerEx("PrijevozScenaSjedni", 400, false, "iii", playerid, racun, pokusaj + 1);
            return;
        }
    }
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    SetTimerEx("PrijevozOtkrij", 300, false, "ii", playerid, racun);
    PrijevozNakonKamiona(playerid);
}

forward PrijevozOtkrij(playerid, racun);
public PrijevozOtkrij(playerid, racun)
{
    if (IsPlayerConnected(playerid) && gAccountId[playerid] == racun) Fade(playerid, FADE_SLOJEVA + 1);
}

static bool:PrijevozStoji(v)
{
    new Float:vx, Float:vy, Float:vz;
    GetVehicleVelocity(v, vx, vy, vz);
    return VectorSize(vx, vy, vz) < PZ_STOJI;
}

static PrijevozLuka(playerid)
{
    gPrijevozFaza[playerid] = PZF_LUKA;
    SetPlayerCheckpoint(playerid, gPrijevozPoz[3][0], gPrijevozPoz[3][1], gPrijevozPoz[3][2], 5.0);
    ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Odvezite se na utovarno mjesto u luci (oznaceno na karti) i stanite."));
    gPrijevozPauza[playerid] = gettime() + PZ_PAUZA_S;
}

static PrijevozTovar(playerid)
{
    return gPrijevozPrikolica[playerid] != INVALID_VEHICLE_ID ? gPrijevozPrikolica[playerid] : gPrijevozKamion[playerid];
}

static PrijevozTeretMakni(playerid)
{
    for (new n = 0; n < PZ_TURA; n++)
    {
        if (_:gPrijevozTeretObj[playerid][n]) DestroyDynamicObject(gPrijevozTeretObj[playerid][n]);
        gPrijevozTeretObj[playerid][n] = STREAMER_TAG_OBJECT:0;
    }
    gPrijevozUtovareno[playerid] = 0;
}

static PrijevozUtovarOslobodi(playerid)
{
    if (gPrijevozUtovar != playerid) return;
    gPrijevozUtovar = INVALID_PLAYER_ID;
    if (gPrijevozViljuskar) SkladisteUtovarStani();
    if (gPrijevozPrikolica[playerid] == INVALID_VEHICLE_ID && gPrijevozKamion[playerid] != INVALID_VEHICLE_ID) SkladisteStraznjaVrata(gPrijevozKamion[playerid], false);
}

PrijevozTeretUtovaren()
{
    new i = gPrijevozUtovar;
    if (i == INVALID_PLAYER_ID || gPrijevozUtovareno[i] >= PZ_TURA) return;
    new v = PrijevozTovar(i), n = gPrijevozUtovareno[i]++, Float:sx, Float:sy, Float:sz;
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    gPrijevozTeretObj[i][n] = SkladisteTeretNaVozilo(v, 0.0, -sy / 2.0 + 1.0 + float(n) * 0.9, -0.2);
    PlayerPlaySound(i, 1056, 0.0, 0.0, 0.0);
}

PrijevozUtovaren()
{
    new i = gPrijevozUtovar;
    if (i == INVALID_PLAYER_ID) return;
    PrijevozUtovarOslobodi(i);
    PrijevozKreni(i);
}

static PrijevozLukaTick(playerid)
{
    new k = gPrijevozKamion[playerid], tr = gPrijevozPrikolica[playerid];
    if (gPrijevozUtovar != INVALID_PLAYER_ID || GetVehicleDistanceFromPoint(k, gPrijevozPoz[3][0], gPrijevozPoz[3][1], gPrijevozPoz[3][2]) > PZ_ISTOVAR || !PrijevozStoji(k) || (tr != INVALID_VEHICLE_ID && GetVehicleTrailer(k) != tr)) return;
    gPrijevozUtovar = playerid;
    gPrijevozUtovarOd = gettime();
    gPrijevozFaza[playerid] = PZF_UTOVAR;
    PrijevozTeretMakni(playerid);
    DisablePlayerCheckpoint(playerid);
    gPrijevozViljuskar = (tr == INVALID_VEHICLE_ID || GetVehicleModel(tr) != PZ_GORIVO_MODEL) && SkladisteUtovarPocni(PrijevozTovar(playerid), gPrijevozKamion[playerid]);
    if (tr == INVALID_VEHICLE_ID) SkladisteStraznjaVrata(k, true);
    gPrijevozPauza[playerid] = 0;
}

static PrijevozUtovarTick(playerid)
{
    new k = gPrijevozKamion[playerid], tr = gPrijevozPrikolica[playerid];
    if (GetVehicleDistanceFromPoint(k, gPrijevozPoz[3][0], gPrijevozPoz[3][1], gPrijevozPoz[3][2]) > PZ_ISTOVAR + 4.0 || (tr != INVALID_VEHICLE_ID && GetVehicleTrailer(k) != tr))
    {
        PrijevozUtovarOslobodi(playerid);
        PrijevozTeretMakni(playerid);
        PrijevozLuka(playerid);
        IgraObavijest(playerid, T(playerid, "Prijevoz tereta"), OBAVIJEST_GRESKA, T(playerid, "Udaljili ste se s utovarnog mjesta, utovar je prekinut."));
        return;
    }
    new proslo = gettime() - gPrijevozUtovarOd;
    if ((!gPrijevozViljuskar && proslo >= PZ_GORIVO_S) || proslo >= PZ_UTOVAR_MAX) PrijevozUtovaren();
}

static PrijevozKreni(playerid)
{
    new bool:veliki = gPrijevozPrikolica[playerid] != INVALID_VEHICLE_ID;
    gPrijevozFaza[playerid] = PZF_VOZI;
    gPrijevozRok[playerid] = gettime() + floatround(gPrijevozPut[playerid] / (veliki ? PZ_BRZINA_VELIKI : PZ_BRZINA_MALI)) + PZ_ROK_DODATAK;
    SetPlayerCheckpoint(playerid, gPrijevozCilj[playerid][0], gPrijevozCilj[playerid][1], gPrijevozCilj[playerid][2], 5.0);
    ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Teret je utovaren. Odvezite ga do odredista oznacenog na karti."));
    gPrijevozPauza[playerid] = gettime() + PZ_PAUZA_S;
}

static PrijevozPrikolicaMakni(playerid)
{
    if (gPrijevozPrikolica[playerid] == INVALID_VEHICLE_ID) return;
    if (gPrijevozKamion[playerid] != INVALID_VEHICLE_ID && GetVehicleTrailer(gPrijevozKamion[playerid]) == gPrijevozPrikolica[playerid]) DetachTrailerFromVehicle(gPrijevozKamion[playerid]);
    DestroyVehicle(gPrijevozPrikolica[playerid]);
    gPrijevozPrikolica[playerid] = INVALID_VEHICLE_ID;
}

PrijevozKraj(playerid)
{
    PrijevozMeniZatvori(playerid);
    gPrijevozPonudaBroj[playerid] = 0;
    gPrijevozOdustaje[playerid] = false;
    if (!gPrijevozFaza[playerid]) return;
    if (gPrijevozFaza[playerid] == PZF_SCENA || gPrijevozFaza[playerid] == PZF_VRACA) PrijevozScenaPrekini(playerid);
    PrijevozIstovarMakni(playerid);
    gPrijevozFaza[playerid] = PZF_NEMA;
    PrijevozUtovarOslobodi(playerid);
    PrijevozTeretMakni(playerid);
    PrijevozPrikolicaMakni(playerid);
    if (gPrijevozKamion[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(gPrijevozKamion[playerid]);
    gPrijevozKamion[playerid] = INVALID_VEHICLE_ID;
    if (!IsPlayerConnected(playerid)) return;
    DisablePlayerCheckpoint(playerid);
    IgraObavijestUkloni(playerid);
}

static PrijevozPovratak(playerid)
{
    PrijevozIstovarMakni(playerid);
    PrijevozUtovarOslobodi(playerid);
    PrijevozTeretMakni(playerid);
    PrijevozPrikolicaMakni(playerid);
    gPrijevozFaza[playerid] = PZF_POVRATAK;
    gPrijevozSavjet[playerid] = 0;
    gPrijevozPauza[playerid] = gettime() + PZ_PAUZA_S;
    SetPlayerCheckpoint(playerid, gPrijevozPoz[1][0], gPrijevozPoz[1][1], gPrijevozPoz[1][2], 5.0);
}

static PrijevozPropalo(playerid, const poruka[])
{
    PrijevozPovratak(playerid);
    IgraObavijest(playerid, T(playerid, "Prijevoz tereta"), OBAVIJEST_GRESKA, poruka);
}

static Float:PrijevozDoCilja(playerid)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gPrijevozKamion[playerid], x, y, z);
    return VectorSize(x - gPrijevozCilj[playerid][0], y - gPrijevozCilj[playerid][1], z - gPrijevozCilj[playerid][2]);
}

static bool:PrijevozUDepou(playerid)
{
    return GetVehicleDistanceFromPoint(gPrijevozKamion[playerid], gPrijevozPoz[1][0], gPrijevozPoz[1][1], gPrijevozPoz[1][2]) < PZ_ISTOVAR;
}

static bool:PrijevozPrevrnut(v)
{
    new Float:qw, Float:qx, Float:qy, Float:qz;
    GetVehicleRotationQuat(v, qw, qx, qy, qz);
    return 1.0 - 2.0 * (qx * qx + qy * qy) < 0.0;
}

static PrijevozSteta(playerid)
{
    new Float:hp, Float:steta;
    GetVehicleHealth(gPrijevozKamion[playerid], hp);
    steta = 1000.0 - hp;
    if (gPrijevozPrikolica[playerid] != INVALID_VEHICLE_ID)
    {
        GetVehicleHealth(gPrijevozPrikolica[playerid], hp);
        steta += 1000.0 - hp;
    }
    return clamp(floatround(steta / (1000.0 - PZ_KAMION_HP) * 100.0), 0, 100);
}

static PrijevozIstovari(playerid)
{
    new t = gPrijevozVrsta[playerid], kasni = gettime() - gPrijevozRok[playerid], steta = PrijevozSteta(playerid), poruka[200];
    new iznos = PrijevozIsplata(PrijevozOsnova(t, gPrijevozPut[playerid]), kasni, steta), xp = gPrijevozTeret[t][ptXp] + (kasni <= 0 ? 1 : 0), staro = PrijevozXp(playerid);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO trucker_xp (account, xp, runs) VALUES (%d, 0, 0)", gAccountId[playerid]));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE trucker_xp SET xp = xp + %d, runs = runs + 1 WHERE account = %d", xp, gAccountId[playerid]));
    PrijevozPovratak(playerid);
    iznos = BlagajnaIsplati(iznos);
    if (!BankaUplati(gAccountId[playerid], iznos, "Prijevoz tereta")) BlagajnaDodaj(iznos), iznos = 0;
    format(poruka, sizeof poruka, T(playerid, "Uplata za prijevoz tereta: $%d."), iznos);
    MobitelPrimiPoruku(gAccountId[playerid], MOBITEL_BANKA_BROJ, poruka);
    if (kasni <= 0) format(poruka, sizeof poruka, T(playerid, "Teret je dostavljen na vrijeme! Steta: %d%%.~n~$%d je uplaceno na vas racun. +%d XP~n~Vratite kamion u depo."), steta, iznos, xp);
    else format(poruka, sizeof poruka, T(playerid, "Teret je dostavljen s %d s zakasnjenja. Steta: %d%%.~n~$%d je uplaceno na vas racun. +%d XP~n~Vratite kamion u depo."), kasni, steta, iznos, xp);
    IgraObavijest(playerid, T(playerid, "Prijevoz tereta"), PZ_BOJA, poruka, PZ_PAUZA_S * 1000);
    new nivo = PrijevozNivo(staro + xp);
    if (nivo == PrijevozNivo(staro)) return;
    format(poruka, sizeof poruka, T(playerid, "Napredovali ste na nivo %d! Otkljucani su novi tereti."), nivo);
    ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), poruka);
}

static PrijevozVrati(playerid)
{
    if (gPrijevozScenaIgrac != INVALID_PLAYER_ID)
    {
        ObavijestGreska(playerid, T(playerid, "Hangar je trenutno zauzet, pricekajte malo."));
        return;
    }
    gPrijevozScenaIgrac = playerid;
    gPrijevozScenaOd = gettime();
    gPrijevozFaza[playerid] = PZF_VRACA;
    TogglePlayerControllable(playerid, false);
    Fade(playerid, 0);
    SetTimerEx("PrijevozVracaPocni", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
}

static PrijevozPanel(playerid)
{
    if (gettime() < gPrijevozPauza[playerid]) return;
    new t = gPrijevozVrsta[playerid], poruka[256];
    switch (gPrijevozFaza[playerid])
    {
        case PZF_KACI: format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Dovezite se unatrag do prikolice da je prikacite."), T(playerid, gPrijevozTeret[t][ptIme]));
        case PZF_LUKA:
        {
            if (gPrijevozUtovar != INVALID_PLAYER_ID) format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Utovarno mjesto je zauzeto, pricekajte da se oslobodi."), T(playerid, gPrijevozTeret[t][ptIme]));
            else format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Odvezite se na utovarno mjesto u luci i stanite."), T(playerid, gPrijevozTeret[t][ptIme]));
        }
        case PZF_UTOVAR:
        {
            if (gPrijevozViljuskar) format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Viljuskar utovaruje teret (%d/%d)."), T(playerid, gPrijevozTeret[t][ptIme]), gPrijevozUtovareno[playerid], PZ_TURA);
            else format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Utovar u tijeku: %d s."), T(playerid, gPrijevozTeret[t][ptIme]), max(0, PZ_GORIVO_S - (gettime() - gPrijevozUtovarOd)));
        }
        case PZF_POVRATAK: strcat(poruka, T(playerid, "Vratite kamion u depo (oznaceno na karti) i pritisnite ~y~ALT~w~."));
        case PZF_ISTOVAR:
        {
            switch (PrijevozIstovarVrsta(playerid))
            {
                case PZI_GORIVO: format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Istovar goriva: %d s."), T(playerid, gPrijevozTeret[t][ptIme]), max(0, PZ_ISTOVAR_GORIVO_S - (gettime() - gPrijevozIstovarOd[playerid])));
                case PZI_RUCNO: format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Odnesite sanduke u trgovinu: %d/%d."), T(playerid, gPrijevozTeret[t][ptIme]), gPrijevozIsporuceno[playerid], PZ_TURA);
                default: format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Radnici istovaruju teret: %d/%d."), T(playerid, gPrijevozTeret[t][ptIme]), gPrijevozIsporuceno[playerid], PZ_TURA);
            }
        }
        default:
        {
            if (gPrijevozOtkaceno[playerid]) format(poruka, sizeof poruka, T(playerid, "Prikolica se otkacila!~n~Prikacite je u roku od %d s."), PZ_OTKACENO_S - (gettime() - gPrijevozOtkaceno[playerid]));
            else
            {
                new ostalo = gPrijevozRok[playerid] - gettime(), Float:km = PrijevozDoCilja(playerid) / 1000.0;
                if (ostalo >= 0) format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Odrediste: %s (%.1f km)~n~Rok: %d:%02d~n~Steta: %d%%"), T(playerid, gPrijevozTeret[t][ptIme]), gPrijevozZona[playerid], km, ostalo / 60, ostalo % 60, PrijevozSteta(playerid));
                else format(poruka, sizeof poruka, T(playerid, "Teret: %s~n~Odrediste: %s (%.1f km)~n~Kasnite: %d:%02d~n~Steta: %d%%"), T(playerid, gPrijevozTeret[t][ptIme]), gPrijevozZona[playerid], km, -ostalo / 60, -ostalo % 60, PrijevozSteta(playerid));
            }
        }
    }
    IgraObavijestPanel(playerid, T(playerid, "Prijevoz tereta"), t == PZ_ILEGALNO && gPrijevozFaza[playerid] != PZF_POVRATAK ? PZ_ILEGALNO_BOJA : PZ_BOJA, poruka);
}

static PrijevozDojavi(playerid)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gPrijevozKamion[playerid], x, y, z);
    gPrijevozDojava[playerid] = true;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: kamion s krijumcarenom robom, %s."), CestaZona(x, y));
    IgraObavijest(playerid, T(playerid, "Prijevoz tereta"), OBAVIJEST_GRESKA, T(playerid, "Netko je dojavio policiji za vas kamion! Pazite se."));
    gPrijevozPauza[playerid] = gettime() + PZ_PAUZA_S;
}

bool:PrijevozKey(playerid, KEY:newkeys, KEY:oldkeys)
{
    if (!gPrijevozFaza[playerid] || !(newkeys & KEY_FIRE) || (oldkeys & KEY_FIRE) || GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetPlayerVehicleID(playerid) != gPrijevozKamion[playerid]) return false;
    if (gPrijevozFaza[playerid] == PZF_POVRATAK)
    {
        if (!PrijevozUDepou(playerid)) return false;
        PrijevozVrati(playerid);
        return true;
    }
    if (gPrijevozFaza[playerid] != PZF_VOZI || PrijevozDoCilja(playerid) > PZ_ISTOVAR) return false;
    if (gPrijevozPrikolica[playerid] != INVALID_VEHICLE_ID && GetVehicleTrailer(gPrijevozKamion[playerid]) != gPrijevozPrikolica[playerid]) return ObavijestGreska(playerid, T(playerid, "Prikolica nije prikacena.")), true;
    if (!PrijevozStoji(gPrijevozKamion[playerid])) return ObavijestGreska(playerid, T(playerid, "Zaustavite kamion.")), true;
    PrijevozIstovarPocni(playerid);
    return true;
}

PrijevozPosao(playerid)
{
    if (gPrijevozFaza[playerid])
    {
        if (gPrijevozOdustaje[playerid]) return 1;
        gPrijevozOdustaje[playerid] = true;
        Fade(playerid, 0);
        SetTimerEx("PrijevozPosaoKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
        return 1;
    }
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !PrijevozNaDepou(playerid)) return 0;
    return PrijevozPonude(playerid), 1;
}

forward PrijevozPosaoKraj(playerid, racun);
public PrijevozPosaoKraj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    gPrijevozOdustaje[playerid] = false;
    PrijevozKraj(playerid);
    ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Tura je prekinuta."));
    SetTimerEx("PrijevozOtkrij", 300, false, "ii", playerid, racun);
}

PrijevozAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (PrijevozRucnoAlt(playerid)) return 1;
    if (!PrijevozNaDepou(playerid)) return 0;
    if (gPrijevozFaza[playerid]) return UpitDaNe(playerid, UPIT_PRIJEVOZ, 0, 1, T(playerid, "Prijevoz tereta"), T(playerid, "Zelite li prekinuti turu?")), 1;
    return PrijevozPonude(playerid), 1;
}

PrijevozUpit(playerid, bool:da)
{
    if (!da || !gPrijevozFaza[playerid]) return 1;
    PrijevozKraj(playerid);
    return ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Tura je prekinuta."));
}

PrijevozStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER) return;
    new v = GetPlayerVehicleID(playerid);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gPrijevozFaza[i] || i == playerid) continue;
        if (v == gPrijevozKamion[i] || v == gPrijevozPrikolica[i])
        {
            RemovePlayerFromVehicle(playerid);
            return;
        }
    }
}

PrijevozTick()
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gPrijevozFaza[i]) continue;
        new k = gPrijevozKamion[i], tr = gPrijevozPrikolica[i], Float:hp;
        GetVehicleHealth(k, hp);
        if (!IsPlayerConnected(i) || IsRanjen(i) || !GetVehicleModel(k) || hp < PZ_KAMION_HP)
        {
            PrijevozKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Kamion je unisten, tura je propala."));
            continue;
        }
        if (gPrijevozFaza[i] == PZF_VRACA)
        {
            if (gettime() - gPrijevozScenaOd > PZ_SCENA_MAX) PrijevozVracaGotov(i, gAccountId[i]);
            continue;
        }
        if (gPrijevozFaza[i] == PZF_SCENA)
        {
            if (gettime() - gPrijevozScenaOd > PZ_SCENA_MAX && gPrijevozScenaIgrac == i) PrijevozScenaPredaj(i, gAccountId[i]);
            continue;
        }
        if (gPrijevozFaza[i] == PZF_ISTOVAR)
        {
            new proslo = gettime() - gPrijevozIstovarOd[i], vrsta = PrijevozIstovarVrsta(i);
            if ((vrsta == PZI_GORIVO && proslo >= PZ_ISTOVAR_GORIVO_S) || proslo >= PZ_ISTOVAR_MAX)
            {
                PrijevozIstovarGotov(i);
                continue;
            }
            new istSavjet = 0, Float:sx, Float:sy, Float:sz;
            if (vrsta == PZI_RUCNO && GetPlayerState(i) == PLAYER_STATE_ONFOOT)
            {
                PrijevozStrag(i, sx, sy, sz, 0.6);
                if (!gPrijevozNosi[i] && gPrijevozPreostalo[i] > 0 && IsPlayerInRangeOfPoint(i, PZ_STRAG_DOMET, sx, sy, sz)) istSavjet = 3;
                else if (gPrijevozNosi[i] && IsPlayerInRangeOfPoint(i, PZ_DUCAN_DOMET, gPrijevozDucan[i][0], gPrijevozDucan[i][1], gPrijevozDucan[i][2] + 1.0)) istSavjet = 4;
            }
            if (istSavjet != gPrijevozSavjet[i] && istSavjet) Savjet(i, istSavjet == 3 ? T(i, "~y~ALT~w~ - uzmi sanduk") : T(i, "~y~ALT~w~ - ostavi sanduk"), PZ_SAVJET_MS);
            gPrijevozSavjet[i] = istSavjet;
            PrijevozPanel(i);
            continue;
        }
        new savjet = 0, bool:vozi = GetPlayerVehicleID(i) == k && GetPlayerState(i) == PLAYER_STATE_DRIVER;
        if (gPrijevozFaza[i] == PZF_POVRATAK)
        {
            if (vozi && PrijevozUDepou(i)) savjet = 2;
        }
        else if (tr != INVALID_VEHICLE_ID && !GetVehicleModel(tr))
        {
            gPrijevozPrikolica[i] = INVALID_VEHICLE_ID;
            PrijevozPropalo(i, T(i, "Prikolica je unistena, tura je propala.~n~Vratite kamion u depo."));
            continue;
        }
        else if (PrijevozPrevrnut(k) || (tr != INVALID_VEHICLE_ID && PrijevozPrevrnut(tr)))
        {
            if (++gPrijevozPrevrnut[i] >= PZ_PREVRNUT_S)
            {
                PrijevozPropalo(i, T(i, "Kamion se prevrnuo i teret je unisten.~n~Vratite kamion u depo."));
                continue;
            }
        }
        else
        {
            gPrijevozPrevrnut[i] = 0;
            if (gPrijevozFaza[i] == PZF_KACI)
            {
                if (GetVehicleTrailer(k) == tr) PrijevozLuka(i);
            }
            else if (gPrijevozFaza[i] == PZF_LUKA) PrijevozLukaTick(i);
            else if (gPrijevozFaza[i] == PZF_UTOVAR) PrijevozUtovarTick(i);
            else
            {
                if (tr != INVALID_VEHICLE_ID)
                {
                    new bool:vuce = GetVehicleTrailer(k) == tr;
                    if (!vuce && !gPrijevozOtkaceno[i])
                    {
                        new Float:x, Float:y, Float:z;
                        GetVehiclePos(tr, x, y, z);
                        gPrijevozOtkaceno[i] = gettime();
                        SetPlayerCheckpoint(i, x, y, z, 3.0);
                    }
                    else if (vuce && gPrijevozOtkaceno[i])
                    {
                        gPrijevozRok[i] += gettime() - gPrijevozOtkaceno[i];
                        gPrijevozOtkaceno[i] = 0;
                        SetPlayerCheckpoint(i, gPrijevozCilj[i][0], gPrijevozCilj[i][1], gPrijevozCilj[i][2], 5.0);
                    }
                    else if (!vuce && gettime() - gPrijevozOtkaceno[i] >= PZ_OTKACENO_S)
                    {
                        PrijevozPropalo(i, T(i, "Niste na vrijeme prikacili prikolicu, tura je propala.~n~Vratite kamion u depo."));
                        continue;
                    }
                }
                if (gPrijevozVrsta[i] == PZ_ILEGALNO && !gPrijevozDojava[i] && PrijevozDoCilja(i) < gPrijevozPut[i] / 2.0) PrijevozDojavi(i);
                if (vozi && !gPrijevozOtkaceno[i] && PrijevozDoCilja(i) <= PZ_ISTOVAR) savjet = 1;
            }
        }
        if (savjet != gPrijevozSavjet[i] && savjet) Savjet(i, savjet == 1 ? T(i, "~y~ALT~w~ - istovari teret") : T(i, "~y~ALT~w~ - vrati kamion"), PZ_SAVJET_MS);
        gPrijevozSavjet[i] = savjet;
        PrijevozPanel(i);
    }
}

ShowPrijevozAdmin(playerid)
{
    new list[384], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Mjesto za prikolicu na mojoj poziciji"));
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Utovarno mjesto u luci na mojoj poziciji"));
    return PrikaziDialog(playerid, DLG_APANEL_PRIJEVOZ, DIALOG_STYLE_LIST, T(playerid, "Prijevoz tereta"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_prijevoz(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || listitem < 0 || listitem > 1) return 1;
    listitem += 2;
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowPrijevozAdmin(playerid);
    new Float:x, Float:y, Float:z, Float:a, v = GetPlayerVehicleID(playerid);
    if (v) GetVehiclePos(v, x, y, z), GetVehicleZAngle(v, a);
    else GetPlayerPos(playerid, x, y, z), GetPlayerFacingAngle(playerid, a);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_trucker (id, x, y, z, a) VALUES (%d, %f, %f, %f, %f)", listitem + 1, x, y, z, a));
    gPrijevozPoz[listitem][0] = x, gPrijevozPoz[listitem][1] = y, gPrijevozPoz[listitem][2] = z, gPrijevozPoz[listitem][3] = a;
    gPrijevozIma[listitem] = true;
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Pozicija za prijevoz tereta postavljena."));
    return ShowPrijevozAdmin(playerid);
}
