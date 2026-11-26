#define PV_JATAKA           4
#define PV_SKIN             29
#define PV_BOJA             0x8E44ADFF
#define PV_DOMET            2.0
#define PV_JATAK_BLIZU      40.0
#define PV_SVIJET           50000
#define PV_PLIJENA          8
#define PV_GEPEK            8
#define PV_KUCA_PAUZA       7200
#define PV_JATAK_UDALJENOST 200.0
#define PV_NOC_OD           19
#define PV_NOC_DO           7
#define PV_TIK_MS           250
#define PV_BUKA_MAX         100
#define PV_BUKA_CRVENO      85.0
#define PV_WANTED           2
#define PV_SLOT             2
#define PV_NOSI_MODEL       1271
#define PV_KUTIJA_DOMET     1.6
#define PV_STRAG_DOMET      1.8
#define PV_SACMARICA_METAKA 200

static const gProvalaUkucani[] = { 7, 14, 15, 20, 24, 44, 58, 60, 72, 94, 128, 132, 160, 182, 200, 206, 234, 235, 236 };
static const gProvalaKombi[] = { 413, 414, 440, 456, 459, 482, 498, 499, 609 };

enum E_PV_STVAR { psModel, psIme[24], psCijena }
static const gProvalaStvar[][E_PV_STVAR] = {
    { 1781, "Televizor", 150 },
    { 1792, "Plazma televizor", 260 },
    { 1785, "Video rekorder", 70 },
    { 2103, "Linija", 110 },
    { 2225, "Skupa linija", 180 },
    { 2149, "Mikrovalna", 60 },
    { 1840, "Zvucnik", 50 }
};

static const Float:gProvalaTocke[7][PV_PLIJENA][3] = {
    { { 2278.04, -1134.28, 1049.90 }, { 2285.54, -1137.78, 1049.90 }, { 2280.04, -1139.78, 1049.90 }, { 2282.54, -1134.28, 1049.90 }, { 2277.54, -1139.28, 1049.90 }, { 2282.04, -1136.78, 1049.90 }, { 2280.04, -1134.78, 1049.90 }, { 0.00, 0.00, 0.00 } },
    { { 217.70, 1293.08, 1081.14 }, { 226.70, 1287.58, 1081.14 }, { 220.20, 1287.58, 1081.14 }, { 224.20, 1293.08, 1081.14 }, { 223.70, 1289.58, 1081.14 }, { 226.70, 1291.08, 1081.14 }, { 220.70, 1292.58, 1081.14 }, { 221.20, 1290.08, 1081.14 } },
    { { 2361.80, -1120.10, 1049.88 }, { 2373.30, -1135.10, 1049.88 }, { 2358.30, -1135.10, 1049.88 }, { 2373.30, -1123.10, 1049.88 }, { 2365.80, -1129.10, 1049.88 }, { 2367.80, -1120.60, 1049.88 }, { 2371.80, -1129.10, 1049.88 }, { 2363.80, -1124.60, 1049.88 } },
    { { 2333.03, -1136.04, 1049.70 }, { 2311.53, -1146.04, 1049.70 }, { 2327.03, -1149.04, 1049.70 }, { 2320.53, -1137.54, 1049.49 }, { 2337.53, -1145.54, 1049.70 }, { 2319.53, -1145.54, 1049.70 }, { 2327.53, -1141.54, 1049.49 }, { 2333.03, -1149.04, 1049.70 } },
    { { 2276.38, -1212.85, 1046.56 }, { 2272.38, -1208.35, 1046.56 }, { 2271.88, -1212.85, 1046.56 }, { 2276.38, -1208.85, 1046.56 }, { 2273.88, -1210.85, 1046.56 }, { 2273.88, -1212.85, 1046.56 }, { 2274.38, -1208.85, 1046.56 }, { 2275.88, -1210.85, 1046.56 } },
    { { 225.68, 1037.45, 1083.01 }, { 241.18, 1017.95, 1083.02 }, { 227.68, 1023.95, 1083.01 }, { 237.68, 1031.95, 1083.01 }, { 236.18, 1023.95, 1083.01 }, { 230.18, 1031.45, 1083.02 }, { 234.18, 1017.95, 1083.01 }, { 224.68, 1029.45, 1083.01 } },
    { { 153.67, 1374.57, 1082.86 }, { 135.17, 1366.57, 1082.86 }, { 137.17, 1381.57, 1082.87 }, { 145.67, 1367.57, 1082.86 }, { 144.67, 1376.57, 1082.86 }, { 153.67, 1366.57, 1082.86 }, { 136.17, 1374.07, 1082.86 }, { 140.17, 1369.57, 1082.86 } }
};
static const gProvalaTockaBroj[7] = { 7, 8, 8, 8, 8, 8, 8 };

enum E_PV_JATAK { bool:pjIma, pjDb, Float:pjX, Float:pjY, Float:pjZ, Float:pjA, pjAktor, STREAMER_TAG_3D_TEXT_LABEL:pjTekst }
static gProvalaJatak[PV_JATAKA][E_PV_JATAK];
static gProvalaKucaZadnja[KUCA_MAX];
static gProvalaGepek[MAX_VEHICLES][PV_GEPEK], gProvalaGepekVlasnik[MAX_VEHICLES];

static gProvalaKuca[MAX_PLAYERS] = { -1, ... }, bool:gProvalaUnutra[MAX_PLAYERS], gProvalaTpl[MAX_PLAYERS], gProvalaNosi[MAX_PLAYERS] = { -1, ... };
static gProvalaUkucanin[MAX_PLAYERS] = { INVALID_LIK, ... }, bool:gProvalaBudan[MAX_PLAYERS], gProvalaBuka[MAX_PLAYERS], gProvalaTimer[MAX_PLAYERS], gProvalaApPopis[MAX_PLAYERS][PV_JATAKA];
static gProvalaPlijen[MAX_PLAYERS][PV_PLIJENA], STREAMER_TAG_OBJECT:gProvalaObjekt[MAX_PLAYERS][PV_PLIJENA], STREAMER_TAG_AREA:gProvalaZona[MAX_PLAYERS][PV_PLIJENA];
static STREAMER_TAG_AREA:gProvalaIzlaz[MAX_PLAYERS], STREAMER_TAG_AREA:gProvalaVrata[MAX_PLAYERS];

InitProvala()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS burglary_fences (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM burglary_fences ORDER BY id LIMIT %d", PV_JATAKA);
    for (new j = 0, n = DB_GetRowCount(res); j < n; j++, DB_SelectNextRow(res))
        ProvalaJatakStvori(j, DB_GetFieldInt(res, 0), DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4));
    DB_FreeResultSet(res);
}

static ProvalaJatakStvori(j, db, Float:x, Float:y, Float:z, Float:a)
{
    gProvalaJatak[j][pjIma] = true;
    gProvalaJatak[j][pjDb] = db;
    gProvalaJatak[j][pjX] = x;
    gProvalaJatak[j][pjY] = y;
    gProvalaJatak[j][pjZ] = z;
    gProvalaJatak[j][pjA] = a;
    gProvalaJatak[j][pjAktor] = CreateActor(PV_SKIN, x, y, z, a);
    SetActorInvulnerable(gProvalaJatak[j][pjAktor], true);
    gProvalaJatak[j][pjTekst] = CreateDynamic3DTextLabel("Jatak", PV_BOJA, x, y, z + 1.2, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gProvalaJatak[j][pjTekst], PV_DOMET);
}

static ProvalaJatakBlizu(playerid, Float:r = PV_DOMET)
{
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return -1;
    for (new j = 0; j < PV_JATAKA; j++)
        if (gProvalaJatak[j][pjIma] && IsPlayerInRangeOfPoint(playerid, r, gProvalaJatak[j][pjX], gProvalaJatak[j][pjY], gProvalaJatak[j][pjZ])) return j;
    return -1;
}

static bool:ProvalaNoc()
{
    new sat = VrijemeIgraSat();
    return sat >= PV_NOC_OD || sat < PV_NOC_DO;
}

static bool:ProvalaJeKombi(v)
{
    new model = GetVehicleModel(v);
    for (new i = 0; i < sizeof gProvalaKombi; i++) if (gProvalaKombi[i] == model) return true;
    return false;
}

static ProvalaKombiIgraca(playerid)
{
    for (new v = 1; v < MAX_VEHICLES; v++) if (VlasnikVozila(v) == gAccountId[playerid] && ProvalaJeKombi(v)) return v;
    return 0;
}

static ProvalaGepekBroj(v)
{
    if (gProvalaGepekVlasnik[v] != VlasnikVozila(v))
    {
        for (new i = 0; i < PV_GEPEK; i++) gProvalaGepek[v][i] = 0;
        gProvalaGepekVlasnik[v] = VlasnikVozila(v);
    }
    new n = 0;
    for (new i = 0; i < PV_GEPEK; i++) if (gProvalaGepek[v][i]) n++;
    return n;
}

static ProvalaStragKombija(playerid)
{
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return 0;
    new v = ProvalaKombiIgraca(playerid);
    if (!v) return 0;
    new Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    return IsPlayerInRangeOfPoint(playerid, PV_STRAG_DOMET + 0.5, x + floatsin(a, degrees) * (sy / 2.0 + 0.4), y - floatcos(a, degrees) * (sy / 2.0 + 0.4), z) ? v : 0;
}

static ProvalaNosiPrikazi(playerid, bool:nosi)
{
    if (nosi)
    {
        SetPlayerAttachedObject(playerid, PV_SLOT, PV_NOSI_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.6, 0.6, 0.6);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        return;
    }
    RemovePlayerAttachedObject(playerid, PV_SLOT);
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
}

static ProvalaUzmi(playerid, stvar)
{
    gProvalaNosi[playerid] = stvar;
    ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, false, false, false, 0);
    ProvalaNosiPrikazi(playerid, true);
}

static ProvalaPusti(playerid)
{
    gProvalaNosi[playerid] = -1;
    ApplyAnimation(playerid, "CARRY", "putdwn", 4.1, false, false, false, false, 0);
    ProvalaNosiPrikazi(playerid, false);
}

static ProvalaUnutraMakni(playerid)
{
    for (new p = 0; p < PV_PLIJENA; p++)
    {
        if (_:gProvalaObjekt[playerid][p]) DestroyDynamicObject(gProvalaObjekt[playerid][p]);
        if (IsValidDynamicArea(gProvalaZona[playerid][p])) DestroyDynamicArea(gProvalaZona[playerid][p]);
        gProvalaObjekt[playerid][p] = STREAMER_TAG_OBJECT:0;
        gProvalaZona[playerid][p] = STREAMER_TAG_AREA:0;
        gProvalaPlijen[playerid][p] = -1;
    }
    if (IsValidDynamicArea(gProvalaIzlaz[playerid])) DestroyDynamicArea(gProvalaIzlaz[playerid]);
    gProvalaIzlaz[playerid] = STREAMER_TAG_AREA:0;
    if (gProvalaTimer[playerid]) KillTimer(gProvalaTimer[playerid]);
    gProvalaTimer[playerid] = 0;
    if (gProvalaUnutra[playerid] && !gProvalaBudan[playerid] && IsPlayerConnected(playerid)) IgraTdMakni(playerid);
    if (gProvalaUkucanin[playerid] != INVALID_LIK) Lik_Ukloni(gProvalaUkucanin[playerid]);
    gProvalaUkucanin[playerid] = INVALID_LIK;
    gProvalaBudan[playerid] = false;
    gProvalaUnutra[playerid] = false;
}

ProvalaKraj(playerid)
{
    ProvalaUnutraMakni(playerid);
    if (gProvalaNosi[playerid] != -1 && IsPlayerConnected(playerid)) ProvalaNosiPrikazi(playerid, false);
    gProvalaNosi[playerid] = -1;
    if (IsValidDynamicArea(gProvalaVrata[playerid])) DestroyDynamicArea(gProvalaVrata[playerid]);
    gProvalaVrata[playerid] = STREAMER_TAG_AREA:0;
    if (gProvalaKuca[playerid] != -1 && IsPlayerConnected(playerid)) DisablePlayerCheckpoint(playerid);
    gProvalaKuca[playerid] = -1;
}

static ProvalaAdresa(playerid, j)
{
    new Float:x, Float:y, Float:z, tpl;
    for (new pokusaj = 0; pokusaj < 80; pokusaj++)
    {
        new k = random(KUCA_MAX);
        if (!KucaProvala(k, x, y, z, tpl) || gettime() - gProvalaKucaZadnja[k] < PV_KUCA_PAUZA) continue;
        if (VectorSize(x - gProvalaJatak[j][pjX], y - gProvalaJatak[j][pjY], 0.0) < PV_JATAK_UDALJENOST) continue;
        gProvalaKuca[playerid] = k;
        gProvalaTpl[playerid] = tpl;
        gProvalaKucaZadnja[k] = gettime();
        SetPlayerCheckpoint(playerid, x, y, z, 1.5);
        gProvalaVrata[playerid] = InterakcijaZona(x, y, z, PV_DOMET, 0, 0, SAVJET_ALT);
        for (new p = 0; p < PV_PLIJENA; p++) gProvalaPlijen[playerid][p] = -1;
        new n = min(gProvalaTockaBroj[tpl] - 1, 3 + 2 * KucaIntVelicina(tpl));
        for (new p = 0; p < n; p++) gProvalaPlijen[playerid][p] = random(sizeof gProvalaStvar);
        new poruka[144];
        format(poruka, sizeof poruka, T(playerid, "Adresa je %s (oznaceno na karti). Udite nocu, budite tihi i pokupite sto vise stvari u kombi."), CestaZona(x, y));
        ObavijestInfo(playerid, T(playerid, "Jatak"), poruka);
        return 1;
    }
    AktorKaze(playerid, gProvalaJatak[j][pjAktor], "Jatak", "Trenutno nemam nista za tebe. Dodji kasnije.");
    return 1;
}

static ProvalaUdji(playerid)
{
    if (!ProvalaNoc()) return ObavijestGreska(playerid, T(playerid, "Provaljuje se samo nocu (19-7h)."));
    if (gProvalaNosi[playerid] != -1) return ObavijestGreska(playerid, T(playerid, "Prvo odlozite ono sto nosite."));
    new tpl = gProvalaTpl[playerid], interijer, Float:ix, Float:iy, Float:iz;
    KucaIntPoz(tpl, interijer, ix, iy, iz);
    DisablePlayerCheckpoint(playerid);
    PrebaciIgraca(playerid, interijer, PV_SVIJET + playerid, ix, iy, iz);
    gProvalaUnutra[playerid] = true;
    gProvalaBuka[playerid] = 0;
    for (new p = 0; p < PV_PLIJENA; p++)
    {
        new s = gProvalaPlijen[playerid][p];
        if (s == -1) continue;
        gProvalaObjekt[playerid][p] = CreateDynamicObject(gProvalaStvar[s][psModel], gProvalaTocke[tpl][p][0], gProvalaTocke[tpl][p][1], gProvalaTocke[tpl][p][2], 0.0, 0.0, float(random(360)), PV_SVIJET + playerid, interijer);
        gProvalaZona[playerid][p] = InterakcijaZona(gProvalaTocke[tpl][p][0], gProvalaTocke[tpl][p][1], gProvalaTocke[tpl][p][2] + 0.5, PV_KUTIJA_DOMET, PV_SVIJET + playerid, interijer, SAVJET_PODIGNI);
    }
    gProvalaIzlaz[playerid] = InterakcijaZona(ix, iy, iz, PV_DOMET, PV_SVIJET + playerid, interijer, SAVJET_ALT);
    new t = gProvalaTockaBroj[tpl] - 1, lik = Lik_Stvori(gProvalaUkucani[random(sizeof gProvalaUkucani)], gProvalaTocke[tpl][t][0], gProvalaTocke[tpl][t][1], gProvalaTocke[tpl][t][2] + 1.0, float(random(360)), PV_SVIJET + playerid, interijer);
    if (lik != INVALID_LIK)
    {
        Lik_Neranjiv(lik, false);
        Lik_Animacija(lik, "INT_HOUSE", "BED_Loop_R", 4.1, 1, 0, 0, 1);
    }
    gProvalaUkucanin[playerid] = lik;
    gProvalaBudan[playerid] = false;
    IgraPanel(playerid, T(playerid, "Buka"), 60.0, PV_BOJA);
    IgraTraka(playerid, 0.0, 60.0, 0x27AE60FF, 0x27AE60FF, 0xF39C12FF, T(playerid, "Cucnite (~k~~PED_DUCK~) i hodajte polako da ne probudite ukucane."), PV_BUKA_CRVENO);
    IgraOznakaPomakni(playerid, 0.0);
    gProvalaTimer[playerid] = SetTimerEx("ProvalaBuka", PV_TIK_MS, true, "i", playerid);
    return 1;
}

static ProvalaIzadji(playerid)
{
    new k = gProvalaKuca[playerid], Float:x, Float:y, Float:z, tpl, preostalo = 0;
    ProvalaUnutraMakni(playerid);
    for (new p = 0; p < PV_PLIJENA; p++) if (gProvalaPlijen[playerid][p] != -1) preostalo++;
    for (new p = 0; p < PV_PLIJENA; p++) gProvalaPlijen[playerid][p] = -1;
    if (KucaProvala(k, x, y, z, tpl)) PrebaciIgraca(playerid, 0, 0, x, y, z);
    if (IsValidDynamicArea(gProvalaVrata[playerid])) DestroyDynamicArea(gProvalaVrata[playerid]);
    gProvalaVrata[playerid] = STREAMER_TAG_AREA:0;
    gProvalaKuca[playerid] = -1;
    if (gProvalaNosi[playerid] != -1) ObavijestInfo(playerid, T(playerid, "Jatak"), T(playerid, "Odnesite stvar do svog kombija (ALT kod straznjih vrata), pa robu prodajte jataku."));
    return 1;
}

forward ProvalaBuka(playerid);
public ProvalaBuka(playerid)
{
    if (!gProvalaUnutra[playerid] || !IsPlayerConnected(playerid)) return;
    new Float:vx, Float:vy, Float:vz;
    GetPlayerVelocity(playerid, vx, vy, vz);
    new Float:brzina = VectorSize(vx, vy, 0.0), korak;
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK) korak = brzina > 0.02 ? 0 : -2;
    else if (brzina > 0.15) korak = 7;
    else if (brzina > 0.09) korak = gProvalaNosi[playerid] != -1 ? 4 : 3;
    else if (brzina > 0.02) korak = 1;
    else korak = -1;
    gProvalaBuka[playerid] = clamp(gProvalaBuka[playerid] + korak, 0, PV_BUKA_MAX);
    IgraOznakaPomakni(playerid, float(gProvalaBuka[playerid]));
    if (gProvalaBuka[playerid] < PV_BUKA_MAX) return;
    KillTimer(gProvalaTimer[playerid]);
    gProvalaTimer[playerid] = 0;
    gProvalaBudan[playerid] = true;
    IgraTdMakni(playerid);
    new k = gProvalaKuca[playerid], Float:x, Float:y, Float:z, tpl;
    KucaProvala(k, x, y, z, tpl);
    for (new w = 0; w < PV_WANTED; w++) DodajWanted(playerid);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
        if (gUlogiran[i] && PolicajacNaDuznosti(i)) SendClientMessage(i, COLOR_ERROR, T(i, "Dojava: provala u kucu, %s."), CestaZona(x, y));
    IgraObavijest(playerid, T(playerid, "Provala"), OBAVIJEST_GRESKA, T(playerid, "Ukucanin se probudio i zgrabio sacmaricu! Policija je pozvana, bjezite!"));
    new lik = gProvalaUkucanin[playerid];
    if (lik == INVALID_LIK) return;
    Lik_OcistiAnimaciju(lik);
    Lik_Oruzje(lik, _:WEAPON_SHOTGUN, PV_SACMARICA_METAKA);
    Lik_Napadni(lik, playerid);
}

static ProvalaPlijenBlizu(playerid)
{
    new tpl = gProvalaTpl[playerid];
    for (new p = 0; p < PV_PLIJENA; p++)
        if (gProvalaPlijen[playerid][p] != -1 && IsPlayerInRangeOfPoint(playerid, PV_KUTIJA_DOMET, gProvalaTocke[tpl][p][0], gProvalaTocke[tpl][p][1], gProvalaTocke[tpl][p][2] + 0.5)) return p;
    return -1;
}

static ProvalaJatakPrica(playerid, j)
{
    if (gProvalaKuca[playerid] != -1)
    {
        new Float:x, Float:y, Float:z, tpl, poruka[144];
        KucaProvala(gProvalaKuca[playerid], x, y, z, tpl);
        format(poruka, sizeof poruka, T(playerid, "Vec imate adresu: %s."), CestaZona(x, y));
        return ObavijestInfo(playerid, T(playerid, "Jatak"), poruka), 1;
    }
    if (!ProvalaKombiIgraca(playerid))
    {
        AktorKaze(playerid, gProvalaJatak[j][pjAktor], "Jatak", "Bez kombija nemamo o cemu pricati. Nabavi kombi pa dodji.");
        return 1;
    }
    if (!ProvalaNoc())
    {
        AktorKaze(playerid, gProvalaJatak[j][pjAktor], "Jatak", "Dodji kad padne mrak, tad imam posla za tebe.");
        return 1;
    }
    AktorKaze(playerid, gProvalaJatak[j][pjAktor], "Jatak", "Imam jednu kucu za tebe. Zanima te?");
    return UpitDaNe(playerid, UPIT_PROVALA, j, 0, T(playerid, "Jatak"), T(playerid, "Zelite li adresu kuce za provalu?"));
}

ProvalaUpit(playerid, j, bool:da)
{
    if (!da || ProvalaJatakBlizu(playerid, PV_DOMET + 2.0) != j || gProvalaKuca[playerid] != -1) return 1;
    if (!ProvalaKombiIgraca(playerid)) return 1;
    return ProvalaAdresa(playerid, j);
}

static ProvalaProdaj(playerid, j)
{
    new s = gProvalaNosi[playerid], poruka[144];
    ProvalaPusti(playerid);
    DajNovac(playerid, gProvalaStvar[s][psCijena]);
    ApplyActorAnimation(gProvalaJatak[j][pjAktor], "DEALER", "DEALER_DEAL", 4.1, false, false, false, false, 0);
    format(poruka, sizeof poruka, T(playerid, "Jatak je platio $%d za: %s."), gProvalaStvar[s][psCijena], T(playerid, gProvalaStvar[s][psIme]));
    return IgraObavijest(playerid, T(playerid, "Jatak"), PV_BOJA, poruka), 1;
}

ProvalaAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (gProvalaUnutra[playerid])
    {
        new tpl = gProvalaTpl[playerid], interijer, Float:ix, Float:iy, Float:iz;
        KucaIntPoz(tpl, interijer, ix, iy, iz);
        if (IsPlayerInRangeOfPoint(playerid, PV_DOMET, ix, iy, iz)) return ProvalaIzadji(playerid);
        if (gProvalaNosi[playerid] != -1) return 0;
        new p = ProvalaPlijenBlizu(playerid);
        if (p == -1) return 0;
        new s = gProvalaPlijen[playerid][p];
        gProvalaPlijen[playerid][p] = -1;
        DestroyDynamicObject(gProvalaObjekt[playerid][p]);
        gProvalaObjekt[playerid][p] = STREAMER_TAG_OBJECT:0;
        if (IsValidDynamicArea(gProvalaZona[playerid][p])) DestroyDynamicArea(gProvalaZona[playerid][p]);
        gProvalaZona[playerid][p] = STREAMER_TAG_AREA:0;
        gProvalaBuka[playerid] = min(gProvalaBuka[playerid] + 10, PV_BUKA_MAX - 1);
        ProvalaUzmi(playerid, s);
        return 1;
    }
    new j = ProvalaJatakBlizu(playerid);
    if (j != -1 && gProvalaNosi[playerid] != -1) return ProvalaProdaj(playerid, j);
    new v = ProvalaStragKombija(playerid);
    if (v && gProvalaNosi[playerid] != -1)
    {
        if (ProvalaGepekBroj(v) >= PV_GEPEK) return ObavijestGreska(playerid, T(playerid, "Kombi je pun.")), 1;
        for (new i = 0; i < PV_GEPEK; i++)
        {
            if (gProvalaGepek[v][i]) continue;
            gProvalaGepek[v][i] = gProvalaNosi[playerid] + 1;
            break;
        }
        ProvalaPusti(playerid);
        return 1;
    }
    if (v && gProvalaNosi[playerid] == -1 && ProvalaGepekBroj(v) && ProvalaJatakBlizu(playerid, PV_JATAK_BLIZU) != -1)
    {
        for (new i = PV_GEPEK - 1; i >= 0; i--)
        {
            if (!gProvalaGepek[v][i]) continue;
            ProvalaUzmi(playerid, gProvalaGepek[v][i] - 1);
            gProvalaGepek[v][i] = 0;
            break;
        }
        return 1;
    }
    if (gProvalaKuca[playerid] != -1 && gProvalaNosi[playerid] == -1)
    {
        new Float:x, Float:y, Float:z, tpl;
        if (KucaProvala(gProvalaKuca[playerid], x, y, z, tpl) && !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, PV_DOMET, x, y, z)) return ProvalaUdji(playerid), 1;
    }
    if (j != -1) return ProvalaJatakPrica(playerid, j);
    return 0;
}

ShowProvalaAdmin(playerid)
{
    new list[512], row[96], len = 0, n = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Jatak na mojoj poziciji"));
    for (new j = 0; j < PV_JATAKA; j++)
    {
        if (!gProvalaJatak[j][pjIma]) continue;
        format(row, sizeof row, T(playerid, "Jatak [%d] %s - obrisi"), gProvalaJatak[j][pjDb], CestaZona(gProvalaJatak[j][pjX], gProvalaJatak[j][pjY]));
        DodajStupac(list, len, row);
        gProvalaApPopis[playerid][n++] = j;
    }
    return PrikaziDialog(playerid, DLG_APANEL_PROVALA, DIALOG_STYLE_LIST, T(playerid, "Provale"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_provala(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    if (listitem == 0)
    {
        if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid)) return ObavijestGreska(playerid, T(playerid, "Ovo se postavlja samo izvana.")), ShowProvalaAdmin(playerid);
        new j = 0;
        while (j < PV_JATAKA && gProvalaJatak[j][pjIma]) j++;
        if (j == PV_JATAKA) return ObavijestGreska(playerid, T(playerid, "Dosegnut je maksimalan broj.")), ShowProvalaAdmin(playerid);
        new Float:x, Float:y, Float:z, Float:a;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, a);
        DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO burglary_fences (x, y, z, a) VALUES (%f, %f, %f, %f)", x, y, z, a));
        ProvalaJatakStvori(j, ZadnjiId(), x, y, z, a);
        SetPlayerPos(playerid, x + 1.0, y, z);
        SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Jatak postavljen."));
        return ShowProvalaAdmin(playerid);
    }
    new i = listitem - 1;
    if (i < 0 || i >= sizeof gProvalaApPopis[]) return 1;
    new j = gProvalaApPopis[playerid][i];
    if (!gProvalaJatak[j][pjIma]) return ShowProvalaAdmin(playerid);
    DestroyActor(gProvalaJatak[j][pjAktor]);
    DestroyDynamic3DTextLabel(gProvalaJatak[j][pjTekst]);
    gProvalaJatak[j][pjIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM burglary_fences WHERE id = %d", gProvalaJatak[j][pjDb]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Jatak obrisan."));
    return ShowProvalaAdmin(playerid);
}
