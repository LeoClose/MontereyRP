#define SLUCAJ_MAX          10
#define SLUCAJ_OBJEKT        18688
#define SLUCAJ_TRAJANJE_S    180
#define SLUCAJ_POZIV_S       30
#define SLUCAJ_GASENJE_S     8
#define SLUCAJ_ODLAZAK_S     120
#define SLUCAJ_DOMET_APARAT  6.0
#define SLUCAJ_DOMET_CIJEV   25.0
#define SLUCAJ_RAZMAK        15.0
#define SLUCAJ_POVRATAK_S   90
#define SLUCAJ_IZLAZAK_MS    2500
#define SLUCAJ_DOMET_NPC     3.0
#define SLUCAJ_MUNICIJA      500
#define KAMION_MODEL        407
#define KAMION_SKIN         277
#define KAMION_BOJA         3
#define KAMION_MAX          24
#define MEHANICAR_IGRAC_DOMET 6.0
#define MEHANICAR_MODEL     525
#define MEHANICAR_SKIN      50
#define MEHANICAR_BOJA      1
#define MEHANICAR_DOMET     3.5
#define MEHANICAR_STETA     900.0
#define MEHANICAR_CEKA_S    60
#define MEHANICAR_GORIVO    60
#define MEHANICAR_POPRAVAK  300
#define MEHANICAR_GUME      200
#define BOLNICA_SPAWN_CIJENA 1500
#define BOLNICA_CRNO_MS     1000
#define MEHANICAR_ODVOZ     2500
#define MEHANICAR_ODVOZ_MS  20000
#define MEHANICAR_ODVOZ_SVIJET 9900
#define MEHANICAR_ODVOZ_VOZILA 8

forward bool:VlasnikNaParkingu(v);
forward bool:GarazaIzvana(db, &Float:x, &Float:y);
#define MEHANICAR_SLEPANJE  150
#define RADIONICA_X         1207.18
#define RADIONICA_Y         -981.57
#define RADIONICA_Z         43.0
#define MEHANICAR_ODGOVOR_MS 2000
#define MEHANICAR_RAD_MS    6000
#define KAMION_PO_TIPU      8
#define AMBULANTA_MODEL     416
#define AMBULANTA_SKIN      274
#define AMBULANTA_BOJA      1
#define RANJEN_TRAJANJE_S   300
#define RANJEN_CEKANJE_MS   10000
#define RANJEN_POGODAKA     5
#define RANJEN_OSNOVA       100
#define RANJEN_PO_KM        60
#define RANJEN_PO_STETI     1.5
#define RANJEN_DOMET_NPC    0.8
#define RANJEN_PRAG         20.0
#define RANJEN_IZLAZ_MS     1500
#define RANJEN_NPC          -1
#define RANJEN_MEDIK_DOMET  5.0
#define SLUCAJ_BRZE         1.3
#define SLUCAJ_RUB_DOMET    30.0
#define SLUCAJ_RUB_SIRINA   5.0
#define SLUCAJ_RUB_MS       6000
#define RANJEN_GRUPA        40.0
#define RANJEN_POMAK        1.0
#define SLUCAJ_VATRA_DUBINA 1.6
#define RANJEN_POPRAVAK_MS  1000
#define KAMION_RESPAWN      120
#define KAMION_PARKING      15.0
#define SLUCAJ_CEKA_MS      2500
#define SLUCAJ_PROGURAJ_MS  8000
#define SLUCAJ_USPUT        40.0
#define SLUCAJ_PJESKE_DOMET 40.0

enum { SL_NEMA, SL_POZIV, SL_IGRAC, SL_NPC, SL_IZLAZI, SL_PJESKE, SL_PRISTANAK, SL_RADI, SL_GASI, SL_ODLAZI, SL_POVRATAK, SL_ULAZI, SL_KRAJ }
enum E_SLUCAJ { slStanje, slTip, slPacijent, slPoziv, STREAMER_TAG_OBJECT:slObjekt, slRok, slGasenje, slIgrac, slNpc, slVozilo, slIdx, slBroj, slKamion, slCeka, slProguraj, slSporo, bool:slObilazi, bool:slPjeske, slNastavi, bool:slNastavak, Float:slX, Float:slY, Float:slZ, slPosao }

static gSlucaj[SLUCAJ_MAX][E_SLUCAJ];
static gSlucajPoziv[MAX_PLAYERS], gMehanicar[MAX_PLAYERS], gMehanicarVozilo[MAX_PLAYERS];

enum E_KAMION { bool:kIma, kTip, kDb, kVozilo, kSlucaj, Float:kX, Float:kY, Float:kZ, Float:kA }

static gKamion[KAMION_MAX][E_KAMION];
static gApKamionDb[MAX_PLAYERS], gApKamionTip[MAX_PLAYERS];

enum { RN_NE, RN_CEKA, RN_DOLJE }

static const Float:gBolnice[][4] = { { 1172.08, -1323.35, 15.40, 270.0 }, { 2034.20, -1401.67, 17.30, 180.0 } };
static bool:gRanjenBolnica[MAX_PLAYERS];
static gRanjenPonuda[MAX_PLAYERS], gRanjen[MAX_PLAYERS], gRanjenOd[MAX_PLAYERS], gRanjenPogoci[MAX_PLAYERS], gRanjenInt[MAX_PLAYERS], gRanjenSvijet[MAX_PLAYERS], gRanjenAnimId[MAX_PLAYERS], gRanjenPopravak[MAX_PLAYERS], Float:gRanjenPos[MAX_PLAYERS][4];

static PromijeniScore(org, delta)
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT OR IGNORE INTO org_score (org, score) VALUES (%d, 0)", org));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "UPDATE org_score SET score = score + %d WHERE org = %d", delta, org));
}

static GetOrgScore(org)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT score FROM org_score WHERE org = %d", org);
    new score = DB_GetRowCount(res) ? DB_GetFieldInt(res, 0) : 0;
    DB_FreeResultSet(res);
    return score;
}

InitSluzbe()
{
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS org_score (org INTEGER PRIMARY KEY, score INTEGER NOT NULL)"));
    PromijeniScore(-1, 1);
    PromijeniScore(-1, -2);
    assert(GetOrgScore(-1) == -1);
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM org_score WHERE org = -1"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS fire_trucks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS ambulances (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "CREATE TABLE IF NOT EXISTS tow_trucks (id INTEGER PRIMARY KEY AUTOINCREMENT, x REAL NOT NULL, y REAL NOT NULL, z REAL NOT NULL, a REAL NOT NULL)"));
    new k = 0;
    KamionUcitaj(ORG_VATROGASCI, k);
    KamionUcitaj(ORG_MEDICINSKA, k);
    KamionUcitaj(ORG_MEHANICAR, k);
}

static KamionUcitaj(tip, &k)
{
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id, x, y, z, a FROM %s ORDER BY id LIMIT %d", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), KAMION_PO_TIPU);
    for (new i = DB_GetRowCount(res); i > 0; i--)
    {
        gKamion[k][kDb] = DB_GetFieldInt(res, 0);
        if (KamionStvori(k, tip, DB_GetFieldFloat(res, 1), DB_GetFieldFloat(res, 2), DB_GetFieldFloat(res, 3), DB_GetFieldFloat(res, 4))) k++;
        DB_SelectNextRow(res);
    }
    DB_FreeResultSet(res);
}

static KamionStvori(k, tip, Float:x, Float:y, Float:z, Float:a)
{
    new boja = tip == ORG_VATROGASCI ? KAMION_BOJA : tip == ORG_MEDICINSKA ? AMBULANTA_BOJA : MEHANICAR_BOJA;
    new v = CreateVehicle(tip == ORG_VATROGASCI ? KAMION_MODEL : tip == ORG_MEDICINSKA ? AMBULANTA_MODEL : MEHANICAR_MODEL, x, y, z, a, boja, boja, KAMION_RESPAWN);
    if (v == INVALID_VEHICLE_ID) return 0;
    gKamion[k][kIma] = true;
    gKamion[k][kTip] = tip;
    gKamion[k][kVozilo] = v;
    gKamion[k][kSlucaj] = 0;
    gKamion[k][kX] = x;
    gKamion[k][kY] = y;
    gKamion[k][kZ] = z;
    gKamion[k][kA] = a;
    return 1;
}

static KamionBroj(tip)
{
    new n = 0;
    for (new k = 0; k < KAMION_MAX; k++) if (gKamion[k][kIma] && gKamion[k][kTip] == tip) n++;
    return n;
}

ShowKamione(playerid, tip)
{
    new list[512], row[64], title[64], len = 0;
    gApKamionTip[playerid] = tip;
    DodajStupac(list, len, T(playerid, "{8AB4F8}+ Novo vozilo na mojoj poziciji"));
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kTip] != tip) continue;
        format(row, sizeof row, "[%d] %s", gKamion[k][kDb], CestaZona(gKamion[k][kX], gKamion[k][kY]));
        DodajStupac(list, len, row);
    }
    format(title, sizeof title, tip == ORG_VATROGASCI ? T(playerid, "Vatrogasni kamioni (%d/%d)") : tip == ORG_MEDICINSKA ? T(playerid, "Ambulante (%d/%d)") : T(playerid, "Mehanicarska vozila (%d/%d)"), KamionBroj(tip), KAMION_PO_TIPU);
    return PrikaziDialog(playerid, DLG_APANEL_KAMION, DIALOG_STYLE_LIST, title, list, T(playerid, "Odaberi"), T(playerid, "Natrag"));
}

static KamionNovi(playerid, tip)
{
    new Float:x, Float:y, Float:z, Float:a, Float:d, k = 0;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, a);
    CestaNajbliza(x, y, z, d);
    while (k < KAMION_MAX && gKamion[k][kIma]) k++;
    if (k == KAMION_MAX || KamionBroj(tip) >= KAMION_PO_TIPU || GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || d > TAKSI_MAX_OD_CESTE || !KamionStvori(k, tip, x, y, z + 0.5, a))
        return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo se ovdje ne moze stvoriti."));
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "INSERT INTO %s (x, y, z, a) VALUES (%f, %f, %f, %f)", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), x, y, z + 0.5, a));
    gKamion[k][kDb] = ZadnjiId();
    return SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo dodano: %s."), CestaZona(x, y));
}

DLG:apanel_kamion(playerid, response, listitem)
{
    if (!response) return ShowKategoriju(playerid);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new tip = gApKamionTip[playerid];
    if (listitem == 0)
    {
        KamionNovi(playerid, tip);
        return ShowKamione(playerid, tip);
    }
    new n = listitem;
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kTip] != tip || --n) continue;
        new body[96];
        gApKamionDb[playerid] = gKamion[k][kDb];
        format(body, sizeof body, T(playerid, "Obrisati vozilo: %s?"), CestaZona(gKamion[k][kX], gKamion[k][kY]));
        return PrikaziDialog(playerid, DLG_APANEL_KAMION_BRISI, DIALOG_STYLE_MSGBOX, tip == ORG_VATROGASCI ? T(playerid, "Vatrogasni kamioni") : tip == ORG_MEDICINSKA ? T(playerid, "Ambulante") : T(playerid, "Mehanicarska vozila"), body, T(playerid, "Obrisi"), T(playerid, "Natrag"));
    }
    return ShowKamione(playerid, tip);
}

DLG:apanel_kamion_brisi(playerid, response)
{
    new tip = gApKamionTip[playerid];
    if (!response) return ShowKamione(playerid, tip);
    if (!ProvjeriOvlasti(playerid, ALVL_ORG)) return 1;
    new k = 0;
    while (k < KAMION_MAX && (!gKamion[k][kIma] || gKamion[k][kTip] != tip || gKamion[k][kDb] != gApKamionDb[playerid])) k++;
    if (k == KAMION_MAX) return ShowKamione(playerid, tip);
    if (gKamion[k][kSlucaj])
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vozilo je trenutno na intervenciji, pokusajte kasnije."));
        return ShowKamione(playerid, tip);
    }
    DestroyVehicle(gKamion[k][kVozilo]);
    gKamion[k][kIma] = false;
    DB_FreeResultSet(DB_ExecuteQuery(gDB, "DELETE FROM %s WHERE id = %d", tip == ORG_VATROGASCI ? ("fire_trucks") : tip == ORG_MEDICINSKA ? ("ambulances") : ("tow_trucks"), gApKamionDb[playerid]));
    SendClientMessage(playerid, COLOR_ADMIN, T(playerid, "Vozilo obrisano."));
    return ShowKamione(playerid, tip);
}

SlucajNaslov(playerid, title[], size)
{
    new tip = GetOrgTip(gOrg[playerid]);
    if (tip != ORG_VATROGASCI && tip != ORG_MEDICINSKA) return;
    new row[32];
    format(row, sizeof row, T(playerid, " | Rezultat: %d"), GetOrgScore(gOrg[playerid]));
    strcat(title, row, size);
}

static bool:JeUSluzbi(playerid, tip)
{
    if (!gUlogiran[playerid] || !gOrg[playerid] || !NaDuznosti(playerid)) return false;
    new org = GetOrgTip(gOrg[playerid]);
    if (tip == ORG_MEHANICAR) return org == ORG_MEHANICAR;
    return (tip == ORG_VATROGASCI || tip == ORG_MEDICINSKA) && org == ORG_VATROGASCI;
}

static bool:SlucajImaIgraca(tip, osim)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (i != osim && JeUSluzbi(i, tip)) return true;
    return false;
}

static SlucajOcistiPozive(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gSlucajPoziv[i] == s + 1) gSlucajPoziv[i] = 0;
}

static bool:SlucajImaPoziva(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (gSlucajPoziv[i] == s + 1) return true;
    return false;
}

static SlucajMakniKamion(s, bool:vrati = true)
{
    new lik = gSlucaj[s][slNpc], k = gSlucaj[s][slKamion] - 1;
    if (lik != INVALID_LIK)
    {
        Lik_Ukloni(lik);
        gSlucaj[s][slNpc] = INVALID_LIK;
    }
    if (k >= 0)
    {
        gKamion[k][kSlucaj] = 0;
        if (vrati) SetVehicleToRespawn(gKamion[k][kVozilo]);
    }
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slVozilo] = 0;
}

static SlucajLika(lik)
{
    if (lik == INVALID_LIK) return -1;
    for (new s = 0; s < SLUCAJ_MAX; s++) if (gSlucaj[s][slStanje] && gSlucaj[s][slNpc] == lik) return s;
    return -1;
}

static SlucajHodaj(lik, Float:x, Float:y, Float:z, Float:domet, nacin = LIK_TRK)
{
    new Float:lx, Float:ly, Float:lz;
    Lik_Pozicija(lik, lx, ly, lz);
    new Float:dx = lx - x, Float:dy = ly - y, Float:d = VectorSize(dx, dy, 0.0);
    if (d > domet && d > 0.01)
    {
        x += dx / d * domet;
        y += dy / d * domet;
    }
    Lik_IdiDo(lik, x, y, HodTlo(x, y, z), nacin);
}

static SlucajVozi(s)
{
    new e, l, a, d, bn, bt, o, v = gSlucaj[s][slVozilo], bool:hitno = gSlucaj[s][slStanje] == SL_NPC;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(v, 1, 1, a, d, bn, bt, o);
    if (hitno) Lik_Vozi(gSlucaj[s][slNpc], gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ], 17.0, true);
    else
    {
        new k = gSlucaj[s][slKamion] - 1;
        if (k < 0) return;
        Lik_Vozi(gSlucaj[s][slNpc], gKamion[k][kX], gKamion[k][kY], gKamion[k][kZ], 13.0, false);
    }
}

static bool:KamionSlobodan(k)
{
    new Float:x, Float:y, Float:z;
    if (!gKamion[k][kIma] || gKamion[k][kSlucaj]) return false;
    GetVehiclePos(gKamion[k][kVozilo], x, y, z);
    if (VectorSize(x - gKamion[k][kX], y - gKamion[k][kY], z - gKamion[k][kZ]) > KAMION_PARKING) return false;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerInVehicle(i, gKamion[k][kVozilo]) && GetPlayerState(i) == PLAYER_STATE_DRIVER) return false;
    return true;
}

static SlucajNajbliziKamion(s)
{
    new best = -1, Float:bd = 1.0e12;
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (gKamion[k][kTip] != gSlucaj[s][slTip] || !KamionSlobodan(k)) continue;
        new Float:d = VectorSize(gKamion[k][kX] - gSlucaj[s][slX], gKamion[k][kY] - gSlucaj[s][slY], 0.0);
        if (d < bd) { bd = d; best = k; }
    }
    return best;
}

static SlucajGrupa(s)
{
    if (gSlucaj[s][slTip] != ORG_MEDICINSKA) return 0;
    for (new i = 0; i < SLUCAJ_MAX; i++)
    {
        new st = gSlucaj[i][slStanje];
        if (i != s && st >= SL_NPC && st < SL_ODLAZI && gSlucaj[i][slTip] == ORG_MEDICINSKA && gSlucaj[i][slVozilo] && VectorSize(gSlucaj[i][slX] - gSlucaj[s][slX], gSlucaj[i][slY] - gSlucaj[s][slY], 0.0) <= RANJEN_GRUPA) return 1;
    }
    return 0;
}

static SlucajPredaj(s, n, ostaje)
{
    new lik = gSlucaj[s][slNpc], k = gSlucaj[s][slKamion];
    SlucajOcistiPozive(n);
    gSlucaj[n][slNpc] = lik;
    gSlucaj[n][slVozilo] = gSlucaj[s][slVozilo];
    gSlucaj[n][slKamion] = k;
    gSlucaj[n][slPjeske] = gSlucaj[s][slPjeske];
    gKamion[k - 1][kSlucaj] = n + 1;
    gSlucaj[s][slNpc] = INVALID_LIK;
    gSlucaj[s][slVozilo] = 0;
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slPjeske] = false;
    gSlucaj[s][slStanje] = ostaje;
}

static bool:SlucajPutDo(s, v)
{
    return gSlucaj[s][slStanje] != SL_NEMA && GetVehicleModel(v) != 0;
}

static SlucajNaPut(s)
{
    Lik_TipkeVozila(gSlucaj[s][slNpc], 0);
    gSlucaj[s][slStanje] = SL_NPC;
    SlucajVozi(s);
}

static bool:SlucajUsput(s)
{
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gSlucaj[s][slVozilo], x, y, z);
    new Float:doCilja = VectorSize(gSlucaj[s][slX] - x, gSlucaj[s][slY] - y, 0.0);
    for (new n = 0; n < SLUCAJ_MAX; n++)
    {
        if (n == s || gSlucaj[n][slStanje] != SL_POZIV || gSlucaj[n][slVozilo] || gSlucaj[n][slTip] != gSlucaj[s][slTip]) continue;
        new Float:d = VectorSize(gSlucaj[n][slX] - x, gSlucaj[n][slY] - y, 0.0);
        if (d > SLUCAJ_USPUT || d >= doCilja || !SlucajPutDo(n, gSlucaj[s][slVozilo])) continue;
        SlucajPredaj(s, n, SL_POZIV);
        gSlucaj[n][slNastavi] = s + 1;
        SlucajNaPut(n);
        return true;
    }
    return false;
}

static SlucajNajbliziPovratak(s, &Float:najblize)
{
    new best = -1, Float:x, Float:y, Float:z;
    najblize = 1.0e12;
    for (new i = 0; i < SLUCAJ_MAX; i++)
    {
        if (i == s || gSlucaj[i][slStanje] != SL_ODLAZI || gSlucaj[i][slTip] != gSlucaj[s][slTip] || !gSlucaj[i][slVozilo] || gSlucaj[i][slPjeske]) continue;
        GetVehiclePos(gSlucaj[i][slVozilo], x, y, z);
        new Float:d = VectorSize(x - gSlucaj[s][slX], y - gSlucaj[s][slY], 0.0);
        if (d < najblize)
        {
            najblize = d;
            best = i;
        }
    }
    return best;
}

static SlucajPosaljiKamion(s)
{
    SlucajOcistiPozive(s);
    for (new i = 0; i < SLUCAJ_MAX; i++) if (gSlucaj[i][slNastavi] == s + 1 && gSlucaj[i][slVozilo]) return;
    if (SlucajGrupa(s)) return;
    new k = SlucajNajbliziKamion(s), Float:dPovratak, r = SlucajNajbliziPovratak(s, dPovratak);
    if (r != -1 && (k == -1 || dPovratak < VectorSize(gKamion[k][kX] - gSlucaj[s][slX], gKamion[k][kY] - gSlucaj[s][slY], 0.0)) && SlucajPutDo(s, gSlucaj[r][slVozilo]))
    {
        SlucajPredaj(r, s, SL_NEMA);
        SlucajNaPut(s);
        return;
    }
    if (k == -1) return;
    new v = gKamion[k][kVozilo], Float:x, Float:y, Float:z, Float:a;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    new lik = Lik_Stvori(gSlucaj[s][slTip] == ORG_VATROGASCI ? KAMION_SKIN : gSlucaj[s][slTip] == ORG_MEDICINSKA ? AMBULANTA_SKIN : MEHANICAR_SKIN, x, y, z, a);
    if (lik == INVALID_LIK) return;
    Lik_Neranjiv(lik, true);
    Lik_UVozilo(lik, v, 0);
    gSlucaj[s][slStanje] = SL_NPC;
    gSlucaj[s][slNpc] = lik;
    gSlucaj[s][slVozilo] = v;
    gSlucaj[s][slKamion] = k + 1;
    gKamion[k][kSlucaj] = s + 1;
    SetTimerEx("SlucajKreni", 1000, false, "i", s);
}

forward SlucajKreni(s);
public SlucajKreni(s)
{
    if (gSlucaj[s][slStanje] == SL_NPC && gSlucaj[s][slVozilo]) SlucajVozi(s);
}

SlucajNpcStigao(lik)
{
    new s = SlucajLika(lik);
    if (s < 0) return 0;
    new st = gSlucaj[s][slStanje], bool:vatra = gSlucaj[s][slTip] == ORG_VATROGASCI;
    if (st == SL_RADI)
    {
        MehanicarAnim(s);
        return 1;
    }
    if (st == SL_PJESKE)
    {
        if (gSlucaj[s][slTip] == ORG_MEHANICAR)
        {
            gSlucaj[s][slStanje] = SL_PRISTANAK;
            gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
            MehanicarPonudi(gSlucaj[s][slPacijent], s);
            return 1;
        }
        if (!vatra)
        {
            if (KupacJeRanjen(gSlucaj[s][slPacijent]))
            {
                gSlucaj[s][slStanje] = SL_GASI;
                Lik_Animacija(lik, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 0, 0);
                return 1;
            }
            gSlucaj[s][slStanje] = SL_PRISTANAK;
            RanjenPonudi(gSlucaj[s][slPacijent], RANJEN_NPC);
            return 1;
        }
        gSlucaj[s][slStanje] = SL_GASI;
        Lik_NaciljaTocku(lik, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ] - CESTA_Z_VOZILA, true);
        return 1;
    }
    if (st == SL_ODLAZI)
    {
        gSlucaj[s][slStanje] = SL_KRAJ;
        return 1;
    }
    if (st != SL_NPC) return 1;
    new Float:x, Float:y, Float:z;
    GetVehiclePos(gSlucaj[s][slVozilo], x, y, z);
    if (vatra && VectorSize(gSlucaj[s][slX] - x, gSlucaj[s][slY] - y, 0.0) <= SLUCAJ_DOMET_CIJEV)
    {
        gSlucaj[s][slStanje] = SL_GASI;
        Lik_TipkeVozila(lik, _:KEY_FIRE);
        return 1;
    }
    gSlucaj[s][slStanje] = SL_IZLAZI;
    gSlucaj[s][slPjeske] = true;
    new e, l, al, d, bn, bt, o;
    GetVehicleParamsEx(gSlucaj[s][slVozilo], e, l, al, d, bn, bt, o);
    SetVehicleParamsEx(gSlucaj[s][slVozilo], e, 0, al, d, bn, bt, o);
    Lik_Izadji(lik);
    SetTimerEx("SlucajPjeske", SLUCAJ_IZLAZAK_MS, false, "i", s);
    return 1;
}

forward SlucajPjeske(s);
public SlucajPjeske(s)
{
    if (gSlucaj[s][slStanje] != SL_IZLAZI) return;
    new lik = gSlucaj[s][slNpc], bool:vatra = gSlucaj[s][slTip] == ORG_VATROGASCI;
    if (Lik_DajVozilo(lik) != -1) Lik_Izadji(lik, true);
    if (vatra) Lik_Oruzje(lik, _:WEAPON_FIREEXTINGUISHER, SLUCAJ_MUNICIJA);
    gSlucaj[s][slStanje] = SL_PJESKE;
    SlucajHodaj(lik, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ], vatra ? SLUCAJ_DOMET_NPC : gSlucaj[s][slTip] == ORG_MEHANICAR ? MEHANICAR_DOMET : RANJEN_DOMET_NPC);
}

static SlucajUKamion(s)
{
    new lik = gSlucaj[s][slNpc];
    Lik_Stani(lik);
    Lik_PrestaniCiljati(lik);
    Lik_OcistiAnimaciju(lik);
    Lik_Oruzje(lik, 0, 0);
    Lik_Udji(lik, gSlucaj[s][slVozilo], 0, LIK_TRK);
    gSlucaj[s][slStanje] = SL_POVRATAK;
    gSlucaj[s][slRok] = SLUCAJ_POVRATAK_S;
}

static SlucajVratiKamion(s)
{
    new k = gSlucaj[s][slKamion] - 1;
    Lik_TipkeVozila(gSlucaj[s][slNpc], 0);
    if (k < 0)
    {
        gSlucaj[s][slStanje] = SL_KRAJ;
        return;
    }
    gSlucaj[s][slStanje] = SL_ODLAZI;
    gSlucaj[s][slRok] = SLUCAJ_ODLAZAK_S;
    SlucajVozi(s);
}

static bool:SlucajSljedeci(s)
{
    new n = gSlucaj[s][slNastavi] - 1;
    gSlucaj[s][slNastavi] = 0;
    new bool:nastavak = n >= 0 && gSlucaj[n][slStanje] == SL_POZIV && !gSlucaj[n][slVozilo];
    if (!nastavak)
    {
        if (gSlucaj[s][slTip] != ORG_MEDICINSKA) return false;
        n = -1;
        new Float:bd = RANJEN_GRUPA;
        for (new i = 0; i < SLUCAJ_MAX; i++)
        {
            if (i == s || gSlucaj[i][slStanje] != SL_POZIV || gSlucaj[i][slTip] != ORG_MEDICINSKA || gSlucaj[i][slVozilo]) continue;
            new Float:d = VectorSize(gSlucaj[i][slX] - gSlucaj[s][slX], gSlucaj[i][slY] - gSlucaj[s][slY], 0.0);
            if (d <= bd) { bd = d; n = i; }
        }
        if (n == -1) return false;
    }
    new st = gSlucaj[s][slStanje], lik = gSlucaj[s][slNpc], bool:pjeske = gSlucaj[s][slPjeske];
    if (!pjeske && !SlucajPutDo(n, gSlucaj[s][slVozilo])) return false;
    SlucajPredaj(s, n, SL_NEMA);
    if (!pjeske)
    {
        SlucajNaPut(n);
        return true;
    }
    if (nastavak)
    {
        gSlucaj[n][slNastavak] = true;
        SlucajUKamion(n);
        return true;
    }
    if (st == SL_IZLAZI)
    {
        gSlucaj[n][slStanje] = SL_IZLAZI;
        SetTimerEx("SlucajPjeske", SLUCAJ_IZLAZAK_MS, false, "i", n);
        return true;
    }
    Lik_Stani(lik);
    Lik_OcistiAnimaciju(lik);
    gSlucaj[n][slStanje] = SL_PJESKE;
    SlucajHodaj(lik, gSlucaj[n][slX], gSlucaj[n][slY], gSlucaj[n][slZ], RANJEN_DOMET_NPC);
    return true;
}

static SlucajZatvori(s)
{
    SlucajOcistiPozive(s);
    if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) DisablePlayerCheckpoint(gSlucaj[s][slIgrac]);
    gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
    gSlucaj[s][slPacijent] = INVALID_PLAYER_ID;
    if (!gSlucaj[s][slVozilo]) gSlucaj[s][slStanje] = SL_NEMA;
    else if (SlucajSljedeci(s)) return;
    else if (gSlucaj[s][slPjeske]) SlucajUKamion(s);
    else SlucajVratiKamion(s);
}

static SlucajKraj(s, bool:ok, ko = INVALID_PLAYER_ID)
{
    new tip = gSlucaj[s][slTip], p = gSlucaj[s][slPacijent];
    if (tip == ORG_MEHANICAR)
    {
        if (p != INVALID_PLAYER_ID)
        {
            if (gMehanicar[p] == s + 1) gMehanicar[p] = 0;
            TuningMehanicarZatvori(p);
            if (!ok) SendClientMessage(p, COLOR_ERROR, T(p, "Mehanicar je otisao."));
        }
        SlucajZatvori(s);
        return;
    }
    if (tip == ORG_VATROGASCI) DestroyDynamicObject(gSlucaj[s][slObjekt]);
    else if (KupacJeRanjen(p)) KupacOzivi(p, ok);
    else RanjenPrimijeni(p, ok);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT id FROM orgs WHERE type = %d", tip);
    if (DB_GetRowCount(res)) do PromijeniScore(DB_GetFieldInt(res, 0), ok ? 1 : -1); while (DB_SelectNextRow(res));
    DB_FreeResultSet(res);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!JeUSluzbi(i, tip)) continue;
        new score = GetOrgScore(gOrg[i]);
        if (tip == ORG_VATROGASCI)
        {
            if (!ok) SendClientMessage(i, COLOR_ORG, T(i, "Pozar nije ugasen na vrijeme. Rezultat vatrogasaca: %d."), score);
            else if (ko == INVALID_PLAYER_ID) SendClientMessage(i, COLOR_ORG, T(i, "NPC vatrogasci su ugasili pozar. Rezultat vatrogasaca: %d."), score);
            else SendClientMessage(i, COLOR_ORG, T(i, "%s je ugasio pozar. Rezultat vatrogasaca: %d."), GetRPIme(ko), score);
        }
        else if (!ok) SendClientMessage(i, COLOR_ORG, T(i, "%s nije spasen. Rezultat hitne: %d."), GetRPIme(p), score);
        else if (ko == INVALID_PLAYER_ID) SendClientMessage(i, COLOR_ORG, T(i, "NPC bolnicari su spasili %s. Rezultat hitne: %d."), GetRPIme(p), score);
        else SendClientMessage(i, COLOR_ORG, T(i, "%s je spasio %s. Rezultat hitne: %d."), GetRPIme(ko), GetRPIme(p), score);
    }
    SlucajZatvori(s);
}

static bool:SlucajPrska(i, s)
{
    new KEY:k, ud, lr, Float:domet = SLUCAJ_DOMET_APARAT;
    GetPlayerKeys(i, k, ud, lr);
    if (GetPlayerState(i) == PLAYER_STATE_DRIVER)
    {
        if (!(k & (KEY_FIRE | KEY_ACTION)) || GetVehicleModel(GetPlayerVehicleID(i)) != KAMION_MODEL) return false;
        domet = SLUCAJ_DOMET_CIJEV;
    }
    else if (GetPlayerState(i) != PLAYER_STATE_ONFOOT || !(k & KEY_FIRE) || GetPlayerWeapon(i) != WEAPON_FIREEXTINGUISHER) return false;
    return IsPlayerInRangeOfPoint(i, domet, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ]) && JeUSluzbi(i, ORG_VATROGASCI);
}

static SlucajKoPrska(s)
{
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerConnected(i) && SlucajPrska(i, s)) return i;
    return INVALID_PLAYER_ID;
}

SlucajTick()
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        new st = gSlucaj[s][slStanje];
        if (st == SL_NEMA) continue;
        if (st == SL_KRAJ)
        {
            SlucajMakniKamion(s);
            gSlucaj[s][slStanje] = SL_NEMA;
            continue;
        }
        if (st == SL_POVRATAK || st == SL_ULAZI)
        {
            new lik = gSlucaj[s][slNpc], v = gSlucaj[s][slVozilo];
            if (Lik_DajVozilo(lik) == v)
            {
                gSlucaj[s][slPjeske] = false;
                if (!gSlucaj[s][slNastavak]) SlucajVratiKamion(s);
                else
                {
                    gSlucaj[s][slNastavak] = false;
                    if (SlucajPutDo(s, v)) SlucajNaPut(s);
                    else
                    {
                        SlucajMakniKamion(s);
                        gSlucaj[s][slStanje] = SL_POZIV;
                    }
                }
            }
            else if (--gSlucaj[s][slRok] <= 0)
            {
                Lik_Stani(lik);
                Lik_UVozilo(lik, v, 0);
                gSlucaj[s][slRok] = SLUCAJ_POVRATAK_S;
            }
            continue;
        }
        if (--gSlucaj[s][slRok] <= 0)
        {
            if (st == SL_ODLAZI) gSlucaj[s][slStanje] = SL_KRAJ;
            else SlucajKraj(s, false);
            continue;
        }
        if (st == SL_ODLAZI) continue;
        if (st == SL_IGRAC && gSlucaj[s][slTip] == ORG_MEHANICAR)
        {
            MehanicarIgracStigao(s);
            continue;
        }
        if (st == SL_NPC && SlucajUsput(s)) continue;
        if (st == SL_POZIV && (--gSlucaj[s][slPoziv] <= 0 || !SlucajImaPoziva(s))) SlucajPosaljiKamion(s);
        new ko = gSlucaj[s][slTip] == ORG_VATROGASCI ? SlucajKoPrska(s) : INVALID_PLAYER_ID;
        if ((st == SL_GASI || ko != INVALID_PLAYER_ID) && ++gSlucaj[s][slGasenje] >= SLUCAJ_GASENJE_S) SlucajKraj(s, true, ko);
    }
}

HitnaZaNpc(npcid, Float:x, Float:y, Float:z)
{
    return SlucajNovi(ORG_MEDICINSKA, x, y, z, npcid);
}

HitnaNpcIzlijecen(npcid, medic)
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        if (!gSlucaj[s][slStanje] || gSlucaj[s][slStanje] >= SL_ODLAZI || gSlucaj[s][slTip] != ORG_MEDICINSKA || gSlucaj[s][slPacijent] != npcid) continue;
        SlucajKraj(s, true, medic);
        return;
    }
    KupacOzivi(npcid, true);
}

static SlucajNovi(tip, Float:x, Float:y, Float:z, pacijent = INVALID_PLAYER_ID)
{
    new s = -1, bool:vatra = tip == ORG_VATROGASCI;
    for (new i = SLUCAJ_MAX - 1; i >= 0; i--)
    {
        new st = gSlucaj[i][slStanje];
        if (st == SL_NEMA) s = i;
        else if (vatra && gSlucaj[i][slTip] == tip && st < SL_ODLAZI && VectorSize(gSlucaj[i][slX] - x, gSlucaj[i][slY] - y, 0.0) < SLUCAJ_RAZMAK) return -1;
    }
    if (s == -1) return -1;
    gSlucaj[s][slStanje] = SL_POZIV;
    gSlucaj[s][slTip] = tip;
    gSlucaj[s][slPacijent] = pacijent;
    gSlucaj[s][slPoziv] = SLUCAJ_POZIV_S;
    if (vatra)
    {
        new Float:hx, Float:hy, Float:tlo = z - CESTA_Z_VOZILA;
        Kolizija_Zraka(x, y, z, x, y, z - 20.0, hx, hy, tlo);
        gSlucaj[s][slObjekt] = CreateDynamicObject(SLUCAJ_OBJEKT, x, y, tlo - SLUCAJ_VATRA_DUBINA, 0.0, 0.0, 0.0, 0);
    }
    gSlucaj[s][slRok] = vatra ? SLUCAJ_TRAJANJE_S : RANJEN_TRAJANJE_S;
    gSlucaj[s][slGasenje] = 0;
    gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
    gSlucaj[s][slNpc] = INVALID_LIK;
    gSlucaj[s][slVozilo] = 0;
    gSlucaj[s][slKamion] = 0;
    gSlucaj[s][slPjeske] = false;
    gSlucaj[s][slNastavi] = 0;
    gSlucaj[s][slNastavak] = false;
    gSlucaj[s][slPosao] = 0;
    gSlucaj[s][slX] = x;
    gSlucaj[s][slY] = y;
    gSlucaj[s][slZ] = z;
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++)
    {
        if (!JeUSluzbi(i, tip) || i == pacijent) continue;
        new body[128], Float:px, Float:py, Float:pz, d;
        GetPlayerPos(i, px, py, pz);
        d = floatround(VectorSize(px - x, py - y, 0.0));
        format(body, sizeof body, vatra ? T(i, "Gori vozilo %d m od vas. Prihvacate li poziv?") : tip == ORG_MEHANICAR ? T(i, "Vozilo u kvaru %d m od vas. Prihvacate li poziv?") : T(i, "Ranjena osoba %d m od vas. Prihvacate li poziv?"), d);
        gSlucajPoziv[i] = s + 1;
        PrikaziDialog(i, DLG_SLUCAJ, DIALOG_STYLE_MSGBOX, vatra ? T(i, "Poziv vatrogascima") : tip == ORG_MEHANICAR ? T(i, "Poziv mehanicaru") : T(i, "Poziv hitnoj"), body, T(i, "Prihvati"), T(i, "Odbij"));
    }
    return s;
}

SlucajEksplozija(v)
{
    new Float:x, Float:y, Float:z, bool:ima = GetVehiclePos(v, x, y, z) && !GetVehicleVirtualWorld(v) && z > 1.0;
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        if (!gSlucaj[s][slStanje] || gSlucaj[s][slVozilo] != v) continue;
        SlucajMakniKamion(s, false);
        gSlucaj[s][slPjeske] = false;
        gSlucaj[s][slStanje] = gSlucaj[s][slStanje] >= SL_ODLAZI ? SL_NEMA : SL_POZIV;
    }
    if (ima) SlucajNovi(ORG_VATROGASCI, x, y, z);
}

JeKamion(v)
{
    for (new k = 0; k < KAMION_MAX; k++) if (gKamion[k][kIma] && gKamion[k][kVozilo] == v) return 1;
    return 0;
}

SlucajStateChange(playerid, PLAYER_STATE:newstate)
{
    if (newstate != PLAYER_STATE_DRIVER || IsPlayerNPC(playerid)) return;
    new v = GetPlayerVehicleID(playerid);
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kVozilo] != v) continue;
        if (JeUSluzbi(playerid, gKamion[k][kTip])) return;
        RemovePlayerFromVehicle(playerid);
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Samo sluzba moze voziti ovo vozilo."));
        return;
    }
}

SlucajDisconnect(playerid)
{
    RanjenOcistiRane(playerid);
    gSlucajPoziv[playerid] = 0;
    gMehanicar[playerid] = 0;
    gRanjen[playerid] = RN_NE;
    gRanjenPonuda[playerid] = 0;
    for (new s = 0; s < SLUCAJ_MAX; s++)
    {
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && (gSlucaj[s][slTip] == ORG_MEDICINSKA || gSlucaj[s][slTip] == ORG_MEHANICAR) && gSlucaj[s][slPacijent] == playerid)
        {
            SlucajZatvori(s);
            continue;
        }
        if (gSlucaj[s][slStanje] != SL_IGRAC || gSlucaj[s][slIgrac] != playerid) continue;
        gSlucaj[s][slIgrac] = INVALID_PLAYER_ID;
        gSlucaj[s][slStanje] = SL_POZIV;
    }
}

ShowMehanicare(playerid)
{
    return ShowKamione(playerid, ORG_MEHANICAR);
}

static gOdvozVozila[MAX_PLAYERS][MEHANICAR_ODVOZ_VOZILA], gOdvozBroj[MAX_PLAYERS], bool:gOdvoz[MAX_VEHICLES];

static MehanicarOdvozPoziv(playerid)
{
    gOdvozBroj[playerid] = VlasnistvaIgraca(playerid, gOdvozVozila[playerid]);
    if (!gOdvozBroj[playerid]) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate vlastitih vozila."));
    SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): Mehanicarski servis, izvolite. Koje vozilo trebate odslepati?"));
    MobitelOdvoz(playerid);
    return 1;
}

static MehanicarOdvozGreska(playerid, const tekst[])
{
    SendClientMessage(playerid, COLOR_ERROR, T(playerid, tekst));
    return 1;
}

MehanicarOdvozBroj(playerid)
{
    return gOdvozBroj[playerid];
}

MehanicarOdvozRed(playerid, i, red[], len)
{
    red[0] = EOS;
    if (i < 0 || i >= gOdvozBroj[playerid] || !GetVehicleModel(gOdvozVozila[playerid][i])) return;
    strcat(red, gImenaVozila[GetVehicleModel(gOdvozVozila[playerid][i]) - 400], len);
}

MehanicarOdvozOdaberi(playerid, listitem)
{
    if (listitem < 0 || listitem >= gOdvozBroj[playerid]) return 1;
    new v = gOdvozVozila[playerid][listitem], bool:zauzeto = false;
    if (!GetVehicleModel(v) || VlasnikVozila(v) != gAccountId[playerid]) return MehanicarOdvozGreska(playerid, "Vozilo vise nije vase.");
    if (gOdvoz[v]) return MehanicarOdvozGreska(playerid, "To vozilo se vec odvozi.");
    for (new i = 0; i < MAX_PLAYERS && !zauzeto; i++) if (IsPlayerInVehicle(i, v)) zauzeto = true;
    if (zauzeto && !gPremium[playerid]) return MehanicarOdvozGreska(playerid, "Netko je u vozilu, ne mozemo ga odslepati.");
    new Float:x, Float:y, Float:z;
    GetVehiclePos(v, x, y, z);
    if (VlasnikNaParkingu(v)) return MehanicarOdvozGreska(playerid, "Vozilo je vec na parkingu.");
    if (!DajNovac(playerid, -MEHANICAR_ODVOZ)) return MehanicarOdvozGreska(playerid, "Nemate dovoljno novca.");
    new model[32], odakle[28], dokle[28];
    strcopy(model, gImenaVozila[GetVehicleModel(v) - 400]);
    odakle = CestaZona(x, y);
    if (GetVehicleVirtualWorld(v) && GarazaIzvana(VlasnikGarazaDb(v), x, y)) odakle = CestaZona(x, y);
    VlasnikParkingZona(v, dokle);
    MobitelPoziv(playerid, T(playerid, "Mehanicar"));
    SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_CHAT, "%s kaze (telefon): Dobar dan, odslepajte mi %s iz %s do parkinga u %s.", GetRPIme(playerid), model, 1, odakle, dokle);
    SetTimerEx("MehanicarOdvozOdgovor", MEHANICAR_ODGOVOR_MS, false, "i", playerid);
    for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerInVehicle(i, v)) RemovePlayerFromVehicle(i);
    gOdvoz[v] = true;
    SetVehicleVirtualWorld(v, MEHANICAR_ODVOZ_SVIJET);
    SetVehiclePos(v, 1477.0 + float(v % 10) * 6.0, 1350.0 + float((v / 10) % 10) * 8.0, 11.0);
    SetTimerEx("MehanicarOdvozGotov", MEHANICAR_ODVOZ_MS, false, "ii", v, VlasnikDb(v));
    return 1;
}

forward MehanicarOdvozOdgovor(playerid);
public MehanicarOdvozOdgovor(playerid)
{
    if (IsPlayerConnected(playerid)) SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): U redu, slep je krenuo, vozilo ce biti na parkingu za par minuta."));
}

forward MehanicarOdvozGotov(v, db);
public MehanicarOdvozGotov(v, db)
{
    if (!gOdvoz[v]) return;
    gOdvoz[v] = false;
    if (!GetVehicleModel(v) || VlasnikDb(v) != db) return;
    VlasnikNaParking(v);
    MobitelMehanicarSms(VlasnikVozila(v), "Vase vozilo %s je dostavljeno na parking.", gImenaVozila[GetVehicleModel(v) - 400]);
}

MehanicarPozovi(playerid)
{
    if (!gUlogiran[playerid]) return 1;
    new v = GetPlayerVehicleID(playerid), Float:zdravlje, Float:x, Float:y, Float:z, Float:d;
    if (!v) return MehanicarOdvozPoziv(playerid);
    GetVehicleHealth(v, zdravlje);
    if (VoziloPunSpremnik(v) && zdravlje >= MEHANICAR_STETA) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Vase vozilo ne treba mehanicara."));
    for (new s = 0; s < SLUCAJ_MAX; s++)
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && gSlucaj[s][slTip] == ORG_MEHANICAR && gSlucaj[s][slPacijent] == playerid)
            return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mehanicar je vec na putu."));
    GetVehiclePos(v, x, y, z);
    CestaNajbliza(x, y, z, d);
    if (GetPlayerInterior(playerid) || GetPlayerVirtualWorld(playerid) || d > TAKSI_MAX_OD_CESTE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mehanicar dolazi samo na ulice Los Santosa."));
    if ((!KamionBroj(ORG_MEHANICAR) && !SlucajImaIgraca(ORG_MEHANICAR, playerid)) || SlucajNovi(ORG_MEHANICAR, x, y, z, playerid) == -1) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Svi mehanicari su zauzeti, pokusajte kasnije."));
    gMehanicarVozilo[playerid] = v;
    MobitelPoziv(playerid, T(playerid, "Mehanicar"));
    SetTimerEx("TaksiSpustiMobitel", TAKSI_MOBITEL_MS, false, "i", playerid);
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_CHAT, "%s kaze (telefon): Dobar dan, trebao bih mehanicara. Nalazim se u %s.", GetRPIme(playerid), CestaZona(x, y), 1);
    SetTimerEx("MehanicarOdgovor", MEHANICAR_ODGOVOR_MS, false, "i", playerid);
    return 1;
}

forward MehanicarOdgovor(playerid);
public MehanicarOdgovor(playerid)
{
    if (IsPlayerConnected(playerid)) SendClientMessage(playerid, COLOR_CHAT, T(playerid, "Dispecer (telefon): U redu, mehanicar je na putu."));
}

static const gMehanicarUsluga[][] = { "Napuni gorivo", "Popravi motor", "Slepaj vozilo u radionicu", "Zamijeni gume" };
static const gMehanicarCijena[] = { MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_SLEPANJE, MEHANICAR_GUME };

static MehanicarSlucaj(playerid)
{
    new s = gMehanicar[playerid] - 1;
    if (s >= 0 && gSlucaj[s][slStanje] == SL_PRISTANAK && gSlucaj[s][slPacijent] == playerid) return s;
    gMehanicar[playerid] = 0;
    return -1;
}

static MehanicarPosao(playerid, i)
{
    new s = gMehanicar[playerid] - 1;
    return s >= 0 && gSlucaj[s][slIgrac] == INVALID_PLAYER_ID && i >= 2 ? i + 1 : i;
}

MehanicarBroj(playerid)
{
    new s = gMehanicar[playerid] - 1;
    return s >= 0 && gSlucaj[s][slIgrac] != INVALID_PLAYER_ID ? 4 : 3;
}

MehanicarStavka(playerid, i, &cijena, ime[], size)
{
    new p = MehanicarPosao(playerid, i);
    if (p < 0 || p >= sizeof gMehanicarCijena) p = 0;
    cijena = gMehanicarCijena[p];
    format(ime, size, T(playerid, gMehanicarUsluga[p]));
}

MehanicarOdustani(playerid)
{
    new s = MehanicarSlucaj(playerid);
    if (s == -1) return;
    gMehanicar[playerid] = 0;
    SlucajKraj(s, true);
}

static MehanicarPonudi(playerid, s)
{
    if (playerid == INVALID_PLAYER_ID || !IsPlayerConnected(playerid)) return;
    new body[256];
    gMehanicar[playerid] = s + 1;
    if (GetPlayerState(playerid) == PLAYER_STATE_DRIVER && GetPlayerVehicleID(playerid) == gMehanicarVozilo[playerid])
    {
        TuningMehanicar(playerid);
        return;
    }
    if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) format(body, sizeof body, T(playerid, "Napuni gorivo ($%d)\nPopravi motor ($%d)\nSlepaj vozilo u radionicu ($%d)\nZamijeni gume ($%d)"), MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_SLEPANJE, MEHANICAR_GUME);
    else format(body, sizeof body, T(playerid, "Napuni gorivo ($%d)\nPopravi motor ($%d)\nZamijeni gume ($%d)"), MEHANICAR_GORIVO, MEHANICAR_POPRAVAK, MEHANICAR_GUME);
    PrikaziDialog(playerid, DLG_MEHANICAR, DIALOG_STYLE_LIST, T(playerid, "Mehanicar"), body, T(playerid, "Odaberi"), T(playerid, "Gotovo"));
}

DLG:mehanicar(playerid, response, listitem)
{
    if (!response)
    {
        MehanicarOdustani(playerid);
        return 1;
    }
    return MehanicarOdaberi(playerid, listitem);
}

MehanicarOdaberi(playerid, listitem)
{
    new s = MehanicarSlucaj(playerid), v = gMehanicarVozilo[playerid];
    if (s == -1) return 1;
    if (!GetVehicleModel(v))
    {
        gMehanicar[playerid] = 0;
        SlucajKraj(s, true);
        return 1;
    }
    new igrac = gSlucaj[s][slIgrac];
    listitem = MehanicarPosao(playerid, listitem);
    if (listitem < 0 || listitem >= sizeof gMehanicarCijena) return 1;
    if (listitem == 3)
    {
        new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
        GetVehicleDamageStatus(v, panels, doors, lights, tires);
        if (!tires)
        {
            SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Gume su ispravne."));
            MehanicarPonudi(playerid, s);
            return 1;
        }
    }
    if (igrac != INVALID_PLAYER_ID ? !TransferNovac(playerid, igrac, gMehanicarCijena[listitem]) : !DajNovac(playerid, -gMehanicarCijena[listitem]))
    {
        SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
        MehanicarPonudi(playerid, s);
        return 1;
    }
    if (listitem == 2)
    {
        for (new i = 0, iZadnji = gIgracZadnji; i <= iZadnji; i++) if (IsPlayerInVehicle(i, v)) RemovePlayerFromVehicle(i);
        new kamion = GetPlayerVehicleID(igrac);
        if (kamion && GetVehicleModel(kamion) == MEHANICAR_MODEL) AttachTrailerToVehicle(v, kamion);
        SendClientMessage(igrac, COLOR_ORG, T(igrac, "Musterija je platila slepanje. Odslepajte vozilo do radionice."));
        gMehanicar[playerid] = 0;
        SlucajKraj(s, true);
        return 1;
    }
    MehanicarRadi(s, v, listitem);
    return 1;
}

static MehanicarHauba(v, bool:otvorena)
{
    new e, l, a, d, bn, bt, o;
    GetVehicleParamsEx(v, e, l, a, d, bn, bt, o);
    SetVehicleParamsEx(v, e, l, a, d, otvorena ? 1 : 0, bt, o);
}

static MehanicarIgracStigao(s)
{
    new i = gSlucaj[s][slIgrac], p = gSlucaj[s][slPacijent], Float:x, Float:y, Float:z;
    if (i == INVALID_PLAYER_ID || p == INVALID_PLAYER_ID || GetPlayerState(i) != PLAYER_STATE_ONFOOT || !GetVehiclePos(gMehanicarVozilo[p], x, y, z) || !IsPlayerInRangeOfPoint(i, MEHANICAR_IGRAC_DOMET, x, y, z)) return;
    DisablePlayerCheckpoint(i);
    gSlucaj[s][slStanje] = SL_PRISTANAK;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    MehanicarPonudi(p, s);
}

static MehanicarRadi(s, v, posao)
{
    new igrac = gSlucaj[s][slIgrac];
    if (igrac != INVALID_PLAYER_ID)
    {
        if (posao == 1) MehanicarHauba(v, true);
        if (posao == 1) ApplyAnimation(igrac, "CAR", "Fixn_Car_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
        else if (posao == 3) ApplyAnimation(igrac, "BOMBER", "BOM_Plant_Loop", 4.1, true, false, false, false, 0, SYNC_ALL);
        else ApplyAnimation(igrac, "BAR", "Barserve_bottle", 4.1, true, false, false, false, 0, SYNC_ALL);
        gSlucaj[s][slStanje] = SL_RADI;
        gSlucaj[s][slPosao] = posao;
        gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
        SetTimerEx("MehanicarGotov", MEHANICAR_RAD_MS, false, "i", s);
        return;
    }
    new Float:x, Float:y, Float:z, Float:a, Float:sx, Float:sy, Float:sz;
    GetVehiclePos(v, x, y, z);
    GetVehicleZAngle(v, a);
    GetVehicleModelInfo(GetVehicleModel(v), VEHICLE_MODEL_INFO_SIZE, sx, sy, sz);
    new Float:fx = -floatsin(a, degrees), Float:fy = floatcos(a, degrees), Float:px, Float:py;
    if (posao == 1)
    {
        px = x + fx * (sy / 2.0 + 0.7);
        py = y + fy * (sy / 2.0 + 0.7);
        MehanicarHauba(v, true);
    }
    else
    {
        px = x - fx * (sy / 2.0 - 0.6) + fy * (sx / 2.0 + 0.6);
        py = y - fy * (sy / 2.0 - 0.6) - fx * (sx / 2.0 + 0.6);
    }
    gSlucaj[s][slStanje] = SL_RADI;
    gSlucaj[s][slPosao] = posao;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    Lik_IdiDo(gSlucaj[s][slNpc], px, py, HodTlo(px, py, z), LIK_HOD);
}

static MehanicarAnim(s)
{
    new lik = gSlucaj[s][slNpc], p = gSlucaj[s][slPacijent], Float:x, Float:y, Float:z, Float:lx, Float:ly, Float:lz;
    if (p != INVALID_PLAYER_ID && GetVehiclePos(gMehanicarVozilo[p], x, y, z))
    {
        Lik_Pozicija(lik, lx, ly, lz);
        Lik_Kut(lik, atan2(y - ly, x - lx) - 90.0);
    }
    if (gSlucaj[s][slPosao] == 1)
    {
        new Float:kut;
        Lik_DajKut(lik, kut);
        Lik_Kut(lik, kut + 180.0);
        Lik_Animacija(lik, "CAR", "Fixn_Car_Loop", 4.1, 1, 0, 0, 0, 0);
    }
    else if (gSlucaj[s][slPosao] == 3) Lik_Animacija(lik, "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 0, 0);
    else Lik_Animacija(lik, "BAR", "Barserve_bottle", 4.1, 1, 0, 0, 0, 0);
    SetTimerEx("MehanicarGotov", MEHANICAR_RAD_MS, false, "i", s);
}

forward MehanicarGotov(s);
public MehanicarGotov(s)
{
    if (gSlucaj[s][slStanje] != SL_RADI) return;
    new p = gSlucaj[s][slPacijent];
    if (gSlucaj[s][slNpc] != INVALID_LIK) Lik_OcistiAnimaciju(gSlucaj[s][slNpc]);
    else if (gSlucaj[s][slIgrac] != INVALID_PLAYER_ID) ClearAnimations(gSlucaj[s][slIgrac]);
    gSlucaj[s][slStanje] = SL_PRISTANAK;
    gSlucaj[s][slRok] = MEHANICAR_CEKA_S;
    if (p == INVALID_PLAYER_ID) return;
    new v = gMehanicarVozilo[p];
    if (GetVehicleModel(v))
    {
        if (gSlucaj[s][slPosao] == 1)
        {
            SetVehicleHealth(v, 1000.0);
            MehanicarHauba(v, false);
        }
        else if (gSlucaj[s][slPosao] == 3)
        {
            new VEHICLE_PANEL_STATUS:panels, VEHICLE_DOOR_STATUS:doors, VEHICLE_LIGHT_STATUS:lights, VEHICLE_TIRE_STATUS:tires;
            GetVehicleDamageStatus(v, panels, doors, lights, tires);
            UpdateVehicleDamageStatus(v, panels, doors, lights, VEHICLE_TIRE_STATUS:0);
        }
        else VoziloNapuni(v);
    }
    MehanicarPonudi(p, s);
}

CMD:radionica(playerid, params[])
{
    if (!gAdmin[playerid]) return 0;
    PrebaciIgraca(playerid, 0, 0, RADIONICA_X, RADIONICA_Y, RADIONICA_Z);
    return 1;
}

DLG:slucaj(playerid, response)
{
    new s = gSlucajPoziv[playerid] - 1;
    gSlucajPoziv[playerid] = 0;
    if (!response) return 1;
    if (s < 0 || gSlucaj[s][slStanje] != SL_POZIV) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Netko je vec prihvatio poziv."));
    SlucajOcistiPozive(s);
    gSlucaj[s][slStanje] = SL_IGRAC;
    gSlucaj[s][slIgrac] = playerid;
    SetPlayerCheckpoint(playerid, gSlucaj[s][slX], gSlucaj[s][slY], gSlucaj[s][slZ], 5.0);
    if (gSlucaj[s][slTip] == ORG_VATROGASCI) return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Pozar je oznacen na karti. Gasi ga vatrogasnim kamionom ili aparatom (/aparat)."));
    if (gSlucaj[s][slTip] == ORG_MEHANICAR) return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Vozilo u kvaru je oznaceno na karti. Kad stignete, vlasnik bira uslugu."));
    return SendClientMessage(playerid, COLOR_ORG, T(playerid, "Ranjenik je oznacen na karti. Ozivi ga s /heal %d (treba /duty)."), gSlucaj[s][slPacijent]);
}

IsRanjen(playerid)
{
    return gRanjen[playerid];
}

static SlucajPacijenta(playerid)
{
    for (new s = 0; s < SLUCAJ_MAX; s++)
        if (gSlucaj[s][slStanje] && gSlucaj[s][slStanje] < SL_ODLAZI && gSlucaj[s][slTip] == ORG_MEDICINSKA && gSlucaj[s][slPacijent] == playerid) return s;
    return -1;
}

RanjenPadni(playerid)
{
    if (!gUlogiran[playerid] || ZatvorProvjera(playerid)) return 0;
    if (gRanjenBolnica[playerid]) return 1;
    if (gRanjen[playerid] == RN_DOLJE)
    {
        gRanjen[playerid] = RN_CEKA;
        return 1;
    }
    GetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    GetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    gRanjenInt[playerid] = GetPlayerInterior(playerid);
    gRanjenSvijet[playerid] = GetPlayerVirtualWorld(playerid);
    gRanjen[playerid] = RN_CEKA;
    SpremiSvoOruzje(playerid);
    return 1;
}

RanjenSpawn(playerid)
{
    if (gRanjen[playerid] != RN_CEKA) return;
    new bool:prvi = SlucajPacijenta(playerid) == -1;
    gRanjen[playerid] = RN_DOLJE;
    gRanjenOd[playerid] = GetTickCount();
    gRanjenPogoci[playerid] = 0;
    SetPlayerInterior(playerid, gRanjenInt[playerid]);
    SetPlayerVirtualWorld(playerid, gRanjenSvijet[playerid]);
    new Float:hx, Float:hy, Float:hz;
    if (!gRanjenInt[playerid] && !gRanjenSvijet[playerid] && Kolizija_Zraka(gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2] + 1.0, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2] - 300.0, hx, hy, hz))
        gRanjenPos[playerid][2] = hz + 1.0;
    SetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    SetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0, SYNC_ALL);
    gRanjenAnimId[playerid] = -2;
    SetTimerEx("RanjenAnim", 200, false, "ii", playerid, gAccountId[playerid]);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Ranjeni ste. Za 10 s mozete /odustani ili cekajte hitnu."));
    if (prvi && !gRanjenInt[playerid] && !gRanjenSvijet[playerid]) SlucajNovi(ORG_MEDICINSKA, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2], playerid);
}

RanjenProvjeri(playerid, Float:zivot)
{
    if (zivot > RANJEN_PRAG) return false;
    if (gRanjenBolnica[playerid])
    {
        SetPlayerHealth(playerid, FLOAT_INFINITY);
        return true;
    }
    RanjenOdmah(playerid);
    return true;
}

RanjenOdmah(playerid)
{
    TaksiPlayerLeft(playerid);
    KurvaDisconnect(playerid);
    MaricaReset(playerid);
    PolicijaDisconnect(playerid, false);
    RanjenPadni(playerid);
    TeritorijDeath(playerid);
    if (gRanjen[playerid] != RN_CEKA || !IsPlayerInAnyVehicle(playerid))
    {
        RanjenSpawn(playerid);
        return;
    }
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    RemovePlayerFromVehicle(playerid);
    SetTimerEx("RanjenIzVozila", RANJEN_IZLAZ_MS, false, "ii", playerid, gAccountId[playerid]);
}

forward RanjenIzVozila(playerid, racun);
public RanjenIzVozila(playerid, racun)
{
    if (gAccountId[playerid] != racun || gRanjen[playerid] != RN_CEKA) return;
    GetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
    GetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    RanjenSpawn(playerid);
}

forward RanjenAnim(playerid, racun);
public RanjenAnim(playerid, racun)
{
    if (gAccountId[playerid] != racun || gRanjen[playerid] != RN_DOLJE) return;
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0);
    gRanjenAnimId[playerid] = -1;
    gRanjenPopravak[playerid] = GetTickCount();
}

RanjenUpdate(playerid)
{
    if (gRanjen[playerid] != RN_DOLJE || gRanjenAnimId[playerid] == -2) return;
    new idx = GetPlayerAnimationIndex(playerid);
    if (idx != gRanjenAnimId[playerid])
    {
        new lib[32], ime[32];
        GetAnimationName(idx, lib, sizeof lib, ime, sizeof ime);
        if (!strcmp(ime, "crckidle1", true)) gRanjenAnimId[playerid] = idx;
    }
    if (idx == gRanjenAnimId[playerid] && GetPlayerDistanceFromPoint(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]) <= RANJEN_POMAK) return;
    new sad = GetTickCount();
    if (sad - gRanjenPopravak[playerid] < RANJEN_POPRAVAK_MS) return;
    gRanjenPopravak[playerid] = sad;
    if (GetPlayerDistanceFromPoint(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]) > RANJEN_POMAK)
    {
        SetPlayerPos(playerid, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]);
        SetPlayerFacingAngle(playerid, gRanjenPos[playerid][3]);
    }
    TogglePlayerControllable(playerid, false);
    ApplyAnimation(playerid, "CRACK", "crckidle1", 4.1, true, false, false, true, 0);
}

static Float:gRanjenRana[MAX_PLAYERS][7];
static const Float:gRanjenTezina[7] = { 2.0, 1.5, 1.0, 1.0, 1.0, 1.0, 4.0 };

RanjenRana(playerid, bodypart, Float:steta)
{
    if (playerid >= 0 && playerid < MAX_PLAYERS && bodypart >= 3 && bodypart <= 9 && steta > 0.0) gRanjenRana[playerid][bodypart - 3] += steta;
}

static RanjenOcistiRane(playerid)
{
    for (new i = 0; i < sizeof gRanjenRana[]; i++) gRanjenRana[playerid][i] = 0.0;
}

static RanjenCijena(playerid)
{
    new Float:d = 1.0e9, Float:c = float(RANJEN_OSNOVA);
    for (new k = 0; k < KAMION_MAX; k++)
    {
        if (!gKamion[k][kIma] || gKamion[k][kTip] != ORG_MEDICINSKA) continue;
        new Float:u = VectorSize(gKamion[k][kX] - gRanjenPos[playerid][0], gKamion[k][kY] - gRanjenPos[playerid][1], 0.0);
        if (u < d) d = u;
    }
    if (d == 1.0e9) for (new b = 0; b < sizeof gBolnice; b++)
    {
        new Float:u = VectorSize(gBolnice[b][0] - gRanjenPos[playerid][0], gBolnice[b][1] - gRanjenPos[playerid][1], 0.0);
        if (u < d) d = u;
    }
    c += d / 1000.0 * float(RANJEN_PO_KM);
    for (new i = 0; i < sizeof gRanjenRana[]; i++) c += gRanjenRana[playerid][i] * gRanjenTezina[i] * RANJEN_PO_STETI;
    new n = floatround(c);
    return gNovac[playerid] < n ? gNovac[playerid] : n;
}

RanjenPonudi(playerid, medic)
{
    new info[128];
    gRanjenPonuda[playerid] = medic == RANJEN_NPC ? RANJEN_NPC : medic + 1;
    format(info, sizeof info, T(playerid, "Bolnicar vam zeli pomoci. Cijena: $%d."), RanjenCijena(playerid));
    UpitDaNe(playerid, UPIT_RANJEN, 0, 0, T(playerid, "Hitna pomoc"), info);
}

RanjenUpit(playerid, bool:response)
{
    new n = gRanjenPonuda[playerid], s = SlucajPacijenta(playerid);
    gRanjenPonuda[playerid] = 0;
    if (!n || gRanjen[playerid] != RN_DOLJE) return 1;
    new bool:npcCeka = s != -1 && gSlucaj[s][slStanje] == SL_PRISTANAK;
    if (n > 0)
    {
        new medic = n - 1, bool:tu = IsPlayerConnected(medic) && gUlogiran[medic];
        if (response && tu && GetPlayerDistanceFromPoint(medic, gRanjenPos[playerid][0], gRanjenPos[playerid][1], gRanjenPos[playerid][2]) <= RANJEN_MEDIK_DOMET)
        {
            PosaljiBlizu(medic, CHAT_RANGE, COLOR_ME, "* %s ozivljava %s.", GetRPIme(medic), GetRPIme(playerid), 1);
            RanjenZavrsi(playerid, true, medic);
            return 1;
        }
        if (!response && tu) SendClientMessage(medic, COLOR_ERROR, T(medic, "Ranjenik je odbio pomoc."));
        if (npcCeka) RanjenPonudi(playerid, RANJEN_NPC);
        return 1;
    }
    if (!npcCeka) return 1;
    if (!response)
    {
        SlucajZatvori(s);
        return 1;
    }
    gSlucaj[s][slStanje] = SL_GASI;
    Lik_Animacija(gSlucaj[s][slNpc], "BOMBER", "BOM_Plant_Loop", 4.1, 1, 0, 0, 0, 0);
    return 1;
}

RanjenAdmin(playerid)
{
    if (gRanjen[playerid] == RN_NE) return;
    new s = SlucajPacijenta(playerid);
    if (s != -1) SlucajKraj(s, true);
    gRanjen[playerid] = RN_NE;
    gRanjenPonuda[playerid] = 0;
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
}

RanjenPrimijeni(playerid, bool:spasen)
{
    UpitMakni(playerid, UPIT_RANJEN);
    gRanjen[playerid] = RN_NE;
    gRanjenPonuda[playerid] = 0;
    SetPlayerHealth(playerid, 100.0);
    TogglePlayerControllable(playerid, true);
    ClearAnimations(playerid);
    if (spasen)
    {
        new cijena = RanjenCijena(playerid);
        RanjenOcistiRane(playerid);
        if (cijena) DajNovac(playerid, -cijena);
        SendClientMessage(playerid, COLOR_INFO, T(playerid, "Spaseni ste. Placate $%d."), cijena);
        return;
    }
    RanjenOcistiRane(playerid);
    new DBResult:res = DB_ExecuteQuery(gDB, "SELECT oruzje_dozvola FROM accounts WHERE id = %d", gAccountId[playerid]), bool:dozvola = DB_GetRowCount(res) && DB_GetFieldInt(res, 0);
    DB_FreeResultSet(res);
    SpremiSvoOruzje(playerid);
    for (new i = 0; i < BROJ_ITEMA; i++)
    {
        if (!gInv[playerid][i]) continue;
        new bool:droga = i == ITEM_MARIHUANA || i == ITEM_KOKAIN || i == ITEM_SJEME || i == ITEM_METH || i == ITEM_KOKA || i == ITEM_KEMIKALIJE, bool:oruzje = gItems[i][ITEM_WEAPON] > 0 || (ITEM_MUN_PISTOLJ <= i <= ITEM_MUN_PUSKA);
        if (droga || (oruzje && !dozvola)) GiveItem(playerid, i, -gInv[playerid][i]);
    }
    SendClientMessage(playerid, COLOR_INFO, dozvola ? T(playerid, "Izgubili ste drogu.") : T(playerid, "Izgubili ste drogu i oruzje (nemate dozvolu za oruzje)."));
    gRanjenBolnica[playerid] = true;
    SetPlayerHealth(playerid, FLOAT_INFINITY);
    TogglePlayerControllable(playerid, false);
    Fade(playerid, 0);
    SetTimerEx("RanjenUmri", FADE_SLOJEVA * FADE_KORAK_MS + BOLNICA_CRNO_MS, false, "ii", playerid, gAccountId[playerid]);
}

forward RanjenUmri(playerid, racun);
public RanjenUmri(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    SpawnPlayer(playerid);
}

RanjenBolnica(playerid)
{
    if (!gRanjenBolnica[playerid]) return;
    gRanjenBolnica[playerid] = false;
    SetPlayerHealth(playerid, 100.0);
    new b = VectorSize(gRanjenPos[playerid][0] - gBolnice[0][0], gRanjenPos[playerid][1] - gBolnice[0][1], 0.0) <= VectorSize(gRanjenPos[playerid][0] - gBolnice[1][0], gRanjenPos[playerid][1] - gBolnice[1][1], 0.0) ? 0 : 1;
    PrebaciIgraca(playerid, 0, 0, gBolnice[b][0], gBolnice[b][1], gBolnice[b][2], gBolnice[b][3]);
    TogglePlayerControllable(playerid, true);
    Fade(playerid, FADE_SLOJEVA + 1);
    SendClientMessage(playerid, COLOR_INFO, T(playerid, "Probudili ste se u bolnici."));
    if (!gPremium[playerid]) return;
    new body[128];
    format(body, sizeof body, T(playerid, "Ostani u bolnici\nVrati me na moj spawn ($%d)"), BOLNICA_SPAWN_CIJENA);
    PrikaziDialog(playerid, DLG_BOLNICA, DIALOG_STYLE_LIST, T(playerid, "Bolnica"), body, T(playerid, "Odaberi"), T(playerid, "Zatvori"));
}

DLG:bolnica(playerid, response, listitem)
{
    if (!response || listitem != 1 || !gPremium[playerid] || IsRanjen(playerid)) return 1;
    if (!DajNovac(playerid, -BOLNICA_SPAWN_CIJENA)) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Nemate dovoljno novca."));
    TogglePlayerControllable(playerid, false);
    Fade(playerid, 0);
    SetTimerEx("BolnicaPrebaci", FADE_SLOJEVA * FADE_KORAK_MS + BOLNICA_CRNO_MS, false, "ii", playerid, gAccountId[playerid]);
    return 1;
}

forward BolnicaPrebaci(playerid, racun);
public BolnicaPrebaci(playerid, racun)
{
    if (!IsPlayerConnected(playerid) || gAccountId[playerid] != racun) return;
    PrebaciIgraca(playerid, 0, 0, SPAWN_X, SPAWN_Y, SPAWN_Z, SPAWN_A);
    KucaSpawn(playerid);
    TogglePlayerControllable(playerid, true);
    Fade(playerid, FADE_SLOJEVA + 1);
}

RanjenZavrsi(playerid, bool:spasen, ko = INVALID_PLAYER_ID)
{
    new s = SlucajPacijenta(playerid);
    if (s != -1) SlucajKraj(s, spasen, ko);
    else RanjenPrimijeni(playerid, spasen);
}

RanjenPogodak(playerid, BULLET_HIT_TYPE:hittype, hitid)
{
    if (hittype != BULLET_HIT_TYPE_PLAYER || playerid == hitid || gRanjen[hitid] != RN_DOLJE || ++gRanjenPogoci[hitid] < RANJEN_POGODAKA) return;
    PosaljiBlizu(playerid, CHAT_RANGE, COLOR_ME, "* %s ubija %s.", GetRPIme(playerid), GetRPIme(hitid), 1);
    RanjenZavrsi(hitid, false);
}

CMD:odustani(playerid, params[])
{
    if (gRanjen[playerid] != RN_DOLJE) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Niste ranjeni."));
    new ms = RANJEN_CEKANJE_MS - (GetTickCount() - gRanjenOd[playerid]);
    if (ms > 0) return SendClientMessage(playerid, COLOR_ERROR, T(playerid, "Mozete odustati za %d s."), ms / 1000 + 1);
    RanjenZavrsi(playerid, false);
    return 1;
}

CMD:giveup(playerid, params[]) return cmd_odustani(playerid, params);
