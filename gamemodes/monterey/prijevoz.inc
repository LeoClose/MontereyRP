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
#define PZ_SCENA_BRZINA     5.0
#define PZ_VOZILO_Z         1.6

enum { PZF_NEMA, PZF_KACI, PZF_LUKA, PZF_UTOVAR, PZF_VOZI, PZF_POVRATAK, PZF_SCENA }
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
static const Float:gPrijevozZadano[3][4] = { { 2123.0557, -2274.9922, 20.6719, 46.2314 }, { 2161.3994, -2282.1294, 12.40, 223.9835 }, { 2214.293, -2236.873, 13.2, 315.0 } };
static const Float:gPrijevozHangar[4] = { 2164.533, -2240.699, 12.30, 225.0 };
static const Float:gPrijevozRezerva[4] = { 2223.130, -2228.035, 12.55, 315.0 };
static const Float:gPrijevozKamera[6] = { 2204.060, -2263.397, 17.5, 2177.968, -2254.134, 15.5 };
static const Float:gPrijevozRuta[] = {
    2164.533, -2240.699, 12.300, 2169.191, -2245.357, 12.300, 2173.584, -2249.750, 13.035, 2177.968, -2254.134, 13.770,
    2180.796, -2256.963, 13.770, 2185.181, -2261.347, 13.145, 2189.591, -2265.757, 12.520, 2187.443, -2270.822, 12.520,
    2182.847, -2276.125, 12.530, 2176.130, -2280.721, 12.510, 2168.351, -2282.843, 12.460, 2164.109, -2282.843, 12.430,
    2161.401, -2282.129, 12.400
};
static gPrijevozScenaIgrac = INVALID_PLAYER_ID, gPrijevozScenaLik = INVALID_LIK, gPrijevozScenaTimer, gPrijevozScenaOd;

static Float:gPrijevozPoz[4][4], bool:gPrijevozIma[4], STREAMER_TAG_3D_TEXT_LABEL:gPrijevozTekst;
static gPrijevozFaza[MAX_PLAYERS], gPrijevozVrsta[MAX_PLAYERS], gPrijevozKamion[MAX_PLAYERS], gPrijevozPrikolica[MAX_PLAYERS], gPrijevozRok[MAX_PLAYERS], gPrijevozOtkaceno[MAX_PLAYERS];
static gPrijevozPrevrnut[MAX_PLAYERS], gPrijevozSavjet[MAX_PLAYERS], gPrijevozPauza[MAX_PLAYERS], bool:gPrijevozDojava[MAX_PLAYERS], Float:gPrijevozCilj[MAX_PLAYERS][3], Float:gPrijevozPut[MAX_PLAYERS], gPrijevozZona[MAX_PLAYERS][28];
static gPrijevozUtovar = INVALID_PLAYER_ID, gPrijevozUtovarOd, bool:gPrijevozViljuskar, gPrijevozUtovareno[MAX_PLAYERS], STREAMER_TAG_OBJECT:gPrijevozTeretObj[MAX_PLAYERS][PZ_TURA];
static gPrijevozPonuda[MAX_PLAYERS][PZ_PONUDA], Float:gPrijevozPonudaPoz[MAX_PLAYERS][PZ_PONUDA][3], gPrijevozPonudaBroj[MAX_PLAYERS];

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
    for (new i = 0; i < MAX_PLAYERS; i++) gPrijevozKamion[i] = gPrijevozPrikolica[i] = INVALID_VEHICLE_ID;
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

static bool:PrijevozNasumicno(m, Float:domet, &Float:x, &Float:y, &Float:z)
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
        new t = ilegalno ? PZ_ILEGALNO : dostupno[random(nd)], m = gPrijevozTeret[t][ptMjesto], Float:x, Float:y, Float:z;
        if (m == -1) m = random(PZM_BROJ);
        if (!PrijevozNasumicno(m, gPrijevozTeret[t][ptDomet], x, y, z)) continue;
        ilegalno = false;
        gPrijevozPonuda[playerid][p] = t;
        gPrijevozPonudaPoz[playerid][p][0] = x;
        gPrijevozPonudaPoz[playerid][p][1] = y;
        gPrijevozPonudaPoz[playerid][p][2] = z;
        p++;
    }
    gPrijevozPonudaBroj[playerid] = p;
    if (!p) return ObavijestGreska(playerid, T(playerid, "Trenutno nema dostupnih tura.")), 1;
    new list[1024], red[160], naslov[64], len = 0;
    DodajStupac(list, len, T(playerid, "Teret\tOdrediste\tUdaljenost\tZarada"));
    for (new i = 0; i < p; i++)
    {
        new t = gPrijevozPonuda[playerid][i], Float:d = VectorSize(gPrijevozPonudaPoz[playerid][i][0] - gPrijevozPoz[3][0], gPrijevozPonudaPoz[playerid][i][1] - gPrijevozPoz[3][1], 0.0);
        format(red, sizeof red, "%s%s\t%s\t%.1f km\t$%d", t == PZ_ILEGALNO ? "{E74C3C}" : "", T(playerid, gPrijevozTeret[t][ptIme]), CestaZona(gPrijevozPonudaPoz[playerid][i][0], gPrijevozPonudaPoz[playerid][i][1]), d / 1000.0, PrijevozOsnova(t, d));
        DodajStupac(list, len, red);
    }
    format(naslov, sizeof naslov, T(playerid, "Prijevoz tereta - nivo %d (%d XP)"), nivo, xp);
    return PrikaziDialog(playerid, DLG_PRIJEVOZ, DIALOG_STYLE_TABLIST_HEADERS, naslov, list, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
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
    new bool:scena = gPrijevozScenaIgrac == INVALID_PLAYER_ID;
    k = scena ? CreateVehicle(gPrijevozTeret[t][ptKamion], gPrijevozHangar[0], gPrijevozHangar[1], gPrijevozHangar[2] + PZ_VOZILO_Z, gPrijevozHangar[3], 1, 1, -1)
        : CreateVehicle(gPrijevozTeret[t][ptKamion], gPrijevozRezerva[0], gPrijevozRezerva[1], gPrijevozRezerva[2] + PZ_VOZILO_Z, gPrijevozRezerva[3], 1, 1, -1);
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
    gPrijevozPut[playerid] = VectorSize(gPrijevozCilj[playerid][0] - gPrijevozPoz[3][0], gPrijevozCilj[playerid][1] - gPrijevozPoz[3][1], 0.0);
    gPrijevozZona[playerid][0] = EOS;
    strcat(gPrijevozZona[playerid], CestaZona(gPrijevozCilj[playerid][0], gPrijevozCilj[playerid][1]));
    gPrijevozDojava[playerid] = false;
    gPrijevozOtkaceno[playerid] = gPrijevozPrevrnut[playerid] = gPrijevozSavjet[playerid] = gPrijevozPauza[playerid] = 0;
    gPrijevozFaza[playerid] = PZF_SCENA;
    gPrijevozScenaOd = gettime();
    if (scena) gPrijevozScenaIgrac = playerid;
    TogglePlayerControllable(playerid, false);
    Fade(playerid, 0);
    if (scena) SetTimerEx("PrijevozScenaPocni", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    else SetTimerEx("PrijevozScenaPredaj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
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
    Fade(playerid, FADE_SLOJEVA + 1);
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
    gPrijevozScenaTimer = SetTimerEx("PrijevozScenaKamera", 250, true, "i", playerid);
}

forward PrijevozScenaKamera(playerid);
public PrijevozScenaKamera(playerid)
{
    new Float:x, Float:y, Float:z;
    if (gPrijevozScenaIgrac != playerid || gPrijevozScenaLik == INVALID_LIK || !Lik_Pozicija(gPrijevozScenaLik, x, y, z)) return;
    SetPlayerCameraLookAt(playerid, x, y, z, CAMERA_MOVE);
}

static PrijevozScenaStigao()
{
    new playerid = gPrijevozScenaIgrac;
    if (playerid == INVALID_PLAYER_ID) return;
    if (gPrijevozScenaTimer) KillTimer(gPrijevozScenaTimer);
    gPrijevozScenaTimer = 0;
    MapaKamionDepoVrata(false, true);
    Fade(playerid, 0);
    SetTimerEx("PrijevozScenaPredaj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
}

PrijevozStigao(lik)
{
    if (lik == INVALID_LIK || lik != gPrijevozScenaLik) return 0;
    PrijevozScenaStigao();
    return 1;
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
    PutPlayerInVehicle(playerid, k, 0);
    TogglePlayerControllable(playerid, true);
    SetCameraBehindPlayer(playerid);
    Fade(playerid, FADE_SLOJEVA + 1);
    PrijevozNakonKamiona(playerid);
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
    gPrijevozViljuskar = (tr == INVALID_VEHICLE_ID || GetVehicleModel(tr) != PZ_GORIVO_MODEL) && SkladisteUtovarPocni(PrijevozTovar(playerid));
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
    gPrijevozPonudaBroj[playerid] = 0;
    if (!gPrijevozFaza[playerid]) return;
    if (gPrijevozFaza[playerid] == PZF_SCENA) PrijevozScenaPrekini(playerid);
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
    PrijevozKraj(playerid);
    PrijevozPonude(playerid);
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
    PrijevozIstovari(playerid);
    return true;
}

PrijevozPosao(playerid)
{
    if (gPrijevozFaza[playerid])
    {
        PrijevozKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Tura je prekinuta.")), 1;
    }
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !PrijevozNaDepou(playerid)) return 0;
    return PrijevozPonude(playerid), 1;
}

PrijevozAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid) || !PrijevozNaDepou(playerid)) return 0;
    if (gPrijevozFaza[playerid]) return UpitDaNe(playerid, UPIT_PRIJEVOZ, 0, 1, T(playerid, "Prijevoz tereta"), T(playerid, "Zelite li prekinuti turu?")), 1;
    return PrijevozPonude(playerid), 1;
}

PrijevozUpit(playerid, bool:da)
{
    if (!da || !gPrijevozFaza[playerid]) return 1;
    PrijevozKraj(playerid);
    return ObavijestInfo(playerid, T(playerid, "Prijevoz tereta"), T(playerid, "Tura je prekinuta."));
}

DLG:prijevoz(playerid, response, listitem)
{
    if (!response || listitem < 0 || listitem >= gPrijevozPonudaBroj[playerid] || gPrijevozFaza[playerid]) return gPrijevozPonudaBroj[playerid] = 0, 1;
    if (IsPlayerInAnyVehicle(playerid)) RemovePlayerFromVehicle(playerid);
    return PrijevozPocni(playerid, listitem);
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
        if (gPrijevozFaza[i] == PZF_SCENA)
        {
            if (gettime() - gPrijevozScenaOd > PZ_SCENA_MAX && gPrijevozScenaIgrac == i) PrijevozScenaPredaj(i, gAccountId[i]);
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
