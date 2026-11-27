#define PV_JATAKA           4
#define PV_SKIN             29
#define PV_BOJA             0x8E44ADFF
#define PV_DOMET            2.0
#define PV_JATAK_BLIZU      40.0
#define PV_SVIJET           50000
#define PV_PLIJENA          7
#define PV_GEPEK            8
#define PV_KUCA_PAUZA       7200
#define PV_JATAK_UDALJENOST 200.0
#define PV_NOC_OD           19
#define PV_NOC_DO           7
#define PV_TIK_MS           100
#define PV_BUKA_MAX         100
#define PV_BUKA_CRVENO      85.0
#define PV_WANTED           2
#define PV_SLOT             2
#define PV_KUTIJA_DOMET     1.6
#define PV_STRAG_DOMET      1.8
#define PV_SACMARICA_METAKA 200
#define PV_PRATI_RAZMAK     2.0
#define PV_PUCAJ_MS         900

static const gProvalaUkucani[] = { 7, 14, 15, 20, 24, 44, 58, 60, 72, 94, 128, 132, 160, 182, 200, 206, 234, 235, 236 };
static const gProvalaKombi[] = { 413, 414, 440, 456, 459, 482, 498, 499, 609 };

enum E_PV_STVAR { psModel, psIme[24], psCijena }
static const gProvalaStvar[][E_PV_STVAR] = {
    { 2318, "Televizor", 150 },
    { 2312, "Televizor", 150 },
    { 2317, "Televizor", 160 },
    { 2316, "Televizor", 170 },
    { 2320, "Televizor", 140 },
    { 2322, "Televizor", 180 },
    { 2149, "Mikrovalna", 60 },
    { 2103, "Linija", 90 },
    { 2102, "Linija", 100 },
    { 2226, "Linija", 110 },
    { 1809, "Linija", 130 },
    { 2101, "Skupa linija", 180 },
    { 1783, "Video rekorder", 80 },
    { 1719, "Igraca konzola", 120 },
    { 2028, "Skupa konzola", 200 }
};

static const Float:gProvalaMjesto[7][PV_PLIJENA][5] = {
    { { 2284.79, -1139.53, 1050.610, 225.0, 0.0 }, { 2281.29, -1136.28, 1050.840, 90.0, 10.0 }, { 2276.79, -1140.03, 1051.110, 90.0, 11.0 }, { 2286.04, -1136.78, 1050.650, 270.0, 8.0 }, { 2280.54, -1138.78, 1051.110, 270.0, 9.0 }, { 2286.04, -1134.03, 1050.650, 270.0, 9.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 } },
    { { 216.95, 1288.83, 1082.190, 90.0, 9.0 }, { 224.70, 1287.08, 1082.390, 180.0, 11.0 }, { 217.45, 1287.08, 1082.450, 180.0, 9.0 }, { 220.95, 1293.58, 1084.230, 180.0, 14.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 } },
    { { 2371.30, -1132.85, 1050.700, 180.0, 8.0 }, { 2368.80, -1122.10, 1051.020, 270.0, 6.0 }, { 2374.55, -1131.35, 1051.050, 270.0, 9.0 }, { 2372.55, -1123.60, 1050.430, 180.0, 12.0 }, { 2360.05, -1130.35, 1051.050, 315.0, 7.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 } },
    { { 2337.53, -1137.04, 1050.910, 270.0, 6.0 }, { 2314.03, -1144.79, 1050.540, 90.0, 13.0 }, { 2334.53, -1138.54, 1050.520, 0.0, 7.0 }, { 2312.53, -1142.29, 1054.540, 180.0, 7.0 }, { 2337.78, -1142.54, 1056.310, 180.0, 10.0 }, { 2311.03, -1135.29, 1056.310, 180.0, 11.0 }, { 2326.28, -1142.54, 1050.370, 180.0, 1.0 } },
    { { 2250.13, -1212.10, 1048.850, 180.0, 7.0 }, { 2255.13, -1213.10, 1048.850, 315.0, 12.0 }, { 2261.63, -1224.60, 1049.180, 180.0, 10.0 }, { 2257.38, -1207.35, 1049.130, 180.0, 14.0 }, { 2262.38, -1209.10, 1048.980, 270.0, 11.0 }, { 2254.63, -1207.35, 1048.920, 0.0, 2.0 }, { 2254.38, -1218.35, 1048.980, 180.0, 9.0 } },
    { { 238.68, 1035.45, 1087.320, 180.0, 11.0 }, { 234.68, 1022.20, 1087.320, 180.0, 9.0 }, { 238.93, 1016.95, 1085.440, 0.0, 7.0 }, { 237.43, 1039.20, 1088.330, 90.0, 10.0 }, { 236.18, 1034.95, 1084.150, 270.0, 11.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 } },
    { { 150.67, 1383.82, 1083.670, 270.0, 8.0 }, { 142.42, 1377.82, 1088.910, 180.0, 6.0 }, { 154.42, 1378.07, 1091.380, 180.0, 9.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 }, { 0.0, 0.0, 0.0, 0.0, 0.0 } }
};
static const gProvalaMjestaBroj[7] = { 6, 4, 5, 7, 7, 5, 3 };
static const Float:gProvalaKrevet[7][4] = {
    { 2285.04, -1135.28, 1050.530, 90.0 },
    { 217.20, 1289.83, 1081.830, 90.0 },
    { 2362.30, -1122.60, 1050.570, 90.0 },
    { 2309.03, -1141.79, 1054.150, 0.0 },
    { 2258.13, -1223.85, 1048.790, 0.0 },
    { 0.0, 0.0, 0.0, 0.0 },
    { 144.42, 1379.82, 1087.370, 0.0 }
};
static const bool:gProvalaImaKrevet[7] = { true, true, true, true, true, false, true };

enum E_PV_JATAK { bool:pjIma, pjDb, Float:pjX, Float:pjY, Float:pjZ, Float:pjA, pjAktor, STREAMER_TAG_3D_TEXT_LABEL:pjTekst }
static gProvalaJatak[PV_JATAKA][E_PV_JATAK];
static gProvalaKucaZadnja[KUCA_MAX];
static gProvalaGepek[MAX_VEHICLES][PV_GEPEK], gProvalaGepekVlasnik[MAX_VEHICLES];

static gProvalaKuca[MAX_PLAYERS] = { -1, ... }, bool:gProvalaUnutra[MAX_PLAYERS], gProvalaTpl[MAX_PLAYERS], gProvalaNosi[MAX_PLAYERS] = { -1, ... }, gProvalaUzeto[MAX_PLAYERS];
static gProvalaUkucanin[MAX_PLAYERS] = { INVALID_LIK, ... }, bool:gProvalaBudan[MAX_PLAYERS], Float:gProvalaBuka[MAX_PLAYERS], bool:gProvalaSunja[MAX_PLAYERS], gProvalaTimer[MAX_PLAYERS], gProvalaApPopis[MAX_PLAYERS][PV_JATAKA];
static STREAMER_TAG_OBJECT:gProvalaObjekt[MAX_PLAYERS][PV_PLIJENA], STREAMER_TAG_AREA:gProvalaZona[MAX_PLAYERS][PV_PLIJENA];
static STREAMER_TAG_AREA:gProvalaIzlaz[MAX_PLAYERS], STREAMER_TAG_AREA:gProvalaVrata[MAX_PLAYERS], STREAMER_TAG_3D_TEXT_LABEL:gProvalaNatpis[MAX_PLAYERS];

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

static ProvalaStvarMjesta(tpl, p)
{
    return floatround(gProvalaMjesto[tpl][p][4]);
}

static ProvalaNosiPrikazi(playerid, bool:nosi)
{
    if (nosi)
    {
        SetPlayerAttachedObject(playerid, PV_SLOT, gProvalaStvar[gProvalaNosi[playerid]][psModel], 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.8, 0.8, 0.8);
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
    }
    if (IsValidDynamicArea(gProvalaIzlaz[playerid])) DestroyDynamicArea(gProvalaIzlaz[playerid]);
    gProvalaIzlaz[playerid] = STREAMER_TAG_AREA:0;
    if (gProvalaTimer[playerid]) KillTimer(gProvalaTimer[playerid]);
    gProvalaTimer[playerid] = 0;
    if (gProvalaUnutra[playerid] && !gProvalaBudan[playerid] && IsPlayerConnected(playerid)) IgraTdMakni(playerid);
    if (gProvalaSunja[playerid] && IsPlayerConnected(playerid)) ClearAnimations(playerid, SYNC_ALL);
    gProvalaSunja[playerid] = false;
    if (gProvalaUkucanin[playerid] != INVALID_LIK) Lik_Ukloni(gProvalaUkucanin[playerid]);
    gProvalaUkucanin[playerid] = INVALID_LIK;
    gProvalaUnutra[playerid] = false;
}

static ProvalaAdresaMakni(playerid)
{
    if (IsValidDynamicArea(gProvalaVrata[playerid])) DestroyDynamicArea(gProvalaVrata[playerid]);
    gProvalaVrata[playerid] = STREAMER_TAG_AREA:0;
    if (IsValidDynamic3DTextLabel(gProvalaNatpis[playerid])) DestroyDynamic3DTextLabel(gProvalaNatpis[playerid]);
    gProvalaNatpis[playerid] = STREAMER_TAG_3D_TEXT_LABEL:0;
    if (gProvalaKuca[playerid] != -1 && IsPlayerConnected(playerid))
    {
        KucaProvalaSakrij(playerid, gProvalaKuca[playerid], false);
        DisablePlayerCheckpoint(playerid);
    }
    gProvalaKuca[playerid] = -1;
    gProvalaBudan[playerid] = false;
}

ProvalaKraj(playerid)
{
    ProvalaUnutraMakni(playerid);
    if (gProvalaNosi[playerid] != -1 && IsPlayerConnected(playerid)) ProvalaNosiPrikazi(playerid, false);
    gProvalaNosi[playerid] = -1;
    ProvalaAdresaMakni(playerid);
}

static ProvalaPreostalo(playerid)
{
    new n = 0, tpl = gProvalaTpl[playerid];
    for (new p = 0; p < gProvalaMjestaBroj[tpl]; p++) if (!(gProvalaUzeto[playerid] >> p & 1)) n++;
    return n;
}

static ProvalaAdresa(playerid, j)
{
    new Float:x, Float:y, Float:z, tpl;
    for (new pokusaj = 0; pokusaj < 80; pokusaj++)
    {
        new k = random(KUCA_MAX);
        if (!KucaProvala(k, x, y, z, tpl) || tpl < 0 || tpl >= sizeof gProvalaImaKrevet || !gProvalaImaKrevet[tpl] || !gProvalaMjestaBroj[tpl]) continue;
        if (gettime() - gProvalaKucaZadnja[k] < PV_KUCA_PAUZA || VectorSize(x - gProvalaJatak[j][pjX], y - gProvalaJatak[j][pjY], 0.0) < PV_JATAK_UDALJENOST) continue;
        gProvalaKuca[playerid] = k;
        gProvalaTpl[playerid] = tpl;
        gProvalaUzeto[playerid] = 0;
        gProvalaBudan[playerid] = false;
        gProvalaKucaZadnja[k] = gettime();
        SetPlayerCheckpoint(playerid, x, y, z, 1.5);
        gProvalaVrata[playerid] = InterakcijaZona(x, y, z, PV_DOMET, 0, 0, SAVJET_ULAZ);
        KucaProvalaSakrij(playerid, k, true);
        gProvalaNatpis[playerid] = CreateDynamic3DTextLabel(T(playerid, "Kuca za provalu"), PV_BOJA, x, y, z + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 1, 0, 0, playerid);
        new poruka[144];
        format(poruka, sizeof poruka, T(playerid, "Adresa je %s (oznaceno na karti). Udite nocu, budite tihi i pokupite sto vise stvari u kombi."), CestaZona(x, y));
        ObavijestInfo(playerid, T(playerid, "Jatak"), poruka);
        return 1;
    }
    AktorKaze(playerid, gProvalaJatak[j][pjAktor], "Jatak", "Trenutno nemam nista za tebe. Dodji kasnije.");
    return 1;
}

static ProvalaUkucaninStvori(playerid, interijer)
{
    new tpl = gProvalaTpl[playerid];
    new lik = Lik_Stvori(gProvalaUkucani[(gProvalaKuca[playerid] + tpl) % sizeof gProvalaUkucani], gProvalaKrevet[tpl][0], gProvalaKrevet[tpl][1], gProvalaKrevet[tpl][2] + 1.0, gProvalaKrevet[tpl][3], PV_SVIJET + playerid, interijer);
    gProvalaUkucanin[playerid] = lik;
    if (lik == INVALID_LIK) return;
    Lik_Neranjiv(lik, true);
    if (gProvalaBudan[playerid])
    {
        ProvalaUkucaninNapadni(playerid);
        return;
    }
    Lik_Animacija(lik, "BEACH", "Lay_Bac_Loop", 4.1, 1, 0, 0, 1);
    SetTimerEx("ProvalaUkucaninSpava", 1000, false, "ii", playerid, lik);
}

forward ProvalaUkucaninSpava(playerid, lik);
public ProvalaUkucaninSpava(playerid, lik)
{
    if (gProvalaUkucanin[playerid] != lik || gProvalaBudan[playerid]) return;
    Lik_Animacija(lik, "BEACH", "Lay_Bac_Loop", 4.1, 1, 0, 0, 1);
}

static ProvalaUkucaninNapadni(playerid)
{
    new lik = gProvalaUkucanin[playerid];
    if (lik == INVALID_LIK) return;
    Lik_OcistiAnimaciju(lik);
    Lik_Oruzje(lik, _:WEAPON_SHOTGUN, PV_SACMARICA_METAKA);
    Lik_Prati(lik, playerid, LIK_TRK, PV_PRATI_RAZMAK);
    Lik_Nacilja(lik, playerid, true, PV_PUCAJ_MS);
}

static ProvalaUdji(playerid)
{
    if (!ProvalaNoc())
    {
        ProvalaAdresaMakni(playerid);
        return ObavijestGreska(playerid, T(playerid, "Svanulo je, ukucani su budni. Adresa je propala."));
    }
    if (gProvalaNosi[playerid] != -1) return ObavijestGreska(playerid, T(playerid, "Prvo odlozite ono sto nosite."));
    new tpl = gProvalaTpl[playerid], interijer, Float:ix, Float:iy, Float:iz;
    KucaIntPoz(tpl, interijer, ix, iy, iz);
    DisablePlayerCheckpoint(playerid);
    PrebaciIgraca(playerid, interijer, PV_SVIJET + playerid, ix, iy, iz);
    gProvalaUnutra[playerid] = true;
    gProvalaBuka[playerid] = 0.0;
    gProvalaSunja[playerid] = false;
    for (new p = 0; p < gProvalaMjestaBroj[tpl]; p++)
    {
        if (gProvalaUzeto[playerid] >> p & 1) continue;
        gProvalaObjekt[playerid][p] = CreateDynamicObject(gProvalaStvar[ProvalaStvarMjesta(tpl, p)][psModel], gProvalaMjesto[tpl][p][0], gProvalaMjesto[tpl][p][1], gProvalaMjesto[tpl][p][2], 0.0, 0.0, gProvalaMjesto[tpl][p][3], PV_SVIJET + playerid, interijer);
        gProvalaZona[playerid][p] = InterakcijaZona(gProvalaMjesto[tpl][p][0], gProvalaMjesto[tpl][p][1], gProvalaMjesto[tpl][p][2], PV_KUTIJA_DOMET, PV_SVIJET + playerid, interijer, SAVJET_PODIGNI);
    }
    gProvalaIzlaz[playerid] = InterakcijaZona(ix, iy, iz, PV_DOMET, PV_SVIJET + playerid, interijer, SAVJET_IZLAZ);
    ProvalaUkucaninStvori(playerid, interijer);
    if (gProvalaBudan[playerid]) return 1;
    IgraPanel(playerid, T(playerid, "Buka"), 60.0, PV_BOJA);
    IgraTraka(playerid, 0.0, 60.0, 0x27AE60FF, 0x27AE60FF, 0xF39C12FF, T(playerid, "Drzite ~y~ALT~w~ i hodajte polako da ne probudite ukucane."), PV_BUKA_CRVENO);
    IgraOznakaPomakni(playerid, 0.0);
    gProvalaTimer[playerid] = SetTimerEx("ProvalaBuka", PV_TIK_MS, true, "i", playerid);
    return 1;
}

static ProvalaIzadji(playerid)
{
    new k = gProvalaKuca[playerid], Float:x, Float:y, Float:z, tpl;
    ProvalaUnutraMakni(playerid);
    if (KucaProvala(k, x, y, z, tpl)) PrebaciIgraca(playerid, 0, 0, x, y, z);
    if (gProvalaBudan[playerid] || !ProvalaPreostalo(playerid)) ProvalaAdresaMakni(playerid);
    else SetPlayerCheckpoint(playerid, x, y, z, 1.5);
    if (gProvalaNosi[playerid] != -1) ObavijestInfo(playerid, T(playerid, "Jatak"), T(playerid, "Odnesite stvar do svog kombija (ALT kod straznjih vrata), pa robu prodajte jataku."));
    return 1;
}

forward ProvalaBuka(playerid);
public ProvalaBuka(playerid)
{
    if (!gProvalaUnutra[playerid] || !IsPlayerConnected(playerid)) return;
    new Float:vx, Float:vy, Float:vz, KEY:tipke, gd, lr;
    GetPlayerVelocity(playerid, vx, vy, vz);
    GetPlayerKeys(playerid, tipke, gd, lr);
    new Float:brzina = VectorSize(vx, vy, 0.0), bool:krece = gd != 0 || lr != 0, bool:nosi = gProvalaNosi[playerid] != -1, Float:korak;
    new bool:sunja = (tipke & KEY_WALK) && krece && GetPlayerState(playerid) == PLAYER_STATE_ONFOOT;
    if (sunja && !nosi)
    {
        new Float:cx, Float:cy, Float:cz;
        GetPlayerCameraFrontVector(playerid, cx, cy, cz);
        SetPlayerFacingAngle(playerid, atan2(-cx, cy));
        if (!gProvalaSunja[playerid]) ApplyAnimation(playerid, "PED", "Player_Sneak", 4.1, true, true, true, false, 0, SYNC_ALL);
        gProvalaSunja[playerid] = true;
    }
    else if (gProvalaSunja[playerid])
    {
        gProvalaSunja[playerid] = false;
        ClearAnimations(playerid, SYNC_ALL);
    }
    if (!krece && brzina <= 0.02) korak = -0.4;
    else if (sunja) korak = nosi ? 0.3 : 0.15;
    else if (brzina > 0.15) korak = 4.5;
    else if (brzina > 0.09) korak = nosi ? 3.0 : 2.5;
    else korak = 1.2;
    gProvalaBuka[playerid] += korak;
    if (gProvalaBuka[playerid] < 0.0) gProvalaBuka[playerid] = 0.0;
    if (gProvalaBuka[playerid] > float(PV_BUKA_MAX)) gProvalaBuka[playerid] = float(PV_BUKA_MAX);
    IgraOznakaPomakni(playerid, gProvalaBuka[playerid]);
    if (gProvalaBuka[playerid] < float(PV_BUKA_MAX)) return;
    if (gProvalaSunja[playerid])
    {
        gProvalaSunja[playerid] = false;
        ClearAnimations(playerid, SYNC_ALL);
    }
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
    ProvalaUkucaninNapadni(playerid);
}

ProvalaStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_PASSENGER) return;
    if (gProvalaKuca[playerid] == -1 || gProvalaUnutra[playerid] || !gProvalaUzeto[playerid]) return;
    new v = GetPlayerVehicleID(playerid);
    if (VlasnikVozila(v) != gAccountId[playerid] || !ProvalaJeKombi(v)) return;
    ProvalaAdresaMakni(playerid);
    ObavijestInfo(playerid, T(playerid, "Jatak"), T(playerid, "Odustali ste od ove kuce. Ukradene stvari prodajte jataku."));
}

static ProvalaPlijenBlizu(playerid)
{
    new tpl = gProvalaTpl[playerid];
    for (new p = 0; p < gProvalaMjestaBroj[tpl]; p++)
        if (!(gProvalaUzeto[playerid] >> p & 1) && IsPlayerInRangeOfPoint(playerid, PV_KUTIJA_DOMET, gProvalaMjesto[tpl][p][0], gProvalaMjesto[tpl][p][1], gProvalaMjesto[tpl][p][2])) return p;
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

ProvalaF(playerid)
{
    if (!gUlogiran[playerid] || IsRanjen(playerid) || gProvalaKuca[playerid] == -1) return 0;
    if (gProvalaUnutra[playerid])
    {
        new interijer, Float:ix, Float:iy, Float:iz;
        KucaIntPoz(gProvalaTpl[playerid], interijer, ix, iy, iz);
        if (!IsPlayerInRangeOfPoint(playerid, PV_DOMET, ix, iy, iz)) return 0;
        return ProvalaIzadji(playerid);
    }
    new Float:x, Float:y, Float:z, tpl;
    if (!KucaProvala(gProvalaKuca[playerid], x, y, z, tpl) || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || !IsPlayerInRangeOfPoint(playerid, PV_DOMET, x, y, z)) return 0;
    return ProvalaUdji(playerid), 1;
}

ProvalaAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (gProvalaUnutra[playerid])
    {
        if (gProvalaNosi[playerid] != -1) return 0;
        new p = ProvalaPlijenBlizu(playerid);
        if (p == -1) return 0;
        gProvalaUzeto[playerid] |= 1 << p;
        DestroyDynamicObject(gProvalaObjekt[playerid][p]);
        gProvalaObjekt[playerid][p] = STREAMER_TAG_OBJECT:0;
        if (IsValidDynamicArea(gProvalaZona[playerid][p])) DestroyDynamicArea(gProvalaZona[playerid][p]);
        gProvalaZona[playerid][p] = STREAMER_TAG_AREA:0;
        if (!gProvalaBudan[playerid]) gProvalaBuka[playerid] = gProvalaBuka[playerid] + 8.0 > float(PV_BUKA_MAX) - 1.0 ? float(PV_BUKA_MAX) - 1.0 : gProvalaBuka[playerid] + 8.0;
        ProvalaUzmi(playerid, ProvalaStvarMjesta(gProvalaTpl[playerid], p));
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
