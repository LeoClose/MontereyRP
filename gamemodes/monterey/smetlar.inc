static const Float:gSmecKanta[][3] = {
    { 338.055, 39.844, 6.935 }, { 339.719, 52.984, 4.038 }, { 1353.102, 201.758, 20.038 }, { 255.117, -161.289, 2.077 },
    { 657.852, -650.070, 16.835 }, { 665.227, -624.375, 16.835 }, { 652.742, -496.516, 16.819 }, { 656.000, -439.273, 17.172 },
    { 1362.039, 232.609, 20.038 }, { 1315.250, 349.562, 20.030 }, { -68.352, -1104.992, 1.431 }, { 1920.484, -2088.172, 13.739 },
    { 2382.023, -1941.039, 13.876 }, { 1336.336, -1842.852, 13.715 }, { 1461.430, -1489.219, 13.731 }, { 1439.859, -1323.703, 14.114 },
    { 1603.602, -1060.344, 24.193 }, { 1520.156, -1018.555, 24.575 }, { 1618.375, -1030.359, 24.193 }, { 1623.562, -1045.047, 24.193 },
    { 1633.023, -1059.852, 24.334 }, { 1649.430, -1076.922, 24.193 }, { 1659.914, -1065.117, 24.528 }, { 1684.414, -1069.930, 24.303 },
    { 1631.922, -987.828, 24.575 }, { 1660.250, -1000.414, 24.520 }, { 1666.398, -997.703, 24.520 }, { 1786.211, -1148.297, 24.411 },
    { 1789.773, -1116.062, 24.622 }, { 1747.859, -1063.297, 24.193 }, { 1758.078, -1064.555, 24.193 }, { 861.984, -1380.461, 14.778 },
    { 387.133, -1831.445, 8.384 }, { 363.023, -1716.688, 24.025 }, { 387.148, -1751.508, 9.162 }, { 911.703, -836.203, 92.456 },
    { 960.242, -822.859, 96.636 }, { 974.922, -815.516, 97.824 }, { 914.312, -670.359, 117.386 }, { 1020.242, -794.883, 101.824 },
    { 1072.141, -778.383, 107.542 }, { 2426.727, -1016.750, 54.542 }, { 2436.711, -1052.250, 54.527 }, { 2459.453, -1051.609, 59.941 },
    { 2457.398, -1023.172, 59.893 }, { 2484.086, -1053.141, 67.738 }, { 2489.617, -1022.609, 65.613 }, { 2505.078, -1061.078, 70.410 },
    { 2527.922, -1058.141, 69.785 }, { 2510.289, -1029.586, 70.277 }, { 2528.672, -1034.922, 69.769 }, { 2538.648, -1058.602, 69.769 },
    { 2545.156, -1034.898, 69.761 }, { 2567.820, -1035.281, 69.769 }, { 1985.051, -1655.907, 13.779 }, { 2213.047, -1318.000, 24.550 }
};

#define SMEC_KAMION_MODEL   408
#define SMEC_VRECA_MODEL    1265
#define SMEC_OZNAKA_MODEL   1318
#define SMEC_SLOT           2
#define SMEC_DOMET          2.5
#define SMEC_KANTA_DOMET    2.2
#define SMEC_STRAG_DOMET    2.6
#define SMEC_ISTOVAR_DOMET  7.0
#define SMEC_RUTA           8
#define SMEC_PRVA_DALEKO    1600.0
#define SMEC_SLJEDECA       550.0
#define SMEC_KANTA_PAUZA    900
#define SMEC_PLACA_VRECA    30
#define SMEC_PLACA_ISTOVAR  40
#define SMEC_ISTOVAR_S      5
#define SMEC_KAMION_HP      300.0
#define SMEC_BOJA           0x7CB342FF
#define SMEC_SAVJET_MS      3000

enum { SMF_RUTA, SMF_ODLAGALISTE, SMF_ISTOVAR }

static const Float:gSmecFlota[][4] = {
    { 2434.500, -2115.000, 13.600, 90.0 }, { 2434.500, -2110.000, 13.600, 90.0 },
    { 2434.500, -2105.000, 13.600, 90.0 }, { 2434.500, -2100.000, 13.600, 90.0 }
};
static const Float:gSmecIstovar[3] = { 2433.000, -2092.000, 13.550 };
static Float:gSmecMjesto[4] = { 2427.500, -2104.000, 13.550, 270.0 }, STREAMER_TAG_3D_TEXT_LABEL:gSmecTekst;
static gSmecFlotaV[sizeof gSmecFlota], gSmecFlotaIgrac[sizeof gSmecFlota] = { -1, ... };
static gSmecKantaDo[sizeof gSmecKanta];
static bool:gSmecSmjena[MAX_PLAYERS], gSmecKamion[MAX_PLAYERS], gSmecRuta[MAX_PLAYERS][SMEC_RUTA], gSmecBr[MAX_PLAYERS], gSmecIdx[MAX_PLAYERS], bool:gSmecNosi[MAX_PLAYERS];
static gSmecUtovareno[MAX_PLAYERS], gSmecFaza[MAX_PLAYERS], gSmecIstovarDo[MAX_PLAYERS], gSmecSljedeca[MAX_PLAYERS], gSmecZarada[MAX_PLAYERS], gSmecSavjet[MAX_PLAYERS];
static STREAMER_TAG_OBJECT:gSmecObj[MAX_PLAYERS];

InitSmetlar()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS job_garbage (id INTEGER PRIMARY KEY, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT x, y, z, a FROM job_garbage WHERE id = 1");
    if (DB_GetRowCount(res)) for (new k = 0; k < 4; k++) gSmecMjesto[k] = DB_GetFieldFloat(res, k);
    DB_FreeResultSet(res);
    for (new f = 0; f < sizeof gSmecFlota; f++) gSmecFlotaV[f] = CreateVehicle(SMEC_KAMION_MODEL, gSmecFlota[f][0], gSmecFlota[f][1], gSmecFlota[f][2], gSmecFlota[f][3], 1, 1, 600);
    for (new i = 0; i < MAX_PLAYERS; i++) gSmecKamion[i] = INVALID_VEHICLE_ID;
    SmecMjestoStvori();
}

static SmecMjestoStvori()
{
    if (IsValidDynamic3DTextLabel(gSmecTekst)) DestroyDynamic3DTextLabel(gSmecTekst);
    gSmecTekst = CreateDynamic3DTextLabel("Smetlar\nALT", SMEC_BOJA, gSmecMjesto[0], gSmecMjesto[1], gSmecMjesto[2] + 0.5, 10.0, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, 0, 0);
    InterakcijaOznaka(gSmecTekst, SMEC_DOMET);
}

static SmecFlota(v)
{
    for (new f = 0; f < sizeof gSmecFlota; f++) if (gSmecFlotaV[f] == v) return f;
    return -1;
}

static bool:SmecKamionZauzet(v)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && GetPlayerVehicleID(i) == v) return true;
    return false;
}

static bool:SmecNaMjestu(playerid, Float:r = SMEC_DOMET)
{
    return !GetPlayerInterior(playerid) && !GetPlayerVirtualWorld(playerid) && IsPlayerInRangeOfPoint(playerid, r, gSmecMjesto[0], gSmecMjesto[1], gSmecMjesto[2]);
}

static bool:SmecIzaKamiona(playerid)
{
    if (gSmecKamion[playerid] == INVALID_VEHICLE_ID || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return false;
    new Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(gSmecKamion[playerid], x, y, z);
    GetVehicleZAngle(gSmecKamion[playerid], a);
    GetVehicleModelInfo(SMEC_KAMION_MODEL, VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    x += floatsin(a, degrees) * (sy / 2.0 + 0.6);
    y -= floatcos(a, degrees) * (sy / 2.0 + 0.6);
    return IsPlayerInRangeOfPoint(playerid, SMEC_STRAG_DOMET, x, y, z);
}

static bool:SmecKodKante(playerid)
{
    if (gSmecFaza[playerid] != SMF_RUTA || gSmecIdx[playerid] >= gSmecBr[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT) return false;
    new b = gSmecRuta[playerid][gSmecIdx[playerid]];
    return IsPlayerInRangeOfPoint(playerid, SMEC_KANTA_DOMET, gSmecKanta[b][0], gSmecKanta[b][1], gSmecKanta[b][2] + 1.0);
}

static SmecVreca(playerid, bool:nosi)
{
    gSmecNosi[playerid] = nosi;
    if (!IsPlayerConnected(playerid)) return;
    if (nosi)
    {
        SetPlayerAttachedObject(playerid, SMEC_SLOT, SMEC_VRECA_MODEL, 5, 0.1, 0.1, 0.15, 0.0, 90.0, 90.0, 0.8, 0.8, 0.8);
        SetPlayerSpecialAction(playerid, SPECIAL_ACTION_CARRY);
        return;
    }
    RemovePlayerAttachedObject(playerid, SMEC_SLOT);
    if (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_CARRY) SetPlayerSpecialAction(playerid, SPECIAL_ACTION_NONE);
}

static SmecOznakaMakni(playerid)
{
    if (_:gSmecObj[playerid]) DestroyDynamicObject(gSmecObj[playerid]);
    gSmecObj[playerid] = STREAMER_TAG_OBJECT:0;
}

static SmecPanel(playerid)
{
    new poruka[192];
    switch (gSmecFaza[playerid])
    {
        case SMF_RUTA:
        {
            if (!gSmecBr[playerid]) format(poruka, sizeof poruka, T(playerid, "Cekate novu rutu..."));
            else format(poruka, sizeof poruka, T(playerid, "Kanta %d/%d - %s~n~U kamionu: %d vreca"), gSmecIdx[playerid] + 1, gSmecBr[playerid], CestaZona(gSmecKanta[gSmecRuta[playerid][gSmecIdx[playerid]]][0], gSmecKanta[gSmecRuta[playerid][gSmecIdx[playerid]]][1]), gSmecUtovareno[playerid]);
        }
        case SMF_ODLAGALISTE: format(poruka, sizeof poruka, T(playerid, "Kamion je pun (%d vreca).~n~Vozite na odlagaliste i istovarite smece."), gSmecUtovareno[playerid]);
        default: format(poruka, sizeof poruka, T(playerid, "Istovar u tijeku..."));
    }
    IgraObavijestPanel(playerid, T(playerid, "Smetlar"), SMEC_BOJA, poruka);
}

static SmecPokaziKantu(playerid)
{
    SmecOznakaMakni(playerid);
    new b = gSmecRuta[playerid][gSmecIdx[playerid]];
    SetPlayerCheckpoint(playerid, gSmecKanta[b][0], gSmecKanta[b][1], gSmecKanta[b][2], 4.0);
    gSmecObj[playerid] = CreateDynamicObject(SMEC_OZNAKA_MODEL, gSmecKanta[b][0], gSmecKanta[b][1], gSmecKanta[b][2] + 2.6, 0.0, 0.0, 0.0, 0, 0, playerid);
    SmecPanel(playerid);
}

static SmecNovaRuta(playerid)
{
    new sad = gettime(), kandidati[sizeof gSmecKanta], n = 0;
    for (new b = 0; b < sizeof gSmecKanta; b++)
        if (gSmecKantaDo[b] <= sad && VectorSize(gSmecKanta[b][0] - gSmecIstovar[0], gSmecKanta[b][1] - gSmecIstovar[1], 0.0) <= SMEC_PRVA_DALEKO) kandidati[n++] = b;
    if (!n)
        for (new b = 0; b < sizeof gSmecKanta; b++)
            if (VectorSize(gSmecKanta[b][0] - gSmecIstovar[0], gSmecKanta[b][1] - gSmecIstovar[1], 0.0) <= SMEC_PRVA_DALEKO) kandidati[n++] = b;
    new b = kandidati[random(n)];
    gSmecBr[playerid] = 0;
    gSmecRuta[playerid][gSmecBr[playerid]++] = b;
    gSmecKantaDo[b] = sad + SMEC_KANTA_PAUZA;
    while (gSmecBr[playerid] < SMEC_RUTA)
    {
        new najbliza = -1, Float:nd = SMEC_SLJEDECA, zauzeta = -1, Float:zd = SMEC_SLJEDECA;
        for (new k = 0; k < sizeof gSmecKanta; k++)
        {
            new bool:u = false;
            for (new r = 0; r < gSmecBr[playerid]; r++) if (gSmecRuta[playerid][r] == k) u = true;
            if (u) continue;
            new Float:d = VectorSize(gSmecKanta[k][0] - gSmecKanta[b][0], gSmecKanta[k][1] - gSmecKanta[b][1], 0.0);
            if (gSmecKantaDo[k] > sad) { if (d < zd) zd = d, zauzeta = k; }
            else if (d < nd) nd = d, najbliza = k;
        }
        if (najbliza == -1) najbliza = zauzeta;
        if (najbliza == -1) break;
        b = najbliza;
        gSmecRuta[playerid][gSmecBr[playerid]++] = b;
        gSmecKantaDo[b] = sad + SMEC_KANTA_PAUZA;
    }
    gSmecIdx[playerid] = 0;
    gSmecFaza[playerid] = SMF_RUTA;
    gSmecSljedeca[playerid] = 0;
    new poruka[128];
    format(poruka, sizeof poruka, T(playerid, "Nova ruta: %d kanti.~n~Pocinje u: %s"), gSmecBr[playerid], CestaZona(gSmecKanta[gSmecRuta[playerid][0]][0], gSmecKanta[gSmecRuta[playerid][0]][1]));
    IgraObavijest(playerid, T(playerid, "Smetlar"), SMEC_BOJA, poruka, 6000);
    SmecPokaziKantu(playerid);
}

SmetlarKraj(playerid)
{
    if (!gSmecSmjena[playerid]) return;
    gSmecSmjena[playerid] = false;
    SmecVreca(playerid, false);
    SmecOznakaMakni(playerid);
    new f = SmecFlota(gSmecKamion[playerid]);
    if (f != -1)
    {
        gSmecFlotaIgrac[f] = -1;
        SetVehicleToRespawn(gSmecKamion[playerid]);
    }
    gSmecKamion[playerid] = INVALID_VEHICLE_ID;
    gSmecBr[playerid] = 0;
    if (IsPlayerConnected(playerid))
    {
        DisablePlayerCheckpoint(playerid);
        IgraObavijestUkloni(playerid);
    }
}

static SmecPocni(playerid)
{
    new f = -1, Float:najblize = 0.0;
    for (new k = 0; k < sizeof gSmecFlota; k++)
    {
        new Float:hp;
        GetVehicleHealth(gSmecFlotaV[k], hp);
        if (gSmecFlotaIgrac[k] != -1 || hp < SMEC_KAMION_HP || SmecKamionZauzet(gSmecFlotaV[k])) continue;
        new Float:d = GetPlayerDistanceFromPoint(playerid, gSmecFlota[k][0], gSmecFlota[k][1], gSmecFlota[k][2]);
        if (f == -1 || d < najblize) f = k, najblize = d;
    }
    if (f == -1) return ObavijestGreska(playerid, T(playerid, "Trenutno nema slobodnih kamiona.")), 1;
    new v = gSmecFlotaV[f];
    gSmecFlotaIgrac[f] = playerid;
    SetVehicleToRespawn(v);
    VoziloNapuni(v);
    gSmecKamion[playerid] = v;
    gSmecSmjena[playerid] = true;
    gSmecZarada[playerid] = 0;
    gSmecUtovareno[playerid] = 0;
    gSmecSavjet[playerid] = 0;
    gSmecBr[playerid] = 0;
    gSmecFaza[playerid] = SMF_RUTA;
    gSmecSljedeca[playerid] = gettime() + 3;
    Fade(playerid, 0);
    SetTimerEx("SmecPocniKraj", FADE_SLOJEVA * FADE_KORAK_MS + 150, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

forward SmecPocniKraj(playerid, racun);
public SmecPocniKraj(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    Fade(playerid, FADE_SLOJEVA + 1);
    if (!gSmecSmjena[playerid]) return;
    PutPlayerInVehicle(playerid, gSmecKamion[playerid], 0);
    VoziloSet(gSmecKamion[playerid], VP_MOTOR, 1);
    SmecPanel(playerid);
    Savjet(playerid, T(playerid, "Vozite do kante, izadite i pritisnite ~y~ALT~w~ kod kante~n~Vrecu ubacite s ~y~ALT~w~ iza kamiona~n~~y~/posao~w~ - kraj smjene"), 8000);
}

static SmecIstovarGotov(playerid)
{
    new vrece = gSmecUtovareno[playerid], iznos = vrece * SMEC_PLACA_VRECA + SMEC_PLACA_ISTOVAR, poruka[160];
    gSmecUtovareno[playerid] = 0;
    gSmecBr[playerid] = 0;
    gSmecFaza[playerid] = SMF_RUTA;
    iznos = BlagajnaIsplati(iznos);
    if (!DajNovac(playerid, iznos)) BlagajnaDodaj(iznos), iznos = 0;
    gSmecZarada[playerid] += iznos;
    format(poruka, sizeof poruka, T(playerid, "Istovarili ste %d vreca. +$%d~n~Zarada ove smjene: $%d~n~Nova ruta stize uskoro."), vrece, iznos, gSmecZarada[playerid]);
    IgraObavijest(playerid, T(playerid, "Smetlar"), SMEC_BOJA, poruka, 6000);
    PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
    gSmecSljedeca[playerid] = gettime() + 5;
}

SmetlarAlt(playerid)
{
    if (!gUlogiran[playerid] || GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || IsRanjen(playerid)) return 0;
    if (gSmecSmjena[playerid])
    {
        if (gSmecNosi[playerid] && SmecIzaKamiona(playerid))
        {
            SmecVreca(playerid, false);
            ApplyAnimation(playerid, "GRENADE", "WEAPON_throwu", 4.1, false, false, false, false, 0, SYNC_ALL);
            PlayerPlaySound(playerid, 1056, 0.0, 0.0, 0.0);
            gSmecUtovareno[playerid]++;
            if (++gSmecIdx[playerid] >= gSmecBr[playerid])
            {
                gSmecFaza[playerid] = SMF_ODLAGALISTE;
                SmecOznakaMakni(playerid);
                SetPlayerCheckpoint(playerid, gSmecIstovar[0], gSmecIstovar[1], gSmecIstovar[2], 6.0);
                IgraObavijest(playerid, T(playerid, "Smetlar"), SMEC_BOJA, T(playerid, "Kamion je pun.~n~Vozite na odlagaliste i istovarite smece."), 6000);
            }
            else SmecPokaziKantu(playerid);
            return 1;
        }
        if (!gSmecNosi[playerid] && SmecKodKante(playerid))
        {
            ApplyAnimation(playerid, "CARRY", "liftup", 4.1, false, false, false, false, 0, SYNC_ALL);
            SmecVreca(playerid, true);
            DisablePlayerCheckpoint(playerid);
            SmecOznakaMakni(playerid);
            return 1;
        }
    }
    if (!SmecNaMjestu(playerid)) return 0;
    if (!gSmecSmjena[playerid]) return UpitDaNe(playerid, UPIT_SMETLAR, 0, 1, T(playerid, "Smetlar"), T(playerid, "Zelite li zapoceti smjenu?")), 1;
    return UpitDaNe(playerid, UPIT_SMETLAR, 0, 2, T(playerid, "Smetlar"), T(playerid, "Zelite li zavrsiti smjenu?")), 1;
}

SmetlarUpit(playerid, opcija, bool:da)
{
    if (!da || !SmecNaMjestu(playerid, SMEC_DOMET + 2.0)) return 1;
    if (opcija == 1 && !gSmecSmjena[playerid]) return SmecPocni(playerid);
    if (opcija == 2 && gSmecSmjena[playerid])
    {
        SmetlarKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Smetlar"), T(playerid, "Smjena je zavrsena."));
    }
    return 1;
}

SmetlarPosao(playerid)
{
    if (gSmecSmjena[playerid])
    {
        SmetlarKraj(playerid);
        return ObavijestInfo(playerid, T(playerid, "Smetlar"), T(playerid, "Smjena je zavrsena.")), 1;
    }
    if (GetPlayerState(playerid) != PLAYER_STATE_ONFOOT || !SmecNaMjestu(playerid)) return 0;
    return UpitDaNe(playerid, UPIT_SMETLAR, 0, 1, T(playerid, "Smetlar"), T(playerid, "Zelite li zapoceti smjenu?")), 1;
}

SmetlarStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER && newstate != PLAYER_STATE_PASSENGER) return;
    if (gSmecNosi[playerid])
    {
        SmecVreca(playerid, false);
        if (gSmecSmjena[playerid] && gSmecFaza[playerid] == SMF_RUTA && gSmecBr[playerid]) SmecPokaziKantu(playerid);
    }
    new v = GetPlayerVehicleID(playerid), f = SmecFlota(v);
    if (f == -1 || gSmecKamion[playerid] == v) return;
    RemovePlayerFromVehicle(playerid);
    ObavijestGreska(playerid, T(playerid, "Kamion je samo za smetlare na smjeni. Smjenu pocinjete s ALT kod smetlara."));
}

SmetlarTick()
{
    new sad = gettime();
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!gSmecSmjena[i]) continue;
        new v = gSmecKamion[i], Float:hp;
        GetVehicleHealth(v, hp);
        if (!IsPlayerConnected(i) || IsRanjen(i) || !GetVehicleModel(v) || hp < SMEC_KAMION_HP)
        {
            SmetlarKraj(i);
            if (IsPlayerConnected(i)) ObavijestGreska(i, T(i, "Kamion je unisten, smjena je zavrsena."));
            continue;
        }
        if (gSmecFaza[i] == SMF_RUTA && !gSmecBr[i] && sad >= gSmecSljedeca[i]) SmecNovaRuta(i);
        else if (gSmecFaza[i] == SMF_ODLAGALISTE && GetPlayerVehicleID(i) == v && IsPlayerInRangeOfPoint(i, SMEC_ISTOVAR_DOMET, gSmecIstovar[0], gSmecIstovar[1], gSmecIstovar[2]))
        {
            new Float:vx, Float:vy, Float:vz;
            GetVehicleVelocity(v, vx, vy, vz);
            if (VectorSize(vx, vy, vz) < 0.05)
            {
                gSmecFaza[i] = SMF_ISTOVAR;
                gSmecIstovarDo[i] = sad + SMEC_ISTOVAR_S;
                DisablePlayerCheckpoint(i);
                SmecPanel(i);
            }
        }
        else if (gSmecFaza[i] == SMF_ISTOVAR)
        {
            if (GetPlayerVehicleID(i) != v || !IsPlayerInRangeOfPoint(i, SMEC_ISTOVAR_DOMET + 3.0, gSmecIstovar[0], gSmecIstovar[1], gSmecIstovar[2]))
            {
                gSmecFaza[i] = SMF_ODLAGALISTE;
                SetPlayerCheckpoint(i, gSmecIstovar[0], gSmecIstovar[1], gSmecIstovar[2], 6.0);
                SmecPanel(i);
            }
            else if (sad >= gSmecIstovarDo[i]) SmecIstovarGotov(i);
        }
        if (!IgraObavijestVidljiva(i)) SmecPanel(i);
        new savjet = 0;
        if (gSmecNosi[i] && SmecIzaKamiona(i)) savjet = 1;
        else if (!gSmecNosi[i] && SmecKodKante(i)) savjet = 2;
        if (savjet && savjet != gSmecSavjet[i])
        {
            if (savjet == 1) Savjet(i, T(i, "~y~ALT~w~ - ubaci vrecu u kamion"), SMEC_SAVJET_MS);
            else Savjet(i, T(i, "~y~ALT~w~ - uzmi vrecu iz kante"), SMEC_SAVJET_MS);
        }
        gSmecSavjet[i] = savjet;
    }
}

ShowSmetlarAdmin(playerid)
{
    new list[128], len = 0;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Smetlar mjesto na mojoj poziciji"));
    return PrikaziDialog(playerid, DLG_APANEL_SMETLAR, DIALOG_STYLE_LIST, T(playerid, "Smetlar"), list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

DLG:apanel_smetlar(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG) || listitem != 0) return 1;
    new Float:x, Float:y, Float:z, Float:a;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR REPLACE INTO job_garbage (id, x, y, z, a) VALUES (1, %f, %f, %f, %f)", x, y, z, a));
    gSmecMjesto[0] = x, gSmecMjesto[1] = y, gSmecMjesto[2] = z, gSmecMjesto[3] = a;
    SmecMjestoStvori();
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Smetlar mjesto postavljeno."));
    return ShowSmetlarAdmin(playerid);
}

SmetlarLokacija(&Float:x, &Float:y, &Float:z)
{
    x = gSmecMjesto[0];
    y = gSmecMjesto[1];
    z = gSmecMjesto[2];
    return 1;
}
